/*!
 * \file   parser.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   21 sep 2007
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
  Evaluator f(var,"exp(cos(x))");
  SmartPtr<ExternalFunction> df = f.differentiate(0);
  df->setVariableValue(0,2.);
  cout << df->getValue() << endl;
  //   SmartPtr<EvaluatorManager> manager(new EvaluatorManager());

//   cout << "****** defining a" << endl;
//   manager->operator[]("a") = SmartPtr<Evaluator>(new Evaluator("12",manager));
//   cout << "****** defining f" << endl;
//   manager->operator[]("f") = SmartPtr<Evaluator>(new Evaluator(var,"2*exp(a*x)",manager));
//   cout << "****** defining g" << endl;
//   var.push_back("y");
//   manager->operator[]("g") = SmartPtr<Evaluator>(new Evaluator(var,"y*f(x)",manager));
//   cout << "****** getting a value" << endl;
//   cout << manager->operator[]("a")->getValue() << endl;
//   cout << "****** setting f variable value" << endl;
//   manager->operator[]("f")->setVariableValue(0,0.4);
//   cout << "****** getting f value" << endl;
//   cout << manager->operator[]("f")->getValue() << endl;
//   cout << "****** setting g variable value" << endl;
//   manager->operator[]("g")->setVariableValue(0,0.4);
//   manager->operator[]("g")->setVariableValue(1,3);
//   cout << "****** getting g value" << endl;
//   cout << manager->operator[]("g")->getValue() << endl;
//   manager->operator[]("a") = SmartPtr<Evaluator>(new Evaluator("24",manager));
//   cout << "****** getting g value" << endl;
//   cout << manager->operator[]("g")->getValue() << endl;
// //   manager->operator[]("f") = SmartPtr<Evaluator>(new Evaluator("2*cos(x)",manager));
// //   manager->operator[]("g")->setVariableValue(0,12.);
// //   cout << manager->operator[]("g")->getValue() << endl;
  return EXIT_SUCCESS;
}
