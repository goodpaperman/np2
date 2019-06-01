#pragma once
#include "ace/task_t.h"
#include "Logrec_Module.h" 

class Logrec_Writer : public ACE_Task<ACE_MT_SYNCH>
{
public:
  Logrec_Writer(void);
  virtual ~Logrec_Writer(void);
  virtual int open(void *); 
  virtual int put(ACE_Message_Block *, ACE_Time_Value * = 0); 
  virtual int svc(); 
};

LOGREC_MODULE(Logrec_Writer); 
