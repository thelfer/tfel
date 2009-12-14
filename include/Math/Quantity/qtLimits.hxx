/*!
 * \file   qtLimits.hxx
 * \brief  This file declares the numeric_limits classes for quantities.
 * \author Helfer Thomas
 * \date   26 Jul 2006
 */

#ifndef _LIB_TFEL_QTLIMITS_H_
#define _LIB_TFEL_QTLIMITS_H_ 

#include<limits>

#include"Math/qt.hxx"

namespace std{
    
  /*
   * Partial specialisation for quantities
   */
  template<typename unit,typename T>
  struct numeric_limits<tfel::math::qt<unit,T> >
  {

    static const bool is_specialized = numeric_limits<T>::is_specialized;
      
    static const int digits      = numeric_limits<T>::digits;
    static const int digits10    = numeric_limits<T>::digits10;
    static const bool is_signed  = numeric_limits<T>::is_signed;
    static const bool is_integer = numeric_limits<T>::is_integer;
    static const bool is_exact   = numeric_limits<T>::is_exact;
    static const int radix       = numeric_limits<T>::radix;

    static const int min_exponent   = numeric_limits<T>::min_exponent;
    static const int min_exponent10 = numeric_limits<T>::min_exponent10;
    static const int max_exponent   = numeric_limits<T>::max_exponent;
    static const int max_exponent10 = numeric_limits<T>::max_exponent10;

    static const bool has_infinity             = numeric_limits<T>::has_infinity;
    static const bool has_quiet_NaN            = numeric_limits<T>::has_quiet_NaN;
    static const bool has_signaling_NaN        = numeric_limits<T>::has_signaling_NaN;
    static const float_denorm_style has_denorm = numeric_limits<T>::has_denorm;
    static const bool has_denorm_loss          = numeric_limits<T>::has_denorm_loss;

    static const bool is_iec559  = numeric_limits<T>::is_iec559;
    static const bool is_bounded = numeric_limits<T>::is_bounded;
    static const bool is_modulo  = numeric_limits<T>::is_modulo;

    static const bool traps                    = numeric_limits<T>::traps;
    static const bool tinyness_before          = numeric_limits<T>::tinyness_before;
    static const float_round_style round_style = numeric_limits<T>::round_style;

    static tfel::math::qt<unit,T> min() throw() { 
      return tfel::math::qt<unit,T>(numeric_limits<T>::min()); 
    }
    static tfel::math::qt<unit,T> max() throw() { 
      return tfel::math::qt<unit,T>(numeric_limits<T>::max()); 
    }
    static tfel::math::qt<unit,T> epsilon() throw() {
      return tfel::math::qt<unit,T>(numeric_limits<T>::epsilon());
    }
    static tfel::math::qt<unit,T> round_error() throw() {
      return tfel::math::qt<unit,T>(numeric_limits<T>::round_error());
    }
    static tfel::math::qt<unit,T> infinity() throw()  {
      return tfel::math::qt<unit,T>(numeric_limits<T>::infinity());
    }
    static tfel::math::qt<unit,T> quiet_NaN() throw() {
      return tfel::math::qt<unit,T>(numeric_limits<T>::quiet_NaN());
    }
    static tfel::math::qt<unit,T> signaling_NaN() throw() {
      return tfel::math::qt<unit,T>(numeric_limits<T>::signaling_NaN());
    }
    static tfel::math::qt<unit,T> denorm_min() throw() {
      return tfel::math::qt<unit,T>(numeric_limits<T>::denorm_min());
    }
    
  };

} // end of namespace std

#endif /* _LIB_TFEL_QTLIMITS_H */

