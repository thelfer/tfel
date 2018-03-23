/*!
 * \file   BrickUtilities.hxx
 * \brief
 * \author Thomas Helfer
 * \date   20/03/2018
 */

#ifndef LIB_MFRONT_BEHAVIOURBRICK_BRICKUTILITIES_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_BRICKUTILITIES_HXX

#include <string>
#include "TFEL/Utilities/Data.hxx"
#include "MFront/MFrontConfig.hxx"
#include "MFront/AbstractBehaviourDSL.hxx"
#include "MFront/BehaviourDescription.hxx"

namespace tfel {

  namespace glossary {

    // forward declaration
    struct GlossaryEntry;

  }  // end of namespace glossary

}  // end of namespace tfel

namespace mfront {

  namespace bbrick {

    /*!
     * \brief check that the options names are in a given set
     * of keys.
     * \param[in] d: options
     * \param[in] k: supported keys
     * \param[in] b: calling brick name
     */
    MFRONT_VISIBILITY_EXPORT void checkOptionsNames(
        const std::map<std::string, tfel::utilities::Data>&,
        const std::vector<std::string>&,
        const std::string&);
    /*!
     * \brief extract a material property usable in a behaviour from a value
     * \parambricksbricksbricks[in]
     * dsl:bricksbricksbricksbricksbricksbricksbricks behaviour dsl
     * \param[in] n: material property name
     * \param[in] d: data
     */
    MFRONT_VISIBILITY_EXPORT BehaviourDescription::MaterialProperty
    getBehaviourDescriptionMaterialProperty(AbstractBehaviourDSL&,
                                            const std::string&,
                                            const tfel::utilities::Data&);
    /*!
     * \brief add a new material property
     * \param[in] bd: behaviour description
     * \param[in] t: type of the material property
     * \param[in] n: name of the variable
     * \param[in] g: glossary name
     * \param[in] s: array size
     */
    MFRONT_VISIBILITY_EXPORT void addMaterialPropertyIfNotDefined(
        BehaviourDescription&,
        const std::string&,
        const std::string&,
        const tfel::glossary::GlossaryEntry&,
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
        BehaviourDescription&,
        const std::string&,
        const std::string&,
        const std::string&,
        const unsigned short = 1u);
    /*!
     * \brief add a new external state variable
     * \param[in] bd: behaviour description
     * \param[in] t: type of the material property
     * \param[in] n: name of the variable
     * \param[in] s: array size
     */
    MFRONT_VISIBILITY_EXPORT void addExternalStateVariable(
        BehaviourDescription&,
        const std::string&,
        const std::string&,
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
        BehaviourDescription&,
        const std::string&,
        const std::string&,
        const tfel::glossary::GlossaryEntry&,
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
        BehaviourDescription&,
        const std::string&,
        const std::string&,
        const std::string&,
        const unsigned short = 1u);
    /*!
     * \brief add a new local variable
     * \param[in] bd: behaviour description
     * \param[in] t: type of the material property
     * \param[in] n: name of the variable
     * \param[in] s: array size
     */
    MFRONT_VISIBILITY_EXPORT void addLocalVariable(BehaviourDescription&,
                                                   const std::string&,
                                                   const std::string&,
                                                   const unsigned short = 1u);
    /*!
     * \brief add a new material property
     * \param[in] bd: behaviour description
     * \param[in] n: name of the variable
     * \param[in] e: entry name
     * \param[in] p: parameter default value
     */
    MFRONT_VISIBILITY_EXPORT void addParameter(
        BehaviourDescription&,
        const std::string&,
        const tfel::glossary::GlossaryEntry& e,
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
        BehaviourDescription&,
        const std::string&,
        const tfel::glossary::GlossaryEntry&,
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
        BehaviourDescription&,
        const std::string&,
        const tfel::glossary::GlossaryEntry&,
        const unsigned short,
        const std::vector<double>&);
    /*!
     * \brief add a new material property
     * \param[in] bd: behaviour description
     * \param[in] n: name of the variable
     * \param[in] e: entry name
     * \param[in] p: parameter default value
     */
    MFRONT_VISIBILITY_EXPORT void addParameter(BehaviourDescription&,
                                               const std::string&,
                                               const std::string&,
                                               const double);
    /*!
     * \brief add a new material property
     * \param[in] bd: behaviour description
     * \param[in] n: name of the variable
     * \param[in] e: entry name
     * \param[in] s: array size
     * \param[in] p: parameter default value
     */
    MFRONT_VISIBILITY_EXPORT void addParameter(BehaviourDescription&,
                                               const std::string&,
                                               const std::string&,
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
    MFRONT_VISIBILITY_EXPORT void addParameter(BehaviourDescription&,
                                               const std::string&,
                                               const std::string&,
                                               const unsigned short,
                                               const std::vector<double>&);

  }  // end of namespace bbrick

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURBRICK_BRICKUTILITIES_HXX */
