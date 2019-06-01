#pragma once
#include "ace/service_object.h"
#include "ace/SOCK_Acceptor.h" 
#include "ace/Reactor.h" 

class Service_Reporter : public ACE_Service_Object
{
public:
  Service_Reporter(ACE_Reactor *r = ACE_Reactor::instance()); 

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

protected:
  virtual int handle_input(ACE_HANDLE); 
  virtual ACE_HANDLE get_handle() const
  {  return acceptor_.get_handle(); } 

private:
  ACE_SOCK_Acceptor acceptor_; 
  enum { DEFAULT_PORT = 9411 }; 
};

ACE_FACTORY_DECLARE(ACE_Local_Service, Service_Reporter); 
//ACE_STATIC_SVC_DECLARE_EXPORT(ACE_Local_Service, Service_Reporter); 
//ACE_STATIC_SVC_DECLARE(Service_Reporter); 



class Server_Shutdown : public ACE_Service_Object
{
public:
  virtual int init(int, ACE_TCHAR *[]); 
  //virtual int fini(); 

private:
  ACE_Reactor *reactor_; 
}; 
