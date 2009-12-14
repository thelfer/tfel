/*!
 * \file   SFTMCV.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   16 oct 2008
 */

#include<iostream>
#include<cstdlib>

#include"Math/Stensor/SFTMCV.hxx"
#include"Math/Stensor/SFTMRV.hxx"
#include"Math/Stensor/SFTVV.hxx"

int
main(void)
{
  using namespace std;
  using namespace tfel::math;
  tmatrix<3,2> m(0.);
  SFTMCV<1,3,2,0,0>::type ms(m);
  SFTMCV<1,3,2,0,1>::type ms2(m);
  stensor<1> s(1.);
  ms(0) = 1.;
  ms(1) = 2.;
  ms(2) = 3.;
  cout << m << endl;
  s += ms;
  cout << s << endl;
  ms2 = ms + 2. * s;
  cout << m << endl;
} // end of main
