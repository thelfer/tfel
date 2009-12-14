/*!
 * \file   derivate.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   16 jan 2007
 */

#include<iostream>
#include<cstdlib>

#include"Utilities/Name.hxx"
#include"Math/functions.hxx"

#ifndef M_PI_2
#define M_PI_2		1.57079632679489661923	/* pi/2 */
#endif /* M_PI_2 */

template<typename Func>
void
compute(Func f,double value,const std::string& name)
{
  using namespace tfel::utilities;
  using tfel::math::stdfunctions::D;
  std::cout << name    << " = " << f(value) << ", " 
	    << "d(" << name << ") = " << D[f](value) << std::endl;
}

int main(void)
{
  using namespace std;
  using namespace tfel::utilities;
  using namespace tfel::math;
  USING_TFEL_FUNCTIONS;

  compute(sin,0.,"sin");
  compute(cos,0.,"cos");
  compute(cos+sin,0.,"sin+cos");

  compute(sin,M_PI_2,"sin");
  compute(cos,M_PI_2,"cos");
  compute(cos+sin,M_PI_2,"sin+cos");

  compute(sin/cos,0.,"sin/cos");
  compute(id,1.,"Id");
  compute(Cst<3,2u>(),1.,"Cst<3,2>");
  compute(exp[12.*id],1.,"exp");
  compute(power<2>(),1.,"x^2");
  compute(power<2>(sin),1.,"sin^2");

  return EXIT_SUCCESS;
}

