#pragma once

#include "ace/svc_handler.h" 
#include "Logging_Handler.h" 

class TPC_Logging_Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
public:
  TPC_Logging_Handler(void);
  ~TPC_Logging_Handler(void);

  virtual int open(void *); 
  virtual int svc(); 

protected:
  ACE_FILE_IO log_file_; 
  Logging_Handler logging_handler_; 
};
