// RT_Thread_Per_Connection_Logging_Server.h: interface for the RT_Thread_Per_Connection_Logging_Server class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RT_THREAD_PER_CONNECTION_LOGGING_SERVER_H__18AE37B6_8921_4366_9098_9326CA73071D__INCLUDED_)
#define AFX_RT_THREAD_PER_CONNECTION_LOGGING_SERVER_H__18AE37B6_8921_4366_9098_9326CA73071D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Thread_Per_Connection_Logging_Server.h"

class RT_Thread_Per_Connection_Logging_Server : public Thread_Per_Connection_Logging_Server  
{
public:
    virtual int open(u_short logger_port = 0); 
    virtual int handle_data(ACE_SOCK_Stream* = 0); 

};

#endif // !defined(AFX_RT_THREAD_PER_CONNECTION_LOGGING_SERVER_H__18AE37B6_8921_4366_9098_9326CA73071D__INCLUDED_)
