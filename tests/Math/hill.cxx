#include<iostream>
#include<cstdlib>

#include"Math/stensor.hxx"
#include"Math/st2tost2.hxx"
#include"MaterialLaw/Hill.hxx"

int main(void)
{
  using namespace std;
  using namespace tfel::math;
  using namespace tfel::materiallaw;
  
  st2tost2<3u,double> H(hillTensor<3u>(2.,3.,4.,5.,6.,7.));
  stensor<3u,double> s(1.);
  cout << "H : " << endl;
  cout << (s|H*s) << endl;

  return EXIT_SUCCESS;
}
