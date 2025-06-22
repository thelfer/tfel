/*!
 * \file   mfront/include/MFront/GenericBehaviour/ComputeStiffnessTensor.hxx
 * \brief  This file declares the CastemComputeStiffnessTensor class
 * \author Thomas Helfer
 * \date   25/07/2018
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_GENERIC_COMPUTESTIFFNESSTENSOR_HXX
#define LIB_MFRONT_GENERIC_COMPUTESTIFFNESSTENSOR_HXX

#include <type_traits>
#include "MFront/MFrontConfig.hxx"
#include "TFEL/Config/TFELTypes.hxx"
#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Material/ModellingHypothesis.hxx"

namespace mfront {

  namespace gb {

    /*!
     * \brief compute the unaltered elastic stiffness tensor
     * \param[out] C: stiffness tensor
     * \param[out] mps: elastic material properties
     */
    template <tfel::material::ModellingHypothesis::Hypothesis H,
              typename stress,
              typename real>
    typename std::enable_if<
        tfel::material::ModellingHypothesisToSpaceDimension<H>::value == 1u,
        void>::type
    computeOrthotropicUnAlteredElasticStiffnessTensor(
        tfel::math::st2tost2<1u, stress>&, const real* const);
    /*!
     * \brief compute the unaltered elastic stiffness tensor
     * \param[out] C: stiffness tensor
     * \param[out] mps: elastic material properties
     */
    template <tfel::material::ModellingHypothesis::Hypothesis H,
              typename stress,
              typename real>
    typename std::enable_if<
        tfel::material::ModellingHypothesisToSpaceDimension<H>::value == 2u,
        void>::type
    computeOrthotropicUnAlteredElasticStiffnessTensor(
        tfel::math::st2tost2<2u, stress>&, const real* const);
    /*!
     * \brief compute the unaltered elastic stiffness tensor
     * \param[out] C: stiffness tensor
     * \param[out] mps: elastic material properties
     */
    template <tfel::material::ModellingHypothesis::Hypothesis H,
              typename stress,
              typename real>
    typename std::enable_if<
        tfel::material::ModellingHypothesisToSpaceDimension<H>::value == 3u,
        void>::type
    computeOrthotropicUnAlteredElasticStiffnessTensor(
        tfel::math::st2tost2<3u, stress>&, const real* const);
    /*!
     * \brief compute the unaltered elastic stiffness tensor
     * \param[out] C: stiffness tensor
     * \param[out] mps: elastic material properties
     */
    template <tfel::material::ModellingHypothesis::Hypothesis H,
              typename stress,
              typename real>
    typename std::enable_if<
        tfel::material::ModellingHypothesisToSpaceDimension<H>::value == 1u,
        void>::type
    computeOrthotropicAlteredElasticStiffnessTensor(
        tfel::math::st2tost2<1u, stress>&, const real* const);
    /*!
     * \brief compute the unaltered elastic stiffness tensor
     * \param[out] C: stiffness tensor
     * \param[out] mps: elastic material properties
     */
    template <tfel::material::ModellingHypothesis::Hypothesis H,
              typename stress,
              typename real>
    typename std::enable_if<
        tfel::material::ModellingHypothesisToSpaceDimension<H>::value == 2u,
        void>::type
    computeOrthotropicAlteredElasticStiffnessTensor(
        tfel::math::st2tost2<2u, stress>&, const real* const);
    /*!
     * \brief compute the unaltered elastic stiffness tensor
     * \param[out] C: stiffness tensor
     * \param[out] mps: elastic material properties
     */
    template <tfel::material::ModellingHypothesis::Hypothesis H,
              typename stress,
              typename real>
    typename std::enable_if<
        tfel::material::ModellingHypothesisToSpaceDimension<H>::value == 3u,
        void>::type
    computeOrthotropicAlteredElasticStiffnessTensor(
        tfel::math::st2tost2<3u, stress>&, const real* const);

  }  // end of namespace gb

}  // end of namespace mfront

#include "MFront/GenericBehaviour/ComputeStiffnessTensor.ixx"

#endif /* LIB_MFRONT_GENERIC_COMPUTESTIFFNESSTENSOR_HXX */
