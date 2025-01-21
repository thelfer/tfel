/*!
 * \file   include/TFEL/Material/LinearHomogenizationBounds.hxx
 * \author Antoine Martin
 * \date   24 October 2024
 * \brief  This file declares some homogenization schemes based on solution of
 * Eshelby problem. \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All
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
   * \param [in] tab_C: std::array<st2tost2<d,StressType>,N> of stiffness tensors of phases
   */
  template <unsigned short int d, unsigned int N, typename real, typename StressType>
  TFEL_HOST_DEVICE const tfel::math::st2tost2<d, StressType>
  computeVoigtStiffness(const std::array<real,N>&,
                      const std::array<tfel::math::st2tost2<d, StressType>,N>&);


   /*!
   * This function gives the Reuss stiffness
   * \tparam real: underlying type
   * \tparam StressType: type of the elastic constants related to the phases
   * \tparam N: number of phases
   * \tparam d: dimension (2 or 3)
   * \return an object of type st2tost2<d,StressType>
   * \param [in] tab_f: std::array<real,N> of volumic fractions of phases
   * \param [in] tab_C: std::array<st2tost2<d,StressType>,N> of stiffness tensors of phases
   */
  template <unsigned short int d, unsigned int N, typename real, typename StressType>
  TFEL_HOST_DEVICE const tfel::math::st2tost2<d, StressType>
  computeReussStiffness(const std::array<real,N>&,
                      const std::array<tfel::math::st2tost2<d, StressType>,N>&);
                      
                      
    /*!
   * This function gives the Hashin-Shtrikman bounds for a d-dimensional isotropic composite
   * composed of N isotropic phases.
   * \tparam real: underlying type
   * \tparam StressType: type of the elastic constants related to the phases
   * \tparam N: number of phases
   * \tparam d: dimension (2 or 3)
   * \return an object of type std::pair<StressType, real>
   * \param [in] tab_f: std::array<real,N> of volumic fractions of phases
   * \param [in] tab_E: std::array<StressType,N> of young moduli of phases
   * \param [in] tab_nu: std::array<real,N> of Poisson ratios of phases
   */
  template <unsigned short int d, unsigned int N, typename real, typename StressType>
  TFEL_HOST_DEVICE const std::pair<StressType, real>
  computeIsotropicHashinShtrikmanBounds(const std::array<real,N>&,
                      const std::array<StressType,N>&,
                      const std::array<real,N>&);
                      
      /*!
   * This function gives the Milton three-point bounds for a 3d
   * two-phase isotropic composite.
   * \tparam real: underlying type
   * \tparam StressType: type of the elastic constants related to the phases
   * \return an object of type std::pair<StressType, real>
   * \param [in] tab_f: std::array<real,2> of volumic fractions of phases
   * \param [in] tab_E: std::array<StressType,2> of young moduli of phases
   * \param [in] tab_nu: std::array<real,2> of Poisson ratios of phases
   */
  template <typename real, typename StressType>
  TFEL_HOST_DEVICE const std::pair<StressType, real>
  computeThreePointMiltonBounds(const std::array<real,2>&,
                      const std::array<StressType,2>&,
                      const std::array<real,2>&);

}  // end of namespace tfel::material::homogenization::elasticity

#include "TFEL/Material/LinearHomogenizationBounds.ixx"

#endif /* LIB_TFEL_MATERIAL_LINEARHOMOGENIZATIONBOUNDS_HXX */
