#include "stdafx.h"
#include "AC_Input_Handler.h"
#include "Logging_Handler.h" 

AC_Input_Handler::AC_Input_Handler(AC_Output_Handler *handler)
: output_handler_(handler)
{
}

AC_Input_Handler::~AC_Input_Handler(void)
{
}

int AC_Input_Handler::open(void *)
{
  ACE_HANDLE handle = peer().get_handle(); 
  if(reactor()->register_handler(handle, this, ACE_Event_Handler::READ_MASK) == -1)
    return -1; 

  connected_clients_.set_bit(handle); 
  return 0; 
}

int AC_Input_Handler::handle_input(ACE_HANDLE handle)
{
  ACE_Message_Block *mblk = 0; 
  Logging_Handler logging_handler(handle); 
  if(logging_handler.recv_log_record(mblk) != -1)
  {
    if(output_handler_->put(mblk->cont()) != -1)
    {
      //printf("recv a message\n"); 
      mblk->cont(0); 
      mblk->release(); 
      return 0; 
    }
    else 
      mblk->release(); 
  }

  return -1; 
}

int AC_Input_Handler::handle_close(ACE_HANDLE h, ACE_Reactor_Mask mask)
{
  connected_clients_.clr_bit(h); 
  return ACE_OS::closesocket(h); 
}

int AC_Input_Handler::close(u_int)
{
  ACE_Message_Block *shutdown_message = 0; 
  ACE_NEW_RETURN(shutdown_message, ACE_Message_Block(0, ACE_Message_Block::MB_STOP), -1); 
  output_handler_->put(shutdown_message); 

  reactor()->remove_handler(connected_clients_, ACE_Event_Handler::READ_MASK); 
  return output_handler_->wait(); 
}