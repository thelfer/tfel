/*!
 * \file   mfront/include/MFront/BehaviourBrick/StressPotentialBaseBase.hxx
 * \brief
 * \author Thomas Helfer
 * \date   27/02/2022
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 * <!-- Local IspellDict: english -->
 */

#ifndef LIB__MFRONT_BEHAVIOURBRICK_STRESSPOTENTIALBASE_HXX
#define LIB__MFRONT_BEHAVIOURBRICK_STRESSPOTENTIALBASE_HXX

#include "MFront/BehaviourBrick/StressPotential.hxx"

namespace mfront::bbrick {

  /*!
   * \brief a base class gathering various helper methods for stress potentials.
   *
   * This class also handles treatment of the thermal expansion.
   */
  struct StressPotentialBase : StressPotential {
    //
    void initialize(BehaviourDescription&,
                    AbstractBehaviourDSL&,
                    const DataMap&) override;
    void endTreatment(BehaviourDescription&,
                      const AbstractBehaviourDSL&) const override;
    //! \brief destructor
    ~StressPotentialBase() override;

   protected:
    //! \brief return general options.
    static std::vector<OptionDescription> getGeneralOptions();
    /*!
     * \return the option declaring the reference temperature for the thermal
     * expansion
     */
    static std::vector<OptionDescription>
    getThermalExpansionReferenceTemperatureOption();
    /*!
     * \return the option describing the mean thermal thermal expansion
     * coefficient of an isotropic material
     */
    static std::vector<OptionDescription> getIsotropicThermalExpansionOptions();
    /*!
     * \return the option describing the mean thermal thermal expansion
     * coefficient of an orthotropic material
     */
    static std::vector<OptionDescription>
    getOrthotropicThermalExpansionOptions();
    /*!
     * \brief declare a parameter associated with the reference temperature of
     * the initial gemeotry.
     * \see `handleThermalExpansionOptions`.
     * \param[in] bd: behaviour description
     * \param[in] d: brick options
`     */
    static void addInitialGeometryReferenceTemperatureParameter(
        BehaviourDescription&, const DataMap&);
    /*!
     * \brief declare a parameter associated with the reference temperature of
     * the thermal expansion
     * \see `handleThermalExpansionOptions`.
     * \param[in] bd: behaviour description
     * \param[in] d: brick options
     */
    static void addThermalExpansionReferenceTemperatureParameter(
        BehaviourDescription&, const DataMap&);
    /*!
     * \brief handle options related to the thermal expansion.
     * \note this function calls
     * `addInitialGeometryReferenceTemperatureParameter` and
     * `addThermalExpansionReferenceTemperatureParameter`  internally.
     * \param[in] bd: behaviour description
     * \param[in] dsl: abstract behaviour dsl
     * \param[in] d: brick options
     */
    static void handleThermalExpansionOptions(BehaviourDescription&,
                                              AbstractBehaviourDSL&,
                                              const DataMap&);
    /*!
     * \brief this function declares the relative value for the equivalent
     * stress lower bound definition parameter
     * \param[in] bd: behaviour description
     * \param[in] d: brick options
     */
    static void
    declareRelativeValueForTheEquivalentStressLowerBoundDefinitionParameter(
        BehaviourDescription&, const DataMap&);
    /*!
     * \return the name of a parameter or a material property with the given
     * external name if this variable is defined for all distinct modelling
     * hypotheses. An empty string is returned if no parameter or a
     * material property matches.
     *
     * \note an exception if thrown if this variable is not found in all
     * distinct modelling hypotheses or if its type does not match.
     *
     * \param[in] bd: behaviour description
     * \param[in] e: external name
     * \param[in] t: expected type
     */
    static std::string findIfParameterOrMaterialPropertyIsUniformelyDefined(
        const BehaviourDescription&, const std::string&, const std::string&);
    /*!
     * \return the material property described by the given option
     * \param[in] dsl: abstract behaviour DSL
     * \param[in] d: options
     * \param[in] n: name of the material property
     */
    static BehaviourDescription::MaterialProperty getMaterialProperty(
        AbstractBehaviourDSL&, const DataMap&, const std::string&);
    /*!
     *
     */
    static void declareElasticStrainIfRequired(BehaviourDescription&);

  };  // end of struct StressPotentialBase

}  // end of namespace mfront::bbrick

#endif /* LIB__MFRONT_BEHAVIOURBRICK_STRESSPOTENTIALBASE_HXX */
