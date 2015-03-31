//stat.h - declaration of Statistic 2.0 class 09-07-09 20:17
#ifndef STAT_H_
#define STAT_H_

#include "calendar.h"
#include "dic_excp.h"
#include "stopwatch.h"
#include <iostream>
#include <string>
#include <vector>

namespace sta {
   class FormatedTime {
      private:
         static const int TIME_UNIT = 60;
         int m_Hours;
         int m_Minutes;
         int m_Seconds;
      public:
         explicit FormatedTime(long secs);               
         friend std::ostream & operator<<(std::ostream &, const FormatedTime &);
   };
   
   class BaseStat {        
      private:        
         PWJ::DateTime * m_pDate; 
      protected:
         virtual void Data() const;
         virtual void Write(std::ostream &) const; 
         virtual void Read(std::istream &) throw(excp::bad_file); 
         virtual void Reset();
         virtual void StartTime() = 0;
         virtual void StopTime() = 0;
         virtual long GetRunningTime() const = 0;                                        
      public:
         BaseStat();
         BaseStat(const std::tm * p_Tm);
         BaseStat(const BaseStat &);
         BaseStat & operator=(const BaseStat &);
         virtual ~BaseStat();
         virtual void Save(std::ostream &) const = 0;  
         virtual void Load(std::istream &) throw(excp::bad_file) = 0;         
         virtual void ShowAll() const = 0;         
         const PWJ::DateTime & GetDate() const { return *m_pDate; }
         friend std::ostream & operator<<(std::ostream &, const BaseStat &);
   };
   
   class LearningStat : virtual public BaseStat {
      private:
         stp::StopWatch * m_pStops;
         long m_MeanTime;
         int m_LearnedVerbs;
         int m_DictPosition;
         bool m_Stopping;
      protected:
         virtual void Data() const;
         virtual void Write(std::ostream &) const;
         virtual void Read(std::istream &) throw(excp::bad_file);
         virtual void Reset();         
         virtual void StartTime();
         virtual void StopTime();
         virtual long GetRunningTime() const;
      public:
         LearningStat();
         LearningStat(const std::tm * p_Tm, long mt, int lv, int dp);
         LearningStat(const LearningStat &);         
         LearningStat & operator=(const LearningStat &);
         virtual ~LearningStat();                 
         void OneLearned(int pos);
         int Position() const { return m_DictPosition; }
         int & Position() { return m_DictPosition; }
   };
   
   class TestingStat : virtual public BaseStat {
      private:
         stp::StopWatch * m_pStops;         
         std::vector<std::string> m_IncorrectAnswers;
         long m_MeanTime;
         int m_IncorrectTries;
         int m_OverallTries;
         bool m_Stopping;
      protected:
         virtual void Write(std::ostream &) const;
         virtual void Read(std::istream &) throw(excp::bad_file);
         virtual void Data() const;
         virtual void Reset();
         virtual void StartTime();
         virtual void StopTime();
         virtual long GetRunningTime() const;
      public:
         TestingStat();
         TestingStat(const std::tm * p_Tm, long mt, int it, int ot,
                     const std::vector<std::string> & ia);
         TestingStat(const TestingStat &);
         TestingStat & operator=(const TestingStat &);
         virtual ~TestingStat();                 
         void OneAnswer(const std::string & ans = ""); 
         const std::vector<std::string> & GetAnswers() const 
               { return m_IncorrectAnswers; } 
         void TestResult() const { TestingStat::Data(); }  
         void CorrectWord(const std::string & word);     
   };
   
   class WorkingStat : virtual public BaseStat {
      private:
         stp::StopWatch * m_pStops;
         long m_MeanTime;
         bool m_Stopping;
      protected:
         virtual void Write(std::ostream &) const;
         virtual void Read(std::istream &) throw(excp::bad_file);
         virtual void Data() const;
         virtual void Reset();
         virtual void StartTime();
         virtual void StopTime();
         virtual long GetRunningTime() const;
      public:
         WorkingStat();
         WorkingStat(const std::tm * p_Tm, long mt);
         WorkingStat(const WorkingStat &);
         WorkingStat & operator=(const WorkingStat &);
         virtual ~WorkingStat();       
   }; 
   
   class Statistic : public LearningStat, 
                     public TestingStat, public WorkingStat {
      protected:
         virtual void Write(std::ostream &) const;
         virtual void Read(std::istream &) throw(excp::bad_file);
         virtual void Data() const;
         virtual void Reset();
         virtual void StartTime();
         virtual void StopTime();
         virtual long GetRunningTime() const;                 
      public:
         enum {LEARNING, TESTING, WORKING, ALL};
         Statistic() {}
         Statistic(std::tm * p_Tm, long l_mt, int l_lv, int l_dp,
                   long t_mt, int t_it, int t_ot, 
                   const std::vector<std::string> & t_ia, long w_mt)
                   : BaseStat(p_Tm), LearningStat(p_Tm, l_mt, l_lv, l_dp),
                     TestingStat(p_Tm, t_mt, t_it, t_ot, t_ia), 
                     WorkingStat(p_Tm, w_mt) {}  
         virtual void Save(std::ostream &) const; 
         virtual void Load(std::istream &) throw(excp::bad_file);          
         virtual void ShowAll() const; 
         void StartTiming(int which);
         void StopTiming(int which);
         long GetRunTime(int which) const;  
         void ResetStat(int which);                    
            
   };
   
} //end of namespace sta

#endif // STAT_H_   
