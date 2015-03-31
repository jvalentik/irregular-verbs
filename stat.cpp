//stat.cpp - methods of Statistic 2.0 class 14-07-09 23:31
#include "stat.h"
#include <algorithm>
#include <iomanip>

using std::cout;
using std::endl;
using std::vector;
using std::string;

namespace sta {
   FormatedTime::FormatedTime(long sec) {
      m_Seconds = sec % TIME_UNIT;
      int mins = sec / TIME_UNIT;
      m_Minutes = mins % TIME_UNIT;
      m_Hours = mins / TIME_UNIT;
   }
   
   std::ostream & operator<<(std::ostream & os, const FormatedTime & ft) {
      os << ft.m_Hours << ":" << ft.m_Minutes << ":" << ft.m_Seconds;
      return os;
   }
   BaseStat::BaseStat() {
      PWJ::CurrentDate date;
      PWJ::CurrentTime time;
      m_pDate = new PWJ::DateTime(date, time);      
   }
   
   BaseStat::BaseStat(const std::tm * p_Tm) {
      m_pDate = new PWJ::DateTime(p_Tm);   
   }
   
   BaseStat::BaseStat(const BaseStat & c_Bs) {
      m_pDate = new PWJ::DateTime(*c_Bs.m_pDate);      
   }
   
   BaseStat & BaseStat::operator=(const BaseStat & b) {
      if (this != &b) {
         delete m_pDate;
         m_pDate = new PWJ::DateTime(*b.m_pDate);         
      }
      return *this;
   }
   
   BaseStat::~BaseStat() {
      delete m_pDate;
   }
   
   void BaseStat::Write(std::ostream & os) const {
      m_pDate->WriteAll(os);      
   }
   
   void BaseStat::Read(std::istream & is) throw(excp::bad_file) {
      std::tm temp;
      is.read((char *) &temp, sizeof(std::tm));
      if (is.fail())          
         throw excp::bad_file("Error during data loading!BaseStat\n");
      delete m_pDate;
      m_pDate = new PWJ::DateTime(&temp);
   }
   
   void BaseStat::Reset() {
      delete m_pDate;
      PWJ::CurrentDate date;
      PWJ::CurrentTime time;
      m_pDate = new PWJ::DateTime(date, time);
   }      
   
   void BaseStat::Data() const {
      cout << "Statistics from " << *m_pDate << endl;      
   }
   
   std::ostream & operator<<(std::ostream & os, const BaseStat & bs) {
      bs.ShowAll();
      return os;
   }
   
   LearningStat::LearningStat() : BaseStat() {
      m_pStops = new stp::StopWatch;
      m_LearnedVerbs = m_DictPosition = 0;
      m_MeanTime = 0L;
      m_Stopping = false;
   }
   
   LearningStat::LearningStat(const std::tm * p_Tm, long mt, int lv, int dp) 
                 : BaseStat(p_Tm) {
      m_pStops = new stp::StopWatch;
      m_LearnedVerbs = lv;
      m_MeanTime = mt;
      m_DictPosition = dp;
      m_Stopping = false;
   }
   
   LearningStat::LearningStat(const LearningStat & c_Ls) : BaseStat(c_Ls) {
      m_pStops = new stp::StopWatch(*c_Ls.m_pStops);
      m_LearnedVerbs = c_Ls.m_LearnedVerbs;
      m_DictPosition = c_Ls.m_DictPosition;
      m_MeanTime = c_Ls.m_MeanTime;
      m_Stopping = c_Ls.m_Stopping;
   }
      
   LearningStat & LearningStat::operator=(const LearningStat & ls) {
      if (this != &ls) {
         BaseStat::operator=(ls);
         delete m_pStops;
         m_pStops = new stp::StopWatch(*ls.m_pStops);
         m_LearnedVerbs = ls.m_LearnedVerbs;
         m_DictPosition = ls.m_DictPosition;
         m_MeanTime = ls.m_MeanTime;
         m_Stopping = ls.m_Stopping;
      }
      return *this;
   }
   
   LearningStat::~LearningStat() {
      delete m_pStops;
   }
   
   void LearningStat::Write(std::ostream & os) const {
      auto long time;
      if (m_Stopping) 
         time = GetRunningTime();
      else
         time = m_MeanTime;      
      os.write((char *) &time, sizeof(long));
      os.write((char *) &m_LearnedVerbs, sizeof(int));
      os.write((char *) &m_DictPosition, sizeof(int));
   }
   
   void LearningStat::Read(std::istream & is) throw(excp::bad_file) {
      is.read((char *) &m_MeanTime, sizeof(long));
      is.read((char *) &m_LearnedVerbs, sizeof(int));
      is.read((char *) &m_DictPosition, sizeof(int));
      if (is.fail())
         throw excp::problem("Error during data loading!LearningStat\n");
   }
   
   void LearningStat::Data() const { 
      cout << "Time of learning: " 
           << ((m_Stopping == true) ? FormatedTime(GetRunningTime())
                                  : FormatedTime(m_MeanTime)) << endl;           
      cout << "Learned verbs: " << m_LearnedVerbs << endl;
   }
   
   void LearningStat::Reset() {
      m_MeanTime = 0L;
      m_LearnedVerbs = m_DictPosition = 0;
      m_Stopping = false;
   }
      
   void LearningStat::StartTime() {
      m_Stopping = true;
      m_pStops->Reset();
   }
   
   void LearningStat::StopTime() {
      m_MeanTime += m_pStops->Reset();
      m_Stopping = false;
   }
   
   long LearningStat::GetRunningTime() const {
      return m_pStops->GetTimer();
   }
         
   void LearningStat::OneLearned(int pos) {
      m_LearnedVerbs++;
      m_DictPosition = pos;
   }
   
   TestingStat::TestingStat() : BaseStat() {
      m_pStops = new stp::StopWatch;
      m_IncorrectTries = m_OverallTries = 0;
      m_MeanTime = 0L;
      m_Stopping = false;
      m_IncorrectAnswers.reserve(5);
   }
   
   TestingStat::TestingStat(const std::tm * p_Tm, long mt, int it,
                                int ot, const vector<string> & ia)
                : BaseStat(p_Tm) {
      m_pStops = new stp::StopWatch;
      m_IncorrectAnswers = ia;
      m_MeanTime = mt;
      m_IncorrectTries = it;
      m_OverallTries = ot;
      m_Stopping = false;
   }
   
   TestingStat::TestingStat(const TestingStat & ts) : BaseStat(ts) {
      m_pStops = new stp::StopWatch(*ts.m_pStops);
      m_IncorrectAnswers = ts.m_IncorrectAnswers;
      m_MeanTime = ts.m_MeanTime;
      m_IncorrectTries = ts.m_IncorrectTries;
      m_OverallTries = ts.m_OverallTries;
      m_Stopping = ts.m_Stopping;
   }
   
   TestingStat & TestingStat::operator=(const TestingStat & ts) {
      if (this != &ts) {
         BaseStat::operator=(ts);
         delete m_pStops;
         m_pStops = new stp::StopWatch(*ts.m_pStops);
         m_IncorrectAnswers.clear();
         m_IncorrectAnswers = ts.m_IncorrectAnswers;
         m_MeanTime = ts.m_MeanTime;
         m_IncorrectTries = ts.m_IncorrectTries;
         m_OverallTries = ts.m_OverallTries;
         m_Stopping = ts.m_Stopping;
      }
      return *this;
   }
   
   TestingStat::~TestingStat() {
      delete m_pStops;
   }
   
   namespace {
      class Store {
         private:
            std::ostream & m_Fout;
         public:
            explicit Store(std::ostream & os) : m_Fout(os) {}
            void operator()(const string & str) const;
      };
      
      void Store::operator()(const string & str) const {
         auto std::size_t len = str.size();         
         m_Fout.write((char *) &len, sizeof(std::size_t));
         m_Fout.write(str.data(), len);
      }
      
      void ShowString(const string & str) {
         cout << str << endl;
      }
   } // end of unnamed namespace
   
   void TestingStat::Write(std::ostream & os) const { 
      auto long time;
      if (m_Stopping) 
         time = GetRunningTime();
      else
         time = m_MeanTime;     
      os.write((char *) &time, sizeof(long));
      os.write((char *) &m_OverallTries, sizeof(int));
      os.write((char *) &m_IncorrectTries, sizeof(int));
      auto std::size_t size = m_IncorrectAnswers.size();
      os.write((char *) &size, sizeof(std::size_t));
      if (size) {
         for_each(m_IncorrectAnswers.begin(), 
                  m_IncorrectAnswers.end(),Store(os));
      }
   }
   
   void TestingStat::Read(std::istream & is) throw(excp::bad_file) {
      is.read((char *) &m_MeanTime, sizeof(long));
      is.read((char *) &m_OverallTries, sizeof(int));
      is.read((char *) &m_IncorrectTries, sizeof(int));
      auto std::size_t size;
      is.read((char *) &size, sizeof(std::size_t));
      if (size) {
         char ch;
         for (int i(0); i < size; i++) {
            string ans;
            std::size_t len;
            is.read((char *) &len, sizeof(std::size_t));
            for (int j(0); j < len; j++) {
               is.read(&ch, sizeof(char));
               ans += ch;
            }
            m_IncorrectAnswers.push_back(ans);
         }
      }
      if (is.fail())
         throw excp::problem("Error during data loading!TestingStat\n");
   }
   
   void TestingStat::Data() const {
      cout << "Time of testing: " 
           << ((m_Stopping == true) ? FormatedTime(GetRunningTime())
                                  : FormatedTime(m_MeanTime)) << endl;            
      cout << m_IncorrectTries << " incorrect answers out of "
           << m_OverallTries << " answers" << endl;      
      cout << std::showpoint << std::fixed << std::setprecision(2);      
      cout << "Average successfullness is "; 
      cout << ((m_OverallTries == 0) ? double(0)
              : ((m_OverallTries - m_IncorrectTries) / 
                 double(m_OverallTries) * 100.0)) << " %.\n";
      if (!m_IncorrectAnswers.empty()) {
         cout << "Incorrect answers:\n";
         for_each(m_IncorrectAnswers.begin(), m_IncorrectAnswers.end(),
                  ShowString);
      }
   }
   
   void TestingStat::Reset() {
      m_MeanTime = 0L;
      m_OverallTries = m_IncorrectTries = 0;
      m_IncorrectAnswers.clear();
      m_Stopping = false;
   }
   
   void TestingStat::StartTime() {
      m_Stopping = true;
      m_pStops->Reset();
   }
   
   void TestingStat::StopTime() {
      m_MeanTime += m_pStops->Reset();
      m_Stopping = false;
   }
   
   long TestingStat::GetRunningTime() const {
      return m_pStops->GetTimer();
   }
   
   void TestingStat::OneAnswer(const string & ans) {
      m_OverallTries++;
      if (!ans.empty()) {
         m_IncorrectTries++;
         vector<string>::const_iterator iter;
         iter = find(m_IncorrectAnswers.begin(), 
                     m_IncorrectAnswers.end(), ans);
         if (iter == m_IncorrectAnswers.end())
            m_IncorrectAnswers.push_back(ans);
      }
   }
   
   void TestingStat::CorrectWord(const string & word) {
      if (!m_IncorrectAnswers.empty()) {
         vector<string>::iterator iter;
         iter = find(m_IncorrectAnswers.begin(), 
                     m_IncorrectAnswers.end(), word);
         if (iter != m_IncorrectAnswers.end()) {
            m_IncorrectAnswers.erase(iter);
            m_IncorrectTries--; 
         }           
      }
   }
   
   WorkingStat::WorkingStat() : BaseStat() {
      m_pStops = new stp::StopWatch;
      m_MeanTime = 0L;
      m_Stopping = false;
   }
   
   WorkingStat::WorkingStat(const std::tm * p_Tm, long mt) : BaseStat(p_Tm) {
      m_pStops = new stp::StopWatch;
      m_MeanTime = mt;
      m_Stopping = false;
   }
   
   WorkingStat::WorkingStat(const WorkingStat & ws) : BaseStat(ws) {
      m_pStops = new stp::StopWatch(*ws.m_pStops);
      m_MeanTime = ws.m_MeanTime;
      m_Stopping = ws.m_Stopping;
   }
   
   WorkingStat & WorkingStat::operator=(const WorkingStat & ws) {
      if (this != &ws) {
         BaseStat::operator=(ws);
         delete m_pStops;
         m_pStops = new stp::StopWatch(*ws.m_pStops);
         m_MeanTime = ws.m_MeanTime;
         m_Stopping = ws.m_Stopping;
      }
      return *this;
   }
   
   WorkingStat::~WorkingStat() {
      delete m_pStops;
   }
   
   void WorkingStat::Write(std::ostream & os) const {      
      auto long time;
      if (m_Stopping) 
         time = GetRunningTime();
      else
         time = m_MeanTime;         
      os.write((char *) &time, sizeof(long));
   }
   
   void WorkingStat::Read(std::istream & is) throw(excp::bad_file) {
      is.read((char *) &m_MeanTime, sizeof(long));
      if (is.fail() && !is.eof())
         throw excp::problem("Error during data loading!WorkingStat\n");
   }
   
   void WorkingStat::Data() const {      
      cout << "Time of working: " 
           << ((m_Stopping == true) ? FormatedTime(GetRunningTime()+ m_MeanTime)
                                  : FormatedTime(m_MeanTime)) << endl;
   } 
   
   void WorkingStat::Reset() {
      m_MeanTime = 0L;
      m_Stopping = false;
   }
   
   void WorkingStat::StartTime() {
      m_Stopping = true;
      m_pStops->Reset();
   }
   
   void WorkingStat::StopTime() {
      m_MeanTime += m_pStops->Reset();
      m_Stopping = false;
   }
   
   long WorkingStat::GetRunningTime() const {
      return m_pStops->GetTimer();
   } 
   
   void Statistic::Write(std::ostream & os) const {
      LearningStat::Write(os);
      TestingStat::Write(os);
      WorkingStat::Write(os);
   }
   
   void Statistic::Read(std::istream & is) throw(excp::bad_file) {
      LearningStat::Read(is);
      TestingStat::Read(is);
      WorkingStat::Read(is);
   }
   
   void Statistic::Data() const {
      LearningStat::Data();
      TestingStat::Data();
      WorkingStat::Data();
   }
   
   void Statistic::Reset() {
      BaseStat::Reset();      
      LearningStat::Reset();
      TestingStat::Reset();
      WorkingStat::Reset();
   }
   
   void Statistic::StartTime() {
      LearningStat::StartTime();
      TestingStat::StartTime();
      WorkingStat::StartTime();
   }
   
   void Statistic::StopTime() {
      LearningStat::StopTime();
      TestingStat::StopTime();
      WorkingStat::StopTime();
   }
   
   long Statistic::GetRunningTime() const {
      return WorkingStat::GetRunningTime();
   }
   
   void Statistic::Save(std::ostream & os) const {
      BaseStat::Write(os);
      Write(os);
   }
   
   void Statistic::Load(std::istream & is) throw(excp::bad_file) {
      BaseStat::Read(is);
      Read(is);
   }
   
   void Statistic::ShowAll() const {
      BaseStat::Data();
      Data();
   }
   
   void Statistic::StartTiming(int which) {
      switch (which) {
         case LEARNING:
            LearningStat::StartTime();
            break;         
         case TESTING:
            TestingStat::StartTime();
            break;
         case WORKING:
            WorkingStat::StartTime();
            break;
         case ALL:
            StartTime();
            break;
         default:
            std::cerr << "Incorrect statistic specifier!\n";
      }
   }
   
   void Statistic::StopTiming(int which) {
      switch (which) {
         case LEARNING:
            LearningStat::StopTime();
            break;         
         case TESTING:
            TestingStat::StopTime();
            break;
         case WORKING:
            WorkingStat::StopTime();
            break;
         case ALL:
            StopTime();
            break;
         default:
            std::cerr << "Incorrect statistic specifier!\n";
      }
   }
   
   long Statistic::GetRunTime(int which) const {
      long time;
      switch (which) {
         case LEARNING:
            time = LearningStat::GetRunningTime();
            break;         
         case TESTING:
            time = TestingStat::GetRunningTime();
            break;
         case WORKING:
            time = WorkingStat::GetRunningTime();
            break;         
         default:
            std::cerr << "Incorrect statistic specifier!\n";
      }
   }  
   
   void Statistic::ResetStat(int which) {
      switch (which) {
         case LEARNING:
            LearningStat::Reset();
            break;         
         case TESTING:
            TestingStat::Reset();
            break;
         case WORKING:
            WorkingStat::Reset();
            break;
         case ALL:
            Reset();
            break;
         default:
            std::cerr << "Incorrect statistic specifier!\n";
      }
   }       
} // end of namespace sta
