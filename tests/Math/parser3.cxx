/*!
 * \file   parser3.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   20 déc 2008
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include<cmath>
#include<cassert>
#include<cstdlib>

#include"TFEL/Math/Evaluator.hxx"

int
main(void)
{
  using namespace std;
  using namespace tfel::utilities;
  using namespace tfel::math;
  using namespace tfel::math::parser;
  vector<string> var(1,"x");
  Evaluator f(var,"diff(cos(x),x)");
  SmartPtr<ExternalFunction> df = f.differentiate(0);
  df->setVariableValue(0,2.);
  assert(abs(df->getValue()+cos(2.))<1.e-14);
  return EXIT_SUCCESS;
}
