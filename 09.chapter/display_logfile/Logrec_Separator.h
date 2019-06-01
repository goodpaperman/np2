#pragma once
#include "ace/task_t.h"
#include "ace/Lock_Adapter_T.h" 
#include "Logrec_Module.h" 

class Logrec_Separator : public ACE_Task<ACE_MT_SYNCH>
{
public:
  Logrec_Separator(void);
  virtual ~Logrec_Separator(void);
  virtual int put(ACE_Message_Block *mblk, ACE_Time_Value * = 0); 

private:
  ACE_Lock_Adapter<ACE_Thread_Mutex> lock_strategy_; 
};


LOGREC_MODULE(Logrec_Separator); 