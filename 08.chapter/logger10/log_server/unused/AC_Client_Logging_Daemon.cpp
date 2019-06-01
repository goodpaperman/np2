#include "stdafx.h"
#include "AC_Client_Logging_Daemon.h"

ACE_FACTORY_DEFINE(ACE_Local_Service, AC_Client_Logging_Daemon)
ACE_STATIC_SVC_DEFINE(CLD_Descriptor, ACE_TEXT("AC_Client_Logging_Daemon"), \
                      ACE_SVC_OBJ_T, &ACE_SVC_NAME(AC_Client_Logging_Daemon), \
                      ACE_Service_Type::DELETE_OBJ | ACE_Service_Type::DELETE_THIS, 0)
ACE_STATIC_SVC_REQUIRE(CLD_Descriptor)

AC_Client_Logging_Daemon::AC_Client_Logging_Daemon(void)
: acceptor_(&output_handler_)
, connector_(&output_handler_)
{
}

AC_Client_Logging_Daemon::~AC_Client_Logging_Daemon(void)
{
}


#define MAXHOSTNAMELEN 260

int AC_Client_Logging_Daemon::init(int argc, ACE_TCHAR *argv[])
{
  u_short cld_port = ACE_DEFAULT_SERVICE_PORT; 
  u_short sld_port = ACE_DEFAULT_LOGGING_SERVER_PORT; 
  ACE_TCHAR sld_host[MAXHOSTNAMELEN] = { 0 }; 
  ACE_OS_String::strcpy(sld_host, ACE_LOCALHOST); 

  ACE_Get_Opt get_opt(argc, argv, ACE_TEXT("p:r:s:"), 0); 
  get_opt.long_option(ACE_TEXT("client_port"), 'p', ACE_Get_Opt::ARG_REQUIRED); 
  get_opt.long_option(ACE_TEXT("server_port"), 'r', ACE_Get_Opt::ARG_REQUIRED); 
  get_opt.long_option(ACE_TEXT("server_name"), 's', ACE_Get_Opt::ARG_REQUIRED); 

  for(int c; (c = get_opt()) != -1;)
  {
    switch(c)
    {
    case 'p':
      cld_port = ACE_static_cast(u_short, ACE_OS::atoi(get_opt.opt_arg())); 
      break; 
    case 'r':
      sld_port = ACE_static_cast(u_short, ACE_OS::atoi(get_opt.opt_arg())); 
      break; 
    case 's':
      ACE_OS_String::strsncpy(sld_host, get_opt.opt_arg(), MAXHOSTNAMELEN); 
      break; 
    }
  }

  ACE_INET_Addr cld_addr(cld_port); 
  ACE_INET_Addr sld_addr(sld_port, sld_host); 

  if(acceptor_.open(cld_addr) == -1)
    return -1; 

  AC_Output_Handler *oh = &output_handler_; 
  if(connector_.connect(oh, sld_addr) == -1)
  {
    acceptor_.close(); 
    return -1; 
  }

  return 0; 
}

int AC_Client_Logging_Daemon::fini()
{
  acceptor_.close(); 
  return 0; 
}

int AC_Client_Logging_Daemon::info(ACE_TCHAR **bufferp, size_t length) const
{
  return -1; 
}

int AC_Client_Logging_Daemon::suspend()
{
  return -1; 
}

int AC_Client_Logging_Daemon::resume()
{
  return -1; 
}