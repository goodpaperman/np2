// Thread_Per_Connection_Logging_Server.cpp: implementation of the Thread_Per_Connection_Logging_Server class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Thread_Per_Connection_Logging_Server.h"
//#include "ace/TSS_T.h" 

//static ACE_TSS<Request_Count> request_count; 
//static int request_count = 0; 
//ACE_Thread_Mutex m; 

//static Atomic_Op request_count; 

#include "ace/Atomic_Op.h" 
static ACE_Atomic_Op<ACE_Thread_Mutex, long> request_count; 

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

int Thread_Per_Connection_Logging_Server::run(int argc, char *argv[])
{
    u_short logger_port = 0; 
    
    if(argc == 2)
        logger_port = atoi(argv[1]); 
    
    if(open(logger_port) == -1)
        return -1; 
    
    for(;;)
        if(handle_connections() == -1)
            return -1; 
        
    return 0;
}

int Thread_Per_Connection_Logging_Server::handle_connections()
{
    Thread_Args *thread_args = new Thread_Args(this); 
    if(acceptor().accept(thread_args->logging_peer_) == -1)
        return -1; 
    else if(ACE_Thread_Manager::instance()->spawn(
        Thread_Per_Connection_Logging_Server::run_svc, 
        ACE_static_cast(void*, thread_args), 
        THR_DETACHED | THR_SCOPE_SYSTEM) == -1)
        return -1; 
    else 
        return 0; 
}

unsigned long Thread_Per_Connection_Logging_Server::run_svc(void *arg)
{
    Thread_Args *thread_args = ACE_static_cast(Thread_Args*, arg); 
    thread_args->this_->handle_data(&thread_args->logging_peer_); 
    thread_args->logging_peer_.close(); 
    delete thread_args; 
    return 0; 
}

int Thread_Per_Connection_Logging_Server::handle_data(ACE_SOCK_Stream* logging_peer)
{
    ACE_FILE_IO log_file; 
    make_log_file(log_file, logging_peer); 
    logging_peer->disable(ACE_NONBLOCK); 

    Logging_Handler logging_handler(log_file, *logging_peer); 
    ACE_Thread_Manager *tm = ACE_Thread_Manager::instance(); 
    ACE_thread_t me = ACE_OS::thr_self(); 

    while(!tm->testcancel(me) && 
        logging_handler.log_record() != -1)
    {
        ++ request_count; 
        //ACE_GUARD_RETURN(ACE_Recursive_Thread_Mutex, obj, *ACE_Static_Object_Lock::instance(), -1); 
        //++ request_count; 

        //ACE_GUARD_RETURN(ACE_Thread_Mutex, obj, m, -1); 
        //++ request_count; 

        //ACE_Guard<ACE_Thread_Mutex> obj(m); 
        //++ request_count; 

        //if(m.acquire() == -1)
        //    return 0; 

        //++ request_count; 
        //m.release(); 

        //request_count->increment(); 
        continue; 
    }

    ACE_DEBUG((LM_DEBUG, "request_count = %d\n", request_count.value())); 

    //ACE_DEBUG((LM_DEBUG, "request_count = %d\n", request_count)); 

    //{
    //    ACE_GUARD_RETURN(ACE_Recursive_Thread_Mutex, obj, *ACE_Static_Object_Lock::instance(), -1);  
    //    ACE_DEBUG((LM_DEBUG, "request_count = %d\n", request_count)); 
    //}

    //{
    //    ACE_Guard<ACE_Thread_Mutex> obj(m); 
    //    ACE_DEBUG((LM_DEBUG, "request_count = %d\n", request_count)); 
    //}

    //m.acquire(); 
    //ACE_DEBUG((LM_DEBUG, "request_count = %d\n", request_count)); 
    //m.release(); 
    log_file.close(); 
    return 0; 
}