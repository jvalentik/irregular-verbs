//stopwatch.cpp - methods of StopWatch class 27-06-09 23:07
#include "stopwatch.h"

using std::time;

namespace stp {
   StopWatch::StopWatch() {
      m_InitialTime = time(0);
      m_AdditionalTime = 0;            
   }   
   
   StopWatch::StopWatch(long sec) {
      m_InitialTime = time(0);
      m_AdditionalTime = sec;
   }   
   
   StopWatch StopWatch::operator+(const StopWatch & s) const {
      time_t temp_time1 = GetTimer();
      time_t temp_time2 = s.GetTimer();           
      return StopWatch(temp_time1 + temp_time2);
   }    
   
   long StopWatch::Reset(long sec) {         
      long res = (time(0) - m_InitialTime) + m_AdditionalTime;
      time(&m_InitialTime);
      m_AdditionalTime = sec;
      return res;      
   }
   
   long StopWatch::GetTimer() const {                   
      return (time(0) - m_InitialTime) + m_AdditionalTime;
   }  
   
   void Wait(int secs) throw() {
      clock_t delay = secs * CLOCKS_PER_SEC;
      clock_t start = clock();
      while (clock() - start < delay)
      ;
   }       
} //end of stp namespace
