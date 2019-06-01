#pragma once

#include "ace/Task_T.h" 
#include "ace/FILE_IO.h" 
#include "ace/SString.h" 
#include "ace/Module.h" 

class Logrec_Reader : public ACE_Task<ACE_MT_SYNCH>
{
public:
  Logrec_Reader(const ACE_TString &file);
  virtual ~Logrec_Reader(void);

  enum 
  { 
    MB_CLIENT = ACE_Message_Block::MB_USER, 
    MB_TYPE, 
    MB_PID, 
    MB_TIME, 
    MB_TEXT, 
  }; 

  virtual int open(void *arg = 0); 
  virtual int svc(); 

private:
  ACE_TString filename_; 
  ACE_FILE_IO logfile_; 
};

class Logrec_Reader_Module : public ACE_Module<ACE_MT_SYNCH>
{
public:
  Logrec_Reader_Module(const ACE_TString &filename) 
    : ACE_Module<ACE_MT_SYNCH>(ACE_TEXT("Logrec Reader"), &task_, 0, 0, ACE_Module<ACE_MT_SYNCH>::M_DELETE_READER)
    , task_(filename)

  {
  }

private:
  Logrec_Reader task_; 
}; 