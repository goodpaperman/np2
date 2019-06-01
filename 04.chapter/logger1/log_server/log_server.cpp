// log_server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ace/Log_Msg.h" 
#include "ace/Reactor.h" 
#include "ace/Select_Reactor.h" 
#include "ace/Thread_Manager.h" 
#include "Reactor_Logging_Server.h" 
#include "Logging_Acceptor_Ex.h" 
#include <string> 
#include <iostream> 

ACE_THR_FUNC_RETURN controller(void *); 
ACE_THR_FUNC_RETURN event_loop(void *); 

class Quit_Handler : public ACE_Event_Handler
{
public:
  Quit_Handler(ACE_Reactor *r) : ACE_Event_Handler(r) { }
  virtual int handle_exception(ACE_HANDLE) { 
    reactor()->end_reactor_event_loop(); 
    return -1; 
  }

  virtual int handle_close(ACE_HANDLE, ACE_Reactor_Mask)
  {
    delete this; 
    return 0; 
  }

private:
  virtual ~Quit_Handler() { }
}; 

int main(int argc, char* argv[])
{
  ACE_Select_Reactor select_reactor; 
  ACE_Reactor reactor(&select_reactor); 
  Reactor_Logging_Server<Logging_Acceptor_Ex> *server = 0; 
  ACE_NEW_RETURN(server, 
    Reactor_Logging_Server<Logging_Acceptor_Ex>(argc, argv, &reactor), 
    1); 

  ACE_Thread_Manager::instance()->spawn(event_loop, &reactor); 
  ACE_Thread_Manager::instance()->spawn(controller, &reactor); 
  //if(reactor.run_reactor_event_loop() == -1)
  //  ACE_ERROR_RETURN((LM_ERROR, "%p\n", "run_reactor_event_loop()"), 1); 

  return ACE_Thread_Manager::instance()->wait();
}

static ACE_THR_FUNC_RETURN event_loop(void *arg)
{
  ACE_Reactor *reactor = ACE_static_cast(ACE_Reactor*, arg); 
  reactor->owner(ACE_OS::thr_self()); 
  reactor->run_reactor_event_loop(); 
  return 0; 
}

static ACE_THR_FUNC_RETURN controller(void *arg)
{
  ACE_Reactor *reactor = ACE_static_cast(ACE_Reactor*, arg); 
  Quit_Handler *quit_handler = 0; 
  ACE_NEW_RETURN(quit_handler, Quit_Handler(reactor), 0); 
  
  while(1)
  {
    std::string user_input; 
    std::getline(std::cin, user_input, '\n'); 
    if(user_input == "quit")
    {
      reactor->notify(quit_handler); 
      break; 
    }
  }

  return 0; 
}
