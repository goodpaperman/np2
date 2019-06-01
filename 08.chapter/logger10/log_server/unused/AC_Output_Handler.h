#pragma once

class AC_CLD_Connector; 
class AC_Output_Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_MT_SYNCH>
{
public:
  enum { QUEUE_MAX = sizeof(ACE_Log_Record) * ACE_IOV_MAX }; 

  AC_Output_Handler(void);
  virtual ~AC_Output_Handler(void);
  virtual int open(void *); 
  virtual int put(ACE_Message_Block *, ACE_Time_Value * = 0); 

protected:
  virtual int handle_input(ACE_HANDLE handle); 
  virtual int svc(); 
  virtual int send(ACE_Message_Block *chunk[], size_t &count); 

  AC_CLD_Connector *connector_; 
};

#if !defined (FLUSH_TIMEOUT)
#define FLUSH_TIMEOUT 120
#endif 
