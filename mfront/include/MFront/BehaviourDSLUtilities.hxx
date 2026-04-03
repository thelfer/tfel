/*!
 * \file   MFront/BehaviourDSLUtilities.hxx
 * \brief  This header declares a set of functions
 * useful to DSL dedicated to behaviours
 * \author Thomas Helfer
 * \date   24/04/2025
 */

#ifndef LIB_MFRONT_BEHAVIOURDSLUTILITIES_HXX
#define LIB_MFRONT_BEHAVIOURDSLUTILITIES_HXX

#include "TFEL/Utilities/Data.hxx"
#include "MFront/MFrontConfig.hxx"
#include "MFront/BehaviourDescription.hxx"

namespace mfront {

  // forward declarations
  struct AbstractBehaviourDSL;

  /*!
   * \brief extract a material property usable in a behaviour from a value. If
   * the material property is handled through an external mfront file, the
   * generation of the associated sources is added to the compilation process
   * handled by the DSL.
   * \param[in,out] dsl: behaviour dsl
   * \param[in] n: material property name
   * \param[in] d: data
   */
  MFRONT_VISIBILITY_EXPORT BehaviourDescription::MaterialProperty
  getBehaviourDescriptionMaterialProperty(AbstractBehaviourDSL &,
                                          const std::string_view,
                                          const tfel::utilities::Data &);
  /*!
   * \brief extract the thermal expansion coefficients from the given options,
   * if they are defined.
   * \param[in] dsl: behaviour dsl
   * \param[in] bd: behaviour description
   * \param[in] d: options
   * \param[in] allow_reference_temperature: boolean stating if the
   * `reference_temperature` option is allowed.
   */
  MFRONT_VISIBILITY_EXPORT
  void addThermalExpansionCoefficientsIfDefined(
      AbstractBehaviourDSL &,
      BehaviourDescription &,
      const tfel::utilities::DataMap &,
      const bool = true);

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURDSLUTILITIES_HXX */
