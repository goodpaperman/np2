#include "stdafx.h"
#include "AC_Output_Handler.h"
#include "AC_CLD_Connector.h" 
//#include "ace/os_signal.h"

AC_Output_Handler::AC_Output_Handler(void)
: connector_(0) 
{
}

AC_Output_Handler::~AC_Output_Handler(void)
{
}

int AC_Output_Handler::open(void *connector)
{
  connector_ = ACE_static_cast(AC_CLD_Connector *, connector); 
  int bufsize = ACE_DEFAULT_MAX_SOCKET_BUFSIZ; 
  peer().set_option(SOL_SOCKET, SO_SNDBUF, &bufsize, sizeof bufsize); 
  if(reactor()->register_handler(this, ACE_Event_Handler::READ_MASK) == -1)
    return -1; 

  OutputDebugString("before output handler activated"); 
  if(msg_queue()->activate() == ACE_Message_Queue_Base::ACTIVATED)
  {
    OutputDebugString("after output handler activated"); 
    msg_queue()->high_water_mark(QUEUE_MAX); 
    return activate(THR_SCOPE_SYSTEM); 
  }
  else 
    return 0; 
}

int AC_Output_Handler::put(ACE_Message_Block *mblk, ACE_Time_Value *timeout)
{
  int result; 
  while((result = putq(mblk, timeout)) == -1)
    if(msg_queue()->state() != ACE_Message_Queue_Base::PULSED)
      break; 

  return result; 
}

int AC_Output_Handler::handle_input(ACE_HANDLE handle)
{
  peer().close(); 
  reactor()->remove_handler(handle, ACE_Event_Handler::READ_MASK | ACE_Event_Handler::DONT_CALL); 
  msg_queue()->pulse(); 
  return 0; 
}

int AC_Output_Handler::svc()
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
    if(getq(mblk, &timeout) == -1)
    {
      if(errno == ESHUTDOWN)
      {
        if(connector_->reconnect() == -1)
          break; 
        else
          continue; 
      }
      else if(message_index == 0)
        continue; 
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

  no_sigpipe.restore_action(SIGPIPE, original_action); 
  return 0; 
}

int AC_Output_Handler::send(ACE_Message_Block *chunk[], size_t &count)
{
  iovec iov[ACE_IOV_MAX] = { 0 }; 
  size_t iov_size = 0; 
  int result = 0; 

  printf("send %d messages\n", count); 
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