#include "StdAfx.h"
#include "Logrec_Writer.h"

Logrec_Writer::Logrec_Writer(void)
{
}

Logrec_Writer::~Logrec_Writer(void)
{
}

int Logrec_Writer::open(void *)
{
  return activate(); 
}

int Logrec_Writer::put(ACE_Message_Block *mblk, ACE_Time_Value *to)
{
  return putq(mblk, to); 
}

int Logrec_Writer::svc()
{
  int stop = 0; 
  for(ACE_Message_Block *mb; !stop && getq(mb) != -1; )
  {
    if(mb->msg_type() == ACE_Message_Block::MB_STOP)
      stop = 1; 
    else 
      ACE::write_n(ACE_STDOUT, mb); 

    put_next(mb); 
  }

  return 0; 
}