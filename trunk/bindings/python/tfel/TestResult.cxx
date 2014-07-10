/*! 
 * \file  TestResult.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 20 sept. 2013
 */

#include<boost/python.hpp>

#include"TFEL/Tests/TestResult.hxx"

void declareTestResult(void)
{
  using namespace boost;
  using namespace boost::python;
  using namespace tfel::tests;
  class_<TestResult>("TestResult")
    ;

} // end of declareExternalLibraryManager
