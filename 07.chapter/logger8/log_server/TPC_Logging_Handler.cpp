#include "stdafx.h"
#include "TPC_Logging_Handler.h"
#include "ace/File_Connector.h" 

TPC_Logging_Handler::TPC_Logging_Handler(void)
: logging_handler_(log_file_) 
{
}

TPC_Logging_Handler::~TPC_Logging_Handler(void)
{
}

#define MAXHOSTNAMELEN 260 

int TPC_Logging_Handler::open(void *)
{
  static const ACE_TCHAR LOGFILE_SUFFIX[] = ACE_TEXT(".log"); 
  ACE_TCHAR filename[MAXHOSTNAMELEN + sizeof(LOGFILE_SUFFIX)]; 
  ACE_INET_Addr logging_peer_addr; 

  peer().get_remote_addr(logging_peer_addr); 
  logging_peer_addr.get_host_name(filename, MAXHOSTNAMELEN); 
  ACE_OS_String::strcat(filename, LOGFILE_SUFFIX); 

  ACE_FILE_Connector connector; 
  if(connector.connect(log_file_, 
    ACE_FILE_Addr(filename), 
    0, 
    ACE_Addr::sap_any, 
    0, 
    O_RDWR|O_CREAT|O_APPEND, 
    ACE_DEFAULT_FILE_PERMS) == -1)
    return -1; 

  logging_handler_.peer().set_handle(peer().get_handle()); 
  return activate(THR_NEW_LWP | THR_DETACHED); 
}

int TPC_Logging_Handler::svc()
{
  for(;;)
  {
    switch(logging_handler_.log_record())
    {
    case -1: return -1; 
    //case 0: return 0; 
    default: continue; 
    }
  }

  return 0; 
}
