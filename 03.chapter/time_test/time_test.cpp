// time_test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <iostream> 
using std::cout; 
using std::endl; 

#include "ace/OS.h" 
#include "ace/Get_Opt.h" 

const ACE_Time_Value max_interval(60 * 60); 


int _tmain(int argc, _TCHAR* argv[])
{
  ACE_Time_Value expiration = ACE_OS::gettimeofday(); 
  ACE_Time_Value interval; 

  ACE_Get_Opt opt(argc, argv, "e:i:"); 
  for(int c; (c = opt()) != -1; )
  {
    switch(c)
    {
      case 'e': 
        expiration += ACE_Time_Value(atoi(opt.opt_arg())); 
        break; 
      case 'i': 
        interval = ACE_Time_Value(atoi(opt.opt_arg())); 
        break; 
    }
  }

  cout << "interval = (" << interval.sec() << "," << interval.usec() << ")" << endl; 
  cout << "expiration = (" << expiration.sec() << "," << expiration.usec() << ")" << endl; 

  if(interval > max_interval)
    cout << "interval must be less than " 
         << max_interval.sec() << endl; 
  /*else */if(expiration > (ACE_Time_Value::max_time - interval))
    cout << "expiration + interval must be less than " 
    << ACE_Time_Value::max_time.sec() << endl; 

	return 0;
}

