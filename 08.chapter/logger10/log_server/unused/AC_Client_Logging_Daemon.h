#pragma once
#include "ace\service_object.h"
#include "AC_CLD_Acceptor.h" 
#include "AC_CLD_Connector.h" 

class AC_Client_Logging_Daemon : public ACE_Service_Object
{
public:
  AC_Client_Logging_Daemon(void);
  ~AC_Client_Logging_Daemon(void);

  virtual int init(int argc, ACE_TCHAR *argv[]); 
  virtual int fini(); 
  virtual int info(ACE_TCHAR **bufferp, size_t length = 0) const; 
  virtual int suspend(); 
  virtual int resume(); 

protected:
  AC_CLD_Acceptor acceptor_; 
  AC_CLD_Connector connector_; 
  AC_Output_Handler output_handler_; 
};

ACE_FACTORY_DECLARE(ACE_Local_Service, AC_Client_Logging_Daemon)
