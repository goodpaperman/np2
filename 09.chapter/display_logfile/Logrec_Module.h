#pragma once

#include "ace/Module.h" 

template <typename TASK> 
class Logrec_Module : public ACE_Module<ACE_MT_SYNCH>
{
public:
  Logrec_Module(const ACE_TCHAR *name)
    : ACE_Module<ACE_MT_SYNCH>(name, &task_, 0, 0, ACE_Module<ACE_MT_SYNCH>::M_DELETE_READER) 
  {
  }

private:
  TASK task_; 
};

#define LOGREC_MODULE(NAME) typedef Logrec_Module<NAME> NAME##_Module 