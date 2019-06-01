#pragma once

#include "AC_Output_Handler.h" 
#include "AC_Input_Handler.h" 

class AC_CLD_Acceptor : public ACE_Acceptor<AC_Input_Handler, ACE_SOCK_ACCEPTOR>
{
public:
  AC_CLD_Acceptor(AC_Output_Handler *handler = 0);
  virtual ~AC_CLD_Acceptor(void);

protected:
  typedef ACE_Acceptor<AC_Input_Handler, ACE_SOCK_ACCEPTOR> PARENT; 
  virtual int make_svc_handler(AC_Input_Handler *&sh); 
  virtual int handle_close(ACE_HANDLE = ACE_INVALID_HANDLE, ACE_Reactor_Mask = 0); 

protected:
  AC_Output_Handler *output_handler_; 
  AC_Input_Handler input_handler_; 
};
