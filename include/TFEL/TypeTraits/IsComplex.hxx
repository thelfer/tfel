/*!
 * \file   include/TFEL/TypeTraits/IsComplex.hxx
 * \brief  This file declares the IsComplex traits class and
 * specialises it for standard numeric types.
 * \author Helfer Thomas
 * \date   04 Jul 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_ISCOMPLEX_HXX_
#define LIB_TFEL_ISCOMPLEX_HXX_

#include "TFEL/Math/Forward/Complex.hxx"

/*!
 * \def    TFEL_META_IS_COMPLEX
 * \brief An helper macro to specialise IsComplex for each standard
 * numeric types.
 * \author Helfer Thomas
 * \date   28 jun 2006
 */
#define TFEL_META_IS_COMPLEX(X)          \
  /*!                                    \
   * \brief Partial specialisation for X \
   * \see   IsComplex                    \
   */                                    \
  template <>                            \
  struct IsComplex<X> {                  \
    /*!                                  \
     *  Result                           \
     */                                  \
    static constexpr bool cond = true;   \
  };                                     \
  /*!                                    \
   * \brief Partial specialisation for X \
   * \see   IsComplex                    \
   */                                    \
  template <>                            \
  struct IsComplex<const X> {            \
    /*!                                  \
     *  Result                           \
     */                                  \
    static constexpr bool cond = true;   \
  }

namespace tfel {

  namespace typetraits {

    /*!
     * \brief Traits class which says it a given type stands for a
     * complex numerical type.
     *
     * \param  T, type to be tested.
     * \return bool cond, true if T stands for a complex numerical
     * type, false otherwise.
     *
     * \see IsScalar, IsReal
     *
     * \author Helfer Thomas
     * \date   04 Jul 2006
     */
    template <typename T>
    struct IsComplex {
      /*!
       *  Result
       */
      static constexpr bool cond = false;
    };

    TFEL_META_IS_COMPLEX(tfel::math::Complex<unsigned short>);
    TFEL_META_IS_COMPLEX(tfel::math::Complex<unsigned int>);
    TFEL_META_IS_COMPLEX(tfel::math::Complex<unsigned long int>);
    TFEL_META_IS_COMPLEX(tfel::math::Complex<short>);
    TFEL_META_IS_COMPLEX(tfel::math::Complex<int>);
    TFEL_META_IS_COMPLEX(tfel::math::Complex<long int>);
    TFEL_META_IS_COMPLEX(tfel::math::Complex<float>);
    TFEL_META_IS_COMPLEX(tfel::math::Complex<double>);
    TFEL_META_IS_COMPLEX(tfel::math::Complex<long double>);

  }  // end of namespace typetraits

}  // end of namespace tfel

#endif /* LIB_TFEL_ISCOMPLEX_HXX_ */
