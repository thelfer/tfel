/*! 
 * \file  LevenbergMarquardtFunctionWrapper.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 24 fév 2010
 */

#ifndef _LIB_LEVENBERGMARQUARDTFUNCTIONWRAPPER_H_
#define _LIB_LEVENBERGMARQUARDTFUNCTIONWRAPPER_H_ 

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Math/vector.hxx"

namespace tfel
{

  namespace math
  {

    template<typename T>
    struct LevenbergMarquardtFunctionWrapper
    {
      typedef void (* PtrFun)(T&,
			      tfel::math::vector<T>&,
			      const tfel::math::vector<T>&,
			      const tfel::math::vector<T>&);

      typedef typename tfel::math::vector<T>::size_type size_type;
      typedef T NumericType;

      TFEL_MATH_INLINE2
      LevenbergMarquardtFunctionWrapper(const size_type,
					const size_type,
					const PtrFun);

      TFEL_MATH_INLINE2
      LevenbergMarquardtFunctionWrapper(const LevenbergMarquardtFunctionWrapper&);

      TFEL_MATH_INLINE void
      operator()(T&,tfel::math::vector<T>&,
		 const tfel::math::vector<T>&,
		 const tfel::math::vector<T>&) const;
      
      TFEL_MATH_INLINE2 size_type
      getNumberOfVariables(void) const;

      TFEL_MATH_INLINE2 size_type
      getNumberOfParameters(void) const;

    private:
      
      LevenbergMarquardtFunctionWrapper();

      LevenbergMarquardtFunctionWrapper&
      operator=(const LevenbergMarquardtFunctionWrapper&);
      
      const size_type nv;
      const size_type np;
      const PtrFun f;

    }; // end of struct LevenbergMarquardtFunctionWrapper

  } // end of namespace math

} // end of namespace tfel

#include"TFEL/Math/LevenbergMarquardt/LevenbergMarquardtFunctionWrapper.ixx"

#endif /* _LIB_LEVENBERGMARQUARDTFUNCTIONWRAPPER_H */

