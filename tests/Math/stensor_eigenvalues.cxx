/*!
 * \file   stensor_eigenvalues.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   03 jui 2006
 */

#include<iostream>
#include<cstdlib>

#include "Math/stensor.hxx"

int main(void)
{
  using namespace tfel::math;

  double vp1;
  double vp2;
  double vp3;
  stensor<1,double> s(0.);
  s(0)=1.;
  s(1)=2.5;
  s(2)=0.234;

  s.computeEigenValues(vp1,vp2,vp3);

  std::cout << "Valeurs propres : " << vp1 << " " << vp2 << " " << vp3 << std::endl;

  return EXIT_SUCCESS;
}
