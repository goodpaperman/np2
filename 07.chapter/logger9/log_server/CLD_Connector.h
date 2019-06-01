#pragma once

#include "CLD_Handler.h" 

class CLD_Connector
{
public:
  CLD_Connector(void);
  ~CLD_Connector(void);

  int connect(CLD_Handler *handler, const ACE_INET_Addr &remote_addr); 
  int reconnect(); 

private:
  CLD_Handler *handler_; 
  ACE_INET_Addr remote_addr_; 
};
