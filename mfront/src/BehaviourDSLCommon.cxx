/*!
 * \file   mfront/src/BehaviourDSLCommon.cxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   05/05/2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <algorithm>
#include <stdexcept>
#include <iterator>
#include <sstream>
#include <utility>
#include <vector>
#include <limits>
#include <cctype>
#include <cmath>

#include "TFEL/Raise.hxx"
#include "TFEL/System/System.hxx"
#include "TFEL/UnicodeSupport/UnicodeSupport.hxx"
#include "TFEL/Glossary/Glossary.hxx"
#include "TFEL/Glossary/GlossaryEntry.hxx"
#include "TFEL/Math/General/IEEE754.hxx"
#include "TFEL/Material/FiniteStrainBehaviourTangentOperator.hxx"
#include "TFEL/Utilities/Data.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"
#include "TFEL/Math/Evaluator.hxx"

#include "MFront/MFront.hxx"
#include "MFront/MFrontHeader.hxx"
#include "MFront/MFrontUtilities.hxx"
#include "MFront/MFrontWarningMode.hxx"
#include "MFront/MFrontDebugMode.hxx"
#include "MFront/PedanticMode.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MFront/SearchPathsHandler.hxx"
#include "MFront/AbstractBehaviourInterface.hxx"
#include "MFront/MFrontMaterialPropertyInterface.hxx"
#include "MFront/BehaviourInterfaceFactory.hxx"
#include "MFront/AbstractBehaviourBrick.hxx"
#include "MFront/BehaviourBrickFactory.hxx"
#include "MFront/TargetsDescription.hxx"
#include "MFront/MaterialPropertyDescription.hxx"
#include "MFront/ModelDSL.hxx"
#include "MFront/MFrontModelInterface.hxx"
#include "MFront/GlobalDomainSpecificLanguageOptionsManager.hxx"
#include "MFront/AbstractBehaviourCodeGenerator.hxx"
#include "MFront/BehaviourVariableDescription.hxx"
#include "MFront/BehaviourDSLUtilities.hxx"
#include "MFront/BehaviourDSLCommon.hxx"

// fixing a bug on current glibc++ cygwin versions (19/08/2015)
#if defined __CYGWIN__ && (!defined _GLIBCXX_USE_C99)
#include <sstream>
namespace std {
  template <typename T>
  std::string to_string(const T& v) {
    std::ostringstream s;
    s << v;
    return s.str();
  }
}  // namespace std
#endif /* defined __CYGWIN__ &&  (!defined _GLIBCXX_USE_C99) */

namespace mfront {

  tfel::utilities::DataMapValidator
  BehaviourDSLCommon::getDSLOptionsValidator() {
    return DSLBase::getDSLOptionsValidator()
        .addDataTypeValidator<bool>(
            BehaviourDescription::
                automaticDeclarationOfTheTemperatureAsFirstExternalStateVariable)
        .addDataTypeValidator<std::string>(
            BehaviourDescription::modellingHypothesis)
        .addDataTypeValidator<std::string>(
            BehaviourDescription::internalNamespace)
        .addDataTypeValidator<bool>(BehaviourDescription::defaultConstructor)
        .addDataTypeValidator<bool>(BehaviourDescription::finalClass)
        .addDataTypeValidator<std::vector<tfel::utilities::Data>>(
            BehaviourDescription::modellingHypotheses);
  }  // end of getDSLOptionsValidator

  BehaviourDSLCommon::StandardVariableModifier::StandardVariableModifier(
      const Hypothesis h, const FunctionType f)
      : hypothesis(h),
        fct(f) {}  // end of StandardVariableModifier::StandardVariableModifier

  std::string BehaviourDSLCommon::StandardVariableModifier::exe(
      const std::string& v, const bool b) {
    return (this->fct)(this->hypothesis, v, b);
  }  // end of StandardVariableModifier::exe

  BehaviourDSLCommon::StandardVariableModifier::~StandardVariableModifier() =
      default;

  BehaviourDSLCommon::StandardWordAnalyser::StandardWordAnalyser(
      const Hypothesis h, const FunctionType f)
      : hypothesis(h),
        fct(f) {}  // end of StandardWordAnalyser::StandardWordAnalyser

  void BehaviourDSLCommon::StandardWordAnalyser::exe(CodeBlock& c,
                                                     const std::string& v) {
    this->fct(c, this->hypothesis, v);
  }  // end of StandardWordAnalyser::exe

  BehaviourDSLCommon::StandardWordAnalyser::~StandardWordAnalyser() = default;

  bool isValidBehaviourName(const std::string& n) {
    return tfel::utilities::CxxTokenizer::isValidIdentifier(n, false);
  }

  BehaviourDSLCommon::BehaviourDSLCommon(const DSLOptions& opts)
      : DSLBase(opts),
        mb(tfel::utilities::extract(
            opts,
            {DSLBase::overridingParameters,
             BehaviourDescription::
                 automaticDeclarationOfTheTemperatureAsFirstExternalStateVariable,
             BehaviourDescription::modellingHypothesis,
             BehaviourDescription::modellingHypotheses,
             BehaviourDescription::defaultConstructor,
             BehaviourDescription::finalClass,
             BehaviourDescription::internalNamespace})),
        explicitlyDeclaredUsableInPurelyImplicitResolution(false) {
    //
    using MemberFunc = void (BehaviourDSLCommon::*)();
    constexpr auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    //
    DSLBase::handleDSLOptions(this->mb, opts);
    // By default, a behaviour can be used in a purely implicit resolution
    this->mb.setUsableInPurelyImplicitResolution(h, true);
    // reserve names
    for (const auto& n : DSLBase::getDefaultReservedNames()) {
      this->reserveName(n);
    }
    // register behaviours specific names
    this->registerDefaultVarNames();
    this->reserveName("minimal_time_step_scaling_factor");
    this->reserveName("maximal_time_step_scaling_factor");
    this->reserveName("current_time_step_scaling_factor");
    this->reserveName("v_sound");
    this->reserveName(tfel::unicode::getMangledString("vₛ"));
    this->reserveName("rho_m0");
    this->reserveName(tfel::unicode::getMangledString("ρₘ₀"));
    // default call backs
    auto add = [this](const std::string& k, const MemberFunc f) {
      this->callBacks.insert({k, [this, f] { (this->*f)(); }});
      this->registredKeyWords.insert(k);
    };
    add(";", &BehaviourDSLCommon::treatLonelySeparator);
    add("@DSL", &BehaviourDSLCommon::treatDSL);
    add("@Parser", &BehaviourDSLCommon::treatDSL);
    add("@Model", &BehaviourDSLCommon::treatModel);
    add("@Brick", &BehaviourDSLCommon::treatBrick);
    add("@ModellingHypothesis", &BehaviourDSLCommon::treatModellingHypothesis);
    add("@ModellingHypotheses", &BehaviourDSLCommon::treatModellingHypotheses);
    add("@Import", &BehaviourDSLCommon::treatImport);
    add("@Material", &BehaviourDSLCommon::treatMaterial);
    add("@Library", &BehaviourDSLCommon::treatLibrary);
    add("@Profiling", &BehaviourDSLCommon::treatProfiling);
    add("@Behaviour", &BehaviourDSLCommon::treatBehaviour);
    add("@BehaviourVariable", &BehaviourDSLCommon::treatBehaviourVariable);
    add("@BehaviourVariableFactory",
        &BehaviourDSLCommon::treatBehaviourVariableFactory);
    add("@StrainMeasure", &BehaviourDSLCommon::treatStrainMeasure);
    add("@Author", &BehaviourDSLCommon::treatAuthor);
    add("@Date", &BehaviourDSLCommon::treatDate);
    add("@Initialize", &BehaviourDSLCommon::treatInitialize);
    add("@MFront", &BehaviourDSLCommon::treatMFront);
    add("@Link", &BehaviourDSLCommon::treatLink);
    add("@TFELLibraries", &BehaviourDSLCommon::treatTFELLibraries);
    add("@Includes", &BehaviourDSLCommon::treatIncludes);
    add("@Members", &BehaviourDSLCommon::treatMembers);
    add("@UnitSystem", &BehaviourDSLCommon::treatUnitSystem);
    add("@Coef", &BehaviourDSLCommon::treatCoef);
    add("@MaterialProperty", &BehaviourDSLCommon::treatCoef);
    add("@LocalVar", &BehaviourDSLCommon::treatLocalVar);
    add("@LocalVariable", &BehaviourDSLCommon::treatLocalVar);
    add("@Parameter", &BehaviourDSLCommon::treatParameter);
    add("@StateVar", &BehaviourDSLCommon::treatStateVariable);
    add("@StateVariable", &BehaviourDSLCommon::treatStateVariable);
    add("@AuxiliaryStateVar", &BehaviourDSLCommon::treatAuxiliaryStateVariable);
    add("@AuxiliaryStateVariable",
        &BehaviourDSLCommon::treatAuxiliaryStateVariable);
    add("@AuxiliaryModel", &BehaviourDSLCommon::treatAuxiliaryModel);
    add("@ExternalStateVar", &BehaviourDSLCommon::treatExternalStateVariable);
    add("@ExternalStateVariable",
        &BehaviourDSLCommon::treatExternalStateVariable);
    add("@InitLocalVars", &BehaviourDSLCommon::treatInitLocalVariables);
    add("@InitLocalVariables", &BehaviourDSLCommon::treatInitLocalVariables);
    add("@InitializeLocalVariables",
        &BehaviourDSLCommon::treatInitLocalVariables);
    add("@MinimalTimeStepScalingFactor",
        &BehaviourDSLCommon::treatMinimalTimeStepScalingFactor);
    add("@MaximalTimeStepScalingFactor",
        &BehaviourDSLCommon::treatMaximalTimeStepScalingFactor);
    add("@APrioriTimeStepScalingFactor",
        &BehaviourDSLCommon::treatAPrioriTimeStepScalingFactor);
    add("@Integrator", &BehaviourDSLCommon::treatIntegrator);
    add("@APosterioriTimeStepScalingFactor",
        &BehaviourDSLCommon::treatAPosterioriTimeStepScalingFactor);
    add("@Interface", &BehaviourDSLCommon::treatInterface);
    add("@StaticVar", &BehaviourDSLCommon::treatStaticVar);
    add("@StaticVariable", &BehaviourDSLCommon::treatStaticVar);
    add("@IntegerConstant", &BehaviourDSLCommon::treatIntegerConstant);
    add("@UseQt", &BehaviourDSLCommon::treatUseQt);
    add("@Description", &BehaviourDSLCommon::treatDescription);
    add("@Bounds", &BehaviourDSLCommon::treatBounds);
    add("@PhysicalBounds", &BehaviourDSLCommon::treatPhysicalBounds);
    add("@RequireStiffnessOperator",
        &BehaviourDSLCommon::treatRequireStiffnessOperator);
    add("@RequireStiffnessTensor",
        &BehaviourDSLCommon::treatRequireStiffnessTensor);
    add("@RequireThermalExpansionCoefficientTensor",
        &BehaviourDSLCommon::treatRequireThermalExpansionCoefficientTensor);
    add("@OrthotropicBehaviour",
        &BehaviourDSLCommon::treatOrthotropicBehaviour);
    add("@IsotropicElasticBehaviour",
        &BehaviourDSLCommon::treatIsotropicElasticBehaviour);
    add("@IsotropicBehaviour", &BehaviourDSLCommon::treatIsotropicBehaviour);
    add("@PredictionOperator", &BehaviourDSLCommon::treatPredictionOperator);
    add("@Private", &BehaviourDSLCommon::treatPrivate);
    add("@Sources", &BehaviourDSLCommon::treatSources);
    add("@UpdateAuxiliaryStateVars",
        &BehaviourDSLCommon::treatUpdateAuxiliaryStateVariables);
    add("@UpdateAuxiliaryStateVariables",
        &BehaviourDSLCommon::treatUpdateAuxiliaryStateVariables);
    add("@ComputeThermalExpansion",
        &BehaviourDSLCommon::treatComputeThermalExpansion);
    add("@ComputeStressFreeExpansion",
        &BehaviourDSLCommon::treatComputeStressFreeExpansion);
    add("@Swelling", &BehaviourDSLCommon::treatSwelling);
    add("@AxialGrowth", &BehaviourDSLCommon::treatAxialGrowth);
    add("@Relocation", &BehaviourDSLCommon::treatRelocation);
    add("@InternalEnergy", &BehaviourDSLCommon::treatInternalEnergy);
    add("@DissipatedEnergy", &BehaviourDSLCommon::treatDissipatedEnergy);
    add("@CrystalStructure", &BehaviourDSLCommon::treatCrystalStructure);
    add("@SlipSystem", &BehaviourDSLCommon::treatSlipSystem);
    add("@GlidingSystem", &BehaviourDSLCommon::treatSlipSystem);
    add("@SlidingSystem", &BehaviourDSLCommon::treatSlipSystem);
    add("@SlipSystems", &BehaviourDSLCommon::treatSlipSystems);
    add("@GlidingSystems", &BehaviourDSLCommon::treatSlipSystems);
    add("@SlidingSystems", &BehaviourDSLCommon::treatSlipSystems);
    add("@InteractionMatrix", &BehaviourDSLCommon::treatInteractionMatrix);
    add("@SpeedOfSound", &BehaviourDSLCommon::treatSpeedOfSound);
    add("@DislocationsMeanFreePathInteractionMatrix",
        &BehaviourDSLCommon::treatDislocationsMeanFreePathInteractionMatrix);
    add("@InitializeFunctionVariable",
        &BehaviourDSLCommon::treatInitializeFunctionVariable);
    add("@InitializeFunction", &BehaviourDSLCommon::treatInitializeFunction);
    add("@PostProcessingVariable",
        &BehaviourDSLCommon::treatPostProcessingVariable);
    add("@PostProcessing", &BehaviourDSLCommon::treatPostProcessing);
  }  // end of BehaviourDSLCommon

  void BehaviourDSLCommon::writeMaterialPropertyEvaluation(
      std::ostream& out,
      const BehaviourDescription::MaterialProperty& m,
      std::function<std::string(const MaterialPropertyInput&)>& f) const {
    const auto g = this->getCodeGenerator();
    g->writeMaterialPropertyEvaluation(out, m, f);
  }  // end of writeMaterialPropertyEvaluation

  std::vector<AbstractDSL::DSLOptionDescription>
  BehaviourDSLCommon::getDSLOptions() const {
    auto opts = DSLBase::getDSLOptions();
    opts.push_back(
        {BehaviourDescription::
             automaticDeclarationOfTheTemperatureAsFirstExternalStateVariable,
         "boolean stating if the temperature shall be automatically declared "
         "as an external state variable"});
    return opts;
  }  // end of getDSLOptions

  AbstractDSL::DSLOptions BehaviourDSLCommon::buildDSLOptions() const {
    return DSLBase::buildCommonDSLOptions(this->mb);
  }  // end of buildDSLOptions

  std::string BehaviourDSLCommon::getMaterialKnowledgeIdentifier() const {
    if (this->mb.isBehaviourNameDefined()) {
      return this->mb.getBehaviourName();
    }
    return {};
  }  // end of getMaterialKnowledgeIdentifier

  std::string BehaviourDSLCommon::getMaterialName() const {
    return this->mb.getMaterialName();
  }  // end of getMaterialName(

  void BehaviourDSLCommon::analyse() {
    const auto& mh = ModellingHypothesis::getModellingHypotheses();
    std::vector<std::string> hn(mh.size());
    std::vector<Hypothesis>::const_iterator pmh;
    std::vector<std::string>::iterator phn;
    for (pmh = mh.begin(), phn = hn.begin(); pmh != mh.end(); ++pmh, ++phn) {
      *phn = ModellingHypothesis::toString(*pmh);
    }
    // strip comments from file
    this->stripComments();
    // begin treatement
    this->current = this->tokens.begin();
    while (this->current != this->tokens.end()) {
      if (find(hn.begin(), hn.end(), this->current->value) != hn.end()) {
        const auto h = ModellingHypothesis::fromString(this->current->value);
        ++(this->current);
        this->checkNotEndOfFile("BehaviourDSLCommon::analyse");
        this->readSpecifiedToken("BehaviourDSLCommon::analyse", "::");
        const auto v = tfel::unicode::getMangledString(this->current->value);
        if (!this->isCallableVariable(h, v)) {
          this->throwRuntimeError("BehaviourDSLCommon::analyse",
                                  "no variable named '" + v +
                                      "' for hypothesis '" +
                                      ModellingHypothesis::toString(h) + "'");
        }
        if (this->mb.isParameterName(h, v)) {
          this->treatParameterMethod(h);
        } else {
          this->treatVariableMethod(h);
        }
      } else {
        const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
        const auto v = tfel::unicode::getMangledString(this->current->value);
        if (this->isCallableVariable(h, v)) {
          const auto isGradient = [this, &v] {
            for (const auto& g : this->gradients) {
              if (g.name == v) {
                return true;
              }
            }
            return this->mb.isGradientName(v);
          }();
          const auto isThermodynamicForce = [this, &v] {
            for (const auto& f : this->thermodynamic_forces) {
              if (f.name == v) {
                return true;
              }
            }
            return this->mb.isThermodynamicForceName(v);
          }();
          if (isGradient) {
            this->treatGradientMethod();
          } else if (isThermodynamicForce) {
            this->treatThermodynamicForceMethod();
          } else if (this->mb.isParameterName(h, v)) {
            this->treatParameterMethod(h);
          } else {
            this->treatVariableMethod(h);
          }
        } else {
          const auto k = this->current->value;
          const auto l = this->current->line;
          CallBack handler;
          auto p = this->callBacks.find(k);
          if (p == this->callBacks.end()) {
            if (getVerboseMode() >= VERBOSE_DEBUG) {
              auto& log = getLogStream();
              log << "treating unknown keyword\n";
            }
            handler = [this] { this->treatUnknownKeyword(); };
          } else {
            if (getVerboseMode() >= VERBOSE_DEBUG) {
              auto& log = getLogStream();
              log << "treating keyword : " << this->current->value << '\n';
            }
            handler = p->second;
          }
          this->currentComment = this->current->comment;
          ++(this->current);
          try {
            handler();
            const auto ph = this->hooks.find(k);
            if (ph != this->hooks.end()) {
              for (auto& hook : ph->second) {
                hook();
              }
            }
          } catch (std::exception& e) {
            std::ostringstream msg;
            msg << "BehaviourDSLCommon::analyse: "
                << "error while treating keyword '" << k << "' at line '" << l
                << "' of file '" << this->fd.fileName << "'.\n"
                << e.what();
            tfel::raise(msg.str());
          } catch (...) {
            this->currentComment.clear();
            throw;
          }
          this->currentComment.clear();
        }
      }
    }
  }  // end of analyse

  void BehaviourDSLCommon::importFile(
      const std::string& fn,
      const std::vector<std::string>& ecmds,
      const std::map<std::string, std::string>& s) {
    this->fd.fileName = fn;
    this->openFile(this->fd.fileName, ecmds, s);
    this->analyse();
  }  // end of importFile

  void BehaviourDSLCommon::analyseString(const std::string& s) {
    this->fd.fileName = "user defined string";
    this->parseString(s);
    this->analyse();
  }  // end of analyseString

  void BehaviourDSLCommon::getKeywordsList(std::vector<std::string>& k) const {
    for (const auto& c : this->callBacks) {
      k.push_back(c.first);
    }
  }  // end of getKeywordsList

  void BehaviourDSLCommon::addCallBack(const std::string& k,
                                       const CallBack c,
                                       const bool b) {
    if (!this->callBacks.insert({k, c}).second) {
      if (!b) {
        this->throwRuntimeError("BehaviourDSLCommon::addCallBack",
                                "callback '" + k + "' already registred");
      } else {
        this->callBacks[k] = c;
      }
    } else {
      this->registredKeyWords.insert(k);
    }
  }  // end of addCallBack

  void BehaviourDSLCommon::addHook(const std::string& k, const Hook h) {
    if (this->callBacks.find(k) == this->callBacks.end()) {
      this->throwRuntimeError("BehaviourDSLCommon::addHook",
                              "no callback called '" + k + "'");
    }
    this->hooks[k].push_back(h);
  }  // end of addHook

  void BehaviourDSLCommon::setMaterial(const std::string& m) {
    if (!isValidMaterialName(m)) {
      this->throwRuntimeError("BehaviourDSLCommon::setMaterial",
                              "invalid material name '" + m + "'");
    }
    this->mb.setMaterialName(m);
    if (!isValidIdentifier(this->mb.getClassName())) {
      this->throwRuntimeError("BehaviourDSLCommon::setMaterial",
                              "resulting class name is not valid (read '" +
                                  this->mb.getClassName() + "')");
    }
  }  // end of setMaterial

  void BehaviourDSLCommon::setUnitSystem(const std::string_view s) {
    this->mb.setUnitSystem(s);
  }  // end of setUnitSystem

  void BehaviourDSLCommon::treatDisabledCallBack() {
    --(this->current);
    tfel::raise("The keyword: '" + this->current->value +
                "' has been disabled");
  }  // end of treatDisabledCallBack

  void BehaviourDSLCommon::disableCallBack(const std::string& k) {
    auto c = [this] { this->treatDisabledCallBack(); };
    auto p = this->callBacks.find(k);
    if (p == this->callBacks.end()) {
      this->callBacks.insert({k, c});
      this->registredKeyWords.insert(k);
      return;
    }
    p->second = c;
  }  // end of disableCallBack

  BehaviourDSLCommon::CodeBlockOptions BehaviourDSLCommon::treatCodeBlock(
      const std::string& n,
      std::function<
          std::string(const Hypothesis, const std::string&, const bool)> m,
      const bool b,
      const bool s) {
    CodeBlockOptions o;
    this->readCodeBlockOptions(o, s);
    this->treatUnsupportedCodeBlockOptions(o);
    this->treatCodeBlock(o, n, m, b);
    return o;
  }

  BehaviourDSLCommon::CodeBlockOptions BehaviourDSLCommon::treatCodeBlock(
      const std::string& n,
      std::function<
          std::string(const Hypothesis, const std::string&, const bool)> m,
      std::function<void(CodeBlock&, const Hypothesis, const std::string&)> a,
      const bool b,
      const bool s) {
    CodeBlockOptions o;
    this->readCodeBlockOptions(o, s);
    this->treatUnsupportedCodeBlockOptions(o);
    this->treatCodeBlock(o, n, m, a, b);
    return o;
  }  // end of treatCodeBlock

  void BehaviourDSLCommon::treatCodeBlock(
      const BehaviourDSLCommon::CodeBlockOptions& o,
      const std::string& n,
      std::function<
          std::string(const Hypothesis, const std::string&, const bool)> m,
      std::function<void(CodeBlock&, const Hypothesis, const std::string&)> a,
      const bool b) {
    const auto beg = this->current;
    for (const auto h : o.hypotheses) {
      this->current = beg;
      auto c = this->readNextBlock(h, n, m, a, b);
      if (o.safe) {
        c.attributes[CodeBlock::safe] = true;
      }
      this->mb.setCode(h, n, c, o.m, o.p);
    }
  }  // end of treatCodeBlock

  void BehaviourDSLCommon::treatCodeBlock(
      const BehaviourDSLCommon::CodeBlockOptions& o,
      const std::string& n,
      std::function<
          std::string(const Hypothesis, const std::string&, const bool)> m,
      const bool b) {
    this->disableVariableDeclaration();
    const auto beg = this->current;
    for (const auto h : o.hypotheses) {
      this->current = beg;
      auto c = this->readNextBlock(h, n, m, b);
      if (o.safe) {
        c.attributes[CodeBlock::safe] = true;
      }
      this->mb.setCode(h, n, c, o.m, o.p);
    }
  }  // end of treatCodeBlock

  CodeBlock BehaviourDSLCommon::readNextBlock(
      const Hypothesis h,
      const std::string& n,
      std::function<
          std::string(const Hypothesis, const std::string&, const bool)> m,
      std::function<void(CodeBlock&, const Hypothesis, const std::string&)> a,
      const bool b) {
    this->disableVariableDeclaration();
    const auto& md = this->mb.getBehaviourData(h);
    auto vm = std::make_shared<StandardVariableModifier>(h, m);
    auto wa = std::make_shared<StandardWordAnalyser>(h, a);
    CodeBlockParserOptions option;
    option.qualifyStaticVariables = b;
    option.qualifyMemberVariables = b;
    option.modifier = vm;
    option.analyser = wa;
    option.member_names = md.getRegistredMembersNames();
    option.static_member_names = md.getRegistredStaticMembersNames();
    this->getSymbols(option.symbols, h, n);
    return this->readNextBlock(option);
  }  // end of readNextBlock

  CodeBlock BehaviourDSLCommon::readNextBlock(
      const Hypothesis h,
      const std::string& n,
      std::function<
          std::string(const Hypothesis, const std::string&, const bool)> m,
      const bool b) {
    this->disableVariableDeclaration();
    const auto& md = this->mb.getBehaviourData(h);
    auto vm = std::make_shared<StandardVariableModifier>(h, m);
    CodeBlockParserOptions option;
    option.qualifyStaticVariables = b;
    option.qualifyMemberVariables = b;
    option.modifier = vm;
    option.member_names = md.getRegistredMembersNames();
    option.static_member_names = md.getRegistredStaticMembersNames();
    this->getSymbols(option.symbols, h, n);
    return this->readNextBlock(option);
  }  // end of readNextBlock

  BehaviourDSLCommon::CodeBlockOptions BehaviourDSLCommon::treatCodeBlock(
      const std::string& n1,
      const std::string& n2,
      std::function<
          std::string(const Hypothesis, const std::string&, const bool)> m1,
      std::function<
          std::string(const Hypothesis, const std::string&, const bool)> m2,
      const bool b,
      const bool s) {
    CodeBlockOptions o;
    this->readCodeBlockOptions(o, s);
    this->treatUnsupportedCodeBlockOptions(o);
    this->treatCodeBlock(o, n1, n2, m1, m2, b);
    return o;
  }  // end of treatCodeBlock

  void BehaviourDSLCommon::treatCodeBlock(
      const BehaviourDSLCommon::CodeBlockOptions& o,
      const std::string& n1,
      const std::string& n2,
      std::function<
          std::string(const Hypothesis, const std::string&, const bool)> m1,
      std::function<
          std::string(const Hypothesis, const std::string&, const bool)> m2,
      const bool b) {
    const auto beg = this->current;
    this->disableVariableDeclaration();
    for (const auto& h : o.hypotheses) {
      const auto& md = this->mb.getBehaviourData(h);
      this->current = beg;
      CodeBlock c1;
      CodeBlock c2;
      if (o.safe) {
        c1.attributes[CodeBlock::safe] = true;
        c2.attributes[CodeBlock::safe] = true;
      }
      CodeBlockParserOptions o1;
      o1.qualifyStaticVariables = b;
      o1.qualifyMemberVariables = b;
      o1.modifier = std::make_shared<StandardVariableModifier>(h, m1);
      o1.member_names = md.getRegistredMembersNames();
      o1.static_member_names = md.getRegistredStaticMembersNames();
      this->getSymbols(o1.symbols, h, n1);
      CodeBlockParserOptions o2;
      o2.qualifyStaticVariables = b;
      o2.qualifyMemberVariables = b;
      o2.modifier = std::make_shared<StandardVariableModifier>(h, m2);
      o2.member_names = md.getRegistredMembersNames();
      o2.static_member_names = md.getRegistredStaticMembersNames();
      this->getSymbols(o2.symbols, h, n1);
      this->readNextBlock(c1, c2, o1, o2);
      this->mb.setCode(h, n1, c1, o.m, o.p);
      this->mb.setCode(h, n2, c2, o.m, o.p);
    }
  }  // end of treatCodeBlock

  void BehaviourDSLCommon::addMaterialProperties(
      const VariableDescriptionContainer& mps) {
    constexpr auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    const auto d = this->getBehaviourDSLDescription();
    tfel::raise_if(!d.allowUserDefinedMaterialProperties,
                   "BehaviourDSLCommon::addMaterialProperties: "
                   "adding material properties is not allowed");
    this->mb.addMaterialProperties(h, mps);
  }  // end of addMaterialProperties

  void BehaviourDSLCommon::addParameters(
      const VariableDescriptionContainer& params) {
    constexpr auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    const auto d = this->getBehaviourDSLDescription();
    tfel::raise_if(!d.allowUserDefinedParameters,
                   "BehaviourDSLCommon::addParameters: "
                   "adding parameters is not allowed");
    this->mb.addParameters(h, params);
  }  // end of addParameters

  void BehaviourDSLCommon::addStateVariables(
      const VariableDescriptionContainer& isvs) {
    constexpr auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    const auto d = this->getBehaviourDSLDescription();
    tfel::raise_if(!d.allowUserDefinedStateVariables,
                   "BehaviourDSLCommon::addStateVariables: "
                   "adding state variables is not allowed");
    this->mb.addStateVariables(h, isvs);
  }  // end of addStateVariables

  void BehaviourDSLCommon::addAuxiliaryStateVariables(
      const VariableDescriptionContainer& isvs) {
    constexpr auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    const auto d = this->getBehaviourDSLDescription();
    tfel::raise_if(!d.allowUserDefinedAuxiliaryStateVariables,
                   "BehaviourDSLCommon::addAuxiliaryStateVariables: "
                   "adding auxiliary state variables is not allowed");
    this->mb.addAuxiliaryStateVariables(h, isvs);
  }  // end of addAuxiliaryStateVariables

  void BehaviourDSLCommon::addExternalStateVariables(
      const VariableDescriptionContainer& esvs) {
    constexpr auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    const auto d = this->getBehaviourDSLDescription();
    tfel::raise_if(!d.allowUserDefinedExternalStateVariables,
                   "BehaviourDSLCommon::addExternalStateVariables: "
                   "adding external state variables is not allowed");
    this->mb.addExternalStateVariables(h, esvs);
  }  // end of addExternalStateVariables

  void BehaviourDSLCommon::addLocalVariables(
      const VariableDescriptionContainer& lvs) {
    constexpr auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    const auto d = this->getBehaviourDSLDescription();
    tfel::raise_if(!d.allowUserDefinedLocalVariables,
                   "BehaviourDSLCommon::addLocalVariables: "
                   "adding local variables is not allowed");
    this->mb.addLocalVariables(h, lvs);
  }  // end of addLocalVariables

  void BehaviourDSLCommon::addIntegrationVariables(
      const VariableDescriptionContainer& ivs) {
    constexpr auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    const auto d = this->getBehaviourDSLDescription();
    tfel::raise_if(!d.allowUserDefinedIntegrationVariables,
                   "BehaviourDSLCommon::addIntegrationVariables: "
                   "adding integration variables is not allowed");
    this->mb.addIntegrationVariables(h, ivs);
  }  // end of addIntegrationVariables

  BehaviourDSLCommon::CodeBlockOptions::CodeBlockOptions()
      : p(BehaviourData::BODY), m(BehaviourData::CREATE) {
    this->hypotheses.insert(ModellingHypothesis::UNDEFINEDHYPOTHESIS);
  }  // end of CodeBlockOptions::CodeBlockOptions

  BehaviourDSLCommon::CodeBlockOptions::~CodeBlockOptions() = default;

  const BehaviourDescription& BehaviourDSLCommon::getBehaviourDescription()
      const {
    return this->mb;
  }  // end of getBehaviourDescription

  void BehaviourDSLCommon::addExternalMFrontFile(
      const std::string& f,
      const std::vector<std::string>& vinterfaces,
      const tfel::utilities::DataMap& dsl_options) {
    this->mb.addExternalMFrontFile(f, vinterfaces, dsl_options);
  }  // end of addExternalMFrontFile

  const MaterialKnowledgeDescription&
  BehaviourDSLCommon::getMaterialKnowledgeDescription() const {
    return this->mb;
  }  // end of getMaterialKnowledgeDescription

  std::string BehaviourDSLCommon::getClassName() const {
    return this->mb.getClassName();
  }  // end of getClassName

  void BehaviourDSLCommon::addMaterialLaw(const std::string& m) {
    this->mb.addMaterialLaw(m);
  }  // end of addMaterialLaw

  void BehaviourDSLCommon::appendToIncludes(const std::string& c) {
    this->mb.appendToIncludes(c);
  }  // end of appendToIncludes

  void BehaviourDSLCommon::appendToMembers(const std::string& c) {
    this->mb.appendToMembers(ModellingHypothesis::UNDEFINEDHYPOTHESIS, c, true);
  }  // end of appendToMembers

  void BehaviourDSLCommon::appendToPrivateCode(const std::string& c) {
    this->mb.appendToPrivateCode(ModellingHypothesis::UNDEFINEDHYPOTHESIS, c,
                                 true);
  }  // end of appendToPrivateCode

  void BehaviourDSLCommon::appendToSources(const std::string& c) {
    this->mb.appendToSources(c);
  }  // end of appendToSources

  void BehaviourDSLCommon::appendToHypothesesList(std::set<Hypothesis>& h,
                                                  const std::string& v) const {
    if (v == ".+") {
      const auto& ash = ModellingHypothesis::getModellingHypotheses();
      for (const auto& lh : ash) {
        this->appendToHypothesesList(h, ModellingHypothesis::toString(lh));
      }
    } else {
      const auto nh = ModellingHypothesis::fromString(v);
      if (!this->isModellingHypothesisSupported(nh)) {
        this->throwRuntimeError(
            "BehaviourDSLCommon::appendToHypothesesList",
            "hypothesis '" + v + "' is not supported by this parser");
      }
      if (this->mb.areModellingHypothesesDefined()) {
        const auto& bh = this->mb.getModellingHypotheses();
        if (bh.find(nh) == bh.end()) {
          this->throwRuntimeError(
              "BehaviourDSLCommon::appendToHypothesesList",
              "hypothesis '" + v +
                  "' is not supported by the "
                  "behaviour (This means that one of the "
                  "'@ModellingHypothesis' or '@ModellingHypotheses'"
                  "keyword was used earlier)");
        }
      }
      if (!h.insert(nh).second) {
        this->throwRuntimeError("BehaviourDSLCommon::appendToHypothesesList",
                                "hypothesis '" + v + "' multiply defined");
      }
    }
  }  // end of appendToHypothesesList

  void BehaviourDSLCommon::getSymbols(
      std::map<std::string, std::string>& symbols,
      const Hypothesis h,
      const std::string&) {
    addSymbol(symbols, "I\u2082", "Stensor::Id()");
    addSymbol(symbols, "I\u2084", "Stensor4::Id()");
    addSymbol(symbols, "\u2297", "^");
    addSymbol(symbols, "\u22C5", "*");
    this->mb.getSymbols(symbols, h);
  }  // end of getSymbols

  void BehaviourDSLCommon::readCodeBlockOptions(CodeBlockOptions& o,
                                                const bool s) {
    using namespace tfel::utilities;
    using namespace tfel::material;
    auto cposition = false;
    auto cmode = false;
    const auto dh = [this] {
      if (this->mb.areModellingHypothesesDefined()) {
        const auto mh = this->mb.getModellingHypotheses();
        if (mh.size() == 1) {
          return *(mh.begin());
        }
      }
      return ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    }();
    o.hypotheses.clear();
    if (this->current == this->tokens.end()) {
      o.hypotheses.insert(dh);
      return;
    }
    if (this->current->value != "<") {
      o.hypotheses.insert(dh);
      return;
    }
    auto options = std::vector<Token>{};
    this->readList(options, "BehaviourDSLCommon::readCodeBlockOptions", "<",
                   ">", true);
    for (const auto& t : options) {
      if ((t.value == "Safe") || (t.value == "safe")) {
        if (o.safe) {
          this->throwRuntimeError("BehaviourDSLCommon::readCodeBlockOptions",
                                  "safe option already specificed");
        }
        o.safe = true;
      } else if (t.value == "Append") {
        if (cmode) {
          this->throwRuntimeError("BehaviourDSLCommon::readCodeBlockOptions",
                                  "insertion mode already specificed");
        }
        cmode = true;
        o.m = BehaviourData::CREATEORAPPEND;
      } else if (t.value == "Replace") {
        if (cmode) {
          this->throwRuntimeError("BehaviourDSLCommon::readCodeBlockOptions",
                                  "insertion mode already specificed");
        }
        cmode = true;
        o.m = BehaviourData::CREATEORREPLACE;
      } else if (t.value == "Create") {
        if (cmode) {
          this->throwRuntimeError("BehaviourDSLCommon::readCodeBlockOptions",
                                  "insertion mode already specificed");
        }
        cmode = true;
        o.m = BehaviourData::CREATE;
      } else if (t.value == "Body") {
        if (cposition) {
          this->throwRuntimeError("BehaviourDSLCommon::readCodeBlockOptions",
                                  "insertion position already specificed");
        }
        cposition = true;
        o.p = BehaviourData::BODY;
      } else if (t.value == "AtBeginning") {
        if (cposition) {
          this->throwRuntimeError("BehaviourDSLCommon::readCodeBlockOptions",
                                  "insertion position already specificed");
        }
        cposition = true;
        o.p = BehaviourData::AT_BEGINNING;
      } else if (t.value == "AtEnd") {
        if (cposition) {
          this->throwRuntimeError("BehaviourDSLCommon::readCodeBlockOptions",
                                  "insertion position already specificed");
        }
        cposition = true;
        o.p = BehaviourData::AT_END;
      } else if ((t.flag == Token::String) &&
                 (t.value.substr(1, t.value.size() - 2) == "+")) {
        this->appendToHypothesesList(o.hypotheses,
                                     t.value.substr(1, t.value.size() - 2));
      } else if (ModellingHypothesis::isModellingHypothesis(t.value)) {
        this->appendToHypothesesList(o.hypotheses, t.value);
      } else {
        o.untreated.push_back(t);
      }
    }
    if (o.hypotheses.empty()) {
      o.hypotheses.insert(dh);
    }
    // checks
    if (!s) {
      if (o.hypotheses.size() != 1u) {
        this->throwRuntimeError("BehaviourDSLCommon::readCodeBlockOptions: ",
                                "specialisation is not allowed");
      }
      if (*(o.hypotheses.begin()) != dh) {
        this->throwRuntimeError("BehaviourDSLCommon::readCodeBlockOptions: ",
                                "specialisation is not allowed");
      }
    }
  }  // end of readCodeBlockOptions

  std::shared_ptr<MaterialPropertyDescription>
  BehaviourDSLCommon::handleMaterialPropertyDescription(const std::string& f) {
    return DSLBase::handleMaterialPropertyDescription(f);
  }  // end of handleMaterialPropertyDescription

  ModelDescription BehaviourDSLCommon::getModelDescription(
      const std::string& f) {
    if (getVerboseMode() >= VERBOSE_DEBUG) {
      getLogStream() << "BehaviourDSLCommon::getModelDescription: "
                     << "treating file '" << f << "'\n";
    }
    //
    const auto path = SearchPathsHandler::search(f);
    // a simple test to fix Issue #524
    const auto adsl = MFrontBase::getDSL(path);
    if (adsl->getTargetType() != AbstractDSL::MODELDSL) {
      this->throwRuntimeError("BehaviourDSLCommon::getModelDescription",
                              "error while treating file '" + f +
                                  "'.\n"
                                  "This file is not handled by the Model DSL");
    }
    //
    const auto& global_options =
        GlobalDomainSpecificLanguageOptionsManager::get();
    auto dsl = ModelDSL{tfel::utilities::merge(
        global_options.getModelDSLOptions(), this->buildDSLOptions(), true)};
    // getting informations the source files
    try {
      dsl.setInterfaces({"mfront"});
      dsl.analyseFile(path, {}, {});
      const auto t = dsl.getTargetsDescription();
      if (!t.specific_targets.empty()) {
        this->throwRuntimeError("BehaviourDSLCommon::getModelDescription",
                                "error while treating file '" + f +
                                    "'.\n"
                                    "Specific targets are not supported");
      }
      for (const auto& h : t.headers) {
        this->appendToIncludes("#include\"" + h + "\"");
      }
      this->atds.push_back(std::move(t));
      this->mb.addExternalMFrontFile(path, {"mfront"}, {});
    } catch (std::exception& e) {
      this->throwRuntimeError(
          "BehaviourDSLCommon::getModelDescription",
          "error while treating file '" + f + "'\n" + std::string(e.what()));
    } catch (...) {
      this->throwRuntimeError("BehaviourDSLCommon::getModelDescription",
                              "error while treating file '" + f + "'");
    }
    const auto& md = dsl.getModelDescription();
    this->reserveName(md.className);
    if (getVerboseMode() >= VERBOSE_DEBUG) {
      getLogStream() << "BehaviourDSLCommon::getModelDescription: "
                     << "end of file '" << f << "' treatment\n";
    }
    return md;
  }  // end of getModelDescription

  BehaviourDescription BehaviourDSLCommon::getBehaviourDescription(
      const std::string& f) {
    if (getVerboseMode() >= VERBOSE_DEBUG) {
      getLogStream() << "BehaviourDSLCommon::getBehaviourDescription: "
                     << "treating file '" << f << "'\n";
    }
    //
    const auto path = SearchPathsHandler::search(f);
    const auto dsl_options = tfel::utilities::DataMap{
        {BehaviourDescription::defaultConstructor, true},
        {BehaviourDescription::finalClass, false},
        {BehaviourDescription::internalNamespace,
         std::string{"mfront_internal_behaviours"}}};
    // a simple test to fix Issue #524
    const auto adsl = MFrontBase::getDSL(path, dsl_options);
    if (adsl->getTargetType() != AbstractDSL::BEHAVIOURDSL) {
      this->throwRuntimeError(
          "BehaviourDSLCommon::getBehaviourDescription",
          "error while treating file '" + f +
              "'.\nThis file is not handled by the Behaviour DSL");
    }
    //
    auto dsl = dynamic_pointer_cast<AbstractBehaviourDSL>(adsl);
    if (dsl.get() == nullptr) {
      this->throwRuntimeError("BehaviourDSLCommon::getBehaviourDescription",
                              "internal error while treating file '" + f +
                                  "(The DSL associated with this file is not "
                                  "convertible to an AbstractBehaviourDSL)\n");
    }
    // getting informations the source files
    try {
      dsl->analyseFile(path, {}, {});
      const auto t = dsl->getTargetsDescription();
      if (!t.specific_targets.empty()) {
        this->throwRuntimeError("BehaviourDSLCommon::getBehaviourDescription",
                                "error while treating file '" + f +
                                    "'.\nSpecific targets are not supported");
      }
      for (const auto& h : t.headers) {
        this->appendToIncludes("#include\"" + h + "\"");
      }
      this->atds.push_back(std::move(t));
      if (dsl->getCodeGenerator()->isSrcFileRequired()) {
        insert_if(this->additional_libraries_sources,
                  dsl->getBehaviourDescription().getSrcFileName());
      }
      this->mb.addExternalMFrontFile(path, {"mfront"}, dsl_options);
    } catch (std::exception& e) {
      this->throwRuntimeError(
          "BehaviourDSLCommon::getBehaviourDescription",
          "error while treating file '" + f + "'\n" + std::string(e.what()));
    } catch (...) {
      this->throwRuntimeError("BehaviourDSLCommon::getBehaviourDescription",
                              "error while treating file '" + f + "'");
    }
    const auto& bd = dsl->getBehaviourDescription();
    if (getVerboseMode() >= VERBOSE_DEBUG) {
      getLogStream() << "BehaviourDSLCommon::getBehaviourDescription: "
                     << "end of file '" << f << "' treatment\n";
    }
    return bd;
  }  // end of getBehaviourDescription

  void BehaviourDSLCommon::declareMainVariables() {
    decltype(this->gradients.size()) n =
        std::min(this->gradients.size(), this->thermodynamic_forces.size());
    while (n != 0) {
      this->mb.addMainVariable(this->gradients.front(),
                               this->thermodynamic_forces.front());
      this->gradients.erase(this->gradients.begin());
      this->thermodynamic_forces.erase(this->thermodynamic_forces.begin());
      --n;
    }
  }  // end of declareMainVariables

  void BehaviourDSLCommon::treatGradient() {
    VariableDescriptionContainer ngradients;
    this->readVarList(ngradients, true);
    std::for_each(ngradients.begin(), ngradients.end(),
                  [this](const VariableDescription& v) {
                    Gradient g(v);
                    Gradient::setIsIncrementKnownAttribute(g, true);
                    this->gradients.emplace_back(std::move(g));
                  });
    this->declareMainVariables();
  }  // end of treatGradient

  void BehaviourDSLCommon::treatThermodynamicForce() {
    VariableDescriptionContainer ntfs;
    this->readVarList(ntfs, true);
    std::for_each(ntfs.begin(), ntfs.end(),
                  [this](const VariableDescription& f) {
                    this->thermodynamic_forces.emplace_back(f);
                  });
    this->declareMainVariables();
  }  // end of treatThermodynamicForce

  void BehaviourDSLCommon::treatSpeedOfSound() {
    std::function<std::string(const Hypothesis, const std::string&, const bool)>
        m = [this](const Hypothesis h, const std::string& sv, const bool b) {
          return this->standardModifier(h, sv, b);
        };
    this->treatCodeBlock(BehaviourData::ComputeSpeedOfSound, m, true, true);
  }  // end of treatSpeedOfSound

  void BehaviourDSLCommon::treatTangentOperatorBlock() {
    const char* const m = "BehaviourDSLCommon::treatTangentOperatorBlock";
    this->checkNotEndOfFile(m);
    const auto b = this->current->value;
    ++(this->current);
    this->checkNotEndOfFile(m);
    this->readSpecifiedToken(m, ";");
    this->mb.setTangentOperatorBlocks(std::vector<std::string>(1u, b));
  }  // end of treatTangentOperatorBlock

  void BehaviourDSLCommon::treatTangentOperatorBlocks() {
    const char* const m = "BehaviourDSLCommon::treatTangentOperatorBlocks";
    this->checkNotEndOfFile(m);
    auto values = std::vector<tfel::utilities::Token>{};
    this->checkNotEndOfFile(m);
    this->readList(values, m, "{", "}", false);
    this->checkNotEndOfFile(m);
    this->readSpecifiedToken(m, ";");
    auto blocks = std::vector<std::string>{};
    for (const auto& v : values) {
      blocks.push_back(v.value);
    }
    this->mb.setTangentOperatorBlocks(blocks);
  }  // end of treatTangentOperatorBlocks

  void BehaviourDSLCommon::treatAdditionalTangentOperatorBlock() {
    const char* const m =
        "BehaviourDSLCommon::treatAdditionalTangentOperatorBlock";
    this->checkNotEndOfFile(m);
    const auto b = this->current->value;
    ++(this->current);
    this->checkNotEndOfFile(m);
    this->readSpecifiedToken(m, ";");
    this->mb.addTangentOperatorBlock(b);
  }  // end of treatAdditionalTangentOperatorBlock

  void BehaviourDSLCommon::treatAdditionalTangentOperatorBlocks() {
    const char* const m =
        "BehaviourDSLCommon::treatAdditionalTangentOperatorBlocks";
    this->checkNotEndOfFile(m);
    auto values = std::vector<tfel::utilities::Token>{};
    this->checkNotEndOfFile(m);
    this->readList(values, m, "{", "}", false);
    this->checkNotEndOfFile(m);
    this->readSpecifiedToken(m, ";");
    for (const auto& v : values) {
      this->mb.addTangentOperatorBlock(v.value);
    }
  }  // end of treatAdditionalTangentOperatorBlock

  void BehaviourDSLCommon::treatModel() {
    const auto m1 =
        static_cast<void (BehaviourDescription::*)(const ModelDescription&)>(
            &BehaviourDescription::addModelDescription);
    const auto m2 = static_cast<void (BehaviourDescription::*)(
        const BehaviourVariableDescription&)>(
        &BehaviourDescription::addModelDescription);
    this->treatModel("BehaviourDSLCommon::treatModel", m1, m2);
  }  // end of treatModel

  void BehaviourDSLCommon::treatModel2() {
    if (getVerboseMode() >= VERBOSE_DEBUG) {
      getLogStream() << "BehaviourDSLCommon::treatModel2: begin\n";
    }
    this->checkNotEndOfFile("BehaviourDSLCommon::treatModel2",
                            "Unexpected end of file.");
    if (current->flag == tfel::utilities::Token::String) {
      this->treatModel();
    } else {
      this->treatBehaviour();
    }
    if (getVerboseMode() >= VERBOSE_DEBUG) {
      getLogStream() << "BehaviourDSLCommon::treatModel2: end\n";
    }
  }  // end of treatModel2

  void BehaviourDSLCommon::treatUnsupportedCodeBlockOptions(
      const CodeBlockOptions& o) {
    if (o.untreated.empty()) {
      return;
    }
    std::ostringstream msg;
    if (o.untreated.size() == 1u) {
      msg << "option '" << o.untreated[0].value << "' is invalid";
    } else {
      msg << "the";
      for (const auto& opt : o.untreated) {
        msg << " '" << opt.value << "'";
      }
      msg << " options are invalid";
    }
    this->throwRuntimeError(
        "BehaviourDSLCommon::"
        "treatUnsupportedCodeBlockOptions",
        msg.str());
  }  // end of treatUnsupportedCodeBlockOptions

  void BehaviourDSLCommon::addStaticVariableDescription(
      const StaticVariableDescription& v) {
    this->mb.addStaticVariable(ModellingHypothesis::UNDEFINEDHYPOTHESIS, v);
  }  // end of addStaticVariableDescription

  std::map<std::string, int> BehaviourDSLCommon::getIntegerConstants() const {
    return this->mb.getIntegerConstants(
        ModellingHypothesis::UNDEFINEDHYPOTHESIS);
  }

  int BehaviourDSLCommon::getIntegerConstant(const std::string& n) const {
    return this->mb.getIntegerConstant(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                                       n);
  }  // end of getIntegerConstant

  std::set<BehaviourDSLCommon::Hypothesis>
  BehaviourDSLCommon::getDefaultModellingHypotheses() const {
    // see the method documentation
    return {ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN,
            ModellingHypothesis::AXISYMMETRICAL,
            ModellingHypothesis::PLANESTRAIN,
            ModellingHypothesis::GENERALISEDPLANESTRAIN,
            ModellingHypothesis::TRIDIMENSIONAL};
  }  // end of getDefaultModellingHypotheses

  bool BehaviourDSLCommon::isModellingHypothesisSupported(
      const Hypothesis h) const {
    const auto mhs =
        this->getBehaviourDSLDescription().supportedModellingHypotheses;
    return std::find(mhs.cbegin(), mhs.cend(), h) != mhs.end();
  }  // end of isModellingHypothesesSupported

  void BehaviourDSLCommon::analyseFile(
      const std::string& fileName_,
      const std::vector<std::string>& ecmds,
      const std::map<std::string, std::string>& s) {
    this->importFile(fileName_, ecmds, s);
    // Adding some stuff
    this->endsInputFileProcessing();
    this->makeConsistencyChecks();
    // setting the name of the output files
    // targets description
    const auto g = this->getCodeGenerator();
    for (const auto& i : this->interfaces) {
      i.second->getTargetsDescription(this->td, this->mb);
    }
    if (g->isSrcFileRequired()) {
      for (auto& l : this->td.libraries) {
        insert_if(this->td.getLibrary(l.name).sources,
                  this->mb.getSrcFileName());
      }
    }
    insert_if(this->td.headers, this->mb.getBehaviourFileName());
    insert_if(this->td.headers, this->mb.getBehaviourDataFileName());
    insert_if(this->td.headers, this->mb.getIntegrationDataFileName());
    if (this->mb.areSlipSystemsDefined()) {
      insert_if(this->td.headers, this->mb.getSlipSystemHeaderFileName());
      insert_if(this->td.headers,
                this->mb.getSlipSystemImplementationFileName());
    }
    this->completeTargetsDescription();
  }

  void BehaviourDSLCommon::disableVariableDeclaration() {
    if (this->mb.allowsNewUserDefinedVariables()) {
      this->completeVariableDeclaration();
      this->mb.completeVariableDeclaration();
    }
  }  // end of disableVariableDeclaration

  void BehaviourDSLCommon::completeVariableDeclaration() {
    using namespace mfront::bbrick;
    if (getVerboseMode() >= VERBOSE_DEBUG) {
      getLogStream()
          << "BehaviourDSLCommon::completeVariableDeclaration: begin\n";
    }
    if ((!this->gradients.empty()) || (!this->thermodynamic_forces.empty())) {
      this->throwRuntimeError(
          "BehaviourDSLCommon::completeVariableDeclaration",
          "The number of gradients does not match the number of "
          "thermodynamic forces");
    }
    // defining modelling hypotheses
    if (!this->mb.areModellingHypothesesDefined()) {
      auto dmh = this->getDefaultModellingHypotheses();
      // taking into account restrictin du to the `Plate` othotropic
      // axes convention
      if ((this->mb.getSymmetryType() == mfront::ORTHOTROPIC) &&
          (this->mb.getOrthotropicAxesConvention() ==
           OrthotropicAxesConvention::PLATE)) {
        for (auto ph = dmh.begin(); ph != dmh.end();) {
          if ((*ph != ModellingHypothesis::TRIDIMENSIONAL) &&
              (*ph != ModellingHypothesis::PLANESTRESS) &&
              (*ph != ModellingHypothesis::PLANESTRAIN) &&
              (*ph != ModellingHypothesis::GENERALISEDPLANESTRAIN)) {
            ph = dmh.erase(ph);
          } else {
            ++ph;
          }
        }
      }
      this->mb.setModellingHypotheses(dmh);
    }
    // treating bricks
    for (const auto& pb : this->bricks) {
      pb->completeVariableDeclaration();
    }
    //
    const auto& mh = this->mb.getModellingHypotheses();
    if (getVerboseMode() >= VERBOSE_DEBUG) {
      auto& log = getLogStream();
      log << "behaviour '" << this->mb.getClassName()
          << "' supports the following hypotheses: \n";
      for (const auto& h : mh) {
        log << " - " << ModellingHypothesis::toString(h);
        if (this->mb.hasSpecialisedMechanicalData(h)) {
          log << " (specialised)";
        }
        log << '\n';
      }
    }
    if (getVerboseMode() >= VERBOSE_DEBUG) {
      getLogStream()
          << "BehaviourDSLCommon::completeVariableDeclaration: end\n";
    }
  }  // end of completeVariableDeclaration

  void BehaviourDSLCommon::endsInputFileProcessing() {
    if (getVerboseMode() >= VERBOSE_DEBUG) {
      getLogStream() << "BehaviourDSLCommon::endsInputFileProcessing: begin\n";
    }
    //
    this->disableVariableDeclaration();
    for (const auto h : this->mb.getDistinctModellingHypotheses()) {
      const auto& d = this->mb.getBehaviourData(h);
      // restrictions on user defined compute stress free expansion
      if (d.hasCode(BehaviourData::ComputeStressFreeExpansion)) {
        const auto& cb =
            d.getCodeBlock(BehaviourData::ComputeStressFreeExpansion);
        for (const auto& v : cb.members) {
          if (d.isLocalVariableName(v)) {
            this->throwRuntimeError(
                "BehaviourDSLCommon::"
                "endsInputFileProcessing: ",
                "local variables can't be used in "
                "@ComputeStressFreeExpansion blocks "
                "(local variables are not initialized yet "
                "when the stress free expansions "
                "are computed)");
          }
        }
      }
    }
    if (this->mb.areSlipSystemsDefined()) {
      this->mb.appendToIncludes("#include \"TFEL/Material/" +
                                this->mb.getClassName() + "SlipSystems.hxx\"");
    }
    //
    if (this->mb.getBehaviourType() ==
        BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
      const auto mhs = this->getModellingHypothesesToBeTreated();
      for (const auto h :
           {ModellingHypothesis::PLANESTRESS,
            ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS}) {
        if (mhs.find(h) != mhs.end()) {
          if (!this->mb.hasSpecialisedMechanicalData(h)) {
            this->mb.specialize(h);
          }
        }
      }
    }
    //
    for (const auto& pb : this->bricks) {
      pb->endTreatment();
    }
    //
    for (const auto& h : this->mb.getDistinctModellingHypotheses()) {
      if (this->mb.getBehaviourType() ==
          BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
        // all available tangent operators for finite strain behaviours
        for (const auto& t :
             tfel::material::getFiniteStrainBehaviourTangentOperatorFlags()) {
          const auto cto_name =
              std::string(BehaviourData::ComputeTangentOperator) + '-' +
              convertFiniteStrainBehaviourTangentOperatorFlagToString(t);
          if (this->mb.hasCode(h, cto_name)) {
            const auto& cto = this->mb.getCodeBlock(h, cto_name);
            this->checkTangentOperatorBlock(cto_name, cto);
          }
        }
      } else {
        if (this->mb.hasCode(h, BehaviourData::ComputeTangentOperator)) {
          const auto& cto =
              this->mb.getCodeBlock(h, BehaviourData::ComputeTangentOperator);
          this->checkTangentOperatorBlock(BehaviourData::ComputeTangentOperator,
                                          cto);
        }
      }
    }
    //
    if (getVerboseMode() >= VERBOSE_DEBUG) {
      getLogStream() << "BehaviourDSLCommon::endsInputFileProcessing: end\n";
    }
  }  // end of endsInputFileProcessing

  void BehaviourDSLCommon::makeConsistencyChecks() const {
    if (getPedanticMode()) {
      this->doPedanticChecks();
    }
    //
    if (getWarningMode()) {
      for (const auto h : this->mb.getDistinctModellingHypotheses()) {
        reportWarning(checkInitializeMethods(this->mb, h));
      }
    }
  }  // end of makeConsistencyChecks

  /*!
   * \return the "true" integration variables (state variables are excluded)
   * \param[in] md : mechanical behaviour data
   */
  static VarContainer getIntegrationVariables(const BehaviourData& md) {
    const auto& ivs = md.getIntegrationVariables();
    VarContainer v;
    for (const auto& iv : ivs) {
      if (!md.isStateVariableName(iv.name)) {
        v.push_back(iv);
      }
    }
    return v;
  }  // end of getIntegrationVariables

  /*!
   * \brief various checks
   * \param[in] v  : variables
   * \param[in] t  : variable type
   * \param[in] uv : list of all used variables
   * \param[in] b1 : check if the variable is used
   * \param[in] b2 : check if the variable increment (or rate) is used
   * \param[in] b3 : check if external name is declared
   * \param[in] b4 : check if variable is used in more than one code block (test
   * for local variables)
   */
  static void performPedanticChecks(
      const BehaviourData& md,
      const VarContainer& v,
      const std::string& t,
      const std::map<std::string, std::size_t>& uv,
      const bool b1 = true,
      const bool b2 = true,
      const bool b3 = true,
      const bool b4 = false) {
    using namespace tfel::glossary;
    const auto& glossary = Glossary::getGlossary();
    auto& log = getLogStream();
    for (const auto& vd : v) {
      if (b1) {
        const auto p = uv.find(vd.name);
        if (p == uv.end()) {
          log << "- " << t << " '" << vd.name << "' is unused.\n";
        } else {
          if (b4 && p->second == 1) {
            log << "- " << t << " '" << vd.name
                << "' is used in one code block only.\n";
          }
        }
      }
      if (b2) {
        if (uv.find("d" + vd.name) == uv.end()) {
          log << "- " << t << " increment 'd" << vd.name << "' is unused.\n";
        }
      }
      if (b3) {
        if ((!md.hasGlossaryName(vd.name)) && (!md.hasEntryName(vd.name))) {
          log << "- " << t << " '" << vd.name << "' has no external name.\n";
        }
      }
      if (vd.description.empty()) {
        auto hasDoc = false;
        if (md.hasGlossaryName(vd.name)) {
          const auto& e =
              glossary.getGlossaryEntry(md.getExternalName(vd.name));
          hasDoc = (!e.getShortDescription().empty()) ||
                   (!e.getDescription().empty());
        }
        if (!hasDoc) {
          log << "- " << t << " '" << vd.name << "' has no description.\n";
        }
      }
    }
  }

  /*!
   * \brief various checks on static variables
   * \param[in] v  : variables
   * \param[in] uv : list of all static variables
   */
  static void performPedanticChecks(
      const StaticVarContainer& v,
      const std::map<std::string, std::size_t>& uv) {
    auto& log = getLogStream();
    for (const auto& vd : v) {
      if (uv.find(vd.name) == uv.end()) {
        log << "- static variable '" << vd.name << "' is unused.\n";
      }
    }
  }

  void BehaviourDSLCommon::doPedanticChecks() const {
    const auto& hs = this->mb.getDistinctModellingHypotheses();
    auto& log = getLogStream();
    log << "\n* Pedantic checks of " << this->fd.fileName << "\n\n";
    performPedanticChecks(this->fd);
    for (auto h : hs) {
      const auto& md = this->mb.getBehaviourData(h);
      // checks if variables are used
      if (h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
        log << "\n** Beginning pedantic checks for default modelling "
               "hypothesis\n\n";
      } else {
        log << "\n** Beginning pedantic checks for modelling hypothesis '"
            << ModellingHypothesis::toString(h) << "'\n\n";
      }
      // getting all used variables
      const auto& cbs = md.getCodeBlockNames();
      // variable names and counts
      auto members = std::map<std::string, std::size_t>{};
      // static variable names and counts
      auto smembers = std::map<std::string, std::size_t>{};
      for (const auto& cbs_pcbs : cbs) {
        const auto& cb = md.getCodeBlock(cbs_pcbs);
        if (cb.description.empty()) {
          log << "- code block '" << cbs_pcbs << "' has no description\n";
        }
        for (const auto& v : cb.members) {
          if (members.count(v) == 0) {
            members[v] = 1;
          } else {
            ++(members[v]);
          }
        }
        for (const auto& v : cb.staticMembers) {
          if (smembers.count(v) == 0) {
            smembers[v] = 1;
          } else {
            ++(smembers[v]);
          }
        }
      }
      //
      reportWarning(checkInitializeMethods(this->mb, h));
      //
      performPedanticChecks(md, md.getMaterialProperties(), "material property",
                            members, true, false, true);
      const auto& ivs = getIntegrationVariables(md);
      performPedanticChecks(md, ivs, "integration variable", members, false,
                            true, false);
      performPedanticChecks(md, md.getStateVariables(), "state variable",
                            members);
      performPedanticChecks(md, md.getAuxiliaryStateVariables(),
                            "auxiliary state variable", members, true, false);
      performPedanticChecks(md, md.getExternalStateVariables(),
                            "external state variable", members);
      performPedanticChecks(md, md.getLocalVariables(), "local variable",
                            members, true, false, false, true);
      performPedanticChecks(md, md.getParameters(), "parameter", members, true,
                            false);
      performPedanticChecks(md.getStaticVariables(), smembers);
    }
    log << "\n# End of pedantic checks\n";
  }  // end of pedanticChecks

  std::set<BehaviourDSLCommon::Hypothesis>
  BehaviourDSLCommon::getModellingHypothesesToBeTreated() const {
    // modelling hypotheses handled by the interfaces (if at least one
    // interface is defined), or by the behaviour
    std::set<Hypothesis> mhs;
    if (this->interfaces.empty()) {
      const auto& bh = this->mb.getModellingHypotheses();
      mhs.insert(bh.begin(), bh.end());
    } else {
      // calling the interfaces
      for (const auto& i : this->interfaces) {
        const auto& ih = i.second->getModellingHypothesesToBeTreated(this->mb);
        mhs.insert(ih.begin(), ih.end());
      }
    }
    return mhs;
  }  // end of getModellingHypothesesToBeTreated

  void BehaviourDSLCommon::generateOutputFiles() {
    for (const auto& em : this->mb.getExternalMFrontFiles()) {
      this->callMFront({em.first}, std::get<0>(em.second),
                       std::get<1>(em.second));
    }
    const auto g = this->getCodeGenerator();
    g->generateOutputFiles(this->getModellingHypothesesToBeTreated());
  }  // end of generateOutputFiles

  void BehaviourDSLCommon::
      declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution(
          const Hypothesis h, const std::string& n) {
    if (!this->explicitlyDeclaredUsableInPurelyImplicitResolution) {
      this->mb.setUsableInPurelyImplicitResolution(h, false);
    }
    this->mb
        .declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution(
            h, n);
  }  // end of
     // declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution

  std::string BehaviourDSLCommon::standardModifier(const Hypothesis h,
                                                   const std::string& var,
                                                   const bool addThisPtr) {
    if ((this->mb.isExternalStateVariableIncrementName(h, var)) ||
        (var == "dT")) {
      this->declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution(
          h, var.substr(1));
    }
    if (addThisPtr) {
      return "this->" + var;
    }
    return var;
  }  // end of standardModifier

  std::string BehaviourDSLCommon::tangentOperatorVariableModifier(
      const Hypothesis h, const std::string& var, const bool addThisPtr) {
    return this->standardModifier(h, var, addThisPtr);
  }  // end of tangentOperatorVariableModifier

  void BehaviourDSLCommon::treatStrainMeasure() {
    this->checkNotEndOfFile("BehaviourDSLCommon::treatStrainMeasure",
                            "Expected strain measure name.");
    const auto fs = this->current->value;
    if ((fs != "Hencky") && (fs != "GreenLagrange") && (fs != "Linearised") &&
        (fs != "Linearized")) {
      this->throwRuntimeError("BehaviourDSLCommon::treatStrainMeasure",
                              "unsupported strain measure '" + fs + "'");
    }
    ++(this->current);
    this->checkNotEndOfFile("BehaviourDSLCommon::treatStrainMeasure",
                            "Expected ';' or options.");
    const auto opts = [this] {
      if (this->current->value == "{") {
        return read<tfel::utilities::DataMap>(this->current,
                                              this->tokens.end());
      }
      return tfel::utilities::DataMap{};
    }();
    this->readSpecifiedToken("BehaviourDSLCommon::treatStrainMeasure", ";");
    for (const auto& o : opts) {
      if ((o.first != "save_strain") && (o.first != "save_stress")) {
        this->throwRuntimeError("BehaviourDSLCommon::treatStrainMeasure",
                                "invalid option '" + o.first + "'");
      }
      if (!o.second.is<bool>()) {
        this->throwRuntimeError("BehaviourDSLCommon::treatStrainMeasure",
                                "invalid type for option '" + o.first +
                                    "', expected a boolean value");
      }
    }
    if (fs == "Hencky") {
      this->mb.setStrainMeasure(BehaviourDescription::HENCKY);
    } else if (fs == "GreenLagrange") {
      this->mb.setStrainMeasure(BehaviourDescription::GREENLAGRANGE);
    } else if ((fs == "Linearised") || (fs == "Linearized")) {
      this->mb.setStrainMeasure(BehaviourDescription::LINEARISED);
    } else {
      this->throwRuntimeError("BehaviourDSLCommon::treatStrainMeasure",
                              "unsupported strain measure '" + fs + "'");
    }
    if (opts.count("save_strain") != 0) {
      this->mb.setSaveStrainMeasure(
          opts.find("save_strain")->second.get<bool>());
    }
    if (opts.count("save_stress") != 0) {
      this->mb.setSaveDualStress(opts.find("save_stress")->second.get<bool>());
    }
  }  // end of treatStrainMeasure

  void BehaviourDSLCommon::treatPostProcessing() {
    auto hs = std::set<Hypothesis>{};
    this->readHypothesesList(hs);
    this->checkNotEndOfFile("BehaviourDSLCommon::treatPostProcessing");
    const auto pname = this->current->value;
    if (!this->isValidIdentifier(pname)) {
      this->throwRuntimeError(
          "BehaviourDSLCommon::treatPostProcessing",
          "invalid post-processing name (read '" + pname + "').");
    }
    ++(this->current);
    this->checkNotEndOfFile("BehaviourDSLCommon::treatPostProcessing");
    const auto beg = this->current;
    for (const auto& h : hs) {
      const auto& d = this->mb.getBehaviourData(h);
      const auto& pvariables = d.getPostProcessingVariables();
      auto used_post_processing_variables = std::vector<VariableDescription>{};
      this->current = beg;
      CodeBlockParserOptions o;
      o.member_names = d.getRegistredMembersNames();
      o.static_member_names = d.getRegistredStaticMembersNames();
      o.qualifyStaticVariables = true;
      o.qualifyMemberVariables = true;
      o.analyser = std::make_shared<StandardWordAnalyser>(
          h, [&used_post_processing_variables, &pvariables](
                 CodeBlock&, const Hypothesis, const std::string& w) {
            if (pvariables.contains(w)) {
              const auto p = std::find_if(
                  used_post_processing_variables.begin(),
                  used_post_processing_variables.end(),
                  [&w](const VariableDescription& v) { return v.name == w; });
              if (p == used_post_processing_variables.end()) {
                used_post_processing_variables.push_back(
                    pvariables.getVariable(w));
              }
            }
          });
      o.modifier = std::make_shared<StandardVariableModifier>(
          h, [this](const Hypothesis hv, const std::string& v, const bool b) {
            return this->standardModifier(hv, v, b);
          });
      auto c = this->readNextBlock(o);
      c.attributes[CodeBlock::used_postprocessing_variables] =
          used_post_processing_variables;
      this->mb.addPostProcessing(h, pname, c);
      this->mb.registerMemberName(h, "execute" + pname + "PostProcessing");
    }
  }  // end of treatPostProcessing

  void BehaviourDSLCommon::treatInitializeFunction() {
    auto hs = std::set<Hypothesis>{};
    this->readHypothesesList(hs);
    this->checkNotEndOfFile("BehaviourDSLCommon::treatInitializeFunction");
    const auto iname = this->current->value;
    if (!this->isValidIdentifier(iname)) {
      this->throwRuntimeError(
          "BehaviourDSLCommon::treatInitializeFunction",
          "invalid post-processing name (read '" + iname + "').");
    }
    ++(this->current);
    this->checkNotEndOfFile("BehaviourDSLCommon::treatInitializeFunction");
    const auto beg = this->current;
    for (const auto& h : hs) {
      const auto& d = this->mb.getBehaviourData(h);
      const auto& pvariables = d.getInitializeFunctionVariables();
      auto used_initialize_function_variables =
          std::vector<VariableDescription>{};
      this->current = beg;
      CodeBlockParserOptions o;
      o.member_names = d.getRegistredMembersNames();
      o.static_member_names = d.getRegistredStaticMembersNames();
      o.qualifyStaticVariables = true;
      o.qualifyMemberVariables = true;
      o.analyser = std::make_shared<StandardWordAnalyser>(
          h, [&used_initialize_function_variables, &pvariables](
                 CodeBlock&, const Hypothesis, const std::string& w) {
            if (pvariables.contains(w)) {
              const auto p = std::find_if(
                  used_initialize_function_variables.begin(),
                  used_initialize_function_variables.end(),
                  [&w](const VariableDescription& v) { return v.name == w; });
              if (p == used_initialize_function_variables.end()) {
                used_initialize_function_variables.push_back(
                    pvariables.getVariable(w));
              }
            }
          });
      o.modifier = std::make_shared<StandardVariableModifier>(
          h, [this](const Hypothesis hv, const std::string& v, const bool b) {
            return this->standardModifier(hv, v, b);
          });
      auto c = this->readNextBlock(o);
      c.attributes[CodeBlock::used_initialize_function_variables] =
          used_initialize_function_variables;
      this->mb.addInitializeFunction(h, iname, c);
      this->mb.registerMemberName(h, "execute" + iname + "InitializeFunction");
    }
  }  // end of treatInitializeFunction

  void BehaviourDSLCommon::treatPrivate() {
    auto hs = std::set<Hypothesis>{};
    this->readHypothesesList(hs);
    const auto beg = this->current;
    for (const auto& h : hs) {
      const auto& d = this->mb.getBehaviourData(h);
      this->current = beg;
      CodeBlockParserOptions o;
      o.member_names = d.getRegistredMembersNames();
      o.static_member_names = d.getRegistredStaticMembersNames();
      o.qualifyStaticVariables = true;
      o.qualifyMemberVariables = true;
      o.modifier = std::make_shared<StandardVariableModifier>(
          h, [this](const Hypothesis hv, const std::string& v, const bool b) {
            return this->standardModifier(hv, v, b);
          });
      this->mb.appendToPrivateCode(h, this->readNextBlock(o).code, true);
    }
  }  // end of treatPrivate

  void BehaviourDSLCommon::treatMembers() {
    auto hs = std::set<Hypothesis>{};
    this->readHypothesesList(hs);
    const auto beg = this->current;
    for (const auto& h : hs) {
      const auto& d = this->mb.getBehaviourData(h);
      this->current = beg;
      CodeBlockParserOptions o;
      o.member_names = d.getRegistredMembersNames();
      o.static_member_names = d.getRegistredStaticMembersNames();
      o.qualifyStaticVariables = true;
      o.qualifyMemberVariables = true;
      o.modifier = std::make_shared<StandardVariableModifier>(
          h, [this](const Hypothesis hv, const std::string& v, const bool b) {
            return this->standardModifier(hv, v, b);
          });
      this->mb.appendToMembers(h, this->readNextBlock(o).code, true);
    }
  }  // end of treatMembers

  void BehaviourDSLCommon::treatBrick() {
    using Parameters = AbstractBehaviourBrick::Parameters;
    auto& f = BehaviourBrickFactory::getFactory();
    auto parameters = Parameters{};
    this->checkNotEndOfFile("BehaviourDSLCommon::treatBrick",
                            "Expected brick name or '<'.");
    if (this->current->value == "<") {
      auto options = std::vector<tfel::utilities::Token>{};
      this->readList(options, "BehaviourDSLCommon::treatBrick", "<", ">", true);
      for (const auto& o : options) {
        const auto pos = o.value.find('=');
        if (pos != std::string::npos) {
          if (pos == 0) {
            this->throwRuntimeError("BehaviourDSLCommon::treatBrick",
                                    "no parameter name given");
          }
          // extracting the name
          const auto& n = o.value.substr(0, pos);
          if (pos == o.value.size()) {
            this->throwRuntimeError("BehaviourDSLCommon::treatBrick",
                                    "no option given to the "
                                    "parameter '" +
                                        n + "'");
          }
          // extracting the option
          parameters.insert({n, o.value.substr(pos + 1)});
        } else {
          parameters.insert({o.value, ""});
        }
      }
    }
    this->checkNotEndOfFile("BehaviourDSLCommon::treatBrick",
                            "Expected brick name.");
    const auto b = [this]() -> std::string {
      if (this->current->flag == tfel::utilities::Token::String) {
        return this->readString("BehaviourDSLCommon::treatBrick");
      }
      const auto r = this->current->value;
      ++(this->current);
      return r;
    }();
    const auto d = [this] {
      using namespace tfel::utilities;
      if ((this->current != this->tokens.end()) &&
          (this->current->value == "{")) {
        DataParsingOptions o;
        o.allowMultipleKeysInMap = true;
        const auto data = Data::read(this->current, this->tokens.end(), o);
        if (data.empty()) {
          return DataMap{};
        }
        if (!data.is<DataMap>()) {
          this->throwRuntimeError("BehaviourDSLCommon::treatBrick",
                                  "expected to read a map");
        }
        return data.get<DataMap>();
      }
      return DataMap{};
    }();
    const auto br = f.get(b, *this, this->mb);
    br->initialize(parameters, d);
    this->readSpecifiedToken("BehaviourDSLCommon::treatBrick", ";");
    this->bricks.push_back(std::move(br));
  }  // end of treatBrick

  void BehaviourDSLCommon::treatTangentOperator() {
    using namespace tfel::material;
    using namespace tfel::utilities;
    if (this->mb.getTangentOperatorBlocks().empty()) {
      this->throwRuntimeError("BehaviourDSLCommon::treatTangentOperator",
                              "no tangent operator block defined");
    }
    CodeBlockOptions o;
    this->readCodeBlockOptions(o, true);
    if (this->mb.getBehaviourType() ==
        BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
      auto po = o.untreated.begin();
      const auto poe = o.untreated.end();
      auto ktype = std::string{};
      while (po != poe) {
        const auto& opt = *po;
        if (opt.flag != Token::Standard) {
          continue;
        }
        for (const auto& to : getFiniteStrainBehaviourTangentOperatorFlags()) {
          if (opt.value ==
              convertFiniteStrainBehaviourTangentOperatorFlagToString(to)) {
            ktype = opt.value;
            break;
          }
        }
        if (!ktype.empty()) {
          o.untreated.erase(po);
          break;
        }
        ++po;
      }
      if (ktype.empty()) {
        std::ostringstream msg;
        msg << "Undefined tangent operator type '" + ktype +
                   "'. Valid tangent operator type are :\n";
        for (const auto& to : getFiniteStrainBehaviourTangentOperatorFlags()) {
          msg << "- "
              << convertFiniteStrainBehaviourTangentOperatorFlagToString(to)
              << " : " << getFiniteStrainBehaviourTangentOperatorDescription(to)
              << '\n';
        }
        this->throwRuntimeError("BehaviourDSLCommon::treatTangentOperator",
                                msg.str());
      }
      this->readTangentOperatorCodeBlock(
          o, std::string(BehaviourData::ComputeTangentOperator) + "-" + ktype);
      for (const auto& h : o.hypotheses) {
        if (!this->mb.hasAttribute(
                h, BehaviourData::hasConsistentTangentOperator)) {
          this->mb.setAttribute(h, BehaviourData::hasConsistentTangentOperator,
                                true);
        }
      }
    } else {
      this->readTangentOperatorCodeBlock(o,
                                         BehaviourData::ComputeTangentOperator);
      for (const auto& h : o.hypotheses) {
        this->mb.setAttribute(h, BehaviourData::hasConsistentTangentOperator,
                              true);
      }
    }
  }  // end of treatTangentOperator

  void BehaviourDSLCommon::readTangentOperatorCodeBlock(
      const CodeBlockOptions& o, const std::string& n) {
    std::function<std::string(const Hypothesis, const std::string&, const bool)>
        m = [this](const Hypothesis h, const std::string& sv, const bool b) {
          return this->tangentOperatorVariableModifier(h, sv, b);
        };
    this->treatUnsupportedCodeBlockOptions(o);
    this->treatCodeBlock(o, n, m, true);
  }  // end of readTangentOperatorCodeBlock

  void BehaviourDSLCommon::treatIsTangentOperatorSymmetric() {
    auto hs = std::set<Hypothesis>{};
    this->readHypothesesList(hs);
    this->checkNotEndOfFile(
        "BehaviourDSLCommon::treatIsTangentOperatorSymmetric : ",
        "Expected 'true' or 'false'.");
    auto b = this->readBooleanValue(
        "BehaviourDSLCommon::treatIsTangentOperatorSymmetric");
    this->readSpecifiedToken(
        "BehaviourDSLCommon::treatIsTangentOperatorSymmetric", ";");
    for (const auto& h : hs) {
      this->mb.setAttribute(
          h, BehaviourData::isConsistentTangentOperatorSymmetric, b);
    }
  }  // end of treatTangentOperator

  void BehaviourDSLCommon::treatLibrary() {
    const auto& l = this->readOnlyOneToken();
    if (!isValidLibraryName(l)) {
      this->throwRuntimeError("BehaviourDSLCommon::treatLibrary",
                              "invalid library name '" + l + "'");
    }
    this->mb.setLibrary(l);
  }  // end of treatLibrary

  void BehaviourDSLCommon::treatComputeThermalExpansion() {
    if (this->mb.getAttribute<bool>(
            BehaviourDescription::requiresThermalExpansionCoefficientTensor,
            false)) {
      tfel::raise(
          "BehaviourDSLCommon::treatComputeThermalExpansion: "
          "@ComputeThermalExpansion can be used along with "
          "@RequireThermalExpansionCoefficientTensor");
    }
    const auto new_syntax = [this] {
      auto c = this->current;
      if (c == this->tokens.end()) {
        return false;
      }
      if (c->value != "{") {
        return false;
      }
      // we have to choose between the new syntax and the historical one
      ++c;
      if (c == this->tokens.end()) {
        return false;
      }
      ++c;
      if (c == this->tokens.end()) {
        return false;
      }
      return c->value == ":";
    }();
    if (new_syntax) {
      this->treatComputeThermalExpansionSecondSyntax();
    } else {
      this->treatComputeThermalExpansionFirstSyntax();
    }
  }  // end of treatComputeThermalExpansion

  void BehaviourDSLCommon::treatComputeThermalExpansionFirstSyntax() {
    using namespace tfel::utilities;
    using ExternalMFrontMaterialProperty =
        BehaviourDescription::ExternalMFrontMaterialProperty;
    const std::string m("BehaviourDSLCommon::treatComputeThermalExpansion");
    const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    const auto n = "thermal_expansion_reference_temperature";
    auto throw_if = [this, m](const bool b, const std::string& msg) {
      if (b) {
        this->throwRuntimeError(m, msg);
      }
    };
    auto addTref = [this, throw_if, n](const double v) {
      if (this->mb.hasParameter(h, n)) {
        const auto Tref = this->mb.getFloattingPointParameterDefaultValue(h, n);
        throw_if(tfel::math::ieee754::fpclassify(Tref - v) != FP_ZERO,
                 "inconsistent reference temperature");
      } else {
        VariableDescription Tref("temperature", n, 1u, 0u);
        Tref.description =
            "value of the reference temperature for "
            "the computation of the thermal expansion";
        this->mb.addParameter(h, Tref, BehaviourData::ALREADYREGISTRED);
        this->mb.setParameterDefaultValue(h, n, v);
        this->mb.setEntryName(h, n, "ThermalExpansionReferenceTemperature");
      }
    };
    auto addTi = [this](const double v) {
      VariableDescription Ti("temperature",
                             "initial_geometry_reference_temperature", 1u, 0u);
      Ti.description =
          "value of the temperature when the initial geometry was measured";
      this->mb.addParameter(h, Ti, BehaviourData::ALREADYREGISTRED);
      this->mb.setParameterDefaultValue(
          h, "initial_geometry_reference_temperature", v);
      this->mb.setEntryName(h, "initial_geometry_reference_temperature",
                            "ReferenceTemperatureForInitialGeometry");
    };  // end of addTi
    const auto& acs = this->readMaterialPropertyOrArrayOfMaterialProperties(m);
    this->checkNotEndOfFile(m);
    if (this->current->value == "{") {
      const auto data = read<DataMap>(this->current, this->tokens.end());
      throw_if(data.size() != 1u,
               "invalid number of data. "
               "Only the 'reference_temperature' is expected");
      const auto pd = data.begin();
      throw_if((pd->first != "reference_temperature") && (pd->first != n),
               "the only data expected is "
               "'reference_temperature' (read '" +
                   pd->first + "')");
      throw_if(!pd->second.is<double>(),
               "invalid type for data 'reference_temperature'");
      addTref(pd->second.get<double>());
    }
    this->readSpecifiedToken(m, ";");
    throw_if((acs.size() != 1u) && (acs.size() != 3u),
             "invalid number of file names given");
    if (acs.size() == 3u) {
      // the material shall have been declared orthotropic
      throw_if(this->mb.getSymmetryType() != mfront::ORTHOTROPIC,
               "the mechanical behaviour must be orthotropic "
               "to give more than one thermal expansion coefficient.");
    }
    for (const auto& a : acs) {
      if (a.is<ExternalMFrontMaterialProperty>()) {
        const auto& mpd = *(a.get<ExternalMFrontMaterialProperty>().mpd);
        if (mpd.staticVars.contains("ReferenceTemperature")) {
          const auto Tref = mpd.staticVars.get("ReferenceTemperature");
          addTref(Tref.value);
        } else {
          if (getVerboseMode() != VERBOSE_QUIET) {
            auto& os = getLogStream();
            os << "no reference temperature in material property '";
            if (mpd.material.empty()) {
              os << mpd.material << '_';
            }
            os << mpd.law << "'\n";
          }
        }
      }
    }
    if (acs.size() == 1u) {
      this->mb.setThermalExpansionCoefficient(acs.front());
    } else {
      this->mb.setThermalExpansionCoefficients(acs[0], acs[1], acs[2]);
    }
    if (!this->mb.hasParameter(h, n)) {
      addTref(293.15);
    }
    if (!this->mb.hasParameter(h, "initial_geometry_reference_temperature")) {
      addTi(293.15);
    }
  }  // end of treatComputeThermalExpansionFirstSyntaxt

  void BehaviourDSLCommon::treatComputeThermalExpansionSecondSyntax() {
    const auto opts = [this] {
      auto o = tfel::utilities::DataParsingOptions{};
      o.allowMultipleKeysInMap = true;
      const auto data =
          tfel::utilities::Data::read(this->current, this->tokens.end(), o);
      if (data.empty()) {
        return tfel::utilities::DataMap{};
      }
      if (!data.is<tfel::utilities::DataMap>()) {
        this->throwRuntimeError(
            "IsotropicBehaviourDSLBase::treatIsotropicHardeningRules",
            "expected to read a map");
      }
      return data.get<tfel::utilities::DataMap>();
    }();
    tfel::utilities::check_keys(
        opts,
        {"thermal_expansion", "thermal_expansion1", "thermal_expansion2",
         "thermal_expansion3", "thermal_expansion_reference_temperature",
         "reference_temperature", "initial_geometry_reference_temperature",
         "save_thermal_expansion"});
    addThermalExpansionCoefficientsIfDefined(*this, this->mb, opts, true);
  }

  void BehaviourDSLCommon::treatElasticMaterialProperties() {
    if (this->mb.getAttribute<bool>(
            BehaviourDescription::requiresStiffnessTensor, false)) {
      this->throwRuntimeError(
          "BehaviourDSLCommon::treatElasticMaterialProperties",
          "@ElasticMaterialProperties can not be used along with "
          "@RequireStiffnessTensor");
    }
    this->readElasticMaterialProperties();
  }

  BehaviourDescription::MaterialProperty
  BehaviourDSLCommon::extractMaterialProperty(const std::string& m,
                                              const tfel::utilities::Token& t) {
    if (t.flag == tfel::utilities::Token::String) {
      // file name of formula
      const auto f = t.value.substr(1, t.value.size() - 2);
      if (tfel::utilities::ends_with(f, ".mfront")) {
        // file name
        BehaviourDescription::ExternalMFrontMaterialProperty mp;
        mp.mpd = this->handleMaterialPropertyDescription(f);
        return mp;
      } else {
        BehaviourDescription::AnalyticMaterialProperty mp;
        mp.f = f;
        return mp;
      }
    }
    BehaviourDescription::ConstantMaterialProperty mp;
    try {
      mp.value = std::stold(t.value);
    } catch (std::exception& e) {
      this->throwRuntimeError(m, "can't convert token '" + t.value +
                                     "' to long double "
                                     "(" +
                                     std::string(e.what()) + ")");
    }
    return mp;
  }

  std::vector<BehaviourDescription::MaterialProperty>
  BehaviourDSLCommon::readMaterialPropertyOrArrayOfMaterialProperties(
      const std::string& m) {
    auto mps = std::vector<BehaviourDescription::MaterialProperty>{};
    this->checkNotEndOfFile(m);
    if (this->current->value == "{") {
      auto mpv = std::vector<tfel::utilities::Token>{};
      this->readList(mpv, m, "{", "}", false);
      for (const auto& t : mpv) {
        mps.push_back(this->extractMaterialProperty(m, t));
      }
    } else {
      mps.push_back(this->extractMaterialProperty(m, *(this->current)));
      ++(this->current);
    }
    return mps;
  }

  std::vector<BehaviourDescription::MaterialProperty>
  BehaviourDSLCommon::readElasticMaterialPropertiesI() {
    auto p = this->current;
    const auto pe = this->tokens.end();
    CxxTokenizer::checkNotEndOfLine(
        "BehaviourDSLCommon::readElasticMaterialPropertiesI", p, pe);
    if (p->value != "{") {
      this->throwRuntimeError(
          "BehaviourDSLCommon::readElasticMaterialPropertiesI", "expected '{'");
    }
    ++p;
    // vector or map
    CxxTokenizer::checkNotEndOfLine(
        "BehaviourDSLCommon::readElasticMaterialPropertiesI", p, pe);
    if (p->value == "}") {
      ++p;
      this->throwRuntimeError(
          "BehaviourDSLCommon::readElasticMaterialPropertiesI",
          "no material property given");
    }
    // map or string vector
    ++p;
    CxxTokenizer::checkNotEndOfLine(
        "BehaviourDSLCommon::readElasticMaterialPropertiesI", p, pe);
    if (p->value == ":") {
      p = std::prev(p, 2);
      return this->readElasticMaterialPropertiesII();
    }
    return this->readMaterialPropertyOrArrayOfMaterialProperties(
        "BehaviourDSLCommon::readElasticMaterialProperties");
  }  // end of readElasticMaterialPropertiesI

  std::vector<BehaviourDescription::MaterialProperty>
  BehaviourDSLCommon::readElasticMaterialPropertiesII() {
    const auto d =
        tfel::utilities::Data::read_map(this->current, this->tokens.end())
            .get<tfel::utilities::DataMap>();
    if ((d.size() != 2u) && (d.size() != 9u)) {
      this->throwRuntimeError(
          "BehaviourDSLCommon::readElasticMaterialProperties",
          "invalid number of elastic material properties given");
    }
    auto get_mp = [this, &d](const char* const n) {
      if (d.count(n) == 0) {
        this->throwRuntimeError(
            "BehaviourDSLCommon::readElasticMaterialPropertiesII",
            "material property '" + std::string(n) + "' is not defined");
      }
      return getBehaviourDescriptionMaterialProperty(*this, n, d.at(n));
    };
    if (d.size() == 2u) {
      return {get_mp("young_modulus"), get_mp("poisson_ratio")};
    }
    return {get_mp("young_modulus1"),  get_mp("young_modulus2"),
            get_mp("young_modulus3"),  get_mp("poisson_ratio12"),
            get_mp("poisson_ratio23"), get_mp("poisson_ratio13"),
            get_mp("shear_modulus12"), get_mp("shear_modulus23"),
            get_mp("shear_modulus13")};
  }  // end of readElasticMaterialPropertiesII

  void BehaviourDSLCommon::readElasticMaterialProperties() {
    const auto emps = readElasticMaterialPropertiesI();
    this->readSpecifiedToken(
        "BehaviourDSLCommon::readElasticMaterialProperties", ";");
    if ((emps.size() != 2u) && (emps.size() != 9u)) {
      this->throwRuntimeError(
          "BehaviourDSLCommon::readElasticMaterialProperties",
          "invalid number of elastic material properties given");
    }
    if (emps.size() == 9u) {
      // the material shall have been declared orthotropic
      if (this->mb.getSymmetryType() != mfront::ORTHOTROPIC) {
        this->throwRuntimeError(
            "BehaviourDSLCommon::readElasticMaterialProperties",
            "the mechanical behaviour must be orthotropic to give more than "
            "two elastic material properties.");
      }
      setElasticSymmetryType(this->mb, mfront::ORTHOTROPIC);
    } else {
      setElasticSymmetryType(this->mb, mfront::ISOTROPIC);
    }
    this->mb.setElasticMaterialProperties(emps);
  }

  void BehaviourDSLCommon::treatComputeStiffnessTensor() {
    if (this->mb.getAttribute<bool>(
            BehaviourDescription::requiresStiffnessTensor, false)) {
      this->throwRuntimeError("BehaviourDSLCommon::treatComputeStiffnessTensor",
                              "@ComputeStiffnessTensor can be used along with "
                              "@RequireStiffnessTensor");
    }
    if (this->current->value == "<") {
      this->treatStiffnessTensorOption();
    }
    this->readElasticMaterialProperties();
    this->mb.setAttribute(BehaviourDescription::computesStiffnessTensor, true,
                          false);
  }  // end of treatComputeStiffnessTensor

  std::vector<BehaviourDescription::MaterialProperty>
  BehaviourDSLCommon::readHillTensorDefinitionI() {
    auto p = this->current;
    const auto pe = this->tokens.end();
    CxxTokenizer::checkNotEndOfLine(
        "BehaviourDSLCommon::readHillTensorDefinitionI", p, pe);
    if (p->value != "{") {
      this->throwRuntimeError("BehaviourDSLCommon::readHillTensorDefinitionI",
                              "expected '{'");
    }
    ++p;
    // vector or map
    CxxTokenizer::checkNotEndOfLine(
        "BehaviourDSLCommon::readHillTensorDefinitionI", p, pe);
    if (p->value == "}") {
      ++p;
      this->throwRuntimeError("BehaviourDSLCommon::readHillTensorDefinitionI",
                              "no material property given");
    }
    // map or string vector
    ++p;
    CxxTokenizer::checkNotEndOfLine(
        "BehaviourDSLCommon::readHillTensorDefinitionI", p, pe);
    if (p->value == ":") {
      p = std::prev(p, 2);
      return this->readHillTensorDefinitionII();
    }
    return this->readMaterialPropertyOrArrayOfMaterialProperties(
        "BehaviourDSLCommon::readHillTensorDefinition");
  }  // end of readHillTensorDefinitionI

  std::vector<BehaviourDescription::MaterialProperty>
  BehaviourDSLCommon::readHillTensorDefinitionII() {
    const auto d =
        tfel::utilities::Data::read_map(this->current, this->tokens.end())
            .get<tfel::utilities::DataMap>();
    if (d.size() != 6u) {
      this->throwRuntimeError(
          "BehaviourDSLCommon::readHillTensorDefinition",
          "invalid number of material properties given (6 expected)");
    }
    auto get_mp = [this, &d](const char* const n) {
      if (d.count(n) == 0) {
        this->throwRuntimeError(
            "BehaviourDSLCommon::readHillTensorDefinitionII",
            "material property '" + std::string(n) + "' is not defined");
      }
      return getBehaviourDescriptionMaterialProperty(*this, n, d.at(n));
    };
    return {get_mp("F"), get_mp("G"), get_mp("H"),
            get_mp("L"), get_mp("M"), get_mp("N")};
  }  // end of readHillTensorDefinitionII

  void BehaviourDSLCommon::treatHillTensor() {
    if (this->mb.getSymmetryType() != mfront::ORTHOTROPIC) {
      this->throwRuntimeError(
          "BehaviourDSLCommon::treatHillTensor",
          "the mechanical behaviour must be orthotropic to define "
          "a Hill tensor.");
    }
    this->checkNotEndOfFile("BehaviourDSLCommon::treatModellingHypothesis");
    // variable name
    if (!this->isValidIdentifier(this->current->value)) {
      this->throwRuntimeError("BehaviourDSLCommon::treatHillTensor: ",
                              "variable name is not valid "
                              "(read '" +
                                  this->current->value + "').");
    }
    auto v = VariableDescription{"tfel::math::st2tost2<N,real>",
                                 this->current->value, 1u, this->current->line};
    v.description = "Hill tensor";
    ++(this->current);
    // Hill coefficients
    const auto& hcs = this->readHillTensorDefinitionI();
    this->readSpecifiedToken("BehaviourDSLCommon::treatHillTensor", ";");
    if (hcs.size() != 6u) {
      this->throwRuntimeError("BehaviourDSLCommon::treatHillTensor",
                              "invalid number of hill coefficients");
    }
    this->mb.addHillTensor(v, hcs);
  }  // end of treatHillTensor

  void BehaviourDSLCommon::treatModellingHypothesis() {
    this->checkNotEndOfFile("BehaviourDSLCommon::treatModellingHypothesis");
    const auto h = ModellingHypothesis::fromString(this->current->value);
    ++(this->current);
    this->checkNotEndOfFile("BehaviourDSLCommon::treatModellingHypothesis");
    this->readSpecifiedToken("BehaviourDSLCommon::treatModellingHypothesis",
                             ";");
    if (!this->isModellingHypothesisSupported(h)) {
      this->throwRuntimeError("BehaviourDSLCommon::treatModellingHypothesis",
                              "unsupported modelling hypothesis '" +
                                  ModellingHypothesis::toString(h) + "'");
    }
    std::set<Hypothesis> hypotheses;
    hypotheses.insert(h);
    this->mb.setModellingHypotheses(hypotheses);
  }  // end of treatModellingHypothesis

  void BehaviourDSLCommon::treatModellingHypotheses() {
    using namespace tfel::utilities;
    auto hypotheses = std::set<Hypothesis>{};
    auto values = std::vector<Token>{};
    this->checkNotEndOfFile("BehaviourDSLCommon::treatModellingHypotheses");
    this->readList(values, "BehaviourDSLCommon::treatModellingHypotheses", "{",
                   "}", false);
    this->checkNotEndOfFile("BehaviourDSLCommon::treatModellingHypotheses");
    this->readSpecifiedToken("BehaviourDSLCommon::treatModellingHypotheses",
                             ";");
    for (const auto& v : values) {
      if (v.flag == Token::String) {
        this->appendToHypothesesList(hypotheses,
                                     v.value.substr(1, v.value.size() - 2));
      } else {
        this->appendToHypothesesList(hypotheses, v.value);
      }
    }
    if (hypotheses.empty()) {
      this->throwRuntimeError("BehaviourDSLCommon::treatModellingHypotheses",
                              "no hypothesis declared");
    }
    this->mb.setModellingHypotheses(hypotheses);
  }  // end of treatModellingHypotheses

  void BehaviourDSLCommon::treatUsableInPurelyImplicitResolution() {
    const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    this->readSpecifiedToken(
        "BehaviourDSLCommon::treatUsableInPurelyImplicitResolution", ";");
    if (this->explicitlyDeclaredUsableInPurelyImplicitResolution) {
      this->throwRuntimeError(
          "BehaviourDSLCommon::treatUsableInPurelyImplicitResolution",
          "keyword '@UsableInPurelyImplicitResolution' already called");
    }
    this->explicitlyDeclaredUsableInPurelyImplicitResolution = true;
    this->mb.setUsableInPurelyImplicitResolution(h, true);
  }  // end of treatUsableInPurelyImplicitResolution

  void BehaviourDSLCommon::treatParameterMethod(const Hypothesis h) {
    using namespace tfel::utilities;
    const auto n = tfel::unicode::getMangledString(this->current->value);
    ++(this->current);
    this->checkNotEndOfFile("BehaviourDSLCommon::treatParameterMethod");
    this->readSpecifiedToken("BehaviourDSLCommon::treatParameterMethod", ".");
    this->checkNotEndOfFile("BehaviourDSLCommon::treatParameterMethod");
    if (this->current->value == "setDefaultValue") {
      ++(this->current);
      this->checkNotEndOfFile("BehaviourDSLCommon::treatParameterMethod");
      this->readSpecifiedToken("BehaviourDSLCommon::treatParameterMethod", "(");
      this->checkNotEndOfFile("BehaviourDSLCommon::treatParameterMethod");
      double value = tfel::utilities::convert<double>(this->current->value);
      ++(this->current);
      this->checkNotEndOfFile("BehaviourDSLCommon::treatParameterMethod");
      this->readSpecifiedToken("BehaviourDSLCommon::treatParameterMethod", ")");
      this->checkNotEndOfFile("BehaviourDSLCommon::treatParameterMethod");
      this->readSpecifiedToken("BehaviourDSLCommon::treatParameterMethod", ";");
      this->mb.setParameterDefaultValue(h, n, value);
    } else {
      --(this->current);
      --(this->current);
      this->treatVariableMethod(h);
    }
  }  // end of treatParameterMethod

  bool BehaviourDSLCommon::isCallableVariable(const Hypothesis h,
                                              const std::string& n) const {
    if (h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      for (const auto& g : this->gradients) {
        if (g.name == n) {
          return true;
        }
      }
      for (const auto& f : this->thermodynamic_forces) {
        if (f.name == n) {
          return true;
        }
      }
      if (this->mb.isGradientName(n) || this->mb.isThermodynamicForceName(n)) {
        return true;
      }
    }
    return ((this->mb.isMaterialPropertyName(h, n)) ||
            (this->mb.isStateVariableName(h, n)) ||
            (this->mb.isAuxiliaryStateVariableName(h, n)) ||
            (this->mb.isExternalStateVariableName(h, n)) ||
            (this->mb.isLocalVariableName(h, n)) ||
            (this->mb.isStaticVariableName(h, n)) ||
            (this->mb.isParameterName(h, n)) ||
            (this->mb.isInitializeFunctionVariableName(h, n)) ||
            (this->mb.isPostProcessingVariableName(h, n)) ||
            (this->mb.isIntegrationVariableName(h, n)));
  }  // end of isCallableVariable

  std::string BehaviourDSLCommon::treatSetGlossaryNameMethod() {
    using namespace tfel::utilities;
    using namespace tfel::glossary;
    const auto& glossary = Glossary::getGlossary();
    this->checkNotEndOfFile("BehaviourDSLCommon::treatSetGlossaryMethod");
    this->readSpecifiedToken("BehaviourDSLCommon::treatSetGlossaryMethod",
                             "setGlossaryName");
    this->readSpecifiedToken("BehaviourDSLCommon::treatSetGlossaryMethod", "(");
    this->checkNotEndOfFile("BehaviourDSLCommon::treatSetGlossaryMethod");
    if (this->current->flag != Token::String) {
      this->throwRuntimeError("BehaviourDSLCommon::treatSetGlossaryMethod: ",
                              "expected to read a string");
    }
    const auto& g =
        this->current->value.substr(1, this->current->value.size() - 2);
    if (!glossary.contains(g)) {
      this->throwRuntimeError("BehaviourDSLCommon::treatSetGlossaryMethod: ",
                              "'" + g + "' is not a glossary name");
    }
    ++(this->current);
    this->checkNotEndOfFile("BehaviourDSLCommon::treatSetGlossaryMethod");
    this->readSpecifiedToken("BehaviourDSLCommon::treatSetGlossaryMethod", ")");
    return g;
  }  // end of treatSetGlossaryNameMethod

  std::string BehaviourDSLCommon::treatSetEntryNameMethod() {
    using namespace tfel::utilities;
    this->checkNotEndOfFile("BehaviourDSLCommon::treatSetEntryNameMethod");
    this->readSpecifiedToken("BehaviourDSLCommon::treatSetEntryNameMethod",
                             "setEntryName");
    this->checkNotEndOfFile("BehaviourDSLCommon::treatSetEntryNameMethod");
    this->readSpecifiedToken("BehaviourDSLCommon::treatSetEntryNameMethod",
                             "(");
    this->checkNotEndOfFile("BehaviourDSLCommon::treatSetEntryNameMethod");
    if (this->current->flag != Token::String) {
      this->throwRuntimeError("BehaviourDSLCommon::treatSetEntryNameMethod: ",
                              "expected to read a string");
    }
    const auto& e =
        this->current->value.substr(1, this->current->value.size() - 2);
    if (!this->isValidIdentifier(e)) {
      this->throwRuntimeError("BehaviourDSLCommon::treatSetEntryNameMethod: ",
                              "invalid entry name '" + e + "'");
    }
    ++(this->current);
    this->readSpecifiedToken("BehaviourDSLCommon::treatSetEntryNameMethod",
                             ")");
    return e;
  }  // end of treatSetEntryNameMethod

  void BehaviourDSLCommon::treatGradientMethod() {
    const auto n = tfel::unicode::getMangledString(this->current->value);
    ++(this->current);
    this->checkNotEndOfFile("BehaviourDSLCommon::treatGradientMethod");
    this->readSpecifiedToken("BehaviourDSLCommon::treatGradientMethod", ".");
    this->checkNotEndOfFile("BehaviourDSLCommon::treatGradientMethod");
    if (this->current->value == "setGlossaryName") {
      const auto gn = this->treatSetGlossaryNameMethod();
      bool treated = false;
      for (auto& g : this->gradients) {
        if (g.name == n) {
          g.setGlossaryName(gn);
          treated = true;
          break;
        }
      }
      if (!treated) {
        if (!this->mb.isGradientName(n)) {
          this->throwRuntimeError(
              "BehaviourDSLCommon::treatGradientMethod",
              "invalid call, '" + n + "' is not a registred gradient");
        }
        this->mb.setGlossaryName(n, gn);
      }
    } else if (this->current->value == "setEntryName") {
      const auto e = this->treatSetEntryNameMethod();
      bool treated = false;
      for (auto& g : this->gradients) {
        if (g.name == n) {
          g.setEntryName(e);
          treated = true;
          break;
        }
      }
      if (!treated) {
        if (!this->mb.isGradientName(n)) {
          this->throwRuntimeError(
              "BehaviourDSLCommon::treatGradientMethod",
              "invalid call, '" + n + "' is not a registred gradient");
        }
        this->mb.setEntryName(n, e);
      }
    } else {
      this->throwRuntimeError(
          "BehaviourDSLCommon::treatGradientMethod",
          "unsupported method '" + this->current->value + "'");
    }
    this->checkNotEndOfFile("BehaviourDSLCommon::treatGradientMethod");
    this->readSpecifiedToken("BehaviourDSLCommon::treatGradientMethod", ";");
  }  // end of treatGradientMethod

  void BehaviourDSLCommon::treatThermodynamicForceMethod() {
    const auto n = tfel::unicode::getMangledString(this->current->value);
    ++(this->current);
    this->checkNotEndOfFile(
        "BehaviourDSLCommon::treatThermodynamicForceMethod");
    this->readSpecifiedToken(
        "BehaviourDSLCommon::treatThermodynamicForceMethod", ".");
    this->checkNotEndOfFile(
        "BehaviourDSLCommon::treatThermodynamicForceMethod");
    if (this->current->value == "setGlossaryName") {
      const auto gn = this->treatSetGlossaryNameMethod();
      bool treated = false;
      for (auto& f : this->thermodynamic_forces) {
        if (f.name == n) {
          f.setGlossaryName(gn);
          treated = true;
          break;
        }
      }
      if (!treated) {
        if (!this->mb.isThermodynamicForceName(n)) {
          this->throwRuntimeError(
              "BehaviourDSLCommon::treatThermodynamicForceMethod",
              "invalid call, '" + n +
                  "' is not a registred thermodynamic force");
        }
        this->mb.setGlossaryName(n, gn);
      }
    } else if (this->current->value == "setEntryName") {
      const auto e = this->treatSetEntryNameMethod();
      bool treated = false;
      for (auto& f : this->thermodynamic_forces) {
        if (f.name == n) {
          f.setEntryName(e);
          treated = true;
          break;
        }
      }
      if (!treated) {
        if (!this->mb.isThermodynamicForceName(n)) {
          this->throwRuntimeError(
              "BehaviourDSLCommon::treatThermodynamicForceMethod",
              "invalid call, '" + n +
                  "' is not a registred thermodynamic force");
        }
        this->mb.setEntryName(n, e);
      }
    } else {
      this->throwRuntimeError(
          "BehaviourDSLCommon::treatThermodynamicForceMethod",
          "unsupported method '" + this->current->value + "'");
    }
    this->checkNotEndOfFile(
        "BehaviourDSLCommon::treatThermodynamicForceMethod");
    this->readSpecifiedToken(
        "BehaviourDSLCommon::treatThermodynamicForceMethod", ";");
  }  // end of treatThermodynamicForceMethod

  void BehaviourDSLCommon::treatVariableMethod(const Hypothesis h) {
    const auto n = tfel::unicode::getMangledString(this->current->value);
    ++(this->current);
    this->checkNotEndOfFile("BehaviourDSLCommon::treatVariableMethod");
    this->readSpecifiedToken("BehaviourDSLCommon::treatVariableMethod", ".");
    this->checkNotEndOfFile("BehaviourDSLCommon::treatVariableMethod");
    if (this->current->value == "setGlossaryName") {
      this->mb.setGlossaryName(h, n, this->treatSetGlossaryNameMethod());
    } else if (this->current->value == "setEntryName") {
      this->mb.setEntryName(h, n, this->treatSetEntryNameMethod());
    } else {
      this->treatUnknownVariableMethod(h, n);
    }
    this->checkNotEndOfFile("BehaviourDSLCommon::treatVariableMethod");
    this->readSpecifiedToken("BehaviourDSLCommon::treatVariableMethod", ";");
  }  // end of treatVariableMethod

  void BehaviourDSLCommon::treatUnknownVariableMethod(const Hypothesis,
                                                      const std::string& n) {
    this->throwRuntimeError(
        "BehaviourDSLCommon::treatUnknownVariableMethod : ",
        "unknown method '" + this->current->value + "' for variable '" + n +
            "', "
            "valid methods are 'setGlossaryName' or 'setEntryName'");
  }  // end of treatUnknownVariableMethod

  void BehaviourDSLCommon::treatUnknownKeyword() {
    TokensContainer::const_iterator p2;
    auto treated = false;
    --(this->current);
    const auto key = this->current->value;
    ++(this->current);
    this->checkNotEndOfFile("BehaviourDSLCommon::treatUnknownKeyword");
    for (const auto& b : bricks) {
      auto p = b->treatKeyword(key, this->current, this->tokens.end());
      if (p.first) {
        if (treated) {
          if (p2 != p.second) {
            this->throwRuntimeError("BehaviourDSLCommon::treatUnknownKeyword",
                                    "the keyword '" + key +
                                        "' has been treated "
                                        "by two interfaces/analysers but "
                                        "results were differents");
          }
        }
        p2 = p.second;
        treated = true;
      }
    }
    if (!treated) {
      if (this->current->value == "[") {
        ++(this->current);
        this->checkNotEndOfFile("BehaviourDSLCommon::treatUnknownKeyword");
        auto s = std::vector<std::string>{};
        while (this->current->value != "]") {
          this->checkNotEndOfFile("BehaviourDSLCommon::treatUnknownKeyword");
          const auto t = [this]() -> std::string {
            if (this->current->flag == tfel::utilities::Token::String) {
              return this->current->value.substr(
                  1, this->current->value.size() - 2);
            }
            return this->current->value;
          }();
          ++(this->current);
          this->checkNotEndOfFile("BehaviourDSLCommon::treatUnknownKeyword");
          if (std::find(s.begin(), s.end(), t) == s.end()) {
            s.push_back(t);
          }
          if (this->current->value != "]") {
            this->readSpecifiedToken("BehaviourDSLCommon::treatUnknownKeyword",
                                     ",");
            this->checkNotEndOfFile("BehaviourDSLCommon::treatUnknownKeyword");
            if (this->current->value == "]") {
              this->throwRuntimeError("BehaviourDSLCommon::treatUnknownKeyword",
                                      "unexpected token ']'");
            }
          }
        }
        ++(this->current);
        for (auto& i : this->interfaces) {
          auto p = i.second->treatKeyword(this->mb, key, s, this->current,
                                          this->tokens.end());
          if (p.first) {
            if (treated) {
              if (p2 != p.second) {
                this->throwRuntimeError(
                    "BehaviourDSLCommon::treatUnknownKeyword",
                    "the keyword '" + key +
                        "' has been treated "
                        "by two interfaces/analysers but "
                        "results were differents");
              }
            }
            p2 = p.second;
            treated = true;
          }
        }
        if (!treated) {
          this->ignoreKeyWord(key);
          return;
        }
      } else {
        for (const auto& i : this->interfaces) {
          auto p = i.second->treatKeyword(this->mb, key, {}, this->current,
                                          this->tokens.end());
          if (p.first) {
            if (treated) {
              if (p2 != p.second) {
                this->throwRuntimeError(
                    "BehaviourDSLCommon::treatUnknownKeyword",
                    "the keyword '" + key +
                        "' has been treated "
                        "by two interfaces/analysers but "
                        "results were differents");
              }
            }
            p2 = p.second;
            treated = true;
          }
        }
      }
    }
    if (!treated) {
      DSLBase::treatUnknownKeyword();
    }
    this->current = p2;
  }  // end of treatUnknownKeyword

  void BehaviourDSLCommon::treatUseQt() {
    this->checkNotEndOfFile("BehaviourDSLCommon::treatUseQt",
                            "Expected 'true' or 'false'.");
    this->mb.setUseQt(this->readBooleanValue("BehaviourDSLCommon::treatUseQt"));
    this->readSpecifiedToken("BehaviourDSLCommon::treatUseQt", ";");
  }  // end of treatUseQt

  void BehaviourDSLCommon::treatIsotropicBehaviour() {
    if (this->mb.getSymmetryType() != mfront::ISOTROPIC) {
      this->throwRuntimeError("BehaviourDSLCommon::treatIsotropicBehaviour",
                              "this behaviour has been declared orthotropic");
    }
    this->readSpecifiedToken("BehaviourDSLCommon::treatIsotropicBehaviour",
                             ";");
  }  // end of treatIsotropicBehaviour

  void BehaviourDSLCommon::treatOrthotropicBehaviour() {
    using namespace tfel::material;
    auto c = OrthotropicAxesConvention::DEFAULT;
    this->checkNotEndOfFile("BehaviourDSLCommon::treatOrthotropicBehaviour");
    if (this->current->value == "<") {
      this->readSpecifiedToken("BehaviourDSLCommon::treatOrthotropicBehaviour",
                               "<");
      this->checkNotEndOfFile("BehaviourDSLCommon::treatOrthotropicBehaviour");
      if (this->current->value == "Pipe") {
        ++this->current;
        c = OrthotropicAxesConvention::PIPE;
      } else if (this->current->value == "Plate") {
        ++this->current;
        c = OrthotropicAxesConvention::PLATE;
      } else if (this->current->value == "Default") {
        ++this->current;
      } else {
        this->throwRuntimeError("BehaviourDSLCommon::treatOrthotropicBehaviour",
                                "unsupported orthotropic axes convention");
      }
      this->readSpecifiedToken("BehaviourDSLCommon::treatOrthotropicBehaviour",
                               ">");
    }
    this->readSpecifiedToken("BehaviourDSLCommon::treatOrthotropicBehaviour",
                             ";");
    this->mb.setSymmetryType(mfront::ORTHOTROPIC);
    this->mb.setOrthotropicAxesConvention(c);
  }  // end of treatOrthotropicBehaviour

  void BehaviourDSLCommon::treatIsotropicElasticBehaviour() {
    this->readSpecifiedToken(
        "BehaviourDSLCommon::treatIsotropicElasticBehaviour", ";");
    if (this->mb.getSymmetryType() != mfront::ORTHOTROPIC) {
      this->throwRuntimeError(
          "BehaviourDSLCommon::treatIsotropicElasticBehaviour",
          "this behaviour has not been declared orthotropic");
    }
    this->mb.setElasticSymmetryType(mfront::ISOTROPIC);
  }  // end of treatIsotropicElasticBehaviour

  void BehaviourDSLCommon::treatRequireStiffnessOperator() {
    if (getVerboseMode() >= VERBOSE_LEVEL2) {
      getLogStream() << "BehaviourDSLCommon::treatRequireStiffnessOperator : "
                     << "@RequireStiffnessOperator is deprecated\n"
                     << "You shall use @RequireStiffnessTensor instead\n";
    }
    this->treatRequireStiffnessTensor();
  }  // end of treatRequireStiffnessOperator

  void BehaviourDSLCommon::treatStiffnessTensorOption() {
    this->readSpecifiedToken("BehaviourDSLCommon::treatStiffnessTensorOption",
                             "<");
    this->checkNotEndOfFile("BehaviourDSLCommon::treatStiffnessTensorOption");
    if (this->current->value == "UnAltered") {
      this->mb.setAttribute(
          BehaviourDescription::requiresUnAlteredStiffnessTensor, true, false);
    } else if (this->current->value == "Altered") {
      this->mb.setAttribute(
          BehaviourDescription::requiresUnAlteredStiffnessTensor, false, false);
    } else {
      this->throwRuntimeError(
          "BehaviourDSLCommon::treatStiffnessTensorOption : ",
          "expected 'Altered' or 'UnAltered' option "
          "(read '" +
              this->current->value + "')");
    }
    ++(this->current);
    this->readSpecifiedToken("BehaviourDSLCommon::treatStiffnessTensorOption",
                             ">");
  }

  void BehaviourDSLCommon::treatRequireStiffnessTensor() {
    if (this->mb.hasAttribute(BehaviourDescription::computesStiffnessTensor)) {
      this->throwRuntimeError("BehaviourDSLCommon::treatRequireStiffnessTensor",
                              "@RequireStiffnessTensor can be used along with "
                              "@ComputeStiffnessTensor");
    }
    this->checkNotEndOfFile("BehaviourDSLCommon::treatRequireStiffnessTensor");
    if (this->current->value == "<") {
      this->treatStiffnessTensorOption();
    }
    this->readSpecifiedToken("BehaviourDSLCommon::treatRequireStiffnessTensor",
                             ";");
    this->mb.setAttribute(BehaviourDescription::requiresStiffnessTensor, true,
                          false);
  }  // end of treatRequireStiffnessTensor

  void BehaviourDSLCommon::treatRequireThermalExpansionCoefficientTensor() {
    this->readSpecifiedToken(
        "BehaviourDSLCommon::treatRequireThermalExpansionCoefficientTensor",
        ";");
    this->mb.setAttribute(
        BehaviourDescription::requiresThermalExpansionCoefficientTensor, true,
        false);
  }  // end of treatRequireThermalExpansionCoefficientTensor

  void BehaviourDSLCommon::setMaterialKnowledgeIdentifier(
      const std::string& i) {
    if (!isValidBehaviourName(i)) {
      this->throwRuntimeError(
          "BehaviourDSLCommon::setMaterialKnowledgeIdentifier",
          "invalid behaviour name '" + i + "'");
    }
    this->mb.setBehaviourName(i);
    if (!isValidIdentifier(this->mb.getClassName())) {
      this->throwRuntimeError(
          "BehaviourDSLCommon::setMaterialKnowledgeIdentifier",
          "resulting class name is not valid (read '" +
              this->mb.getClassName() + "')");
    }
  }  // end of setMaterialKnowledgeIdentifier

  void BehaviourDSLCommon::treatBehaviour() {
    const auto& b = this->readOnlyOneToken();
    if (!isValidBehaviourName(b)) {
      this->throwRuntimeError("BehaviourDSLCommon::treatBehaviour",
                              "invalid behaviour name '" + b + "'");
    }
    if (this->overriden_implementation_name.empty()) {
      this->setMaterialKnowledgeIdentifier(b);
    }
  }  // end of treatBehaviour

  BehaviourVariableDescription
  BehaviourDSLCommon::readBehaviourVariableDescription(
      const std::string& sname,
      const tfel::utilities::Token::size_type lineNumber,
      const std::optional<std::string>& fileName,
      const bool is_auxiliary_model) {
    using namespace tfel::utilities;
    const auto mname = "readBehaviourVariableDescription";
    const auto bname = tfel::unicode::getMangledString(sname);
    //
    this->checkNotEndOfFile("BehaviourDSLCommon::treatBehaviourVariable");
    auto options = [this]() {
      if ((this->current != this->tokens.end()) &&
          (this->current->value == "{")) {
        return read<DataMap>(this->current, this->tokens.end());
      }
      return DataMap{};
    }();
    this->readSpecifiedToken(mname, ";");
    //
    auto validator =
        DataMapValidator{}
            .addDataTypeValidator<std::string>("variables_prefix")
            .addDataTypeValidator<std::string>("variables_suffix")
            .addDataTypeValidator<std::string>("external_names_prefix")
            .addDataTypeValidator<std::string>("external_names_suffix")
            .addDataTypeValidator<bool>("store_gradients")
            .addDataTypeValidator<bool>("store_thermodynamic_forces")
            .addDataTypeValidator<bool>(
                "automatically_save_associated_auxiliary_state_variables")
            .addDataTypeValidator<std::vector<Data>>(
                "shared_material_properties")
            .addDataTypeValidator<std::vector<Data>>(
                "evaluated_material_properties")
            .addDataTypeValidator<std::vector<Data>>(
                "shared_external_state_variables")
            .addDataTypeValidator<std::vector<Data>>(
                "evaluated_external_state_variables");
    if (!fileName.has_value()) {
      validator.addDataTypeValidator<std::string>("file");
    }
    validator.validate(options);
    //
    auto extract_regex_vector =
        [this, &options,
         mname](const std::string_view& n) -> std::vector<std::regex> {
      if (!options.contains(n)) {
        return {};
      }
      auto r = std::vector<std::regex>{};
      const auto& values = get<std::vector<Data>>(options, n);
      r.reserve(values.size());
      for (const auto& v : values) {
        if (!v.is<std::string>()) {
          this->throwRuntimeError(mname, "can't convert option '" +
                                             std::string{n} +
                                             "' to a vector of strings");
        }
        try {
          r.emplace_back(get<std::string>(v));
        } catch (std::exception& e) {
          this->throwRuntimeError(
              mname, "failed to compile '" + get<std::string>(v) +
                         "' as a regular expression: " + std::string{e.what()});
        }
      }
      return r;
    };
    const auto& file = [&fileName, &options] {
      if (fileName.has_value()) {
        return *fileName;
      }
      return get<std::string>(options, "file");
    }();
    //
    const auto v_prefix = get_if<std::string>(options, "variables_prefix", "");
    const auto v_suffix = get_if<std::string>(options, "variables_suffix", "");
    const auto enames_prefix =
        get_if<std::string>(options, "external_names_prefix", "");
    const auto enames_suffix =
        get_if<std::string>(options, "external_names_suffix", "");
    if ((!v_prefix.empty()) && (!isValidUserDefinedVariableName(v_prefix))) {
      this->throwRuntimeError(mname,
                              "invalid variables prefix '" + v_prefix + "'");
    }
    if ((!v_suffix.empty()) &&
        (!isValidUserDefinedVariableName("v" + v_suffix))) {
      this->throwRuntimeError(mname,
                              "invalid variables suffix '" + v_suffix + "'");
    }
    if ((!enames_prefix.empty()) &&
        (!isValidUserDefinedVariableName(enames_prefix))) {
      this->throwRuntimeError(
          mname, "invalid external names prefix '" + enames_prefix + "'");
    }
    if ((!enames_suffix.empty()) &&
        (!isValidUserDefinedVariableName("v" + enames_suffix))) {
      this->throwRuntimeError(
          mname, "invalid external names suffix '" + enames_suffix + "'");
    }
    //
    auto d = BehaviourVariableDescription{
        .file = file,
        .symbolic_form = sname,
        .name = bname,
        .description =
            [this] {
              if (!this->currentComment.empty()) {
                return this->currentComment;
              }
              return std::string{};
            }(),
        .line_number = lineNumber,
        .variables_prefix = v_prefix,
        .variables_suffix = v_suffix,
        .external_names_prefix = enames_prefix,
        .external_names_suffix = enames_suffix,
        .shared_material_properties =
            extract_regex_vector("shared_material_properties"),
        .evaluated_material_properties =
            extract_regex_vector("evaluated_material_properties"),
        .shared_external_state_variables =
            extract_regex_vector("shared_external_state_variables"),
        .evaluated_external_state_variables =
            extract_regex_vector("evaluated_external_state_variables"),
        .store_gradients = get_if<bool>(options, "store_gradients", true),
        .store_thermodynamic_forces =
            get_if<bool>(options, "store_thermodynamic_forces", true),
        .automatically_save_associated_auxiliary_state_variables = get_if<bool>(
            options, "automatically_save_associated_auxiliary_state_variables",
            true),
        .is_auxiliary_model = is_auxiliary_model,
        .behaviour = this->getBehaviourDescription(file)};
    // some restrictions of the behaviours
    if (d.behaviour.getAttribute(BehaviourDescription::requiresStiffnessTensor,
                                 false)) {
      this->throwRuntimeError(
          mname,
          "behaviour requiring the stiffness tensor are not supported yet");
    }
    if (d.behaviour.getAttribute(
            BehaviourDescription::requiresThermalExpansionCoefficientTensor,
            false)) {
      this->throwRuntimeError(mname,
                              "behaviour requiring the thermal expansion "
                              "coefficient tensor are not supported yet");
    }
    this->reserveName(getBehaviourWrapperClassName(d));
    this->reserveName("mfront_behaviour_variable_" + d.name);
    return d;
  }  // end of readBehaviourVariableDescription

  BehaviourVariableDescription
  BehaviourDSLCommon::readBehaviourVariableDescription() {
    using namespace tfel::utilities;
    const char* const mname =
        "BehaviourDSLCommon::readBehaviourVariableDescription";
    this->checkNotEndOfFile(mname);
    const auto lineNumber = this->current->line;
    const auto sname = this->current->value;
    ++(this->current);
    if (!isValidUserDefinedVariableName(
            tfel::unicode::getMangledString(sname))) {
      this->throwRuntimeError(mname, "invalid variable name '" + sname + "'");
    }
    return readBehaviourVariableDescription(sname, lineNumber, {}, false);
  }  // end of readBehaviourVariableDescription

  void BehaviourDSLCommon::treatBehaviourVariable() {
    this->mb.addBehaviourVariable(this->readBehaviourVariableDescription());
  }  // end of treatBehaviourVariable

  void BehaviourDSLCommon::treatBehaviourVariableFactory() {
    auto d = this->readBehaviourVariableDescription();
    this->reserveName(getBehaviourVariableFactoryClassName(d));
    this->mb.addBehaviourVariableFactory(std::move(d));
  }  // end of treatBehaviourVariableFactory

  void BehaviourDSLCommon::readStringList(std::vector<std::string>& cont) {
    this->checkNotEndOfFile("BehaviourDSLCommon::readStringList",
                            "Cannot read interface name.");
    auto endOfTreatment = false;
    while ((this->current != this->tokens.end()) && (!endOfTreatment)) {
      const auto s = this->current->value;
      if (!isValidIdentifier(s)) {
        --(this->current);
        this->throwRuntimeError(
            "BehaviourDSLCommon::readStringList",
            "interface name is not valid (read '" + s + "')");
      }
      ++(this->current);
      this->checkNotEndOfFile("BehaviourDSLCommon::readStringList");
      if (this->current->value == ",") {
        ++(this->current);
      } else if (this->current->value == ";") {
        endOfTreatment = true;
        ++(this->current);
      } else {
        this->throwRuntimeError("BehaviourDSLCommon::readStringList",
                                "',' or ';' expected after '" + s + "'");
      }
      if (find(cont.begin(), cont.end(), s) != cont.end()) {
        this->throwRuntimeError("BehaviourDSLCommon::readStringList",
                                "'" + s + "' has already been registred.\n");
      }
      cont.push_back(s);
    }
    if (!endOfTreatment) {
      --(this->current);
      this->throwRuntimeError("BehaviourDSLCommon::readStringList",
                              "Expected ';' before end of file.");
    }
  }

  std::set<BehaviourDSLCommon::Hypothesis>
  BehaviourDSLCommon::readHypothesesList() {
    std::set<Hypothesis> mh;
    this->readHypothesesList(mh);
    return mh;
  }  // end of readHypothesesList

  void BehaviourDSLCommon::readHypothesesList(std::set<Hypothesis>& h) {
    h.clear();
    if (this->current == this->tokens.end()) {
      h.insert(ModellingHypothesis::UNDEFINEDHYPOTHESIS);
      return;
    }
    if (this->current->value != "<") {
      h.insert(ModellingHypothesis::UNDEFINEDHYPOTHESIS);
      return;
    }
    auto values = std::vector<tfel::utilities::Token>{};
    this->readList(values, "BehaviourDSLCommon::readHypothesesList", "<", ">",
                   true);
    for (const auto& v : values) {
      if (v.flag == tfel::utilities::Token::String) {
        this->appendToHypothesesList(h, v.value.substr(1, v.value.size() - 2));
      } else {
        this->appendToHypothesesList(h, v.value);
      }
    }
    if (h.empty()) {
      h.insert(ModellingHypothesis::UNDEFINEDHYPOTHESIS);
    }
  }  // end of readHypothesesList

  void BehaviourDSLCommon::readVariableList(
      VariableDescriptionContainer& v,
      std::set<Hypothesis>& h,
      void (BehaviourDescription::*m)(const Hypothesis,
                                      const VariableDescriptionContainer&,
                                      const BehaviourData::RegistrationStatus),
      const bool b1) {
    h.clear();
    v.clear();
    this->readHypothesesList(h);
    this->readVarList(v, b1);
    this->addVariableList(h, v, m);
  }  // end of readVariableList

  void BehaviourDSLCommon::addVariableList(
      const std::set<Hypothesis>& hypotheses,
      const VariableDescriptionContainer& v,
      void (BehaviourDescription::*m)(
          const Hypothesis,
          const VariableDescriptionContainer&,
          const BehaviourData::RegistrationStatus)) {
    for (const auto& h : hypotheses) {
      (this->mb.*m)(h, v, BehaviourData::UNREGISTRED);
    }
  }  // end of addVariableList

  void BehaviourDSLCommon::treatCoef() {
    VarContainer v;
    auto h = std::set<Hypothesis>{};
    this->readVariableList(v, h, &BehaviourDescription::addMaterialProperties,
                           true);
  }  // end of treatCoef

  void BehaviourDSLCommon::treatLocalVar() {
    VarContainer v;
    auto h = std::set<Hypothesis>{};
    this->readVariableList(v, h, &BehaviourDescription::addLocalVariables,
                           true);
  }  // end of treatLocalVar

  void BehaviourDSLCommon::treatInterface() {
    auto& mbif = BehaviourInterfaceFactory::getBehaviourInterfaceFactory();
    auto inames = std::vector<std::string>{};
    this->readStringList(inames);
    for (const auto& i : inames) {
      if (this->interfaces.count(i) == 0) {
        this->interfaces.insert({i, mbif.getInterface(i)});
      }
    }
  }  // end of treatInterface

  void BehaviourDSLCommon::setInterfaces(const std::set<std::string>& inames) {
    auto& mbif = BehaviourInterfaceFactory::getBehaviourInterfaceFactory();
    for (const auto& i : inames) {
      if (this->interfaces.count(i) == 0) {
        this->interfaces.insert({i, mbif.getInterface(i)});
      }
    }
  }  // end of setInterfaces

  void BehaviourDSLCommon::treatAPrioriTimeStepScalingFactor() {
    std::function<std::string(const Hypothesis, const std::string&, const bool)>
        m = [this](const Hypothesis h, const std::string& sv, const bool b) {
          return this->standardModifier(h, sv, b);
        };
    this->treatCodeBlock(BehaviourData::APrioriTimeStepScalingFactor,  //
                         m, true, true);
  }

  void BehaviourDSLCommon::treatIntegrator() {
    std::function<std::string(const Hypothesis, const std::string&, const bool)>
        m = [this](const Hypothesis h, const std::string& sv, const bool b) {
          return this->standardModifier(h, sv, b);
        };
    this->treatCodeBlock(BehaviourData::Integrator, m, true, true);
  }  // end of treatIntegrator

  void BehaviourDSLCommon::treatAPosterioriTimeStepScalingFactor() {
    std::function<std::string(const Hypothesis, const std::string&, const bool)>
        m = [this](const Hypothesis h, const std::string& sv, const bool b) {
          return this->standardModifier(h, sv, b);
        };
    this->treatCodeBlock(BehaviourData::APosterioriTimeStepScalingFactor,  //
                         m, true, true);
  }

  void BehaviourDSLCommon::treatStateVariable() {
    VarContainer v;
    auto h = std::set<Hypothesis>{};
    this->readVariableList(v, h, &BehaviourDescription::addStateVariables,
                           true);
  }  // end of treatStateVariable

  void BehaviourDSLCommon::treatAuxiliaryStateVariable() {
    VarContainer v;
    auto h = std::set<Hypothesis>{};
    this->readVariableList(
        v, h, &BehaviourDescription::addAuxiliaryStateVariables, true);
  }  // end of treatAuxiliaryStateVariable

  void BehaviourDSLCommon::treatModel(
      const std::string& method,
      void (BehaviourDescription::*m1)(const ModelDescription&),
      void (BehaviourDescription::*m2)(const BehaviourVariableDescription&)) {
    if (getVerboseMode() >= VERBOSE_DEBUG) {
      getLogStream() << method << ": begin\n";
    }
    this->checkNotEndOfFile(method);
    const auto lineNumber = this->current->line;
    const auto f = SearchPathsHandler::search(this->readString(method));
    const auto adsl = MFrontBase::getDSL(f);
    if (adsl->getTargetType() == AbstractDSL::MODELDSL) {
      auto md = this->getModelDescription(f);
      this->readSpecifiedToken(method, ";");
      (this->mb.*m1)(md);
    } else {
      const auto d = [this, &f, &method, &lineNumber] {
        this->checkNotEndOfFile(method);
        auto md = this->getBehaviourDescription(f);
        const auto name = "mfront_external_model_" + md.getClassName();
        if (this->current->value == "{") {
          return readBehaviourVariableDescription(name, lineNumber, f);
        }
        this->readSpecifiedToken(method, ";");
        return BehaviourVariableDescription{
            .file = f,
            .symbolic_form = name,
            .name = name,
            .description =
                [this] {
                  if (!this->currentComment.empty()) {
                    return this->currentComment;
                  }
                  return std::string{};
                }(),
            .line_number = lineNumber,
            .variables_prefix = "",
            .variables_suffix = "",
            .external_names_prefix = "",
            .external_names_suffix = "",
            .shared_material_properties = {},
            .evaluated_material_properties = {std::regex{".+"}},
            .shared_external_state_variables = {},
            .evaluated_external_state_variables = {std::regex{".+"}},
            .store_gradients = false,
            .store_thermodynamic_forces = false,
            .automatically_save_associated_auxiliary_state_variables = false,
            .is_auxiliary_model = true,
            .behaviour = std::move(md)};
      }();
      if (!d.behaviour.isModel()) {
        this->throwRuntimeError(
            method, "file path '" + f + "' does not describe a model");
      }
      const auto fname = getBehaviourVariableFactoryClassName(d);
      this->reserveName(fname);
      this->reserveName(fname + "_instance");
      (this->mb.*m2)(d);
    }
    if (getVerboseMode() >= VERBOSE_DEBUG) {
      getLogStream() << method << ": end\n";
    }
  }

  void BehaviourDSLCommon::treatAuxiliaryModel() {
    const auto m1 =
        static_cast<void (BehaviourDescription::*)(const ModelDescription&)>(
            &BehaviourDescription::addModelDescription);
    const auto m2 = static_cast<void (BehaviourDescription::*)(
        const BehaviourVariableDescription&)>(
        &BehaviourDescription::addModelDescription);
    this->treatModel("BehaviourDSLCommon::treatAuxiliaryModel", m1, m2);
  }  // end of treatAuxiliaryModel

  void BehaviourDSLCommon::treatExternalStateVariable() {
    VarContainer v;
    auto h = std::set<Hypothesis>{};
    this->readVariableList(
        v, h, &BehaviourDescription::addExternalStateVariables, true);
  }  // end of treatExternalStateVariable()

  void BehaviourDSLCommon::treatInitializeFunctionVariable() {
    auto v = VariableDescriptionContainer{};
    auto hypotheses = std::set<Hypothesis>{};
    this->readHypothesesList(hypotheses);
    this->readVarList(v, true);
    for (const auto& h : hypotheses) {
      this->mb.addInitializeFunctionVariables(h, v);
    }
  }  // end of treatInitializeFunctionVariable()

  void BehaviourDSLCommon::treatPostProcessingVariable() {
    auto v = VariableDescriptionContainer{};
    auto hypotheses = std::set<Hypothesis>{};
    this->readHypothesesList(hypotheses);
    this->readVarList(v, true);
    for (const auto& h : hypotheses) {
      this->mb.addPostProcessingVariables(h, v);
    }
  }  // end of treatPostProcessingVariable()

  void BehaviourDSLCommon::treatBounds() {
    auto hs = std::set<Hypothesis>{};
    this->readHypothesesList(hs);
    auto b = this->current;
    for (const auto& h : hs) {
      this->current = b;
      const auto r = this->readVariableBounds();
      const auto v = extractVariableNameAndArrayPosition(r.first);
      if (std::get<1>(v)) {
        this->mb.setBounds(h, std::get<0>(v), std::get<2>(v), r.second);
      } else {
        this->mb.setBounds(h, std::get<0>(v), r.second);
      }
    }
    this->readSpecifiedToken("BehaviourDSLCommon::treatBounds", ";");
  }  // end of treatBounds

  void BehaviourDSLCommon::treatPhysicalBounds() {
    auto hs = std::set<Hypothesis>{};
    this->readHypothesesList(hs);
    auto b = current;
    for (const auto& h : hs) {
      this->current = b;
      const auto bounds = this->readVariableBounds();
      this->mb.setPhysicalBounds(h, bounds.first, bounds.second);
    }
    this->readSpecifiedToken("BehaviourDSLCommon::treatBounds", ";");
  }  // end of treatPhysicalBounds

  void BehaviourDSLCommon::registerDefaultVarNames() {
    const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    // all available tangent operators for finite strain behaviours
    const auto tos =
        tfel::material::getFiniteStrainBehaviourTangentOperatorFlags();
    // stiffness tensor
    this->mb.registerMemberName(h, "D");
    // stiffness tensor at the end of the time step
    this->mb.registerMemberName(h, "D_tdt");
    // tangent operator
    this->mb.registerMemberName(h, "Dt");
    this->reserveName("N");
    this->reserveName("Type");
    this->reserveName("use_qt");
    this->reserveName("src1");
    this->reserveName("src2");
    this->reserveName("policy_value");
    this->reserveName("integrate");
    this->reserveName("Psi_s");
    this->reserveName("Psi_d");
    this->reserveName("thermal_expansion_reference_temperature");
    this->reserveName("initial_geometry_reference_temperature");
    this->mb.registerMemberName(h, "computeThermodynamicForces");
    this->mb.registerMemberName(h, "computeFinalThermodynamicForces");
    this->mb.registerMemberName(h, "computeStressFreeStrain");
    this->mb.registerMemberName(h, "computeStressFreeExpansion");
    this->mb.registerMemberName(h, "computeInternalEnergy");
    this->mb.registerMemberName(h, "computeDissipatedEnergy");
    this->mb.registerMemberName(h, "computeFdF");
    this->mb.registerMemberName(h, "updateIntegrationVariables");
    this->mb.registerMemberName(h, "updateStateVariables");
    this->mb.registerMemberName(h, "updateAuxiliaryStateVariables");
    this->mb.registerMemberName(h, "getTangentOperator");
    this->mb.registerMemberName(h, "getMinimalTimeStepScalingFactor");
    this->mb.registerMemberName(h, "computeAPrioriTimeStepScalingFactor");
    this->mb.registerMemberName(h, "computeAPrioriTimeStepScalingFactorII");
    this->mb.registerMemberName(h, "computeAPosterioriTimeStepScalingFactor");
    this->mb.registerMemberName(h, "computeAPosterioriTimeStepScalingFactorII");
    this->reserveName("computeTangentOperator_");
    this->mb.registerMemberName(h, "computeConsistentTangentOperator");
    for (const auto& to : tos) {
      const auto ktype =
          convertFiniteStrainBehaviourTangentOperatorFlagToString(to);
      this->mb.registerMemberName(h, ktype);
      this->mb.registerMemberName(h,
                                  "computeConsistentTangentOperator_" + ktype);
      this->mb.registerMemberName(h, "tangentOperator_" + ktype);
    }
    this->reserveName("tangentOperator_sk2");
    this->reserveName("computePredictionOperator");
    for (const auto& to : tos) {
      const auto ktype =
          convertFiniteStrainBehaviourTangentOperatorFlagToString(to);
      this->mb.registerMemberName(h, "computePredictionOperator_" + ktype);
    }
    this->reserveName("smt");
    this->reserveName("smflag");
    this->reserveName("dl0_l0");
    this->reserveName("dl1_l0");
    this->reserveName("mfront_dl0_l0");
    this->reserveName("mfront_dl1_l0");
    this->reserveName("mfront_dl01_l0");
    this->reserveName("mfront_sfs_bts");
    this->reserveName("mfront_sfs_ets");
    this->reserveName("alpha_Ti");
    this->reserveName("alpha0_Ti");
    this->reserveName("alpha1_Ti");
    this->reserveName("alpha2_Ti");
    this->reserveName("alpha_T_t");
    this->reserveName("alpha_T_t_dt");
    this->reserveName("alpha0_T_t");
    this->reserveName("alpha0_T_t_dt");
    this->reserveName("alpha1_T_t");
    this->reserveName("alpha1_T_t_dt");
    this->reserveName("alpha2_T_t");
    this->reserveName("alpha2_T_t_dt");
    this->reserveName("StressFreeExpansionType");
    this->reserveName("behaviourData");
    this->reserveName("time_scaling_factor");
    this->reserveName("mp_bounds_check_status");
    this->reserveName("initial_state");
  }  // end of registerDefaultVarNames

  bool BehaviourDSLCommon::useQt() const {
    return this->mb.useQt();
  }  // end of useQt

  void BehaviourDSLCommon::disableQuantitiesUsageIfNotAlreadySet() {
    this->mb.disableQuantitiesUsageIfNotAlreadySet();
  }  // end of disableQuantitiesUsageIfNotAlreadySet

  void BehaviourDSLCommon::reserveName(const std::string& n) {
    this->mb.reserveName(ModellingHypothesis::UNDEFINEDHYPOTHESIS, n);
  }

  bool BehaviourDSLCommon::isNameReserved(const std::string& n) const {
    return this->mb.isNameReserved(n);
  }

  void BehaviourDSLCommon::treatUpdateAuxiliaryStateVariables() {
    std::function<std::string(const Hypothesis, const std::string&, const bool)>
        m = [this](const Hypothesis h, const std::string& sv, const bool b) {
          return this->standardModifier(h, sv, b);
        };
    this->treatCodeBlock(BehaviourData::UpdateAuxiliaryStateVariables,  //
                         m, true, true);
  }  // end of treatUpdateAuxiliaryStateVarBase

  void BehaviourDSLCommon::treatComputeStressFreeExpansion() {
    std::function<std::string(const Hypothesis, const std::string&, const bool)>
        m = [this](const Hypothesis h, const std::string& sv, const bool b) {
          return this->standardModifier(h, sv, b);
        };
    this->treatCodeBlock(BehaviourData::ComputeStressFreeExpansion,  //
                         m, true, true);
  }  // end of treatComputeStressFreeExpansion

  void BehaviourDSLCommon::treatSwelling() {
    using VolumeSwelling = BehaviourData::VolumeSwellingStressFreeExpansion;
    using IsotropicSwelling = BehaviourData::IsotropicStressFreeExpansion;
    using OrthotropicSwelling = BehaviourData::OrthotropicStressFreeExpansion;
    using OrthotropicSwellingII =
        BehaviourData::OrthotropicStressFreeExpansionII;
    auto throw_if = [this](const bool b, const std::string& m) {
      if (b) {
        this->throwRuntimeError("BehaviourDSLCommon::treatSwelling", m);
      }
    };
    enum { VOLUME, LINEAR, ORTHOTROPIC, UNDEFINED } etype = UNDEFINED;
    const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    throw_if((this->mb.getBehaviourType() !=
              BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) &&
                 (this->mb.getBehaviourType() !=
                  BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR),
             "the @Swelling keyword is only valid for small or "
             "finite strain behaviours");
    this->checkNotEndOfFile("BehaviourDSLCommon::treatSwelling");
    if (this->current->value == "<") {
      auto options = std::vector<tfel::utilities::Token>{};
      this->readList(options, "BehaviourDSLCommon::treatSwelling", "<", ">",
                     true);
      for (const auto& o : options) {
        this->checkNotEndOfFile("BehaviourDSLCommon::treatSwelling");
        if (o.value == "Orthotropic") {
          throw_if(etype != UNDEFINED,
                   "error while treating option "
                   "'Orthotropic', swelling type already defined");
          etype = ORTHOTROPIC;
        } else if (o.value == "Volume") {
          throw_if(etype != UNDEFINED,
                   "error while treating option "
                   "'Volume', swelling type already defined");
          etype = VOLUME;
        } else if (o.value == "Linear") {
          throw_if(etype != UNDEFINED,
                   "error while treating option "
                   "'Linear', swelling type already defined");
          etype = LINEAR;
        } else {
          throw_if(true, "unsupported option '" + o.value + "'");
        }
      }
    }
    throw_if(etype == UNDEFINED,
             "the user must explicitly state if "
             "what kind of swelling is expected using"
             "one of the options 'Linear', 'Volume' or 'Orthotropic'");
    const auto sd = this->readStressFreeExpansionHandler();
    this->readSpecifiedToken("BehaviourDSLCommon::treatSwelling", ";");
    if (sd.size() == 1) {
      throw_if(sd[0].is<BehaviourData::NullExpansion>(),
               "a null swelling is not allowed here");
      if (etype == VOLUME) {
        VolumeSwelling vs = {sd[0]};
        this->mb.addStressFreeExpansion(uh, vs);
      } else if (etype == LINEAR) {
        IsotropicSwelling is = {sd[0]};
        this->mb.addStressFreeExpansion(uh, is);
      } else if (etype == ORTHOTROPIC) {
        throw_if(!sd[0].is<BehaviourData::SFED_ESV>(),
                 "one expects a external state variable name here");
        OrthotropicSwellingII os = {sd[0].get<BehaviourData::SFED_ESV>()};
        this->mb.addStressFreeExpansion(uh, os);
      } else {
        throw_if(true, "internal error");
      }
    } else if (sd.size() == 3) {
      throw_if(etype != ORTHOTROPIC,
               "the 'Orthotropic' option must be "
               "used for an orthotropic swelling");
      throw_if(sd[0].is<BehaviourData::NullExpansion>() &&
                   sd[1].is<BehaviourData::NullExpansion>() &&
                   sd[2].is<BehaviourData::NullExpansion>(),
               "all swelling component are null");
      OrthotropicSwelling os = {sd[0], sd[1], sd[2]};
      this->mb.addStressFreeExpansion(uh, os);
    } else {
      throw_if(true, "invalid number of swelling handler (shall be 1 or 3, " +
                         std::to_string(sd.size()) + " given)");
    }
  }  // end of treatSwelling

  BehaviourData::StressFreeExpansionHandler
  BehaviourDSLCommon::readStressFreeExpansionHandler(
      const tfel::utilities::Token& t) {
    auto throw_if = [this](const bool b, const std::string& m) {
      if (b) {
        this->throwRuntimeError(
            "BehaviourDSLCommon::readStressFreeExpansionHandler", m);
      }
    };
    if (t.flag == tfel::utilities::Token::String) {
      // using an external model
      const auto md =
          this->getModelDescription(t.value.substr(1, t.value.size() - 2));
      // check that the variable
      auto ptr = std::make_shared<ModelDescription>(md);
      return {ptr};
    }
    if (t.value == "0") {
      return {BehaviourData::NullExpansion{}};
    }
    throw_if(!CxxTokenizer::isValidIdentifier(t.value, true),
             "unexpected token '" + t.value +
                 "', expected "
                 "external state variable name");
    // using an external state variable
    // defining modelling hypotheses
    if (!this->mb.areModellingHypothesesDefined()) {
      this->mb.setModellingHypotheses(this->getDefaultModellingHypotheses());
    }
    for (const auto h : this->mb.getDistinctModellingHypotheses()) {
      throw_if(!this->mb.isExternalStateVariableName(h, t.value),
               "no external state variable named '" + t.value +
                   "' "
                   "has been declared");
    }
    return {BehaviourData::SFED_ESV{t.value}};
  }  // end of readStressFreeExpansionHandler

  std::vector<BehaviourData::StressFreeExpansionHandler>
  BehaviourDSLCommon::readStressFreeExpansionHandler() {
    auto throw_if = [this](const bool b, const std::string& m) {
      if (b) {
        this->throwRuntimeError(
            "BehaviourDSLCommon::readStressFreeExpansionHandler", m);
      }
    };
    auto sda = std::vector<tfel::utilities::Token>{};
    auto sd = std::vector<BehaviourData::StressFreeExpansionHandler>{};
    this->checkNotEndOfFile("BehaviourDSLCommon::treatSwelling");
    if (this->current->value == "{") {
      this->readList(sda, "BehaviourDSLCommon::readCodeBlockOptions", "{", "}",
                     true);
    } else {
      sda.push_back(*(this->current));
      ++(this->current);
    }
    if (sda.size() == 1u) {
      sd.push_back(this->readStressFreeExpansionHandler(sda[0]));
    } else if (sda.size() == 3u) {
      throw_if(this->mb.getSymmetryType() != mfront::ORTHOTROPIC,
               "orthotropic swelling is only  supported for "
               "orthotropic behaviours");
      sd.push_back(this->readStressFreeExpansionHandler(sda[0]));
      sd.push_back(this->readStressFreeExpansionHandler(sda[1]));
      sd.push_back(this->readStressFreeExpansionHandler(sda[2]));
    } else {
      throw_if(true,
               "invalid number of swelling description "
               "(expected one or three descriptions)");
    }
    return sd;
  }  // end of readStressFreeExpansionHandler

  void BehaviourDSLCommon::treatAxialGrowth() {
    using AxialGrowth = BehaviourData::AxialGrowth;
    const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    auto throw_if = [this](const bool b, const std::string& m) {
      if (b) {
        this->throwRuntimeError("BehaviourDSLCommon::treatAxialGrowth", m);
      }
    };
    throw_if((this->mb.getBehaviourType() !=
              BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) &&
                 (this->mb.getBehaviourType() !=
                  BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR),
             "the @AxialGrowth keyword is only valid for small or "
             "finite strain behaviours");
    throw_if(this->mb.getSymmetryType() != mfront::ORTHOTROPIC,
             "@AxialGrowth is only valid for orthotropic behaviour");
    this->checkNotEndOfFile("BehaviourDSLCommon::treatAxialGrowth");
    auto s = this->readStressFreeExpansionHandler(*(this->current));
    ++(this->current);
    this->readSpecifiedToken("BehaviourDSLCommon::treatAxialGrowth", ";");
    this->mb.addStressFreeExpansion(uh, AxialGrowth{s});
  }  // end of treatAxialGrowth

  void BehaviourDSLCommon::treatRelocation() {
    using Relocation = BehaviourData::Relocation;
    auto throw_if = [this](const bool b, const std::string& m) {
      if (b) {
        this->throwRuntimeError("BehaviourDSLCommon::treatRelocation", m);
      }
    };
    throw_if((this->mb.getBehaviourType() !=
              BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) &&
                 (this->mb.getBehaviourType() !=
                  BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR),
             "the @Relocation keyword is only valid for small or "
             "finite strain behaviours");
    if (!this->mb.areModellingHypothesesDefined()) {
      this->mb.setModellingHypotheses(this->getDefaultModellingHypotheses());
    }
    const auto& mh = this->mb.getModellingHypotheses();
    throw_if(
        (mh.find(ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS) ==
         mh.end()) &&
            (mh.find(
                 ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN) ==
             mh.end()) &&
            (mh.find(ModellingHypothesis::GENERALISEDPLANESTRAIN) == mh.end()),
        "the @Relocation keyword has not effect on this behaviour as the none "
        "of "
        "the following hypothesis is supported:\n"
        "- AxisymmetricalGeneralisedPlaneStress\n"
        "- AxisymmetricalGeneralisedPlaneStrain\n"
        "- GeneralisedPlaneStrain");
    this->checkNotEndOfFile("BehaviourDSLCommon::treatRelocation");
    const auto s = this->readStressFreeExpansionHandler(*(this->current));
    ++(this->current);
    this->readSpecifiedToken("BehaviourDSLCommon::treatRelocation", ";");
    auto add = [this, &mh, &s](const Hypothesis h) {
      if (mh.find(h) != mh.end()) {
        this->mb.addStressFreeExpansion(h, Relocation{s});
      }
    };
    add(ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS);
    add(ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN);
    add(ModellingHypothesis::GENERALISEDPLANESTRAIN);
  }  // end of treatRelocation

  std::string BehaviourDSLCommon::predictionOperatorVariableModifier(
      const Hypothesis h, const std::string& var, const bool addThisPtr) {
    if (this->mb.isIntegrationVariableIncrementName(h, var)) {
      this->throwRuntimeError(
          "BehaviourDSLCommon::predictionOperatorVariableModifier : ",
          "integration variable '" + var +
              "' can't be used in @PredictionOperator");
    }
    if (addThisPtr) {
      return "(this->" + var + ")";
    }
    return var;
  }  // end of predictionOperatorVariableModifier

  void BehaviourDSLCommon::treatProfiling() {
    const auto b = this->readBooleanValue("BehaviourDSLCommon::treatProfiling");
    this->readSpecifiedToken("BehaviourDSLCommon::treatProfiling", ";");
    this->mb.setAttribute(BehaviourData::profiling, b, false);
  }  // end of treatProfiling

  void BehaviourDSLCommon::treatPredictionOperator() {
    using namespace tfel::material;
    using namespace tfel::utilities;
    if (this->mb.getTangentOperatorBlocks().empty()) {
      this->throwRuntimeError("BehaviourDSLCommon::treatTangentOperator",
                              "no thermodynamic force defined");
    }
    std::function<std::string(const Hypothesis, const std::string&, const bool)>
        modifier =
            [this](const Hypothesis h, const std::string& sv, const bool b) {
              return this->predictionOperatorVariableModifier(h, sv, b);
            };
    CodeBlockOptions o;
    this->readCodeBlockOptions(o, true);
    if (this->mb.getBehaviourType() ==
        BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
      bool found = false;
      if (o.untreated.size() != 1u) {
        std::ostringstream msg;
        msg << "tangent operator type is undefined. Valid tanget operator type "
               "are :\n";
        for (const auto& to : getFiniteStrainBehaviourTangentOperatorFlags()) {
          msg << "- "
              << convertFiniteStrainBehaviourTangentOperatorFlagToString(to)
              << " : " << getFiniteStrainBehaviourTangentOperatorDescription(to)
              << '\n';
        }
        this->throwRuntimeError("BehaviourDSLCommon::treatPredictionOperator",
                                msg.str());
      }
      if (o.untreated[0].flag != Token::Standard) {
        this->throwRuntimeError(
            "BehaviourDSLCommon::treatPredictionOperator",
            "invalid option '" + o.untreated[0].value + "'");
      }
      const auto& ktype = o.untreated[0].value;
      for (const auto& to : getFiniteStrainBehaviourTangentOperatorFlags()) {
        if (ktype ==
            convertFiniteStrainBehaviourTangentOperatorFlagToString(to)) {
          found = true;
          break;
        }
      }
      if (!found) {
        std::ostringstream msg;
        msg << "invalid tangent operator type '" + ktype +
                   "'. Valid tanget operator type are :\n";
        for (const auto& to : getFiniteStrainBehaviourTangentOperatorFlags()) {
          msg << "- "
              << convertFiniteStrainBehaviourTangentOperatorFlagToString(to)
              << " : " << getFiniteStrainBehaviourTangentOperatorDescription(to)
              << '\n';
        }
        this->throwRuntimeError("BehaviourDSLCommon::treatPredictionOperator",
                                msg.str());
      }
      const auto po =
          std::string(BehaviourData::ComputePredictionOperator) + "-" + ktype;
      this->treatCodeBlock(o, po, modifier, true);
      for (const auto& h : o.hypotheses) {
        if (!this->mb.hasAttribute(h, BehaviourData::hasPredictionOperator)) {
          this->mb.setAttribute(h, BehaviourData::hasPredictionOperator, true);
        }
      }
    } else {
      this->treatUnsupportedCodeBlockOptions(o);
      this->treatCodeBlock(o, BehaviourData::ComputePredictionOperator,
                           modifier, true);
      for (const auto& h : o.hypotheses) {
        this->mb.setAttribute(h, BehaviourData::hasPredictionOperator, true);
      }
    }
  }  // end of treatPredictionOperator

  void BehaviourDSLCommon::treatParameter() {
    auto throw_if = [this](const bool b, const std::string& m) {
      if (b) {
        this->throwRuntimeError("BehaviourDSLCommon::treatParameter", m);
      }
    };
    std::set<Hypothesis> mh;
    this->readHypothesesList(mh);
    auto endOfTreatment = false;
    while ((this->current != this->tokens.end()) && (!endOfTreatment)) {
      const auto vtype = [this]() -> std::string {
        const auto otype = this->readVariableTypeIfPresent();
        if (!otype) {
          return "real";
        }
        return *otype;
      }();
      this->checkNotEndOfFile("BehaviourDSLCommon::treatParameter");
      const auto sname = this->current->value;
      const auto vname = tfel::unicode::getMangledString(sname);
      throw_if(!isValidIdentifier(vname),
               "variable given is not valid (read "
               "'" +
                   sname + "').");
      const auto lineNumber = this->current->line;
      ++(this->current);
      this->checkNotEndOfFile("BehaviourDSLCommon::treatParameter");
      const auto arraySize = this->readArrayOfVariablesSize(sname, true);
      this->checkNotEndOfFile("BehaviourDSLCommon::treatParameter");
      if ((this->current->value == "=") || (this->current->value == "{") ||
          (this->current->value == "(")) {
        std::string ci;  // closing initializer
        if (this->current->value == "{") {
          ci = "}";
        }
        if (this->current->value == "(") {
          throw_if(arraySize != 1u,
                   "invalid initalisation syntax for "
                   "the default values of an array of parameters.\n"
                   "Unexpected token '" +
                       current->value + "'");
          ci = ")";
        }
        ++(this->current);
        this->checkNotEndOfFile("BehaviourDSLCommon::treatParameter");
        if (arraySize != 1u) {
          if (ci != "}") {
            this->readSpecifiedToken("BehaviourDSLCommon::treatParameter", "{");
          }
          --(this->current);
          const auto r =
              this->readArrayOfDouble("BehaviourDSLCommon::treatParameter");
          throw_if(
              r.size() != arraySize,
              "number of values given does not match the numberf of parameters "
              "(" +
                  std::to_string(r.size()) + " vs +" +
                  std::to_string(arraySize) + ").\n");
          for (const auto& h : mh) {
            VariableDescription p;
            if (vname == sname) {
              p = VariableDescription(vtype, vname, arraySize, lineNumber);
            } else {
              p = VariableDescription(vtype, sname, vname, arraySize,
                                      lineNumber);
            }
            p.description = this->currentComment;
            this->mb.addParameter(h, p);
            for (decltype(r.size()) i = 0; i != r.size(); ++i) {
              this->mb.setParameterDefaultValue(h, vname, i, r[i]);
            }
          }
        } else {
          double value;
          std::istringstream converter(this->current->value);
          converter >> value;
          if (!converter || (!converter.eof())) {
            this->throwRuntimeError(
                "BehaviourDSLCommon::treatParameter",
                "could not read default value for parameter '" + sname + "'");
          }
          ++(this->current);
          this->checkNotEndOfFile("BehaviourDSLCommon::treatParameter");
          if (!ci.empty()) {
            this->readSpecifiedToken("BehaviourDSLCommon::treatParameter", ci);
          }
          for (const auto& h : mh) {
            VariableDescription p;
            if (vname == sname) {
              p = VariableDescription(vtype, vname, 1u, lineNumber);
            } else {
              p = VariableDescription(vtype, sname, vname, 1u, lineNumber);
            }
            p.description = this->currentComment;
            this->mb.addParameter(h, p);
            this->mb.setParameterDefaultValue(h, vname, value);
          }
        }
      } else {
        throw_if(arraySize != 1,
                 "default values of parameters array "
                 "must be defined with the array. "
                 "Unexpected token '" +
                     current->value + "'");
        for (const auto& h : mh) {
          VariableDescription p;
          if (vname == sname) {
            p = VariableDescription(vtype, vname, 1u, lineNumber);
          } else {
            p = VariableDescription(vtype, sname, vname, 1u, lineNumber);
          }
          p.description = this->currentComment;
          this->mb.addParameter(h, p);
        }
      }
      if (this->current->value == ",") {
        ++(this->current);
      } else if (this->current->value == ";") {
        endOfTreatment = true;
        ++(this->current);
      } else {
        throw_if(true, "',' or ';' expected after '" + sname + "', read '" +
                           this->current->value + "'");
      }
    }
    if (!endOfTreatment) {
      --(this->current);
      throw_if(true, "expected ';' before end of file");
    }
  }  // end of treatParameter

  void BehaviourDSLCommon::treatInitLocalVariables() {
    std::function<std::string(const Hypothesis, const std::string&, const bool)>
        m = [this](const Hypothesis h, const std::string& sv, const bool b) {
          return this->standardModifier(h, sv, b);
        };
    this->treatCodeBlock(BehaviourData::InitializeLocalVariables,  //
                         m, true, true);
  }  // end of BehaviourDSLCommon:treatInitLocalVariables

  void BehaviourDSLCommon::treatMinimalTimeStepScalingFactor() {
    const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    double r_dt;
    this->checkNotEndOfFile(
        "BehaviourDSLCommon::treatMinimalTimeStepScalingFactor",
        "Cannot read value.");
    std::istringstream flux(current->value);
    flux >> r_dt;
    if ((flux.fail()) || (!flux.eof())) {
      this->throwRuntimeError(
          "BehaviourDSLCommon::treatMinimalTimeStepScalingFactor",
          "Failed to read value.");
    }
    if (r_dt < 10 * std::numeric_limits<double>::min()) {
      this->throwRuntimeError(
          "BehaviourDSLCommon::treatMinimalTimeStepScalingFactor",
          "minimal time step scaling factor either too "
          "low value or negative.");
    }
    ++(this->current);
    this->readSpecifiedToken(
        "BehaviourDSLCommon::treatMinimalTimeStepScalingFactor", ";");
    VariableDescription e("real", "minimal_time_step_scaling_factor", 1u, 0u);
    e.description = "minimal value for the time step scaling factor";
    this->mb.addParameter(h, e, BehaviourData::ALREADYREGISTRED);
    this->mb.setParameterDefaultValue(h, "minimal_time_step_scaling_factor",
                                      r_dt);
    this->mb.setEntryName(h, "minimal_time_step_scaling_factor",
                          "minimal_time_step_scaling_factor");
  }  // end of treatMinimalTimeStepScalingFactor

  void BehaviourDSLCommon::treatMaximalTimeStepScalingFactor() {
    const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    double r_dt;
    this->checkNotEndOfFile(
        "BehaviourDSLCommon::treatMaximalTimeStepScalingFactor",
        "Cannot read value.");
    std::istringstream flux(current->value);
    flux >> r_dt;
    if ((flux.fail()) || (!flux.eof())) {
      this->throwRuntimeError(
          "BehaviourDSLCommon::treatMaximalTimeStepScalingFactor",
          "Failed to read value.");
    }
    if (r_dt < 1) {
      this->throwRuntimeError(
          "BehaviourDSLCommon::treatMaximalTimeStepScalingFactor",
          "maximal time step scaling factor value either too "
          "low or negative.");
    }
    ++(this->current);
    this->readSpecifiedToken(
        "BehaviourDSLCommon::treatMaximalTimeStepScalingFactor", ";");
    VariableDescription e("real", "maximal_time_step_scaling_factor", 1u, 0u);
    e.description = "maximal value for the time step scaling factor";
    this->mb.addParameter(h, e, BehaviourData::ALREADYREGISTRED);
    this->mb.setParameterDefaultValue(h, "maximal_time_step_scaling_factor",
                                      r_dt);
    this->mb.setEntryName(h, "maximal_time_step_scaling_factor",
                          "maximal_time_step_scaling_factor");
  }  // end of treatMaximalTimeStepScalingFactor

  void BehaviourDSLCommon::setMinimalTangentOperator() {
    if (this->mb.getBehaviourType() !=
        BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
      for (const auto& h : this->mb.getDistinctModellingHypotheses()) {
        // basic check
        if (this->mb.hasAttribute(
                h, BehaviourData::hasConsistentTangentOperator)) {
          if (!this->mb.hasCode(h, BehaviourData::ComputeTangentOperator)) {
            this->throwRuntimeError(
                "BehaviourDSLCommon::setMinimalTangentOperator",
                "behaviour has attribute 'hasConsistentTangentOperator' but "
                "no associated code");
          }
        }
      }
      if (this->mb.getAttribute(BehaviourDescription::requiresStiffnessTensor,
                                false)) {
        if (this->mb.getBehaviourType() ==
            BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
          const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
          // if the user provided a tangent operator, it won't be
          // overriden
          CodeBlock tangentOperator;
          std::ostringstream code;
          code << "if(smt == ELASTIC){\n"
               << "this->Dt = this->D;\n"
               << "} else {\n"
               << "return false;\n"
               << "}\n";
          tangentOperator.code = code.str();
          tangentOperator.members.insert({"Dt", "D"});
          this->mb.setCode(h, BehaviourData::ComputeTangentOperator,
                           tangentOperator, BehaviourData::CREATEBUTDONTREPLACE,
                           BehaviourData::BODY);
          this->mb.setAttribute(h, BehaviourData::hasConsistentTangentOperator,
                                true, true);
        }
      }
    }
  }  // end of setMinimalTangentOperator

  void BehaviourDSLCommon::treatInitialize() {
    CodeBlockOptions o;
    this->readCodeBlockOptions(o, true);
    //
    auto id = std::string{};
    auto po = o.untreated.begin();
    const auto poe = o.untreated.end();
    while (po != poe) {
      const auto& opt = *po;
      if (!ModellingHypothesis::isModellingHypothesis(opt.value)) {
        id = opt.value;
        if (!isValidIdentifier(id)) {
          this->throwRuntimeError(
              "BehaviourDSLCommon::treatInitialize",
              "initialize function identifier is invalid (read '" + id + "').");
        }
        o.untreated.erase(po);
        break;
      }
      ++po;
    }
    if (id.empty()) {
      this->throwRuntimeError("BehaviourDSLCommon::treatInitialize",
                              "no initialize function identifier given");
    }
    this->treatUnsupportedCodeBlockOptions(o);
    const auto n =
        std::string(BehaviourData::UserDefinedInitializeCodeBlock) + id;
    std::function<std::string(const Hypothesis, const std::string&, const bool)>
        m = [this](const Hypothesis h, const std::string& sv, const bool bv) {
          return this->standardModifier(h, sv, bv);
        };
    for (const auto h : o.hypotheses) {
      const auto& d = this->mb.getBehaviourData(h);
      const auto c = this->readNextBlock(h, n, m, true);
      for (const auto& mn : c.members) {
        if ((d.isStateVariableIncrementName(mn)) ||
            (d.isExternalStateVariableIncrementName(mn))) {
          this->throwRuntimeError("BehaviourDSLCommon::treatInitialize",
                                  "variable '" + mn + "' is not allowed here");
        }
      }
      this->mb.setCode(h, n, c, o.m, o.p);
    }
  }  // end of treatInitialize

  void BehaviourDSLCommon::treatInternalEnergy() {
    std::function<std::string(const Hypothesis, const std::string&, const bool)>
        m = [this](const Hypothesis h, const std::string& sv, const bool b) {
          return this->standardModifier(h, sv, b);
        };
    this->treatCodeBlock(BehaviourData::ComputeInternalEnergy, m, true, true);
  }  // end of treatInternalEnergy

  void BehaviourDSLCommon::treatDissipatedEnergy() {
    std::function<std::string(const Hypothesis, const std::string&, const bool)>
        m = [this](const Hypothesis h, const std::string& sv, const bool b) {
          return this->standardModifier(h, sv, b);
        };
    this->treatCodeBlock(BehaviourData::ComputeDissipatedEnergy, m, true, true);
  }  // end of treatDissipatedEnergy

  static BehaviourDescription::SlipSystem readSlipSystem(
      BehaviourDSLCommon::CxxTokenizer::const_iterator& p,
      const BehaviourDSLCommon::CxxTokenizer::const_iterator pe) {
    using tfel::material::SlipSystemsDescription;
    using tfel::utilities::CxxTokenizer;
    auto throw_if = [](const bool c, const std::string& msg) {
      tfel::raise_if(c, "readSlipSystem: " + msg);
    };
    const auto direction =
        CxxTokenizer::readList("readSlipSystem", "<", ">", p, pe);
    const auto plane =
        CxxTokenizer::readList("readSlipSystem", "{", "}", p, pe);
    throw_if(plane.size() != direction.size(),
             "plane and direction don't match in size");
    throw_if((plane.size() != 3u) && (plane.size() != 4u),
             "invalid definition of a plane "
             "(must be an array of 3 or 4 integers, read '" +
                 std::to_string(plane.size()) + "' values)");
    if (plane.size() == 3u) {
      SlipSystemsDescription::system3d s3d;
      for (tfel::math::tvector<3u, int>::size_type i = 0; i != 3; ++i) {
        s3d.plane[i] = std::stoi(plane[i].value);
        s3d.burgers[i] = std::stoi(direction[i].value);
      }
      return {s3d};
    }
    SlipSystemsDescription::system4d s4d;
    for (tfel::math::tvector<4u, int>::size_type i = 0; i != 4; ++i) {
      s4d.plane[i] = std::stoi(plane[i].value);
      s4d.burgers[i] = std::stoi(direction[i].value);
    }
    return {s4d};
  }

  void BehaviourDSLCommon::treatSlipSystem() {
    const auto s = readSlipSystem(this->current, this->tokens.end());
    this->mb.setSlipSystems({1u, s});
    this->readSpecifiedToken("BehaviourDSLCommon::treatSlipSystem", ";");
  }  // end of treatSlipSystem()

  void BehaviourDSLCommon::treatSlipSystems() {
    const std::string m = "BehaviourDSLCommon::treatSlipSystems";
    std::vector<BehaviourDescription::SlipSystem> ss;
    this->readSpecifiedToken(m, "{");
    this->checkNotEndOfFile(m, "expected token");
    while (this->current->value != "}") {
      this->checkNotEndOfFile(m, "expected slip system");
      ss.push_back(readSlipSystem(this->current, this->tokens.end()));
      this->checkNotEndOfFile(m, "expected ',' or '}'");
      if (this->current->value != "}") {
        this->readSpecifiedToken(m, ",");
        this->checkNotEndOfFile(m, "expected slip system");
        if (this->current->value == "}") {
          this->throwRuntimeError(m, "unexpected token '}'");
        }
      }
    }
    this->readSpecifiedToken(m, "}");
    this->readSpecifiedToken(m, ";");
    this->mb.setSlipSystems(ss);
  }  // end of treatSlipSystems

  void BehaviourDSLCommon::treatCrystalStructure() {
    using tfel::material::CrystalStructure;
    this->checkNotEndOfFile("BehaviourDSLCommon::treatCrystalStructure",
                            "expected crystal structure");
    if (this->current->value == "Cubic") {
      this->mb.setCrystalStructure(CrystalStructure::Cubic);
    } else if (this->current->value == "FCC") {
      this->mb.setCrystalStructure(CrystalStructure::FCC);
    } else if (this->current->value == "BCC") {
      this->mb.setCrystalStructure(CrystalStructure::BCC);
    } else if (this->current->value == "HCP") {
      this->mb.setCrystalStructure(CrystalStructure::HCP);
    } else {
      this->throwRuntimeError("BehaviourDSLCommon::treatCrystalStructure",
                              "unsupported crystal structure "
                              "'" +
                                  this->current->value + "'");
    }
    ++(this->current);
    this->readSpecifiedToken("BehaviourDSLCommon::treatCrystalStructure", ";");
  }  // end of treatCrystalStructure

  void BehaviourDSLCommon::treatInteractionMatrix() {
    auto throw_if = [this](const bool b, const std::string& m) {
      if (b) {
        this->throwRuntimeError("BehaviourDSLCommon::treatInteractionMatrix",
                                m);
      }
    };
    throw_if(!this->mb.areSlipSystemsDefined(),
             "slip systems have not been defined");
    const auto& im = this->mb.getInteractionMatrixStructure();
    const auto r = im.rank();
    const auto mv =
        CxxTokenizer::readArray("BehaviourDSLCommon::treatInteractionMatrix",
                                this->current, this->tokens.end());
    this->readSpecifiedToken("BehaviourDSLCommon::treatInteractionMatrix", ";");
    throw_if(mv.size() != r,
             "the number of values does "
             "not match the number of independent coefficients "
             "in the interaction matrix");
    auto imv = std::vector<long double>{};
    imv.reserve((mv.size()));
    for (const auto& v : mv) {
      imv.push_back(tfel::utilities::convert<long double>(v));
    }
    this->mb.setInteractionMatrix(imv);
  }  // end of treatInteractionMatrix

  void BehaviourDSLCommon::treatDislocationsMeanFreePathInteractionMatrix() {
    auto throw_if = [this](const bool b, const std::string& m) {
      if (b) {
        this->throwRuntimeError(
            "BehaviourDSLCommon::"
            "treatDislocationsMeanFreePathInteractionMatrix",
            m);
      }
    };
    throw_if(!this->mb.areSlipSystemsDefined(),
             "slip systems have not been defined");
    const auto& im = this->mb.getInteractionMatrixStructure();
    const auto r = im.rank();
    const auto mv = CxxTokenizer::readArray(
        "BehaviourDSLCommon::"
        "treatDislocationsMeanFreePathInteractionMatrix",
        this->current, this->tokens.end());
    this->readSpecifiedToken(
        "BehaviourDSLCommon::"
        "treatDislocationsMeanFreePathInteractionMatrix",
        ";");
    throw_if(mv.size() != r,
             "the number of values does "
             "not match the number of independent coefficients "
             "in the interaction matrix");
    auto imv = std::vector<long double>{};
    imv.reserve((mv.size()));
    for (const auto& v : mv) {
      imv.push_back(tfel::utilities::convert<long double>(v));
    }
    this->mb.setDislocationsMeanFreePathInteractionMatrix(imv);
  }  // end of treatDislocationsMeanFreePathInteractionMatrix

  void BehaviourDSLCommon::
      setComputeFinalThermodynamicForcesFromComputeFinalThermodynamicForcesCandidateIfNecessary() {
    // first treating specialised mechanical data
    for (const auto& h : this->mb.getDistinctModellingHypotheses()) {
      if (h != ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
        if (!this->mb.hasCode(h,
                              BehaviourData::ComputeFinalThermodynamicForces)) {
          if (this->mb.hasCode(
                  h, BehaviourData::ComputeFinalThermodynamicForcesCandidate)) {
            this->mb.setCode(
                h, BehaviourData::ComputeFinalThermodynamicForces,
                this->mb.getCodeBlock(
                    h, BehaviourData::ComputeFinalThermodynamicForcesCandidate),
                BehaviourData::CREATE, BehaviourData::BODY);
          }
        }
      }
    }
    // now treating the default hypothesis case
    if (!this->mb.areAllMechanicalDataSpecialised()) {
      const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      if (!this->mb.hasCode(h,
                            BehaviourData::ComputeFinalThermodynamicForces)) {
        if (this->mb.hasCode(
                h, BehaviourData::ComputeFinalThermodynamicForcesCandidate)) {
          this->mb.setCode(
              h, BehaviourData::ComputeFinalThermodynamicForces,
              this->mb.getCodeBlock(
                  h, BehaviourData::ComputeFinalThermodynamicForcesCandidate),
              BehaviourData::CREATEBUTDONTREPLACE, BehaviourData::BODY);
        }
      }
    }
  }  // end of
     // setComputeFinalThermodynamicForcesFromComputeFinalThermodynamicForcesCandidateIfNecessary

  std::string BehaviourDSLCommon::getOverridableVariableNameByExternalName(
      const std::string& en) const {
    constexpr auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    const auto& d = this->mb.getBehaviourData(uh);
    const auto& parameters = d.getParameters();
    const auto pp = findByExternalName(parameters, en);
    if (pp != parameters.end()) {
      return pp->name;
    }
    const auto& mps = d.getMaterialProperties();
    const auto pmp = findByExternalName(mps, en);
    if (pmp != mps.end()) {
      return pmp->name;
    }
    const auto& esvs = d.getExternalStateVariables();
    const auto pesv = findByExternalName(esvs, en);
    if (pesv == esvs.end()) {
      tfel::raise(
          "BehaviourDSLCommon::getOverridableVariableNameByExternalName: "
          "no overridable variable associated with external name '" +
          en + "'");
    }
    return pesv->name;
  }  // end of getOverridableVariableNameByExternalName

  void BehaviourDSLCommon::overrideByAParameter(const std::string& n,
                                                const double v) {
    this->mb.overrideByAParameter(n, v);
  }  // end of overrideByAParameter

  std::map<std::string, double> BehaviourDSLCommon::getOverridenParameters()
      const {
    return this->mb.getOverridenParameters();
  }  // end of getOverridenParameters

  std::map<std::string, std::shared_ptr<AbstractBehaviourInterface>>
  BehaviourDSLCommon::getBehaviourInterfaces() const {
    return this->interfaces;
  }  // end of getBehaviourInterfaces

  void BehaviourDSLCommon::checkTangentOperatorBlock(const std::string& cname,
                                                     const CodeBlock& c) const {
    if (isSafe(c)) {
      return;
    }
    if (this->mb.getBehaviourType() ==
        BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
      if (!c.members.contains("Dt")) {
        reportWarning("the variable 'Dt' is not used in the '" + cname +
                      "' code block.");
      }
      return;
    }
    if (this->mb.getBehaviourType() ==
        BehaviourDescription::COHESIVEZONEMODEL) {
      const auto uses_Dt = c.members.contains("Dt");
      const auto uses_Dt_c =
          c.members.contains("Dt_nn") || c.members.contains("Dt_tt") ||
          c.members.contains("Dt_tn") || c.members.contains("Dt_nt");
      if ((!uses_Dt) && (!uses_Dt_c)) {
        reportWarning("the variable 'Dt' is not used in the '" + cname +
                      "' code block, nor its block decomposition ('Dt_nn', "
                      "'Dt_tt', 'Dt_tn', 'Dt_nt').");
      }
      if ((uses_Dt) && (uses_Dt_c)) {
        reportWarning(
            "both the variable 'Dt' and some parts of its block decomposition "
            "('Dt_nn', 'Dt_tt', 'Dt_tn', 'Dt_nt') are used in the '" +
            cname + "' code block. This is unexpected.");
      }
      if (uses_Dt_c) {
        if (!c.members.contains("Dt_nn")) {
          reportWarning(
              "some parts of its block decomposition of the tangent operator "
              "are used in the '" +
              cname + "' code block, but not 'Dt_nn'). This is unexpected.");
        }
        if (!c.members.contains("Dt_tt")) {
          reportWarning(
              "some parts of its block decomposition of the tangent operator "
              "are used in the '" +
              cname + "' code block, but not 'Dt_tt'). This is unexpected.");
        }
      }
      return;
    }
    auto used_tblocks =
        std::vector<std::pair<VariableDescription, VariableDescription>>{};
    auto unused_tblocks =
        std::vector<std::pair<VariableDescription, VariableDescription>>{};
    const auto& tblocks = this->mb.getTangentOperatorBlocks();
    for (const auto& tblock : tblocks) {
      const auto bn = this->mb.getTangentOperatorBlockName(tblock);
      if (c.members.contains(bn)) {
        used_tblocks.push_back(tblock);
      } else {
        unused_tblocks.push_back(tblock);
      }
    }
    const auto uses_Dt = c.members.contains("Dt");
    if ((!this->mb.hasTrivialTangentOperatorStructure()) && (uses_Dt)) {
      reportWarning(
          "the consistent tangent operator has a non trivial structure so "
          "using directly the variable 'Dt' in the '" +
          cname + "' code block is unexpected.");
    }
    if ((!uses_Dt) && (used_tblocks.empty())) {
      reportWarning(
          "nor the variable 'Dt' or any of the tangent operator blocks "
          "(" +
          makeTangentOperatorBlocksList(this->mb, unused_tblocks) +
          ") is used in the '" + cname + "' code block.");
    }
    if ((uses_Dt) && (!used_tblocks.empty())) {
      reportWarning(
          "both the variable 'Dt' and some tangent operator blocks (" +
          makeTangentOperatorBlocksList(this->mb, used_tblocks) +
          ") are used in the '" + cname +
          "' code block, which is unexepected.");
    }
    if ((!used_tblocks.empty()) && (!unused_tblocks.empty())) {
      reportWarning("some tangent operator blocks (" +
                    makeTangentOperatorBlocksList(this->mb, used_tblocks) +
                    ") are used while other are not (" +
                    makeTangentOperatorBlocksList(this->mb, unused_tblocks) +
                    ") in the '" + cname +
                    "' code block, which is unexepected.");
    }
  }  // end of checkTangentOperatorBlock

  BehaviourDSLCommon::~BehaviourDSLCommon() = default;

}  // end of namespace mfront
