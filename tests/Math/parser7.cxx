/*! 
 * \file  parser.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 17 jun 2010
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include<iostream>

#include<cmath>
#include<limits>
#include<cstdlib>
#include<cassert>

#include"TFEL/Tests/Test.hxx"
#include"TFEL/Tests/TestManager.hxx"
#include"TFEL/Tests/TestFunctionWrapper.hxx"
#include"TFEL/System/ExternalFunctionsPrototypes.hxx"

#include"TFEL/Math/Evaluator.hxx"

template<tfel::system::CFunction1Ptr fct>
struct TestFunction
  : public tfel::tests::Test
{
  
  TestFunction(const std::string& f_,
	       const double xa_,
	       const double xb_,
	       const double dx_,
	       const double e_ = std::numeric_limits<double>::epsilon())
    : f(f_),
      xa(xa_),
      xb(xb_),
      dx(dx_),
      e(e_)
  {
    assert(xa<xb);
    assert(dx>0);
    assert(e>0);
  } // end of TestFunction

  std::string
  name(void) const
  {
    return f;
  }

  tfel::tests::TestResult
  execute(void)
  {
    using namespace std;
    using namespace tfel::math;
    Evaluator ev(this->f+"(x)");
    double x;
    for(x=this->xa;x<=this->xb;x+=this->dx){
      ev.setVariableValue("x",x);
      if(abs(fct(x)-ev.getValue())>e){
	return false;
      }
    }
    return true;
  } // end of execute

private:

  const std::string f;
  const double xa;
  const double xb;
  const double dx;
  const double e;

}; // end of struct TestFunction

#define TFEL_MATH_PARSER7_TESTFUNCTION(F,XA,XB,DX) \
  manager.addTest("parser7",shared_ptr<Test>(new TestFunction<F>(#F,XA,XB,DX)))


int main(void)
{
  using namespace std;
  using namespace tfel::tests;
  using namespace tfel::utilities;
  TestManager& manager = TestManager::getTestManager();
  manager.addTestOutput(cout);
  TFEL_MATH_PARSER7_TESTFUNCTION(sin,-1.,1.,0.1);
  TFEL_MATH_PARSER7_TESTFUNCTION(cos,-1.,1.,0.1);
  TFEL_MATH_PARSER7_TESTFUNCTION(tan,-1.,1.,0.1);
  TFEL_MATH_PARSER7_TESTFUNCTION(exp,-1.,1.,0.1);
  TFEL_MATH_PARSER7_TESTFUNCTION(sinh,-1.,1.,0.1);
  TFEL_MATH_PARSER7_TESTFUNCTION(cosh,-1.,1.,0.1);
  TFEL_MATH_PARSER7_TESTFUNCTION(log,0.1,1.0,0.1);
  TFEL_MATH_PARSER7_TESTFUNCTION(log10,0.1,1.0,0.1);
  TFEL_MATH_PARSER7_TESTFUNCTION(asin,-0.5,0.5,0.1);
  TFEL_MATH_PARSER7_TESTFUNCTION(acos,-0.5,0.5,0.1);
  TFEL_MATH_PARSER7_TESTFUNCTION(atan,-0.5,0.5,0.1);
  TestResult r = manager.execute();
  if(!r.success()){
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
} // end of main

