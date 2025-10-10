/*!
 * \file   include/MFront/GenericBehaviour/BehaviourData.hxx
 * \brief
 * \author Thomas Helfer
 * \date   02/07/2018
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_GENERICBEHAVIOUR_BEHAVIOURDATA_HXX
#define LIB_MFRONT_GENERICBEHAVIOUR_BEHAVIOURDATA_HXX

#include "TFEL/Material/ModellingHypothesis.hxx"
#include "MFront/GenericBehaviour/Types.hxx"
#include "MFront/GenericBehaviour/BehaviourData.h"

namespace mfront {

  namespace gb {

    //! a simple alias
    using BehaviourData = ::mfront_gb_BehaviourData;

    /*!
     * \brief structure returning the number of elastic properties
     * required to build the stiffness tensor
     * \tparam H: modelling hypothesis
     * \tparam b: true for isotropic elastic behaviours, false for
     * orthotropic elasticbehaviours
     */
    template <tfel::material::ModellingHypothesis::Hypothesis, const bool>
    struct ElasticMaterialPropertiesNumber;

    /*!
     * \brief structure returning the number of material properties
     * required to build the thermal expansion coefficient tensor
     * \tparam b: true for isotropic elastic behaviours, false for
     * orthotropic elasticbehaviours
     */
    template <const bool>
    struct ThermalExpansionCoefficientPropertiesNumber;

    template <>
    struct ThermalExpansionCoefficientPropertiesNumber<true> {
      static constexpr unsigned short value = 1u;
    };  // end of struct ThermalExpansionCoefficientPropertiesNumber<true>

    template <>
    struct ThermalExpansionCoefficientPropertiesNumber<false> {
      static constexpr unsigned short value = 3u;
    };  // end of struct ThermalExpansionCoefficientPropertiesNumber<false>

    template <unsigned short N>
    struct OrthotropicElasticMaterialPropertiesNumber;

    template <>
    struct OrthotropicElasticMaterialPropertiesNumber<1u> {
      static constexpr unsigned short value = 6u;
    };  // end of OrthotropicElasticMaterialPropertiesNumber<1u>

    template <>
    struct OrthotropicElasticMaterialPropertiesNumber<2u> {
      static constexpr unsigned short value = 7u;
    };  // end of struct OrthotropicElasticMaterialPropertiesNumber<2u>

    template <>
    struct OrthotropicElasticMaterialPropertiesNumber<3u> {
      static constexpr unsigned short value = 9u;
    };  // end of OrthotropicElasticMaterialPropertiesNumber<3u>

    //! \brief partial specialisation for elastic isotropic behaviour
    template <tfel::material::ModellingHypothesis::Hypothesis H>
    struct ElasticMaterialPropertiesNumber<H, true> {
      static constexpr unsigned short value = 2u;
    };  // end of ElasticMaterialPropertiesNumber<H,true>

    //! \brief partial specialisation for elastic isotropic behaviour
    template <tfel::material::ModellingHypothesis::Hypothesis H>
    struct ElasticMaterialPropertiesNumber<H, false>
        : OrthotropicElasticMaterialPropertiesNumber<
              tfel::material::ModellingHypothesisToSpaceDimension<H>::value> {
    };  // end of ElasticMaterialPropertiesNumber<H,false>

    template <tfel::material::ModellingHypothesis::Hypothesis H,
              const bool,
              const bool,
              const bool,
              const bool>
    struct ThermoElasticMaterialPropertiesNumber;

    template <tfel::material::ModellingHypothesis::Hypothesis H,
              const bool es,
              const bool bs>
    struct ThermoElasticMaterialPropertiesNumber<H, false, es, false, bs> {
      static constexpr unsigned short value = 0u;
    };

    template <tfel::material::ModellingHypothesis::Hypothesis H,
              const bool es,
              const bool bs>
    struct ThermoElasticMaterialPropertiesNumber<H, true, es, false, bs> {
      static constexpr unsigned short value =
          ElasticMaterialPropertiesNumber<H, es>::value;
    };

    template <tfel::material::ModellingHypothesis::Hypothesis H,
              const bool es,
              const bool bs>
    struct ThermoElasticMaterialPropertiesNumber<H, false, es, true, bs> {
      static constexpr unsigned short value =
          ThermalExpansionCoefficientPropertiesNumber<bs>::value;
    };

    template <tfel::material::ModellingHypothesis::Hypothesis H,
              const bool es,
              const bool bs>
    struct ThermoElasticMaterialPropertiesNumber<H, true, es, true, bs> {
      static constexpr unsigned short value =
          ElasticMaterialPropertiesNumber<H, es>::value +
          ThermalExpansionCoefficientPropertiesNumber<bs>::value;
    };

  }  // end of namespace gb

}  // end of namespace mfront

#endif /* LIB_MFRONT_GENERICBEHAVIOUR_BEHAVIOURDATA_HXX */
