/*!
 * \file   test.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   20 jui 2006
 */

#include<iostream>
#include<cstdlib>
#include<cassert>

#include"Utilities/Name.hxx"

#include"TypeTraits/Promote.hxx"
#include"Math/General/BasicOperations.hxx"

int main(void)
{
  using namespace std;
  using namespace tfel::typetraits;
  using namespace tfel::math;
  using namespace tfel::utilities;

  cout << Name<Promote<float,long double>::type>::getName() << endl; 
  cout << Name<Promote<long double,float>::type>::getName() << endl; 
  cout << Name<ResultType<long double,float,OpMult>::type>::getName() << endl; 
  cout << Name<ResultType<float,long double,OpMult>::type>::getName() << endl; 

  cout << "success" << endl;

  return EXIT_SUCCESS;
}
