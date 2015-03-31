//menu.h - Declaration of Menu class 28-06-09 00:20
#ifndef MENU_H_
#define MENU_H_

#include <exception>
#include <fstream>
#include <string>
#include <vector>

namespace PWJ {
   class Menu {
      public:          
         class problem : public std::exception {
            private:
               std::string msg;
            public:
               problem(const char * m) throw() : std::exception(), msg(m) {}
               ~problem() throw() {}
               virtual const char * what() { return msg.c_str(); }
         }; 
      private:           
         const std::string m_Separator;
         const char m_Char;
         static const char TERMINATOR = '[';
         static const int IGNORED_CHARS = 512;
         typedef std::vector<char> choices;                 
         std::vector<choices> m_Choices;
         std::vector<std::string> m_SubMenus;
         std::ifstream m_Fin; 
         mutable char m_Chosen;
         mutable int m_MenuNumber;        
         void LoadMenu() throw(problem); 
         void GetChoice(int which) const throw();                 
      public:
         explicit Menu(const char * fname = "menu.conf", 
                       const char * separator = "[END]", char choice = '$') 
                       throw(problem);
         ~Menu() throw();
         void ShowMenu(int which) const throw(problem);  
         char GetMenuChar() const throw() { return m_Chosen; }       
         int GetMenuNum() const throw() { return m_MenuNumber; }
   };
   
} // end of PWJ namespace

#endif //MENU_H_
         
