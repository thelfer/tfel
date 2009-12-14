/*!
 * \file   ST2toST2FTMV.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   17 oct 2008
 */

#include<iostream>
#include<cstdlib>

#include"Math/ST2toST2/ST2toST2FTMV.hxx"

int
main(void)
{
  using namespace std;
  using namespace tfel::math;
  tmatrix<6,6> m(0.);
  ST2toST2FTMV<1,6,6,0,0>::type ms(m);
  ST2toST2FTMV<1,6,6,3,3>::type ms2(m);
  unsigned short i;
  for(i=0;i!=6;++i){
    m(i,i) = 1.;
  }
  cout << ms  << endl;
  cout << ms2 << endl;
}
