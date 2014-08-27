/*!
 * \file   MultipleTestOutputs.cxx
 * \author Helfer Thomas
 * \date   10 Apr 10
 * \brief  
 */

#include<fstream>
#include<stdexcept>

#include"TFEL/Tests/MultipleTestOutputs.hxx"

namespace tfel
{

  namespace tests
  {

    MultipleTestOutputs::MultipleTestOutputs()
    {} // end of MultipleTestOutputs::MultipleTestOutputs

    void
    MultipleTestOutputs::addTestOutput(const TestOutputPtr& o)
    {
      this->outputs.push_back(o);
    } // end of MultipleTestOutputs::addTestOutput
    
    void
    MultipleTestOutputs::beginTestSuite(const std::string& n)
    {
      using namespace std;
      vector<TestOutputPtr>::iterator p;
      for(p=this->outputs.begin();p!=this->outputs.end();++p){
	(*p)->beginTestSuite(n);
      }
    } // end of MultipleTestOutputs::beginTestSuite

    void
    MultipleTestOutputs::addTest(const std::string& g,
				 const std::string& n,
				 const TestResult& r)
    {
      using namespace std;
      vector<TestOutputPtr>::iterator p;
      for(p=this->outputs.begin();p!=this->outputs.end();++p){
	(*p)->addTest(g,n,r);
      }
    } // end of MultipleTestOutputs::addTest
     
    void
    MultipleTestOutputs::endTestSuite(const TestResult& r)
    {
      using namespace std;
      vector<TestOutputPtr>::iterator p;
      for(p=this->outputs.begin();p!=this->outputs.end();++p){
	(*p)->endTestSuite(r);
      }
    } // end of MultipleTestOutputs::endTestSuite
  
    MultipleTestOutputs::~MultipleTestOutputs()
    {} // end of MultipleTestOutputs::~MultipleTestOutputs

  } // end of namespace tests

} // end of namespace tfel
