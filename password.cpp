//password.cpp - methods of Password class 2.0 24-06-09 22:36
#include "password.h"
#include <cstdlib>
#include <iostream>

using std::string;

namespace pwd {
   namespace {
      const int ASCII_CHARS(256);
      
      inline char pseudo_random() {
         return char(std::rand() % ASCII_CHARS);
      }
   }
   
   void Password::CreateKey(const string & un) const throw() {                 
      int key = 0;
      for (int i(0); i < un.size(); i++) {
         key += int(un[i]);         
      }      
      std::srand(key);      
   }
   
   void Password::Encrypt(const string & un, const string & p, 
                                   const string & fn) throw() {            
      string p_cr, fn_cr;      
      register int i; 
      for (i = 0; i < p.size(); i++) {
         p_cr += p[i] ^ pseudo_random();         
      }      
      for (i = 0; i < fn.size(); i++) {
         fn_cr += fn[i] ^ pseudo_random();         
      }
      m_pData->uname = un;
      m_pData->passwd = p_cr;      
      m_pData->fullname = fn_cr;
   }
   
   const Password::Data * Password::Decrypt() const throw() {            
      string fn, pd;
      register int i;      
      for (i = 0; i < m_pData->passwd.size(); i++) {
         pd += m_pData->passwd[i] ^ pseudo_random();
      } 
      for (i = 0; i < m_pData->fullname.size(); i++) {
         fn += m_pData->fullname[i] ^ pseudo_random();
      }
      Data * p_Result = new Data;     
      p_Result->uname = m_pData->uname;      
      p_Result->passwd = pd;
      p_Result->fullname = fn;
      return p_Result;      
   }      
   
   void Password::Save(std::ofstream & fout) const 
                              throw(excp::problem) {
      if (m_pData->uname.empty())
         throw excp::problem("No name created!\n");
      std::size_t len_un = m_pData->uname.size();
      std::size_t len_p = m_pData->passwd.size();                  
      std::size_t len_fn = m_pData->fullname.size();      
      fout.write((char *) &len_un, sizeof(std::size_t));      
      fout.write(m_pData->uname.data(), len_un);      
      fout.write((char *) &len_p, sizeof(std::size_t));      
      fout.write(m_pData->passwd.data(), len_p);               
      fout.write((char *) &len_fn, sizeof(std::size_t));      
      fout.write(m_pData->fullname.data(), len_fn) << std::flush;      
      if (!fout)
         throw excp::problem("Error during writing!\n");   
   }
   
   void Password::Load(std::ifstream & fin) throw(excp::problem) {      
      m_pData->uname.clear();
      m_pData->passwd.clear();
      m_pData->fullname.clear();
      std::size_t len_un, len_p, len_fn;   
      register int i;    
      char ch;     
      fin.read((char *) &len_un, sizeof(std::size_t));
      if (fin.eof())
         return;            
      for (i = 0; i < len_un; i++) {
         fin.read(&ch, sizeof(char));
         if (fin.fail())
            throw excp::problem("Corrupted file!\n");
         m_pData->uname += ch;           
      }                  
      fin.read((char *) &len_p, sizeof(std::size_t));           
      for (i = 0; i < len_p; i++) {
         fin.read(&ch, sizeof(char));
         if (fin.fail())
            throw excp::problem("Corrupted file!\n");
         m_pData->passwd += ch;         
      }      
      fin.read((char *) &len_fn, sizeof(std::size_t));         
      for (i = 0; i < len_fn; i++) {
         fin.read(&ch, sizeof(char));         
         if (fin.eof())
            throw excp::problem("Corrupted file!\n");
         m_pData->fullname += ch;         
      }                        
   } 
   
   Password::Password() throw() {
      m_pData = new Data;
      m_pData->uname = "";
      m_pData->passwd = "";      
      m_pData->fullname = "";
      m_Tries = 0;
   }     
      
   Password::Password(const string & un, const string & p,
                                 const string & fn) throw() {  
      m_pData = new Data; 
      CreateKey(un);         
      Encrypt(un, p, fn);
      m_Tries = 0;                       
   }
   
   Password::~Password() throw() {
      delete m_pData;
      m_pData = 0;
   }
   
   Password::Password(const Password & p) throw() {      
      m_pData = new Data;
      m_pData->uname = p.m_pData->uname;
      m_pData->passwd = p.m_pData->passwd;
      m_pData->fullname = p.m_pData->fullname;
      m_Tries = p.m_Tries;
   } 
   
   Password & Password::operator=(const Password & p) throw() {      
      if (this != &p) {         
         delete m_pData;
         m_pData = new Data;
         m_pData->uname = p.m_pData->uname;
         m_pData->passwd = p.m_pData->passwd;
         m_pData->fullname = p.m_pData->fullname;
         m_Tries = p.m_Tries;
      }
      return *this;
   }      
   
   bool Password::IsValid(const string & un, const string & p) const 
                                                throw(excp::problem) {                  
      CreateKey(un);     
      const Data * p_Data = Decrypt();   
      if (p_Data->uname == un && p_Data->passwd == p) {
         m_Tries = 0;                 
         delete p_Data;         
         return true;
      }
      else {
         if (++m_Tries >= MAX_TRIES) {
            delete p_Data;                                  
            throw excp::problem("Access denied!\n");  
         }
         delete p_Data;         
         return false;   
      }
   }  
   
   void Password::Change(const string & un, const string & p, 
                                   const string & fn) throw() {
      CreateKey(un);
      Encrypt(un, p, fn);  
   }
   
   string Password::GetFname(const string & un, const string & p) const 
                                                   throw(excp::problem) {
      if (IsValid(un, p)) {
         CreateKey(un);
         const Data * p_Data = Decrypt();
         string fname = p_Data->fullname;
         delete p_Data;
         return fname;
      }
      else
         throw excp::problem("Invalid login data!\n");
   }
   
   string Password::GetUname() const throw() {
      string un = m_pData->uname;
      return un;
   }         
} // end of namespace pwd  
