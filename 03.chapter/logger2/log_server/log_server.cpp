// log_server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ace/Log_Msg.h" 
#include "ace/Reactor.h" 
#include "Reactor_Logging_Server.h" 
#include "Logging_Acceptor.h" 

int main(int argc, char* argv[])
{
  ACE_Reactor reactor; 
  Reactor_Logging_Server<Logging_Acceptor> *server = 0; 
  ACE_NEW_RETURN(server, 
    Reactor_Logging_Server<Logging_Acceptor>(argc, argv, &reactor), 
    1); 

  if(reactor.run_reactor_event_loop() == -1)
    ACE_ERROR_RETURN((LM_ERROR, "%p\n", "run_reactor_event_loop()"), 1); 

  return 0;
}
