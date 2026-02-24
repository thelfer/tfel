/*!
 * \file   mfront/include/MFront/ModelDSLCommon.hxx
 * \brief  This file declares the ModelDSLCommon class
 * \author Thomas Helfer
 * \date   10 Nov 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_MFRONTMODELDSLCOMMON_HXX
#define LIB_MFRONT_MFRONTMODELDSLCOMMON_HXX

#include <set>
#include <map>
#include <vector>
#include <string>
#include "MFront/MFrontConfig.hxx"
#include "MFront/DSLBase.hxx"
#include "MFront/AbstractDSL.hxx"
#include "MFront/ModelDescription.hxx"

namespace mfront {

  // forward declaration
  struct AbstractModelInterface;

  /*!
   * \return if the given name is valid
   * \param[in] n: model name
   */
  MFRONT_VISIBILITY_EXPORT bool isValidModelName(const std::string&);

  /*!
   * \brief base class for domain specific languages for models
   */
  struct MFRONT_VISIBILITY_EXPORT ModelDSLCommon : public DSLBase {
    /*!
     * \brief constructor
     *\param[in] opts: options passed to the DSL
     */
    ModelDSLCommon(const DSLOptions&);
    //
    DSLTarget getTargetType() const override final;
    const MaterialKnowledgeDescription& getMaterialKnowledgeDescription()
        const override;
    std::string getMaterialKnowledgeIdentifier() const override;
    std::string getMaterialName() const override;
    std::string getOverridableVariableNameByExternalName(
        const std::string&) const override;
    void overrideByAParameter(const std::string&, const double) override;
    std::map<std::string, double> getOverridenParameters() const override;
    void endsInputFileProcessing() override;
    void makeConsistencyChecks() const override;
    void generateOutputFiles() override;
    void setInterfaces(const std::set<std::string>&) override;
    //! \brief destructor
    ~ModelDSLCommon() override;
    void doPedanticChecks() const;

   protected:
    //
    void addExternalMFrontFile(const std::string&,
                               const std::vector<std::string>&,
                               const tfel::utilities::DataMap&) override;
    DSLOptions buildDSLOptions() const override;
    void disableQuantitiesUsageIfNotAlreadySet() override;
    bool useQt() const override;
    void reserveName(const std::string&) override;
    bool isNameReserved(const std::string&) const override;
    std::string getClassName() const override;
    void addMaterialLaw(const std::string&) override;
    void appendToIncludes(const std::string&) override;
    void appendToMembers(const std::string&) override;
    void appendToPrivateCode(const std::string&) override;
    void appendToSources(const std::string&) override;
    void addStaticVariableDescription(
        const StaticVariableDescription&) override;
    std::map<std::string, int> getIntegerConstants() const override;
    int getIntegerConstant(const std::string&) const override;
    void treatUnknownKeyword() override;
    void setUnitSystem(const std::string_view) override;
    void setMaterial(const std::string&) override;
    void setMaterialKnowledgeIdentifier(const std::string&) override;
    /*!
     * \brief get all symbols required to interpret the `@Function` code block.
     * \param[out] symbols: symbols
     */
    virtual std::map<std::string, std::string> getSymbols();
    //! treat the `@UseQt` keyword
    virtual void treatUseQt();
    //! treat the `@Library` keyword
    virtual void treatLibrary();
    //! treat the `@Model` keyword
    virtual void treatModel();
    //! treat the `@Domain` keyword
    virtual void treatDomain();
    //! treat the `@Domains` keyword
    virtual void treatDomains();
    //! treat the `@Bounds` keyword
    virtual void treatBounds();
    //! treat the `@PhysicalBounds` keyword
    virtual void treatPhysicalBounds();
    //! \brief treat the `@ConstantMaterialProperty` keyword
    virtual void treatConstantMaterialProperty();

    virtual void treatConstantMaterialPropertyMethod();
    //! \brief treat the `@Integrator` keyword
    virtual void treatIntegrator();
    //! \brief treat the `@Function` keyword
    virtual void treatFunction();
    /*!
     * \brief read the body of a function and register it
     * \param[in] fn: function name
     */
    virtual void readFunction(const std::string&);
    //! \brief treat the `@Output` keyword or the `@StateVariable` keyword
    virtual void treatOutput();

    virtual void treatOutputMethod();
    /*!
     * \brief treat the `@Input` keyword or the `@ExternalStateVariable` keyword
     */
    virtual void treatInput();

    virtual void treatInputMethod();
    //! \brief treat the `@Parameter` keyword
    virtual void treatParameter();

    virtual void treatLocalParameter();

    virtual void treatParameterMethod();

    virtual void readDefaultValue();

    virtual bool isOutputVariable(const std::string&) const;

    virtual bool isInputVariable(const std::string&) const;

    //! \brief description of the model
    ModelDescription md;
    //! \brief overriding parameters
    std::map<std::string, double> overriding_parameters;
    //! \brief list of registred interfaces
    std::map<std::string, std::shared_ptr<AbstractModelInterface>> interfaces;
    //! \brief current variable. This variable is set before calling the
    //! `treatVariableMethod` method
    std::string currentVar;

  };  // end of class ModelDSLCommon

}  // end of namespace mfront

#endif /* LIB_MFRONT_MFRONTMODELDSLCOMMON_HXX */
