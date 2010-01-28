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

#include"TFEL/Math/General/floating_point_exceptions.hxx"
#include"TFEL/Math/Kriging.hxx"

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
  
  init_floating_point_exceptions();

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
