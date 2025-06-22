/*!
 * \file   ConvertSpatialModuliToKirchhoffJaumanRateModuli.hxx
 * \brief
 * \author Thomas Helfer
 * \date   12 août 2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_CONVERTSPATIALMODULITOKIRCHOFFJAUMANRATEMODULI_HXX
#define LIB_TFEL_MATH_CONVERTSPATIALMODULITOKIRCHOFFJAUMANRATEMODULI_HXX

#include <type_traits>

#include "TFEL/Metaprogramming/Implements.hxx"
#include "TFEL/TypeTraits/BaseType.hxx"
#include "TFEL/Math/General/BasicOperations.hxx"
#include "TFEL/Math/General/ResultType.hxx"
#include "TFEL/Math/General/ConstExprMathFunctions.hxx"
#include "TFEL/Math/Tensor/TensorConcept.hxx"
#include "TFEL/Math/T2toST2/T2toST2Concept.hxx"
#include "TFEL/Math/t2tost2.hxx"

namespace tfel {

  namespace math {

    /*!
     * \brief convert the spatial moduli in the moduli associated with
     * Jauman rate of the Kirchhoff stress
     * \param[in] C_s: spatial moduli
     * \param[in] tau: Kirchhoff stress
     * \return the moduli associated with Jauman rate of the Kirchhoff
     * stress
     */
    template <typename ST2toST2Type, typename StensorType>
    typename std::enable_if<
        ((ST2toST2Traits<ST2toST2Type>::dime == 1u) &&
         (StensorTraits<StensorType>::dime == 1u) &&
         tfel::meta::Implements<ST2toST2Type,
                                tfel::math::ST2toST2Concept>::cond &&
         tfel::meta::Implements<StensorType,
                                tfel::math::StensorConcept>::cond &&
         std::is_same<ST2toST2NumType<ST2toST2Type>,
                      StensorNumType<StensorType>>::value),
        st2tost2<1u, ST2toST2NumType<ST2toST2Type>>>::type
    convertSpatialModuliToKirchhoffJaumanRateModuli(const ST2toST2Type& C_s,
                                                    const StensorType& tau) {
      using NumType = ST2toST2NumType<ST2toST2Type>;
      st2tost2<1u, NumType> C_tJ;
      C_tJ(0, 0) = C_s(0, 0) + 2 * tau[0];
      C_tJ(0, 1) = C_s(0, 1);
      C_tJ(0, 2) = C_s(0, 2);
      C_tJ(1, 0) = C_s(1, 0);
      C_tJ(1, 1) = C_s(1, 1) + 2 * tau[1];
      C_tJ(1, 2) = C_s(1, 2);
      C_tJ(2, 0) = C_s(2, 0);
      C_tJ(2, 1) = C_s(2, 1);
      C_tJ(2, 2) = C_s(2, 2) + 2 * tau[2];
      return C_tJ;
    }

    /*!
     * \brief convert the spatial moduli in the moduli associated with
     * Jauman rate of the Kirchhoff stress
     * \param[in] C_s: spatial moduli
     * \param[in] tau: Kirchhoff stress
     * \return the moduli associated with Jauman rate of the Kirchhoff
     * stress
     */
    template <typename ST2toST2Type, typename StensorType>
    typename std::enable_if<
        ((ST2toST2Traits<ST2toST2Type>::dime == 2u) &&
         (StensorTraits<StensorType>::dime == 2u) &&
         tfel::meta::Implements<ST2toST2Type,
                                tfel::math::ST2toST2Concept>::cond &&
         tfel::meta::Implements<StensorType,
                                tfel::math::StensorConcept>::cond &&
         std::is_same<ST2toST2NumType<ST2toST2Type>,
                      StensorNumType<StensorType>>::value),
        st2tost2<2u, ST2toST2NumType<ST2toST2Type>>>::type
    convertSpatialModuliToKirchhoffJaumanRateModuli(const ST2toST2Type& C_s,
                                                    const StensorType& tau) {
      using NumType = ST2toST2NumType<ST2toST2Type>;
      st2tost2<2u, NumType> C_tJ;
      C_tJ(0, 0) = C_s(0, 0) + 2 * tau[0];
      C_tJ(1, 1) = C_s(1, 1) + 2 * tau[1];
      C_tJ(2, 2) = C_s(2, 2) + 2 * tau[2];
      C_tJ(3, 0) = C_s(3, 0) + tau[3];
      C_tJ(3, 1) = C_s(3, 1) + tau[3];
      C_tJ(0, 3) = C_s(0, 3) + tau[3];
      C_tJ(1, 3) = C_s(1, 3) + tau[3];
      C_tJ(3, 3) = C_s(3, 3) + tau[1] + tau[0];
      C_tJ(0, 1) = C_s(0, 1);
      C_tJ(0, 2) = C_s(0, 2);
      C_tJ(3, 2) = C_s(3, 2);
      C_tJ(1, 0) = C_s(1, 0);
      C_tJ(1, 2) = C_s(1, 2);
      C_tJ(2, 0) = C_s(2, 0);
      C_tJ(2, 1) = C_s(2, 1);
      C_tJ(2, 3) = C_s(2, 3);
      return C_tJ;
    }

    /*!
     * \brief convert the spatial moduli in the moduli associated with
     * Jauman rate of the Kirchhoff stress
     * \param[in] C_s: spatial moduli
     * \param[in] tau: Kirchhoff stress
     * \return the moduli associated with Jauman rate of the Kirchhoff
     * stress
     */
    template <typename ST2toST2Type, typename StensorType>
    typename std::enable_if<
        ((ST2toST2Traits<ST2toST2Type>::dime == 3u) &&
         (StensorTraits<StensorType>::dime == 3u) &&
         tfel::meta::Implements<ST2toST2Type,
                                tfel::math::ST2toST2Concept>::cond &&
         tfel::meta::Implements<StensorType,
                                tfel::math::StensorConcept>::cond &&
         std::is_same<ST2toST2NumType<ST2toST2Type>,
                      StensorNumType<StensorType>>::value),
        st2tost2<3u, ST2toST2NumType<ST2toST2Type>>>::type
    convertSpatialModuliToKirchhoffJaumanRateModuli(const ST2toST2Type& C_s,
                                                    const StensorType& tau) {
      using NumType = ST2toST2NumType<ST2toST2Type>;
      constexpr const auto icste = Cste<NumType>::isqrt2;
      st2tost2<3u, NumType> C_tJ;
      C_tJ(0, 0) = C_s(0, 0) + 2 * tau[0];
      C_tJ(1, 1) = C_s(1, 1) + 2 * tau[1];
      C_tJ(2, 2) = C_s(2, 2) + 2 * tau[2];
      C_tJ(0, 3) = C_s(0, 3) + tau[3];
      C_tJ(0, 4) = C_s(0, 4) + tau[4];
      C_tJ(1, 3) = C_s(1, 3) + tau[3];
      C_tJ(1, 5) = C_s(1, 5) + tau[5];
      C_tJ(2, 4) = C_s(2, 4) + tau[4];
      C_tJ(2, 5) = C_s(2, 5) + tau[5];
      C_tJ(3, 3) = C_s(3, 3) + tau[1] + tau[0];
      C_tJ(3, 0) = C_s(3, 0) + tau[3];
      C_tJ(3, 1) = C_s(3, 1) + tau[3];
      C_tJ(3, 4) = C_s(3, 4) + tau[5] * icste;
      C_tJ(3, 5) = C_s(3, 5) + tau[4] * icste;
      C_tJ(4, 0) = C_s(4, 0) + tau[4];
      C_tJ(4, 2) = C_s(4, 2) + tau[4];
      C_tJ(4, 4) = C_s(4, 4) + tau[2] + tau[0];
      C_tJ(4, 3) = C_s(4, 3) + tau[5] * icste;
      C_tJ(4, 5) = C_s(4, 5) + tau[3] * icste;
      C_tJ(5, 1) = C_s(5, 1) + tau[5];
      C_tJ(5, 2) = C_s(5, 2) + tau[5];
      C_tJ(5, 3) = C_s(5, 3) + tau[4] * icste;
      C_tJ(5, 4) = C_s(5, 4) + tau[3] * icste;
      C_tJ(5, 5) = C_s(5, 5) + tau[2] + tau[1];
      C_tJ(0, 1) = C_s(0, 1);
      C_tJ(0, 2) = C_s(0, 2);
      C_tJ(5, 0) = C_s(5, 0);
      C_tJ(0, 5) = C_s(0, 5);
      C_tJ(1, 0) = C_s(1, 0);
      C_tJ(1, 2) = C_s(1, 2);
      C_tJ(1, 4) = C_s(1, 4);
      C_tJ(2, 0) = C_s(2, 0);
      C_tJ(2, 1) = C_s(2, 1);
      C_tJ(2, 3) = C_s(2, 3);
      C_tJ(3, 2) = C_s(3, 2);
      C_tJ(4, 1) = C_s(4, 1);
      return C_tJ;
    }

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_CONVERTSPATIALMODULITOKIRCHOFFJAUMANRATEMODULI_HXX */
