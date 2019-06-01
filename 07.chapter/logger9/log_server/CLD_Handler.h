#pragma once

#include "ace\event_handler.h"
//#include "../common/Log_Record.h" 

class CLD_Connector; 
class CLD_Handler : public ACE_Event_Handler
{
public:
  CLD_Handler(void);
  ~CLD_Handler(void);

  enum { 
    QUEUE_MAX = 1024 * ACE_IOV_MAX, 
    FLUSH_TIMEOUT = 120, 
  }; 

  virtual int open(CLD_Connector *c); 
  virtual int close(); 
  virtual int handle_input(ACE_HANDLE handle); 
  virtual int handle_close(ACE_HANDLE handle = ACE_INVALID_HANDLE, ACE_Reactor_Mask mask = 0); 
  virtual ACE_SOCK_Stream& peer() { return peer_; } 

protected:
  virtual ACE_THR_FUNC_RETURN forward(); 
  virtual int send(ACE_Message_Block *chunk[], size_t &count); 
  static ACE_THR_FUNC_RETURN run_svc(void *arg); 

  ACE_Message_Queue<ACE_MT_SYNCH> msg_queue_; 
  ACE_Thread_Manager thr_mgr_; 
  CLD_Connector *connector_; 
  ACE_SOCK_Stream peer_; 
};
