/*!
 * \file   ConvertLogarithmicStrainTangentOperator.hxx
 * \brief
 * \author Thomas Helfer
 * \date   29 août 2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_CONVERTLOGARITHMICSTRAINTANGENTOPERATOR_HXX
#define LIB_TFEL_MATH_CONVERTLOGARITHMICSTRAINTANGENTOPERATOR_HXX

#include <type_traits>
#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/tmatrix.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/st2tost2.hxx"

namespace tfel::math {

  /*!
   * \brief A class used to convert the tangent operator in the
   * logarithmic strain framework into the CSE tangent moduli
   *
   * \tparam N:      space dimension
   * \tparam stress: stress type
   */
  template <unsigned short N, typename stress>
  struct ConvertLogarithmicStrainTangentOperator;

  /*!
   * \brief Partial specialisation of the
   * ConvertLogarithmicStrainTangentOperator in 1D.
   * \tparam stress: stress type
   */
  template <typename stress>
  struct ConvertLogarithmicStrainTangentOperator<1u, stress> {
    //! a simple alias
    using real = base_type<stress>;
    /*!
     *  \param[out] Cse: tangent moduli relating the rate of the
     *                   second Kirchhoff stress to the rate of the
     *                   Green-Lagrange strain
     *  \param[in] C:    tangent operator in the logarithmic strain
     *                   framework
     *  \param[in] P:    two times the derivative of the logarithmic
     *                   strain with respect to the right Cauchy
     *                   Green tensor
     *  \param[in] T:    dual stress of the logarithmic strain
     *  \param[in] m:    eigen vectors of the right Cauchy Green tensor
     *  \param[in] vp:   eigen values  of the right Cauchy Green tensor
     */
    template <typename ST2toST2Type,
              typename ST2toST2Type2,
              typename ST2toST2Type3,
              typename StressStensorType>
    static typename std::enable_if<
        ((implementsST2toST2Concept<ST2toST2Type>()) &&
             (tfel::math::getSpaceDimension<ST2toST2Type>() == 1u) &&
             (std::is_same<numeric_type<ST2toST2Type>, stress>::value) &&
             (implementsST2toST2Concept<ST2toST2Type2>()) &&
             (tfel::math::getSpaceDimension<ST2toST2Type2>() == 1u) &&
             (isAssignableTo<numeric_type<ST2toST2Type2>, stress>()),
         (implementsST2toST2Concept<ST2toST2Type3>()) &&
             (tfel::math::getSpaceDimension<ST2toST2Type3>() == 1u) &&
             (isAssignableTo<numeric_type<ST2toST2Type3>, real>()),
         (implementsStensorConcept<StressStensorType>()) &&
             (tfel::math::getSpaceDimension<StressStensorType>() == 1u) &&
             (isAssignableTo<numeric_type<StressStensorType>, stress>())),
        void>::type
    exe(ST2toST2Type& Cse,
        const ST2toST2Type2& C,
        const ST2toST2Type2& P,
        const StressStensorType& T,
        const tmatrix<3u, 3u, real>&,
        const tvector<3u, real>&) {
      const auto iC0 = P(0, 0);
      const auto iC1 = P(1, 1);
      const auto iC2 = P(2, 2);
      Cse(0, 0) = (C(0, 0) - T[0]) * iC0 * iC0;
      Cse(0, 1) = C(0, 1) * iC1 * iC0;
      Cse(0, 2) = C(0, 2) * iC2 * iC0;
      Cse(1, 0) = (C(1, 0) - T[1]) * iC0 * iC1;
      Cse(1, 1) = C(1, 1) * iC1 * iC1;
      Cse(1, 2) = C(1, 2) * iC2 * iC1;
      Cse(2, 0) = (C(2, 0) - T[2]) * iC0 * iC2;
      Cse(2, 1) = C(2, 1) * iC1 * iC2;
      Cse(2, 2) = C(2, 2) * iC2 * iC2;
    }
  };  // end of struct ConvertLogarithmicStrainTangentOperator<1u,stress>

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_CONVERTLOGARITHMICSTRAINTANGENTOPERATOR_HXX */
