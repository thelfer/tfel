/*! 
 * \file  qtComplexOperations.hxx
 * \brief
 * \author Helfer Thomas
 * \date   10 févr. 2015
 */

#ifndef LIB_TFEL_MATH_QTCOMPLEXOPERATIONS_H_
#define LIB_TFEL_MATH_QTCOMPLEXOPERATIONS_H_ 

#include"TFEL/Math/qt.hxx"
#include"TFEL/Math/Genral/Complex.hxx"

namespace tfel{

  namespace math{
    
    TFEL_MATH_QT_RESULT_TYPE_IMPL(tfel::math::Complex<unsigned short>)
    TFEL_MATH_QT_RESULT_TYPE_IMPL(tfel::math::Complex<unsigned int>)
    TFEL_MATH_QT_RESULT_TYPE_IMPL(tfel::math::Complex<long unsigned int>)
    TFEL_MATH_QT_RESULT_TYPE_IMPL(tfel::math::Complex<short>)
    TFEL_MATH_QT_RESULT_TYPE_IMPL(tfel::math::Complex<int>)
    TFEL_MATH_QT_RESULT_TYPE_IMPL(tfel::math::Complex<long int>)
    TFEL_MATH_QT_RESULT_TYPE_IMPL(tfel::math::Complex<float>)
    TFEL_MATH_QT_RESULT_TYPE_IMPL(tfel::math::Complex<double>)
    TFEL_MATH_QT_RESULT_TYPE_IMPL(tfel::math::Complex<long double>)

  }

}

#endif /* LIB_TFEL_MATH_QTCOMPLEXOPERATIONS_H_ */

