#pragma once

#include "ace/Acceptor.h" 
#include "ace/SOCK_Acceptor.h" 
#include "TPC_Logging_Handler.h" 
#include <openssl/ssl.h> 

#if !defined (TPC_CERTIFICATE_FILENAME)
#  define TPC_CERTIFICATE_FILENAME "tpc-cert.pem" 
#endif 

#if !defined (TPC_KEY_FILENAME)
#  define TPC_KEY_FILENAME "tpc-key.pem" 
#endif 

class TPC_Logging_Acceptor : public ACE_Acceptor<TPC_Logging_Handler, ACE_SOCK_ACCEPTOR>
{
public:
  TPC_Logging_Acceptor(ACE_Reactor *r);
  ~TPC_Logging_Acceptor(void);

  typedef ACE_Acceptor<TPC_Logging_Handler, ACE_SOCK_ACCEPTOR> PARENT; 
  typedef PARENT::addr_type PEER_ADDR; 

  virtual int open(const addr_type &local_addr, 
    ACE_Reactor *reactor = ACE_Reactor::instance(), 
    int flags = 0, int use_select = 1, int reuse_addr = 1); 

  virtual int handle_close(ACE_HANDLE = ACE_INVALID_HANDLE, 
    ACE_Reactor_Mask = ACE_Event_Handler::ALL_EVENTS_MASK); 

  virtual int accept_svc_handler(TPC_Logging_Handler *sh); 

private:
  SSL_CTX *ssl_ctx_; 
  SSL *ssl_; 
};
