/*!
 * \file   mfront/include/MFront/BehaviourDSLCommon.hxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   05 mai 2008
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_MFRONTBEHAVIOURDSLCOMMON_HXX
#define LIB_MFRONT_MFRONTBEHAVIOURDSLCOMMON_HXX

#include <set>
#include <map>
#include <vector>
#include <string>
#include <memory>
#include <fstream>
#include <functional>

#include "MFront/MFrontConfig.hxx"
#include "MFront/DSLBase.hxx"
#include "MFront/SupportedTypes.hxx"
#include "MFront/AbstractBehaviourDSL.hxx"
#include "MFront/BehaviourDescription.hxx"

namespace mfront {

  // forward declarations
  struct AbstractBehaviourInterface;
  struct AbstractBehaviourBrick;

  /*!
   * \return if the given name is valid
   * \param[in] n: behaviour name
   */
  MFRONT_VISIBILITY_EXPORT bool isValidBehaviourName(const std::string&);

  /*!
   * \brief this class provides most functionnalities used by mechanical
   * behaviour parsers.
   */
  struct MFRONT_VISIBILITY_EXPORT BehaviourDSLCommon
      : public virtual AbstractBehaviourDSL,
        public DSLBase,
        public SupportedTypes {
    //! \return a validator for the options passed to the DSL
    static tfel::utilities::DataMapValidator getDSLOptionsValidator();
    //! a simple alias
    using Hook = std::function<void()>;
    //! a simple alias
    using OrthotropicAxesConvention = tfel::material::OrthotropicAxesConvention;
    //
    const BehaviourDescription& getBehaviourDescription() const override final;
    std::map<std::string, std::shared_ptr<AbstractBehaviourInterface>>
    getBehaviourInterfaces() const override;
    std::shared_ptr<MaterialPropertyDescription>
    handleMaterialPropertyDescription(const std::string&) override;
    std::string getMaterialKnowledgeIdentifier() const override;
    std::string getMaterialName() const override;
    std::string getOverridableVariableNameByExternalName(
        const std::string&) const override;
    void overrideByAParameter(const std::string&, const double) override;
    std::map<std::string, double> getOverridenParameters() const override;

    void analyseFile(const std::string&,
                     const std::vector<std::string>&,
                     const std::map<std::string, std::string>&) override;
    void importFile(const std::string&,
                    const std::vector<std::string>&,
                    const std::map<std::string, std::string>&) override;
    void analyseString(const std::string&) override;

    void endsInputFileProcessing() override;

    void makeConsistencyChecks() const override;
    /*!
     * \brief method called when a new gradient or a new thermodynamic force is
     * defined. It declares as many pair of gradient and thermodynamic force as
     * possible.
     */
    virtual void declareMainVariables();

    virtual void addHook(const std::string&, const Hook);
    /*!
     * \brief return the list of keywords usable with this parser
     * \param[out] k : the list of keywords registred for this parser
     */
    void getKeywordsList(std::vector<std::string>&) const override;

    void addMaterialProperties(const VariableDescriptionContainer&) override;
    void addParameters(const VariableDescriptionContainer&) override;
    void addStateVariables(const VariableDescriptionContainer&) override;
    void addAuxiliaryStateVariables(
        const VariableDescriptionContainer&) override;
    void addExternalStateVariables(
        const VariableDescriptionContainer&) override;
    void addLocalVariables(const VariableDescriptionContainer&) override;
    void addIntegrationVariables(const VariableDescriptionContainer&) override;

   protected:
    //! \brief a simple alias
    using CallBack = std::function<void()>;
    //! \brief create a variable modifier from a method
    struct TFEL_VISIBILITY_LOCAL StandardVariableModifier final
        : public VariableModifier {
      //! a simple alias
      using FunctionType = std::function<std::string(
          const Hypothesis, const std::string&, const bool)>;
      /*!
       * \brief constructor from a std::function
       * \param[in] h: hypothesis
       * \param[in] f: function object
       */
      StandardVariableModifier(const Hypothesis, const FunctionType);
      /*!
       * \param[in] v : the variable name
       * \param[in] b : true if "this" shall be added
       */
      std::string exe(const std::string&, const bool) override;
      //! destructor
      ~StandardVariableModifier() override;

     private:
      const Hypothesis hypothesis;
      FunctionType fct;
    };
    struct TFEL_VISIBILITY_LOCAL StandardWordAnalyser final
        : public WordAnalyser {
      //! a simple alias
      using FunctionType =
          std::function<void(CodeBlock&, const Hypothesis, const std::string&)>;
      /*!
       * \brief constructor
       * \param[in] h: hypothesis
       * \param[in] f: function object
       */
      StandardWordAnalyser(const Hypothesis, const FunctionType);
      /*!
       * \param[in] cb : code block
       * \param[in] k : the current word
       */
      void exe(CodeBlock&, const std::string&) override;
      //! destructor
      ~StandardWordAnalyser() override;

     private:
      const Hypothesis hypothesis;
      const FunctionType fct;
    };
    /*!
     * \brief structure holding options passed to code blocks
     */
    struct CodeBlockOptions {
      //! a simple alias
      using Mode = BehaviourData::Mode;
      //! a simple alias
      using Position = BehaviourData::Position;
      //! constructor
      CodeBlockOptions();
      CodeBlockOptions(CodeBlockOptions&&) = default;
      CodeBlockOptions(const CodeBlockOptions&) = default;
      CodeBlockOptions& operator=(CodeBlockOptions&&) = default;
      CodeBlockOptions& operator=(const CodeBlockOptions&) = default;
      ~CodeBlockOptions();
      //! \brief position where the code block will be inserted (body by defaut)
      Position p;
      //! \brief insertion mode (create or append by default)
      Mode m;
      //! \brief list of hypothesis
      std::set<Hypothesis> hypotheses;
      //! \brief list of untreated options
      std::vector<tfel::utilities::Token> untreated;
      //! \brief flag stating if potential warnings shall be ignored
      bool safe = false;
    };
    /*!
     * \brief constructor
     * \param[in] opts: options passed to the DSL
     */
    BehaviourDSLCommon(const DSLOptions&);
    //
    void writeMaterialPropertyEvaluation(
        std::ostream&,
        const BehaviourDescription::MaterialProperty&,
        std::function<std::string(const MaterialPropertyInput&)>&)
        const override;
    //
    void addExternalMFrontFile(const std::string&,
                               const std::vector<std::string>&,
                               const tfel::utilities::DataMap&) override;
    const MaterialKnowledgeDescription& getMaterialKnowledgeDescription()
        const override;
    std::vector<DSLOptionDescription> getDSLOptions() const override;
    DSLOptions buildDSLOptions() const override;

    bool useQt() const override;
    void disableQuantitiesUsageIfNotAlreadySet() override;
    std::string getClassName() const override;
    void addMaterialLaw(const std::string&) override;
    void appendToIncludes(const std::string&) override;
    void appendToMembers(const std::string&) override;
    void appendToPrivateCode(const std::string&) override;
    void appendToSources(const std::string&) override;

    std::set<Hypothesis> getModellingHypothesesToBeTreated() const;

    virtual void analyse();

    virtual void treatDisabledCallBack();

    virtual void disableCallBack(const std::string&);
    /*!
     * \brief add a new call-back associated with a keyword
     *
     * \param[in] k: keyword
     * \param[in] c: call-back
     * \param[in] b: if true, allow the given call-back to override an existing
     * call-back, if any.
     */
    virtual void addCallBack(const std::string&,
                             const CallBack,
                             const bool = false);
    /*!
     * \brief get all symbols required to interpret the given code block.
     * \param[out] symbols: symbols
     * \param[in] h: modelling hypothesis
     * \param[in] n: name of the code block
     */
    virtual void getSymbols(std::map<std::string, std::string>&,
                            const Hypothesis,
                            const std::string&);
    /*!
     * \param[out] o : options to be read
     * \param[in]  s : allow specialisation
     */
    virtual void readCodeBlockOptions(CodeBlockOptions&, const bool);
    /*!
     * \brief read the next code block and adds it tho the mechanical
     * behaviour
     * \param[in] n     : name of the method read
     * \param[in] m     : modifier
     * \param[in] b     : add "this->" in front of variables
     * \param[in] s     : allow specialisation
     */
    CodeBlockOptions treatCodeBlock(
        const std::string&,
        std::function<
            std::string(const Hypothesis, const std::string&, const bool)>,
        const bool,
        const bool);
    /*!
     * \brief read the next code block and adds it tho the mechanical
     * behaviour
     * \param[in] o     : options
     * \param[in] n     : name of the method read
     * \param[in] m     : modifier
     * \param[in] b     : add "this->" in front of variables
     */
    void treatCodeBlock(const CodeBlockOptions&,
                        const std::string&,
                        std::function<std::string(
                            const Hypothesis, const std::string&, const bool)>,
                        const bool);
    /*!
     * \brief read the next code block and adds it tho the mechanical
     * behaviour
     * \param[in] n     : name of the method read
     * \param[in] m     : modifier
     * \param[in] a     : word analyser
     * \param[in] b     : add "this->" in front of variables
     * \param[in] s     : allow specialisation
     */
    CodeBlockOptions treatCodeBlock(
        const std::string&,
        std::function<
            std::string(const Hypothesis, const std::string&, const bool)>,
        std::function<void(CodeBlock&, const Hypothesis, const std::string&)>,
        const bool,
        const bool);
    /*!
     * \brief read the next code block and adds it tho the mechanical
     * behaviour
     * \param[in] o     : options
     * \param[in] n     : name of the method read
     * \param[in] m     : modifier
     * \param[in] a     : word analyser
     * \param[in] b     : add "this->" in front of variables
     */
    void treatCodeBlock(
        const CodeBlockOptions&,
        const std::string&,
        std::function<
            std::string(const Hypothesis, const std::string&, const bool)>,
        std::function<void(CodeBlock&, const Hypothesis, const std::string&)>,
        const bool);
    /*!
     * \brief read the next code block and adds it tho the mechanical
     * behaviour
     * \param[in] n1    : name of the first method read
     * \param[in] n2    : name of the second method read
     * \param[in] m1    : modifier
     * \param[in] m2    : modifier
     * \param[in] b     : add "this->" in front of variables
     * \param[in] s     : allow specialisation
     */
    CodeBlockOptions treatCodeBlock(
        const std::string&,
        const std::string&,
        std::function<
            std::string(const Hypothesis, const std::string&, const bool)>,
        std::function<
            std::string(const Hypothesis, const std::string&, const bool)>,
        const bool,
        const bool);
    /*!
     * \brief read the next code block and adds it tho the mechanical
     * behaviour
     * \param[in] n1    : name of the first method read
     * \param[in] n2    : name of the second method read
     * \param[in] m1    : modifier
     * \param[in] m2    : modifier
     * \param[in] b     : add "this->" in front of variables
     */
    void treatCodeBlock(const CodeBlockOptions&,
                        const std::string&,
                        const std::string&,
                        std::function<std::string(
                            const Hypothesis, const std::string&, const bool)>,
                        std::function<std::string(
                            const Hypothesis, const std::string&, const bool)>,
                        const bool);
    /*!
     * \brief read the next code block for the given hypothesis
     * \param[in] h: hypothesis
     * \param[in] n: name of the method read
     * \param[in] m: modifier
     * \param[in] a: word analyser
     * \param[in] b: add "this->" in front of variables
     */
    CodeBlock readNextBlock(
        const Hypothesis,
        const std::string&,
        std::function<
            std::string(const Hypothesis, const std::string&, const bool)>,
        std::function<void(CodeBlock&, const Hypothesis, const std::string&)>,
        const bool);
    /*!
     * \brief read the next code block for the given hypothesis
     * \param[in] h: hypothesis
     * \param[in] n: name of the method read
     * \param[in] m: modifier
     * \param[in] b: add "this->" in front of variables
     */
    CodeBlock readNextBlock(
        const Hypothesis,
        const std::string&,
        std::function<
            std::string(const Hypothesis, const std::string&, const bool)>,
        const bool);
    //
    using DSLBase::readNextBlock;
    //! \brief throw an exception is some options were not recognized
    void treatUnsupportedCodeBlockOptions(const CodeBlockOptions&);
    //
    void addStaticVariableDescription(
        const StaticVariableDescription&) override;
    std::map<std::string, int> getIntegerConstants() const override;
    int getIntegerConstant(const std::string&) const override;
    /*!
     * \brief disable the declaration of new variables
     * \param[in] h : modelling hypothesis
     */
    virtual void disableVariableDeclaration();
    //! \brief method called at the end of the input file processing.
    virtual void completeVariableDeclaration();
    //
    void generateOutputFiles() override;
    //
    std::set<Hypothesis> getDefaultModellingHypotheses() const override;
    bool isModellingHypothesisSupported(const Hypothesis) const override;
    /*!
     * \brief the standard variable modifier
     * \param[in] h : modelling hypothesis
     * \param[in] v : variable name
     * \param[in] b : if true, shall add the "this" qualifier
     */
    virtual std::string standardModifier(const Hypothesis,
                                         const std::string&,
                                         const bool);

    virtual std::string predictionOperatorVariableModifier(const Hypothesis,
                                                           const std::string&,
                                                           const bool);

    virtual std::string tangentOperatorVariableModifier(const Hypothesis,
                                                        const std::string&,
                                                        const bool);
    //! \brief read the description of a behaviour variable
    virtual BehaviourVariableDescription readBehaviourVariableDescription();
    /*!
     * \brief extract a material property from a token. If the token
     * is a string, it is interpred as a mfront file name. Otherwise,
     * the token is converted to a scalar.
     * \param[in] m: calling method
     * \param[in] t: token
     * \return a material property
     */
    BehaviourDescription::MaterialProperty extractMaterialProperty(
        const std::string&, const tfel::utilities::Token&);
    /*!
     * \brief read an an array of material properties. String are
     * interpreted as mfront file name. Other tokens are interpreted
     * as long double.
     * \param[in] m: calling method
     * \return the array of material properties
     */
    virtual std::vector<BehaviourDescription::MaterialProperty>
    readMaterialPropertyOrArrayOfMaterialProperties(const std::string& m);
    /*!
     *
     */
    virtual void readStringList(std::vector<std::string>&);
    /*!
     * \return a list of hypotheses
     * \note by default, the returning set contains UNDEFINEDHYPOTHESIS
     */
    virtual std::set<Hypothesis> readHypothesesList();
    /*!
     * read a list of hypotheses
     * \param[out] h : list of hypotheses
     * \note by default, the returning set contains UNDEFINEDHYPOTHESIS
     */
    virtual void readHypothesesList(std::set<Hypothesis>&);
    /*!
     * \brief append the given modelling hypothesis to the set of hypothesis
     * \param[out] h : list of hypotheses
     * \param[in]  v : hypothesis to be inserted
     */
    void appendToHypothesesList(std::set<Hypothesis>&,
                                const std::string&) const;
    /*!
     * First read a set of Hypothesis. Then read a list variables and
     * assign them to mechanical data associated with those hypotheses.
     * \param[out] v  : the declared variables
     * \param[out] h  : modelling hypothesis on which the variables were
     * declared
     * \param[in]  m  : method used to assign the variables
     * \param[in]  b1 : allows variables to be declared as array
     */
    virtual void readVariableList(
        VariableDescriptionContainer&,
        std::set<Hypothesis>&,
        void (BehaviourDescription::*)(const Hypothesis,
                                       const VariableDescriptionContainer&,
                                       const BehaviourData::RegistrationStatus),
        const bool);

    /*!
     * Assign a list variables to mechanical data associated with the given
     * hypotheses.
     * \param[out] h : modelling hypothesis on which the variables were
     * declared \param[out] v : the declared variables \param[in]  m : method
     * used to assign the variables
     */
    virtual void addVariableList(const std::set<Hypothesis>&,
                                 const VariableDescriptionContainer&,
                                 void (BehaviourDescription::*)(
                                     const Hypothesis,
                                     const VariableDescriptionContainer&,
                                     const BehaviourData::RegistrationStatus));

    /*!
     * set the interfaces to be used
     */
    void setInterfaces(const std::set<std::string>&) override;
    /*!
     * \brief register a name.
     * \param[in] n : name
     */
    void reserveName(const std::string&) override;
    /*!
     * \brief look if a name is reserved
     * \param[in] n : name
     */
    bool isNameReserved(const std::string&) const override;
    //! \brief register the default variable names
    virtual void registerDefaultVarNames();
    void setUnitSystem(const std::string_view) override;
    void setMaterial(const std::string&) override;
    void setMaterialKnowledgeIdentifier(const std::string&) override;
    //!\brief treat the `@Gradient` keyword.
    virtual void treatGradient();
    /*!
     * \brief treat the `@ThermodynamicForce` keyword. Also treat the `@Flux`
     * keyword.
     */
    virtual void treatThermodynamicForce();
    //!\brief treat the `@TangentOperatorBlock` keyword.
    virtual void treatTangentOperatorBlock();
    //!\brief treat the `@SpeedOfSound` keyword.
    virtual void treatSpeedOfSound();
    //!\brief treat the `@TangentOperatorBlocks` keyword.
    virtual void treatTangentOperatorBlocks();
    //!\brief treat the `@AdditionalTangentOperatorBlock` keyword.
    virtual void treatAdditionalTangentOperatorBlock();
    //!\brief treat the `@AdditionalTangentOperatorBlocks` keyword.
    virtual void treatAdditionalTangentOperatorBlocks();
    //!\brief treat the `@Brick` keyword
    virtual void treatBrick();
    //!\brief treat the `@Model` keyword
    virtual void treatModel();
    /*!
     * \brief alternative treatment of the `@Model` keyword
     *
     * This alternative treatment is meant to be used by simple point-wise
     * models.
     */
    virtual void treatModel2();
    /*!
     * \brief get a model description from an mfront file
     * \param[in] m: file
     * \return a model description
     * \note the class name associated with the model is automatically
     * reserved.
     */
    virtual ModelDescription getModelDescription(const std::string&);
    /*!
     * \brief get a behaviour description from an mfront file
     * \param[in] m: file
     * \return a behaviour description
     */
    virtual BehaviourDescription getBehaviourDescription(const std::string&);
    //! \brief treat the `@Private` keyword
    void treatPrivate() override;
    //! \brief treat the `@Members` keyword
    void treatMembers() override;
    //! \brief treat the `@InitializeFunction` keyword
    virtual void treatInitializeFunction();
    //! \brief treat the `@PostProcessing` keyword
    virtual void treatPostProcessing();
    //! \brief treat the `@StrainMeasure` keyword
    virtual void treatStrainMeasure();
    /*!
     * \brief treat the `@TangentOperator` keyword
     * \note this method read the code block options and determines
     * which tangent operator is computed (for finite strain behaviours)
     * and then calls the `readTangentOperatorCodeBlock` method
     */
    virtual void treatTangentOperator();
    /*!
     * \brief read a code block describing the computation of one of the
     * tangent operator
     * \param[in] o: code block options
     * \param[in] n: name of the code block
     */
    virtual void readTangentOperatorCodeBlock(const CodeBlockOptions&,
                                              const std::string&);
    //! \brief treat the `@IsTangentOperatorSymmetric` keyword
    virtual void treatIsTangentOperatorSymmetric();
    //! \brief treat the `@Library` keyword
    virtual void treatLibrary();
    //! \brief treat the `@Profiling` keyword
    virtual void treatProfiling();
    //! \brief treat the `@ModellingHypothesis` keyword
    virtual void treatModellingHypothesis();
    //! \brief treat the `@ModellingHypotheses` keyword
    virtual void treatModellingHypotheses();
    //! \brief treat the `@UpdateAuxiliaryStateVariables` keyword
    virtual void treatUpdateAuxiliaryStateVariables();
    //! \brief treat the `@Initialize` keyword
    virtual void treatInitialize();
    //! \brief treat the `@InternalEnergy` keyword
    virtual void treatInternalEnergy();
    //! \brief treat the `@DissipatedEnergy` keyword
    virtual void treatDissipatedEnergy();
    //! \brief treat the `@ComputeStressFreeExpansion` keyword
    virtual void treatComputeStressFreeExpansion();
    //! \brief treat the `@UsableInPurelyImplicitResolution` keyword
    virtual void treatUsableInPurelyImplicitResolution();
    //! \brief treat the `@Parameter` keyword
    virtual void treatParameter();
    //! \brief treat the `@LocalVariables` keyword
    virtual void treatLocalVar();
    //! handle the `@ComputeThermalExpansion` keyword
    virtual void treatComputeThermalExpansion();
    virtual void treatComputeThermalExpansionFirstSyntax();
    virtual void treatComputeThermalExpansionSecondSyntax();
    //! handle the `@ComputeStiffnessTensor` keyword
    virtual void treatComputeStiffnessTensor();
    //! handle the `@ElasticMaterialProperties` keyword
    virtual void treatElasticMaterialProperties();
    /*!
     * \brief read the elastic material properties and assign them to
     * the behaviour Description
     */
    virtual void readElasticMaterialProperties();
    //! \brief helper function to read the elastic material properties
    virtual std::vector<BehaviourDescription::MaterialProperty>
    readElasticMaterialPropertiesI();
    //! \brief helper function to read the elastic material properties
    virtual std::vector<BehaviourDescription::MaterialProperty>
    readElasticMaterialPropertiesII();
    //! \brief handle the `@HillTensor` keyword
    virtual void treatHillTensor();
    //! \brief helper function to read the elastic material properties
    virtual std::vector<BehaviourDescription::MaterialProperty>
    readHillTensorDefinitionI();
    //! \brief helper function to read the elastic material properties
    virtual std::vector<BehaviourDescription::MaterialProperty>
    readHillTensorDefinitionII();
    //! \brief handle the `@InitLocalVariables` keyword
    virtual void treatInitLocalVariables();
    //! \brief handle the `@OrthotropicBehaviour` keyword
    virtual void treatOrthotropicBehaviour();
    //! \brief handle the `@IsotropicElasticBehaiour` keyword
    virtual void treatIsotropicElasticBehaviour();
    //! \brief handle the `@IsotropicBehaviour` keyword
    virtual void treatIsotropicBehaviour();
    //! \brief handle the `@RequireStiffnessOperator` keyword
    virtual void treatRequireStiffnessOperator();
    //! \brief handle the `@RequireStiffnessTensor` keyword
    virtual void treatRequireStiffnessTensor();

    virtual void treatStiffnessTensorOption();
    //! \brief handle the `@RequireThermalExpansionCoefficientTensor` keyword
    virtual void treatRequireThermalExpansionCoefficientTensor();
    //! \brief handle the `@Behaviour` keyword
    virtual void treatBehaviour();
    //! \brief handle the `@BehaviourVariable` keyword
    virtual void treatBehaviourVariable();
    //! \brief handle the `@BehaviourVariableFactory` keyword
    virtual void treatBehaviourVariableFactory();
    //! \brief handle the `@Interface` keyword
    virtual void treatInterface();
    //! \brief handle the `@StateVariable` keyword
    virtual void treatStateVariable();
    //! \brief handle the `@AuxiliaryStateVariable` keyword
    virtual void treatAuxiliaryStateVariable();
    //! \brief handle the `@ExternalStateVariable` keyword
    virtual void treatExternalStateVariable();
    //! \brief handle the `@InitializeFunctionVariable` keyword
    virtual void treatInitializeFunctionVariable();
    //! \brief handle the `@PostProcessingVariable` keyword
    virtual void treatPostProcessingVariable();
    //! \brief treat the `@MinimalTimeStepScalingFactor` keyword
    virtual void treatMinimalTimeStepScalingFactor();
    //! \brief treat the `@MaximalTimeStepScalingFactor` keyword
    virtual void treatMaximalTimeStepScalingFactor();
    //! \brief treat the `@APrioriTimeStepScalingFactor` keyword
    virtual void treatAPrioriTimeStepScalingFactor();
    //! \brief treat the `@Integrator` keyword
    virtual void treatIntegrator();
    //! \brief treat the `@APosterioriTimeStepScalingFactor` keyword
    virtual void treatAPosterioriTimeStepScalingFactor();
    //! \brief treat the `@MaterialProperty` and the `@Coef` keywords
    virtual void treatCoef();
    //! \brief treat the `@UseQt` keyword
    virtual void treatUseQt();
    //! \brief treat the `@Bounds` keyword
    virtual void treatBounds();
    //! \brief treat the `@PhysicalBounds` keyword
    virtual void treatPhysicalBounds();
    //! \brief treat the `@PredictionOperator` keyword
    virtual void treatPredictionOperator();
    //! \brief treat the `@Swelling` keyword
    virtual void treatSwelling();
    //! \brief treat the `@AxialGrowth` keyword
    virtual void treatAxialGrowth();
    //! \brief treat the `@Relocation` keyword
    virtual void treatRelocation();
    //! \brief treat the `@CrystalStructure` keyword
    virtual void treatCrystalStructure();
    //! \brief treat the `@SlipSystem` keyword
    virtual void treatSlipSystem();
    //! \brief treat the `@SlipSystems` keyword
    virtual void treatSlipSystems();
    //! \brief treat the `@InteractionMatrix` keyword
    virtual void treatInteractionMatrix();
    /*!
     * \brief treat the `@DislocationsMeanFreePathInteractionMatrix`
     * keyword.
     */
    virtual void treatDislocationsMeanFreePathInteractionMatrix();
    /*!
     * \brief read a swelling description.
     *
     * An array is expected at the current point of the file. Each
     * token of the array is analysed throw the
     * readStressFreeExpansionHandler method.
     */
    virtual std::vector<BehaviourData::StressFreeExpansionHandler>
    readStressFreeExpansionHandler();
    /*!
     * \brief extract a swelling description from a token
     * \param[in] t: treated token
     *
     * - if the token is a string, a mfront file is treated.
     * - if the token is not a string, one expects an external state
     *   variable name
     */
    virtual BehaviourData::StressFreeExpansionHandler
    readStressFreeExpansionHandler(const tfel::utilities::Token&);
    //! \brief treat methods associated with a gradient
    virtual void treatGradientMethod();
    //! \brief treat methods associated with a thermodynamic force
    virtual void treatThermodynamicForceMethod();
    /*!
     * \brief treat methods associated with a parameter
     * \param[in] h : modelling hypothesis
     */
    virtual void treatParameterMethod(const Hypothesis);
    //! \brief analyse the `setGlossaryNameMethod` and returns its argument
    virtual std::string treatSetGlossaryNameMethod();
    //! \brief analyse the `setEntryNameMethod` and returns its argument
    virtual std::string treatSetEntryNameMethod();
    /*!
     * \return true if the the given variable may have methods
     * \param[in] h : modelling hypothesis
     * \param[in] n : name
     */
    virtual bool isCallableVariable(const Hypothesis, const std::string&) const;
    /*!
     * treat a method
     * \param[in] h : modelling hypothesis
     */
    virtual void treatVariableMethod(const Hypothesis);
    /*!
     * \param[in] h : modelling hypothesis
     * \param[in] n : variable name
     */
    virtual void treatUnknownVariableMethod(const Hypothesis,
                                            const std::string&);
    //! \brief method called when an unknown keyword is parsed
    void treatUnknownKeyword() override;
    //! \brief destructor
    ~BehaviourDSLCommon() override;
    /*!
     * \param[in] h : modelling hypothesis
     * \param[in] n : variable name
     */
    virtual void
    declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution(
        const Hypothesis, const std::string&);
    /*!
     * \brief if no tangent operator was provided, but that the
     * behaviour requires a stiffness matrix, this method creates a
     * minimal tangent operator for elasticity.
     * \note This method is not trivial because one has to take care
     * not to create artifical mechanical data specialisation
     * \note This method is meant to be used in the
     * endsInputFileProcessing method.
     */
    virtual void setMinimalTangentOperator();
    /*!
     * \brief if the compte final thermodynamic forces code is not available,
     * create it from the ComputeFinalThermodynamicForcesCandidate code if it
     * is available. \note This method is not trivial because one has to take
     * care not to create artifical mechanical data specialisation \note This
     * method is meant to be used in the endsInputFileProcessing method.
     */
    virtual void
    setComputeFinalThermodynamicForcesFromComputeFinalThermodynamicForcesCandidateIfNecessary();
    //! \brief perform pedantic checks
    virtual void doPedanticChecks() const;
    /*!
     * \brief performs common checks on a code block computing the consistent
     * tangent operator or the tangent operator blocks
     * \param[in] cname: name of the code block
     * \param[in] c: code block
     */
    void checkTangentOperatorBlock(const std::string&, const CodeBlock&) const;
    //! \brief behaviour description
    BehaviourDescription mb;
    //! \brief registred bricks
    std::vector<std::shared_ptr<AbstractBehaviourBrick>> bricks;
    //! \brief the list of registred keywords
    std::set<std::string> registredKeyWords;
    //! \brief list of registred interfaces, indexed by their name
    std::map<std::string, std::shared_ptr<AbstractBehaviourInterface>>
        interfaces;
    //! \brief list of call backs
    std::map<std::string, CallBack> callBacks;
    //! \brief hooks assigned to callbacks
    std::map<std::string, std::vector<Hook>> hooks;
    //! \brief list of declared gradients
    std::vector<Gradient> gradients;
    //! \brief list of declared thermodynamic forces
    std::vector<ThermodynamicForce> thermodynamic_forces;

    bool explicitlyDeclaredUsableInPurelyImplicitResolution;
  };  // end of struct BehaviourDSLCommon

}  // end of namespace mfront

#endif /* LIB_MFRONT_MFRONTBEHAVIOURDSLCOMMON_HXX */
