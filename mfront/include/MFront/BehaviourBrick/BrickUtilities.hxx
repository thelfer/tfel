/*!
 * \file   BrickUtilities.hxx
 * \brief
 * \author Thomas Helfer
 * \date   20/03/2018
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_BEHAVIOURBRICK_BRICKUTILITIES_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_BRICKUTILITIES_HXX

#include <array>
#include <string>
#include <memory>
#include <vector>
#include <functional>
#include "TFEL/Utilities/Data.hxx"
#include "TFEL/Glossary/GlossaryEntry.hxx"
#include "MFront/MFrontConfig.hxx"
#include "MFront/AbstractBehaviourDSL.hxx"
#include "MFront/BehaviourDescription.hxx"

namespace mfront {

  namespace bbrick {

    // forward declaration
    struct IsotropicHardeningRule;

    /*!
     * \return a modifier suitable for evaluating a material property at
     * \(t+\theta\,dt\).
     * \param[in] bd: behaviour description
     */
    MFRONT_VISIBILITY_EXPORT std::function<
        std::string(const BehaviourDescription::MaterialPropertyInput &)>
    getMiddleOfTimeStepModifier(const BehaviourDescription &);
    /*!
     * \brief check that the options names are in a given set
     * of keys.
     * \param[in] d: options
     * \param[in] k: supported keys
     * \param[in] b: calling brick name
     */
    MFRONT_VISIBILITY_EXPORT void checkOptionsNames(
        const std::map<std::string, tfel::utilities::Data> &,
        const std::vector<std::string> &,
        const std::string &);
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
                                            const std::string &,
                                            const tfel::utilities::Data &);
    /*!
     * \brief declare a parameter or a local variable used to store the
     * evaluation of the material property.
     * \param[out] bd: behaviour description
     * \param[in,out] mp: material property
     * \param[in] n: variable name
     */
    MFRONT_VISIBILITY_EXPORT void declareParameterOrLocalVariable(
        BehaviourDescription &,
        BehaviourDescription::MaterialProperty &,
        const std::string &);
    /*!
     * \brief declare a parameter or a local variable used to store the
     * evaluation of the material property.
     * \param[out] bd: behaviour description
     * \param[in,out] mp: material property
     * \param[in] n: variable name
     * \param[in] en: external name
     */
    MFRONT_VISIBILITY_EXPORT void declareParameterOrLocalVariable(
        BehaviourDescription &,
        BehaviourDescription::MaterialProperty &,
        const std::string &,
        const std::string &);
    /*!
     * \return the code initializing the variable containing the material
     * property value at \(t+\theta\,dt\).
     * \note If the material property is constant, an empty string is returned,
     * as this material property is assumed to be associated with a parameter.
     * \param[in] dsl: abstract behaviour dsl.
     * \param[in] bd: behaviour description
     * \param[in] n: name of the variable storing the material property value.
     * \param[in] mp: material property description.
     */
    MFRONT_VISIBILITY_EXPORT std::string
    generateMaterialPropertyInitializationCode(
        const AbstractBehaviourDSL &,
        const BehaviourDescription &bd,
        const std::string &,
        const BehaviourDescription::MaterialProperty &);
    /*!
     * \brief declare a parameter or a local variable used to store the
     * evaluation of the material property.
     * \param[out] bd: behaviour description
     * \param[in,out] mp: material property
     * \param[in] n: variable name
     * \param[in] g: glossary entry
     */
    MFRONT_VISIBILITY_EXPORT void declareParameterOrLocalVariable(
        BehaviourDescription &,
        BehaviourDescription::MaterialProperty &,
        const std::string &,
        const tfel::glossary::GlossaryEntry &);
    /*!
    *\brief add a new material property
    *\param[in] bd: behaviour description
    *\param[in] t: type of the material property
    *\param[in] n: name of the variable
    *\param[in] g: glossary name
    *\param[in] s: array size
    */
    MFRONT_VISIBILITY_EXPORT void addMaterialPropertyIfNotDefined(
        BehaviourDescription &,
        const std::string &,
        const std::string &,
        const tfel::glossary::GlossaryEntry &,
        const unsigned short = 1u);
    /*!
     * \brief add a new material property
     * \param[in] bd: behaviour description
     * \param[in] t: type of the material property
     * \param[in] n: name of the variable
     * \param[in] e: entry name
     * \param[in] s: array size
     */
    MFRONT_VISIBILITY_EXPORT void addMaterialPropertyIfNotDefined(
        BehaviourDescription &,
        const std::string &,
        const std::string &,
        const std::string &,
        const unsigned short = 1u);
    /*!
     * \brief add a new state variable
     * \param[in] bd: behaviour description
     * \param[in] t: type of the material property
     * \param[in] n: name of the variable
     * \param[in] s: array size
     */
    MFRONT_VISIBILITY_EXPORT void addStateVariable(BehaviourDescription &,
                                                   const std::string &,
                                                   const std::string &,
                                                   const unsigned short = 1u);
    /*!
     * \brief add a new state variable
     * \param[in] bd: behaviour description
     * \param[in] t: type of the material property
     * \param[in] n: name of the variable
     * \param[in] g: glossary entry
     * \param[in] s: array size
     */
    MFRONT_VISIBILITY_EXPORT void addStateVariable(
        BehaviourDescription &,
        const std::string &,
        const std::string &,
        const tfel::glossary::GlossaryEntry &,
        const unsigned short = 1u);
    /*!
     * \brief add a new state variable
     * \param[in] bd: behaviour description
     * \param[in] t: type of the material property
     * \param[in] n: name of the variable
     * \param[in] e: entry name
     * \param[in] s: array size
     */
    MFRONT_VISIBILITY_EXPORT void addStateVariable(BehaviourDescription &,
                                                   const std::string &,
                                                   const std::string &,
                                                   const std::string &,
                                                   const unsigned short = 1u);
    /*!
     * \brief add a new external state variable
     * \param[in] bd: behaviour description
     * \param[in] t: type of the material property
     * \param[in] n: name of the variable
     * \param[in] s: array size
     */
    MFRONT_VISIBILITY_EXPORT void addExternalStateVariable(
        BehaviourDescription &,
        const std::string &,
        const std::string &,
        const unsigned short = 1u);
    /*!
     * \brief add a new external state variable
     * \param[in] bd: behaviour description
     * \param[in] t: type of the material property
     * \param[in] n: name of the variable
     * \param[in] g: glossary entry
     * \param[in] s: array size
     */
    MFRONT_VISIBILITY_EXPORT void addExternalStateVariable(
        BehaviourDescription &,
        const std::string &,
        const std::string &,
        const tfel::glossary::GlossaryEntry &,
        const unsigned short = 1u);
    /*!
     * \brief add a new external state variable
     * \param[in] bd: behaviour description
     * \param[in] t: type of the material property
     * \param[in] n: name of the variable
     * \param[in] e: entry name
     * \param[in] s: array size
     */
    MFRONT_VISIBILITY_EXPORT void addExternalStateVariable(
        BehaviourDescription &,
        const std::string &,
        const std::string &,
        const std::string &,
        const unsigned short = 1u);
    /*!
     * \brief add a new local variable
     * \param[in] bd: behaviour description
     * \param[in] t: type of the material property
     * \param[in] n: name of the variable
     * \param[in] s: array size
     */
    MFRONT_VISIBILITY_EXPORT void addLocalVariable(BehaviourDescription &,
                                                   const std::string &,
                                                   const std::string &,
                                                   const unsigned short = 1u);
    /*!
     * \brief add a new material property
     * \param[in] bd: behaviour description
     * \param[in] n: name of the variable
     * \param[in] e: entry name
     * \param[in] p: parameter default value
     */
    MFRONT_VISIBILITY_EXPORT void addParameter(
        BehaviourDescription &,
        const std::string &,
        const tfel::glossary::GlossaryEntry &e,
        const double);
    /*!
     * \brief add a new material property
     * \param[in] bd: behaviour description
     * \param[in] n: name of the variable
     * \param[in] e: entry name
     * \param[in] s: array size
     * \param[in] p: parameter default value
     */
    MFRONT_VISIBILITY_EXPORT void addParameter(
        BehaviourDescription &,
        const std::string &,
        const tfel::glossary::GlossaryEntry &,
        const unsigned short,
        const double);
    /*!
     * \brief add a new material property
     * \param[in] bd: behaviour description
     * \param[in] n: name of the variable
     * \param[in] e: entry name
     * \param[in] s: array size
     * \param[in] p: parameter default values
     */
    MFRONT_VISIBILITY_EXPORT void addParameter(
        BehaviourDescription &,
        const std::string &,
        const tfel::glossary::GlossaryEntry &,
        const unsigned short,
        const std::vector<double> &);
    /*!
     * \brief add a new material property
     * \param[in] bd: behaviour description
     * \param[in] n: name of the variable
     * \param[in] e: entry name
     * \param[in] p: parameter default value
     */
    MFRONT_VISIBILITY_EXPORT void addParameter(BehaviourDescription &,
                                               const std::string &,
                                               const std::string &,
                                               const double);
    /*!
     * \brief add a new material property
     * \param[in] bd: behaviour description
     * \param[in] n: name of the variable
     * \param[in] e: entry name
     * \param[in] s: array size
     * \param[in] p: parameter default value
     */
    MFRONT_VISIBILITY_EXPORT void addParameter(BehaviourDescription &,
                                               const std::string &,
                                               const std::string &,
                                               const unsigned short,
                                               const double);
    /*!
     * \brief add a new material property
     * \param[in] bd: behaviour description
     * \param[in] n: name of the variable
     * \param[in] e: entry name
     * \param[in] s: array size
     * \param[in] p: parameter default values
     */
    MFRONT_VISIBILITY_EXPORT void addParameter(BehaviourDescription &,
                                               const std::string &,
                                               const std::string &,
                                               const unsigned short,
                                               const std::vector<double> &);

    MFRONT_VISIBILITY_EXPORT std::string computeElasticLimitInitialValue(
        const std::vector<std::shared_ptr<IsotropicHardeningRule>> &,
        const std::string &);

    MFRONT_VISIBILITY_EXPORT std::string computeElasticLimitAndDerivative(
        const std::vector<std::shared_ptr<IsotropicHardeningRule>> &,
        const std::string &);

    MFRONT_VISIBILITY_EXPORT std::string computeElasticLimit(
        const std::vector<std::shared_ptr<IsotropicHardeningRule>> &,
        const std::string &);

  }  // end of namespace bbrick

}  // end of namespace mfront

#include "MFront/BehaviourBrick/BrickUtilities.ixx"

#endif /* LIB_MFRONT_BEHAVIOURBRICK_BRICKUTILITIES_HXX */
