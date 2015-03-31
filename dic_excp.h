//dic_excp.h - declaration of exception classes 0.1 16-06-09 22:47
#ifndef DIC_EXCP_H_
#define DIC_EXCP_H_

#include <iostream>
#include <stdexcept>
#include <string>

namespace excp {
   class problem : public std::exception {
      private:
         std::string msg;
      public:             
         problem(const std::string & m) : std::exception(), msg(m) {}
         ~problem() throw() {}
         virtual const char * what() const throw() { return msg.c_str(); } 
   };
   
   class bad_file : public problem {
      private:
         std::string m_Message;      
      public:
         bad_file(const std::string & fn, const std::string & msg) throw()
                  : problem(fn), m_Message(msg) {}
         bad_file(const std::string & fn) throw() 
             : problem(fn), m_Message(" is missing or is corrupted!\n") {}
         ~bad_file() throw() {}         
         void report() const throw() { 
              std::cout << problem::what() << m_Message; }
   };
   
   class invalid_value : public std::logic_error {
      private:
         int val;
      public:
         invalid_value(const std::string & m, int v) throw() 
                      : logic_error(m), val(v) {}
         void report() const throw()  
         { std::cout << logic_error::what() << " " << char(val); }
   }; 
   
} // end of namespace excp

#endif // DIC_EXCP_H_
         
