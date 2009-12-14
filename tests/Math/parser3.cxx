/*!
 * \file   parser3.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   20 déc 2008
 */

#include<iostream>
#include<cstdlib>

#include"Math/Evaluator.hxx"

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
  cout << df->getValue() << endl;
  return EXIT_SUCCESS;
}
