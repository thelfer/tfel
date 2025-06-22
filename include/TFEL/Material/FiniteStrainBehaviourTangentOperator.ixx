/*!
 * \file   FiniteStrainBehaviourTangentOperator.ixx
 * \brief
 * \author THOMAS HELFER
 * \date   19 août 2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATOR_IXX_
#define LIB_TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATOR_IXX_

#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/tensor.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/t2tost2.hxx"
#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Math/General/MathConstants.hxx"
#include "TFEL/Math/ST2toST2/ConvertSpatialModuliToKirchhoffJaumanRateModuli.hxx"

/*!
 * \brief a convenient macro for shorted declaration of partial
 * specialisation of the FiniteStrainBehaviourTangentOperatorConverter
 * structure
 */
#define TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(OP1, OP2) \
  template <>                                                                 \
  struct FiniteStrainBehaviourTangentOperatorConverter<                       \
      FiniteStrainBehaviourTangentOperatorBase::OP1,                          \
      FiniteStrainBehaviourTangentOperatorBase::OP2>                          \
      : public FiniteStrainBehaviourTangentOperatorConverterBase<             \
            FiniteStrainBehaviourTangentOperatorBase::OP1,                    \
            FiniteStrainBehaviourTangentOperatorBase::OP2>

namespace tfel {

  namespace material {

    /*!
     * An helper class used to define simple alias
     */
    template <
        FiniteStrainBehaviourTangentOperatorBase::Flag TangenOperatorType1,
        FiniteStrainBehaviourTangentOperatorBase::Flag TangenOperatorType2>
    struct FiniteStrainBehaviourTangentOperatorConverterBase {
      //! a simple alias
      template <typename T>
      using base_type = tfel::typetraits::base_type<T>;
      //! a simple alias
      template <unsigned short N, typename stress>
      using Result = tangent_operator<TangenOperatorType1, N, stress>;
      //! a simple alias
      template <unsigned short N, typename stress>
      using Source = tangent_operator<TangenOperatorType2, N, stress>;
      //! a simple alias
      template <unsigned short N, typename stress>
      using DeformationGradientTensor =
          tfel::math::tensor<N, base_type<stress>>;
      //! a simple alias
      template <unsigned short N, typename stress>
      using StressStensor = tfel::math::stensor<N, stress>;
      //! a simple alias
      using size_type = unsigned short;

     protected:
      /*!
       * \return the vector index associated with a the matrix indexes
       * of a symmetric tensor
       * \param[in] i: row index
       * \param[in] j: column index
       */
      TFEL_MATERIAL_INLINE static size_type index(const size_type i,
                                                  const size_type j) {
        // i,j are valid for the space dimension considered
        if (i == j) {
          return i;
        } else if ((i == 0) && (j == 1)) {
          return 3;
        } else if ((i == 1) && (j == 0)) {
          return 3;
        } else if ((i == 0) && (j == 2)) {
          return 4;
        } else if ((i == 2) && (j == 0)) {
          return 4;
        } else if ((i == 1) && (j == 2)) {
          return 5;
        }
        return 5;
      }
      /*!
       * \brief: generate a deformation gradient pertubation dFkl such
       * that the associated deformation rate is:
       * D_rkl = ((ek^el)+(el^ek))/2
       * with D = (L+tL)/2 and L = dF.F^{-1}
       * \param[in] F: deformation gradient
       * \param[in] idx: index associated to (k,l) in the stensor vector
       * convention
       */
      template <unsigned short N, typename real>
      static TFEL_MATERIAL_INLINE tfel::math::tensor<N, real>
      getDeformationGradient(const tfel::math::tensor<N, real>& F,
                             const size_type idx) {
        const auto c = [&idx]() -> std::pair<size_type, size_type> {
          if ((idx == 0) || (idx == 1) || (idx == 2)) {
            return {idx, idx};
          }
          return {2 * idx - 3, 2 * idx - 2};
        }();
        // i,j are valid for the space dimension considered
        tfel::math::tensor<N, real> dF;
        tfel::math::tensor<N, real> ekel(real(0));
        const auto v =
            ((idx > 2) ? tfel::math::Cste<real>::sqrt2 : real(1)) / 2;
        ekel(c.first) += v;
        ekel(c.second) += v;
        return ekel * F;
      }
    };  // end of struct FiniteStrainBehaviourTangentOperatorConverterBase
    /*!
     * \brief partial specialisation of
     * FiniteStrainBehaviourTangentOperatorConverter structure
     */
    TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(DS_DC, DS_DEGL){
        /*!
         * \param[out] Kr: the result of the convertion
         * \param[in]  Ks: the initial stiffness tensor
         * \param[in]  F0:  the deformation gradient
         * \param[in]  F1:  the deformation gradient
         * \param[in]  s:  the Cauchy stress tensor
         */
        template <unsigned short N, typename stress>
        static TFEL_MATERIAL_INLINE void exe(
            Result<N, stress> & Kr,
            const Source<N, stress>& Ks,
            const DeformationGradientTensor<N, stress>&,
            const DeformationGradientTensor<N, stress>&,
            const StressStensor<N, stress>&){Kr = Ks / 2;
  }  // namespace material
};   // namespace tfel
/*!
 * \brief partial specialisation of
 * FiniteStrainBehaviourTangentOperatorConverter structure
 */
TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(DS_DEGL, DS_DC){
    /*!
     * \param[out] Kr: the result of the convertion
     * \param[in]  Ks: the initial stiffness tensor
     * \param[in]  F0:  the deformation gradient
     * \param[in]  F1:  the deformation gradient
     * \param[in]  s:  the Cauchy stress tensor
     */
    template <unsigned short N, typename stress>
    static TFEL_MATERIAL_INLINE void exe(
        Result<N, stress> & Kr,
        const Source<N, stress>& Ks,
        const DeformationGradientTensor<N, stress>&,
        const DeformationGradientTensor<N, stress>&,
        const StressStensor<N, stress>&){Kr = 2 * Ks;
}  // end of exe
}
;  // end of struct FiniteStrainBehaviourTangentOperatorConverter
/*!
 * \brief partial specialisation of
 * FiniteStrainBehaviourTangentOperatorConverter structure
 */
TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(SPATIAL_MODULI,
                                                            DS_DEGL){
    /*!
     * \param[out] Kr: the result of the convertion
     * \param[in]  Ks: the initial stiffness tensor
     * \param[in]  F0:  the deformation gradient
     * \param[in]  F1:  the deformation gradient
     * \param[in]  s:  the Cauchy stress tensor
     */
    template <unsigned short N, typename stress>
    static TFEL_MATERIAL_INLINE void exe(
        Result<N, stress> & Kr,
        const Source<N, stress>& Ks,
        const DeformationGradientTensor<N, stress>&,
        const DeformationGradientTensor<N, stress>& F1,
        const StressStensor<N, stress>&){Kr = tfel::math::push_forward(Ks, F1);
}  // end of exe
}
;  // end of struct FiniteStrainBehaviourTangentOperatorConverter
/*!
 * \brief partial specialisation of
 * FiniteStrainBehaviourTangentOperatorConverter structure
 */
TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(DS_DEGL,
                                                            SPATIAL_MODULI){
    /*!
     * \param[out] Kr: the result of the convertion
     * \param[in]  Ks: the initial stiffness tensor
     * \param[in]  F0:  the deformation gradient
     * \param[in]  F1:  the deformation gradient
     * \param[in]  s:  the Cauchy stress tensor
     */
    template <unsigned short N, typename stress>
    static TFEL_MATERIAL_INLINE void exe(
        Result<N, stress> & Kr,
        const Source<N, stress>& Ks,
        const DeformationGradientTensor<N, stress>&,
        const DeformationGradientTensor<N, stress>& F1,
        const StressStensor<N, stress>&){Kr = tfel::math::pull_back(Ks, F1);
}  // end of exe
}
;  // end of struct FiniteStrainBehaviourTangentOperatorConverter
/*!
 * \brief partial specialisation of
 * FiniteStrainBehaviourTangentOperatorConverter structure
 */
TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(DS_DF, DS_DC){
    /*!
     * \param[out] Kr: the result of the convertion
     * \param[in]  Ks: the initial stiffness tensor
     * \param[in]  F0:  the deformation gradient
     * \param[in]  F1:  the deformation gradient
     * \param[in]  s:  the Cauchy stress tensor
     */
    template <unsigned short N, typename stress>
    static TFEL_MATERIAL_INLINE void exe(
        Result<N, stress> & Kr,
        const Source<N, stress>& Ks,
        const DeformationGradientTensor<N, stress>&,
        const DeformationGradientTensor<N, stress>& F1,
        const StressStensor<N, stress>&){
        Kr = Ks * tfel::math::t2tost2<N, base_type<stress>>::dCdF(F1);
}  // end of exe
}
;  // end of struct FiniteStrainBehaviourTangentOperatorConverter
/*!
 * \brief partial specialisation of
 * FiniteStrainBehaviourTangentOperatorConverter structure
 */
TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(DS_DF, DS_DEGL){
    /*!
     * \param[out] Kr: the result of the convertion
     * \param[in]  Ks: the initial stiffness tensor
     * \param[in]  F0:  the deformation gradient
     * \param[in]  F1:  the deformation gradient
     * \param[in]  s:  the Cauchy stress tensor
     */
    template <unsigned short N, typename stress>
    static TFEL_MATERIAL_INLINE void exe(
        Result<N, stress> & Kr,
        const Source<N, stress>& Ks,
        const DeformationGradientTensor<N, stress>&,
        const DeformationGradientTensor<N, stress>& F1,
        const StressStensor<N, stress>&){
        Kr = 2 * Ks * tfel::math::t2tost2<N, base_type<stress>>::dCdF(F1);
}  // end of exe
}
;  // end of struct FiniteStrainBehaviourTangentOperatorConverter
/*!
 * \brief partial specialisation of
 * FiniteStrainBehaviourTangentOperatorConverter structure
 */
TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(ABAQUS,
                                                            SPATIAL_MODULI){
    /*!
     * \param[out] Kr: the result of the convertion
     * \param[in]  Ks: the initial stiffness tensor
     * \param[in]  F0:  the deformation gradient
     * \param[in]  F1:  the deformation gradient
     * \param[in]  s:  the Cauchy stress tensor
     */
    template <unsigned short N, typename stress>
    static TFEL_MATERIAL_INLINE void exe(
        Result<N, stress> & Kr,
        const Source<N, stress>& Ks,
        const DeformationGradientTensor<N, stress>&,
        const DeformationGradientTensor<N, stress>& F1,
        const StressStensor<N, stress>& s){const auto J = tfel::math::det(F1);
Kr = tfel::math::convertSpatialModuliToKirchhoffJaumanRateModuli(Ks, s* J) / J;
}  // end of exe
}
;  // end of struct FiniteStrainBehaviourTangentOperatorConverter
/*!
 * \brief partial specialisation of
 * FiniteStrainBehaviourTangentOperatorConverter structure
 */
TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(ABAQUS, DS_DEGL){
    /*!
     * \param[out] Kr: the result of the convertion
     * \param[in]  Ks: the initial stiffness tensor
     * \param[in]  F0:  the deformation gradient
     * \param[in]  F1:  the deformation gradient
     * \param[in]  s:  the Cauchy stress tensor
     */
    template <unsigned short N, typename stress>
    static TFEL_MATERIAL_INLINE void exe(
        Result<N, stress> & Kr,
        const Source<N, stress>& Ks,
        const DeformationGradientTensor<N, stress>& F0,
        const DeformationGradientTensor<N, stress>& F1,
        const StressStensor<N, stress>& s){
        using TangentOperator = FiniteStrainBehaviourTangentOperatorBase;
const auto J = tfel::math::det(F1);
const auto C =
    convert<TangentOperator::SPATIAL_MODULI, TangentOperator::DS_DEGL>(
        Ks, F0, F1, s);
Kr = tfel::math::convertSpatialModuliToKirchhoffJaumanRateModuli(C, s* J) / J;
}  // end of exe
}
;  // end of struct FiniteStrainBehaviourTangentOperatorConverter
/*!
 * \brief partial specialisation of
 * FiniteStrainBehaviourTangentOperatorConverter structure
 */
TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(DSIG_DF,
                                                            C_TRUESDELL){
    /*!
     * \param[out] Kr: the result of the convertion
     * \param[in]  Ks: the initial stiffness tensor
     * \param[in]  F0:  the deformation gradient
     * \param[in]  F1:  the deformation gradient
     * \param[in]  s:  the Cauchy stress tensor
     */
    template <unsigned short N, typename stress>
    static TFEL_MATERIAL_INLINE void exe(
        Result<N, stress> & Kr,
        const Source<N, stress>& Ks,
        const DeformationGradientTensor<N, stress>& F0,
        const DeformationGradientTensor<N, stress>& F1,
        const StressStensor<N, stress>& s){
        using TangentOperator = FiniteStrainBehaviourTangentOperatorBase;
const auto Cs =
    convert<TangentOperator::SPATIAL_MODULI, TangentOperator::C_TRUESDELL>(
        Ks, F0, F1, s);
const auto Dt =
    convert<TangentOperator::DTAU_DF, TangentOperator::SPATIAL_MODULI>(
        Cs, F0, F1, s);
Kr = convert<TangentOperator::DSIG_DF, TangentOperator::DTAU_DF>(Dt, F0, F1, s);
}  // end of exe
}
;  // end of struct FiniteStrainBehaviourTangentOperatorConverter
/*!
 * \brief partial specialisation of
 * FiniteStrainBehaviourTangentOperatorConverter structure
 */
TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(SPATIAL_MODULI,
                                                            ABAQUS){
    /*!
     * \param[out] Kr: the result of the convertion
     * \param[in]  Ks: the initial stiffness tensor
     * \param[in]  F0:  the deformation gradient
     * \param[in]  F1:  the deformation gradient
     * \param[in]  s:  the Cauchy stress tensor
     */
    template <unsigned short N, typename stress>
    static TFEL_MATERIAL_INLINE void exe(
        Result<N, stress> & Kr,
        const Source<N, stress>& Ks,
        const DeformationGradientTensor<N, stress>&,
        const DeformationGradientTensor<N, stress>& F1,
        const StressStensor<N, stress>& s){const auto J = tfel::math::det(F1);
// a little trick: the invert of the transformation from the
// spatial moduli to the KirchhoffJaumanRateModuli is obtained
// by changing the sign of the Kirchoff stress
Kr = tfel::math::convertSpatialModuliToKirchhoffJaumanRateModuli(Ks * J, -s* J);
}  // end of exe
}
;  // end of struct FiniteStrainBehaviourTangentOperatorConverter
/*!
 * \brief partial specialisation of
 * FiniteStrainBehaviourTangentOperatorConverter structure
 */
TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(C_TRUESDELL,
                                                            SPATIAL_MODULI){
    /*!
     * \param[out] Kr: the result of the convertion
     * \param[in]  Ks: the initial stiffness tensor
     * \param[in]  F0:  the deformation gradient
     * \param[in]  F1:  the deformation gradient
     * \param[in]  s:  the Cauchy stress tensor
     */
    template <unsigned short N, typename stress>
    static TFEL_MATERIAL_INLINE void exe(
        Result<N, stress> & Kr,
        const Source<N, stress>& Ks,
        const DeformationGradientTensor<N, stress>&,
        const DeformationGradientTensor<N, stress>& F1,
        const StressStensor<N, stress>&){Kr = Ks / (tfel::math::det(F1));
}  // end of exe
}
;  // end of struct FiniteStrainBehaviourTangentOperatorConverter
/*!
 * \brief partial specialisation of
 * FiniteStrainBehaviourTangentOperatorConverter structure
 */
TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(C_TRUESDELL,
                                                            DS_DEGL){
    /*!
     * \param[out] Kr: the result of the convertion
     * \param[in]  Ks: the initial stiffness tensor
     * \param[in]  F0:  the deformation gradient
     * \param[in]  F1:  the deformation gradient
     * \param[in]  s:  the Cauchy stress tensor
     */
    template <unsigned short N, typename stress>
    static TFEL_MATERIAL_INLINE void exe(
        Result<N, stress> & Kr,
        const Source<N, stress>& Ks,
        const DeformationGradientTensor<N, stress>&,
        const DeformationGradientTensor<N, stress>& F1,
        const StressStensor<N, stress>&){Kr = tfel::math::push_forward(Ks, F1) /
                                              (tfel::math::det(F1));
}  // end of exe
}
;  // end of struct FiniteStrainBehaviourTangentOperatorConverter
/*!
 * \brief partial specialisation of
 * FiniteStrainBehaviourTangentOperatorConverter structure
 */
TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(SPATIAL_MODULI,
                                                            C_TRUESDELL){
    /*!
     * \param[out] Kr: the result of the convertion
     * \param[in]  Ks: the initial stiffness tensor
     * \param[in]  F0:  the deformation gradient
     * \param[in]  F1:  the deformation gradient
     * \param[in]  s:  the Cauchy stress tensor
     */
    template <unsigned short N, typename stress>
    static TFEL_MATERIAL_INLINE void exe(
        Result<N, stress> & Kr,
        const Source<N, stress>& Ks,
        const DeformationGradientTensor<N, stress>&,
        const DeformationGradientTensor<N, stress>& F1,
        const StressStensor<N, stress>&){Kr = Ks * (tfel::math::det(F1));
}  // end of exe
}
;  // end of struct FiniteStrainBehaviourTangentOperatorConverter
/*!
 * \brief partial specialisation of
 * FiniteStrainBehaviourTangentOperatorConverter structure
 */
TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(DSIG_DDF, DSIG_DF){
    /*!
     * \param[out] Kr: the result of the convertion
     * \param[in]  Ks: the initial stiffness tensor
     * \param[in]  F0:  the deformation gradient
     * \param[in]  F1:  the deformation gradient
     * \param[in]  s:  the Cauchy stress tensor
     */
    template <unsigned short N, typename stress>
    static TFEL_MATERIAL_INLINE void exe(
        Result<N, stress> & Kr,
        const Source<N, stress>& Ks,
        const DeformationGradientTensor<N, stress>& F0,
        const DeformationGradientTensor<N, stress>&,
        const StressStensor<N, stress>&){
        Kr = Ks * tfel::math::t2tot2<N, base_type<stress>>::tpld(F0);
}  // end of exe
}
;  // end of struct FiniteStrainBehaviourTangentOperatorConverter
/*!
 * \brief partial specialisation of
 * FiniteStrainBehaviourTangentOperatorConverter structure
 */
TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(DSIG_DF, DSIG_DDF){
    /*!
     * \param[out] Kr: the result of the convertion
     * \param[in]  Ks: the initial stiffness tensor
     * \param[in]  F0:  the deformation gradient
     * \param[in]  F1:  the deformation gradient
     * \param[in]  s:  the Cauchy stress tensor
     */
    template <unsigned short N, typename stress>
    static TFEL_MATERIAL_INLINE void exe(
        Result<N, stress> & Kr,
        const Source<N, stress>& Ks,
        const DeformationGradientTensor<N, stress>& F0,
        const DeformationGradientTensor<N, stress>&,
        const StressStensor<N, stress>&){
        const auto iF0 = tfel::math::invert(F0);
Kr = Ks * tfel::math::t2tot2<N, base_type<stress>>::tpld(iF0);
}  // end of exe
}
;  // end of struct FiniteStrainBehaviourTangentOperatorConverter
/*!
 * \brief partial specialisation of
 * FiniteStrainBehaviourTangentOperatorConverter structure
 */
TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(DTAU_DDF, DTAU_DF){
    /*!
     * \param[out] Kr: the result of the convertion
     * \param[in]  Ks: the initial stiffness tensor
     * \param[in]  F0:  the deformation gradient
     * \param[in]  F1:  the deformation gradient
     * \param[in]  s:  the Cauchy stress tensor
     */
    template <unsigned short N, typename stress>
    static TFEL_MATERIAL_INLINE void exe(
        Result<N, stress> & Kr,
        const Source<N, stress>& Ks,
        const DeformationGradientTensor<N, stress>& F0,
        const DeformationGradientTensor<N, stress>&,
        const StressStensor<N, stress>&){
        Kr = Ks * tfel::math::t2tot2<N, base_type<stress>>::tpld(F0);
}  // end of exe
}
;  // end of struct FiniteStrainBehaviourTangentOperatorConverter
/*!
 * \brief partial specialisation of
 * FiniteStrainBehaviourTangentOperatorConverter structure
 */
TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(DTAU_DF, DTAU_DDF){
    /*!
     * \param[out] Kr: the result of the convertion
     * \param[in]  Ks: the initial stiffness tensor
     * \param[in]  F0:  the deformation gradient
     * \param[in]  F1:  the deformation gradient
     * \param[in]  s:  the Cauchy stress tensor
     */
    template <unsigned short N, typename stress>
    static TFEL_MATERIAL_INLINE void exe(
        Result<N, stress> & Kr,
        const Source<N, stress>& Ks,
        const DeformationGradientTensor<N, stress>& F0,
        const DeformationGradientTensor<N, stress>&,
        const StressStensor<N, stress>&){
        const auto iF0 = tfel::math::invert(F0);
Kr = Ks * tfel::math::t2tot2<N, base_type<stress>>::tpld(iF0);
}  // end of exe
}
;  // end of struct FiniteStrainBehaviourTangentOperatorConverter
/*!
 * \brief partial specialisation of
 * FiniteStrainBehaviourTangentOperatorConverter structure
 */
TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(DSIG_DF, DTAU_DF){
    /*!
     * \param[out] Kr: the result of the convertion
     * \param[in]  Ks: the initial stiffness tensor
     * \param[in]  F0:  the deformation gradient
     * \param[in]  F1:  the deformation gradient
     * \param[in]  s:  the Cauchy stress tensor
     */
    template <unsigned short N, typename stress>
    static TFEL_MATERIAL_INLINE void exe(
        Result<N, stress> & Kr,
        const Source<N, stress>& Ks,
        const DeformationGradientTensor<N, stress>&,
        const DeformationGradientTensor<N, stress>& F1,
        const StressStensor<N, stress>& s){
        tfel::math::computeCauchyStressDerivativeFromKirchhoffStressDerivative(
            Kr, Ks, s, F1);
}  // end of exe
}
;  // end of struct FiniteStrainBehaviourTangentOperatorConverter
/*!
 * \brief partial specialisation of
 * FiniteStrainBehaviourTangentOperatorConverter structure
 */
TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(DTAU_DF, DS_DF){
    /*!
     * \param[out] Kr: the result of the convertion
     * \param[in]  Ks: the initial stiffness tensor
     * \param[in]  F0:  the deformation gradient
     * \param[in]  F1:  the deformation gradient
     * \param[in]  s:  the Cauchy stress tensor
     */
    template <unsigned short N, typename stress>
    static TFEL_MATERIAL_INLINE void exe(
        Result<N, stress> & Kr,
        const Source<N, stress>& Ks,
        const DeformationGradientTensor<N, stress>&,
        const DeformationGradientTensor<N, stress>& F1,
        const StressStensor<N, stress>& s){
        const auto sk2 =
            tfel::math::convertCauchyStressToSecondPiolaKirchhoffStress(s, F1);
tfel::math::computePushForwardDerivative(Kr, Ks, sk2, F1);
}  // end of exe
}
;  // end of struct FiniteStrainBehaviourTangentOperatorConverter
/*!
 * \brief partial specialisation of
 * FiniteStrainBehaviourTangentOperatorConverter structure
 */
TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(SPATIAL_MODULI,
                                                            DTAU_DF){
    /*!
     * \param[out] Kr: the result of the convertion
     * \param[in]  Ks: the initial stiffness tensor
     * \param[in]  F0:  the deformation gradient
     * \param[in]  F1:  the deformation gradient
     * \param[in]  s:  the Cauchy stress tensor
     */
    template <unsigned short N, typename stress>
    static TFEL_MATERIAL_INLINE void exe(
        Result<N, stress> & Kr,
        const Source<N, stress>& Ks,
        const DeformationGradientTensor<N, stress>& F0,
        const DeformationGradientTensor<N, stress>& F1,
        const StressStensor<N, stress>& s){
        using TangentOperator = FiniteStrainBehaviourTangentOperatorBase;
const auto CtJ =
    convert<TangentOperator::C_TAU_JAUMANN, TangentOperator::DTAU_DF>(
        Ks, F0, F1, s);
const auto J = tfel::math::det(F1);
Kr = convertSpatialModuliToKirchhoffJaumanRateModuli(CtJ, -J* s);
}  // end of exe
}
;  // end of struct FiniteStrainBehaviourTangentOperatorConverter
/*!
 * \brief partial specialisation of
 * FiniteStrainBehaviourTangentOperatorConverter structure
 */
TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(C_TAU_JAUMANN,
                                                            DTAU_DF){
    /*!
     * \param[out] Kr: the result of the convertion
     * \param[in]  Ks: the initial stiffness tensor
     * \param[in]  F0:  the deformation gradient
     * \param[in]  F1:  the deformation gradient
     * \param[in]  s:  the Cauchy stress tensor
     */
    template <unsigned short N, typename stress>
    static TFEL_MATERIAL_INLINE void exe(
        Result<N, stress> & Kr,
        const Source<N, stress>& Ks,
        const DeformationGradientTensor<N, stress>&,
        const DeformationGradientTensor<N, stress>& F1,
        const StressStensor<N, stress>&){
        for (size_type k = 0; k != 3; ++k){for (size_type l = 0; l != 3; ++l){
            if (((k == 1) && (l == 0)) || ((k == 2) && (l == 1))){continue;
}
const auto rkl = FiniteStrainBehaviourTangentOperatorConverterBase::index(k, l);
if (rkl >= tfel::math::StensorDimeToSize<N>::value) {
  continue;
}
const auto cCj = Ks * getDeformationGradient(F1, rkl);
for (size_type i = 0; i != cCj.size(); ++i) {
  Kr(i, rkl) = cCj(i);
}
}
}
}  // end of exe
}
;  // end of struct FiniteStrainBehaviourTangentOperatorConverter
/*!
 * \brief partial specialisation of
 * FiniteStrainBehaviourTangentOperatorConverter structure
 */
TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(C_TRUESDELL,
                                                            DTAU_DF){
    /*!
     * \param[out] Kr: the result of the convertion
     * \param[in]  Ks: the initial stiffness tensor
     * \param[in]  F0:  the deformation gradient
     * \param[in]  F1:  the deformation gradient
     * \param[in]  s:  the Cauchy stress tensor
     */
    template <unsigned short N, typename stress>
    static TFEL_MATERIAL_INLINE void exe(
        Result<N, stress> & Kr,
        const Source<N, stress>& Ks,
        const DeformationGradientTensor<N, stress>& F0,
        const DeformationGradientTensor<N, stress>& F1,
        const StressStensor<N, stress>& s){
        using TangentOperator = FiniteStrainBehaviourTangentOperatorBase;
const auto Cs =
    convert<TangentOperator::SPATIAL_MODULI, TangentOperator::DTAU_DF>(
        Ks, F0, F1, s);
Kr = Cs / (tfel::math::det(F1));
}  // end of exe
}
;  // end of struct FiniteStrainBehaviourTangentOperatorConverter
/*!
 * \brief partial specialisation of
 * FiniteStrainBehaviourTangentOperatorConverter structure
 */
TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(ABAQUS,
                                                            C_TAU_JAUMANN){
    /*!
     * \param[out] Kr: the result of the convertion
     * \param[in]  Ks: the initial stiffness tensor
     * \param[in]  F0:  the deformation gradient
     * \param[in]  F1:  the deformation gradient
     * \param[in]  s:  the Cauchy stress tensor
     */
    template <unsigned short N, typename stress>
    static TFEL_MATERIAL_INLINE void exe(
        Result<N, stress> & Kr,
        const Source<N, stress>& Ks,
        const DeformationGradientTensor<N, stress>&,
        const DeformationGradientTensor<N, stress>& F1,
        const StressStensor<N, stress>&){Kr = Ks / (tfel::math::det(F1));
}  // end of exe
}
;  // end of struct FiniteStrainBehaviourTangentOperatorConverter
/*!
 * \brief partial specialisation of
 * FiniteStrainBehaviourTangentOperatorConverter structure
 */
TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(C_TAU_JAUMANN,
                                                            ABAQUS){
    /*!
     * \param[out] Kr: the result of the convertion
     * \param[in]  Ks: the initial stiffness tensor
     * \param[in]  F0:  the deformation gradient
     * \param[in]  F1:  the deformation gradient
     * \param[in]  s:  the Cauchy stress tensor
     */
    template <unsigned short N, typename stress>
    static TFEL_MATERIAL_INLINE void exe(
        Result<N, stress> & Kr,
        const Source<N, stress>& Ks,
        const DeformationGradientTensor<N, stress>&,
        const DeformationGradientTensor<N, stress>& F1,
        const StressStensor<N, stress>&){Kr = Ks * (tfel::math::det(F1));
}  // end of exe
}
;  // end of struct FiniteStrainBehaviourTangentOperatorConverter
/*!
 * \brief partial specialisation of
 * FiniteStrainBehaviourTangentOperatorConverter structure
 */
TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(C_TAU_JAUMANN,
                                                            SPATIAL_MODULI){
    /*!
     * \param[out] Kr: the result of the convertion
     * \param[in]  Ks: the initial stiffness tensor
     * \param[in]  F0:  the deformation gradient
     * \param[in]  F1:  the deformation gradient
     * \param[in]  s:  the Cauchy stress tensor
     */
    template <unsigned short N, typename stress>
    static TFEL_MATERIAL_INLINE void exe(
        Result<N, stress> & Kr,
        const Source<N, stress>& Ks,
        const DeformationGradientTensor<N, stress>&,
        const DeformationGradientTensor<N, stress>& F1,
        const StressStensor<N, stress>& s){const auto J = tfel::math::det(F1);
Kr = tfel::math::convertSpatialModuliToKirchhoffJaumanRateModuli(Ks, J* s);
}  // end of exe
}
;  // end of struct FiniteStrainBehaviourTangentOperatorConverter
/*!
 * \brief partial specialisation of
 * FiniteStrainBehaviourTangentOperatorConverter structure
 */
TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(SPATIAL_MODULI,
                                                            C_TAU_JAUMANN){
    /*!
     * \param[out] Kr: the result of the convertion
     * \param[in]  Ks: the initial stiffness tensor
     * \param[in]  F0:  the deformation gradient
     * \param[in]  F1:  the deformation gradient
     * \param[in]  s:  the Cauchy stress tensor
     */
    template <unsigned short N, typename stress>
    static TFEL_MATERIAL_INLINE void exe(
        Result<N, stress> & Kr,
        const Source<N, stress>& Ks,
        const DeformationGradientTensor<N, stress>&,
        const DeformationGradientTensor<N, stress>& F1,
        const StressStensor<N, stress>& s){const auto J = tfel::math::det(F1);
// a little trick: the invert of the transformation from the
// spatial moduli to the KirchhoffJaumanRateModuli is obtained
// by changing the sign of the Kirchoff stress
Kr = tfel::math::convertSpatialModuliToKirchhoffJaumanRateModuli(Ks, -J* s);
}  // end of exe
}
;  // end of struct FiniteStrainBehaviourTangentOperatorConverter
/*!
 * \brief partial specialisation of
 * FiniteStrainBehaviourTangentOperatorConverter structure
 */
TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(ABAQUS, DTAU_DF){
    /*!
     * \param[out] Kr: the result of the convertion
     * \param[in]  Ks: the initial stiffness tensor
     * \param[in]  F0:  the deformation gradient
     * \param[in]  F1:  the deformation gradient
     * \param[in]  s:  the Cauchy stress tensor
     */
    template <unsigned short N, typename stress>
    static TFEL_MATERIAL_INLINE void exe(
        Result<N, stress> & Kr,
        const Source<N, stress>& Ks,
        const DeformationGradientTensor<N, stress>&,
        const DeformationGradientTensor<N, stress>& F1,
        const StressStensor<N, stress>&){const auto iJ = 1 /
                                                         tfel::math::det(F1);
for (size_type k = 0; k != 3; ++k) {
  for (size_type l = 0; l != 3; ++l) {
    if (((k == 1) && (l == 0)) || ((k == 2) && (l == 1))) {
      continue;
    }
    const auto rkl =
        FiniteStrainBehaviourTangentOperatorConverterBase::index(k, l);
    if (rkl >= tfel::math::StensorDimeToSize<N>::value) {
      continue;
    }
    const auto cCj = Ks * getDeformationGradient(F1, rkl);
    for (size_type i = 0; i != cCj.size(); ++i) {
      Kr(i, rkl) = cCj(i) * iJ;
    }
  }
}
}  // end of exe
}
;  // end of struct FiniteStrainBehaviourTangentOperatorConverter
/*!
 * \brief partial specialisation of
 * FiniteStrainBehaviourTangentOperatorConverter structure
 */
TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(DTAU_DF,
                                                            C_TAU_JAUMANN){
    /*!
     * \param[out] Kr: the result of the convertion
     * \param[in]  Ks: the initial stiffness tensor
     * \param[in]  F0:  the deformation gradient
     * \param[in]  F1:  the deformation gradient
     * \param[in]  s:  the Cauchy stress tensor
     */
    template <unsigned short N, typename stress>
    static TFEL_MATERIAL_INLINE void exe(
        Result<N, stress> & Kr,
        const Source<N, stress>& Ks,
        const DeformationGradientTensor<N, stress>&,
        const DeformationGradientTensor<N, stress>& F1,
        const StressStensor<N, stress>& s){
        // not supported by gcc 4.7.2: auto toT2toST2=[](const t2tot2& s) ->
        // t2tost2{
        auto toT2toST2 = [&](tfel::math::t2tot2<N, stress> src) {
          tfel::math::t2tost2<N, stress> r;
          for (unsigned short i = 0; i != 3; ++i) {
            for (unsigned short j = 0;
                 j != tfel::math::TensorDimeToSize<N>::value; ++j) {
              r(i, j) = src(i, j);
            }
          }
          for (unsigned short i = 0;
               i != tfel::math::StensorDimeToSize<N>::value - 3; ++i) {
            for (unsigned short j = 0;
                 j != tfel::math::TensorDimeToSize<N>::value; ++j) {
              r(3 + i, j) = (src(3 + 2 * i, j) + src(3 + 2 * i + 1, j)) *
                            tfel::math::Cste<stress>::isqrt2;
            }
          }
          return r;
        };
const auto t = s * tfel::math::det(F1);
const auto tus = tfel::math::unsyme(t);
const auto dD = tfel::math::computeRateOfDeformationDerivative(F1);
const auto dW = tfel::math::computeSpinRateDerivative(F1);
Kr = Ks * dD + toT2toST2((tfel::math::t2tot2<N, base_type<stress>>::tpld(tus) -
                          tfel::math::t2tot2<N, base_type<stress>>::tprd(tus)) *
                         dW);
}  // end of exe
}
;  // end of struct FiniteStrainBehaviourTangentOperatorConverter
/*!
 * \brief partial specialisation of
 * FiniteStrainBehaviourTangentOperatorConverter structure
 */
TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(DTAU_DF, ABAQUS){
    /*!
     * \param[out] Kr: the result of the convertion
     * \param[in]  Ks: the initial stiffness tensor
     * \param[in]  F0:  the deformation gradient
     * \param[in]  F1:  the deformation gradient
     * \param[in]  s:  the Cauchy stress tensor
     */
    template <unsigned short N, typename stress>
    static TFEL_MATERIAL_INLINE void exe(
        Result<N, stress> & Kr,
        const Source<N, stress>& Ks,
        const DeformationGradientTensor<N, stress>&,
        const DeformationGradientTensor<N, stress>& F1,
        const StressStensor<N, stress>& s){
        // not supported by gcc 4.7.2: auto toT2toST2=[](const t2tot2& s) ->
        // t2tost2{
        auto toT2toST2 = [&](tfel::math::t2tot2<N, stress> src) {
          tfel::math::t2tost2<N, stress> r;
          for (unsigned short i = 0; i != 3; ++i) {
            for (unsigned short j = 0;
                 j != tfel::math::TensorDimeToSize<N>::value; ++j) {
              r(i, j) = src(i, j);
            }
          }
          for (unsigned short i = 0;
               i != tfel::math::StensorDimeToSize<N>::value - 3; ++i) {
            for (unsigned short j = 0;
                 j != tfel::math::TensorDimeToSize<N>::value; ++j) {
              r(3 + i, j) = (src(3 + 2 * i, j) + src(3 + 2 * i + 1, j)) *
                            tfel::math::Cste<stress>::isqrt2;
            }
          }
          return r;
        };
const auto J = tfel::math::det(F1);
const auto t = s * J;
const auto tus = tfel::math::unsyme(t);
const auto dD = tfel::math::computeRateOfDeformationDerivative(F1);
const auto dW = tfel::math::computeSpinRateDerivative(F1);
Kr = J * Ks * dD +
     toT2toST2((tfel::math::t2tot2<N, base_type<stress>>::tpld(tus) -
                tfel::math::t2tot2<N, base_type<stress>>::tprd(tus)) *
               dW);
}  // end of exe
}
;  // end of struct FiniteStrainBehaviourTangentOperatorConverter
/*!
 * \brief partial specialisation of
 * FiniteStrainBehaviourTangentOperatorConverter structure
 */
TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(DTAU_DF,
                                                            SPATIAL_MODULI){
    /*!
     * \param[out] Kr: the result of the convertion
     * \param[in]  Ks: the initial stiffness tensor
     * \param[in]  F0:  the deformation gradient
     * \param[in]  F1:  the deformation gradient
     * \param[in]  s:  the Cauchy stress tensor
     */
    template <unsigned short N, typename stress>
    static TFEL_MATERIAL_INLINE void exe(
        Result<N, stress> & Kr,
        const Source<N, stress>& Ks,
        const DeformationGradientTensor<N, stress>& F0,
        const DeformationGradientTensor<N, stress>& F1,
        const StressStensor<N, stress>& s){
        using TangentOperator = FiniteStrainBehaviourTangentOperatorBase;
// not supported by gcc 4.7.2: auto toT2toST2=[](const t2tot2& s) -> t2tost2{
auto toT2toST2 =
    [&](tfel::math::t2tot2<N, stress> src) -> tfel::math::t2tost2<N, stress> {
  tfel::math::t2tost2<N, stress> r;
  for (unsigned short i = 0; i != 3; ++i) {
    for (unsigned short j = 0; j != tfel::math::TensorDimeToSize<N>::value;
         ++j) {
      r(i, j) = src(i, j);
    }
  }
  for (unsigned short i = 0; i != tfel::math::StensorDimeToSize<N>::value - 3;
       ++i) {
    for (unsigned short j = 0; j != tfel::math::TensorDimeToSize<N>::value;
         ++j) {
      r(3 + i, j) = (src(3 + 2 * i, j) + src(3 + 2 * i + 1, j)) *
                    tfel::math::Cste<stress>::isqrt2;
      ;
    }
  }
  return r;
};
const auto CJ =
    convert<TangentOperator::C_TAU_JAUMANN, TangentOperator::SPATIAL_MODULI>(
        Ks, F0, F1, s);
const auto t = s * det(F1);
const auto tus = tfel::math::unsyme(t);
const auto dD = tfel::math::computeRateOfDeformationDerivative(F1);
const auto dW = tfel::math::computeSpinRateDerivative(F1);
Kr = CJ * dD + toT2toST2((tfel::math::t2tot2<N, base_type<stress>>::tpld(tus) -
                          tfel::math::t2tot2<N, base_type<stress>>::tprd(tus)) *
                         dW);
}  // end of exe
}
;  // end of struct FiniteStrainBehaviourTangentOperatorConverter
/*!
 * \brief partial specialisation of
 * FiniteStrainBehaviourTangentOperatorConverter structure
 */
TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(DSIG_DF, ABAQUS){
    /*!
     * \param[out] Kr: the result of the convertion
     * \param[in]  Ks: the initial stiffness tensor
     * \param[in]  F0:  the deformation gradient
     * \param[in]  F1:  the deformation gradient
     * \param[in]  s:  the Cauchy stress tensor
     */
    template <unsigned short N, typename stress>
    static TFEL_MATERIAL_INLINE void exe(
        Result<N, stress> & Kr,
        const Source<N, stress>& Ks,
        const DeformationGradientTensor<N, stress>& F0,
        const DeformationGradientTensor<N, stress>& F1,
        const StressStensor<N, stress>& s){
        using TangentOperator = FiniteStrainBehaviourTangentOperatorBase;
const auto Dt =
    convert<TangentOperator::DTAU_DF, TangentOperator::ABAQUS>(Ks, F0, F1, s);
Kr = convert<TangentOperator::DSIG_DF, TangentOperator::DTAU_DF>(Dt, F0, F1, s);
}  // end of exe
}
;  // end of struct FiniteStrainBehaviourTangentOperatorConverter
template <FiniteStrainBehaviourTangentOperatorBase::Flag TangenOperatorType1,
          FiniteStrainBehaviourTangentOperatorBase::Flag TangenOperatorType2,
          unsigned short N,
          typename StressType>
tangent_operator<TangenOperatorType1, N, StressType> convert(
    const tangent_operator<TangenOperatorType2, N, StressType>& K,
    const tfel::math::tensor<N, tfel::typetraits::base_type<StressType>>& F0,
    const tfel::math::tensor<N, tfel::typetraits::base_type<StressType>>& F1,
    const tfel::math::stensor<N, StressType>& s) {
  tangent_operator<TangenOperatorType1, N, StressType> r;
  FiniteStrainBehaviourTangentOperatorConverter<
      TangenOperatorType1, TangenOperatorType2>::exe(r, K, F0, F1, s);
  return r;
}

template <FiniteStrainBehaviourTangentOperatorBase::Flag TangenOperatorType1,
          FiniteStrainBehaviourTangentOperatorBase::Flag TangenOperatorType2,
          unsigned short N,
          typename StressType>
void convert(
    tangent_operator<TangenOperatorType1, N, StressType>& Kr,
    const tangent_operator<TangenOperatorType2, N, StressType>& Ks,
    const tfel::math::tensor<N, tfel::typetraits::base_type<StressType>>& F0,
    const tfel::math::tensor<N, tfel::typetraits::base_type<StressType>>& F1,
    const tfel::math::stensor<N, StressType>& s) {
  FiniteStrainBehaviourTangentOperatorConverter<
      TangenOperatorType1, TangenOperatorType2>::exe(Kr, Ks, F0, F1, s);
}

}  // end of namespace material

}  // end of namespace tfel

#endif /* LIB_TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATOR_IXX_ */
