/*! 
 * \file   tests/Math/parser8.cxx
 * \brief
 * \author Helfer Thomas
 * \brief  18 jan 2011
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include<cmath>
#include<limits>
#include<cstdlib>
#include<cassert>
#include<sstream>
#include<algorithm>
#include<stdexcept>

#include<iterator>

#include"TFEL/Tests/Test.hxx"
#include"TFEL/Tests/TestCase.hxx"
#include"TFEL/Tests/TestProxy.hxx"
#include"TFEL/Tests/TestManager.hxx"

#include"TFEL/Math/Evaluator.hxx"

struct ParserTest
  : public tfel::tests::TestCase
{
  ParserTest()
    : tfel::tests::TestCase("TFEL/Math",
			    "Parser8")
  {} // end of MyTest
  tfel::tests::TestResult
  execute()
  {
    using namespace std;
    using namespace tfel::math;
    using namespace tfel::utilities;
    using namespace tfel::math::parser;
    const double eps = 1.e-14;
    const std::string f("12*x*y*cos(u)*sin(v)");
    map<string,double> values;
    vector<string> vars;
    vector<string> v1;
    vector<string>::const_iterator p;
    vector<string>::iterator p2;
    map<string,double>::const_iterator p3;
    values["u"] = 3.987;
    values["v"] = 4.32;
    shared_ptr<ExternalFunctionManager> manager(new ExternalFunctionManager());
    shared_ptr<ExternalFunctionManager> fmanager(new ExternalFunctionManager());
    vars.push_back("x");
    vars.push_back("y");
    try{
      Evaluator ev1(f);
      v1 = ev1.getVariablesNames();
      for(p=vars.begin();p!=vars.end();++p){
	p2 = find(v1.begin(),v1.end(),*p);
	if(p2==v1.end()){
	  string msg("unsued variable '"+*p+"'");
	  throw(runtime_error(msg));
	}
	v1.erase(p2);
      }
      for(p=v1.begin();p!=v1.end();++p){
	p3 = values.find(*p);
	if(p3==values.end()){
	  string msg("unknown external value '"+*p+"'");
	  throw(runtime_error(msg));
	}
	ostringstream os;
	os << p3->second;
	fmanager->operator[](*p) = shared_ptr<ExternalFunction>(new Evaluator(os.str())); 
      }
      manager->operator[]("f") = shared_ptr<ExternalFunction>(new Evaluator(vars,f,fmanager)); 
      manager->operator[]("u") = shared_ptr<ExternalFunction>(new Evaluator("1.2"));
      manager->operator[]("v") = shared_ptr<ExternalFunction>(new Evaluator("2."));
      Evaluator ev(vector<string>(),"f(u,v)",manager);
      ev.removeDependencies();
      TFEL_TESTS_ASSERT(ev.getVariablesNames().empty());
      TFEL_TESTS_ASSERT(abs(ev.getValue()-12*2.4*cos(3.987)*sin(4.32))<eps);
    } catch(runtime_error& e){
      cout << e.what() << endl;;
      return false;
    }
    return this->result;
  } // end of execute()
};

TFEL_TESTS_GENERATE_PROXY(ParserTest,"Parser");

int main(void)
{
  using namespace std;
  using namespace tfel::tests;
  using namespace tfel::utilities;
  TestManager& manager = TestManager::getTestManager();
  manager.addTestOutput(cout);
  manager.addXMLTestOutput("parser8.xml");
  TestResult r = manager.execute();
  if(!r.success()){
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
} // end of main

