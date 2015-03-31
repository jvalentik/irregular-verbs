//dictionary.h - declaration of Dictionary class 2.1 13-07-09 22:06
#ifndef DICTIONARY_H_
#define DICTIONARY_H_

#include "dic_excp.h"
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace dict {
   class VerbItem {
      private:
         std::string m_Infinitive;         
         std::string m_PastTense;
         std::string m_PastParticiple;
         std::string m_Translation;
         int m_Position;         
      public:
         enum {INFINIT, PAST, PARTICIP, TRANS};
         VerbItem(const std:: string & in, const std::string & pt,
                  const std::string & pp, const std::string & t, int pos) 
                  : m_Infinitive(in), m_PastTense(pt), 
                  m_PastParticiple(pp), m_Translation(t), m_Position(pos) {}
         const std::string & GetWord(int what = INFINIT) const
                                     throw(excp::invalid_value);
         int GetPosition() const throw() { return m_Position; }        
         void ShowAll() const throw();
         bool operator==(const VerbItem &) const throw();
   };        
         
   class Dictionary {
      private:
         typedef std::pair<std::string, std::string> Pair;
         typedef std::multimap<std::string, std::string> Storage;
         std::vector<std::string> m_Infinitives;
         std::vector<std::string> m_IncorrectAnswers;
         Storage m_Verbs;
         int m_Position;
         int m_IncorPos;         
         char m_ChosenChar; 
         bool m_IsSorted;        
         void LoadDict(const char * fn_dc, const char * fn_tr) 
                                        throw(excp::bad_file);
         const std::string & GetInfinitive(char group) 
                                        throw(excp::invalid_value);
         const std::string & GetInfinitive() throw();
         const std::string & GetRmInfinitive() throw();         
         
      public:         
         Dictionary(const char * fn_dc, const char * fn_tr = "") 
                                        throw(excp::bad_file); 
         void Reset(int pos = 0);
         std::auto_ptr<VerbItem> WrapVerbs(const std::string & infin) const 
                                        throw(excp::invalid_value);          
         std::auto_ptr<VerbItem> GetVerb(char group)  
                                        throw(excp::invalid_value); 
         std::auto_ptr<VerbItem> GetVerb() throw();               
         std::auto_ptr<VerbItem> GetRmVerb() throw();
         void SetIncorAnswers(const std::vector<std::string> & ia) throw()
            { m_IncorrectAnswers = ia; }                 
   };
   
} // end of dict namespace

#endif // DICTIONARY_H_  
