#include "stdafx.h"
#include "TP_Logging_Acceptor.h"
#include "TP_Logging_Handler.h" 

TP_Logging_Acceptor::TP_Logging_Acceptor(ACE_Reactor *r /* =ACE_Reactor::instance() */)
: Logging_Acceptor(r) 
{
}

TP_Logging_Acceptor::~TP_Logging_Acceptor(void)
{
}

int TP_Logging_Acceptor::handle_input(ACE_HANDLE)
{
  TP_Logging_Handler *peer_handler = 0; 
  ACE_NEW_RETURN(peer_handler, TP_Logging_Handler(reactor()), -1); 

  if(acceptor_.accept(peer_handler->peer()) == -1)
  {
    delete peer_handler; 
    return -1; 
  }
  else if(peer_handler->open() == -1)
    peer_handler->handle_close(ACE_INVALID_HANDLE, 0); 

  return 0; 
}
