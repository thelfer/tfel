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

#include"Math/Kriging.hxx"

double
rand(const double min,
     const double max)
{
  return min+((max-min)*rand())/RAND_MAX;
}

int main () {
  using namespace std;
  using namespace tfel::math;
  Kriging<2> k;
  tvector<2u> v;
  double x;
  double y;
  unsigned int i;
  ofstream data("data.txt");
  ofstream out("out2D.txt");

#ifdef HAVE_FENV
  feenableexcept(FE_DIVBYZERO); //	division by zero
  feenableexcept(FE_UNDERFLOW); //	result not representable due to underflow
  feenableexcept(FE_OVERFLOW);  //	result not representable due to overflow
  feenableexcept(FE_INVALID);   //	invalid operation
#endif /* HAVE_FENV */

  for(i=0;i!=100;++i){
    v(0) = rand(0.,1.);
    v(1) = rand(0.,1.);
    y = cos(v(0)+v(1))*exp(v(0));
    k.addValue(v,y);
    data << v(0) << " " << v(1) << " " << y << endl;
  }
  k.buildInterpolation();
  for(x=-0.5;x<1.5;x+=0.05){
    for(y=-0.5;y<1.5;y+=0.05){
      v(0) = x;
      v(1) = y;
      out << x << " " << y << " " << k(v) << endl;
    }
  }
  
  return EXIT_SUCCESS;
}
