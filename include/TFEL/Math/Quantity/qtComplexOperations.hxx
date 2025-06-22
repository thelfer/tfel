/*!
 * \file  qtComplexOperations.hxx
 * \brief
 * \author Thomas Helfer
 * \date   10 févr. 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_QTCOMPLEXOPERATIONS_HXX
#define LIB_TFEL_MATH_QTCOMPLEXOPERATIONS_HXX

#include "TFEL/Math/qt.hxx"
#include "TFEL/Math/Genral/Complex.hxx"

namespace tfel {

  namespace math {

    TFEL_MATH_QT_RESULT_TYPE_IMPL(tfel::math::Complex<unsigned short>)
    TFEL_MATH_QT_RESULT_TYPE_IMPL(tfel::math::Complex<unsigned int>)
    TFEL_MATH_QT_RESULT_TYPE_IMPL(tfel::math::Complex<long unsigned int>)
    TFEL_MATH_QT_RESULT_TYPE_IMPL(tfel::math::Complex<short>)
    TFEL_MATH_QT_RESULT_TYPE_IMPL(tfel::math::Complex<int>)
    TFEL_MATH_QT_RESULT_TYPE_IMPL(tfel::math::Complex<long int>)
    TFEL_MATH_QT_RESULT_TYPE_IMPL(tfel::math::Complex<float>)
    TFEL_MATH_QT_RESULT_TYPE_IMPL(tfel::math::Complex<double>)
    TFEL_MATH_QT_RESULT_TYPE_IMPL(tfel::math::Complex<long double>)

  }  // namespace math

}  // namespace tfel

#endif /* LIB_TFEL_MATH_QTCOMPLEXOPERATIONS_HXX */
