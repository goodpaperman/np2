// RT_Thread_Per_Connection_Logging_Server.cpp: implementation of the RT_Thread_Per_Connection_Logging_Server class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RT_Thread_Per_Connection_Logging_Server.h"
#include "ace/Sched_Params.h" 

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

int RT_Thread_Per_Connection_Logging_Server::open(u_short logger_port)
{
    ACE_Sched_Params fifo_sched_params(ACE_SCHED_FIFO, 
        ACE_Sched_Params::priority_min(ACE_SCHED_FIFO), 
        ACE_SCOPE_PROCESS); 

    if(ACE_OS::sched_params(fifo_sched_params) == -1)
    {
        if(errno == EPERM || errno == ENOTSUP)
            ACE_DEBUG((LM_DEBUG, 
            "Warning: user's not superuser, so "
            "we'll run in the time-shared class\n")); 
        else 
            ACE_ERROR_RETURN((LM_ERROR, 
            "%p\n", "ACE_OS::sched_params()"), -1); 
    }

    return Thread_Per_Connection_Logging_Server::open(logger_port); 
}

int RT_Thread_Per_Connection_Logging_Server::handle_data(ACE_SOCK_Stream *logging_peer)
{
    int prio = ACE_Sched_Params::next_priority(ACE_SCHED_FIFO, 
        ACE_Sched_Params::priority_min(ACE_SCHED_FIFO), 
        ACE_SCOPE_THREAD); 

    ACE_OS::thr_setprio(prio); 
    return Thread_Per_Connection_Logging_Server::handle_data(logging_peer); 
}