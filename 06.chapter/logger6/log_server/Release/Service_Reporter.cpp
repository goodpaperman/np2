#include "stdafx.h"
#include "Service_Reporter.h"
#include "ace/Get_Opt.h" 
#include "ace/Service_Repository.h" 
#include "ace/os_ns_stdio.h" 
#include "ace/os_ns_string.h" 
#include "ace/Service_Types.h" 
#include "ace/Service_Config.h" 

ACE_FACTORY_DEFINE (ACE_Local_Service, Service_Reporter)
ACE_STATIC_SVC_DEFINE(Reporter_Descriptor, ACE_TEXT("Service_Reporter"), \
                      ACE_SVC_OBJ_T, &ACE_SVC_NAME(Service_Reporter), \
                      ACE_Service_Type::DELETE_OBJ | ACE_Service_Type::DELETE_THIS, 0)
ACE_STATIC_SVC_REQUIRE(Reporter_Descriptor)
//ACE_STATIC_SVC_REGISTER(Reporter_Descriptor)

Service_Reporter::Service_Reporter(ACE_Reactor *r)
: ACE_Service_Object(r)
{
}

int Service_Reporter::init(int argc, ACE_TCHAR *argv[])
{
  ACE_INET_Addr local_addr(DEFAULT_PORT); 
  ACE_Get_Opt get_opt(argc, argv, ACE_TEXT("p:"), 0); 
  get_opt.long_option(ACE_TEXT("port"), 'p', ACE_Get_Opt::ARG_REQUIRED); 
  for(int c=0; (c=get_opt() != -1);)
  {
    if(c == 'p') 
      local_addr.set_port_number(ACE_OS::atoi(get_opt.opt_arg())); 
  }

  acceptor_.open(local_addr); 
  return reactor()->register_handler(this, ACE_Event_Handler::ACCEPT_MASK); 
}

int Service_Reporter::fini()
{
  reactor()->remove_handler(this, 
    ACE_Event_Handler::ACCEPT_MASK
    | ACE_Event_Handler::DONT_CALL); 

  return acceptor_.close(); 
}

int Service_Reporter::info(ACE_TCHAR **info_string, size_t length) const 
{
  ACE_INET_Addr local_addr; 
  acceptor_.get_local_addr(local_addr); 

  ACE_TCHAR buf[1024] = { 0 }; 
  ACE_OS::sprintf(buf, ACE_TEXT("%hu"), local_addr.get_port_number()); 
  ACE_OS_String::strcat(buf, ACE_TEXT("/tcp   #   lists services in daemon\n")); 
  if(*info_string == 0)
    *info_string = ACE::strnew(buf); 
  else
    ACE_OS_String::strncpy(*info_string, buf, length); 

  return ACE_OS_String::strlen(*info_string); 
}

int Service_Reporter::suspend()
{
  return reactor()->suspend_handler(this); 
}

int Service_Reporter::resume()
{
  return reactor()->resume_handler(this); 
}

int Service_Reporter::handle_input(ACE_HANDLE)
{
  ACE_SOCK_Stream peer_stream; 
  acceptor_.accept(peer_stream); 

  ACE_Service_Repository_Iterator iter(*ACE_Service_Repository::instance(), 0); 

  for(const ACE_Service_Type *st; iter.next(st) != 0; iter.advance())
  {
    iovec iov[3] = { 0 }; 
    iov[0].iov_base = ACE_const_cast(char*, st->name()); 
    iov[0].iov_len = ACE_OS_String::strlen(st->name())* sizeof(ACE_TCHAR); 

    const ACE_TCHAR *state = st->active() ? ACE_TEXT(" (active) ") : ACE_TEXT(" (paused) "); 
    iov[1].iov_base = ACE_const_cast(char*, state); 
    iov[1].iov_len = ACE_OS_String::strlen(state) * sizeof(ACE_TCHAR); 

    ACE_TCHAR *report = 0; 
    int len = st->type()->info(&report, 0); 
    iov[2].iov_base = ACE_static_cast(char* , report); 
    iov[2].iov_len = ACE_static_cast(size_t, len); 
    iov[2].iov_len *= sizeof(TCHAR); 

    peer_stream.sendv_n(iov, 3); 
    ACE::strdelete(report); 
  }

  peer_stream.close(); 
  return 0; 
}

#include "ace/Manual_Event.h" 
#include "ace/Thread_Manager.h" 

#include <iostream> 
#include <string> 


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
    ACE_Event_Handler::remove_stdin_handler(reactor(), ACE_Thread_Manager::instance()); 
    reactor()->remove_handler(quit_seen_.handle(), ACE_Event_Handler::DONT_CALL); 
    return 0; 
  }

  virtual ~Quit_Handler() {
    ACE_Event_Handler::remove_stdin_handler(reactor(), ACE_Thread_Manager::instance()); 
    reactor()->remove_handler(quit_seen_.handle(), ACE_Event_Handler::DONT_CALL); 
  }

private:
  ACE_Manual_Event quit_seen_; 
}; 

static ACE_THR_FUNC_RETURN controller(void *arg)
{
  ACE_Reactor *reactor = ACE_static_cast(ACE_Reactor*, arg); 
  Quit_Handler *quit_handler = 0; 
  ACE_NEW_RETURN(quit_handler, Quit_Handler(reactor), 0); 

  for(;;)
  {
    std::string user_input; 
    std::getline(std::cin, user_input, '\n'); 
    if(user_input == "quit")
    {
      reactor->notify(quit_handler); 
      break; 
    }
  }
}



ACE_FACTORY_DEFINE (ACE_Local_Service, Server_Shutdown)
ACE_STATIC_SVC_DEFINE(Shutdown_Descriptor, ACE_TEXT("Server_Shutdown"), \
                      ACE_SVC_OBJ_T, &ACE_SVC_NAME(Server_Shutdown), \
                      ACE_Service_Type::DELETE_OBJ | ACE_Service_Type::DELETE_THIS, 0)
ACE_STATIC_SVC_REQUIRE(Shutdown_Descriptor)

int Server_Shutdown::init(int, ACE_TCHAR *[]) 
{
  reactor_ = ACE_Reactor::instance(); 
  return ACE_Thread_Manager::instance()->spawn(controller, reactor_, THR_DETACHED); 
}
//
//int Server_Shutdown::fini()
//{
//  Quit_Handler *quit_handler = 0; 
//  ACE_NEW_RETURN(quit_handler, Quit_Handler(reactor_), -1); 
//  return reactor_->notify(quit_handler); 
//}
