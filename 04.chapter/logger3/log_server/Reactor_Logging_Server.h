#ifndef __REACTOR_LOGGING_SERVER_H__
#define __REACTOR_LOGGING_SERVER_H__

#include "ace/ACE.h" 
#include "ace/Reactor.h" 

//#include "ace/Handle_Set.h" 
//#include "ace/Hash_Map_Manager.h" 
//#include "ace/Synch.h" 
//#include "Logging_Server.h" 
//#include "Logging_Handler.h" 
#include "ace/Manual_Event.h" 
#include "ace/Thread_Manager.h" 
#include "ace/os_ns_string.h" 

class Quit_Handler : public ACE_Event_Handler
{
public:
  enum { BUFSIZE = 128, }; 

  Quit_Handler(ACE_Reactor *r) : ACE_Event_Handler(r) { 
    SetConsoleMode(ACE_STDIN, 
      ENABLE_LINE_INPUT | 
      ENABLE_ECHO_INPUT | 
      ENABLE_PROCESSED_INPUT); 

    if(r->register_handler(this, quit_seen_.handle()) == -1 ||
      ACE_Event_Handler::register_stdin_handler(this, r, ACE_Thread_Manager::instance()) == -1)
      r->end_event_loop(); 
  }

  virtual int handle_input(ACE_HANDLE h) { 
    CHAR user_input[BUFSIZE] = { 0 }; 
    DWORD count = 0; 
    if(!ReadFile(h, user_input, BUFSIZE, &count, 0))
      return -1; 

    user_input[count] = 0; 
    if(ACE_OS_String::strncmp(user_input, "quit", 4) == 0)
      return -1; 

    return 0; 
  }

  virtual int handle_close(ACE_HANDLE, ACE_Reactor_Mask)
  {
    quit_seen_.signal(); 
    return 0; 
  }

  virtual int handle_signal(int, siginfo_t*, ucontext_t*)
  {
    reactor()->end_reactor_event_loop(); 
    return 0; 
  }

  virtual ~Quit_Handler() {
    ACE_Event_Handler::remove_stdin_handler(reactor(), ACE_Thread_Manager::instance()); 
    reactor()->remove_handler(quit_seen_.handle(), ACE_Event_Handler::DONT_CALL); 
  }

private:
  ACE_Manual_Event quit_seen_; 
}; 

template <class ACCEPTOR>
class Reactor_Logging_Server : public ACCEPTOR
{
public:
  Reactor_Logging_Server(int argc, char *argv[], ACE_Reactor *reactor); 
}; 

template <class ACCEPTOR>
Reactor_Logging_Server<ACCEPTOR>::Reactor_Logging_Server(
  int argc, char *argv[], ACE_Reactor *reactor) : ACCEPTOR(reactor)
{
  u_short logger_port = argc > 1 ? atoi(argv[1]) : 0; 
  ACE_TYPENAME ACCEPTOR::PEER_ADDR server_addr; 
  int result = 0; 

  if(logger_port != 0)
    result = server_addr.set(logger_port); 
  else 
    result = server_addr.set("ace_logger"); 

  if(result != -1)
    result = ACCEPTOR::open(server_addr); 

  if(result == -1)
    reactor->end_reactor_event_loop(); 
}

#endif 