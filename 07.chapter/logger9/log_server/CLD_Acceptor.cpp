#include "stdafx.h"
#include "CLD_Acceptor.h"

CLD_Acceptor::CLD_Acceptor(void)
{
}

CLD_Acceptor::~CLD_Acceptor(void)
{
}

int CLD_Acceptor::open(CLD_Handler *handler, const ACE_INET_Addr &local_addr, ACE_Reactor *r)
{
  reactor(r); 
  handler_ = handler; 
  if(acceptor_.open(local_addr) == -1 || 
    reactor()->register_handler(this, ACE_Event_Handler::ACCEPT_MASK) == -1)
    return -1; 

  return 0; 
}

int CLD_Acceptor::handle_input(ACE_HANDLE handle)
{
  ACE_SOCK_Stream peer_stream; 
  if(acceptor_.accept(peer_stream) == -1)
    return -1; 
  else if(reactor()->register_handler(peer_stream.get_handle(), handler_, ACE_Event_Handler::READ_MASK) == -1)
    return -1; 
  else 
    return 0; 
}

int CLD_Acceptor::handle_close(ACE_HANDLE handle, ACE_Reactor_Mask mask)
{
  acceptor_.close(); 
  handler_->close(); 
  return 0; 
}

ACE_HANDLE CLD_Acceptor::get_handle() const
{
  return acceptor_.get_handle(); 
}
