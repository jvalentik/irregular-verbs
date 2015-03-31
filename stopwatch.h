//stopwatch.h - definition of StopWatch class 2.0 03-07-09 22:52
#ifndef STOPWATCH_H_
#define STOPWATCH_H_

#include <ctime>
#include <iostream>

namespace stp {      
   class StopWatch {              
      private:
         time_t m_InitialTime;                                           
         time_t m_AdditionalTime;
      public:                    
         StopWatch(); 
         explicit StopWatch(long sec);         
         StopWatch operator+(const StopWatch & s) const;                
         long Reset(long sec = 0);
         long GetTimer() const;                                
   };
   
   void Wait(int secs) throw();   
} //end of stp namespace

#endif // STOPWATCH_H_
