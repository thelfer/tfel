/*!
 * \file   parser.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   21 sep 2007
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

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
  Evaluator f(var,"exp(cos(x))");
  SmartPtr<ExternalFunction> df = f.differentiate(0);
  df->setVariableValue(0,2.);
  assert(abs(df->getValue()+sin(2)*exp(cos(2)))<1.e-14);
  
  SmartPtr<ExternalFunctionManager> manager(new ExternalFunctionManager());
  manager->operator[]("a") = SmartPtr<ExternalFunction>(new Evaluator("12",manager));
  manager->operator[]("f") = SmartPtr<ExternalFunction>(new Evaluator(var,"2*exp(a*x)",manager));
  var.push_back("y");
  manager->operator[]("g") = SmartPtr<ExternalFunction>(new Evaluator(var,"y*f(x)",manager));

  assert(abs(manager->operator[]("a")->getValue()-12.)<1.e-14);
  manager->operator[]("f")->setVariableValue(0,0.4);
  assert(abs(manager->operator[]("f")->getValue()-2*exp(12*0.4))<1.e-14);
  manager->operator[]("g")->setVariableValue(0,0.4);
  manager->operator[]("g")->setVariableValue(1,3);
  assert(abs(manager->operator[]("g")->getValue()-3*2*exp(12*0.4))<1.e-14);
  // Changing a function
  manager->operator[]("a") = SmartPtr<ExternalFunction>(new Evaluator("24",manager));
  assert(abs(manager->operator[]("a")->getValue()-24.)<1.e-14);
  assert(abs(manager->operator[]("g")->getValue()-3*2*exp(24*0.4))<1.e-14);
  // Changing f function
  manager->operator[]("f") = SmartPtr<ExternalFunction>(new Evaluator("2*cos(x)",manager));
  manager->operator[]("g")->setVariableValue(0,12.);
  assert(abs(manager->operator[]("g")->getValue()-3*2*cos(12.))<1.e-14);

  return EXIT_SUCCESS;
}
