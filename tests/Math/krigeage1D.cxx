/*! 
 * \file  krigeage.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 09 avr 2009
 */

#include<fstream>
#include<cstdlib>

#include <fenv.h>

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
  Kriging<1> k;
  double x;
  double y;
  unsigned int i;
  ofstream data("data.txt");
  ofstream out("out.txt");

  feenableexcept(FE_DIVBYZERO); //	division by zero
  feenableexcept(FE_UNDERFLOW); //	result not representable due to underflow
  feenableexcept(FE_OVERFLOW);  //	result not representable due to overflow
  feenableexcept(FE_INVALID);   //	invalid operation

  for(i=0;i!=10;++i){
    x = rand(0.,1.);
    y = exp(cos(x*x))+0.1*(rand(0.,1.)-0.5);
    k.addValue(x,y);
    data << x << " " << y << endl;
  }
  k.setNuggetEffect(0.02);
  k.buildInterpolation();
  for(x=-0.5;x<1.5;x+=0.05){
    out << x << " " << k(x) << endl;
  }

  return EXIT_SUCCESS;
}
