/*!
 * \file   RungeKutta4.ixx  
 * \brief  This file implements the RungeKutta4 class.
 * \author Helfer Thomas
 * \date   01 Sep 2006
 */

#ifndef _RUNGE_KUTTA4_I_
#define _RUNGE_KUTTA4_I_ 1

#include"TFEL/Config/TFELConfig.hxx"

#include"TFEL/Math/tvector.hxx"

namespace tfel{
  
  namespace math{

    template<unsigned int N,typename T, typename Func>
    class RungeKutta4
    {

      T h;
      T t;
      tvector<N,T> y;
      tvector<N,T> k1;
      tvector<N,T> k2;
      tvector<N,T> k3;
      tvector<N,T> k4;
      tvector<N,T> tmp;

      RungeKutta4(const RungeKutta4&);
      RungeKutta4& operator=(const RungeKutta4&);

    protected:

      tvector<N,T> f;

    public:

      RungeKutta4()
      {};
      
      TFEL_MATH_INLINE2 void set_y(const tvector<N,T>& y_){
	this->y = y_;
      }

      TFEL_MATH_INLINE2 void set_t(const T t_){
	this->t = t_;
      }

      TFEL_MATH_INLINE2 void set_h(const T h_){
	this->h = h_;
      }

      void increm(void){
	static const T Cste_1_3 = static_cast<T>(1.f)/static_cast<T>(3.f);
	static const T Cste_1_6 = static_cast<T>(1.f)/static_cast<T>(6.f);
	static_cast<Func&>(*this).computeF(t,y);
	k1  = h*f;
	tmp = y+0.5f*k1;
	t  += 0.5f*h;
	static_cast<Func&>(*this).computeF(t,tmp);
	k2  = h*f;
	tmp = y+0.5f*k2;
	static_cast<Func&>(*this).computeF(t,tmp);
	k3  = h*f;
	tmp = y+k3;
	t  += 0.5f*h;
	static_cast<Func&>(*this).computeF(t,tmp);
	k4  = h*f;
	y+=Cste_1_6*(k1+k4)+Cste_1_3*(k2+k3);
     }
	
      TFEL_MATH_INLINE2 void exe(const T begin,const T end){
	this->t = begin;
	while(this->t<end){
	  this->increm();
	}
      }

      TFEL_MATH_INLINE2 const tvector<N,T>& get_y(void) const{
	return this->y;
      }

      TFEL_MATH_INLINE2 T get_t(void) const {
	return this->t;
      }

      TFEL_MATH_INLINE2 T get_h(void) const {
	return this->h;
      }

    };
    
  } // end of namespace math

} // end of namespace tfel

#endif /* _RUNGE_KUTTA4_I_ */
