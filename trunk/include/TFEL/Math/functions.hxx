/*!
 * \file   functions.hxx
 * \brief  This file is an header all functions' related stuff.
 * \author Helfer Thomas
 * \date   04 Jan 2007
 */

#ifndef _LIB_TFEL_FUNCTIONS_H_
#define _LIB_TFEL_FUNCTIONS_H_ 

/*
 * \def USING_TFEL_FUNCTIONS
 * \brief an helper class to use TFEL's functions in place of standard ones.
 * This macro is made necessary du to declartion of ::sin and others C 
 * functions in the global namespace.
 */
#define USING_TFEL_FUNCTIONS               \
  using tfel::math::stdfunctions::id;      \
  using tfel::math::stdfunctions::sin;     \
  using tfel::math::stdfunctions::cos;     \
  using tfel::math::stdfunctions::tan;     \
  using tfel::math::stdfunctions::sqrt;    \
  using tfel::math::stdfunctions::exp;     \
  using tfel::math::stdfunctions::log;     \
  using tfel::math::stdfunctions::log10;   \
  using tfel::math::stdfunctions::asin;    \
  using tfel::math::stdfunctions::acos;    \
  using tfel::math::stdfunctions::atan;    \
  using tfel::math::stdfunctions::sinh;    \
  using tfel::math::stdfunctions::cosh;    \
  using tfel::math::stdfunctions::tanh;    \
  using tfel::math::stdfunctions::power;   \
  using tfel::math::stdfunctions::pow

#include"TFEL/Math/Function/StandardFunctions.hxx"
#include"TFEL/Math/Function/Power.hxx"
#include"TFEL/Math/Function/Cst.hxx"

#endif /* _LIB_TFEL_FUNCTIONS_H */
