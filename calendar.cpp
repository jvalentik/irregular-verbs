//calendar.cpp - implementations of methods for Calendar classes 21-05-09 18:18
#include "calendar.h"
#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>

namespace PWJ { 
   using std::tm;
   using std::strlen;
   using std::strcpy;
   using std::mktime;
   using std::difftime;
   using std::localtime;
   using std::abs;
   using std::cin;
   using std::cout;
   using std::endl;
         
   namespace { 
      const int DAYS_PER_YEAR(365);
      const int SECS_PER_HOUR(3600);
      const int HOURS_PER_DAY(24);      
      const int DAYS_PER_MONTH(30);
      const int MONTHS_PER_YEAR(12);                      
      
      template <typename T>
      T readItem(const char * prompt = ":", std::istream & in = cin) 
                            throw(std::invalid_argument) {
         T temp;
         if (in == cin) {
            cout << prompt;       		      
            while (!(in >> temp)) {		            
               in.clear();
               while (in.get() != '\n')
                  continue;            
               cout << "Neplatne zadanie! Opakuj.\n";
               cout << prompt;                               
            }            
            while (in.get() != '\n') 
               continue;           
         }
         else {            
            in >> temp;           
            if (in.fail() && !in.eof()) {                                            
               throw std::invalid_argument("Neplatny format suboru!\n");                             
            }
            while (!isspace(in.get())) {
                  continue;
            }
         }		  
		   return temp;
	   }
   } //end of unnamed namespace
         
   
   void Calendar::invalid_date::report() const throw() {
      cout << std::logic_error::what() << val1 
           << ". " << val2 << ". " << val3 << std::endl;      
   } 
   
   Calendar::Calendar(UINT day, UINT mon, UINT year, UINT hour, 
                      UINT min, UINT sec) throw(invalid_date, std::bad_alloc) {
      m_pTm = new tm;
      tm temp = {sec, min, hour, day, mon-1, year - 1900, 0, 0, 0};
      *m_pTm = temp;
      m_Format.set();
   }
   
   Calendar::Calendar(const tm * const t) throw(std::bad_alloc) {
      m_pTm = new tm;      
      *m_pTm = *t;
      m_Format.set();
   }
   
   Calendar::Calendar(const Calendar & c) throw(std::bad_alloc) {
      m_pTm = new tm;
      *m_pTm = *c.m_pTm;
      m_Format = c.m_Format;
   }
   
   Calendar::~Calendar() throw() {
      delete m_pTm;
      m_pTm = NULL;
   }
   
   Calendar & Calendar::operator=(const Calendar & c) throw(std::bad_alloc) {
      if (this != &c) {
         delete m_pTm;
         m_pTm = new tm;
         *m_pTm = *c.m_pTm;
         m_Format = c.m_Format;         
      }
      return *this;
   }
   
   void Calendar::WriteAll(std::ostream & os) const throw() {      
      os.write((char *) m_pTm, sizeof(tm));      
   }      
   
   void Calendar::Setf(int f) throw() {
        switch (f) {
           case long_format:                           
              m_Format = long_format;                        
              break;
           case long_no_year:              
              m_Format = long_no_year;              
              break;
           case short_format:              
              m_Format = short_format;              
              break;
           case short_no_year:                
              m_Format = short_no_year;              
              break;
           default:
              std::cerr << "Neplatny formatovaci znak!";              
        }
   }
     
   void Calendar::SetAll(std::istream & is) 
        throw(invalid_date, std::bad_alloc) {
      UINT day, mon, year, hour, min, sec;
      try {
         day = readItem<UINT>("Zadaj den: ", is);
         mon = readItem<UINT>("Zadaj mesiac: ", is);
         year = readItem<UINT>("Zadaj rok: ", is);
         hour = readItem<UINT>("Zadaj hodinu: ", is);
         min = readItem<UINT>("Zadaj minutu: ", is);
         sec = readItem<UINT>("Zadaj sekundu: ", is);
         if (mon < 0 || mon >= 12)
            throw invalid_date(day, mon, year);         
      }
      catch (std::invalid_argument & ia) {
         std::cerr << ia.what() << "\nDatum nezmeneny!\n";
         return;
      }
      tm temp = { sec, min, hour, day, mon - 1, year - 1900, 0, 0, 0};
      *m_pTm = temp;
   }
   
   void Calendar::ShowAll(std::ostream & os) const throw() {
      os << m_pTm->tm_mday << ". " << m_pTm->tm_mon + 1
           << ". " << m_pTm->tm_year + 1900 << ", "
           << m_pTm->tm_hour << ":" << m_pTm->tm_min
           << ":" << m_pTm->tm_sec;
   }
   
   std::ostream & operator<<(std::ostream & os, const Calendar & c) {
      c.ShowAll(os);
      return os;
   }
   
   std::istream & operator>>(std::istream & is, Calendar & c) {
      c.SetAll(is);
      return is;
   }   
   
   const char * const Date::MONTHS_NAMES[Date::MONTHS] = {
            "januar", "februar", "marec", "april",
            "maj", "jun", "jul", "august", "september",
            "oktober", "november", "december"
   };  
   
   Date::Date(unsigned d, unsigned m, unsigned y) 
         throw(invalid_date, std::bad_alloc) : Calendar(d, m, y) { 
      if (m < 0 || m >= MONTHS + 1)
         throw invalid_date(d, m, y); 
      if (!IsValid())
         throw invalid_date(d, m, y);           
      m_MonthName = new char[strlen(MONTHS_NAMES[m-1]) + 1];
      strcpy(m_MonthName, MONTHS_NAMES[m-1]); 
      m_IsLeap = (y % 4 == 0) ? true : false;      
   }
   
   Date::Date(const tm * const t) throw(std::bad_alloc) : Calendar(t)  {      
      m_MonthName = new char[strlen(MONTHS_NAMES[m_pTm->tm_mon]) + 1];
      strcpy(m_MonthName, MONTHS_NAMES[m_pTm->tm_mon]); 
      m_IsLeap = (m_pTm->tm_year % 4 == 0) ? true : false;      
   }
   
   Date::Date(const Date & d) throw(std::bad_alloc) : Calendar(d) {      
      m_MonthName = new char[strlen(d.m_MonthName) + 1];
      strcpy(m_MonthName, d.m_MonthName);
      m_IsLeap = d.m_IsLeap;      
   }
   
   Date::~Date() throw() {       
      delete [] m_MonthName;
   }
   
   Date & Date::operator=(const Date & d) throw(std::bad_alloc) {
      if (this != &d) {
         Calendar::operator=(d);     
         delete [] m_MonthName;
         m_MonthName = new char[strlen(d.m_MonthName) + 1];
         strcpy(m_MonthName, d.m_MonthName);
         m_IsLeap = d.m_IsLeap;         
      }
      return *this;
   }
   
   UINT Date::operator-(const Date & d) const {          
      time_t const timer1(mktime(m_pTm));
      time_t const timer2(mktime(d.m_pTm));
      double difference(difftime(timer2, timer1));      
      //convert from secs to days
      difference = difference / SECS_PER_HOUR / HOURS_PER_DAY;
      return abs(int(difference));           
   }
   
   Date Date::operator-(UINT d) const {                
        time_t timer(mktime(m_pTm));
        time_t timer2(d * HOURS_PER_DAY * SECS_PER_HOUR);
        time_t res = timer - timer2; 
        time_t * const p_res = &res;
        tm * new_date = localtime(p_res);
        return Date(new_date);         
   }
   
   Date Date::operator+(UINT d) const {                
        time_t timer(mktime(m_pTm));
        time_t timer2(d * HOURS_PER_DAY * SECS_PER_HOUR);
        time_t res = timer + timer2; 
        time_t * const p_res = &res;
        tm * new_date = localtime(p_res);
        return Date(new_date);
   }
   
   Date Date::operator++() const {
      return operator+(1);
   }
   
   Date Date::operator--() const {
      return operator-(1);
   }
   
   bool Date::operator>(const Date & bDate) const {
        if (m_pTm->tm_year > bDate.m_pTm->tm_year) {
           return true;
        }        
        if ((m_pTm->tm_year == bDate.m_pTm->tm_year) &&
            (m_pTm->tm_mon > bDate.m_pTm->tm_mon)) {
           return true;
        }
        if ((m_pTm->tm_year == bDate.m_pTm->tm_year) &&
            (m_pTm->tm_mon == bDate.m_pTm->tm_mon) && 
             m_pTm->tm_mday > bDate.m_pTm->tm_mday) {
           return true;
        }
        return false;
   }
   
   bool Date::operator<(const Date & bDate) const {
        if (*this > bDate)
           return false;
        else if (*this == bDate)
           return false;
        else         
           return true;
   }        
   
   bool Date::operator==(const Date & bDate) const {
        return ((m_pTm->tm_year == bDate.m_pTm->tm_year) &&
                (m_pTm->tm_mon == bDate.m_pTm->tm_mon) && 
                (m_pTm->tm_mday == bDate.m_pTm->tm_mday));
   }
   
   bool Date::IsValid() const throw() {
      if(m_pTm->tm_mday > 31 || m_pTm->tm_mday < 1) return false;
      if(m_pTm->tm_mon + 1 > 12) return false;
      if(m_pTm->tm_year + 1900  < 1970) return false;
      if(m_pTm->tm_year + 1900  > 2037) return false;
      if (((m_pTm->tm_mon + 1==4) || (m_pTm->tm_mon + 1==6) || 
           (m_pTm->tm_mon+1 ==9) || (m_pTm->tm_mon + 1==11))
            && (m_pTm->tm_mday == 31)) return false;      
      if ((m_pTm->tm_mon + 1 == 2) && (m_pTm->tm_mday > 29)) return false;         
      if ((m_pTm->tm_mon + 1 == 2) && (m_pTm->tm_mday > 28) 
           && (!m_IsLeap)) return false;
      return true;                
   }
     
   void Date::SetAll(std::istream & is) throw(invalid_date, std::bad_alloc) {
      UINT day, mon, year;
      try {
         day = readItem<UINT>("Zadaj den: ", is);              
         mon = readItem<UINT>("Zadaj mesiac: ", is);                        
         year = readItem<UINT>("Zadaj rok: ", is);            
         if (mon < 1 || mon >= MONTHS + 1)
            throw invalid_date(day, mon, year);
      }
      catch (std::invalid_argument & ia) {
         std::cerr << ia.what();         
         return;
      }
      
      tm temp = {0, 0, 1, day, mon-1, year - 1900, 0, 0, 0};  
      Date tmp(&temp);
      if (!tmp.IsValid())
         throw invalid_date(day, mon, year);          
      *m_pTm = temp;              
      delete [] m_MonthName;
      m_MonthName = new char[strlen(MONTHS_NAMES[mon-1]) + 1];
      strcpy(m_MonthName, MONTHS_NAMES[mon-1]); 
      m_IsLeap = (year % 4 == 0) ? true : false;
   }
   
   void Date::ShowAll(std::ostream & os) const throw() {
      if (m_Format[0] == true && m_Format[1] == true) { // long_format
         os << m_pTm->tm_mday << ". " << m_MonthName
            << " " << m_pTm->tm_year + 1900;
      }         
      else if (m_Format[0] == true && m_Format[1] == false) {//long_no_year
         os << m_pTm->tm_mday << ". " << m_MonthName;
      }
      else if (m_Format[0] == false && m_Format[1] == true) { //short_format         
         os << m_pTm->tm_mday << ". " << m_pTm->tm_mon + 1
            << ". " << m_pTm->tm_year + 1900;   
      }
      else if (m_Format[0] == false && m_Format[1] == false) { //short_no_year
         os << m_pTm->tm_mday << ". " << m_pTm->tm_mon + 1
            << '.';  
      }    
   }      
   
   const char * const CurrentDate::DAYS_NAMES[CurrentDate::DAYS] = {
            "nedela", "pondelok", "utorok",
            "streda", "stvrtok", "piatok",
            "sobota"            
   };   
   
   CurrentDate::CurrentDate() throw(std::bad_alloc)
               : Calendar(Set()), Date(Set()) {
      m_DayName = new char[strlen(CurrentDate::DAYS_NAMES[m_pTm->tm_wday]) + 1];
      strcpy(m_DayName, CurrentDate::DAYS_NAMES[m_pTm->tm_wday]);      
   }
   
   CurrentDate::~CurrentDate() throw() {
      delete [] m_DayName;
   }
   
   const tm * const CurrentDate::Set() {
      time_t timer = std::time(0);//time() returns time in secs
      time_t * pTimer = &timer;
      const tm * const today(localtime(pTimer));         
      return today;
   }
   
   void CurrentDate::ShowAll(std::ostream & os) const throw() {
      os << m_DayName << ", ";
      Date::ShowAll(os);
   }   
   
   void CurrentDate::SetAll(std::istream & is) 
                            throw(invalid_date, std::bad_alloc) {
      std::cerr << "Operacia nie je mozna!\n";
   }
   
   Time::Time(UINT h, UINT m, UINT s) throw(invalid_date, std::bad_alloc)
        : Calendar(1, 1, 1970, h, m, s) {
      if (!IsValid())
         throw invalid_date(h, m, s, "Neplatny cas!");
   }
   
   Time::~Time() throw() {}
   
   Time::Time(const Time & t) throw() : Calendar(t) {}
   
   Time::Time(const tm* const t) throw() : Calendar(t) {}
   
   Time & Time::operator=(const Time & t) throw() {
      if (this != &t) {
         Calendar::operator=(t);
      }
      return *this;
   }
   
   double Time::operator-(const Time & bTime) const {
      time_t const timer1(mktime(m_pTm));
      time_t const timer2(mktime(bTime.m_pTm));
      double difference(difftime(timer2, timer1));            
      return abs(difference); //returns in sec     
   }  
   
   Time Time::operator-(UINT secs ) const {
      time_t timer(mktime(m_pTm));
      time_t timer2(secs);
      time_t res = timer - timer2; 
      time_t * const p_res = &res;
      tm * new_time = localtime(p_res);
      return Time(new_time);
   }
   
   Time Time::operator+(UINT secs) const {
      time_t timer(mktime(m_pTm));
      time_t timer2(secs);
      time_t res = timer + timer2; 
      time_t * const p_res = &res;
      tm * new_time = localtime(p_res);
      return Time(new_time);
   }
   
   bool Time::operator>(const Time & bTime) const {
      if (m_pTm->tm_hour > bTime.m_pTm->tm_hour) 
         return true;
      if (m_pTm->tm_hour == bTime.m_pTm->tm_hour 
          && m_pTm->tm_min > bTime.m_pTm->tm_min)
          return true;
      if (m_pTm->tm_hour == bTime.m_pTm->tm_hour
          && m_pTm->tm_min == bTime.m_pTm->tm_min
          && m_pTm->tm_sec > bTime.m_pTm->tm_sec)
          return true;
      return false;
   }
    
   bool Time::operator<(const Time & bTime) const {
      if (*this > bTime) 
         return false;
      if (*this == bTime)
         return false;
      else 
         return true;   
   }
         
   bool Time::operator==(const Time & bTime) const {
      if (m_pTm->tm_hour == bTime.m_pTm->tm_hour
          && m_pTm->tm_min == bTime.m_pTm->tm_min
          && m_pTm->tm_sec == bTime.m_pTm->tm_sec)
          return true;
      return false;
   }
   
   bool Time::IsValid() const throw() {
      if ((m_pTm->tm_hour <= 23 && m_pTm->tm_hour >= 0)
          && (m_pTm->tm_min <= 59 && m_pTm->tm_min >= 0)
          && (m_pTm->tm_sec <= 59 && m_pTm->tm_sec >= 0))
         return true;
      return false;
   }                        
   
   void Time::SetAll(std::istream & is) throw(invalid_date, std::bad_alloc) {
      UINT hour, min, sec;
      try {
         hour = readItem<UINT>("Zadaj hodinu: ", is);
         min = readItem<UINT>("Zadaj minutu: ", is);
         sec = readItem<UINT>("Zadaj sekundu: ", is);
      }
      catch (std::invalid_argument & ia) {
         std::cerr << ia.what() << "Cas nezmeneny!\n";
         return;
      }
      tm temp = {sec, min, hour, 1, 0, 70, 0, 0, 0};
      Time tempTime(&temp);
      if (!tempTime.IsValid())
         throw invalid_date(hour, min, sec, "Neplatny cas!");
      *m_pTm = temp;
   }
   
   void Time::Setf(int f) throw() {
      switch (f) {
           case Calendar::long_format:                           
              m_Format = long_format;                        
              break;           
           case Calendar::short_format:              
              m_Format = short_format;              
              break;           
           default:
              std::cerr << "Neplatny formatovaci znak!";              
        }
   }
      
      
   void Time::ShowAll(std::ostream & os) const throw() {
      if (m_Format[0] == true && m_Format[1] == true) { // long_format
         os << m_pTm->tm_hour << ":" << m_pTm->tm_min
            << ":" << m_pTm->tm_sec;
      }
      else { //short_format
         os << m_pTm->tm_hour << ":" << m_pTm->tm_min;
      }      
   } 
   
   const tm * const CurrentTime::Set() {
      time_t timer = std::time(0);//time() returns time in secs
      time_t * pTimer = &timer;
      tm * now(localtime(pTimer));
      now->tm_mday = 1;
      now->tm_mon = 0;
      now->tm_year = 70;
      now->tm_wday = 0;
      now->tm_yday = 0;
	   now->tm_isdst = 0;         
      return now;   
   }
   
   CurrentTime::CurrentTime() : Calendar(Set()), Time(Set()) {}
   
   CurrentTime::~CurrentTime() throw() {}
   
   void CurrentTime::SetAll(std::istream & is) throw() {
      std::cerr << "Operacia nie je mozna!\n";
   }
   
   bool DateTime::IsValid() const throw() {
     if (Date::IsValid() && Time::IsValid())
        return true;
     return false;
   }
     
   DateTime::DateTime(const Date & d, const Time & t) throw() 
            : Calendar(d), Date(d), Time(t) {
      m_pTm->tm_hour = t.GetHour();
      m_pTm->tm_min = t.GetMin();
      m_pTm->tm_sec = t.GetSec();      
   }
   
   DateTime::DateTime(const DateTime & dt) throw()
            : Calendar(dt), Date(dt), Time(dt) {}
            
   DateTime::DateTime(const std::tm * const t) throw()
            : Calendar(t), Date(t), Time(t) {}
   
   DateTime::~DateTime() throw() {}
   
   DateTime & DateTime::operator=(const DateTime & dt) {
      if (this != &dt) {
         Calendar::operator=(dt);
         Date::operator=(dt);
         Time::operator=(dt);
      }
      return *this;
   } 
   
   bool DateTime::operator>(const DateTime & bDT) const {
      if (this->Date::operator>(bDT) == true)
         return true;
      if (this->Date::operator==(bDT) == true 
          && this->Time::operator>(bDT) == true)
         return true;
      return false;      
   }
   
   bool DateTime::operator<(const DateTime & bDT) const {
     if (*this > bDT)
        return false;
     if (*this == bDT)
        return false;
     return true;
   }
   
   bool DateTime::operator==(const DateTime & bDT) const {
      if (this->Date::operator==(bDT) == true 
         && this->Time::operator==(bDT) == true)
         return true;
      return false;
   } 
   
   void DateTime::Setf(int f) throw() {
      switch (f) {
           case Calendar::long_format:                           
              Date::Setf(Calendar::long_format);
              Time::Setf(Calendar::long_format);                        
              break;           
           case Calendar::short_format:              
              Date::Setf(Calendar::short_format);
              Time::Setf(Calendar::short_format);              
              break;           
           default:
              std::cerr << "Neplatny formatovaci znak!";              
        }
   }
   
   void DateTime::ShowAll(std::ostream & os) const throw() {
      Date::ShowAll(os);
      os << " - ";
      Time::ShowAll(os);
   }
   
   void DateTime::SetAll(std::istream & is) throw() {
      Calendar::SetAll(is);
   }
} // namespace PWJ closed
