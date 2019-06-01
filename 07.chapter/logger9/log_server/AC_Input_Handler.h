#pragma once

#include "AC_Output_Handler.h" 
#include "ace/Handle_Set.h" 

class AC_Input_Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
public:
  AC_Input_Handler(AC_Output_Handler *handler = 0);
  virtual ~AC_Input_Handler(void);
  virtual int open(void *); 
  virtual int close(u_int = 0); 

protected:
  virtual int handle_input(ACE_HANDLE handle); 
  virtual int handle_close(ACE_HANDLE = ACE_INVALID_HANDLE, ACE_Reactor_Mask = 0); 

protected:
  AC_Output_Handler *output_handler_; 
  ACE_Handle_Set connected_clients_; 
};
