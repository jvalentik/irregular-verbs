//password.h - declaration of password class 2.0 24-06-09 22:35
#ifndef PASSWORD_H_
#define PASSWORD_H_

#include "dic_excp.h"
#include <fstream>
#include <string>

namespace pwd {   
   class Password {     
      private:
         static const int MAX_TRIES = 3; // maximum incorrect logins
         struct Data {
            std::string uname;            
            std::string passwd;
            std::string fullname;            
         };             
         Data * m_pData;                  
         mutable int m_Tries;
      protected:
         void CreateKey(const std::string & un) const throw();
         void Encrypt(const std::string & un, const std::string & p,
                              const std::string & fn) throw();
         const Data * Decrypt() const throw();         
      public: 
         Password() throw();        
         Password(const std::string & un, const std::string & p, 
                                         const std::string & fn) throw();
         virtual ~Password() throw();
         Password(const Password & p) throw();
         Password & operator=(const Password & p) throw();
         bool IsValid(const std::string & un, const std::string & p) const 
                                                       throw(excp::problem);         
         void Change(const std::string & un, const std::string & p,
                                       const std::string & fn) throw();
         void Save(std::ofstream &) const throw(excp::problem);
         void Load(std::ifstream &) throw(excp::problem); 
         std::string GetFname(const std::string & un, const std::string & p)
                                                   const throw(excp::problem);
         std::string GetUname() const throw();                     
   };
} // end of namespace pwd

#endif // PASWORD_H_
      
