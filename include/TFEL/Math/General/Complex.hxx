/*!
 * \file   include/TFEL/Math/General/Complex.hxx
 * \brief
 * \author Thomas Helfer
 * \date   10 déc 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_COMPLEX_HXX
#define LIB_TFEL_MATH_COMPLEX_HXX

#include <complex>

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Math/Forward/Complex.hxx"
#include "TFEL/Math/General/UnaryResultType.hxx"

namespace tfel {

  namespace math {

    template <int N, unsigned int D>
    struct Power;

    template <int N, unsigned int D>
    class TFEL_VISIBILITY_LOCAL UnaryResultType<Complex<float>, Power<N, D>> {
     public:
      typedef Complex<float> type;
    };

    template <int N, unsigned int D>
    class TFEL_VISIBILITY_LOCAL UnaryResultType<Complex<double>, Power<N, D>> {
     public:
      typedef Complex<double> type;
    };

    template <int N, unsigned int D>
    class TFEL_VISIBILITY_LOCAL
        UnaryResultType<Complex<long double>, Power<N, D>> {
     public:
      typedef Complex<long double> type;
    };

    // /*
    //  * \brief return the conjugated Complex.
    //  * \param ValueType, base type of the Complex.
    //  * \param const Complex<ValueType>&, object to be conjugated.
    //  * \return const Complex<ValueType>.
    //  */
    // template<typename ValueType>
    // TFEL_MATH_INLINE constexpr
    // Complex<ValueType>
    // conj(const Complex<ValueType>&);

    // /*
    //  * \brief return the real part of a Complex.
    //  * \param ValueType, base type of the Complex.
    //  * \param const Complex<ValueType>&, source object.
    //  * \return const ValueType.
    //  */
    // template<typename ValueType>
    // TFEL_MATH_INLINE constexpr
    // ValueType
    // real(const Complex<ValueType>&);

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_COMPLEX_HXX */
