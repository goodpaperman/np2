#pragma once

#include "ace\service_object.h"
#include "CLD_Handler.h" 
#include "CLD_Acceptor.h" 
#include "CLD_Connector.h" 

class Client_Logging_Daemon : public ACE_Service_Object
{
public:
  Client_Logging_Daemon(void);
  ~Client_Logging_Daemon(void);

  virtual int init(int argc, ACE_TCHAR *argv[]); 
  virtual int fini(); 
  virtual int info(ACE_TCHAR **bufferp, size_t length = 0) const; 
  virtual int suspend(); 
  virtual int resume(); 

protected:
  CLD_Handler handler_; 
  CLD_Acceptor acceptor_; 
  CLD_Connector connector_; 
};

ACE_FACTORY_DECLARE(ACE_Local_Service, Client_Logging_Daemon)
