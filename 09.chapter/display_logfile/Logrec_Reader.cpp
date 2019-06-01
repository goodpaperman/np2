#include "StdAfx.h"
#include "Logrec_Reader.h"
#include "ace/FILE_Addr.h" 
#include "ace/FILE_Connector.h" 
#include "ace/CDR_Stream.h" 

Logrec_Reader::Logrec_Reader(const ACE_TString &file)
: filename_(file) 
{
}

Logrec_Reader::~Logrec_Reader(void)
{
}

int Logrec_Reader::open(void *)
{
  ACE_FILE_Addr name(filename_.c_str()); 
  ACE_FILE_Connector con; 
  if(con.connect(logfile_, name) == -1)
    return -1; 

  return activate(); 
}

int Logrec_Reader::svc()
{
  const size_t FILE_READ_SIZE = 8 * 1024; 
  ACE_Message_Block mblk(FILE_READ_SIZE); 

  for(;;mblk.crunch())
  {
    ssize_t bytes_read = logfile_.recv(mblk.wr_ptr(), mblk.space()); 
    if(bytes_read <= 0)
      break; 

    mblk.wr_ptr(ACE_static_cast(size_t, bytes_read)); 
    for(;;)
    {
      size_t name_len = ACE_OS_String::strnlen(mblk.rd_ptr(), mblk.length()); 
      if(name_len == mblk.length())
        break; 

      char *name_p = mblk.rd_ptr(); 
      ACE_Message_Block *rec = 0, *head = 0, *temp = 0; 
      ACE_NEW_RETURN(head, ACE_Message_Block(name_len, MB_CLIENT), 0); 
      head->copy(name_p, name_len); 
      mblk.rd_ptr(name_len + 1); 

      size_t need = mblk.length() + ACE_CDR::MAX_ALIGNMENT; 
      ACE_NEW_RETURN(rec, ACE_Message_Block(need), 0); 
      ACE_CDR::mb_align(rec); 
      rec->copy(mblk.rd_ptr(), mblk.length()); 

      ACE_InputCDR cdr(rec); 
      rec->release(); 
      ACE_CDR::Boolean byte_order; 
      if(!cdr.read_boolean(byte_order))
      {
        head->release(); 
        mblk.rd_ptr(name_p); 
        break; 
      }

      cdr.reset_byte_order(byte_order); 
      ACE_CDR::ULong length; 
      if(!cdr.read_ulong(length) || length > cdr.length()) 
      {
        head->release(); 
        mblk.rd_ptr(name_p); 
        break; 
      }

      ACE_NEW_RETURN(temp, ACE_Message_Block(length, MB_TEXT), 0); 
      ACE_NEW_RETURN(temp, ACE_Message_Block(2 * sizeof(ACE_CDR::Long), MB_TIME, temp), 0); 
      ACE_NEW_RETURN(temp, ACE_Message_Block(sizeof(ACE_CDR::Long), MB_PID, temp), 0); 
      ACE_NEW_RETURN(temp, ACE_Message_Block(sizeof(ACE_CDR::Long), MB_TYPE, temp), 0); 
      head->cont(temp); 

      ACE_CDR::Long *lp = ACE_reinterpret_cast(ACE_CDR::Long *, temp->wr_ptr()); 
      cdr >> *lp; 
      temp->wr_ptr(sizeof(ACE_CDR::Long)); 
      temp = temp->cont(); 

      lp = ACE_reinterpret_cast(ACE_CDR::Long *, temp->wr_ptr()); 
      cdr >> *lp; 
      temp->wr_ptr(sizeof(ACE_CDR::Long)); 
      temp = temp->cont(); 

      lp = ACE_reinterpret_cast(ACE_CDR::Long *, temp->wr_ptr()); 
      cdr >> *lp; 
      ++ lp; 
      cdr >> *lp; 
      temp->wr_ptr(2 * sizeof(ACE_CDR::Long)); 
      temp = temp->cont(); 

      ACE_CDR::ULong text_len; 
      cdr >> text_len; 
      cdr.read_char_array(temp->wr_ptr(), text_len); 
      temp->wr_ptr(text_len); 

      if(put_next(head) == -1)
        break; 

      mblk.rd_ptr(mblk.length() - cdr.length()); 
    }
  }

  ACE_Message_Block *stop = 0; 
  ACE_NEW_RETURN(stop, ACE_Message_Block(0, ACE_Message_Block::MB_STOP), 0); 
  put_next(stop); 
  return 0; 
}