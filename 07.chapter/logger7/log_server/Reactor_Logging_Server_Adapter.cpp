#include "stdafx.h"
#include "ace/Acceptor.h"  
#include "ace/SOCK_Acceptor.h" 
#include "ace/Service_Config.h" 
#include "Reactor_Logging_Server_Adapter.h"
#include "TPC_Logging_Handler.h"


typedef Reactor_Logging_Server_Adapter<ACE_Acceptor<TPC_Logging_Handler, ACE_SOCK_ACCEPTOR> > Server_Logging_Daemon; 
ACE_FACTORY_DEFINE (ACE_Local_Service, Server_Logging_Daemon)
ACE_STATIC_SVC_DEFINE(Server_Logging_Daemon, ACE_TEXT("Server_Logging_Daemon"), \
                      ACE_SVC_OBJ_T, &ACE_SVC_NAME(Server_Logging_Daemon), \
                      ACE_Service_Type::DELETE_OBJ | ACE_Service_Type::DELETE_THIS, 0)
ACE_STATIC_SVC_REQUIRE(Server_Logging_Daemon)