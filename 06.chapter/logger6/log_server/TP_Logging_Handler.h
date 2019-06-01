#pragma once
#include "Logging_Event_Handler.h"
#include "TP_Logging_Acceptor.h" 


class TP_Logging_Handler : public Logging_Event_Handler
{
  friend class TP_Logging_Acceptor; 
public:
  TP_Logging_Handler(ACE_Reactor *reactor);
  virtual int handle_input(ACE_HANDLE); 
  virtual int handle_close(ACE_HANDLE, ACE_Reactor_Mask); 

protected:
  virtual ~TP_Logging_Handler(void);

private:
  int queued_count_; 
  int deferred_close_; 
  ACE_Thread_Mutex lock_; 
};
