#pragma once

#include "ace/Task_T.h" 
#include "ace/singleton.h" 

class TP_Logging_Task : public ACE_Task<ACE_MT_SYNCH>
{
public:
  enum { MAX_THREADS = 4 }; 
  TP_Logging_Task(void);
  ~TP_Logging_Task(void);

  virtual int open(void * = 0); 
  virtual int put(ACE_Message_Block *, ACE_Time_Value * = 0); 
  virtual int svc(); 
};

typedef ACE_Unmanaged_Singleton<TP_Logging_Task, ACE_Null_Mutex> TP_LOGGING_TASK; 
//typedef ACE_Singleton<TP_Logging_Task, ACE_Null_Mutex> TP_LOGGING_TASK; 
