/*!
 * \file   mfront/include/MFront/AbstractBehaviourDSL.hxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   October 23, 2014
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_ABSTRACTBEHAVIOURDSL_HXX
#define LIB_MFRONT_ABSTRACTBEHAVIOURDSL_HXX

#include <memory>
#include <iosfwd>
#include "MFront/MFrontConfig.hxx"
#include "TFEL/Material/ModellingHypothesis.hxx"

#include "MFront/AbstractDSL.hxx"
#include "MFront/VariableDescription.hxx"
#include "MFront/BehaviourDescription.hxx"
#include "MFront/BehaviourDSLDescription.hxx"
#include "MFront/MFrontTemplateGenerationOptions.hxx"

namespace mfront {

  // forward declarations
  struct AbstractBehaviourInterface;
  struct MaterialPropertyDescription;
  struct AbstractBehaviourCodeGenerator;

  //! \brief interface class for all domain specific languages associated with
  //! behaviours.
  struct MFRONT_VISIBILITY_EXPORT AbstractBehaviourDSL
      : public virtual AbstractDSL {
    //! a simple alias
    using MaterialPropertyInput = BehaviourDescription::MaterialPropertyInput;
    //! constructor
    AbstractBehaviourDSL() = default;
    //! a simple alias
    using ModellingHypothesis = tfel::material::ModellingHypothesis;
    //! a simple alias
    using Hypothesis = ModellingHypothesis::Hypothesis;
    //! \return the target of the dsl
    DSLTarget getTargetType() const override final;
    //! \return the declared behaviour interfaces
    virtual std::map<std::string, std::shared_ptr<AbstractBehaviourInterface>>
    getBehaviourInterfaces() const = 0;
    //! \return a description of the DSL
    virtual BehaviourDSLDescription getBehaviourDSLDescription() const = 0;
    //! \return the behaviour description
    virtual const BehaviourDescription& getBehaviourDescription() const = 0;
    //! \return a suitable code generator
    virtual std::unique_ptr<AbstractBehaviourCodeGenerator> getCodeGenerator()
        const = 0;
    /*!
     * \brief add the given material properties
     * \param[in] mps: material properties
     */
    virtual void addMaterialProperties(const VariableDescriptionContainer&) = 0;
    /*!
     * \brief add the given parameters
     * \param[in] params: parameters
     */
    virtual void addParameters(const VariableDescriptionContainer&) = 0;
    /*!
     * \brief add the given state variables
     * \param[in] ivs: state variables
     */
    virtual void addStateVariables(const VariableDescriptionContainer&) = 0;
    /*!
     * \brief add the given auxiliary state variables
     * \param[in] ivs: auxiliary state variables
     */
    virtual void addAuxiliaryStateVariables(
        const VariableDescriptionContainer&) = 0;
    /*!
     * \brief add the given external state variables
     * \param[in] evs: external state variables
     */
    virtual void addExternalStateVariables(
        const VariableDescriptionContainer&) = 0;
    /*!
     * \brief add the given local state variables
     * \param[in] lvs: local state variables
     */
    virtual void addLocalVariables(const VariableDescriptionContainer&) = 0;
    /*!
     * \brief add the given integration variables
     * \param[in] lvs: integration variables
     */
    virtual void addIntegrationVariables(
        const VariableDescriptionContainer&) = 0;
    /*!
     * \return a template for the given code block
     * \param[in] c: code block name
     * \param[in] o: generation options
     */
    virtual std::string getCodeBlockTemplate(
        const std::string&, const MFrontTemplateGenerationOptions&) const = 0;
    /*!
     * \return the list of hypothesis a priori supported by
     * the parser.
     *
     * To enable other hypothesis or restrict the hypotheses
     * supported, the user must use the `@ModellingHypothesis` or
     * `@ModellingHypotheses` keywords.
     */
    virtual std::set<Hypothesis> getDefaultModellingHypotheses() const = 0;
    /*!
     * \brief write the call to a material property
     * \param[out] out: output stream
     * \param[in]  m:   material property description
     * \param[in]  f:   function converting input variable name.
     * The function f can be used to specify how evaluate a variable value.
     * For example, if we want to evaluate the variable name 'V' at
     * the end of the time step, we could make f return V+dV
     */
    virtual void writeMaterialPropertyEvaluation(
        std::ostream&,
        const BehaviourDescription::MaterialProperty&,
        std::function<std::string(const MaterialPropertyInput&)>&) const = 0;
    /*!
     * \brief This function handles a material property treated as a
     * dependency of the current file.
     *
     * This function:
     * - analyse the given file using the MaterialPropertyDSL
     * - register the function name generated by the MFront interface
     * - add the function name to the list of material laws used by
     *   the current file
     * - declared the headers generated by the MFront interface for inclusion
     * - launch mfront in a sub-process to generate the source files
     *   associated with this material property and have them compiled
     *   when mandatory (done by the callMFront method).
     *
     * \param[in] f : file in which the material law is
     * implemented. This must be the full path.
     */
    virtual std::shared_ptr<MaterialPropertyDescription>
    handleMaterialPropertyDescription(const std::string&) = 0;
    /*!
     * \return true if the given modelling hypothesis is handled by
     * the parser
     *
     * Some parsers have restrictions on the modelling hypotheses
     * supported. For example, the isotropic behaviours were not able
     * to handle plane stress hypotheses when this comment was
     * written(but it was planned, so this comment may be outdated
     * now).
     *
     * The fact that this method returns true means that the user
     * *can* provide code to support this modelling hypothesis. For
     * example, to support plane stress in RungeKutta and Implicit
     * parsers, the user must provide some hand-crafted code. He must
     * enable this modelling hypothesis by calling explicitely
     * `@ModellingHypothesis` or `@ModellingHypotheses` keywords.
     */
    virtual bool isModellingHypothesisSupported(const Hypothesis) const = 0;
    //! \brief destructor
    ~AbstractBehaviourDSL() override;
  };

}  // end of namespace mfront

#endif /* LIB_MFRONT_ABSTRACTBEHAVIOURDSL_HXX */
