/*! 
 * \file  include/TFEL/Math/Kriging/FactorizedKriging.ixx
 * \brief
 * \author Helfer Thomas
 * \brief 12 avr 2009
 */

#ifndef _LIB_TFEL_MATH_FACTORIZEDKRIGING_IXX_
#define _LIB_TFEL_MATH_FACTORIZEDKRIGING_IXX_ 

#include<algorithm>

#include"TFEL/Math/matrix.hxx"
#ifdef HAVE_ATLAS
#include"TFEL/Math/Bindings/atlas.hxx"
#else
#include"TFEL/Math/LUSolve.hxx"
#endif
#include"TFEL/Math/Kriging/KrigingErrors.hxx"

namespace tfel
{

  namespace math
  {

    template<unsigned short N,
	     unsigned short M,
	     typename T,
	     typename Model1,
	     typename Model2>
    FactorizedKriging<N,M,T,Model1,Model2>::FactorizedKriging()
    {}

    template<unsigned short N,
	     unsigned short M,
	     typename T,
	     typename Model1,
	     typename Model2>
    FactorizedKriging<N,M,T,Model1,Model2>::FactorizedKriging(const Model1& m1_,
							      const Model2& m2_)
      : m1(m1_),m2(m2_)
    {}

    template<unsigned short N,
	     unsigned short M,
	     typename T,
	     typename Model1,
	     typename Model2>
    T
    FactorizedKriging<N,M,T,Model1,Model2>::operator()(const typename KrigingVariable<N,T>::type& xv1,
						       const typename KrigingVariable<M,T>::type& xv2) const
    {
      using namespace tfel::math;
      using namespace tfel::math::internals;
      typedef typename vector<T>::difference_type diff;
      typename vector<T>::size_type i;
      typename vector<T>::const_iterator p = a.begin()+static_cast<diff>(this->x1.size());
      T r(0);
      for(i=0;i!=x1.size();++i){
	r+=a[i]*(m1.covariance(xv1-this->x1[i])*m2.covariance(xv2-this->x2[i]));
      }
      ApplySpecificationDrifts<0,Model1::nb,N,T,Model1>::apply(r,p,xv1);
      ApplySpecificationDrifts<0,Model2::nb,M,T,Model2>::apply(r,p,xv2);
      return r;
    } // end of FactorizedKriging<N,M,T,Model>::operator()

    template<unsigned short N,
	     unsigned short M,
	     typename T,
	     typename Model1,
	     typename Model2>
    void
    FactorizedKriging<N,M,T,Model1,Model2>::addValue(const typename KrigingVariable<N,T>::type& xv1,
						     const typename KrigingVariable<M,T>::type& xv2,
						     const T& fv)
    {
      using namespace tfel::math;
      this->x1.push_back(xv1);
      this->x2.push_back(xv2);
      this->f.push_back(fv);
    }

    template<unsigned short N,
	     unsigned short M,
	     typename T,
	     typename Model1,
	     typename Model2>
    void
    FactorizedKriging<N,M,T,Model1,Model2>::buildInterpolation(void)
    {
      using namespace std;
      using namespace tfel::math;
      using namespace tfel::math::internals;
#ifdef HAVE_ATLAS
      using namespace tfel::math::atlas;
#endif
      using tfel::math::vector;
      typename vector<T>::size_type i;
      typename vector<T>::size_type j;
      if((x1.size()!=f.size())||
	 (x2.size()!=f.size())){
	throw(KrigingErrorInvalidLength());
      }
      if((x1.empty())||(x2.empty())){
	throw(KrigingErrorNoDataSpecified());
      }
      if(x1.size()<=Model1::nb){
	throw(KrigingErrorInsufficientData());
      }
      if(x2.size()<=Model2::nb){
	throw(KrigingErrorInsufficientData());
      }
      matrix<T> m(x1.size()+Model1::nb+Model2::nb,
		  x1.size()+Model1::nb+Model2::nb,T(0));    // temporary matrix
      this->a.resize(x1.size()+Model1::nb+Model2::nb,T(0)); // unknowns
      copy(this->f.begin(),this->f.end(),this->a.begin());  // copy values
      // filling the matrix
      for(i=0;i!=x1.size();++i){
	for(j=0;j!=i;++j){
	  m(i,j) = m(j,i) = m1.covariance(this->x1[i]-this->x1[j])*m2.covariance(this->x2[i]-this->x2[j]);
	}
	m(i,i) = T(0);
      }
      ApplySpecificationDrifts<0,Model1::nb,N,T,Model1>::apply(m,this->x1.size(),this->x1);
      ApplySpecificationDrifts<0,Model2::nb,M,T,Model2>::apply(m,this->x1.size()+Model1::nb,this->x2);
      // computing unknown values
#ifdef HAVE_ATLAS
      gesv(m,this->a);
#else
      LUSolve::exe(m,this->a);
#endif
    }

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_FACTORIZEDKRIGING_IXX */

