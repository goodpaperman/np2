#pragma once

#include "ace/Connector.h" 
#include "AC_Output_Handler.h" 

class AC_CLD_Connector : public ACE_Connector<AC_Output_Handler, ACE_SOCK_CONNECTOR>
{
public:
  typedef ACE_Connector<AC_Output_Handler, ACE_SOCK_CONNECTOR> PARENT; 
  AC_CLD_Connector(AC_Output_Handler *handler = 0);
  virtual ~AC_CLD_Connector(void);

  int reconnect(); 
  virtual int open(ACE_Reactor *r = ACE_Reactor::instance(), int flags = 0); 
  virtual int connect_svc_handler(
    AC_Output_Handler *&svc_handler,
    const ACE_SOCK_Connector::PEER_ADDR &remote_addr,
    ACE_Time_Value *timeout,
    const ACE_SOCK_Connector::PEER_ADDR &local_addr,
    int reuse_addr,
    int flags,
    int perms);

protected:
  AC_Output_Handler *handler_; 
  ACE_INET_Addr remote_addr_; 
};
