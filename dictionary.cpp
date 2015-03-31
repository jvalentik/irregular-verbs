//dictionary.cpp - methods of Dictionary 2.1 class 13-07-09 22:06
#include "dictionary.h"
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <iomanip>

using std::cout;
using std::string;
using std::vector;

namespace dict {
   namespace {
      class FirstWith {
         protected:
            char m_Character;
         public:
            explicit FirstWith(char c);
            virtual ~FirstWith() {}
            virtual bool operator()(const string & str) const;
      };
      FirstWith::FirstWith(char c) {
         m_Character = std::tolower(c);
      }
      
      bool FirstWith::operator()(const string & str) const {
         return std::tolower(str[0]) == m_Character;
      }
      
      class LastWith : public FirstWith {
         public:
            explicit LastWith(char c) : FirstWith(c) {}
            virtual bool operator()(const string & str) const;
      };
      
      bool LastWith::operator()(const string & str) const {
         return std::tolower(str[0]) != m_Character;
      } 
      
      std::size_t RanGen(std::size_t size) {
         std::srand(std::time(0));
         return std::rand() % (size -1);
      }                     
   } // end of unnamed namespace

   const string & VerbItem::GetWord(int what) const
                            throw(excp::invalid_value) {
      if ((what != INFINIT) && (what != PAST) && 
          (what != PARTICIP) && (what != TRANS)) {
         throw excp::invalid_value("Invalid value: ", what);
      }
      return (what == INFINIT) ? m_Infinitive : (what == PAST) ? 
              m_PastTense : (what == PARTICIP) ? 
              m_PastParticiple : m_Translation;
   }
   
   void VerbItem::ShowAll() const throw() {   
      using std::setw; 
      using std::endl; 
      const int WIDTH(30);
      const int LINES(5);
      cout << std::left;
      register int i;
      for (i = 0; i < LINES; i++) 
         cout << endl;
      cout << setw(WIDTH) << m_Infinitive
           << setw(WIDTH) << m_PastTense 
           << setw(WIDTH) << m_PastParticiple << endl;
      cout << endl;
      cout << setw(WIDTH) << " " << setw(50) << m_Translation << endl;
      for (i = 0; i < LINES; i++) 
         cout << endl;
   }
   
   bool VerbItem::operator==(const VerbItem & v2) const throw() {
      return m_Infinitive == v2.m_Infinitive;
   }
   
   Dictionary::Dictionary(const char * fn_dc, const char * fn_tr) 
                                            throw(excp::bad_file) {      
      try {       
         LoadDict(fn_dc, fn_tr);
      }
      catch (excp::bad_file & bf) {
         throw bf;
      }
      m_IncorrectAnswers.reserve(5);
      m_Position = m_IncorPos = 0;      
      m_ChosenChar = 'a';
      m_IsSorted = true;         
   } 
   
   void Dictionary::LoadDict(const char * fn_dc, const char * fn_tr) 
                                               throw(excp::bad_file) {
      const char WORD_SEPARATOR(';');
      bool use_trans(true);
      if (std::strcmp(fn_tr, "") == 0)
         use_trans = false;      
      std::ifstream fin_eng(fn_dc);
      if (!fin_eng.is_open())
         throw excp::bad_file(fn_dc);
      std::ifstream fin_trans;
      if (use_trans) {
         fin_trans.open(fn_tr);      
         if (!fin_trans.is_open())
            throw excp::bad_file(fn_tr, " is missing or is corrupted!\n"
                                 "Use -notrans mode!\n");
      }      
      string infinitive, tense, particip, trans = " ";
      getline(fin_eng, infinitive, WORD_SEPARATOR);      
      getline(fin_eng, tense, WORD_SEPARATOR);
      getline(fin_eng, particip);
      if (use_trans)
         getline(fin_trans, trans);
      while (fin_eng && fin_trans) {
         m_Infinitives.push_back(infinitive);
         m_Verbs.insert(Pair(infinitive, tense));
         m_Verbs.insert(Pair(infinitive, particip));
         m_Verbs.insert(Pair(infinitive, trans));
         getline(fin_eng, infinitive, WORD_SEPARATOR);      
         getline(fin_eng, tense, WORD_SEPARATOR);
         getline(fin_eng, particip);
         if (use_trans)
            getline(fin_trans, trans);
      }
      if (use_trans) {
         if (fin_eng.eof() && !fin_trans.eof()) {         
            throw excp::bad_file(fn_dc);         
         }
         if (!fin_eng.eof() && fin_trans.eof()) {         
            throw excp::bad_file(fn_tr); 
         }
      }
      else {
         if (!fin_eng.eof())
            throw excp::bad_file(fn_dc);
      }   
      fin_eng.close();
      if (use_trans)
         fin_trans.close();  
      sort(m_Infinitives.begin(), m_Infinitives.end());          
   }   
   
   const string & Dictionary::GetInfinitive(char group)  
                             throw(excp::invalid_value) {
      const int ASCII_A(97);
      const int ASCII_W(119);
      if (int(group) < ASCII_A || int(group) >  ASCII_W)
         throw excp::invalid_value("Doesn`t contain: ", group);
      if (!m_IsSorted) {
         sort(m_Infinitives.begin(), m_Infinitives.end());
         m_IsSorted = true;
      }           
      vector<string>::iterator iter_low, iter_high;
      iter_low = find_if(m_Infinitives.begin(), 
                         m_Infinitives.end(), FirstWith(group));
      iter_high = find_if(iter_low, m_Infinitives.end(), LastWith(group));
      if (iter_low == m_Infinitives.end())
         throw excp::invalid_value("Doesn`t contain: ", group);
      if (m_ChosenChar != group) {
         m_ChosenChar = group;
         m_Position = iter_low - m_Infinitives.begin();
      } 
      while (true) {          
         if (m_Position++ < (iter_high - m_Infinitives.begin())) {                
            return m_Infinitives[m_Position - 1];
         }
         else {
            m_Position = iter_low - m_Infinitives.begin();
            continue;         
         }
      }
   }
   
   const string & Dictionary::GetInfinitive() throw() {
      if (!m_IsSorted) {
         sort(m_Infinitives.begin(), m_Infinitives.end());
         m_IsSorted = true;
      }
      while (true) {
         if (m_Position++ < m_Infinitives.size()) {
            return m_Infinitives[m_Position - 1];
         }
         else {
            m_Position = 0;
            continue;
         }
      }
   }
   
   const string & Dictionary::GetRmInfinitive() throw() {                  
      if (m_IsSorted) {
         random_shuffle(m_Infinitives.begin(), m_Infinitives.end(), RanGen);
         random_shuffle(m_IncorrectAnswers.begin(), m_IncorrectAnswers.end());
         m_IsSorted = false;
      }
      if (!m_IncorrectAnswers.empty()) {
         if (m_IncorPos++ < m_IncorrectAnswers.size()) {            
            return m_IncorrectAnswers[m_IncorPos - 1];
         }
      }
      while (true) {
         if (m_Position++ < m_Infinitives.size()) {                       
            return m_Infinitives[m_Position - 1];
         }
         else {
            m_Position = 0;
            continue;
         }
      }            
   }
   
   void Dictionary::Reset(int pos) {
      m_Position = pos;
      sort(m_Infinitives.begin(), m_Infinitives.end());
      m_IsSorted = true;
   }
      
   std::auto_ptr<VerbItem> Dictionary::WrapVerbs(const string & infin) const 
                                               throw(excp::invalid_value) {
      if (m_Verbs.find(infin) == m_Verbs.end())
         throw excp::invalid_value("Invalid key: " ,infin[0]);
      Storage::const_iterator iter;
      std::pair<Storage::const_iterator, Storage::const_iterator> range = 
                m_Verbs.equal_range(infin);
      iter = range.first;
      string tense = (*iter).second;
      iter++;
      string particip = (*iter).second;
      iter++;
      string trans = (*iter).second;      
      std::auto_ptr<VerbItem> 
           p_res(new VerbItem(infin, tense, particip, trans, m_Position - 1));
      return p_res;      
   } 
   
   std::auto_ptr<VerbItem> Dictionary::GetVerb(char group)  
                                        throw(excp::invalid_value) {
      return WrapVerbs(GetInfinitive(group));
   }
   
   std::auto_ptr<VerbItem> Dictionary::GetVerb() throw() {
      return WrapVerbs(GetInfinitive());
   }      
   
   std::auto_ptr<VerbItem> Dictionary::GetRmVerb() throw() {
      return WrapVerbs(GetRmInfinitive());
   }     
} // end of dict namespce
