#include "stdafx.h"
#include "TPC_Logging_Acceptor.h"

TPC_Logging_Acceptor::TPC_Logging_Acceptor(ACE_Reactor *r)
: ACE_Acceptor<TPC_Logging_Handler, ACE_SOCK_ACCEPTOR>(r) 
, ssl_ctx_(0)
, ssl_(0) 
{
}

TPC_Logging_Acceptor::~TPC_Logging_Acceptor(void)
{
  SSL_free(ssl_); 
  SSL_CTX_free(ssl_ctx_); 
}

int TPC_Logging_Acceptor::open(const addr_type &local_addr, ACE_Reactor *reactor, int flags, int use_select, int reuse_addr)
{
  if(PARENT::open(local_addr, reactor, flags, use_select, reuse_addr) == -1)
    return -1; 

  OpenSSL_add_ssl_algorithms(); 
  ssl_ctx_ = SSL_CTX_new(SSLv3_server_method()); 
  if(ssl_ctx_ == 0)
    return -1; 

  if(SSL_CTX_use_certificate_file(ssl_ctx_, TPC_CERTIFICATE_FILENAME, SSL_FILETYPE_PEM) <= 0)
    return -1; 
  
  if(SSL_CTX_use_PrivateKey_file(ssl_ctx_, TPC_KEY_FILENAME, SSL_FILETYPE_PEM) <= 0)
    return -1; 

  if(!SSL_CTX_check_private_key(ssl_ctx_))
    return -1;

  ssl_ = SSL_new(ssl_ctx_); 
  return ssl_ == 0 ? -1 : 0; 
}

int TPC_Logging_Acceptor::handle_close(ACE_HANDLE h, ACE_Reactor_Mask mask)
{
  PARENT::handle_close(h, mask); 
  delete this; 
  return 0; 
}

int TPC_Logging_Acceptor::accept_svc_handler(TPC_Logging_Handler *sh)
{
  if(PARENT::accept_svc_handler(sh) == -1)
    return -1; 

  SSL_clear(ssl_); 
  SSL_set_fd(ssl_, ACE_reinterpret_cast(int, sh->get_handle())); 
  SSL_set_verify(ssl_, SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT, 0); 
  if(SSL_accept(ssl_) == -1)
    return -1; 

  if(SSL_shutdown(ssl_) == -1)
    return -1; 

  return 0; 
}
