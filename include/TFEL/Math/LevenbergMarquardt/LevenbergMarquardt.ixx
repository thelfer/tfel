/*!
 * \file   LevenbergMarquardt.ixx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   21 nov 2008
 */

#ifndef _LIB_TFEL_MATH_LEVENBERGMARQUARDT_IXX_
#define _LIB_TFEL_MATH_LEVENBERGMARQUARDT_IXX_ 

#include<algorithm>

#include"TFEL/Math/MathException.hxx"
#include"TFEL/Math/Function/Power.hxx"

namespace tfel
{

  namespace math
  {

    template<typename T>
    LevenbergMarquardt<T>::LevenbergMarquardt(const LevenbergMarquardt::size_type nv,
					      const LevenbergMarquardt::size_type np,
					      const LevenbergMarquardt::PtrFun f_)
      : p(np),
	lambda0(T(1.e-3)),
	factor(T(2)),
	eps1(1.e-10),
	eps2(1.e-10),
	iterMax(100),
	f(f_),
	n(nv),
	m(np)
    {} // end of LevenbergMarquardt::LevenbergMarquardt

    template<typename T>
    void
    LevenbergMarquardt<T>::setInitialDampingParameter(const T value)
    {
      this->lambda0 = value;
    } // end of LevenbergMarquardt<T>::setInitialDampingParameter

    template<typename T>
    void
    LevenbergMarquardt<T>::setFirstCriterium(const T value)
    {
      this->eps1 = value;
    } // end of LevenbergMarquardt<T>::setFirstCriterium

    template<typename T>
    void
    LevenbergMarquardt<T>::setSecondCriterium(const T value)
    {
      this->eps2 = value;
    } // end of LevenbergMarquardt<T>::setSecondCriterium

    template<typename T>
    void
    LevenbergMarquardt<T>::setMultiplicationFactor(const T value)
    {
      this->factor = value;
    } // end of LevenbergMarquardt<T>::setMultiplicationFactor

    template<typename T>
    void
    LevenbergMarquardt<T>::addData(const LevenbergMarquardt<T>::Variable& x,
				   const T y)
    {
      using namespace std;
      this->data.push_back(pair<Variable,T>(x,y));
    } // end of LevenbergMarquardt::addData

    template<typename T>
    void
    LevenbergMarquardt<T>::setInitialGuess(const LevenbergMarquardt<T>::Parameter& p_)
    {
      this->p = p_;
    } // end of LevenbergMarquardt::setInitialGuess

    template<typename T>
    void
    LevenbergMarquardt<T>::setMaximumIteration(const T nb)
    {
      this->iterMax = nb;
    } // end of LevenbergMarquardt::setInitialGuess

    template<typename T>
    const typename LevenbergMarquardt<T>::Parameter&
    LevenbergMarquardt<T>::execute(void)
    {
      using namespace std;
      using tfel::math::stdfunctions::power;
      typename vector<pair<Variable,T> >::const_iterator it;
      // grad is the opposite of the gradient
      matrix<T> J(this->m,this->m,T(0));
      matrix<T> Jn(this->m,this->m,T(0));
      Parameter g(this->m,T(0));
      Parameter gn(this->m,T(0));
      Parameter gradient(this->m,T(0));
      Parameter h(this->m);
      Parameter p_(this->m);
      T s(T(0));
      T v(0);
      T lambda = this->lambda0;
      unsigned short i;
      unsigned short iter;
      bool success;
      s = T(0);
      for(it=this->data.begin();it!=this->data.end();++it){
	p_ = this->p+h;
	(*f)(v,gradient,it->first,p_);
	g += (v-it->second)*gradient;
	J += gradient^gradient;
	s += power<2>(v-it->second);
      }
      lambda *= *(max_element(J.begin(),J.end()));
      for(i=0;i!=this->m;++i){
	J(i,i) += lambda;
      }
      this->factor=2;
      success = false;
      for(iter=0;(iter!=this->iterMax)&&(!success);++iter){
	Jn=J;
	fill(gn.begin(),gn.end(),T(0));
	h = -g;
	T sn(T(0));
	LUSolve::exe(Jn,h);
	fill(Jn.begin(),Jn.end(),T(0));
	for(it=this->data.begin();it!=this->data.end();++it){
	  p_ = this->p+h;
	  (*f)(v,gradient,it->first,p_);
	  gn += (v-it->second)*gradient;
	  Jn += gradient^gradient;
	  sn += power<2>(v-it->second);
	}
	T rho = (s-sn)/(0.5*(h|(lambda*h-g)));
	if(rho>0){
	  lambda  *= max(T(0.3333),T(1)-power<3>(2*rho-1));
	  this->factor = 2;
	} else {
	  lambda *= this->factor;
	  this->factor *= 2;
	}
	for(i=0;i!=this->m;++i){
	  Jn(i,i) += lambda;
	}
	this->p += h;
	T ng = norm(gn);
	T nh = norm(h);
	T np = norm(p);
	if(nh<this->eps2*(np+this->eps2)){
	  success = true;
	} else if (ng<this->eps1){
	  success = true;
	} else {
	  // preparing next iteration
	  J = Jn;
	  g = gn;
	  s = sn;
	}
      }
      if(!success){
	string msg("LevenbergMarquardt<T>::execute : ");
	msg += "maximum number of iterations reached";
	throw(MathDivergenceException(msg));
      }

      return this->p;
    } // end of execute

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_LEVENBERGMARQUARDT_IXX */

