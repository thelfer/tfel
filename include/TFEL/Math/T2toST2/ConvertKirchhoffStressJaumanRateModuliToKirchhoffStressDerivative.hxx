/*!
 * \file   ConvertKirchhoffStressJaumanRateModuliToKirchhoffStressDerivative.hxx
 * \brief
 * \author Thomas Helfer
 * \date   18 août 2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_CONVERTKIRCHHOFFSTRESSJAUMANRATEMODULITOKIRCHHOFFSTRESSDERIVATIVE_HXX
#define LIB_TFEL_MATH_CONVERTKIRCHHOFFSTRESSJAUMANRATEMODULITOKIRCHHOFFSTRESSDERIVATIVE_HXX

#include <type_traits>
#include "TFEL/TypeTraits/BaseType.hxx"
#include "TFEL/Math/General/ConstExprMathFunctions.hxx"
#include "TFEL/Math/t2tost2.hxx"
#include "TFEL/Math/Tensor/TensorConcept.hxx"
#include "TFEL/Math/Stensor/StensorConcept.hxx"
#include "TFEL/Math/ST2toST2/ST2toST2Concept.hxx"

namespace tfel {

  namespace math {

    template <unsigned short N, typename T>
    struct ConvertKirchhoffStressJaumanRateModuliToKirchhoffStressDerivative;
    //! partial specialisation in 1D
    template <typename T>
    struct ConvertKirchhoffStressJaumanRateModuliToKirchhoffStressDerivative<
        1u,
        T> {
      /*!
       * \param[in] CsJ: moduli associated to the Jauman rate of the Kirchhoff
       * stress \param[in] F: deformation gradient \param[in] t: Kirchhoff
       * stress
       */
      template <typename ST2toST2Type,
                typename TensorType,
                typename StensorType>
      static t2tost2<1u, T> exe(const ST2toST2Type& CsJ,
                                const TensorType& F,
                                const StensorType&) {
        return {CsJ(0, 0) / F(0), CsJ(0, 1) / F(1), CsJ(0, 2) / F(2),
                CsJ(1, 0) / F(0), CsJ(1, 1) / F(1), CsJ(1, 2) / F(2),
                CsJ(2, 0) / F(0), CsJ(2, 1) / F(1), CsJ(2, 2) / F(2)};
      }
    };
    //! partial specialisation in 2D
    template <typename T>
    struct ConvertKirchhoffStressJaumanRateModuliToKirchhoffStressDerivative<
        2u,
        T> {
      /*!
       * \param[in] CsJ: moduli associated to the Jauman rate of the Kirchhoff
       * stress \param[in] F: deformation gradient \param[in] t: Kirchhoff
       * stress
       */
      template <typename ST2toST2Type,
                typename TensorType,
                typename StensorType>
      static t2tost2<2u, T> exe(const ST2toST2Type& CsJ,
                                const TensorType& F,
                                const StensorType& t) {
        constexpr const auto icste = Cste<T>::isqrt2;
        const auto iF = invert(F);
        return {(CsJ(0, 3) * iF[3]) * icste + (t[3] * iF[3]) * icste +
                    CsJ(0, 0) * iF[0],
                (CsJ(0, 3) * iF[4]) * icste - (t[3] * iF[4]) * icste +
                    CsJ(0, 1) * iF[1],
                CsJ(0, 2) * iF[2],
                CsJ(0, 0) * iF[4] + (CsJ(0, 3) * iF[1]) * icste +
                    (t[3] * iF[1]) * icste,
                CsJ(0, 1) * iF[3] + (CsJ(0, 3) * iF[0]) * icste -
                    (t[3] * iF[0]) * icste,
                (CsJ(1, 3) * iF[3]) * icste - (t[3] * iF[3]) * icste +
                    CsJ(1, 0) * iF[0],
                (CsJ(1, 3) * iF[4]) * icste + (t[3] * iF[4]) * icste +
                    CsJ(1, 1) * iF[1],
                CsJ(1, 2) * iF[2],
                CsJ(1, 0) * iF[4] + (CsJ(1, 3) * iF[1]) * icste -
                    (t[3] * iF[1]) * icste,
                CsJ(1, 1) * iF[3] + (CsJ(1, 3) * iF[0]) * icste +
                    (t[3] * iF[0]) * icste,
                (CsJ(2, 3) * iF[3]) * icste + CsJ(2, 0) * iF[0],
                (CsJ(2, 3) * iF[4]) * icste + CsJ(2, 1) * iF[1],
                CsJ(2, 2) * iF[2],
                CsJ(2, 0) * iF[4] + (CsJ(2, 3) * iF[1]) * icste,
                CsJ(2, 1) * iF[3] + (CsJ(2, 3) * iF[0]) * icste,
                (CsJ(3, 3) * iF[3]) * icste + ((t[1] - t[0]) * iF[3]) * icste +
                    CsJ(3, 0) * iF[0],
                (CsJ(3, 3) * iF[4]) * icste - ((t[1] - t[0]) * iF[4]) * icste +
                    CsJ(3, 1) * iF[1],
                CsJ(3, 2) * iF[2],
                CsJ(3, 0) * iF[4] + (CsJ(3, 3) * iF[1]) * icste +
                    ((t[1] - t[0]) * iF[1]) * icste,
                CsJ(3, 1) * iF[3] + (CsJ(3, 3) * iF[0]) * icste -
                    ((t[1] - t[0]) * iF[0]) * icste};
      }  // end of exe
    };
    //! partial specialisation in 3D
    template <typename T>
    struct ConvertKirchhoffStressJaumanRateModuliToKirchhoffStressDerivative<
        3u,
        T> {
      /*!
       * \param[in] CsJ: moduli associated to the Jauman rate of the Kirchhoff
       * stress \param[in] F: deformation gradient \param[in] t: Kirchhoff
       * stress
       */
      template <typename ST2toST2Type,
                typename TensorType,
                typename StensorType>
      static t2tost2<3u, T> exe(const ST2toST2Type& CsJ,
                                const TensorType& F,
                                const StensorType& t) {
        constexpr const auto cste = Cste<T>::sqrt2;
        constexpr const auto icste = Cste<T>::isqrt2;
        TFEL_CONSTEXPR const auto icste2 = icste / 2;
        const auto iF = invert(F);
        return {
            (cste * t[4] * iF[5] + cste * t[3] * iF[3]) / 2 +
                (CsJ(0, 4) * iF[5]) * icste + (CsJ(0, 3) * iF[3]) * icste +
                CsJ(0, 0) * iF[0],
            (CsJ(0, 5) * iF[7]) * icste + (CsJ(0, 3) * iF[4]) * icste -
                (t[3] * iF[4]) * icste + CsJ(0, 1) * iF[1],
            (CsJ(0, 5) * iF[8]) * icste + (CsJ(0, 4) * iF[6]) * icste -
                (t[4] * iF[6]) * icste + CsJ(0, 2) * iF[2],
            (cste * t[4] * iF[7] + cste * t[3] * iF[1]) / 2 +
                (CsJ(0, 4) * iF[7]) * icste + CsJ(0, 0) * iF[4] +
                (CsJ(0, 3) * iF[1]) * icste,
            (CsJ(0, 5) * iF[5]) * icste + CsJ(0, 1) * iF[3] +
                (CsJ(0, 3) * iF[0]) * icste - (t[3] * iF[0]) * icste,
            (cste * t[3] * iF[8] + cste * t[4] * iF[2]) / 2 +
                (CsJ(0, 3) * iF[8]) * icste + CsJ(0, 0) * iF[6] +
                (CsJ(0, 4) * iF[2]) * icste,
            CsJ(0, 2) * iF[5] + (CsJ(0, 5) * iF[3]) * icste +
                (CsJ(0, 4) * iF[0]) * icste - (t[4] * iF[0]) * icste,
            CsJ(0, 1) * iF[8] + (CsJ(0, 3) * iF[6]) * icste -
                (t[3] * iF[6]) * icste + (CsJ(0, 5) * iF[2]) * icste,
            CsJ(0, 2) * iF[7] + (CsJ(0, 4) * iF[4]) * icste -
                (t[4] * iF[4]) * icste + (CsJ(0, 5) * iF[1]) * icste,
            (CsJ(1, 4) * iF[5]) * icste + (CsJ(1, 3) * iF[3]) * icste -
                (t[3] * iF[3]) * icste + CsJ(1, 0) * iF[0],
            (cste * t[5] * iF[7] + cste * t[3] * iF[4]) / 2 +
                (CsJ(1, 5) * iF[7]) * icste + (CsJ(1, 3) * iF[4]) * icste +
                CsJ(1, 1) * iF[1],
            (CsJ(1, 5) * iF[8]) * icste - (t[5] * iF[8]) * icste +
                (CsJ(1, 4) * iF[6]) * icste + CsJ(1, 2) * iF[2],
            (CsJ(1, 4) * iF[7]) * icste + CsJ(1, 0) * iF[4] +
                (CsJ(1, 3) * iF[1]) * icste - (t[3] * iF[1]) * icste,
            (cste * t[5] * iF[5] + cste * t[3] * iF[0]) / 2 +
                (CsJ(1, 5) * iF[5]) * icste + CsJ(1, 1) * iF[3] +
                (CsJ(1, 3) * iF[0]) * icste,
            (CsJ(1, 3) * iF[8]) * icste - (t[3] * iF[8]) * icste +
                CsJ(1, 0) * iF[6] + (CsJ(1, 4) * iF[2]) * icste,
            CsJ(1, 2) * iF[5] + (CsJ(1, 5) * iF[3]) * icste -
                (t[5] * iF[3]) * icste + (CsJ(1, 4) * iF[0]) * icste,
            CsJ(1, 1) * iF[8] +
                (cste * t[3] * iF[6] + cste * t[5] * iF[2]) / 2 +
                (CsJ(1, 3) * iF[6]) * icste + (CsJ(1, 5) * iF[2]) * icste,
            CsJ(1, 2) * iF[7] + (CsJ(1, 4) * iF[4]) * icste +
                (CsJ(1, 5) * iF[1]) * icste - (t[5] * iF[1]) * icste,
            (CsJ(2, 4) * iF[5]) * icste - (t[4] * iF[5]) * icste +
                (CsJ(2, 3) * iF[3]) * icste + CsJ(2, 0) * iF[0],
            (CsJ(2, 5) * iF[7]) * icste - (t[5] * iF[7]) * icste +
                (CsJ(2, 3) * iF[4]) * icste + CsJ(2, 1) * iF[1],
            (cste * t[5] * iF[8] + cste * t[4] * iF[6]) / 2 +
                (CsJ(2, 5) * iF[8]) * icste + (CsJ(2, 4) * iF[6]) * icste +
                CsJ(2, 2) * iF[2],
            (CsJ(2, 4) * iF[7]) * icste - (t[4] * iF[7]) * icste +
                CsJ(2, 0) * iF[4] + (CsJ(2, 3) * iF[1]) * icste,
            (CsJ(2, 5) * iF[5]) * icste - (t[5] * iF[5]) * icste +
                CsJ(2, 1) * iF[3] + (CsJ(2, 3) * iF[0]) * icste,
            (CsJ(2, 3) * iF[8]) * icste + CsJ(2, 0) * iF[6] +
                (CsJ(2, 4) * iF[2]) * icste - (t[4] * iF[2]) * icste,
            CsJ(2, 2) * iF[5] +
                (cste * t[5] * iF[3] + cste * t[4] * iF[0]) / 2 +
                (CsJ(2, 5) * iF[3]) * icste + (CsJ(2, 4) * iF[0]) * icste,
            CsJ(2, 1) * iF[8] + (CsJ(2, 3) * iF[6]) * icste +
                (CsJ(2, 5) * iF[2]) * icste - (t[5] * iF[2]) * icste,
            CsJ(2, 2) * iF[7] +
                (cste * t[4] * iF[4] + cste * t[5] * iF[1]) / 2 +
                (CsJ(2, 4) * iF[4]) * icste + (CsJ(2, 5) * iF[1]) * icste,
            (cste * t[5] * iF[5] + (2 * t[1] - 2 * t[0]) * iF[3]) * icste2 +
                (CsJ(3, 4) * iF[5]) * icste + (CsJ(3, 3) * iF[3]) * icste +
                CsJ(3, 0) * iF[0],
            (cste * t[4] * iF[7] + (2 * t[0] - 2 * t[1]) * iF[4]) * icste2 +
                (CsJ(3, 5) * iF[7]) * icste + (CsJ(3, 3) * iF[4]) * icste +
                CsJ(3, 1) * iF[1],
            -(t[4] * iF[8] + t[5] * iF[6]) / 2 + (CsJ(3, 5) * iF[8]) * icste +
                (CsJ(3, 4) * iF[6]) * icste + CsJ(3, 2) * iF[2],
            (cste * t[5] * iF[7] + (2 * t[1] - 2 * t[0]) * iF[1]) * icste2 +
                (CsJ(3, 4) * iF[7]) * icste + CsJ(3, 0) * iF[4] +
                (CsJ(3, 3) * iF[1]) * icste,
            (cste * t[4] * iF[5] + (2 * t[0] - 2 * t[1]) * iF[0]) * icste2 +
                (CsJ(3, 5) * iF[5]) * icste + CsJ(3, 1) * iF[3] +
                (CsJ(3, 3) * iF[0]) * icste,
            ((2 * t[1] - 2 * t[0]) * iF[8] + cste * t[5] * iF[2]) * icste2 +
                (CsJ(3, 3) * iF[8]) * icste + CsJ(3, 0) * iF[6] +
                (CsJ(3, 4) * iF[2]) * icste,
            CsJ(3, 2) * iF[5] - (t[4] * iF[3] + t[5] * iF[0]) / 2 +
                (CsJ(3, 5) * iF[3]) * icste + (CsJ(3, 4) * iF[0]) * icste,
            CsJ(3, 1) * iF[8] -
                ((2 * t[1] - 2 * t[0]) * iF[6] - cste * t[4] * iF[2]) * icste2 +
                (CsJ(3, 3) * iF[6]) * icste + (CsJ(3, 5) * iF[2]) * icste,
            CsJ(3, 2) * iF[7] - (t[5] * iF[4] + t[4] * iF[1]) / 2 +
                (CsJ(3, 4) * iF[4]) * icste + (CsJ(3, 5) * iF[1]) * icste,
            ((2 * t[2] - 2 * t[0]) * iF[5] + cste * t[5] * iF[3]) * icste2 +
                (CsJ(4, 4) * iF[5]) * icste + (CsJ(4, 3) * iF[3]) * icste +
                CsJ(4, 0) * iF[0],
            -(t[3] * iF[7] + t[5] * iF[4]) / 2 + (CsJ(4, 5) * iF[7]) * icste +
                (CsJ(4, 3) * iF[4]) * icste + CsJ(4, 1) * iF[1],
            (cste * t[3] * iF[8] + (2 * t[0] - 2 * t[2]) * iF[6]) * icste2 +
                (CsJ(4, 5) * iF[8]) * icste + (CsJ(4, 4) * iF[6]) * icste +
                CsJ(4, 2) * iF[2],
            ((2 * t[2] - 2 * t[0]) * iF[7] + cste * t[5] * iF[1]) * icste2 +
                (CsJ(4, 4) * iF[7]) * icste + CsJ(4, 0) * iF[4] +
                (CsJ(4, 3) * iF[1]) * icste,
            -(t[3] * iF[5] + t[5] * iF[0]) / 2 + (CsJ(4, 5) * iF[5]) * icste +
                CsJ(4, 1) * iF[3] + (CsJ(4, 3) * iF[0]) * icste,
            (cste * t[5] * iF[8] + (2 * t[2] - 2 * t[0]) * iF[2]) * icste2 +
                (CsJ(4, 3) * iF[8]) * icste + CsJ(4, 0) * iF[6] +
                (CsJ(4, 4) * iF[2]) * icste,
            CsJ(4, 2) * iF[5] +
                (cste * t[3] * iF[3] + (2 * t[0] - 2 * t[2]) * iF[0]) * icste2 +
                (CsJ(4, 5) * iF[3]) * icste + (CsJ(4, 4) * iF[0]) * icste,
            CsJ(4, 1) * iF[8] - (t[5] * iF[6] + t[3] * iF[2]) / 2 +
                (CsJ(4, 3) * iF[6]) * icste + (CsJ(4, 5) * iF[2]) * icste,
            CsJ(4, 2) * iF[7] -
                ((2 * t[2] - 2 * t[0]) * iF[4] - cste * t[3] * iF[1]) * icste2 +
                (CsJ(4, 4) * iF[4]) * icste + (CsJ(4, 5) * iF[1]) * icste,
            -(t[3] * iF[5] + t[4] * iF[3]) / 2 + (CsJ(5, 4) * iF[5]) * icste +
                (CsJ(5, 3) * iF[3]) * icste + CsJ(5, 0) * iF[0],
            ((2 * t[2] - 2 * t[1]) * iF[7] + cste * t[4] * iF[4]) * icste2 +
                (CsJ(5, 5) * iF[7]) * icste + (CsJ(5, 3) * iF[4]) * icste +
                CsJ(5, 1) * iF[1],
            -((2 * t[2] - 2 * t[1]) * iF[8] - cste * t[3] * iF[6]) * icste2 +
                (CsJ(5, 5) * iF[8]) * icste + (CsJ(5, 4) * iF[6]) * icste +
                CsJ(5, 2) * iF[2],
            -(t[3] * iF[7] + t[4] * iF[1]) / 2 + (CsJ(5, 4) * iF[7]) * icste +
                CsJ(5, 0) * iF[4] + (CsJ(5, 3) * iF[1]) * icste,
            ((2 * t[2] - 2 * t[1]) * iF[5] + cste * t[4] * iF[0]) * icste2 +
                (CsJ(5, 5) * iF[5]) * icste + CsJ(5, 1) * iF[3] +
                (CsJ(5, 3) * iF[0]) * icste,
            -(t[4] * iF[8] + t[3] * iF[2]) / 2 + (CsJ(5, 3) * iF[8]) * icste +
                CsJ(5, 0) * iF[6] + (CsJ(5, 4) * iF[2]) * icste,
            CsJ(5, 2) * iF[5] -
                ((2 * t[2] - 2 * t[1]) * iF[3] - cste * t[3] * iF[0]) * icste2 +
                (CsJ(5, 5) * iF[3]) * icste + (CsJ(5, 4) * iF[0]) * icste,
            CsJ(5, 1) * iF[8] +
                (cste * t[4] * iF[6] + (2 * t[2] - 2 * t[1]) * iF[2]) * icste2 +
                (CsJ(5, 3) * iF[6]) * icste + (CsJ(5, 5) * iF[2]) * icste,
            CsJ(5, 2) * iF[7] +
                (cste * t[3] * iF[4] + (2 * t[1] - 2 * t[2]) * iF[1]) * icste2 +
                (CsJ(5, 4) * iF[4]) * icste + (CsJ(5, 5) * iF[1]) * icste};
      }  // end of exe
    };
  }  // namespace math
}  // namespace tfel

#endif /* LIB_TFEL_MATH_CONVERTKIRCHHOFFSTRESSJAUMANRATEMODULITOKIRCHHOFFSTRESSDERIVATIVE_HXX \
        */
