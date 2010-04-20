/*!
 * \file   TestManager.cxx
 * \author Helfer Thomas <helferthomas@free.fr>
 * \date   10 Apr 10
 * \brief  
 */

#include<iostream>
#include<stdexcept>

#include"TFEL/Tests/TestManager.hxx"
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
	r.append(p->second->execute(*output));
      }
      return r;
    } // end of TestManager::execute()

    TestManager::TestManager()
    {} // end of TestManager::~TestManager

  } // end of namespace tests

} // end of namespace tfel
