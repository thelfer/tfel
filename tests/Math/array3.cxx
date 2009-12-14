/*!
 * \file   array3.cxx
 * \brief  
 * \author Helfer Thomas
 * \date   01 jui 2006
 */

#include<iostream>
#include<cstdlib>
#include<cassert>
#include<algorithm>

#include"Math/Array/ArrayRunTimeProperties.hxx"
#include"Math/array.hxx"

int main(void)
{
  using namespace std;
  using namespace tfel::math;

  array<1u>::RunTimeProperties RT(4);
  array<1u>::RunTimeProperties RT2(3);

  array<1u> a(RT,2.);
  array<1u> b(RT2,4.);
  array<1u> c(RT2,4.);

  try{
    c = a+b;
  }
  catch(MathRunTimeException& e){
    cout << e.getMsg() << endl;
  }

  try{
    c = 2*a;
  }
  catch(MathRunTimeException& e){
    cout << e.getMsg() << endl;
  }

  return EXIT_SUCCESS;
}
