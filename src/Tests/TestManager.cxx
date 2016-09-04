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

#include<chrono>
#include<stdexcept>

#include"TFEL/Tests/TestManager.hxx"
#include"TFEL/Tests/XMLTestOutput.hxx"
#include"TFEL/Tests/StdStreamTestOutput.hxx"

namespace tfel
{

  namespace tests
  {
    
    TestManager& TestManager::getTestManager()
    {
      static TestManager m;
      return m;
    } // end of TestManager::getTestManager()

    void
    TestManager::addTestOutput(TestOutputPtr o)
    {
      if(this->default_outputs.get()==nullptr){
	this->default_outputs = std::make_shared<MultipleTestOutputs>();
      }
      this->default_outputs->addTestOutput(o);
    } // end of TestManager::addTestOutput

    void
    TestManager::addTestOutput(const std::string& n,
			       TestOutputPtr o)
    {
      auto p = this->outputs.find(n);
      if(p==this->outputs.end()){
	MultipleTestOutputsPtr out(new MultipleTestOutputs());
	p = this->outputs.insert({n,out}).first;
      }
      p->second->addTestOutput(o);
    } // end of TestManager::addTestOutput

    void
    TestManager::addTestOutput(std::ostream& o,
			       const bool b)
    {
      TestOutputPtr output(new StdStreamTestOutput(o,b));
      this->addTestOutput(output);
    } // end of TestManager::addTestOutput

    void
    TestManager::addTestOutput(const std::string& n,
			       std::ostream& o,
			       const bool b)
    {
      TestOutputPtr output(new StdStreamTestOutput(o,b));
      this->addTestOutput(n,output);
    } // end of TestManager::addTestOutput

    void
    TestManager::addTestOutput(const std::string& f)
    {
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
      TestOutputPtr o(new StdStreamTestOutput(f));
      this->addTestOutput(n,o);
    } // end of TestManager::addTestOutput

    void
    TestManager::addTest(const std::string& n,TestPtr t)
    {
      auto p = this->tests.find(n);
      if(p==this->tests.end()){
	auto s = std::make_shared<TestSuite>(n);
	p = this->tests.insert({n,s}).first;
      }
      p->second->add(t);
    } // end of TestManager::addTest

    TestResult
    TestManager::execute(void)
    {
      using namespace std;
      using namespace std::chrono;
      TestResult r;
      for(const auto& t: this->tests){
	MultipleTestOutputsPtr output;
	const auto& n = t.first;
	auto p2 = this->outputs.find(n);
	if(p2!=this->outputs.end()){
	  output = p2->second;
	} else{
	  if(this->default_outputs.get()==nullptr){
	    throw(std::runtime_error("TestManager::execute: "
				     "no output defined for "
				     "test suite '"+n+"'"));
	  }
	  output = this->default_outputs;
	}
	const auto start = high_resolution_clock::now();
	r.append(t.second->execute(*output));
	const auto stop = high_resolution_clock::now();
	const auto nsec = duration_cast<nanoseconds>(stop-start).count();
	r.setTestDuration(1.e-9*nsec);
      }
      return r;
    } // end of TestManager::execute()

    TestManager::TestManager() = default;

  } // end of namespace tests

} // end of namespace tfel
