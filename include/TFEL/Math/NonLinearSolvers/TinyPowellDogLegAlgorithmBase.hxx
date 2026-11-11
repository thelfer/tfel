/*!
 * \file include/TFEL/Math/NonLinearSolvers/TinyPowellDogLegAlgorithmBase.hxx
 * \brief This file gathers the versions of the functions implementing Powell'
 * dog leg algorithm, one per compatibility mode
 * \author Thomas Helfer
 * \date 06/07/2021
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_TINYPOWELLDOGLEGALGORITHMBASE_HXX
#define LIB_TFEL_MATH_TINYPOWELLDOGLEGALGORITHMBASE_HXX

#include "TFEL/Math/NonLinearSolvers/TinyPowellDogLegAlgorithmBase-5.2.0.hxx"
#include "TFEL/Math/NonLinearSolvers/TinyPowellDogLegAlgorithmBase-6.0.0.hxx"

namespace tfel::math {

  using tfel::math::v5_2_0::applyPowellDogLegAlgorithm;

}  // end of namespace tfel::math

namespace tfel::math::future {

  using tfel::math::v6_0_0::applyPowellDogLegAlgorithm;
  using tfel::math::v6_0_0::updatePowellDogLegTrustRegionRadius;

}  // end of namespace tfel::math::future

#endif /* LIB_TFEL_MATH_TINYPOWELLDOGLEGALGORITHMBASE_HXX */
