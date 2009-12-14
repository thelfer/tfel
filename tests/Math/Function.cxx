/*!
 * \file   Function.cxx
 * \brief  
 * \author Helfer Thomas
 * \date   05 nov 2006
 */

#include<iostream>
#include<cstdlib>

#include"Math/functions.hxx"

int main(void){

  using namespace std;
  USING_TFEL_FUNCTIONS;

  cout << cos(12.) << endl;
  cout << sin(12.) << endl;

  cout << (cos+sin)(12.) << endl;
  cout << (exp[sin])(12.) << endl;
  cout << (exp[sin])(12.) << endl;
  cout << (2.321*sin)(12.) << endl;
  cout << (sin*3.421)(12.) << endl;

  return EXIT_SUCCESS;
}
