#include "stdafx.h"
#include "AC_CLD_Acceptor.h"

AC_CLD_Acceptor::AC_CLD_Acceptor(AC_Output_Handler *handler)
: output_handler_(handler)
, input_handler_(handler) 
{
}

AC_CLD_Acceptor::~AC_CLD_Acceptor(void)
{
}

int AC_CLD_Acceptor::make_svc_handler(AC_Input_Handler *&sh)
{
  sh = &input_handler_; 
  return 0; 
}

int AC_CLD_Acceptor::handle_close(ACE_HANDLE h, ACE_Reactor_Mask mask)
{
  PARENT::handle_close(h, mask); 
  input_handler_.close(); 
  return 0; 
}