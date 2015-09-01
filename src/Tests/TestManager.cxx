/*!
 * \file   src/Tests/TestManager.cxx
 * \author Helfer Thomas
 * \date   10 Apr 10
 * \brief  
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<iostream>
#include<stdexcept>

#if !(defined _WIN32 || defined _WIN64 ||defined __CYGWIN__)
#include <sys/time.h> 
#include <unistd.h> 
#include <time.h>
#endif

#include"TFEL/Tests/TestManager.hxx"
#include"TFEL/Tests/XMLTestOutput.hxx"
#include"TFEL/Tests/StdStreamTestOutput.hxx"

namespace tfel
{

  namespace tests
  {
    
    TestManager&
    TestManager::getTestManager()
    {
      static TestManager m;
      return m;
    } // end of TestManager::getTestManager()

    void
    TestManager::addTestOutput(const TestOutputPtr& o)
    {
      if(this->default_outputs.get()==0){
	this->default_outputs = MultipleTestOutputsPtr(new MultipleTestOutputs());
      }
      this->default_outputs->addTestOutput(o);
    } // end of TestManager::addTestOutput

    void
    TestManager::addTestOutput(const std::string& n,
			       const TestOutputPtr& o)
    {
      using namespace std;
      typedef map<string,MultipleTestOutputsPtr>::value_type MVType;
      map<string,MultipleTestOutputsPtr>::iterator p;
      p = this->outputs.find(n);
      if(p==this->outputs.end()){
	MultipleTestOutputsPtr out(new MultipleTestOutputs());
	p = this->outputs.insert(MVType(n,out)).first;
      }
      p->second->addTestOutput(o);
    } // end of TestManager::addTestOutput

    void
    TestManager::addTestOutput(std::ostream& o,
			       const bool b)
    {
      using namespace std;
      TestOutputPtr output(new StdStreamTestOutput(o,b));
      this->addTestOutput(output);
    } // end of TestManager::addTestOutput

    void
    TestManager::addTestOutput(const std::string& n,
			       std::ostream& o,
			       const bool b)
    {
      using namespace std;
      TestOutputPtr output(new StdStreamTestOutput(o,b));
      this->addTestOutput(n,output);
    } // end of TestManager::addTestOutput

    void
    TestManager::addTestOutput(const std::string& f)
    {
      using namespace std;
      TestOutputPtr o(new StdStreamTestOutput(f));
      this->addTestOutput(o);
    } // end of TestManager::addTestOutput

    void
    TestManager::addXMLTestOutput(const std::string& f)
    {
      using namespace std;
      TestOutputPtr o(new XMLTestOutput(f));
      this->addTestOutput(o);
    } // end of TestManager::addTestOutput

    void
    TestManager::addTestOutput(const std::string& n,
			       const std::string& f)
    {
      using namespace std;
      TestOutputPtr o(new StdStreamTestOutput(f));
      this->addTestOutput(n,o);
    } // end of TestManager::addTestOutput

    void
    TestManager::addTest(const std::string& n,
			 const TestPtr& t)
    {
      using namespace std;
      typedef map<string,TestSuitePtr>::value_type MVType;
      map<string,TestSuitePtr>::iterator p;
      p = this->tests.find(n);
      if(p==this->tests.end()){
	TestSuitePtr s(new TestSuite(n));
	p = this->tests.insert(MVType(n,s)).first;
      }
      p->second->add(t);
    } // end of TestManager::addTest

    TestResult
    TestManager::execute(void)
    {
      using namespace std;
      map<string,TestSuitePtr>::iterator p;
      map<string,MultipleTestOutputsPtr>::iterator p2;
      TestResult r;
      for(p=this->tests.begin();p!=this->tests.end();++p){
	MultipleTestOutputsPtr output;
	const string& n = p->first;
	p2 = this->outputs.find(n);
	if(p2!=this->outputs.end()){
	  output = p2->second;
	} else{
	  if(this->default_outputs.get()==0){
	    string msg("TestManager::execute : ");
	    msg += "no output defined for test suite '"+n+"'";
	    throw(runtime_error(msg));
	  }
	  output = this->default_outputs;
	}
#ifdef _POSIX_TIMERS
#if (_POSIX_TIMERS != 0) && (! defined __APPLE__)
	timespec start;
	timespec stop;
	if (clock_gettime(CLOCK_MONOTONIC,&start) == -1){
	  throw(runtime_error("TestManager::execute : "
			      "invalid call to 'clock_gettime'"));
	}
#endif
#endif
	r.append(p->second->execute(*output));
#ifdef _POSIX_TIMERS
#if (_POSIX_TIMERS != 0) && (! defined __APPLE__)
	if (clock_gettime(CLOCK_MONOTONIC,&stop) == -1){
	  throw(runtime_error("TestManager::execute : "
			      "invalid call to 'clock_gettime'"));
	}
	r.setTestDuration((stop.tv_sec+1.e-9*stop.tv_nsec)-
			  (start.tv_sec+1.e-9*start.tv_nsec));
#endif
#endif
      }
      return r;
    } // end of TestManager::execute()

    TestManager::TestManager()
    {} // end of TestManager::~TestManager

  } // end of namespace tests

} // end of namespace tfel
