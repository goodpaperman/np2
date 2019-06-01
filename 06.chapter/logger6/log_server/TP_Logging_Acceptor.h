#pragma once

#include "Logging_Acceptor.h" 

class TP_Logging_Acceptor : public Logging_Acceptor 
{
public:
  TP_Logging_Acceptor(ACE_Reactor *r = ACE_Reactor::instance());
  ~TP_Logging_Acceptor(void);
  
  virtual int handle_input(ACE_HANDLE = ACE_INVALID_HANDLE); 
};
