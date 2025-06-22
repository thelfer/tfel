/*!
 * \file   StensorDeterminant.hxx
 * \brief
 * \author Thomas Helfer
 * \date   04 janv. 2017
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_INTERNALS_STENSORDETERMINANT_HXX
#define LIB_TFEL_MATH_INTERNALS_STENSORDETERMINANT_HXX

namespace tfel {

  namespace math {

    namespace internals {

      template <unsigned short N>
      struct StensorDeterminant;

      template <>
      struct StensorDeterminant<1u> {
        template <typename T>
        static TFEL_MATH_INLINE typename ComputeUnaryResult<T, Power<3>>::Result
        exe(const tfel::math::stensor<1u, T>& s) {
          return s(0) * s(1) * s(2);
        }
      };  // end of struct StensorDeterminant

      template <>
      struct StensorDeterminant<2u> {
        template <typename T>
        static TFEL_MATH_INLINE typename ComputeUnaryResult<T, Power<3>>::Result
        exe(const tfel::math::stensor<2u, T>& s) {
          return s(2) * (s(0) * s(1) - s(3) * s(3) / 2);
        }
      };  // end of struct StensorDeterminant

      template <>
      struct StensorDeterminant<3u> {
        template <typename T>
        static TFEL_MATH_INLINE typename ComputeUnaryResult<T, Power<3>>::Result
        exe(const tfel::math::stensor<3u, T>& s) {
          constexpr const auto cste = Cste<T>::sqrt2;
          return (2 * s(0) * s(1) * s(2) + cste * s(3) * s(4) * s(5) -
                  s(2) * s(3) * s(3) - s(1) * s(4) * s(4) -
                  s(0) * s(5) * s(5)) /
                 2;
        }
      };  // end of struct StensorDeterminant

    }  // end of namespace internals

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_INTERNALS_STENSORDETERMINANT_HXX */
