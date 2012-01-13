/*!
 * \file   derivate.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   16 jan 2007
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include<cmath>
#include<cassert>
#include<cstdlib>

#include"TFEL/Math/functions.hxx"

#ifndef M_PI_2
#define M_PI_2		1.57079632679489661923	/* pi/2 */
#endif /* M_PI_2 */

template<typename Func>
void
compute(Func f,double value,
	const double v0,
	const double v1,
	const double eps)
{
  using namespace std;
  using tfel::math::stdfunctions::D;
  assert(abs(f(value)-v0)<eps);
  assert(abs(D[f](value)-v1)<eps);
}

int main(void)
{
  using namespace std;
  using namespace tfel::math;
  USING_TFEL_FUNCTIONS;

  compute(sin,0.,0.,1.,1.e-14);
  compute(cos,0.,1.,0.,1.e-14);
  compute(cos+sin,0.,1.,1.,1.e-14);
  compute(sin,M_PI_2,1.,0.,1.e-14);
  compute(cos,M_PI_2,0.,-1.,1.e-14);
  compute(cos+sin,M_PI_2,1.,-1.,1.e-14);

  compute(sin/cos,0.,0.,1.,1.e-14);
  compute(id,1.,1.,1.,1.e-14);
  compute(Cst<3,2u>(),1.,1.5,0.,1.e-14);
  compute(exp[12.*id],1.,std::exp(12.),12*std::exp(12.),1.e-14);
  compute(power<2>(),1.,1.,2.,1.e-14);
  compute(power<2>(sin),1.,std::sin(1.)*std::sin(1.),2*std::cos(1.)*std::sin(1.),1.e-14);

  return EXIT_SUCCESS;
}

