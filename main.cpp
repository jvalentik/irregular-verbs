/* Name: Iregular Verbs 1.0 
  Copyright: GPL Open Source
  Author: PowerJ Slovakia
  Date: 15-07-09 21:25
  Description: Simple educational program written using Dev-C++
*/
//main.cpp - final version of IVerbs1.0 
#include "dic_excp.h"
#include "dictionary.h"
#include "menu.h"
#include "program.h"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>

enum MENUS {MAIN_MENU};
enum MENU_COMMANDS {CREATE_USER, LOAD_USER, END_PROGRAM};

bool make_choice(const PWJ::Menu &, prg::Program &);

int main(int argc, char * argv[]) {
   using std::cout;
   using std::cerr;
   //no command line or -notrans mode
   if (argc == 1 || (argc == 2 && std::strcmp(argv[1], "-notrans") == 0)) { 
      try {
         bool use_trans(true);         
         if (argc == 2)
            use_trans = ((std::strcmp(argv[1], "-notrans") == 0) 
                          ? false : true); 
         
         PWJ::Menu menu;        
         prg::Program program(menu, use_trans);               
         do {
            menu.ShowMenu(MAIN_MENU);
         } while (make_choice(menu, program));
         cout << "Good bye...";
      } 
      catch (PWJ::Menu::problem & p) {
         cerr << "\n***Error ocurred!***\n";
         cerr << p.what();
         cerr << "Use -i mode!\n";       
         return EXIT_FAILURE;
      }
      catch (excp::bad_file & bf) {
         cerr << "\n***Error ocurred!***\n";
         bf.report();
         return EXIT_FAILURE;
      }            
      catch (std::exception & e) {
         cerr << "\n***Error ocurred!***\n";
         cerr << e.what();       
         return EXIT_FAILURE;
      }
   }
   //command line -i mode
   else if (argc == 3 && std::strcmp(argv[1], "-i") == 0) { 
      try {
         dict::Dictionary dictionary("verbs.txt", "trans.txt");
         std::auto_ptr<dict::VerbItem> verb(dictionary.WrapVerbs(argv[2]));
         verb->ShowAll();
      }
      catch (excp::invalid_value & iv) {
         cerr << argv[2] 
              << " is not an English irregular verb in infinitive!\n";
      }
      catch (excp::bad_file & bf) {
         cerr << "\n***Error ocurred!***\n";
         bf.report();
         return EXIT_FAILURE;
      } 
      catch (std::exception & e) {
         cerr << "\n***Error ocurred!***\n";
         cerr << e.what();       
         return EXIT_FAILURE;
      }
   }
   //command line -notrans -i mode
   else if (argc == 4 && (std::strcmp(argv[1], "-notrans") == 0) && 
                                 (std::strcmp(argv[2], "-i") == 0)){
      try {
         dict::Dictionary dictionary("verbs.txt");
         std::auto_ptr<dict::VerbItem> verb(dictionary.WrapVerbs(argv[3]));
         verb->ShowAll();
      }
      catch (excp::invalid_value & iv) {
         cerr << argv[3] 
              << " is not an English irregular verb in infinitive!\n";
      }
      catch (excp::bad_file & bf) {
         cerr << "\n***Error ocurred!***\n";
         bf.report();
         return EXIT_FAILURE;
      } 
      catch (std::exception & e) {
         cerr << "\n***Error ocurred!***\n";
         cerr << e.what();       
         return EXIT_FAILURE;
      }
   }
   // invalid CLI commands                                                       
   else {                                                    
      cerr << "Correct use is " << argv[0] 
           << " -i infinitive or -notrans -i infinitive\n";
   }
   return EXIT_SUCCESS;
}

bool make_choice(const PWJ::Menu & m, prg::Program & p) {
   bool run(true);
   int choice = m.GetMenuNum();   
   switch (choice) {
      case CREATE_USER:
         while (true) {
            try {
               p.AddUser();
               break;
            }
            catch (std::exception & e) {
               std::cerr << e.what();
               continue;
            }
         }
         break;
      case LOAD_USER:
         while (true) {
            try {               
               p.LoadUser();
               break;
            }
            catch (std::exception & e) {
               std::cerr << e.what();
               p.AddUser();
               continue;
            }
         }
         break;
      case END_PROGRAM:
         run = false;
         break;
      default:
         std::cerr << "ERROR";
   }
   return run;
}      
