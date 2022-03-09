/*!
 * \file   include/TFEL/Material/CamClayElasticityStressPotential.hxx
 * \brief
 * \author Thomas Helfer
 * \date   07/03/2022
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_CAMCLAYELASTICITYSTRESSPOTENTIAL_HXX
#define LIB_TFEL_MATERIAL_CAMCLAYELASTICITYSTRESSPOTENTIAL_HXX

#include <tuple>
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/st2tost2.hxx"

namespace tfel::material {

  /*!
   * \brief compute the stress and the stress derivative using the Cam-Clay
   * relationship assuming that the shear modulus is deduced from the Poisson
   * ratio and the bulk modulus. The shear modulus is thus pressure-dependant.
   *
   * The deviatoric part of the stress is assumed proportional to the current
   * elastic strain.
   *
   * \param[in] eel: elastic strain
   * \param[in] e0: initial void ratio
   * \param[in] kappa: unloading/reloading slope
   * \param[in] nu: poisson ratio
   * \param[in] p_min: pressure threshold
   */
  template <typename StrainStensorType,
            typename real,
            typename stress>
  std::tuple<
      tfel::math::stensor<tfel::math::getSpaceDimension<StrainStensorType>(),
                          stress>,
      tfel::math::st2tost2<tfel::math::getSpaceDimension<StrainStensorType>(),
                           stress>>
  computeCamClayElasticity(const StrainStensorType&,
                           const real,
                           const real,
                           const real,
                           const stress);
  /*!
   * \brief compute the stress and the stress derivative using the Cam-Clay
   * relationship assuming that the shear modulus is deduced from the Poisson
   * ratio and the bulk modulus. The shear modulus is thus pressure-dependant.
   *
   * The increment of the deviatoric part of the stress is assumed proportional
   * to the elastic strain increment.
   *
   * \param[in] sig0: stress at the beginning of the time step
   * \param[in] eel: elastic strain at the beginning of the time step
   * \param[in] deel: elastic strain increment
   * \param[in] e0: initial void ratio
   * \param[in] kappa: unloading/reloading slope
   * \param[in] nu: poisson ratio
   * \param[in] p_min: pressure threshold
   */
  template <typename StressStensorType,
            typename StrainStensorType,
            typename StrainStensorType2,
            typename real,
            typename stress>
  std::tuple<
      tfel::math::stensor<tfel::math::getSpaceDimension<StressStensorType>(),
                          stress>,
      tfel::math::st2tost2<tfel::math::getSpaceDimension<StressStensorType>(),
                           stress>>
  computeCamClayElasticityII(const StressStensorType&,
                             const StrainStensorType&,
                             const StrainStensorType2&,
                             const real,
                             const real,
                             const real,
                             const stress);
  /*!
   * \brief compute the stress and the stress derivative using the Cam-Clay
   * relationship.
   *
   * \param[in] eel: elastic strain
   * \param[in] e0: initial void ratio
   * \param[in] kappa: unloading/reloading slope
   * \param[in] mu: shear modulus
   * \param[in] p_min: pressure threshold
   */
  template <typename StrainStensorType,
            typename real,
            typename stress>
  std::tuple<
      tfel::math::stensor<tfel::math::getSpaceDimension<StrainStensorType>(),
                          stress>,
      tfel::math::st2tost2<tfel::math::getSpaceDimension<StrainStensorType>(),
                           stress>>
  computeCamClayElasticityIII(const StrainStensorType&,
                              const real,
                              const real,
                              const stress,
                              const stress);

}  // end of namespace tfel::material

#include "TFEL/Material/CamClayElasticityStressPotential.ixx"

#endif /* LIB_TFEL_MATERIAL_CAMCLAYELASTICITYSTRESSPOTENTIAL_HXX */
