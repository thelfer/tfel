/*!
 * \file   TestSuite.cxx
 * \author Helfer Thomas <helferthomas@free.fr>
 * \date   10 Apr 10
 * \brief  
 */

#include<iostream>

#include"TFEL/Tests/TestSuite.hxx"

namespace tfel
{

  namespace tests
  {
    
    TestSuite::TestSuite()
      : sname()
    {} // end of TestSuite::TestSuite

    TestSuite::TestSuite(const std::string& n)
      : sname(n)
    {} // end of TestSuite::TestSuite

    TestSuite::TestSuite(const char * const n)
      : sname(n)
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
	try{
	  const TestResult& r1 = (*p)->execute();
	  if(!r1.success()){
	    success = false;
	  }
  	  r.append(r1);
	} catch(exception& e){
	  string msg("test '"+(*p)->name());
	  msg += "' has thrown an exception (";
	  msg += e.what();
	  msg += ")";
	  const TestResult res(false,msg);
	  r.append(res);
	  success = false;
	} catch(...){
	  string msg("test '"+(*p)->name());
	  msg += "' has thrown an unknown exception";
	  const TestResult res(false,msg);
	  r.append(res);
	  success = false;
	}
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
	try{
	  const TestResult& r1 = (*p)->execute();
	  if(!r1.success()){
	    success = false;
	  }
  	  r.append(r1);
	  o.addTest((*p)->name(),r1);
	} catch(exception& e){
	  string msg("test '"+(*p)->name());
	  msg += "' has thrown an exception (";
	  msg += e.what();
	  msg += ")";
	  const TestResult res(false,msg);
	  r.append(res);
	  o.addTest((*p)->name(),res);
	  success = false;
	} catch(...){
	  string msg("test '"+(*p)->name());
	  msg += "' has thrown an unknown exception";
	  const TestResult res(false,msg);
	  r.append(res);
	  o.addTest((*p)->name(),res);
	  success = false;
	}
      }	
      o.endTestSuite(TestResult(success));
      return r;
    } // end of TestSuite::execute

    TestSuite::~TestSuite()
    {} // end of TestSuite::~TestSuite()

  } // end of namespace tests

} // end of namespace tfel
