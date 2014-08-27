/*! 
 * \file   hill.cxx
 * \brief
 * \author Helfer Thomas
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include<cstdlib>

#include"TFEL/Math/stensor.hxx"
#include"TFEL/Math/st2tost2.hxx"
#include"TFEL/Material/Hill.hxx"

int main(void)
{
  using namespace std;
  using namespace tfel::math;
  using namespace tfel::material;
  
  st2tost2<3u,double> H(hillTensor<3u>(2.,3.,4.,5.,6.,7.));
  stensor<3u,double> s(1.);
  //   cout << "H : " << endl;
  //   cout << (s|H*s) << endl;

  return EXIT_SUCCESS;
}
