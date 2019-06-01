#ifndef __REACTIVE_LOGGING_SERVER_H__
#define __REACTIVE_LOGGING_SERVER_H__

#include "ace/ACE.h" 
#include "ace/Handle_Set.h" 
#include "ace/Hash_Map_Manager.h" 
#include "ace/Synch.h" 
#include "Logging_Server.h" 
#include "Logging_Handler.h" 

typedef ACE_Hash_Map_Manager<ACE_HANDLE, ACE_FILE_IO*, ACE_Null_Mutex> LOG_MAP; 

class Reactive_Logging_Server_Ex : public Logging_Server
{
public:
    virtual int open(u_short logger_port = 0); 
    virtual int wait_for_multiple_events(); 
    virtual int handle_connections(); 
    virtual int handle_data(ACE_SOCK_Stream* = 0); 

private:
    LOG_MAP log_map_; 
    ACE_Handle_Set active_handles_; 
    ACE_Handle_Set master_handle_set_; 
}; 

#endif 