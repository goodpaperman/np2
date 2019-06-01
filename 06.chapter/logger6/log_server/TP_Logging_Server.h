#pragma once

#include "ace/Service_Object.h" 
#include "TP_Logging_Acceptor.h" 
#include "Reactor_Logging_Server.h" 

class TP_Logging_Server : public ACE_Service_Object 
{
public:
  TP_Logging_Server(void);
  ~TP_Logging_Server(void);

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
  typedef Reactor_Logging_Server<TP_Logging_Acceptor> LOGGING_DISPATCHER; 
  LOGGING_DISPATCHER *logging_dispatcher_; 
};

ACE_FACTORY_DECLARE(ACE_Local_Service, TP_Logging_Server); 