/*!
 * \file   TestCase.cxx
 * \author Helfer Thomas <helferthomas@free.fr>
 * \date   10 Apr 10
 * \brief  
 */

#include"TFEL/Tests/TestCase.hxx"

namespace tfel
{

  namespace tests
  {
    
    TestCase::TestCase(const std::string& n)
      : tname(n)
    {} // end of TestCase::~TestCase

    std::string
    TestCase::name(void) const
    {
      return this->tname;
    } // end of TestCase::~TestCase
    
    void
    TestCase::registerResult(const std::string& n,
			     const bool b)
    {
      using namespace std;
      this->result.append(TestResult(b,n));
    } // end of TestCase::registerResult    

    TestCase::~TestCase()
    {} // end of TestCase::~TestCase

  } // end of namespace tests

} // end of namespace tfel
