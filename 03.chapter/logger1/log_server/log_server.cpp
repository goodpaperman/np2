// log_server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ace/Log_Msg.h" 
#include "ace/Signal.h" 
#include "ace/Reactor.h"
#include "ace/Timer_Queue.h" 
#include "ace/High_Res_Timer.h" 
#include "RT_Thread_Per_Connection_Logging_Server.h" 

static void sigterm_handler(int signum) { }

int main(int argc, char* argv[])
{
  ACE_Sig_Action sa(sigterm_handler, SIGTERM); 
  ACE_Reactor *r = ACE_Reactor::instance(); 
  ACE_High_Res_Timer::global_scale_factor(); 
  r->timer_queue()->gettimeofday(&ACE_High_Res_Timer::gettimeofday_hr); 

  RT_Thread_Per_Connection_Logging_Server server; 
  if(server.run(argc, argv) == -1)
    ACE_ERROR_RETURN((LM_ERROR, "%p\n", "server.run()"), 1); 

  ACE_Thread_Manager::instance()->cancel_all(); 
  ACE_Time_Value timeout(60); 
  return ACE_Thread_Manager::instance()->wait(&timeout); 
}
