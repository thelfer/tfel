/*! 
 * \file  krigeage.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 09 avr 2009
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include<fstream>
#include<cstdlib>

#ifdef HAVE_FENV
#include <fenv.h>
#endif /* HAVE_FENV */

#include"Math/FactorizedKriging.hxx"

double
rand(const double min,
     const double max)
{
  return min+((max-min)*rand())/RAND_MAX;
}

int main () {
  using namespace std;
  using namespace tfel::math;
  FactorizedKriging<1,1> k;
  double x;
  double t;
  double f;
  unsigned int i;
  ofstream data("data.txt");
  ofstream out("out.txt");

#ifdef HAVE_FENV
  feenableexcept(FE_DIVBYZERO); //	division by zero
  feenableexcept(FE_UNDERFLOW); //	result not representable due to underflow
  feenableexcept(FE_OVERFLOW);  //	result not representable due to overflow
  feenableexcept(FE_INVALID);   //	invalid operation
#endif /* HAVE_FENV */

  for(i=0;i!=500;++i){
    x = rand(0.,1.);
    t = rand(0.,1.);
    f = cos(x+t)*exp(x);
    k.addValue(x,t,f);
    data << x << " " << t << " " << f << endl;
  }
  k.buildInterpolation();
  for(x=-0.5;x<1.5;x+=0.05){
    for(t=-0.5;t<1.5;t+=0.05){
      out << x << " " << t << " " << k(x,t) << endl;
    }
  }

  return EXIT_SUCCESS;
}
