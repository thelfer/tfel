/*!
 * \file   RungeKutta2.ixx  
 * \brief    
 * \author Helfer Thomas
 */

#ifndef _RUNGE_KUTTA2_I_
#define _RUNGE_KUTTA2_I_ 1

#include"Config/TFELConfig.hxx"

#include"Math/tvector.hxx"

namespace tfel{
  
  namespace math{

    template<unsigned int N,typename T, typename Func>
    class RungeKutta2
    {

      T h;
      T t;
      tvector<N,T> y;
      tvector<N,T> k1;

      RungeKutta2(const RungeKutta2&);
      RungeKutta2& operator=(const RungeKutta2&);

    protected:

      tvector<N,T> f;

    public:

      RungeKutta2()
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

      TFEL_MATH_INLINE2 void increm(void){
	static_cast<Func &>(*this).computeF(t,y);
	k1 = y+0.5f*h*f;
	static_cast<Func &>(*this).computeF(t+0.5f*h,k1);
	t += h;
	y += h*f;
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


#endif /* _RUNGE_KUTTA2_I_ */
