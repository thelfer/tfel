/*!
 * \file   tmatrix1.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   12 avr 2008
 */

#include<iostream>
#include<cstdlib>

#include "Math/tmatrix.hxx"
#include "Math/TinyMatrixSolve.hxx"

int main(void){
  
  using namespace std;
  using namespace tfel::math;

  tmatrix<2,2,double> m;
  tmatrix<2,2,double> m2;

  m(0,0)=0.;
  m(0,1)=1.;
  m(1,0)=2.;
  m(1,1)=3.;
  
  m2 = m;

  cout << m  << endl;
  cout << m2 << endl;


  return EXIT_SUCCESS;

}

