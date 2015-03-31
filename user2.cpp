//user2.cpp - methods of user class2.0 11-07-09 23:54
#include "user2.h"
#include <fstream>

using pwd::Password;
using sta::Statistic;
using std::string;
using excp::problem;
using std::cout;
using std::endl;

namespace usr {
   BaseUser::BaseUser(const string & un, const string & p, 
                      const string & fn, const string & pwd_file) throw(problem)
                                                         : Password(un, p, fn) {
      m_IsLogged = false;
      std::ofstream fout(pwd_file.c_str(), 
                          std::ios_base::app | std::ios_base::binary);
      if (!fout.is_open())
         throw problem("User data file cannot be created!\n");
      Password::Save(fout);
      fout.close();
      cout << "User account " << un << " created and saved.\n";
   }
      
   bool BaseUser::LogIn(const string & uname, 
                        const string & passwd) throw(problem) {
      if (!m_IsLogged) {                  
         if (Password::IsValid(uname, passwd)) {            
            m_IsLogged = true;
            return true;
         }
         else {            
            return false;
         }
      }      
      return false;
   }
   
   bool BaseUser::LogOut() throw() {
      if (m_IsLogged) {
         m_IsLogged = false;
         return true;
      }
      else 
         return false;
   }
   
   string BaseUser::GetFname(const string & un, 
                        const string & p) const throw(problem) {
      if (m_IsLogged) {
         return Password::GetFname(un, p);
      }
      else
         throw problem("Access denied!\n");
   }
   
   void BaseUser::ChangeData(const string & un, const string & p,
                             const string & fn) throw(problem) {
      if (m_IsLogged)
         Password::Change(un, p, fn);
      else
         throw problem("Access denied!\n");
   }
   
   User::User() : BaseUser() {
      m_pCurrentSession = new Statistic;
      m_pLastSession = new Statistic;
      m_pOverallStat = new Statistic;
   }
   
   User::User(const string & un, const string & p,
                 const string & fn, const string & pwd_file) throw(problem) 
                     : BaseUser(un,p,fn, pwd_file) {
      m_pCurrentSession = new Statistic;
      m_pLastSession = new Statistic;
      m_pOverallStat = new Statistic;
   }
   
   User::User(const User & u) : BaseUser(u) {
      m_pCurrentSession = new Statistic(*u.m_pCurrentSession);
      m_pLastSession = new Statistic(*u.m_pLastSession);
      m_pOverallStat = new Statistic(*u.m_pOverallStat);
   }
   
   User & User::operator=(const User & u) {
      if (this != &u) {
         BaseUser::operator=(u);
         delete m_pCurrentSession;
         m_pCurrentSession = new Statistic(*u.m_pCurrentSession);
         delete m_pLastSession;
         m_pLastSession = new Statistic(*u.m_pLastSession);
         delete m_pOverallStat;
         m_pOverallStat = new Statistic(*u.m_pOverallStat);     
      }
      return *this;
   }
   
   User::~User() throw() {
      delete m_pCurrentSession;
      delete m_pLastSession;
      delete m_pOverallStat;
   }
   
   bool User::LogIn(const string & uname, 
                    const string & passwd) throw(problem) {
      if (BaseUser::LogIn(uname, passwd)) {
         m_pCurrentSession->StartTiming(Statistic::WORKING);
         m_pOverallStat->StartTiming(Statistic::WORKING);
         return true;
      }
      else
         return false;
   }
   
   bool User::LogOut() throw() {
      if (BaseUser::LogOut()) {
         m_pCurrentSession->StopTiming(Statistic::WORKING); 
         m_pOverallStat->StopTiming(Statistic::WORKING);  
         SaveProfile();
         return true;
      }
      else
         return false;
   }
   
   void User::ShowStat(int which) const {
      switch (which) {
         case CURRENT:
            cout << "Current session:\n\n";
            cout << *m_pCurrentSession << endl;
            break;
         case LAST_TIME:
            cout << "Last time session:\n\n";  
            cout << *m_pLastSession << endl;
            break;
         case OVERALL:
            cout << "Overall statistic:\n\n";
            cout << *m_pOverallStat << endl;
            break;
         default:
            std::cerr << "Invalid statistic specifier!\n";
      }
   }     
   
   void User::StartLearning() const {
      m_pCurrentSession->StartTiming(Statistic::LEARNING); 
      m_pOverallStat->StartTiming(Statistic::LEARNING);
   }
   
   void User::StopLearning() const {
      m_pCurrentSession->StopTiming(Statistic::LEARNING); 
      m_pOverallStat->StopTiming(Statistic::LEARNING);
   }
      
   void User::StartTesting() const {
      m_pCurrentSession->StartTiming(Statistic::TESTING); 
      m_pOverallStat->StartTiming(Statistic::TESTING);
   }
   
   void User::StopTesting() const {
      m_pCurrentSession->StopTiming(Statistic::TESTING); 
      m_pOverallStat->StopTiming(Statistic::TESTING);
   }
   
   void User::OneLearned(int dict_pos) {
      m_pCurrentSession->OneLearned(dict_pos);
      m_pOverallStat->OneLearned(dict_pos);
   }
   
   void User::OneAnswer(const string & ans) {
      m_pCurrentSession->OneAnswer(ans);
      m_pOverallStat->OneAnswer(ans);
   }
   
   void User::LoadUser(std::ifstream & fin_pwd) throw(excp::bad_file) {
      Load(fin_pwd);
      string profile_file = GetUname();
      profile_file += ".prf";
      std::ifstream fin_prf(profile_file.c_str(), std::ios_base::in |
                            std::ios_base::binary);
      if (fin_prf.is_open()) {
         m_pLastSession->Load(fin_prf);
         m_pOverallStat->Load(fin_prf);
      }
      fin_prf.close();
   }
   
   void User::SaveProfile() const {
      string profile_file = GetUname();
      profile_file += ".prf";
      std::ofstream fout_prf(profile_file.c_str(), std::ios_base::out |
                             std::ios_base::binary);
      if (fout_prf.is_open()) {
         m_pCurrentSession->Save(fout_prf);
         m_pOverallStat->Save(fout_prf);
         fout_prf.close();
      }
      else {
         std::cerr << "Data cannot be written!\n";
      }
   }
   
   void User::ShowLastDate() const {
      cout << m_pLastSession->GetDate();
   }   
   
   void User::NewTest() {
      m_pCurrentSession->ResetStat(Statistic::TESTING); 
   }      
} // end of usr namespace
