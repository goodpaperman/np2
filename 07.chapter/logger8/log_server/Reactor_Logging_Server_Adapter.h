#pragma once

#include "ace/Service_Object.h" 
#include "Reactor_Logging_Server.h" 

template <class ACCEPTOR> 
class Reactor_Logging_Server_Adapter : public ACE_Service_Object
{
public:
  Reactor_Logging_Server_Adapter() : server_(0) {} 

  /// Initializes object when dynamic linking occurs.
  virtual int init (int argc, ACE_TCHAR *argv[]);

  /// Terminates object when dynamic unlinking occurs.
  virtual int fini (void);

  /// Returns information on a service object.
  virtual int info (ACE_TCHAR **info_string, size_t length = 0) const;

    /// Temporarily disable a service without removing it completely.
  virtual int suspend (void);

    /// Re-enable a previously suspended service.
  virtual int resume (void);

private:
  Reactor_Logging_Server<ACCEPTOR> *server_; 
};

template <class ACCEPTOR>
int Reactor_Logging_Server_Adapter<ACCEPTOR>::init(int argc, ACE_TCHAR *argv[])
{
  ACE_NEW_NORETURN(server_, Reactor_Logging_Server<ACCEPTOR>(argc, argv, ACE_Reactor::instance())); 
  return server_ == 0 ? -1 : 0; 
}


template <class ACCEPTOR>
int Reactor_Logging_Server_Adapter<ACCEPTOR>::fini()
{
  server_->handle_close(); 
  server_ = 0; 
  return 0; 
}


template <class ACCEPTOR>
int Reactor_Logging_Server_Adapter<ACCEPTOR>::info(ACE_TCHAR **info_string, size_t length) const 
{
  ACE_TCHAR buf[1024]; 
  if(server_)
  {
    ACE_TYPENAME ACCEPTOR::addr_type local_addr; 
    server_->acceptor().get_local_addr(local_addr); 
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
}

template <class ACCEPTOR>
int Reactor_Logging_Server_Adapter<ACCEPTOR>::suspend()
{
  return ACE_Reactor::instance()->suspend_handler(server_); 
}

template <class ACCEPTOR>
int Reactor_Logging_Server_Adapter<ACCEPTOR>::resume()
{
  return ACE_Reactor::instance()->resume_handler(server_); 
}
