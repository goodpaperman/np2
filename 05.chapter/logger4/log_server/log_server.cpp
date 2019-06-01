// log_server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ace/os.h" 
#include "ace/Service_Config.h" 
#include "ace/Reactor.h" 

int ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  ACE_STATIC_SVC_REGISTER(Reporter_Descriptor); 
  ACE_Service_Config::open(argc, argv, ACE_DEFAULT_LOGGER_KEY/*, 0*/); 
  ACE_Reactor::instance()->run_reactor_event_loop(); 
  return 0; 
}

//#include "ace/Log_Msg.h" 
//#include "ace/Reactor.h" 
//#include "ace/WFMO_Reactor.h" 
//#include "ace/Thread_Manager.h" 
//#include "ace/Dynamic_Service.h" 
//#include "Reactor_Logging_Server.h" 
//#include "Logging_Acceptor_WFMO.h" 
//#include "Reactor_Logging_Server_Adapter.h" 
//#include "Service_Reporter.h" 
//#include <string> 
//#include <iostream> 
//
//ACE_THR_FUNC_RETURN event_loop(void *); 
//
//template Reactor_Logging_Server_Adapter<Logging_Acceptor_WFMO>; 

//int main(int argc, char* argv[])
//{
//  const size_t N_THREADS = 4; 
//  ACE_WFMO_Reactor wfmo_reactor; 
//  ACE_Reactor reactor(&wfmo_reactor); 
//  Reactor_Logging_Server<Logging_Acceptor_WFMO> *server = 0; 
//  ACE_NEW_RETURN(server, 
//    Reactor_Logging_Server<Logging_Acceptor_WFMO>(argc, argv, &reactor), 
//    1); 
//
//  Quit_Handler quit_handler(&reactor); 
//  ACE_Thread_Manager::instance()->spawn_n(N_THREADS, event_loop, &reactor); 
//
//  Service_Reporter *reporter = ACE_Dynamic_Service<Service_Reporter>::instance(ACE_TEXT("Service_Reporter")); 
//  if(reporter)
//  {
//    ACE_TCHAR *service_info = 0; 
//    reporter->info(&service_info); 
//    ACE_DEBUG((LM_DEBUG, "%s\n", service_info)); 
//    ACE::strdelete(service_info); 
//  }
//
//  return ACE_Thread_Manager::instance()->wait();
//}


//
//static ACE_THR_FUNC_RETURN event_loop(void *arg)
//{
//  ACE_Reactor *reactor = ACE_static_cast(ACE_Reactor*, arg); 
//  //reactor->owner(ACE_OS::thr_self()); 
//  reactor->run_reactor_event_loop(); 
//  return 0; 
//}

