#include "StdAfx.h"
#include "Logrec_Formatter.h"
#include "Logrec_Reader.h" 
#include "ace/CDR_Stream.h" 
#include "ace/os_ns_time.h" 

static void format_null(ACE_Message_Block *) { return; } 
static void format_long(ACE_Message_Block *mblk)
{
  ACE_CDR::Long type = *(ACE_CDR::Long *)mblk->rd_ptr(); 
  mblk->size(11); 
  mblk->reset(); 
  mblk->wr_ptr((size_t) sprintf(mblk->wr_ptr(), "%d", type)); 
}

static void format_time(ACE_Message_Block *mblk)
{
  ACE_CDR::Long secs = *(ACE_CDR::Long *)mblk->rd_ptr(); 
  mblk->rd_ptr(sizeof(ACE_CDR::Long)); 
  ACE_CDR::Long usecs = *(ACE_CDR::Long *)mblk->rd_ptr(); 

  char timestamp[26]; 
  time_t time_secs(secs); 
  ACE_OS::ctime_r(&time_secs, timestamp, sizeof timestamp); 
  mblk->size(26); 
  mblk->reset(); 
  timestamp[19] = 0; 
  timestamp[24] = 0; 
  size_t fmt_len = sprintf(mblk->wr_ptr(), "%s.%03d %s", timestamp + 4, usecs/1000, timestamp+20); 
  mblk->wr_ptr(fmt_len); 
}

Logrec_Formatter::FORMATTER Logrec_Formatter::format_ = 
{
  format_null, 
  format_long, 
  format_long, 
  format_time, 
  format_null, 
}; 

Logrec_Formatter::Logrec_Formatter(void)
{
}

Logrec_Formatter::~Logrec_Formatter(void)
{
}

int Logrec_Formatter::put(ACE_Message_Block *mblk, ACE_Time_Value *)
{
  if(mblk->msg_type() == Logrec_Reader::MB_CLIENT)
  {
    for(ACE_Message_Block *temp = mblk; 
      temp != 0; 
      temp = temp->cont())
    {
      int mb_type = temp->msg_type() - ACE_Message_Block::MB_USER; 
      (*format_[mb_type])(temp); 
    }
  }

  return put_next(mblk); 
}
