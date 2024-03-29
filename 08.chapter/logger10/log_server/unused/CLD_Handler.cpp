#include "stdafx.h"
#include "CLD_Handler.h"
#include "CLD_Connector.h" 
#include "Logging_Handler.h" 

CLD_Handler::CLD_Handler(void)
{
}

CLD_Handler::~CLD_Handler(void)
{
}

int CLD_Handler::open(CLD_Connector *connector)
{
  connector_ = connector; 
  int bufsize = ACE_DEFAULT_MAX_SOCKET_BUFSIZ; 
  peer().set_option(SOL_SOCKET, SO_SNDBUF, &bufsize, sizeof(bufsize)); 
  msg_queue_.high_water_mark(CLD_Handler::QUEUE_MAX); 
  return thr_mgr_.spawn(&CLD_Handler::run_svc, this, THR_SCOPE_SYSTEM); 
}

int CLD_Handler::close()
{
  ACE_Message_Block *shutdown_message = 0; 
  ACE_NEW_RETURN(shutdown_message, ACE_Message_Block(0, ACE_Message_Block::MB_STOP), -1); 
  msg_queue_.enqueue_tail(shutdown_message); 
  return thr_mgr_.wait(); 
}

int CLD_Handler::handle_input(ACE_HANDLE handle)
{
  ACE_Message_Block *mblk = 0; 
  Logging_Handler logging_handler(handle); 
  if(logging_handler.recv_log_record(mblk) != -1)
  {
    if(msg_queue_.enqueue_tail(mblk->cont()) != -1)
    {
      mblk->cont(0); 
      mblk->release(); 
      return 0; 
    }
    else 
      mblk->release(); 
  }

  return -1; 
}

int CLD_Handler::handle_close(ACE_HANDLE handle, ACE_Reactor_Mask mask)
{
  return ACE_OS::closesocket(handle); 
}

ACE_THR_FUNC_RETURN CLD_Handler::run_svc(void *arg)
{
  CLD_Handler *handler = ACE_static_cast(CLD_Handler*, arg); 
  return handler->forward(); 
}

ACE_THR_FUNC_RETURN CLD_Handler::forward()
{
  ACE_Message_Block *chunk[ACE_IOV_MAX] = { 0 }; 
  size_t message_index = 0; 
  ACE_Time_Value time_of_last_send(ACE_OS::gettimeofday()); 
  ACE_Time_Value timeout; 
  ACE_Sig_Action no_sigpipe((ACE_SignalHandler)SIG_IGN); 
  ACE_Sig_Action original_action; 
  no_sigpipe.register_action(SIGPIPE, &original_action); 

  for(;;)
  {
    if(message_index == 0)
    {
      timeout = ACE_OS::gettimeofday(); 
      timeout += FLUSH_TIMEOUT; 
    }

    ACE_Message_Block *mblk = 0; 
    if(msg_queue_.dequeue_head(mblk, &timeout) == -1)
    {
      if(errno != EWOULDBLOCK)
        break; 
    }
    else
    {
      if(mblk->size() == 0 && 
        mblk->msg_type() == ACE_Message_Block::MB_STOP)
      {
        mblk->release(); 
        break; 
      }

      chunk[message_index] = mblk; 
      ++ message_index; 
    }

    if(message_index >= ACE_IOV_MAX ||
      (ACE_OS::gettimeofday() - time_of_last_send >= FLUSH_TIMEOUT))
    {
      if(send(chunk, message_index) == -1)
        break; 

      time_of_last_send = ACE_OS::gettimeofday(); 
    }
  }

  if(message_index > 0)
    send(chunk, message_index); 

  msg_queue_.close(); 
  no_sigpipe.restore_action(SIGPIPE, original_action); 
  return 0; 
}

int CLD_Handler::send(ACE_Message_Block *chunk[], size_t &count)
{
  iovec iov[ACE_IOV_MAX] = { 0 }; 
  size_t iov_size = 0; 
  int result = 0; 

  for(iov_size = 0; iov_size < count; ++ iov_size)
  {
    iov[iov_size].iov_base = chunk[iov_size]->rd_ptr(); 
    iov[iov_size].iov_len = chunk[iov_size]->length(); 
  }

  while(peer().sendv_n(iov, iov_size) == -1)
  {
    if(connector_->reconnect() == -1)
    {
      result = -1; 
      break; 
    }
  }

  while(iov_size > 0)
  {
    chunk[--iov_size]->release(); 
    chunk[iov_size] = 0; 
  }

  count = iov_size; 
  return result; 
}