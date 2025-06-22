/*!
 * \file
 * include/TFEL/Math/LevenbergMarquardt/LevenbergMarquardtFunctionWrapper.hxx
 * \brief
 * \author Thomas Helfer
 * \date 24/02/2010
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_LEVENBERGMARQUARDTFUNCTIONWRAPPER_HXX
#define LIB_LEVENBERGMARQUARDTFUNCTIONWRAPPER_HXX

#include "TFEL/Math/vector.hxx"

namespace tfel {

  namespace math {

    template <typename T>
    struct LevenbergMarquardtFunctionWrapper {
      typedef void (*PtrFun)(T&,
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
      LevenbergMarquardtFunctionWrapper(
          const LevenbergMarquardtFunctionWrapper&);

      TFEL_MATH_INLINE void operator()(T&,
                                       tfel::math::vector<T>&,
                                       const tfel::math::vector<T>&,
                                       const tfel::math::vector<T>&) const;

      TFEL_MATH_INLINE2 size_type getNumberOfVariables() const;

      TFEL_MATH_INLINE2 size_type getNumberOfParameters() const;

     private:
      LevenbergMarquardtFunctionWrapper();

      LevenbergMarquardtFunctionWrapper& operator=(
          const LevenbergMarquardtFunctionWrapper&);

      const size_type nv;
      const size_type np;
      const PtrFun f;

    };  // end of struct LevenbergMarquardtFunctionWrapper

  }  // end of namespace math

}  // end of namespace tfel

#include "TFEL/Math/LevenbergMarquardt/LevenbergMarquardtFunctionWrapper.ixx"

#endif /* LIB_LEVENBERGMARQUARDTFUNCTIONWRAPPER_HXX */
