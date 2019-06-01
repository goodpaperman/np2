#include "stdafx.h"
#include "Reactor_Logging_Server_Adapter.h"
#include "Logging_Acceptor.h" 
#include "ace/Service_Config.h" 


typedef Reactor_Logging_Server_Adapter<Logging_Acceptor> Server_Logging_Daemon; 
ACE_FACTORY_DEFINE (ACE_Local_Service, Server_Logging_Daemon)
ACE_STATIC_SVC_DEFINE(Server_Logging_Daemon, ACE_TEXT("Server_Logging_Daemon"), \
                      ACE_SVC_OBJ_T, &ACE_SVC_NAME(Server_Logging_Daemon), \
                      ACE_Service_Type::DELETE_OBJ | ACE_Service_Type::DELETE_THIS, 0)
ACE_STATIC_SVC_REQUIRE(Server_Logging_Daemon)