/*!
 * \file  include/TFEL/Math/Vector/VectorVectorDotProduct.hxx
 * \brief
 * \author Thomas Helfer
 * \date   04/02/2010
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_VECTORVECTORDOTPRODUCT_HXX
#define LIB_TFEL_MATH_VECTORVECTORDOTPRODUCT_HXX

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Math/General/RunTimeCheck.hxx"

namespace tfel::math {

  struct VectorVectorDotProduct {
    template <typename Result, typename T1, typename T2>
    static Result exe(const T1& v1, const T2& v2) {
#ifndef TFEL_NO_RUNTIME_CHECK_BOUNDS
//	typedef typename T1::RunTimeProperties RunTimeProperties;
//	RunTimeCheck<RunTimeProperties>::exe(v1.getRunTimeProperties(),
//					     v2.getRunTimeProperties());
#endif
      using size_type = typename T1::size_type;
      auto r = Result{};
      for (size_type i = 0; i != v1.size(); ++i) {
        r += v1(i) * v2(i);
      }
      return r;
    }  // end of VectorVectorDotProduct::exe

  };  // end of struct VectorVectorDotProduct

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_VECTORVECTORDOTPRODUCT_H */
