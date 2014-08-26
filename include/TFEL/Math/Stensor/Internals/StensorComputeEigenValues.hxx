/*!
 * \file   StensorComputeEigenValues.hxx
 * \brief    
 * \author Helfer Thomas
 * \date   13 Jul 2006
 */

#ifndef _LIB_TFEL_STENSORCOMPUTEEIGENVALUES_H_
#define _LIB_TFEL_STENSORCOMPUTEEIGENVALUES_H_ 

#include <cmath>
#include <sstream>

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Math/MathException.hxx"
#include"TFEL/Math/General/CubicRoots.hxx"
#include"TFEL/Exception/TFELException.hxx"

namespace tfel{

  namespace math {

    namespace internals{

      struct TFEL_VISIBILITY_LOCAL StensorEigenValuesComputationFailureException
	: tfel::math::MathException
      {
	template<typename T>
	StensorEigenValuesComputationFailureException(const T* const v)
	  : tfel::math::MathException(StensorEigenValuesComputationFailureException::buildErrorMessage(v))
	{}
      private:
	template<typename T>
	static std::string
	buildErrorMessage(const T* const v)
	{
	  using namespace std;
	  ostringstream msg;
	  msg.precision(16);
	  msg << "StensorEigenValuesComputationFailureException::StensorEigenValuesComputationFailureException : "
	    "eigen value computation failed for symmetric tensor [" << v[0] << " " << v[1] << " " << v[2] << " "
	      << v[3] << " " << v[4] << " " << v[5] << "]";
	  return msg.str();
	}
      }; // end of struct StensorEigenValuesComputationFailureException
	
      // computeEigenValues
	template<unsigned short N>
      struct StensorComputeEigenValues_;
      
      template<>
      struct StensorComputeEigenValues_<1u>
      {
	template<typename T>
	static TFEL_MATH_INLINE2 void exe(const T* const v,T& vp1,T& vp2,T& vp3,
					  const bool)
	{
	  TFEL_STATIC_ASSERT(tfel::typetraits::IsFundamentalNumericType<T>::cond);
	  TFEL_STATIC_ASSERT(tfel::typetraits::IsReal<T>::cond);
	  vp1 = *v;
	  vp2 = *(v+1);
	  vp3 = *(v+2);
	}
      };
      
      template<>
      struct StensorComputeEigenValues_<2u>
      {
	template<typename T>
	static TFEL_MATH_INLINE2 void exe(const T* const v,
					  T& vp1,T& vp2,T& vp3,
					  const bool)
	{
	  TFEL_STATIC_ASSERT(tfel::typetraits::IsFundamentalNumericType<T>::cond);
	  TFEL_STATIC_ASSERT(tfel::typetraits::IsReal<T>::cond);
	  const T one_half = T(1)/T(2);
	  T tr    = v[0] + v[1];
	  T tmp   = v[0] - v[1];
	  T delta = tmp*tmp*0.25+(v[3]*v[3])*one_half;
	  vp1 = one_half*tr+std::sqrt(delta); 
	  vp2 = one_half*tr-std::sqrt(delta); 
	  vp3 = *(v+2);
	}
      };

      template<>
      struct StensorComputeEigenValues_<3u>
      {
	template<typename T>
	static TFEL_MATH_INLINE2 void exe(const T* const v,T& vp1,T& vp2,T& vp3,
					  const bool b)
	{
	  TFEL_STATIC_ASSERT(tfel::typetraits::IsFundamentalNumericType<T>::cond);
	  TFEL_STATIC_ASSERT(tfel::typetraits::IsReal<T>::cond);
	  static const T M_1_SQRT2 = 1/std::sqrt(static_cast<T>(2));
	  const T one_half = T(1)/T(2);
	  const T p3 = -1.;
	  const T p2 = v[0]+v[1]+v[2];
	  const T p1 =  one_half*(v[5]*v[5]+v[4]*v[4]+v[3]*v[3])-(v[0]*(v[2]+v[1])+v[1]*v[2]);
	  const T p0 = -one_half*(v[0]*v[5]*v[5]+v[1]*v[4]*v[4]+v[2]*v[3]*v[3])+M_1_SQRT2*(v[3]*v[4]*v[5])+v[0]*v[1]*v[2];
	  if(CubicRoots::exe(vp1,vp2,vp3,p3,p2,p1,p0,b)==0u){
	    throw(StensorEigenValuesComputationFailureException(v));
	  }
	}
      };
      
    } //end of namespace internals
    
  } //end of namespace math
  
} // end of namespace tfel


#endif /* _LIB_TFEL_STENSORCOMPUTEEIGENVALUES_H */

