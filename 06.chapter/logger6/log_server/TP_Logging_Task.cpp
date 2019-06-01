#include "stdafx.h"
#include "TP_Logging_Task.h"
#include "TP_Logging_Handler.h" 
#include "Logging_Handler.h" 

TP_Logging_Task::TP_Logging_Task(void)
{
}

TP_Logging_Task::~TP_Logging_Task(void)
{
}

int TP_Logging_Task::open(void *)
{
  return activate(THR_NEW_LWP, MAX_THREADS); 
}

int TP_Logging_Task::put(ACE_Message_Block *mblk, ACE_Time_Value *timeout /* = 0*/)
{
  return putq(mblk, timeout); 
}

int TP_Logging_Task::svc()
{
  for(ACE_Message_Block *log_blk; getq(log_blk) != -1; )
  {
    //printf("TP_Logging_Task::svc\n"); 
    TP_Logging_Handler *tp_handler = ACE_reinterpret_cast(TP_Logging_Handler*, log_blk->rd_ptr()); 
    Logging_Handler logging_handler(tp_handler->log_file()); 
    logging_handler.write_log_record(log_blk->cont()); 
    log_blk->release(); 
    tp_handler->handle_close(ACE_INVALID_HANDLE, 0); 
  }

  return 0; 
}