/*!
 * \file   IEEE754.hxx
 * \brief
 * \author Thomas Helfer
 * \date   13 mars 2017
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_IEEE754_HXX
#define LIB_TFEL_MATH_IEEE754_HXX

#include <cmath>
#include "TFEL/Config/TFELConfig.hxx"

namespace tfel {

  namespace math {

    namespace ieee754 {

      /*!
       * \brief a portable implementation of the std::fpclassify
       * function based on the MUSL library (https://www.musl-libc.org).
       * \return the class of the floatting point value according to the
       * IEEE754 standard?
       * Possible return values are:
       * - FP_NORMAL: standard floatting point value
       * - FP_SUBNORMAL: subnormal floatting point value
       * - FP_NAN: NaN value
       * - FP_INFINITE: +Inf or -Inf value
       * \param[in] x: value to be tested
       */
      TFEL_VISIBILITY_LOCAL inline int fpclassify(const float);
      /*!
       * \brief a portable implementation of the std::fpclassify
       * function based on the MUSL library (https://www.musl-libc.org)
       * \return the class of the floatting point value according to the
       * IEEE754 standard.
       * Possible return values are:
       * - FP_NORMAL: standard floatting point value
       * - FP_SUBNORMAL: subnormal floatting point value
       * - FP_NAN: NaN value
       * - FP_INFINITE: +Inf or -Inf value
       * \param[in] x: value to be tested
       */
      TFEL_VISIBILITY_LOCAL inline int fpclassify(const double);
      /*!
       * \brief a portable implementation of the std::fpclassify
       * function based on the MUSL library (https://www.musl-libc.org)
       * \return the class of the floatting point value according to the
       * IEEE754 standard.
       * Possible return values are:
       * - FP_NORMAL: standard floatting point value
       * - FP_SUBNORMAL: subnormal floatting point value
       * - FP_NAN: NaN value
       * - FP_INFINITE: +Inf or -Inf value
       * \param[in] x: value to be tested
       */
      TFEL_VISIBILITY_LOCAL inline int fpclassify(const long double);
      /*!
       * \brief Determines if the given floating point number arg is a
       * not-a-number (NaN) value.
       * \param[in] x: value to be tested
       * \return true if the given floating point number arg is a
       * not-a-number (NaN) value.
       */
      TFEL_VISIBILITY_LOCAL inline bool isnan(const float);
      /*!
       * \brief Determines if the given floating point number arg is a
       * not-a-number (NaN) value.
       * \param[in] x: value to be tested
       * \return true if the given floating point number arg is a
       * not-a-number (NaN) value.
       */
      TFEL_VISIBILITY_LOCAL inline bool isnan(const double);
      /*!
       * \brief Determines if the given floating point number arg is a
       * not-a-number (NaN) value.
       * \param[in] x: value to be tested
       * \return true if the given floating point number arg is a
       * not-a-number (NaN) value.
       */
      TFEL_VISIBILITY_LOCAL inline bool isnan(const long double);
      /*!
       * \brief Determines if the given floating point number arg has
       * finite value i.e. it is normal, subnormal or zero, but not
       * infinite or NaN.
       * \param[in] x: value to be tested
       * \return true if if the given floating point number arg has
       * finite value i.e. it is normal, subnormal or zero, but not
       * infinite or NaN.
       */
      TFEL_VISIBILITY_LOCAL inline bool isfinite(const float);
      /*!
       * \brief Determines if the given floating point number arg has
       * finite value i.e. it is normal, subnormal or zero, but not
       * infinite or NaN.
       * \param[in] x: value to be tested
       * \return true if if the given floating point number arg has
       * finite value i.e. it is normal, subnormal or zero, but not
       * infinite or NaN.
       */
      TFEL_VISIBILITY_LOCAL inline bool isfinite(const double);
      /*!
       * \brief Determines if the given floating point number arg has
       * finite value i.e. it is normal, subnormal or zero, but not
       * infinite or NaN.
       * \param[in] x: value to be tested
       * \return true if if the given floating point number arg has
       * finite value i.e. it is normal, subnormal or zero, but not
       * infinite or NaN.
       */
      TFEL_VISIBILITY_LOCAL inline bool isfinite(const long double);

    }  // end of namespace ieee754

  }  // end of namespace math

}  // end of namespace tfel

#include "TFEL/Math/General/IEEE754.ixx"

#endif /* LIB_TFEL_MATH_IEEE754_HXX */
