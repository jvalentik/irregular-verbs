//program.h - definition of program class 29-06-09 21:10
#ifndef PROGRAM_H_
#define PROGRAM_H_

#include "dic_excp.h"
#include "dictionary.h"
#include "menu.h"
#include "user2.h"
#include <vector>

namespace prg {
   class Program {
      private:         
         std::vector<usr::User *> m_Users;
         dict::Dictionary * m_Dictionary; 
         const PWJ::Menu * m_pMenu; 
         void LoadData(const char * fname) throw(excp::problem);  
         void Learn(std::vector<usr::User *>::const_iterator) throw(); 
         void Test(std::vector<usr::User *>::const_iterator) throw();
         bool Examine(const dict::VerbItem &) const throw();     
         bool ExamineRm(const dict::VerbItem &) const throw();
         void StatManage(std::vector<usr::User *>::const_iterator) throw();
         bool ChangeUserData(std::vector<usr::User *>::const_iterator) throw (); 
      public:
         Program(const PWJ::Menu &, bool use_trans = true);
         ~Program();
         void AddUser() throw(excp::problem);
         void LoadUser() throw(excp::problem);         
   };   
} // end of prg namespace

#endif // PROGRAM_H_
