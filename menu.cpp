//menu.cpp - methods of Menu class 28-06-09 20:32
#include "menu.h"
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <sstream>

namespace PWJ {      
   Menu::Menu(const char * fname, const char * sep, char choice)
                        throw(problem) : m_Separator(sep), m_Char(choice) {
      m_Fin.open(fname);
      if (!m_Fin.is_open())
         throw problem("Menu file is missing or is corrupted!\n");
      LoadMenu();
      m_Chosen = '\0';
      m_MenuNumber = 0;
   }
   
   Menu::~Menu() throw() {
      if (m_Fin.is_open())
         m_Fin.close();      
   }
   
   void Menu::LoadMenu() throw(problem) {      
      m_Fin.ignore(IGNORED_CHARS, TERMINATOR);
      m_Fin.putback(TERMINATOR);            
      while (m_Fin) {      
         choices temp_choices;      
         std::ostringstream temp_menu;           
         while (true) { 
            std::string line;                        
            getline(m_Fin, line);            
            if (line[0] == TERMINATOR && line != m_Separator) 
               continue;
            else if (line == m_Separator)
               break;                                                   
            std::size_t loc = line.find(m_Char);
            char choice;
            if (loc == std::string::npos) {
               std::srand(std::time(0));
               choice = (std::rand() % int('Z'));  
            }          
            else {
               choice = std::toupper(line[loc + 1]);  
               line.erase(loc, 1);
            }                        
            temp_choices.push_back(choice);                                              
            temp_menu << "\t" << temp_choices.back()
                      << "\t" << line << std::endl;
         }            
         m_Choices.push_back(temp_choices);
         std::string temp = temp_menu.str();         
         m_SubMenus.push_back(temp);          
         m_Fin.ignore(IGNORED_CHARS, TERMINATOR);
         m_Fin.putback(TERMINATOR);                               
      } 
   } 
   
   void Menu::GetChoice(int which) const throw() {
      using std::cout;
      using std::cin;
      while (true) {
         cout << "Action: ";
         char choice;
         (cin >> choice).get();
         choices::const_iterator iter;
         iter = find(m_Choices[which].begin(), m_Choices[which].end(), 
                std::toupper(choice));
         if (iter == m_Choices[which].end()) {
            cout << choice << " is not valid option!\n";   
            continue;
         }
         else {
            m_Chosen = *iter;
            m_MenuNumber = iter - m_Choices[which].begin();            
            break;
         }
      }
   }     
   
   void Menu::ShowMenu(int which) const throw(problem) {
      if (which < 0 || which > (m_SubMenus.size() - 1))      
         throw problem("This menu does not exist!");
      std::cout << m_SubMenus[which] << std::endl; 
      GetChoice(which);     
   }       
} // end of PWJ namespace           
