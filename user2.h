//user2.h - declaration of User Class 11-07-09 23:24
#ifndef USER_H_
#define USER_H_

#include "dic_excp.h"
#include "password.h"
#include "stat.h"
#include <fstream>
#include <string>

namespace usr {
   class BaseUser : private pwd::Password {
      private:         
         bool m_IsLogged;                          
      protected:  
         using pwd::Password::Load;        
      public:
         using pwd::Password::GetUname; 
         using pwd::Password::IsValid;        
         BaseUser() : pwd::Password(), m_IsLogged(false) {}
         BaseUser(const std::string & un, const std::string & p, 
                  const std::string & fn, const std::string & pwd_file) 
                                                   throw(excp::problem);                 
         virtual ~BaseUser() throw() {}
         bool IsLogged() const { return m_IsLogged; }
         virtual bool LogIn(const std::string & uname, 
                    const std::string & passwd) throw(excp::problem); 
         virtual bool LogOut() throw();
         std::string GetFname(const std::string & un, const std::string & p) 
                                                  const throw(excp::problem);
         void ChangeData(const std::string & un, const std::string & p,
                         const std::string & fn) throw(excp::problem);
   };
   
   class User : public BaseUser {
      private:
         sta::Statistic * m_pCurrentSession;
         sta::Statistic * m_pLastSession;
         sta::Statistic * m_pOverallStat;
      public:
         enum {CURRENT, LAST_TIME, OVERALL};
         User();
         User(const std::string & un, const std::string & p, 
              const std::string & fn, const std::string & pwd_file) 
                                               throw(excp::problem);
         User(const User &);
         User & operator=(const User &);
         ~User() throw();                  
         virtual bool LogIn(const std::string & uname, 
                    const std::string & passwd) throw(excp::problem); 
         virtual bool LogOut() throw();
         void ShowStat(int which) const;
         void StartLearning() const;
         void StopLearning() const;
         void StartTesting() const;
         void StopTesting() const;
         void OneLearned(int dict_pos);
         void OneAnswer(const std::string & ans = "");
         void LoadUser(std::ifstream & fin_pwd) throw(excp::bad_file);
         void SaveProfile() const;
         void ShowLastDate() const;
         int GetLastTimePosition() const 
             { return m_pLastSession->Position(); }
         void NewTest();              
         void TestResult() const { m_pCurrentSession->TestResult(); }
         long GetRunTime(int which) const 
              { return m_pCurrentSession->GetRunTime(which); }
         void EraseOverallStat() 
              { m_pOverallStat->ResetStat(sta::Statistic::ALL); }
         const std::vector<std::string> & GetIncorAnswers() const
            { return m_pOverallStat->GetAnswers(); }
         void CorrectAnswer(const std::string & word)
            { m_pOverallStat->CorrectWord(word); }
   };     
} //end of usr namespace

#endif // USER_H_ 
