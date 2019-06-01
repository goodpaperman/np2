#include "StdAfx.h"
#include "Logrec_Separator.h"
#include "Logrec_Reader.h" 

Logrec_Separator::Logrec_Separator(void)
{
}

Logrec_Separator::~Logrec_Separator(void)
{
}

int Logrec_Separator::put(ACE_Message_Block *mblk, ACE_Time_Value*)
{
  if(mblk->msg_type() == Logrec_Reader::MB_CLIENT)
  {
    ACE_Message_Block *separator = 0; 
    ACE_NEW_RETURN(separator, ACE_Message_Block(ACE_OS_String::strlen("|")+1, ACE_Message_Block::MB_DATA, 0, 0, 0, &lock_strategy_), -1); 
    separator->copy("|"); 

    ACE_Message_Block *dup = 0; 
    for(ACE_Message_Block *temp = mblk; temp != 0;)
    {
      dup = separator->duplicate(); 
      dup->cont(temp->cont()); 
      temp->cont(dup); 
      temp = dup->cont(); 
    }

    ACE_Message_Block *nl = 0; 
    ACE_NEW_RETURN(nl, ACE_Message_Block(2), 0); 
    nl->copy("\n"); 
    dup->cont(nl); 
    separator->release(); 
  }

  return put_next(mblk); 
}