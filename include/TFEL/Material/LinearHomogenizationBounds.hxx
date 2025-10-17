/*!
 * \file   include/TFEL/Material/LinearHomogenizationBounds.hxx
 * \author Antoine Martin
 * \date   23 January 2025
 * \brief  This file declares some well-known homogenization bounds.
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All
 * rights reserved. This project is publicly released under either the GNU GPL
 * Licence or the CECILL-A licence. A copy of thoses licences are delivered with
 * the sources of TFEL. CEA or EDF may also distribute this project under
 * specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_LINEARHOMOGENIZATIONBOUNDS_HXX
#define LIB_TFEL_MATERIAL_LINEARHOMOGENIZATIONBOUNDS_HXX

#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Material/IsotropicEshelbyTensor.hxx"

namespace tfel::material::homogenization::elasticity {
  /*!
   * This function gives the Voigt stiffness
   * \tparam real: underlying type
   * \tparam StressType: type of the elastic constants related to the phases
   * \tparam N: number of phases
   * \tparam d: dimension (2 or 3)
   * \return an object of type st2tost2<d,StressType>
   * \param [in] tab_f: std::array<real,N> of volumic fractions of phases
   * \param [in] tab_C: std::array<st2tost2<d,StressType>,N> of stiffness
   * tensors of phases
   */
  template <unsigned short int d,
            unsigned int N,
            tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>()) TFEL_HOST_DEVICE
      const tfel::math::st2tost2<d, StressType> computeVoigtStiffness(
          const std::array<types::real<StressType>, N>&,
          const std::array<tfel::math::st2tost2<d, StressType>, N>&);

  /*!
   * This function gives the Reuss stiffness
   * \tparam real: underlying type
   * \tparam StressType: type of the elastic constants related to the phases
   * \tparam N: number of phases
   * \tparam d: dimension (2 or 3)
   * \return an object of type st2tost2<d,StressType>
   * \param [in] tab_f: std::array<real,N> of volumic fractions of phases
   * \param [in] tab_C: std::array<st2tost2<d,StressType>,N> of stiffness
   * tensors of phases
   */
  template <unsigned short int d,
            unsigned int N,
            tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>()) TFEL_HOST_DEVICE
      const tfel::math::st2tost2<d, StressType> computeReussStiffness(
          const std::array<types::real<StressType>, N>&,
          const std::array<tfel::math::st2tost2<d, StressType>, N>&);

  /*!
   * This function gives the Hashin-Shtrikman bounds for a d-dimensional
   * isotropic composite composed of N isotropic phases. The formulas can be
   * found in Torquato (2002).
   * \tparam real: underlying type
   * \tparam StressType: type of the elastic constants related to the phases
   * \tparam N: number of phases
   * \tparam d: dimension (2 or 3)
   * \return an object of type std::pair<KGModuli<StressType>,
   * KGModuli<StressType>>. First element of the pair is the lower
   * bound and second element is the upper bound. Each bound is KGModuli with bulk
   * and shear moduli.
   * \param [in] tab_f: std::array<real,N> of volumic fractions of phases
   * \param [in] tab_K: std::array<StressType,N> of bulk moduli of phases
   * \param [in] tab_mu: std::array<StressType,N> of shear moduli of phases
   */
  template <unsigned short int d,
            unsigned int N,
            tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>())
      TFEL_HOST_DEVICE const std::pair<KGModuli<StressType>,KGModuli<StressType>> computeIsotropicHashinShtrikmanBounds(const std::
                                                                     array<
                                                                         types::real<
                                                                             StressType>,
                                                                         N>&,
                                                                 const std::array<
                                                                     StressType,
                                                                     N>&,
                                                                 const std::array<
                                                                     StressType,
                                                                     N>&);

}  // end of namespace tfel::material::homogenization::elasticity

#include "TFEL/Material/LinearHomogenizationBounds.ixx"

#endif /* LIB_TFEL_MATERIAL_LINEARHOMOGENIZATIONBOUNDS_HXX */
