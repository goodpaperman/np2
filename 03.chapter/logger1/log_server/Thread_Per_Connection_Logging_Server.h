// Thread_Per_Connection_Logging_Server.h: interface for the Thread_Per_Connection_Logging_Server class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_THREAD_PER_CONNECTION_LOGGING_SERVER_H__0EC7DAD2_8D5F_4829_8B67_60D85E4848E7__INCLUDED_)
#define AFX_THREAD_PER_CONNECTION_LOGGING_SERVER_H__0EC7DAD2_8D5F_4829_8B67_60D85E4848E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ace/SOCK_Stream.h" 
#include "ace/Thread_Manager.h" 
#include "Logging_Server.h"
#include "Logging_Handler.h" 

class Thread_Per_Connection_Logging_Server : public Logging_Server  
{
public:
    virtual int run(int argc, char *argv[]); 
    virtual int handle_connections(); 
    virtual int handle_data(ACE_SOCK_Stream* = 0); 

private:
    class Thread_Args 
    {
    public:
        Thread_Args(Thread_Per_Connection_Logging_Server *lsp) : this_(lsp) {}

        Thread_Per_Connection_Logging_Server *this_; 
        ACE_SOCK_Stream logging_peer_; 
    }; 

    static unsigned long run_svc(void *arg); 

};

class Atomic_Op
{
public:
    Atomic_Op(long count = 0) : count_(count) {}
    long operator++() { 
        ACE_WRITE_GUARD_RETURN(ACE_RW_Thread_Mutex, guard, lock_, -1); 
        return ++ count_; 
    }

    operator long() { 
        ACE_READ_GUARD_RETURN(ACE_RW_Thread_Mutex, guard, lock_, -1); 
        return count_; 
    }

private:
    ACE_RW_Thread_Mutex lock_; 
    long count_; 
}; 

#endif // !defined(AFX_THREAD_PER_CONNECTION_LOGGING_SERVER_H__0EC7DAD2_8D5F_4829_8B67_60D85E4848E7__INCLUDED_)
