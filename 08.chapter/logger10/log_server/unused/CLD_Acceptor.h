#pragma once

#include "ace\event_handler.h"
#include "CLD_Handler.h" 

class CLD_Acceptor : public ACE_Event_Handler
{
public:
  CLD_Acceptor(void);
  ~CLD_Acceptor(void);

  virtual int open(CLD_Handler *handler, const ACE_INET_Addr &addr, ACE_Reactor *r = ACE_Reactor::instance()); 
  virtual int handle_input(ACE_HANDLE handle); 
  virtual int handle_close(ACE_HANDLE handle = ACE_INVALID_HANDLE, ACE_Reactor_Mask mask = 0); 
  virtual ACE_HANDLE get_handle() const; 

protected:
  ACE_SOCK_Acceptor acceptor_; 
  CLD_Handler *handler_; 
};
