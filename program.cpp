//program.cpp - methods of program class 29-06-09 21:18
#include "dic_excp.h"
#include "program.h"
#include <algorithm>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;
using usr::User;
using dict::VerbItem;
using excp::problem;

namespace prg {
   namespace {
      enum MENUS {
           MAIN_MENU, USER_CHOICES, SUBMENU_LEARN, 
           SUBMENU_TEST, SUBMENU_STAT, SUBMENU_YES_NO
      };
      enum MENU_COMMANDS {
           FIRST_COM, SECOND_COM, THIRD_COM, 
           FOURTH_COM, FIFTH_COM
      };
      const int WAIT_TIME(5);
      const char END_CHAR('E');
      
      void FreeMemory(const User * p_User) {
         delete p_User;
         p_User = 0;
      } 
      
      class HasUname {
         private:
            const string m_Uname;            
         public:
            HasUname(const string & un) : m_Uname(un) {}
            bool operator()(const User * p_User) const;
      };
      
      bool HasUname::operator()(const User * p_User) const {
         if (p_User->GetUname() == m_Uname)             
            return true;         
         return false;
      } 
      
      bool is_logged(const User * p_User) {
         return p_User->IsLogged();
      }       
   } // end of unnamed namespace
     
   Program::Program(const PWJ::Menu & m, bool use_trans) {
      string trans;
      if (use_trans)
         trans = "trans.txt";
      else
         trans = "";      
      m_Dictionary = new dict::Dictionary("verbs.txt", trans.c_str());      
      m_pMenu = &m;
      try {
         LoadData("users.pwd");
      }
      catch(problem & p) {
         cout << "No users accounts created!\n\n";
      }      
   }
   
   Program::~Program() {
      delete m_Dictionary;
      for_each(m_Users.begin(), m_Users.end(), FreeMemory);      
   }
   
   void Program::LoadData(const char * fname) throw(problem) {
      std::ifstream fin(fname, std::ios_base::in | std::ios_base::binary);
      if (!fin.is_open())
         throw problem("Data cannot be loaded!\n");
      User tempUser;
      string uname;
      while (fin) {         
         tempUser.LoadUser(fin);         
         if (!fin) break;
         m_Users.push_back(new User(tempUser));
      }
      fin.close();      
   }        
   
   void Program::AddUser() throw(problem) {
      string fname, uname, passwd, passwd2;
      cout << "Enter nickname: ";
      getline(cin, uname);
      cout << "Enter whole name: ";
      getline(cin, fname);      
      cout << "Enter password: ";
      getline(cin, passwd);
      cout << "Re-enter password: ";
      getline(cin, passwd2);
      if (passwd == passwd2) {         
         m_Users.push_back(new User(uname, passwd, fname, "users.pwd"));                  
      }
      else 
         throw problem("Passwords don`t match!\n");      
   }       
   
   void Program::LoadUser() throw(problem) {
      if (m_Users.empty())
         throw problem("No user`s accounts exists!\n");
      string uname, passwd;
      vector<User *>::const_iterator it_ActiveUser;
      it_ActiveUser = find_if(m_Users.begin(), m_Users.end(), is_logged);
      if (it_ActiveUser != m_Users.end()) {
         (*it_ActiveUser)->LogOut();
      }
      cout << "Enter nickname: ";
      getline(cin, uname);
      it_ActiveUser = find_if(m_Users.begin(), 
                              m_Users.end(), HasUname(uname));
      if (it_ActiveUser == m_Users.end())
         cout << "User " << uname << " doesn`t exist!\n";
      else {
         while (true) {
            cout << "Enter password: ";
            getline(cin, passwd);     
            try {
               if ((*it_ActiveUser)->LogIn(uname, passwd)) {
                  cout << "Welcome " 
                       << (*it_ActiveUser)->GetFname(uname, passwd) 
                       << "!\n";
                  try {                     
                     cout << "You were logged last time ";
                     (*it_ActiveUser)->ShowLastDate();
                     cout << endl;
                     m_Dictionary->Reset(
                                (*it_ActiveUser)->GetLastTimePosition());
                     m_Dictionary->SetIncorAnswers(
                                (*it_ActiveUser)->GetIncorAnswers());
                  }
                  catch (problem & p) {
                     cout << p.what();
                  }                 
                  cout << "\n\t***Menu***\n\n";
                  bool run(true);
                  while (run) {
                     m_pMenu->ShowMenu(USER_CHOICES); 
                     int choice = m_pMenu->GetMenuNum();
                     switch (choice) {
                        case FIRST_COM:
                           Learn(it_ActiveUser);
                           break;
                        case SECOND_COM:
                           Test(it_ActiveUser);                           
                           break;
                        case THIRD_COM:
                           StatManage(it_ActiveUser);
                           break;                     
                        case FOURTH_COM:                                                      
                           (*it_ActiveUser)->LogOut();
                           run = false;
                           break;
                      }
                  }        
                  break;
               }
               else {
                  cout << "Incorrect password!\n";
                  continue;
               }
            }
            catch (problem & p) {
               cout << p.what();
               break;
            }
         }
      }      
   }
   
   void Program::Learn(vector<User *>::const_iterator it_ActiveUser) 
                                                             throw() {
      bool run(true);
      std::auto_ptr<VerbItem> verb;
      while (run) {
         m_pMenu->ShowMenu(SUBMENU_LEARN);
         int choice = m_pMenu->GetMenuNum();         
         switch (choice) {                            
            case FIRST_COM:
               cout << "Type " << END_CHAR << " to end learning.\n";               
               (*it_ActiveUser)->StartLearning();
               m_Dictionary->Reset();
               do {                  
                  verb = m_Dictionary->GetVerb();
                  (*it_ActiveUser)->OneLearned(verb->GetPosition());
                  verb->ShowAll();
                  stp::Wait(WAIT_TIME);
               } while (std::toupper(cin.get()) != END_CHAR);
               (*it_ActiveUser)->StopLearning();               
               break;            
            case SECOND_COM:
               cout << "Type " << END_CHAR << " to end learning.\n";
               (*it_ActiveUser)->StartLearning();
               m_Dictionary->Reset(((*it_ActiveUser)->GetLastTimePosition()));               
               do {                  
                  verb = m_Dictionary->GetVerb();
                  (*it_ActiveUser)->OneLearned(verb->GetPosition());
                  verb->ShowAll();
                  stp::Wait(WAIT_TIME);
               } while (std::toupper(cin.get()) != END_CHAR);
               (*it_ActiveUser)->StopLearning();               
               break; 
            case THIRD_COM:
               cout << "Type " << END_CHAR << " to end learning.\n";
               m_Dictionary->SetIncorAnswers(
                                (*it_ActiveUser)->GetIncorAnswers());
               (*it_ActiveUser)->StartLearning();
               do {
                  verb = m_Dictionary->GetRmVerb();
                  (*it_ActiveUser)->OneLearned(verb->GetPosition());
                  verb->ShowAll();
                  stp::Wait(WAIT_TIME);
               } while (std::toupper(cin.get()) != END_CHAR);
               (*it_ActiveUser)->StopLearning(); 
               break;
            case FOURTH_COM:
               cout << "Enter a letter from which you want to start learning: ";
               char ch;
               (cin >> ch).get();
               cout << "Type " << END_CHAR << " to end learning.\n";
               (*it_ActiveUser)->StartLearning();
               do {
                  try {
                     verb = m_Dictionary->GetVerb(ch);
                     (*it_ActiveUser)->OneLearned(verb->GetPosition());
                     verb->ShowAll();
                     stp::Wait(WAIT_TIME);
                  }
                  catch (excp::invalid_value & iv) {
                     iv.report();
                     cout << "\nEnter new letter: ";
                     cin >> ch;
                     continue;
                  }
               } while (std::toupper(cin.get()) != END_CHAR);
               (*it_ActiveUser)->StopLearning();
               break;                     
            case FIFTH_COM:
               run = false;
               break;       
         } 
      }
   }
   
   bool Program::Examine(const VerbItem & verb) const throw() {
      string past, particip;      
      cout << "Infinitive of verb " 
           << verb.GetWord(VerbItem::INFINIT) << endl;
      cout << "Enter past tense: ";
      getline(cin, past);
      cout << "Enter past participle: ";
      getline(cin, particip);
      if (past == verb.GetWord(VerbItem::PAST) &&
          particip == verb.GetWord(VerbItem::PARTICIP))
         return true;
      return false;
   }
   
   bool Program::ExamineRm(const VerbItem & verb) const throw() {
      string infinit, past, particip;
      std::srand(std::time(0));
      int tense = std::rand() % 3;
      bool answer(false);
      switch (tense) {
         case VerbItem::INFINIT:
            infinit = verb.GetWord(VerbItem::INFINIT);
            cout << "Infinitive of verb " << infinit  << endl;
            cout << "Enter past tense: ";
            getline(cin, past);
            cout << "Enter past participle: ";
            getline(cin, particip);
            if (past == verb.GetWord(VerbItem::PAST) &&
                particip == verb.GetWord(VerbItem::PARTICIP))
                answer = true;
            break;
         case VerbItem::PAST:
            past =  verb.GetWord(VerbItem::PAST);
            cout << "Past tense: " << past << endl;
            cout << "Enter infinitive: ";
            getline(cin, infinit);
            cout << "Enter past participle: ";
            getline(cin, particip);
            if (infinit == verb.GetWord(VerbItem::INFINIT) &&
                particip == verb.GetWord(VerbItem::PARTICIP))
                answer = true;
            break;
         case VerbItem::PARTICIP:
            particip = verb.GetWord(VerbItem::PARTICIP);
            cout << "Past participle: " << particip << endl;
            cout << "Enter infinitive: ";
            getline(cin, infinit);
            cout << "Enter past tense: ";
            getline(cin, past);
            if (infinit == verb.GetWord(VerbItem::INFINIT) &&
                past == verb.GetWord(VerbItem::PAST))
                answer = true;
            break;
      }
      return answer;
   }    
   
   void Program::Test(vector<User *>::const_iterator it_ActiveUser)
                                                            throw() {
      bool run(true);
      std::auto_ptr<VerbItem> verb;
      while (run) {
         m_pMenu->ShowMenu(SUBMENU_TEST);
         int choice = m_pMenu->GetMenuNum();         
         switch (choice) {
            case FIRST_COM:
               m_Dictionary->Reset();
               m_Dictionary->SetIncorAnswers(
                                (*it_ActiveUser)->GetIncorAnswers());
               (*it_ActiveUser)->NewTest();
               (*it_ActiveUser)->StartTesting();
               cout << "Type " << END_CHAR << " to end testing.\n";
               do {
                  verb = m_Dictionary->GetRmVerb();
                  if (Examine(*verb)) {
                     (*it_ActiveUser)->OneAnswer();
                     (*it_ActiveUser)->
                          CorrectAnswer(verb->GetWord(VerbItem::INFINIT));
                     cout << "Correct!\n";
                  }
                  else {
                     (*it_ActiveUser)->
                          OneAnswer(verb->GetWord(VerbItem::INFINIT));
                     cout << "Incorrect!\n"
                          << "Correct tenses are:\n";
                     verb->ShowAll();
                  }
                  cout << "Press ENTER to continue, or " 
                       << END_CHAR << " to end: ";
               } while (std::toupper(cin.get()) != END_CHAR);
               (*it_ActiveUser)->StopTesting();
               (*it_ActiveUser)->TestResult();
               break;
            case SECOND_COM:
               m_Dictionary->Reset();
               m_Dictionary->SetIncorAnswers(
                                (*it_ActiveUser)->GetIncorAnswers());
               (*it_ActiveUser)->NewTest();  
               (*it_ActiveUser)->StartTesting();
               cout << "Type " << END_CHAR << " to end testing.\n";
               do {
                  verb = m_Dictionary->GetRmVerb();
                  if (ExamineRm(*verb)) {
                     (*it_ActiveUser)->OneAnswer();
                     (*it_ActiveUser)->
                          CorrectAnswer(verb->GetWord(VerbItem::INFINIT));
                     cout << "Correct!\n";
                  }
                  else {
                     (*it_ActiveUser)->
                          OneAnswer(verb->GetWord(VerbItem::INFINIT));  
                     cout << "Incorrect!\n"
                          << "Correct tenses are:\n";
                     verb->ShowAll();
                  }
                  cout << "Press ENTER to continue, or " 
                       << END_CHAR << " to end: ";
               } while (std::toupper(cin.get()) != END_CHAR);
               (*it_ActiveUser)->StopTesting();
               (*it_ActiveUser)->TestResult();                       
               break;
            case THIRD_COM:
                m_Dictionary->Reset();
                m_Dictionary->SetIncorAnswers(
                                (*it_ActiveUser)->GetIncorAnswers());
                (*it_ActiveUser)->NewTest(); 
                cout << "Enter time of test: ";
                int test_time;
                (cin >> test_time).get();
                cout << "You have " << test_time 
                     << " minutes to complete the test...\n";
               (*it_ActiveUser)->StartTesting(); 
               cout << "Type " << END_CHAR << " to end testing.\n"; 
               do {
                  verb = m_Dictionary->GetRmVerb();
                  if (ExamineRm(*verb)) {
                     (*it_ActiveUser)->OneAnswer();
                     (*it_ActiveUser)->
                          CorrectAnswer(verb->GetWord(VerbItem::INFINIT));
                  }
                  else {
                     (*it_ActiveUser)->
                          OneAnswer(verb->GetWord(VerbItem::INFINIT));
                  }
               } while ((*it_ActiveUser)->
                         GetRunTime(sta::Statistic::TESTING) < test_time * 60);
               cout << "Your time is up!\n";
               (*it_ActiveUser)->StopTesting();               
               (*it_ActiveUser)->TestResult();            
               break;
            case FOURTH_COM:
                 run = false;
                 break;
         }
      }
   }
   
   bool Program::ChangeUserData(vector<User *>::const_iterator it_ActiveUser) 
                                                                    throw() {
      string uname, passwd, passwd2, fname;
      cout << "Enter password: ";
      getline(cin, passwd);
      try {
         if ((*it_ActiveUser)->IsValid((*it_ActiveUser)->GetUname(), passwd)) {                  
            cout << "Do you wish to change your nickname?\n";
            m_pMenu->ShowMenu(SUBMENU_YES_NO);
            if (m_pMenu->GetMenuNum()) {
               cout << "Enter new nickname: ";
               getline(cin, uname);
            }
            else
               uname = (*it_ActiveUser)->GetUname();
            cout << "Do you wish to change your full name?\n";
            m_pMenu->ShowMenu(SUBMENU_YES_NO);
            if (m_pMenu->GetMenuNum()) {
               cout << "Enter new full name: ";
               getline(cin, fname);
            }
            else
              fname = (*it_ActiveUser)->
                           GetFname((*it_ActiveUser)->GetUname(), passwd);                 
            cout << "Do you wish to change your password?\n";
            m_pMenu->ShowMenu(SUBMENU_YES_NO);
            if (m_pMenu->GetMenuNum()) {
               cout << "Enter new password: ";
               getline(cin, passwd);
               cout << "Re-enter new password: ";
               getline(cin, passwd2);
               if (passwd == passwd2) 
                  (*it_ActiveUser)->ChangeData(uname, passwd, fname);
               else { 
                  cout << "Password don`t match!\n";
                  return false;
               }
            }
            else {
               (*it_ActiveUser)->ChangeData(uname, passwd, fname);
               return true;
            }   
         }
         else {
            cout << "Incorrect password!\n";
            return false;
         }
      }
      catch (problem &p) {
         std::cerr << p.what() << endl;
         return false;
      }
   }      
   
   void Program::StatManage(vector<User *>::const_iterator it_ActiveUser) 
                                                                throw () {
      bool run(true);
      while (run) {
         m_pMenu->ShowMenu(SUBMENU_STAT);
         int choice = m_pMenu->GetMenuNum();
         switch (choice) {
            case FIRST_COM:
               (*it_ActiveUser)->ShowStat(User::CURRENT);
               break;
            case SECOND_COM:
               (*it_ActiveUser)->ShowStat(User::LAST_TIME);
               break;
            case THIRD_COM:
               (*it_ActiveUser)->ShowStat(User::OVERALL);
               cout << "Do you wish to erase overall statistics?\n"; 
               m_pMenu->ShowMenu(SUBMENU_YES_NO);               
               if (m_pMenu->GetMenuNum())
                  (*it_ActiveUser)->EraseOverallStat();
               break;
            case FOURTH_COM:
               if (ChangeUserData(it_ActiveUser))
                  cout << "Users data changed successfully!\n\n";
               else
                  cout << "Users data unchanged!\n\n";               
               break;
            case FIFTH_COM:
               run = false;
               break;            
         }
      }
   }     
} // end of prg namespace    
