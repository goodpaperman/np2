// log_server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ace/Log_Msg.h" 
#include "ace/Reactor.h" 
#include "ace/WFMO_Reactor.h" 
#include "ace/Thread_Manager.h" 
#include "Reactor_Logging_Server.h" 
#include "Logging_Acceptor_WFMO.h" 
#include <string> 
#include <iostream> 

ACE_THR_FUNC_RETURN event_loop(void *); 

int main(int argc, char* argv[])
{
  const size_t N_THREADS = 4; 
  ACE_WFMO_Reactor wfmo_reactor; 
  ACE_Reactor reactor(&wfmo_reactor); 
  Reactor_Logging_Server<Logging_Acceptor_WFMO> *server = 0; 
  ACE_NEW_RETURN(server, 
    Reactor_Logging_Server<Logging_Acceptor_WFMO>(argc, argv, &reactor), 
    1); 

  Quit_Handler quit_handler(&reactor); 
  ACE_Thread_Manager::instance()->spawn_n(N_THREADS, event_loop, &reactor); 
  return ACE_Thread_Manager::instance()->wait();
}

static ACE_THR_FUNC_RETURN event_loop(void *arg)
{
  ACE_Reactor *reactor = ACE_static_cast(ACE_Reactor*, arg); 
  //reactor->owner(ACE_OS::thr_self()); 
  reactor->run_reactor_event_loop(); 
  return 0; 
}

