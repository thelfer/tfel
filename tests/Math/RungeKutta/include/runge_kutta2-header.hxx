/*!
 * \file   tests/Math/RungeKutta/include/runge_kutta2-header.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   05 fév 2008
 */

#ifndef _LIB_TFEL_RUNGE_KUTTA2_HEADER_HXX_
#define _LIB_TFEL_RUNGE_KUTTA2_HEADER_HXX_ 

#include<cstdlib>
#include<fstream>
#include<string>

#include"TFEL/Utilities/Name.hxx"
#include"TFEL/Math/tvector.hxx"
#include"TFEL/Math/RungeKutta2.hxx"

template<typename T>
class VanDerPol
  : public tfel::math::RungeKutta2<2,T,VanDerPol<T> >
{
  T mu;

  typedef tfel::math::tvector<2,T> vector;

public:

  void set_mu(const T mu_)
  {
    this->mu=mu_;
  }

  TFEL_MATH_INLINE void computeF(const T, const vector& x)
  {
    (this->f)(0) =  x(1);
    (this->f)(1) = -x(0)+mu*x(1)*(1-x(0)*x(0));
  }

};

template<typename T>
void test(void);

template<typename T>
void test(void){

  using namespace std;
  using namespace tfel::math;

  string name("rk2_");
  name += tfel::utilities::Name<T>::getName();
  name += ".txt";
  
  ofstream out(name.c_str());
  VanDerPol<T> rk;
  tvector<2,T> y;
  T begin;
  T end;

  y(0) = 1.f;
  y(1) = 0.f;

  rk.set_mu(10.f);
  rk.set_h(1.e-2f);
  rk.set_y(y);
  
  begin = 0.;
  end   = 100.;

  rk.set_t(begin);

  out.precision(15);
  while(rk.get_t() < end){
    rk.increm();
    out << rk.get_t() << "  " << (rk.get_y())(0) << endl;
  }
}
  
#endif /* _LIB_TFEL_RUNGE_KUTTA2_HEADER_HXX */
