
#include "stdafx.h" 
#include "Reactive_Logging_Server.h" 


int Reactive_Logging_Server_Ex::open(u_short logger_port)
{
    Logging_Server::open(logger_port); 
    master_handle_set_.set_bit(acceptor().get_handle()); 
    acceptor().enable(ACE_NONBLOCK); 
    return 0; 
}

int Reactive_Logging_Server_Ex::wait_for_multiple_events()
{
    active_handles_ = master_handle_set_; 
    int width = (int)active_handles_.max_set() + 1; 
    return ACE::select(width, active_handles_); 
}


int Reactive_Logging_Server_Ex::handle_connections()
{
    ACE_SOCK_Stream logging_peer; 
    while(active_handles_.is_set(acceptor().get_handle()) && 
        acceptor().accept(logging_peer) != -1)
    {
        ACE_FILE_IO *log_file = new ACE_FILE_IO; 
        make_log_file(*log_file, &logging_peer); 

        log_map_.bind(logging_peer.get_handle(), log_file); 
        master_handle_set_.set_bit(logging_peer.get_handle()); 
    }

    active_handles_.clr_bit(acceptor().get_handle()); 
    return 0; 
}


int Reactive_Logging_Server_Ex::handle_data(ACE_SOCK_Stream*) 
{
    ACE_Handle_Set_Iterator peer_iterator(active_handles_); 
    for(ACE_HANDLE handle; (handle = peer_iterator()) != ACE_INVALID_HANDLE; )
    {
        ACE_FILE_IO *log_file = 0; 
        log_map_.find(handle, log_file); 
        Logging_Handler logging_handler(*log_file, handle); 
        
        //logging_handler().peer().set_handle(handle); 
        if(logging_handler.log_record() == -1)
        {
            logging_handler.close(); 
            master_handle_set_.clr_bit(handle); 
            log_map_.unbind(handle); 
            log_file->close(); 
            delete log_file; 
        }
    }

    return 0; 
}


