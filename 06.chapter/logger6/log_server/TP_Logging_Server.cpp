#include "stdafx.h"
#include "TP_Logging_Server.h"
#include "TP_Logging_Task.h" 
#include "ace/Service_Config.h" 


ACE_FACTORY_DEFINE (ACE_Local_Service, TP_Logging_Server)
ACE_STATIC_SVC_DEFINE(Logging_Server_Descriptor, ACE_TEXT("TP_Logging_Server"), \
                      ACE_SVC_OBJ_T, &ACE_SVC_NAME(TP_Logging_Server), \
                      ACE_Service_Type::DELETE_OBJ | ACE_Service_Type::DELETE_THIS, 0)
ACE_STATIC_SVC_REQUIRE(Logging_Server_Descriptor)


TP_Logging_Server::TP_Logging_Server(void)
: logging_dispatcher_(0) 
{
}

TP_Logging_Server::~TP_Logging_Server(void)
{
}

int TP_Logging_Server::init(int argc, ACE_TCHAR *argv[])
{
  int i = 0; 
  char **array = 0; 
  ACE_NEW_RETURN(array, char*[argc], -1); 
  ACE_Auto_Array_Ptr<char*> char_argv(array); 

  for(i=0; i<argc; ++ i)
    char_argv[i] = ACE::strnew(ACE_TEXT_ALWAYS_CHAR(argv[i])); 

  ACE_NEW_NORETURN(logging_dispatcher_, TP_Logging_Server::LOGGING_DISPATCHER(i, char_argv.get(), ACE_Reactor::instance())); 

  for(i=0; i<argc; ++ i)
    ACE::strdelete(char_argv[i]); 

  if(logging_dispatcher_ == 0)
    return -1; 
  else 
    return TP_LOGGING_TASK::instance()->open(); 
}

int TP_Logging_Server::fini()
{
  TP_LOGGING_TASK::instance()->flush(); 
  TP_LOGGING_TASK::instance()->wait(); 
  TP_LOGGING_TASK::close(); 
  delete logging_dispatcher_; 
  return 0; 
}

int TP_Logging_Server::info(ACE_TCHAR **info_string, size_t length) const 
{
  ACE_TCHAR buf[1024]; 
  if(logging_dispatcher_)
  {
    //ACE_TYPENAME ACCEPTOR::PEER_ADDR local_addr; 
    ACE_INET_Addr local_addr; 
    logging_dispatcher_->acceptor().get_local_addr(local_addr); 
    ACE_OS::sprintf(buf, ACE_TEXT("%hu"), local_addr.get_port_number()); 
  }
  else 
    ACE_OS::strcpy(buf, ACE_TEXT("0")); 

  ACE_OS_String::strcat(buf, ACE_TEXT("/tcp # Reactive logging server\n")); 
  if(*info_string == 0)
    *info_string = ACE::strnew(buf); 
  else 
    ACE_OS_String::strncpy(*info_string, buf, length); 

  return ACE_OS_String::strlen(*info_string); 
  return -1; 
}

int TP_Logging_Server::suspend()
{
  //return -1; 
  return ACE_Reactor::instance()->suspend_handler(logging_dispatcher_); 
}

int TP_Logging_Server::resume()
{
  //return -1; 
  return ACE_Reactor::instance()->resume_handler(logging_dispatcher_); 
}

