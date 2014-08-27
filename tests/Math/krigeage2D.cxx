/*! 
 * \file   krigeage2D.cxx
 * \brief
 * \author Helfer Thomas
 * \brief  09 avr 2009
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include<fstream>
#include<cstdlib>

#include"TFEL/Math/General/floating_point_exceptions.hxx"
#include"TFEL/Math/Kriging.hxx"
#include"TFEL/Math/FactorizedKriging1D1D.hxx"

double
rand(const double min,
     const double max)
{
  return min+((max-min)*rand())/RAND_MAX;
}

int main () {
  using namespace std;
  using namespace tfel::math;
  using std::vector;
  Kriging<2> k;
  tvector<2u> v;
  vector<double> vx;
  vector<double> vy;
  vector<double> vz;
  double x;
  double y;
  double z;
  unsigned int i;
  ofstream data("data.txt");
  ofstream out("out2D.txt");
  ofstream out2("out2D-2.txt");

  init_floating_point_exceptions();

  for(i=0;i!=100;++i){
    v(0) = rand(0.,1.);
    v(1) = rand(0.,1.);
    z = cos(v(0)+v(1))*exp(v(0));
    k.addValue(v,z);
    vx.push_back(v(0));
    vy.push_back(v(1));
    vz.push_back(z);
    data << v(0) << " " << v(1) << " " << z << endl;
  }
  FactorizedKriging1D1D k2(vx,vy,vz);
  k.buildInterpolation();
  for(x=-0.5;x<1.5;x+=0.05){
    for(y=-0.5;y<1.5;y+=0.05){
      v(0) = x;
      v(1) = y;
      out  << x << " " << y << " " << k(v) << endl;
      out2 << x << " " << y << " " << k2(v(0),v(1)) << endl;
    }
  }
  
  return EXIT_SUCCESS;
}
