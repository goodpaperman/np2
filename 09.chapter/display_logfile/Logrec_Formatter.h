#pragma once
#include "ace/task_t.h"
#include "Logrec_Module.h" 

class Logrec_Formatter : public ACE_Task<ACE_MT_SYNCH>
{
public:
  Logrec_Formatter(void);
  virtual ~Logrec_Formatter(void);
  virtual int put(ACE_Message_Block *mblk, ACE_Time_Value * = 0); 

private:
  typedef void (*FORMATTER[5])(ACE_Message_Block *); 
  static FORMATTER format_; 
};

LOGREC_MODULE(Logrec_Formatter); 