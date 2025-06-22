/*!
 * \file   OrthotropicAxesConvention.hxx
 * \brief
 * \author THOMAS HELFER
 * \date   24 août 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_ORTHOTROPICAXESCONVENTION_HXX_
#define LIB_TFEL_MATERIAL_ORTHOTROPICAXESCONVENTION_HXX_

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Material/StiffnessTensor.hxx"
#include "TFEL/Material/ModellingHypothesis.hxx"

namespace tfel {

  namespace material {

    /*!
     * Most finite element solver can't have a uniq definition of the
     * orthotropic axes for all the modelling hypotheses. One thus
     * need to define a convention. This leads to many troubles when
     * trying to define an the hill tensor or the thermal expansion.
     *
     * For example, one can define a pipe using the following axes
     * definition:
     * - (rr,zz,tt,...) in 3D, 2D Axysymmetric, 1D Axysymmetric
     *   generalised plane strain or generalised plane stress.
     * - (rr,zz,tt,...) in 2D plane stress, strain, generalized plane
     *   strain.
     * This means that we have to exchange the second and third
     * axis. This is the PIPE (orthotropic axes) convention.
     */
    enum class OrthotropicAxesConvention {
      DEFAULT,
      PIPE
    };  // end of enum class OrthotropicAxesConvention

    /*!
     * convert a diagonal tensor from a convention to another.
     * \tparam mh       : modelling hypothesis
     * \tparam c        : convention for the orthotropic axes
     * \tparam T        : type holded by the tensor
     * \param[in,out] s : stensor
     */
    template <ModellingHypothesis::Hypothesis mh,
              OrthotropicAxesConvention c,
              typename T>
    void convertStressFreeExpansionStrain(
        tfel::math::stensor<ModellingHypothesisToSpaceDimension<mh>::value,
                            T>&);

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

  }  // end of namespace material

}  // end of namespace tfel

#include "TFEL/Material/OrthotropicAxesConvention.ixx"

#endif /* LIB_TFEL_MATERIAL_ORTHOTROPICAXESCONVENTION_HXX_ */
