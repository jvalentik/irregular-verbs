//calendar.h - latest version of Calendar classes 3.0 12-06-09 20:57
#ifndef CALENDAR_H_
#define CALENDAR_H_

#include <bitset>
#include <ctime>
#include <iostream>
#include <new>
#include <stdexcept>

namespace PWJ {
   typedef unsigned UINT;
   
   class Calendar {                               
      protected:
         std::tm * m_pTm; 
         std::bitset<2> m_Format;                                 
      public: 
         class invalid_date : public std::logic_error {
            private:
               int val1;
               int val2;
               int val3;               
            public:
               invalid_date(int a, int b, int c, 
                            const char * s = "Neplatny datum: ")
                   throw() : std::logic_error(s), val1(a), val2(b), val3(c) {}
               ~invalid_date() throw () {}
               void report() const throw();
         };                             
         Calendar(UINT day = 1, UINT month =1, UINT year =1970, UINT hour =1,
              UINT min = 0, UINT sec = 0) throw(invalid_date, std::bad_alloc);
         Calendar(const std::tm * const t) throw(std::bad_alloc);
         Calendar(const Calendar & c) throw(std::bad_alloc);
         virtual ~Calendar() throw() = 0;
         enum {short_no_year, short_format, long_no_year, long_format};
         Calendar & operator=(const Calendar & c) throw(std::bad_alloc);
         void WriteAll(std::ostream & os) const throw(); 
         virtual void Setf(int f = long_format) throw();
         virtual bool IsValid() const throw() {return true;}        
         virtual void SetAll(std::istream & is) 
                 throw(invalid_date, std::bad_alloc);
         virtual void ShowAll(std::ostream & os) const throw();
         friend std::ostream & operator<<(std::ostream & os,
                                          const Calendar & c);
         friend std::istream & operator>>(std::istream & is,
                                          Calendar & c);
   };
   
   class Date : virtual public Calendar {
      private:
         bool m_IsLeap;         
         char * m_MonthName;
         static const int MONTHS = 12;
         static const char * const MONTHS_NAMES[MONTHS];
      protected:
         virtual bool IsValid() const throw();                                       
      public:         
         explicit Date(unsigned d = 1, unsigned m = 1, 
                  unsigned y = 1970) throw(invalid_date, std::bad_alloc); 
         Date(const std::tm * const t) throw(std::bad_alloc);        
         Date(const Date & d) throw(std::bad_alloc);
         virtual ~Date() throw();                
         Date & operator=(const Date & d) throw(std::bad_alloc);
         UINT operator-(const Date & d) const;
         Date operator-(UINT days) const;
         Date operator+(UINT days) const;
         Date operator++() const;
         Date operator--() const;
         bool operator>(const Date & bDate) const;
         bool operator<(const Date & bDate) const;
         bool operator==(const Date & bDate) const;         
         bool GetLeap() const { return m_IsLeap; }                 
         virtual void SetAll(std::istream & is) 
                 throw(invalid_date, std::bad_alloc);
         virtual void ShowAll(std::ostream & os) const throw();
   };
   
   class CurrentDate : public Date {
      private:
         char * m_DayName;
         static const int DAYS = 7;
         static const char * const DAYS_NAMES[DAYS];
         const std::tm * const Set();      
      public:
         CurrentDate() throw(std::bad_alloc);
         ~CurrentDate() throw();
         virtual void SetAll(std::istream & is) 
                      throw(invalid_date, std::bad_alloc);
         virtual void ShowAll(std::ostream & os) const throw();
   };
      
   class Time : virtual public Calendar {                        
      protected:
         virtual bool IsValid() const throw();         
      public:
         explicit Time(UINT h = 0, UINT m = 0, UINT s = 0)
                  throw(invalid_date, std::bad_alloc);
         Time(const Time & t) throw();
         Time(const std::tm * const t) throw();
         virtual ~Time() throw();               
         Time & operator=(const Time & t) throw();
         double operator-(const Time & t) const;
         Time operator-(UINT secs) const;
         Time operator+(UINT secs) const;
         bool operator>(const Time & bTime) const;
         bool operator<(const Time & bTime) const;
         bool operator==(const Time & bTime) const;                          
         virtual void SetAll(std::istream & is) 
                 throw(invalid_date, std::bad_alloc);
         virtual void Setf(int f = Calendar::long_format) throw();
         virtual void ShowAll(std::ostream & os) const throw();  
         int GetHour() const { return m_pTm->tm_hour; }
         int GetMin() const { return m_pTm->tm_min; }
         int GetSec() const { return m_pTm->tm_sec; }       
   };
   
   class CurrentTime : public Time {
      private:
         const std::tm * const Set();
      public:
         CurrentTime();
         ~CurrentTime() throw();
         virtual void SetAll(std::istream & is) throw();         
   };
   
   class DateTime : public Date, public Time {    
         bool IsValid() const throw();  
      public:         
         DateTime(const Date & d, const Time & t) throw();
         DateTime(const std::tm * const t) throw();         
         DateTime(const DateTime & c) throw();         
         ~DateTime() throw();
         DateTime & operator=(const DateTime & dt);
         bool operator>(const DateTime & bDT) const;
         bool operator<(const DateTime & bDT) const;
         bool operator==(const DateTime & bDT) const;         
         virtual void ShowAll(std::ostream & os) const throw();
         virtual void SetAll(std::istream & is) throw();
         virtual void Setf(int f = Calendar::long_format) throw();
   };  
} // end of namespace PWJ

#endif // CALENDAR_H_
