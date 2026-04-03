/*!
 * \file   include/TFEL/Material/HomogenizationSecondMoments.hxx
 * \author Antoine Martin
 * \date   7 January 2026
 * \brief  This file gives the second moments with some classical homogenization
 * schemes. \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved. This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered with the
 * sources of TFEL. CEA or EDF may also distribute this project under specific
 * licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_HOMOGENIZATIONSECONDMOMENTS_HXX
#define LIB_TFEL_MATERIAL_HOMOGENIZATIONSECONDMOMENTS_HXX

#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Material/IsotropicModuli.hxx"

namespace tfel::material::homogenization::elasticity {

  /*!
   * This function computes the averages on each phase of the square of the
   * equivalent strain assuming a Hashin-Shtrikman homogenization scheme, and
   * assuming two phases: spherical isotropic inclusions embedded in an
   * isotropic matrix (note that the inclusion can be softer or stiffer than the
   * matrix) \tparam StressType: type of the elastic constants related to the
   * matrix and the inclusions \return an object of type
   * std::pair<real<StressType>,real<StressType>> \param [in] IM0:
   * IsotropicModuli of the matrix \param [in] f: volumic fraction of inclusions
   * \param [in] IMi: IsotropicModuli of the spherical inclusions
   * \param [in] em2: Square of the hydrostatic part of the macroscopic strain
   * (third of the trace of the strain) \param [in] eeq2: Square of the
   * equivalent macroscopic strain
   */
  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<
           tfel::math::unit::Stress,
           StressType>()) TFEL_HOST_DEVICE const std::
      pair<types::real<StressType>, types::real<StressType>> computeMeanSquaredEquivalentStrain(
          const IsotropicModuli<StressType>&,
          const types::real<StressType>&,
          const IsotropicModuli<StressType>&,
          const types::real<StressType>&,
          const types::real<StressType>&);

  /*!
   * This function computes the averages on each phase of the square of the
   * third of the trace of the strain assuming a Hashin-Shtrikman homogenization
   * scheme, and assuming two phases: spherical isotropic inclusions embedded in
   * an isotropic matrix (note that the inclusion can be softer or stiffer than
   * the matrix). \tparam StressType: type of the elastic constants related to
   * the matrix and the inclusions \return an object of type
   * std::pair<real<StressType>,real<StressType>> \param [in] IM0:
   * IsotropicModuli of the matrix \param [in] f: volumic fraction of inclusions
   * \param [in] IMi: IsotropicModuli of the spherical inclusions
   * \param [in] em2: Square of the hydrostatic part of the macroscopic strain
   * (third of the trace of the strain) \param [in] eeq2: Square of the
   * equivalent macroscopic strain
   */
  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<
           tfel::math::unit::Stress,
           StressType>()) TFEL_HOST_DEVICE const std::
      pair<types::real<StressType>, types::real<StressType>> computeMeanSquaredHydrostaticStrain(
          const IsotropicModuli<StressType>&,
          const types::real<StressType>&,
          const IsotropicModuli<StressType>&,
          const types::real<StressType>&,
          const types::real<StressType>&);

}  // end of namespace tfel::material::homogenization::elasticity

#include "TFEL/Material/HomogenizationSecondMoments.ixx"

#endif /* LIB_TFEL_MATERIAL_HOMOGENIZATIONSECONDMOMENTS_HXX */
