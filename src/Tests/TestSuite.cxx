/*!
 * \file   TestSuite.cxx
 * \author Helfer Thomas <helferthomas@free.fr>
 * \date   10 Apr 10
 * \brief  
 */

#if !(defined _WIN32 || defined _WIN64 ||defined __CYGWIN__)
#include<sys/time.h>
#endif

#include<iostream>
#include<stdexcept>

#include"TFEL/Tests/TestSuite.hxx"

namespace tfel
{

  namespace tests
  {
    
    TestSuite::TestSuite()
      : tests(),
	sname()
    {} // end of TestSuite::TestSuite

    TestSuite::TestSuite(const std::string& n)
      : tests(),
	sname(n)
    {} // end of TestSuite::TestSuite

    TestSuite::TestSuite(const char * const n)
      : tests(),
	sname(n)
    {} // end of TestSuite::TestSuite

    std::string
    TestSuite::name(void) const
    {
      return this->sname;
    } // end of TestSuite::name

    void
    TestSuite::add(const TestSuite::TestPtr& t)
    {
      this->tests.push_back(t);
    } // end of TestSuite::execute

    TestResult
    TestSuite::execute(void)
    {
      using namespace std;
      vector<TestPtr>::iterator p;
      TestResult r;
      bool success =true;
      for(p=this->tests.begin();p!=tests.end();++p){
	TestResult r1;
#ifdef _POSIX_TIMERS
#if _POSIX_TIMERS != 0
	timespec start;
	timespec stop;
	if (clock_gettime(CLOCK_MONOTONIC,&start) == -1){
	  throw(runtime_error("TestManager::execute : "
			      "invalid call to 'clock_gettime'"));
	}
#endif
#endif
	try{
	  r1 = (*p)->execute();
	  if(!r1.success()){
	    success = false;
	  }
	} catch(exception& e){
	  string msg("test '"+(*p)->name());
	  msg += "' has thrown an exception (";
	  msg += e.what();
	  msg += ")";
	  cerr << msg << endl;
	  r1 = TestResult(false,msg);
	  success = false;
	} catch(...){
	  string msg("test '"+(*p)->name());
	  msg += "' has thrown an unknown exception";
	  cerr << msg << endl;
	  r1 = TestResult(false,msg);
	  success = false;
	}
#ifdef _POSIX_TIMERS
#if _POSIX_TIMERS != 0
	if (clock_gettime(CLOCK_MONOTONIC,&stop) == -1){
	  throw(runtime_error("TestManager::execute : "
			      "invalid call to 'clock_gettime'"));
	}
	r1.setTestDuration((stop.tv_sec+1.e-9*stop.tv_nsec)-
			   (start.tv_sec+1.e-9*start.tv_nsec));
#endif
#endif
	r.append(r1);
      }	
      return r;
    } // end of TestSuite::execute

    TestResult
    TestSuite::execute(TestOutput& o)
    {
      using namespace std;
      vector<TestPtr>::iterator p;
      o.beginTestSuite(this->name());
      TestResult r;
      bool success =true;
      for(p=this->tests.begin();p!=tests.end();++p){
	TestResult r1;
#ifdef _POSIX_TIMERS
#if _POSIX_TIMERS != 0
	timespec start;
	timespec stop;
	if (clock_gettime(CLOCK_MONOTONIC,&start) == -1){
	  throw(runtime_error("TestManager::execute : "
			      "invalid call to 'clock_gettime'"));
	}
#endif
#endif
	try{
	  r1 = (*p)->execute();
	  if(!r1.success()){
	    success = false;
	  }
	} catch(exception& e){
	  string msg("test '"+(*p)->name());
	  msg += "' has thrown an exception (";
	  msg += e.what();
	  msg += ")";
	  cerr << msg << endl;
	  r1 = TestResult(false,msg);
	  success = false;
	} catch(...){
	  string msg("test '"+(*p)->name());
	  msg += "' has thrown an unknown exception";
	  cerr << msg << endl;
	  r1 = TestResult(false,msg);
	  success = false;
	}
#ifdef _POSIX_TIMERS
#if _POSIX_TIMERS != 0
	if (clock_gettime(CLOCK_MONOTONIC,&stop) == -1){
	  throw(runtime_error("TestManager::execute : "
			      "invalid call to 'clock_gettime'"));
	}
	r1.setTestDuration((stop.tv_sec+1.e-9*stop.tv_nsec)-
			   (start.tv_sec+1.e-9*start.tv_nsec));
#endif
#endif
	o.addTest((*p)->classname(),(*p)->name(),r1);
	r.append(r1);
      }
      o.endTestSuite(TestResult(success));
      return r;
    } // end of TestSuite::execute

    TestSuite::~TestSuite()
    {} // end of TestSuite::~TestSuite()

  } // end of namespace tests

} // end of namespace tfel
