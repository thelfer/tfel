/*!
 * \file   StiffnessTensor.hxx
 * \brief
 * \author Thomas Helfer
 * \date   23 oct. 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_STIFFNESSTENSOR_HXX
#define LIB_TFEL_MATERIAL_STIFFNESSTENSOR_HXX

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Material/ModellingHypothesis.hxx"
#include "TFEL/Material/OrthotropicAxesConvention.hxx"

namespace tfel::material {

  //! a small enumerattion
  enum struct StiffnessTensorAlterationCharacteristic {
    UNALTERED,
    ALTERED
  };  // end of enum struct StiffnessTensorAlterationCharacteristic

  /*!
   * a simple metafunction which returns the more naturel stiffness
   * tensor alteration characteristic for a given hypothesis
   * \tparam H: modelling hypothesis
   */
  template <ModellingHypothesis::Hypothesis H>
  struct GetDefaultStiffnessTensorAlterationCharacteristic {
    static constexpr auto value =
        StiffnessTensorAlterationCharacteristic::UNALTERED;
  };

  template <>
  struct GetDefaultStiffnessTensorAlterationCharacteristic<
      ModellingHypothesis::PLANESTRESS> {
    static constexpr auto value =
        StiffnessTensorAlterationCharacteristic::ALTERED;
  };

  template <>
  struct GetDefaultStiffnessTensorAlterationCharacteristic<
      ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS> {
    static constexpr auto value =
        StiffnessTensorAlterationCharacteristic::ALTERED;
  };

  /*!
   * \brief compute the altered stiffness tensor from the unaltered
   * one.
   */
  template <ModellingHypothesis::Hypothesis H>
  struct ComputeAlteredStiffnessTensor {
    /*!
     * \param[out] Da : altered stiffness
     * \param[in]  D  : unaltered stiffness
     */
    template <typename StressType>
    static TFEL_MATERIAL_INLINE void exe(
        tfel::math::st2tost2<ModellingHypothesisToSpaceDimension<H>::value,
                             StressType>&,
        const tfel::math::st2tost2<
            ModellingHypothesisToSpaceDimension<H>::value,
            StressType>&);
  };
  /*!
   * \brief compute the altered stiffness tensor from the unaltered
   * one in plane stress.
   */
  template <>
  struct ComputeAlteredStiffnessTensor<ModellingHypothesis::PLANESTRESS> {
    /*!
     * \param[out] Da : altered stiffness
     * \param[in]  D  : unaltered stiffness
     */
    template <typename StressType>
    static TFEL_MATERIAL_INLINE void exe(
        tfel::math::st2tost2<2u, StressType>&,
        const tfel::math::st2tost2<2u, StressType>&);
  };

  /*!
   * \param[out] D:  stiffness tensor
   * \param[in]  yg: young modulus
   * \param[in]  nu: poisson ratio
   */
  template <unsigned short N,
            StiffnessTensorAlterationCharacteristic,
            typename StressType,
            typename RealType>
  TFEL_MATERIAL_INLINE void computeIsotropicStiffnessTensorII(
      tfel::math::st2tost2<N, StressType>&, const StressType, const RealType);
  /*!
   * \param[out] D: stiffness tensor
   * \param[in]  yg1:  young modulus in the first direction
   * \param[in]  yg2:  young modulus in the second direction
   * \param[in]  yg3:  young modulus in the third direction
   * \param[in]  nu12: PoissonRatio
   * \param[in]  nu23: PoissonRatio
   * \param[in]  nu13: PoissonRatio
   * \param[in]  G12:  shear modulus
   * \param[in]  G23:  shear modulus
   * \param[in]  G13:  shear modulus
   */
  template <unsigned short N,
            StiffnessTensorAlterationCharacteristic,
            typename StressType,
            typename RealType>
  TFEL_MATERIAL_INLINE void computeOrthotropicStiffnessTensorII(
      tfel::math::st2tost2<N, StressType>&,
      const StressType,
      const StressType,
      const StressType,
      const RealType,
      const RealType,
      const RealType,
      const StressType,
      const StressType,
      const StressType);

  /*!
   * \param[out] D:  stiffness tensor
   * \param[in]  yg: young modulus
   * \param[in]  nu: poisson ratio
   */
  template <ModellingHypothesis::Hypothesis H,
            StiffnessTensorAlterationCharacteristic,
            typename StressType,
            typename RealType>
  TFEL_MATERIAL_INLINE void computeIsotropicStiffnessTensor(
      tfel::math::st2tost2<ModellingHypothesisToSpaceDimension<H>::value,
                           StressType>&,
      const StressType,
      const RealType);
  /*!
   * \param[out] D: stiffness tensor
   * \param[in]  yg1:  young modulus in the first direction
   * \param[in]  yg2:  young modulus in the second direction
   * \param[in]  yg3:  young modulus in the third direction
   * \param[in]  nu12: PoissonRatio
   * \param[in]  nu23: PoissonRatio
   * \param[in]  nu13: PoissonRatio
   * \param[in]  G12:  shear modulus
   * \param[in]  G23:  shear modulus
   * \param[in]  G13:  shear modulus
   */
  template <ModellingHypothesis::Hypothesis H,
            StiffnessTensorAlterationCharacteristic,
            typename StressType,
            typename RealType>
  TFEL_MATERIAL_INLINE void computeOrthotropicStiffnessTensor(
      tfel::math::st2tost2<ModellingHypothesisToSpaceDimension<H>::value,
                           StressType>&,
      const StressType,
      const StressType,
      const StressType,
      const RealType,
      const RealType,
      const RealType,
      const StressType,
      const StressType,
      const StressType);

  /*!
   * \param[out] D: stiffness tensor
   * \param[in]  yg1:  young modulus in the first direction
   * \param[in]  yg2:  young modulus in the second direction
   * \param[in]  yg3:  young modulus in the third direction
   * \param[in]  nu12: PoissonRatio
   * \param[in]  nu23: PoissonRatio
   * \param[in]  nu13: PoissonRatio
   * \param[in]  G12:  shear modulus
   * \param[in]  G23:  shear modulus
   * \param[in]  G13:  shear modulus
   */
  template <ModellingHypothesis::Hypothesis H,
            StiffnessTensorAlterationCharacteristic smt,
            OrthotropicAxesConvention c,
            typename StressType,
            typename RealType>
  TFEL_MATERIAL_INLINE void computeOrthotropicStiffnessTensor(
      tfel::math::st2tost2<ModellingHypothesisToSpaceDimension<H>::value,
                           StressType>&,
      const StressType,
      const StressType,
      const StressType,
      const RealType,
      const RealType,
      const RealType,
      const StressType,
      const StressType,
      const StressType);

}  // end of namespace tfel::material

#include "TFEL/Material/StiffnessTensor.ixx"

#endif /* LIB_TFEL_MATERIAL_STIFFNESSTENSOR_HXX */
