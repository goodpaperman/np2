#include "stdafx.h"
#include "AC_CLD_Connector.h"

AC_CLD_Connector::AC_CLD_Connector(AC_Output_Handler *handler)
: handler_(handler) 
{
}

AC_CLD_Connector::~AC_CLD_Connector(void)
{
}


int AC_CLD_Connector::reconnect()
{
  int i = 0; 
  const size_t MAX_RETRIES = 5; 
  ACE_SOCK_Connector connector; 
  ACE_Time_Value timeout(1); 
  for(i=0; i<MAX_RETRIES; ++ i)
  {
    if(i > 0)
      ACE_OS::sleep(timeout); 

    ACE_Synch_Options options(ACE_Synch_Options::USE_TIMEOUT, timeout); 
    if(connect(handler_, remote_addr_, options) == -1)
      timeout *= 2; 
    else 
      break; 
  }

  return i == MAX_RETRIES ? -1 : 0; 
}

int AC_CLD_Connector::open(ACE_Reactor *r, int flags)
{
  if(PARENT::open(r, flags) == -1)
    return -1; 

  return 0; 
}

int AC_CLD_Connector::connect_svc_handler (
  AC_Output_Handler *&svc_handler,
  const ACE_SOCK_Connector::PEER_ADDR &remote_addr,
  ACE_Time_Value *timeout,
  const ACE_SOCK_Connector::PEER_ADDR &local_addr,
  int reuse_addr,
  int flags,
  int perms)
{
  if(PARENT::connect_svc_handler(svc_handler, remote_addr, timeout, local_addr, reuse_addr, flags, perms) == -1)
    return -1;

  remote_addr_ = remote_addr; 
  return 0; 
}
