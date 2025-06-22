/*!
 * \file   mfront/src/BehaviourDSLCommon.cxx
 * \brief
 *
 * \author Helfer Thomas
 * \date   05 mai 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <algorithm>
#include <iterator>
#include <vector>
#include <sstream>
#include <utility>
#include <stdexcept>
#include <limits>
#include <cctype>
#include <cmath>

#include "TFEL/System/System.hxx"
#include "TFEL/Glossary/Glossary.hxx"
#include "TFEL/Glossary/GlossaryEntry.hxx"
#include "TFEL/Material/FiniteStrainBehaviourTangentOperator.hxx"
#include "TFEL/Utilities/Data.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"

#include "MFront/MFront.hxx"
#include "MFront/MFrontHeader.hxx"
#include "MFront/DSLUtilities.hxx"
#include "MFront/MFrontUtilities.hxx"
#include "MFront/MFrontDebugMode.hxx"
#include "MFront/PedanticMode.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MFront/SearchFile.hxx"
#include "MFront/AbstractBehaviourInterface.hxx"
#include "MFront/MFrontMaterialPropertyInterface.hxx"
#include "MFront/PerformanceProfiling.hxx"
#include "MFront/BehaviourInterfaceFactory.hxx"
#include "MFront/FiniteStrainBehaviourTangentOperatorConversionPath.hxx"
#include "MFront/AbstractBehaviourBrick.hxx"
#include "MFront/AbstractBehaviourBrickFactory.hxx"
#include "MFront/TargetsDescription.hxx"
#include "MFront/MaterialPropertyDescription.hxx"
#include "MFront/ModelDSL.hxx"
#include "MFront/MFrontModelInterface.hxx"
#include "MFront/BehaviourDSLCommon.hxx"
#include "MFront/BehaviourBrick/Requirement.hxx"
#include "MFront/BehaviourBrick/RequirementManager.hxx"

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

#ifndef _MSC_VER
static const char* const constexpr_c = "constexpr";
#else
static const char* const constexpr_c = "const";
#endif

namespace mfront {

  void BehaviourDSLCommon::writeModelCall(std::ostream& out,
                                          const Hypothesis h,
                                          const ModelDescription& md,
                                          const std::string& vo,
                                          const std::string& vs,
                                          const std::string& bn) {
    auto throw_if = [this](const bool b, const std::string& m) {
      if (b) {
        this->throwRuntimeError("BehaviourDSLCommon::writeModelCall", m);
      }
    };
    auto write_variable = [throw_if, &md, &out](const std::string& v,
                                                const unsigned short d) {
      if (d == 0) {
        out << "this->" << v << "+this->d" << v;
      } else if (d == 1) {
        out << "this->" << v;
      } else {
        throw_if(true, "invalid depth for the temperature '" + v +
                           "' "
                           "in model '" +
                           md.className + "'");
      }
    };
    const auto& bd = this->mb.getBehaviourData(h);
    throw_if(md.outputs.size() != 1u,
             "invalid number of outputs for model '" + md.className + "'");
    throw_if(!md.constantMaterialProperties.empty(),
             "constant material properties are not supported yet");
    throw_if(md.functions.size() != 1u,
             "invalid number of functions in model '" + md.className + "'");
    const auto& f = md.functions[0];
    throw_if(f.modifiedVariables.empty(),
             "no modified variable for function '" + f.name + "'");
    throw_if(f.modifiedVariables.size() != 1u,
             "invalid number of functions in model '" + md.className + "'");
    throw_if(f.name.empty(), "unnamed function");
    throw_if((f.usedVariables.empty()) && (!f.useTimeIncrement),
             "no used variable for function '" + f.name + "'");
    const auto sm = this->getTemporaryVariableName(bn);
    out << "// updating " << vs << "\n"
        << "mfront::" << md.className << "<Type> " << sm << ";\n"
        << "" << sm << ".setOutOfBoundsPolicy(this->policy);\n"
        << "this->" << vo << " = " << sm << "." << f.name << "(";
    const auto args = [&f]() {
      auto a = std::vector<std::string>{};
      for (const auto& uv : f.usedVariables) {
        a.push_back(uv);
      }
      if (f.useTimeIncrement) {
        a.emplace_back("dt");
      }
      return a;
    }();
    const auto asvn = bd.getExternalNames(bd.getAuxiliaryStateVariables());
    for (auto pa = std::begin(args); pa != std::end(args);) {
      if (*pa == "dt") {
        out << "this->dt";
        ++pa;
        continue;
      }
      const auto a = md.decomposeVariableName(*pa);
      const auto& ea = md.getExternalName(a.first);
      if (ea == bd.getExternalName(vs)) {
        throw_if(a.second != 1, "invalid depth for variable '" + a.first +
                                    "' "
                                    "in model '" +
                                    md.className + "'");
        out << "this->" << vs;
      } else if (ea == tfel::glossary::Glossary::Temperature) {
        write_variable("T", a.second);
      } else if (std::find(std::begin(asvn), std::end(asvn), ea) !=
                 std::end(asvn)) {
        const auto& av =
            bd.getAuxiliaryStateVariableDescriptionByExternalName(ea);
        throw_if(!av.getAttribute<bool>("ComputedByExternalModel", false),
                 "only auxiliary state variable computed by a model are "
                 "allowed here");
        write_variable(av.name, a.second);
      } else {
        const auto& en =
            bd.getExternalStateVariableDescriptionByExternalName(ea);
        write_variable(en.name, a.second);
      }
      if (++pa != std::end(args)) {
        out << ",";
      }
    }
    out << ");\n";
  }

  BehaviourDSLCommon::CodeBlockOptions::CodeBlockOptions()
      : p(BehaviourData::BODY), m(BehaviourData::CREATE) {
    this->hypotheses.insert(ModellingHypothesis::UNDEFINEDHYPOTHESIS);
  }  // end of BehaviourDSLCommon::CodeBlockOptions::CodeBlockOptions

  BehaviourDSLCommon::CodeBlockOptions::~CodeBlockOptions() = default;

  const BehaviourDescription& BehaviourDSLCommon::getBehaviourDescription()
      const {
    return this->mb;
  }  // end of BehaviourDSLCommon::getBehaviourDescription

  std::string BehaviourDSLCommon::getClassName() const {
    return this->mb.getClassName();
  }  // end of BehaviourDSLCommon::getClassName

  void BehaviourDSLCommon::addMaterialLaw(const std::string& m) {
    this->mb.addMaterialLaw(m);
  }  // end of BehaviourDSLCommon::addMaterialLaw

  void BehaviourDSLCommon::appendToIncludes(const std::string& c) {
    this->mb.appendToIncludes(c);
  }  // end of BehaviourDSLCommon::appendToIncludes

  void BehaviourDSLCommon::appendToMembers(const std::string& c) {
    this->mb.appendToMembers(ModellingHypothesis::UNDEFINEDHYPOTHESIS, c, true);
  }  // end of BehaviourDSLCommon::appendToMembers

  void BehaviourDSLCommon::appendToPrivateCode(const std::string& c) {
    this->mb.appendToPrivateCode(ModellingHypothesis::UNDEFINEDHYPOTHESIS, c,
                                 true);
  }  // end of BehaviourDSLCommon::appendToPrivateCode

  void BehaviourDSLCommon::appendToSources(const std::string& c) {
    this->mb.appendToSources(c);
  }  // end of BehaviourDSLCommon::appendToSources

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
  }  // end of BehaviourDSLCommon::appendToHypothesesList

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
      if (t.value == "Append") {
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
        this->throwRuntimeError("BehaviourDSLCommon::readCodeBlockOptions : ",
                                "specialisation is not allowed");
      }
      if (*(o.hypotheses.begin()) != dh) {
        this->throwRuntimeError("BehaviourDSLCommon::readCodeBlockOptions : ",
                                "specialisation is not allowed");
      }
    }
  }  // end of BehaviourDSLCommon::readCodeBlockOptions

  std::shared_ptr<MaterialPropertyDescription>
  BehaviourDSLCommon::handleMaterialPropertyDescription(const std::string& f) {
    return DSLBase::handleMaterialPropertyDescription(f);
  }  // end of BehaviourDSLCommon::handleMaterialPropertyDescription

  ModelDescription BehaviourDSLCommon::getModelDescription(
      const std::string& f) {
    if (getVerboseMode() >= VERBOSE_DEBUG) {
      getLogStream() << "BehaviourDSLCommon::treatModel: treating file '" << f
                     << "'\n";
    }
    // getting informations the source files
    const auto path = SearchFile::search(f);
    ModelDSL dsl;
    try {
      dsl.analyseFile(path, {}, {});
    } catch (std::exception& e) {
      this->throwRuntimeError(
          "BehaviourDSLCommon::getModelDescription",
          "error while treating file '" + f + "'\n" + std::string(e.what()));
    } catch (...) {
      this->throwRuntimeError("BehaviourDSLCommon::getModelDescription",
                              "error while treating file '" + f + "'");
    }
    // generating the sources
    try {
      MFront m;
      m.setInterface("mfront");
      const auto t = m.treatFile(path);
      if (!t.specific_targets.empty()) {
        this->throwRuntimeError("BehaviourDSLCommon::getModelDescription",
                                "error while treating file '" + f +
                                    "'.\n"
                                    "Specific targets are not supported");
      }
      for (const auto& h : t.headers) {
        this->appendToIncludes("#include\"" + h + "\"");
      }
      this->atds.push_back(t);
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
      getLogStream() << "BehaviourDSLCommon::treatModel: end of file '" << f
                     << "' treatment\n";
    }
    return md;
  }  // end of BehaviourDSLCommon::getModelDescription

  void BehaviourDSLCommon::treatModel() {
    if (getVerboseMode() >= VERBOSE_DEBUG) {
      getLogStream() << "BehaviourDSLCommon::treatModel: begin\n";
    }
    auto md = this->getModelDescription(
        this->readString("BehaviourDSLCommon::treatModel"));
    this->mb.addModelDescription(md);
    if (getVerboseMode() >= VERBOSE_DEBUG) {
      getLogStream() << "BehaviourDSLCommon::treatModel: end\n";
    }
    this->readSpecifiedToken("BehaviourDSLCommon::treatModel", ";");
  }  // end of BehaviourDSLCommon::treatModel

  void BehaviourDSLCommon::treatUnsupportedCodeBlockOptions(
      const CodeBlockOptions& o) {
    if (!o.untreated.empty()) {
      std::ostringstream msg;
      msg << "BehaviourDSLCommon::treatUnsupportedCodeBlockOptions : ";
      if (o.untreated.size() == 1u) {
        msg << "option '" << o.untreated[0].value << "' is invalid";
      } else {
        msg << "the";
        for (const auto& opt : o.untreated) {
          msg << " '" << opt.value << "'";
        }
        msg << " options are invalid";
      }
      throw(std::runtime_error(msg.str()));
    }
  }  // end of BehaviourDSLCommon::treatUnsupportedCodeBlockOptions

  void BehaviourDSLCommon::addStaticVariableDescription(
      const StaticVariableDescription& v) {
    this->mb.addStaticVariable(ModellingHypothesis::UNDEFINEDHYPOTHESIS, v);
  }  // end of BehaviourDSLCommon::addStaticVariableDescription

  std::set<BehaviourDSLCommon::Hypothesis>
  BehaviourDSLCommon::getDefaultModellingHypotheses() const {
    return {ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN,
            ModellingHypothesis::AXISYMMETRICAL,
            ModellingHypothesis::PLANESTRAIN,
            ModellingHypothesis::GENERALISEDPLANESTRAIN,
            ModellingHypothesis::TRIDIMENSIONAL};
  }  // end of BehaviourDSLCommon::getDefaultModellingHypotheses

  bool BehaviourDSLCommon::isModellingHypothesisSupported(
      const Hypothesis) const {
    return true;
  }  // end of BehaviourDSLCommon::isModellingHypothesesSupported

  void BehaviourDSLCommon::analyseFile(
      const std::string& fileName_,
      const std::vector<std::string>& ecmds,
      const std::map<std::string, std::string>& s) {
    this->importFile(fileName_, ecmds, s);
    // Adding some stuff
    this->endsInputFileProcessing();
    // setting the name of the output files
    this->behaviourFileName = "TFEL/Material/";
    this->behaviourFileName += this->mb.getClassName();
    this->behaviourFileName += ".hxx";
    this->behaviourDataFileName = "TFEL/Material/";
    this->behaviourDataFileName += this->mb.getClassName();
    this->behaviourDataFileName += "BehaviourData.hxx";
    this->integrationDataFileName = "TFEL/Material/";
    this->integrationDataFileName += this->mb.getClassName();
    this->integrationDataFileName += "IntegrationData.hxx";
    this->srcFileName = this->mb.getClassName();
    this->srcFileName += ".cxx";
    // targets description
    for (const auto& i : this->interfaces) {
      i.second->getTargetsDescription(this->td, this->mb);
    }
    for (auto& l : this->td) {
      insert_if(this->td[l.name].sources, this->srcFileName);
    }
    insert_if(this->td.headers, this->behaviourFileName);
    insert_if(this->td.headers, this->behaviourDataFileName);
    insert_if(this->td.headers, this->integrationDataFileName);
    this->completeTargetsDescription();
  }

  void BehaviourDSLCommon::endsInputFileProcessing() {
    using namespace mfront::bbrick;
    const auto& g = tfel::glossary::Glossary::getGlossary();
    if (getVerboseMode() >= VERBOSE_DEBUG) {
      getLogStream() << "BehaviourDSLCommon::endsInputFileProcessing : begin\n";
    }
    // defining modelling hypotheses
    if (!this->mb.areModellingHypothesesDefined()) {
      this->mb.setModellingHypotheses(this->getDefaultModellingHypotheses());
    }
    const auto& mh = this->mb.getModellingHypotheses();
    // treating bricks
    if (!this->bricks.empty()) {
      if (getVerboseMode() >= VERBOSE_DEBUG) {
        getLogStream() << "BehaviourDSLCommon::endsInputFileProcessing : "
                       << "treating bricks\n";
      }
      for (const auto h : mh) {
        auto& d = this->mb.getBehaviourData(h);
        RequirementManager r{d, this->mb.useQt()};
        // for(const auto& pb : this->bricks){
        //   pb->declareProviders(r,h);
        // }
        for (const auto& pb : this->bricks) {
          pb->addRequirements(r, h);
        }
        // unmatched requirements
        auto umrqs = std::vector<std::string>{};
        const auto& urs = r.getUnresolvedRequirements();
        for (const auto& n : urs) {
          const auto s = SupportedTypes{};
          const auto& ur = r.getRequirement(n);
          if ((s.getTypeFlag(ur.type) != SupportedTypes::Scalar) ||
              (find(ur.aproviders.begin(), ur.aproviders.end(),
                    ProviderIdentifier::MATERIALPROPERTY) ==
               ur.aproviders.end())) {
            umrqs.push_back(ur.name);
          }
        }
        if (!umrqs.empty()) {
          std::string msg = "the following requirements can't be met: ";
          for (const auto& umrq : umrqs) {
            msg += "\n- " + umrq;
          }
          this->throwRuntimeError("BehaviourDSLCommon::endsInputFileProcessing",
                                  msg);
        }
        for (const auto& n : urs) {
          const auto& ur = r.getRequirement(n);
          this->mb.addMaterialProperty(h, {ur.type, ur.name, ur.asize, 0u});
          if (!g.contains(ur.name)) {
            this->mb.setEntryName(h, ur.name, ur.name);
          }
        }
      }
    }
    for (const auto& pb : this->bricks) {
      pb->endTreatment();
    }
    if (getVerboseMode() >= VERBOSE_DEBUG) {
      auto& log = getLogStream();
      log << "behaviour '" << this->mb.getClassName()
          << "' supports the following hypotheses : \n";
      for (const auto& h : mh) {
        log << " - " << ModellingHypothesis::toString(h);
        if (this->mb.hasSpecialisedMechanicalData(h)) {
          log << " (specialised)";
        }
        log << '\n';
      }
    }
    // time step scaling factors
    if (!this->mb.hasParameter(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                               "minimal_time_step_scaling_factor")) {
      VariableDescription e("real", "minimal_time_step_scaling_factor", 1u, 0u);
      e.description = "minimal value for the time step scaling factor";
      this->mb.addParameter(ModellingHypothesis::UNDEFINEDHYPOTHESIS, e,
                            BehaviourData::ALREADYREGISTRED);
      this->mb.setParameterDefaultValue(
          ModellingHypothesis::UNDEFINEDHYPOTHESIS,
          "minimal_time_step_scaling_factor", 0.1);
      this->mb.setEntryName(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                            "minimal_time_step_scaling_factor",
                            "minimal_time_step_scaling_factor");
    }
    if (!this->mb.hasParameter(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                               "maximal_time_step_scaling_factor")) {
      VariableDescription e("real", "maximal_time_step_scaling_factor", 1u, 0u);
      e.description = "maximal value for the time step scaling factor";
      this->mb.addParameter(ModellingHypothesis::UNDEFINEDHYPOTHESIS, e,
                            BehaviourData::ALREADYREGISTRED);
      this->mb.setParameterDefaultValue(
          ModellingHypothesis::UNDEFINEDHYPOTHESIS,
          "maximal_time_step_scaling_factor",
          std::numeric_limits<double>::max());
      this->mb.setEntryName(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                            "maximal_time_step_scaling_factor",
                            "maximal_time_step_scaling_factor");
    }
    // incompatible options
    if ((this->mb.getAttribute(BehaviourDescription::computesStiffnessTensor,
                               false)) &&
        (this->mb.getAttribute(BehaviourDescription::requiresStiffnessTensor,
                               false))) {
      this->throwRuntimeError(
          "BehaviourDSLCommon::endsInputFileProcessing",
          "internal error, incompatible options for stiffness tensor");
    }
    // check of stiffness tensor requirement
    if ((this->mb.getBehaviourType() ==
         BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) ||
        (this->mb.getBehaviourType() ==
         BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR)) {
      if ((mh.find(ModellingHypothesis::PLANESTRESS) != mh.end()) ||
          (mh.find(ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS) !=
           mh.end())) {
        if (this->mb.hasAttribute(
                BehaviourDescription::requiresStiffnessTensor)) {
          if (!this->mb.hasAttribute(
                  BehaviourDescription::requiresUnAlteredStiffnessTensor)) {
            this->throwRuntimeError(
                "BehaviourDSLCommon::endsInputFileProcessing",
                "No option was given to the '@RequireStiffnessTensor' "
                "keyword.\n"
                "For plane stress hypotheses, it is required to precise "
                "whether "
                "the expected stiffness tensor is 'Altered' (the plane stress "
                "hypothesis is taken into account) or 'UnAltered' (the "
                "stiffness "
                "tensor is the same as in plane strain)");
          }
        }
      }
    }
    // defining the reference temperature for thermal expansion
    if (this->mb.areThermalExpansionCoefficientsDefined()) {
      // check if the reference temperature is defined
      const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      if (!this->mb.isParameterName(
              uh, "referenceTemperatureForThermalExpansion")) {
        this->mb.addParameter(
            uh, VariableDescription(
                    "real", "referenceTemperatureForThermalExpansion", 1u, 0u));
        this->mb.setParameterDefaultValue(
            uh, "referenceTemperatureForThermalExpansion", 293.15);
      }
    }
    if (this->mb.getSymmetryType() == mfront::ORTHOTROPIC) {
      // if no orthotropic axes convention is defined, one can't compute
      // stiffness tensor, thermal expansion or stress free expansion
      // correctly, except for the 3D modelling hypothesis
      for (const auto h : this->mb.getDistinctModellingHypotheses()) {
        if (((this->mb.areElasticMaterialPropertiesDefined()) &&
             (this->mb.getElasticMaterialProperties().size() == 9u)) ||
            ((this->mb.areThermalExpansionCoefficientsDefined()) &&
             (this->mb.getThermalExpansionCoefficients().size() == 3u)) ||
            (this->mb.isStressFreeExansionAnisotropic(h))) {
          if (this->mb.getOrthotropicAxesConvention() ==
              OrthotropicAxesConvention::DEFAULT) {
            // in this case, only tridimensional case is supported
            if (h != ModellingHypothesis::TRIDIMENSIONAL) {
              this->throwRuntimeError(
                  "BehaviourDSLCommon::endsInputFileProcessing",
                  "An orthotropic axes convention must be choosen when "
                  "using one of @ComputeStiffnessTensor, "
                  "@ComputeThermalExpansion, @Swelling, @AxilalGrowth keywords "
                  "in behaviours which "
                  "shall be valid in other modelling hypothesis than "
                  "'Tridimensional'. This message was triggered because "
                  "either the thermal expansion or to the stiffness tensor "
                  "is orthotropic.\n"
                  "Either restrict the validity of the behaviour to "
                  "'Tridimensional' (see @ModellingHypothesis) or "
                  "choose and orthotropic axes convention as on option "
                  "to the @OrthotropicBehaviour keyword");
            }
          }
        }
      }
    }
    // restrictions on user defined compute stress free expansion
    for (const auto h : this->mb.getDistinctModellingHypotheses()) {
      const auto& d = this->mb.getBehaviourData(h);
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
    // calling interfaces
    for (const auto& i : this->interfaces) {
      i.second->allowDynamicallyAllocatedArrays(
          this->areDynamicallyAllocatedVectorsAllowed());
    }
    if (getPedanticMode()) {
      this->doPedanticChecks();
    }
    if (getVerboseMode() >= VERBOSE_DEBUG) {
      getLogStream() << "BehaviourDSLCommon::endsInputFileProcessing : end\n";
    }
  }  // end of BehaviourDSLCommon::endsInputFileProcessing

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
   * \param[in] b3 : check if glossary name is declared
   * \param[in] b4 : check if variable is used in more than one code block (test
   * for local variables)
   */
  static void performPedanticChecks(
      const BehaviourData& md,
      const VarContainer& v,
      const std::string& t,
      const std::map<std::string, unsigned short>& uv,
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
          log << "- " << t << " '" << vd.name << "' has no glossary name.\n";
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
      const std::map<std::string, unsigned short>& uv) {
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
    log << "\n* Pedantic checks\n";
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
      auto members =
          std::map<std::string, unsigned short>{};  // variable names and counts
      auto smembers =
          std::map<std::string,
                   unsigned short>{};  // static variable nanes and counts
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
  }  // end of BehaviourDSLCommon::pedanticChecks

  void BehaviourDSLCommon::generateOutputFiles() {
    tfel::system::systemCall::mkdir("src");
    tfel::system::systemCall::mkdir("include");
    tfel::system::systemCall::mkdir("include/TFEL/");
    tfel::system::systemCall::mkdir("include/TFEL/Material");
    this->behaviourFile.open("include/" + this->behaviourFileName);
    if (!this->behaviourFile) {
      this->throwRuntimeError("BehaviourDSLCommon::generateOutputFiles",
                              "unable to open '" + this->behaviourFileName +
                                  "' "
                                  "for writing output file");
    }
    this->behaviourDataFile.open("include/" + this->behaviourDataFileName);
    if (!this->behaviourDataFile) {
      this->throwRuntimeError("BehaviourDSLCommon::generateOutputFiles",
                              "unable to open '" + this->behaviourDataFileName +
                                  "' "
                                  "for writing output file");
    }
    this->integrationDataFile.open("include/" + this->integrationDataFileName);
    if (!this->integrationDataFile) {
      this->throwRuntimeError("BehaviourDSLCommon::generateOutputFiles",
                              "unable to open '" +
                                  this->integrationDataFileName +
                                  "' "
                                  "for writing output file");
    }
    this->srcFile.open("src/" + this->srcFileName);
    this->behaviourFile.precision(14);
    this->behaviourDataFile.precision(14);
    this->integrationDataFile.precision(14);
    this->srcFile.precision(14);
    if (!this->srcFile) {
      this->throwRuntimeError("BehaviourDSLCommon::generateOutputFiles",
                              "unable to open '" + this->srcFileName +
                                  "' "
                                  "for writing output file");
    }
    // generate outpout files
    this->writeBehaviourDataFileBegin();
    this->writeIntegrationDataFileBegin();
    this->writeBehaviourFileBegin();
    // modelling hypotheses handled by the interfaces (if at least one
    // interface is defined), or by the behaviour
    std::set<Hypothesis> hh;
    if (this->interfaces.empty()) {
      const auto& bh = this->mb.getModellingHypotheses();
      hh.insert(bh.begin(), bh.end());
    } else {
      // calling the interfaces
      for (const auto& i : this->interfaces) {
        const auto& ih = i.second->getModellingHypothesesToBeTreated(this->mb);
        hh.insert(ih.begin(), ih.end());
      }
    }
    if (!this->mb.areAllMechanicalDataSpecialised(hh)) {
      if (getVerboseMode() >= VERBOSE_DEBUG) {
        auto& log = getLogStream();
        log << "BehaviourDSLCommon::writeOutputFiles : "
            << "treating default hypothesis\n";
      }
      const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      // Generating BehaviourData's outputClass
      if (getVerboseMode() >= VERBOSE_DEBUG) {
        auto& log = getLogStream();
        log << "BehaviourDSLCommon::writeOutputFiles : writing behaviour data "
            << "for default hypothesis\n";
      }
      this->writeBehaviourDataClass(h);
      // Generating IntegrationData's outputClass
      if (getVerboseMode() >= VERBOSE_DEBUG) {
        auto& log = getLogStream();
        log << "BehaviourDSLCommon::writeOutputFiles : writing integration "
               "data "
            << "for default hypothesis\n";
      }
      this->writeIntegrationDataClass(h);
      // Generating Behaviour's outputFile
      if (getVerboseMode() >= VERBOSE_DEBUG) {
        auto& log = getLogStream();
        log << "BehaviourDSLCommon::writeOutputFiles : writing behaviour class "
            << "for default hypothesis\n";
      }
      this->writeBehaviourClass(h);
    }
    for (const auto& h : hh) {
      if (mb.hasSpecialisedMechanicalData(h)) {
        if (getVerboseMode() >= VERBOSE_DEBUG) {
          auto& log = getLogStream();
          log << "BehaviourDSLCommon::writeOutputFiles : "
              << "treating hypothesis '" << ModellingHypothesis::toString(h)
              << "'\n";
        }
        // Generating BehaviourData's outputClass
        if (getVerboseMode() >= VERBOSE_DEBUG) {
          auto& log = getLogStream();
          log << "BehaviourDSLCommon::writeOutputFiles : writing behaviour "
                 "data "
              << "for hypothesis '" << ModellingHypothesis::toString(h)
              << "'\n";
        }
        this->writeBehaviourDataClass(h);
        // Generating IntegrationData's outputClass
        if (getVerboseMode() >= VERBOSE_DEBUG) {
          auto& log = getLogStream();
          log << "BehaviourDSLCommon::writeOutputFiles : writing integration "
                 "data "
              << "for hypothesis '" << ModellingHypothesis::toString(h)
              << "'\n";
        }
        this->writeIntegrationDataClass(h);
        // Generating behaviour's outputClass
        if (getVerboseMode() >= VERBOSE_DEBUG) {
          auto& log = getLogStream();
          log << "BehaviourDSLCommon::writeOutputFiles : writing behaviour "
                 "class "
              << "for hypothesis '" << ModellingHypothesis::toString(h)
              << "'\n";
        }
        this->writeBehaviourClass(h);
      }
    }
    this->writeBehaviourDataFileEnd();
    this->writeIntegrationDataFileEnd();
    this->writeBehaviourFileEnd();
    // Generating behaviour's source file
    if (getVerboseMode() >= VERBOSE_DEBUG) {
      auto& log = getLogStream();
      log << "BehaviourDSLCommon::writeOutputFiles : writing source file\n";
    }
    this->writeSrcFile();
    // calling the interfaces
    for (const auto& i : this->interfaces) {
      if (getVerboseMode() >= VERBOSE_DEBUG) {
        auto& log = getLogStream();
        log << "BehaviourDSLCommon::writeOutputFiles : "
            << "calling interface '" << i.first << "'\n";
      }
      i.second->endTreatment(this->mb, *this);
    }
    behaviourFile.close();
    behaviourDataFile.close();
    integrationDataFile.close();
    srcFile.close();
  }

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
     // BehaviourDSLCommon::declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution

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
  }  // end of BehaviourDSLCommon::standardModifier

  std::string BehaviourDSLCommon::tangentOperatorVariableModifier(
      const Hypothesis h, const std::string& var, const bool addThisPtr) {
    return this->standardModifier(h, var, addThisPtr);
  }  // end of BehaviourDSLCommon::tangentOperatorVariableModifier

  void BehaviourDSLCommon::treatPrivate() {
    auto hs = std::set<Hypothesis>{};
    this->readHypothesesList(hs);
    const auto beg = this->current;
    for (const auto& h : hs) {
      const auto& d = this->mb.getBehaviourData(h);
      this->current = beg;
      CodeBlockParserOptions o;
      o.mn = d.getRegistredMembersNames();
      o.smn = d.getRegistredStaticMembersNames();
      o.qualifyStaticVariables = true;
      o.qualifyMemberVariables = true;
      o.modifier =
          makeVariableModifier(*this, h, &BehaviourDSLCommon::standardModifier);
      this->mb.appendToPrivateCode(h, this->readNextBlock(o).code, true);
    }
  }  // end of void BehaviourDSLCommon::treatPrivate(void)

  void BehaviourDSLCommon::treatMembers() {
    auto hs = std::set<Hypothesis>{};
    this->readHypothesesList(hs);
    const auto beg = this->current;
    for (const auto& h : hs) {
      const auto& d = this->mb.getBehaviourData(h);
      this->current = beg;
      CodeBlockParserOptions o;
      o.mn = d.getRegistredMembersNames();
      o.smn = d.getRegistredStaticMembersNames();
      o.qualifyStaticVariables = true;
      o.qualifyMemberVariables = true;
      o.modifier =
          makeVariableModifier(*this, h, &BehaviourDSLCommon::standardModifier);
      this->mb.appendToMembers(h, this->readNextBlock(o).code, true);
    }
  }  // end of BehaviourDSLCommon::treatMembers

  void BehaviourDSLCommon::treatBrick() {
    auto& f = AbstractBehaviourBrickFactory::getFactory();
    AbstractBehaviourBrick::Parameters parameters;
    AbstractBehaviourBrick::DataMap data;
    if (this->current->value == "<") {
      auto options = std::vector<tfel::utilities::Token>{};
      this->readList(options, "BehaviourDSLCommon::treatBehaviourBrick", "<",
                     ">", true);
      for (const auto& o : options) {
        const auto pos = o.value.find('=');
        if (pos != std::string::npos) {
          if (pos == 0) {
            this->throwRuntimeError("BehaviourDSLCommon::treatBehaviourBrick",
                                    "no parameter name given");
          }
          // extracting the name
          const auto& n = o.value.substr(0, pos);
          if (pos == o.value.size()) {
            this->throwRuntimeError("BehaviourDSLCommon::treatBehaviourBrick",
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
    this->checkNotEndOfFile(
        "BehaviourDSLCommon::treatIsTangentOperatorSymmetric : ",
        "Expected 'true' or 'false'.");
    const auto b = [this]() -> std::string {
      if (this->current->flag == tfel::utilities::Token::String) {
        return this->readString("BehaviourDSLCommon::treatBehaviourBrick");
      }
      const auto r = this->current->value;
      ++(this->current);
      return r;
    }();
    if (this->current->value == "{") {
      data =
          AbstractBehaviourBrick::Data::read(this->current, this->tokens.end())
              .get<AbstractBehaviourBrick::DataMap>();
    }
    this->readSpecifiedToken("BehaviourDSLCommon::treatBehaviourBrick", ";");
    this->bricks.emplace_back(f.get(b, *this, this->mb, parameters, data));
  }  // end of BehaviourDSLCommon::treatBrick

  void BehaviourDSLCommon::treatTangentOperator() {
    using namespace std;
    using namespace tfel::material;
    using namespace tfel::utilities;
    CodeBlockOptions o;
    this->readCodeBlockOptions(o, true);
    if (this->mb.getBehaviourType() ==
        BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR) {
      bool found = false;
      if (o.untreated.size() != 1u) {
        ostringstream msg;
        msg << "BehaviourDSLCommon::treatTangentOperator : "
            << "tangent operator type is undefined. Valid tanget operator type "
               "are :\n";
        for (const auto& to : getFiniteStrainBehaviourTangentOperatorFlags()) {
          msg << "- "
              << convertFiniteStrainBehaviourTangentOperatorFlagToString(to)
              << " : " << getFiniteStrainBehaviourTangentOperatorDescription(to)
              << '\n';
        }
        throw(runtime_error(msg.str()));
      }
      if (o.untreated[0].flag != Token::Standard) {
        throw(
            runtime_error("BehaviourDSLCommon::treatTangentOperator : "
                          "invalid option '" +
                          o.untreated[0].value + "'"));
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
        ostringstream msg;
        msg << "BehaviourDSLCommon::treatTangentOperator : "
            << "invalid tangent operator type '" + ktype +
                   "'. Valid tanget operator type are :\n";
        for (const auto& to : getFiniteStrainBehaviourTangentOperatorFlags()) {
          msg << "- "
              << convertFiniteStrainBehaviourTangentOperatorFlagToString(to)
              << " : " << getFiniteStrainBehaviourTangentOperatorDescription(to)
              << '\n';
        }
        throw(runtime_error(msg.str()));
      }
      this->readCodeBlock(
          *this, o, BehaviourData::ComputeTangentOperator + "-" + ktype,
          &BehaviourDSLCommon::tangentOperatorVariableModifier, true);
      for (const auto& h : o.hypotheses) {
        if (!this->mb.hasAttribute(
                h, BehaviourData::hasConsistentTangentOperator)) {
          this->mb.setAttribute(h, BehaviourData::hasConsistentTangentOperator,
                                true);
        }
      }
    } else {
      this->treatUnsupportedCodeBlockOptions(o);
      this->readCodeBlock(*this, o, BehaviourData::ComputeTangentOperator,
                          &BehaviourDSLCommon::tangentOperatorVariableModifier,
                          true);
      for (const auto& h : o.hypotheses) {
        this->mb.setAttribute(h, BehaviourData::hasConsistentTangentOperator,
                              true);
      }
    }
  }  // end of BehaviourDSLCommon::treatTangentOperator

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
  }  // end of BehaviourDSLCommon::treatTangentOperator

  void BehaviourDSLCommon::treatMaterial() {
    const auto& m = this->readOnlyOneToken();
    if (!CxxTokenizer::isValidIdentifier(m, true)) {
      this->throwRuntimeError("BehaviourDSLCommon::treatMaterial",
                              "invalid material name '" + m + "'");
    }
    this->mb.setMaterialName(m);
    if (!isValidIdentifier(this->mb.getClassName())) {
      this->throwRuntimeError("BehaviourDSLCommon::treatMaterial",
                              "resulting class name is not valid (read '" +
                                  this->mb.getClassName() + "')");
    }
  }  // end of BehaviourDSLCommon::treatMaterial

  void BehaviourDSLCommon::treatLibrary() {
    const auto& m = this->readOnlyOneToken();
    if (!CxxTokenizer::isValidIdentifier(m, true)) {
      this->throwRuntimeError("BehaviourDSLCommon::treatLibrary",
                              "invalid library name '" + m + "'");
    }
    this->mb.setLibrary(m);
  }  // end of BehaviourDSLCommon::treatLibrary

  void BehaviourDSLCommon::treatComputeThermalExpansion() {
    using ComputedMaterialProperty =
        BehaviourDescription::ComputedMaterialProperty;
    const std::string m("BehaviourDSLCommon::treatComputeThermalExpansion");
    if (this->mb.getAttribute<bool>(
            BehaviourDescription::requiresThermalExpansionCoefficientTensor,
            false)) {
      this->throwRuntimeError(m,
                              "@ComputeThermalExpansion can be used along with "
                              "@RequireThermalExpansionCoefficientTensor");
    }
    const auto& acs = this->readMaterialPropertyOrArrayOfMaterialProperties(m);
    this->readSpecifiedToken(m, ";");
    if ((acs.size() != 1u) && (acs.size() != 3u)) {
      this->throwRuntimeError(m, "invalid number of file names given");
    }
    if (acs.size() == 3u) {
      // the material shall have been declared orthotropic
      if (this->mb.getSymmetryType() != mfront::ORTHOTROPIC) {
        this->throwRuntimeError(
            m,
            "the mechanical behaviour must be orthotropic "
            "to give more than one thermal expansion coefficient.");
      }
    }
    for (const auto& a : acs) {
      if (a.is<ComputedMaterialProperty>()) {
        const auto& mpd = *(a.get<ComputedMaterialProperty>().mpd);
        if (!mpd.staticVars.contains("ReferenceTemperature")) {
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
  }  // end of BehaviourDSLCommon::treatComputeThermalExpansion

  void BehaviourDSLCommon::treatElasticMaterialProperties() {
    if (this->mb.getAttribute<bool>(
            BehaviourDescription::requiresStiffnessTensor, false)) {
      this->throwRuntimeError(
          "BehaviourDSLCommon::treatComputeStiffnessTensor",
          "@ElasticMaterialProperties can not be used along with "
          "@RequireStiffnessTensor");
    }
    this->readElasticMaterialProperties();
  }

  BehaviourDescription::MaterialProperty
  BehaviourDSLCommon::extractMaterialProperty(const std::string& m,
                                              const tfel::utilities::Token& t) {
    if (t.flag == tfel::utilities::Token::String) {
      BehaviourDescription::ComputedMaterialProperty mp;
      const auto f = t.value.substr(1, t.value.size() - 2);
      mp.mpd = this->handleMaterialPropertyDescription(f);
      return mp;
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

  void BehaviourDSLCommon::readElasticMaterialProperties() {
    const auto& emps = this->readMaterialPropertyOrArrayOfMaterialProperties(
        "BehaviourDSLCommon::readElasticMaterialProperties");
    this->readSpecifiedToken(
        "BehaviourDSLCommon::readElasticMaterialProperties", ";");
    if ((emps.size() != 2u) && (emps.size() != 9u)) {
      this->throwRuntimeError(
          "BehaviourDSLCommon::readElasticMaterialProperties",
          "invalid number of file names given");
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
                          true);
  }  // end of BehaviourDSLCommon::treatComputeStiffnessTensor

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
    auto v = VariableDescription{"tfel::math::st2tost2<N,stress>",
                                 this->current->value, 1u, this->current->line};
    v.description = "Hill tensor";
    ++(this->current);
    // Hill coefficients
    const auto& hcs = this->readMaterialPropertyOrArrayOfMaterialProperties(
        "BehaviourDSLCommon::treatHillTensor");
    this->readSpecifiedToken("BehaviourDSLCommon::treatHillTensor", ";");
    if (hcs.size() != 6u) {
      this->throwRuntimeError("BehaviourDSLCommon::treatHillTensor",
                              "invalid number of hill coefficients");
    }
    this->mb.addHillTensor(v, hcs);
  }  // end of BehaviourDSLCommon::treatHillTensor

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
  }  // end of BehaviourDSLCommon::treatModellingHypothesis

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
  }  // end of BehaviourDSLCommon::treatModellingHypotheses

  void BehaviourDSLCommon::treatUsableInPurelyImplicitResolution() {
    const Hypothesis h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    this->readSpecifiedToken(
        "BehaviourDSLCommon::treatUsableInPurelyImplicitResolution", ";");
    if (this->explicitlyDeclaredUsableInPurelyImplicitResolution) {
      this->throwRuntimeError(
          "BehaviourDSLCommon::treatUsableInPurelyImplicitResolution",
          "keyword '@UsableInPurelyImplicitResolution' already called");
    }
    this->explicitlyDeclaredUsableInPurelyImplicitResolution = true;
    this->mb.setUsableInPurelyImplicitResolution(h, true);
  }  // end of BehaviourDSLCommon::treatUsableInPurelyImplicitResolution

  void BehaviourDSLCommon::treatParameterMethod(const Hypothesis h) {
    using namespace std;
    using namespace tfel::utilities;
    const auto& n = this->current->value;
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
  }  // end of BehaviourDSLCommon::treatParameterMethod

  bool BehaviourDSLCommon::isCallableVariable(const Hypothesis h,
                                              const std::string& n) const {
    return ((this->mb.isMaterialPropertyName(h, n)) ||
            (this->mb.isStateVariableName(h, n)) ||
            (this->mb.isAuxiliaryStateVariableName(h, n)) ||
            (this->mb.isExternalStateVariableName(h, n)) ||
            (this->mb.isLocalVariableName(h, n)) ||
            (this->mb.isStaticVariableName(h, n)) ||
            (this->mb.isParameterName(h, n)) ||
            (this->mb.isIntegrationVariableName(h, n)));
  }  // end of BehaviourDSLCommon::isCallableVariable

  void BehaviourDSLCommon::treatVariableMethod(const Hypothesis h) {
    using namespace std;
    using namespace tfel::utilities;
    using namespace tfel::glossary;
    const auto& glossary = Glossary::getGlossary();
    const auto& n = this->current->value;
    ++(this->current);
    this->checkNotEndOfFile("BehaviourDSLCommon::treatVariableMethod");
    this->readSpecifiedToken("BehaviourDSLCommon::treatVariableMethod", ".");
    this->checkNotEndOfFile("BehaviourDSLCommon::treatVariableMethod");
    if (this->current->value == "setGlossaryName") {
      ++(this->current);
      this->checkNotEndOfFile("BehaviourDSLCommon::treatVariableMethod");
      this->readSpecifiedToken("BehaviourDSLCommon::treatVariableMethod", "(");
      this->checkNotEndOfFile("BehaviourDSLCommon::treatVariableMethod");
      if (this->current->flag != Token::String) {
        this->throwRuntimeError("BehaviourDSLCommon::treatVariableMethod: ",
                                "expected to read a string");
      }
      const auto& g =
          this->current->value.substr(1, this->current->value.size() - 2);
      if (!glossary.contains(g)) {
        this->throwRuntimeError("BehaviourDSLCommon::treatVariableMethod: ",
                                "'" + g + "' is not a glossary name");
      }
      this->mb.setGlossaryName(h, n, g);
      ++(this->current);
    } else if (this->current->value == "setEntryName") {
      ++(this->current);
      this->checkNotEndOfFile("BehaviourDSLCommon::treatVariableMethod");
      this->readSpecifiedToken("BehaviourDSLCommon::treatVariableMethod", "(");
      this->checkNotEndOfFile("BehaviourDSLCommon::treatVariableMethod");
      if (this->current->flag != Token::String) {
        this->throwRuntimeError("BehaviourDSLCommon::treatVariableMethod: ",
                                "expected to read a string");
      }
      const auto& e =
          this->current->value.substr(1, this->current->value.size() - 2);
      if (!this->isValidIdentifier(e)) {
        this->throwRuntimeError("BehaviourDSLCommon::treatVariableMethod: ",
                                "invalid entry name '" + e + "'");
      }
      ++(this->current);
      this->mb.setEntryName(h, n, e);
    } else {
      this->treatUnknownVariableMethod(h, n);
    }
    this->checkNotEndOfFile("BehaviourDSLCommon::treatVariableMethod");
    this->readSpecifiedToken("BehaviourDSLCommon::treatVariableMethod", ")");
    this->checkNotEndOfFile("BehaviourDSLCommon::treatVariableMethod");
    this->readSpecifiedToken("BehaviourDSLCommon::treatVariableMethod", ";");
  }  // end of BehaviourDSLCommon::treatVariableMethod

  void BehaviourDSLCommon::treatUnknownVariableMethod(const Hypothesis,
                                                      const std::string& n) {
    this->throwRuntimeError(
        "BehaviourDSLCommon::treatUnknownVariableMethod : ",
        "unknown method '" + this->current->value + "' for variable '" + n +
            "', "
            "valid methods are 'setGlossaryName' or 'setEntryName'");
  }  // end of BehaviourDSLCommon::treatUnknownVariableMethod

  void BehaviourDSLCommon::treatUnknownKeyword() {
    TokensContainer::const_iterator p2;
    auto treated = false;
    --(this->current);
    const auto key = this->current->value;
    ++(this->current);
    this->checkNotEndOfFile("BehaviourDSLCommon::treatUnknownKeyword");
    if (this->current->value == "[") {
      ++(this->current);
      this->checkNotEndOfFile("BehaviourDSLCommon::treatUnknownKeyword");
      auto s = std::vector<std::string>{};
      while (this->current->value != "]") {
        this->checkNotEndOfFile("BehaviourDSLCommon::treatUnknownKeyword");
        const auto t = [this]() -> std::string {
          if (this->current->flag == tfel::utilities::Token::String) {
            return this->current->value.substr(1,
                                               this->current->value.size() - 2);
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
        this->ignoreKeyWord(key);
        return;
      }
    } else {
      for (const auto& i : this->interfaces) {
        auto p = i.second->treatKeyword(this->mb, key, {i.first}, this->current,
                                        this->tokens.end());
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
    }
    if (!treated) {
      DSLBase::treatUnknownKeyword();
    }
    this->current = p2;
  }  // end of BehaviourDSLCommon::treatUnknownKeyword

  void BehaviourDSLCommon::treatUseQt() {
    this->checkNotEndOfFile("BehaviourDSLCommon::treatUseQt : ",
                            "Expected 'true' or 'false'.");
    if (this->current->value == "true") {
      this->mb.setUseQt(true);
    } else if (this->current->value == "false") {
      this->mb.setUseQt(false);
    } else {
      this->throwRuntimeError(
          "BehaviourDSLCommon::treatUseQt",
          "Expected to read 'true' or 'false' instead of '" +
              this->current->value + ".");
    }
    ++(this->current);
    this->readSpecifiedToken("BehaviourDSLCommon::treatUseQt", ";");
  }  // end of BehaviourDSLCommon::treatUseQt

  void BehaviourDSLCommon::treatIsotropicBehaviour() {
    if (this->mb.getSymmetryType() != mfront::ISOTROPIC) {
      this->throwRuntimeError("BehaviourDSLCommon::treatIsotropicBehaviour",
                              "this behaviour has been declared orthotropic");
    }
    this->readSpecifiedToken("BehaviourDSLCommon::treatIsotropicBehaviour",
                             ";");
  }  // end of BehaviourDSLCommon::treatIsotropicBehaviour

  void BehaviourDSLCommon::treatOrthotropicBehaviour() {
    using namespace tfel::material;
    OrthotropicAxesConvention c = OrthotropicAxesConvention::DEFAULT;
    this->checkNotEndOfFile("BehaviourDSLCommon::treatOrthotropicBehaviour");
    if (this->current->value == "<") {
      this->readSpecifiedToken("BehaviourDSLCommon::treatOrthotropicBehaviour",
                               "<");
      this->checkNotEndOfFile("BehaviourDSLCommon::treatOrthotropicBehaviour");
      if (this->current->value == "Pipe") {
        ++this->current;
        c = OrthotropicAxesConvention::PIPE;
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
  }  // end of BehaviourDSLCommon::treatOrthotropicBehaviour

  void BehaviourDSLCommon::treatIsotropicElasticBehaviour() {
    this->readSpecifiedToken(
        "BehaviourDSLCommon::treatIsotropicElasticBehaviour", ";");
    if (this->mb.getSymmetryType() != mfront::ORTHOTROPIC) {
      this->throwRuntimeError(
          "BehaviourDSLCommon::treatIsotropicElasticBehaviour",
          "this behaviour has not been declared orthotropic");
    }
    this->mb.setElasticSymmetryType(mfront::ISOTROPIC);
  }  // end of BehaviourDSLCommon::treatIsotropicElasticBehaviour

  void BehaviourDSLCommon::treatRequireStiffnessOperator() {
    if (getVerboseMode() >= VERBOSE_LEVEL2) {
      getLogStream() << "BehaviourDSLCommon::treatRequireStiffnessOperator : "
                     << "@RequireStiffnessOperator is deprecated\n"
                     << "You shall use @RequireStiffnessTensor instead\n";
    }
    this->treatRequireStiffnessTensor();
  }  // end of BehaviourDSLCommon::treatRequireStiffnessOperator

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
  }  // end of BehaviourDSLCommon::treatRequireStiffnessTensor

  void BehaviourDSLCommon::treatRequireThermalExpansionCoefficientTensor() {
    this->readSpecifiedToken(
        "BehaviourDSLCommon::treatRequireThermalExpansionCoefficientTensor",
        ";");
    this->mb.setAttribute(
        BehaviourDescription::requiresThermalExpansionCoefficientTensor, true,
        false);
  }  // end of BehaviourDSLCommon::treatRequireThermalExpansionCoefficientTensor

  void BehaviourDSLCommon::treatBehaviour() {
    const auto& b = this->readOnlyOneToken();
    this->mb.setBehaviourName(b);
    if (!isValidIdentifier(this->mb.getClassName())) {
      this->throwRuntimeError("BehaviourDSLCommon::treatBehaviour",
                              "resulting class name is not valid (read '" +
                                  this->mb.getClassName() + "')");
    }
  }  // end of BehaviourDSLCommon::treatMaterial

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
  }  // end of BehaviourDSLCommon::readHypothesesList

  void BehaviourDSLCommon::readVariableList(
      VariableDescriptionContainer& v,
      std::set<Hypothesis>& h,
      void (BehaviourDescription::*m)(const Hypothesis,
                                      const VariableDescriptionContainer&,
                                      const BehaviourData::RegistrationStatus),
      const bool b1,
      const bool b2) {
    h.clear();
    v.clear();
    this->readHypothesesList(h);
    this->readVarList(v, b1);
    this->addVariableList(h, v, m, b2);
  }  // end of BehaviourDSLCommon::readVariableList

  void BehaviourDSLCommon::addVariableList(
      const std::set<Hypothesis>& hypotheses,
      const VariableDescriptionContainer& v,
      void (BehaviourDescription::*m)(const Hypothesis,
                                      const VariableDescriptionContainer&,
                                      const BehaviourData::RegistrationStatus),
      const bool b) {
    for (const auto& h : hypotheses) {
      if (!b) {
        if (!this->mb.getAttribute<bool>(
                h, BehaviourData::allowsNewUserDefinedVariables, true)) {
          const auto cbn = this->mb.getCodeBlockNames(h);
          if (cbn.empty()) {
            this->throwRuntimeError(
                "BehaviourDSLCommon::readVariableList : ",
                "no more variable can be defined. This may mean that "
                "the parser does not expect you to add variables");
          } else {
            auto cbs = std::string{};
            for (const auto& n : cbn) {
              cbs += "\n- " + n;
            }
            this->throwRuntimeError(
                "BehaviourDSLCommon::readVariableList : ",
                "no more variable can be defined. This may mean that "
                "you already declared a block of code (or that the parser does "
                "not "
                "expect you to add variables for whatever reason). This is the "
                "list of "
                "code blocks defined :" +
                    cbs);
          }
        }
      }
      (this->mb.*m)(h, v, BehaviourData::UNREGISTRED);
    }
  }  // end of BehaviourDSLCommon::addVariableList

  void BehaviourDSLCommon::treatCoef() {
    VarContainer v;
    auto h = std::set<Hypothesis>{};
    this->readVariableList(v, h, &BehaviourDescription::addMaterialProperties,
                           true, false);
  }  // end of BehaviourDSLCommon::treatCoef

  void BehaviourDSLCommon::treatLocalVar() {
    VarContainer v;
    auto h = std::set<Hypothesis>{};
    this->readVariableList(v, h, &BehaviourDescription::addLocalVariables, true,
                           true);
  }  // end of BehaviourDSLCommon::treatLocalVar

  void BehaviourDSLCommon::treatInterface() {
    auto& mbif = BehaviourInterfaceFactory::getBehaviourInterfaceFactory();
    auto inames = std::vector<std::string>{};
    this->readStringList(inames);
    for (const auto& i : inames) {
      if (this->interfaces.count(i) == 0) {
        this->interfaces.insert({i, mbif.getInterface(i)});
      }
    }
  }  // end of BehaviourDSLCommon::treatInterface

  void BehaviourDSLCommon::setInterfaces(const std::set<std::string>& inames) {
    auto& mbif = BehaviourInterfaceFactory::getBehaviourInterfaceFactory();
    for (const auto& i : inames) {
      if (this->interfaces.count(i) == 0) {
        this->interfaces.insert({i, mbif.getInterface(i)});
      }
    }
  }  // end of BehaviourDSLCommon::setInterfaces

  void BehaviourDSLCommon::treatAPrioriTimeStepScalingFactor() {
    this->readCodeBlock(*this, BehaviourData::APrioriTimeStepScalingFactor,
                        &BehaviourDSLCommon::standardModifier, true, true);
  }

  void BehaviourDSLCommon::treatIntegrator() {
    this->readCodeBlock(*this, BehaviourData::Integrator,
                        &BehaviourDSLCommon::standardModifier, true, true);
  }  // end of BehaviourDSLCommon::treatIntegrator

  void BehaviourDSLCommon::treatAPosterioriTimeStepScalingFactor() {
    this->readCodeBlock(*this, BehaviourData::APosterioriTimeStepScalingFactor,
                        &BehaviourDSLCommon::standardModifier, true, true);
  }

  void BehaviourDSLCommon::treatStateVariable() {
    VarContainer v;
    auto h = std::set<Hypothesis>{};
    this->readVariableList(v, h, &BehaviourDescription::addStateVariables, true,
                           false);
  }

  void BehaviourDSLCommon::treatAuxiliaryStateVariable() {
    VarContainer v;
    auto h = std::set<Hypothesis>{};
    this->readVariableList(
        v, h, &BehaviourDescription::addAuxiliaryStateVariables, true, false);
  }

  void BehaviourDSLCommon::treatExternalStateVariable() {
    VarContainer v;
    auto h = std::set<Hypothesis>{};
    this->readVariableList(
        v, h, &BehaviourDescription::addExternalStateVariables, true, false);
  }

  void BehaviourDSLCommon::treatBounds() {
    auto hs = std::set<Hypothesis>{};
    this->readHypothesesList(hs);
    auto b = current;
    for (const auto& h : hs) {
      current = b;
      BoundsDescription d;
      d.category = BoundsDescription::Standard;
      this->treatBounds(d, h);
      this->mb.setBounds(h, d);
    }
  }  // end of BehaviourDSLCommon::treatBounds

  void BehaviourDSLCommon::treatPhysicalBounds() {
    auto hs = std::set<Hypothesis>{};
    this->readHypothesesList(hs);
    auto b = current;
    for (const auto& h : hs) {
      current = b;
      BoundsDescription d;
      d.category = BoundsDescription::Physical;
      this->treatBounds(d, h);
      this->mb.setBounds(h, d);
    }
  }  // end of BehaviourDSLCommon::treatPhysicalBounds

  void BehaviourDSLCommon::treatBounds(BoundsDescription& d,
                                       const Hypothesis h) {
    using namespace std;
    VariableDescriptionContainer::const_iterator p;
    const auto& md = this->mb.getBehaviourData(h);
    bool found;
    this->checkNotEndOfFile("BehaviourDSLCommon::treatBounds");

    d.lineNumber = this->current->line;
    d.name = this->current->value;
    d.varName = this->current->value;
    d.component = -1;

    found = false;
    d.arraySize = 1u;
    if (h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      map<DrivingVariable, ThermodynamicForce>::const_iterator p3;
      for (p3 = this->mb.getMainVariables().begin();
           p3 != this->mb.getMainVariables().end(); ++p3) {
        // check if the driving variable match
        if (d.varName == p3->first.name) {
          found = true;
          if (p3->first.increment_known) {
            d.varCategory = BoundsDescription::ExternalStateVariable;
          } else {
            d.varCategory = BoundsDescription::ExternalStateVariableII;
          }
          d.varType = this->getTypeFlag(p3->first.type);
        }
        // check if the associated thermodynamic force match
        if (d.varName == p3->second.name) {
          found = true;
          d.varCategory = BoundsDescription::StateVariable;
          d.varType = this->getTypeFlag(p3->second.type);
        }
      }
    }
    // temperature
    if (d.varName == "T") {
      found = true;
      d.varCategory = BoundsDescription::StateVariable;
      d.varType = Scalar;
    }
    for (p = md.getMaterialProperties().begin();
         (p != md.getMaterialProperties().end()) && (!found); ++p) {
      if (p->name == d.varName) {
        found = true;
        d.varCategory = BoundsDescription::MaterialProperty;
        d.varType = this->getTypeFlag(p->type);
        d.arraySize = p->arraySize;
      }
    }
    for (p = md.getLocalVariables().begin();
         (p != md.getLocalVariables().end()) && (!found); ++p) {
      if (p->name == d.varName) {
        found = true;
        d.varCategory = BoundsDescription::LocalVariable;
        d.varType = this->getTypeFlag(p->type);
        d.arraySize = p->arraySize;
      }
    }
    for (p = md.getStateVariables().begin();
         (p != md.getStateVariables().end()) && (!found); ++p) {
      if (p->name == d.varName) {
        found = true;
        d.varCategory = BoundsDescription::StateVariable;
        d.varType = this->getTypeFlag(p->type);
        d.arraySize = p->arraySize;
      }
    }
    for (p = md.getAuxiliaryStateVariables().begin();
         (p != md.getAuxiliaryStateVariables().end()) && (!found); ++p) {
      if (p->name == d.varName) {
        found = true;
        d.varCategory = BoundsDescription::StateVariable;
        d.varType = this->getTypeFlag(p->type);
        d.arraySize = p->arraySize;
      }
    }
    for (p = md.getExternalStateVariables().begin();
         (p != md.getExternalStateVariables().end()) && (!found); ++p) {
      if (p->name == d.varName) {
        found = true;
        d.varCategory = BoundsDescription::ExternalStateVariable;
        d.varType = this->getTypeFlag(p->type);
        d.arraySize = p->arraySize;
      }
    }
    if (!found) {
      this->throwRuntimeError(
          "BehaviourDSLCommon::treatBounds",
          this->current->value + " is not a valid identifier.");
    }
    if ((d.varType == SupportedTypes::TVector) ||
        (d.varType == SupportedTypes::Stensor) ||
        (d.varType == SupportedTypes::Tensor)) {
      ++(this->current);
      this->readSpecifiedToken("BehaviourDSLCommon::treatBounds : ", "(");
      this->checkNotEndOfFile("BehaviourDSLCommon::treatBounds : ");
      if (this->current->value != "*") {
        unsigned int component;
        istringstream converter(this->current->value);
        converter >> component;
        if (!converter || (!converter.eof())) {
          this->throwRuntimeError(
              "BehaviourDSLCommon::treatBounds",
              "could not read component number for variable '" + d.varName +
                  "'");
        }
        d.component = component;
        d.varType = Scalar;
        d.varName += "(";
        d.varName += to_string(component);
        d.varName += ")";
      }
      ++(this->current);
      this->checkNotEndOfFile("BehaviourDSLCommon::treatBounds");
      if (this->current->value != ")") {
        this->throwRuntimeError(
            "BehaviourDSLCommon::treatBounds",
            "Expected ')' (read '" + this->current->value + "')");
      }
    }
    ++(this->current);
    this->readSpecifiedToken("BehaviourDSLCommon::treatBounds", "in");
    this->checkNotEndOfFile("BehaviourDSLCommon::treatBounds");
    if (this->current->value == "]") {
      ++(this->current);
      this->checkNotEndOfFile("BehaviourDSLCommon::treatBounds");
      if (this->current->value != "*") {
        this->throwRuntimeError(
            "BehaviourDSLCommon::treatBounds",
            "Expected '*' (read '" + this->current->value + "')");
      }
      d.boundsType = BoundsDescription::Upper;
    } else if (this->current->value == "[") {
      ++(this->current);
      this->checkNotEndOfFile(
          "BehaviourDSLCommon::treatBounds",
          "expected to read lower bound value for variable " + d.varName);
      istringstream converter(this->current->value);
      converter >> d.lowerBound;
      d.boundsType = BoundsDescription::LowerAndUpper;
      if (!converter || (!converter.eof())) {
        this->throwRuntimeError(
            "BehaviourDSLCommon::treatBounds",
            "could not read lower bound value for variable '" + d.varName +
                "'");
      }
    } else {
      this->throwRuntimeError(
          "BehaviourDSLCommon::treatBounds : ",
          "Expected ']' or '[' (read '" + this->current->value + "')");
    }
    ++(this->current);
    this->readSpecifiedToken("BehaviourDSLCommon::treatBounds", ":");
    this->checkNotEndOfFile(
        "BehaviourDSLCommon::treatBounds",
        "expected to read upper bound value for variable " + d.varName);
    if (this->current->value == "*") {
      if (d.boundsType == BoundsDescription::Upper) {
        this->throwRuntimeError(
            "BehaviourDSLCommon::treatBounds",
            "upper and lower values bounds are both infinity. "
            "This is inconsistent.");
      }
      d.boundsType = BoundsDescription::Lower;
      ++(this->current);
      this->checkNotEndOfFile("BehaviourDSLCommon::treatBounds",
                              "Expected '['.");
      if (this->current->value != "[") {
        this->throwRuntimeError(
            "BehaviourDSLCommon::treatBounds",
            "Expected '[' (read '" + this->current->value + "')");
      }
    } else {
      istringstream converter(this->current->value);
      converter >> d.upperBound;
      if (!converter || (!converter.eof())) {
        this->throwRuntimeError("BehaviourDSLCommon::treatBounds",
                                "could not read upper bound value "
                                "for variable '" +
                                    d.varName + "'");
      }
      if (d.boundsType == BoundsDescription::LowerAndUpper) {
        if (d.lowerBound > d.upperBound) {
          this->throwRuntimeError("BehaviourDSLCommon::treatBounds",
                                  "lower bound value is greater than upper "
                                  "bound value for variable '" +
                                      d.varName + "'");
        }
      }
      ++(this->current);
      this->checkNotEndOfFile("BehaviourDSLCommon::treatBounds",
                              "Expected ']'.");
      if (this->current->value != "]") {
        this->throwRuntimeError(
            "BehaviourDSLCommon::treatBounds",
            "Expected ']' (read '" + this->current->value + "')");
      }
    }
    ++(this->current);
    this->readSpecifiedToken("BehaviourDSLCommon::treatBounds", ";");
  }  // end of BehaviourDSLCommon::treatBounds

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
    this->mb.registerMemberName(h, "computeStress");
    this->mb.registerMemberName(h, "computeFinalStress");
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
    this->reserveName("dl01_l0");
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
    this->reserveName("TangentOperator");
    this->reserveName("StressFreeExpansionType");
    this->reserveName("behaviourData");
    this->reserveName("time_scaling_factor");
    this->reserveName("mp_bounds_check_status");
  }  // end of BehaviourDSLCommon::registerDefaultVarNames

  BehaviourDSLCommon::BehaviourDSLCommon()
      : DSLBase(),
        useStateVarTimeDerivative(false),
        explicitlyDeclaredUsableInPurelyImplicitResolution(false) {
    // By default disable use of quantities
    this->mb.setUseQt(false);
    // By default, a behaviour can be used in a purely implicit resolution
    const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    this->mb.setUsableInPurelyImplicitResolution(h, true);
    // reserve names
    for (const auto& v : DSLBase::getDefaultReservedNames()) {
      this->mb.reserveName(h, v);
    }
    // register behaviours specific names
    this->registerDefaultVarNames();
    this->reserveName("minimal_time_step_scaling_factor");
    this->reserveName("maximal_time_step_scaling_factor");
    this->reserveName("current_time_step_scaling_factor");
  }  // end of BehaviourDSLCommon::MFrontParserCommon

  void BehaviourDSLCommon::reserveName(const std::string& n) {
    this->mb.reserveName(ModellingHypothesis::UNDEFINEDHYPOTHESIS, n);
  }

  bool BehaviourDSLCommon::isNameReserved(const std::string& n) const {
    return this->mb.isNameReserved(n);
  }

  void BehaviourDSLCommon::writeIncludes(std::ostream& file) {
    if ((!file) || (!file.good())) {
      this->throwRuntimeError("BehaviourDSLCommon::writeIncludes",
                              "ouput file is not valid");
    }
    const auto& h = this->mb.getIncludes();
    if (!h.empty()) {
      file << h << '\n';
    }
  }

  void BehaviourDSLCommon::writeNamespaceBegin(std::ostream& file) {
    if ((!file) || (!file.good())) {
      this->throwRuntimeError("BehaviourDSLCommon::writeNamespaceBegin",
                              "ouput file is not valid");
    }
    file << "namespace tfel{\n\n"
         << "namespace material{\n\n";
  }

  void BehaviourDSLCommon::writeNamespaceEnd(std::ostream& file) {
    if ((!file) || (!file.good())) {
      this->throwRuntimeError("BehaviourDSLCommon::writeNamespaceEnd",
                              "ouput file is not valid");
    }
    file << "} // end of namespace material\n\n"
         << "} // end of namespace tfel\n\n";
  }

  void BehaviourDSLCommon::writeStandardTFELTypedefs(std::ostream& file) {
    if ((!file) || (!file.good())) {
      this->throwRuntimeError("BehaviourDSLCommon::writeStandardTFELTypedefs",
                              "ouput file is not valid");
    }
    file << "typedef unsigned short ushort;\n";
    if (this->mb.useQt()) {
      file << "typedef tfel::config::Types<N,Type,use_qt> Types;\n";
    } else {
      file << "typedef tfel::config::Types<N,Type,false> Types;\n";
    }
    file
        << "typedef typename Types::real                              real;\n"
        << "typedef typename Types::time                              time;\n"
        << "typedef typename Types::length                            length;\n"
        << "typedef typename Types::frequency                         "
           "frequency;\n"
        << "typedef typename Types::stress                            stress;\n"
        << "typedef typename Types::strain                            strain;\n"
        << "typedef typename Types::strainrate                        "
           "strainrate;\n"
        << "typedef typename Types::stressrate                        "
           "stressrate;\n"
        << "typedef typename Types::temperature                       "
           "temperature;\n"
        << "typedef typename Types::thermalexpansion                  "
           "thermalexpansion;\n"
        << "typedef typename Types::massdensity                       "
           "massdensity;\n"
        << "typedef typename Types::TVector                           "
           "TVector;\n"
        << "typedef typename Types::Stensor                           "
           "Stensor;\n"
        << "typedef typename Types::Stensor4                          "
           "Stensor4;\n"
        << "typedef typename Types::FrequencyStensor                  "
           "FrequencyStensor;\n"
        << "typedef typename Types::ForceTVector                      "
           "ForceTVector;\n"
        << "typedef typename Types::StressStensor                     "
           "StressStensor;\n"
        << "typedef typename Types::StressRateStensor                 "
           "StressRateStensor;\n"
        << "typedef typename Types::DisplacementTVector               "
           "DisplacementTVector;\n"
        << "typedef typename Types::StrainStensor                     "
           "StrainStensor;\n"
        << "typedef typename Types::StrainRateStensor                 "
           "StrainRateStensor;\n"
        << "typedef typename Types::StiffnessTensor                   "
           "StiffnessTensor;\n"
        << "typedef typename Types::Tensor                            Tensor;\n"
        << "typedef typename Types::StressTensor                      "
           "StressTensor;\n"
        << "typedef typename Types::ThermalExpansionCoefficientTensor "
           "ThermalExpansionCoefficientTensor;\n"
        << "typedef typename Types::DeformationGradientTensor         "
           "DeformationGradientTensor;\n"
        << "typedef typename Types::DeformationGradientRateTensor     "
           "DeformationGradientRateTensor;\n"
        // tangent operator
        << "typedef " << this->mb.getTangentOperatorType()
        << " TangentOperator;\n";
  }  // end of BehaviourDSLCommon::writeStandardTFELTypedefs

  BehaviourDSLCommon::~BehaviourDSLCommon() {
    this->behaviourFile.close();
    this->behaviourDataFile.close();
    this->integrationDataFile.close();
    this->srcFile.close();
  }  // end of BehaviourDSLCommon::~MFrontParserCommon

  void BehaviourDSLCommon::checkBehaviourDataFile() const {
    if ((!this->behaviourDataFile) || (!this->behaviourDataFile.good())) {
      this->throwRuntimeError(
          "BehaviourDSLCommon::checkBehaviourDataOutputFile",
          "ouput file is not valid");
    }
  }

  void BehaviourDSLCommon::writeBehaviourDataFileHeader() {
    this->checkBehaviourDataFile();
    this->behaviourDataFile
        << "/*!\n"
        << "* \\file   " << this->behaviourDataFileName << '\n'
        << "* \\brief  "
        << "this file implements the " << this->mb.getClassName()
        << "BehaviourData"
        << " class.\n"
        << "*         File generated by " << MFrontHeader::getVersionName()
        << " "
        << "version " << MFrontHeader::getVersionNumber() << '\n';
    if (!this->authorName.empty()) {
      this->behaviourDataFile << "* \\author " << this->authorName << '\n';
    }
    if (!this->date.empty()) {
      this->behaviourDataFile << "* \\date   " << this->date << '\n';
    }
    this->behaviourDataFile << " */\n\n";
  }

  void BehaviourDSLCommon::writeBehaviourDataFileHeaderBegin() {
    this->checkBehaviourDataFile();
    this->behaviourDataFile << "#ifndef LIB_TFELMATERIAL_"
                            << makeUpperCase(this->mb.getClassName())
                            << "_BEHAVIOUR_DATA_HXX\n";
    this->behaviourDataFile << "#define LIB_TFELMATERIAL_"
                            << makeUpperCase(this->mb.getClassName())
                            << "_BEHAVIOUR_DATA_HXX\n\n";
  }

  void BehaviourDSLCommon::writeBehaviourDataFileHeaderEnd() {
    this->checkBehaviourDataFile();
    this->behaviourDataFile << "#endif /* LIB_TFELMATERIAL_"
                            << makeUpperCase(this->mb.getClassName())
                            << "_BEHAVIOUR_DATA_HXX */\n";
  }

  void BehaviourDSLCommon::writeBehaviourDataStandardTFELIncludes() {
    auto b1 = false;
    auto b2 = false;
    this->checkBehaviourDataFile();
    this->behaviourDataFile << "#include<limits>\n";
    this->behaviourDataFile << "#include<string>\n";
    this->behaviourDataFile << "#include<sstream>\n";
    this->behaviourDataFile << "#include<iostream>\n";
    this->behaviourDataFile << "#include<stdexcept>\n";
    this->behaviourDataFile << "#include<algorithm>\n\n";
    this->behaviourDataFile << "#include\"TFEL/Config/TFELConfig.hxx\"\n";
    this->behaviourDataFile << "#include\"TFEL/Config/TFELTypes.hxx\"\n";
    this->behaviourDataFile
        << "#include\"TFEL/Metaprogramming/StaticAssert.hxx\"\n";
    this->behaviourDataFile
        << "#include\"TFEL/TypeTraits/IsFundamentalNumericType.hxx\"\n";
    this->behaviourDataFile << "#include\"TFEL/TypeTraits/IsReal.hxx\"\n";
    if (this->mb.useQt()) {
      this->behaviourDataFile << "#include\"TFEL/Math/General/BaseCast.hxx\"\n";
    }
    this->mb.requiresTVectorOrVectorIncludes(b1, b2);
    if (b1) {
      this->behaviourDataFile << "#include\"TFEL/Math/tvector.hxx\"\n"
                              << "#include\"TFEL/Math/Vector/tvectorIO.hxx\"\n";
    }
    if (b2) {
      this->behaviourDataFile << "#include\"TFEL/Math/vector.hxx\"\n";
    }
    this->behaviourDataFile
        << "#include\"TFEL/Math/stensor.hxx\"\n"
        << "#include\"TFEL/Math/Stensor/StensorConceptIO.hxx\"\n"
        << "#include\"TFEL/Math/tmatrix.hxx\"\n"
        << "#include\"TFEL/Math/Matrix/tmatrixIO.hxx\"\n"
        << "#include\"TFEL/Math/st2tost2.hxx\"\n"
        << "#include\"TFEL/Math/ST2toST2/ST2toST2ConceptIO.hxx\"\n";
    if (this->mb.getBehaviourType() ==
        BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR) {
      this->behaviourDataFile
          << "#include\"TFEL/Math/tensor.hxx\"\n"
          << "#include\"TFEL/Math/Tensor/TensorConceptIO.hxx\"\n"
          << "#include\"TFEL/Math/t2tot2.hxx\"\n"
          << "#include\"TFEL/Math/T2toT2/T2toT2ConceptIO.hxx\"\n"
          << "#include\"TFEL/Math/t2tost2.hxx\"\n"
          << "#include\"TFEL/Math/T2toST2/T2toST2ConceptIO.hxx\"\n"
          << "#include\"TFEL/Math/st2tot2.hxx\"\n"
          << "#include\"TFEL/Math/ST2toT2/ST2toT2ConceptIO.hxx\"\n"
          << "#include\"TFEL/Math/ST2toST2/ConvertToTangentModuli.hxx\"\n"
          << "#include\"TFEL/Math/ST2toST2/"
             "ConvertSpatialModuliToKirchhoffJaumanRateModuli.hxx\"\n"
          << "#include\"TFEL/Material/"
             "FiniteStrainBehaviourTangentOperator.hxx\"\n";
    }
    this->behaviourDataFile
        << "#include\"TFEL/Material/ModellingHypothesis.hxx\"\n\n";
  }

  void BehaviourDSLCommon::writeBehaviourDataDefaultMembers() {
    this->checkBehaviourDataFile();
    if (this->mb.getAttribute(BehaviourDescription::requiresStiffnessTensor,
                              false)) {
      this->behaviourDataFile
          << "//! stiffness tensor computed by the calling solver\n"
          << "StiffnessTensor D;\n";
    }
    if (this->mb.getAttribute(
            BehaviourDescription::requiresThermalExpansionCoefficientTensor,
            false)) {
      this->behaviourDataFile << "ThermalExpansionCoefficientTensor A;\n";
    }
    for (const auto& mv : this->mb.getMainVariables()) {
      if (mv.first.increment_known) {
        this->behaviourDataFile << mv.first.type << " " << mv.first.name
                                << ";\n\n";
      } else {
        this->behaviourDataFile << mv.first.type << " " << mv.first.name
                                << "0;\n\n";
      }
      this->behaviourDataFile << mv.second.type << " " << mv.second.name
                              << ";\n\n";
    }
    this->behaviourDataFile << "temperature T;\n\n";
  }

  void BehaviourDSLCommon::writeBehaviourDataStandardTFELTypedefs() {
    this->checkBehaviourDataFile();
    this->behaviourDataFile << "static " << constexpr_c
                            << " unsigned short TVectorSize = N;\n";
    this->behaviourDataFile
        << "typedef tfel::math::StensorDimeToSize<N> StensorDimeToSize;\n";
    this->behaviourDataFile << "static " << constexpr_c
                            << " unsigned short StensorSize = ";
    this->behaviourDataFile << "StensorDimeToSize::value;\n";
    this->behaviourDataFile
        << "typedef tfel::math::TensorDimeToSize<N> TensorDimeToSize;\n";
    this->behaviourDataFile << "static " << constexpr_c
                            << " unsigned short TensorSize = ";
    this->behaviourDataFile << "TensorDimeToSize::value;\n\n";
    this->writeStandardTFELTypedefs(this->behaviourDataFile);
    this->behaviourDataFile << '\n';
  }

  void BehaviourDSLCommon::writeBehaviourDataDisabledConstructors() {
    this->checkBehaviourDataFile();
  }

  void BehaviourDSLCommon::writeBehaviourDataConstructors(const Hypothesis h) {
    const auto& md = this->mb.getBehaviourData(h);
    this->checkBehaviourDataFile();
    this->behaviourDataFile << "/*!\n"
                            << "* \\brief Default constructor\n"
                            << "*/\n"
                            << this->mb.getClassName() << "BehaviourData()\n"
                            << "{}\n\n"
                            << "/*!\n"
                            << "* \\brief Copy constructor\n"
                            << "*/\n"
                            << this->mb.getClassName() << "BehaviourData(const "
                            << this->mb.getClassName()
                            << "BehaviourData& src)\n"
                            << ": ";
    if (this->mb.getAttribute(BehaviourDescription::requiresStiffnessTensor,
                              false)) {
      this->behaviourDataFile << "D(src.D),\n";
    }
    if (this->mb.getAttribute(
            BehaviourDescription::requiresThermalExpansionCoefficientTensor,
            false)) {
      this->behaviourDataFile << "A(src.A),\n";
    }
    for (const auto& mv : this->mb.getMainVariables()) {
      if (mv.first.increment_known) {
        this->behaviourDataFile << mv.first.name << "(src." << mv.first.name
                                << "),\n";
      } else {
        this->behaviourDataFile << mv.first.name << "0(src." << mv.first.name
                                << "0),\n";
      }
      this->behaviourDataFile << mv.second.name << "(src." << mv.second.name
                              << "),\n";
    }
    this->behaviourDataFile << "T(src.T)";
    for (const auto& v : md.getMaterialProperties()) {
      this->behaviourDataFile << ",\n";
      this->behaviourDataFile << v.name << "(src." << v.name << ")";
    }
    for (const auto& v : md.getStateVariables()) {
      this->behaviourDataFile << ",\n";
      this->behaviourDataFile << v.name << "(src." << v.name << ")";
    }
    for (const auto& v : md.getAuxiliaryStateVariables()) {
      this->behaviourDataFile << ",\n";
      this->behaviourDataFile << v.name << "(src." << v.name << ")";
    }
    for (const auto& v : md.getExternalStateVariables()) {
      this->behaviourDataFile << ",\n";
      this->behaviourDataFile << v.name << "(src." << v.name << ")";
    }
    this->behaviourDataFile << "\n{}\n\n";
    // Creating constructor for external interfaces
    for (const auto& i : this->interfaces) {
      if (i.second->isModellingHypothesisHandled(h, this->mb)) {
        i.second->writeBehaviourDataConstructor(this->behaviourDataFile, h,
                                                this->mb);
      }
    }
  }  // end of BehaviourDSLCommon::WriteBehaviourDataConstructors

  void BehaviourDSLCommon::writeBehaviourDataAssignementOperator(
      const Hypothesis h) {
    using namespace std;
    const auto& md = this->mb.getBehaviourData(h);
    map<DrivingVariable, ThermodynamicForce>::const_iterator p3;
    VariableDescriptionContainer::const_iterator p;
    this->checkBehaviourDataFile();
    this->behaviourDataFile << "/*\n";
    this->behaviourDataFile << "* \\brief Assignement operator\n";
    this->behaviourDataFile << "*/\n";
    this->behaviourDataFile << this->mb.getClassName() << "BehaviourData&\n";
    this->behaviourDataFile << "operator=(const " << this->mb.getClassName()
                            << "BehaviourData& src){\n";
    for (p3 = this->mb.getMainVariables().begin();
         p3 != this->mb.getMainVariables().end(); ++p3) {
      if (p3->first.increment_known) {
        this->behaviourDataFile << "this->" << p3->first.name << " = src."
                                << p3->first.name << ";\n";
      } else {
        this->behaviourDataFile << "this->" << p3->first.name << "0 = src."
                                << p3->first.name << "0;\n";
      }
      this->behaviourDataFile << "this->" << p3->second.name << " = src."
                              << p3->second.name << ";\n";
    }
    if (!md.getMaterialProperties().empty()) {
      for (p = md.getMaterialProperties().begin();
           p != md.getMaterialProperties().end(); ++p) {
        this->behaviourDataFile << "this->" << p->name << " = src." << p->name
                                << ";\n";
      }
    }
    if (!md.getStateVariables().empty()) {
      for (p = md.getStateVariables().begin();
           p != md.getStateVariables().end(); ++p) {
        this->behaviourDataFile << "this->" << p->name << " = src." << p->name
                                << ";\n";
      }
    }
    if (!md.getAuxiliaryStateVariables().empty()) {
      for (p = md.getAuxiliaryStateVariables().begin();
           p != md.getAuxiliaryStateVariables().end(); ++p) {
        this->behaviourDataFile << "this->" << p->name << " = src." << p->name
                                << ";\n";
      }
    }
    if (!md.getExternalStateVariables().empty()) {
      for (p = md.getExternalStateVariables().begin();
           p != md.getExternalStateVariables().end(); ++p) {
        this->behaviourDataFile << "this->" << p->name << " = src." << p->name
                                << ";\n";
      }
    }
    this->behaviourDataFile << "return *this;\n";
    this->behaviourDataFile << "}\n\n";
  }  // end of BehaviourDSLCommon::writeBehaviourAssignementOperator

  void BehaviourDSLCommon::writeBehaviourDataExport(const Hypothesis h) {
    this->checkBehaviourDataFile();
    for (const auto& i : this->interfaces) {
      i.second->exportMechanicalData(this->behaviourDataFile, h, this->mb);
    }
  }

  void BehaviourDSLCommon::writeBehaviourDataPublicMembers() {
    this->checkBehaviourDataFile();
    if (this->mb.getAttribute(BehaviourDescription::requiresStiffnessTensor,
                              false)) {
      this->behaviourDataFile << "StiffnessTensor&\n"
                              << "getStiffnessTensor(void)\n"
                              << "{\nreturn this->D;\n}\n\n"
                              << "const StiffnessTensor&\n"
                              << "getStiffnessTensor(void) const\n"
                              << "{\nreturn this->D;\n}\n\n";
    }
    if (this->mb.getAttribute(
            BehaviourDescription::requiresThermalExpansionCoefficientTensor,
            false)) {
      this->behaviourDataFile
          << "ThermalExpansionCoefficientTensor&\n"
          << "getThermalExpansionCoefficientTensor(void)\n"
          << "{\nreturn this->A;\n}\n\n"
          << "const ThermalExpansionCoefficientTensor&\n"
          << "getThermalExpansionCoefficientTensor(void) const\n"
          << "{\nreturn this->A;\n}\n\n";
    }
  }  // end of BehaviourDSLCommon::writeBehaviourDataPublicMembers

  void BehaviourDSLCommon::writeBehaviourDataClassHeader() {
    this->checkBehaviourDataFile();
    this->behaviourDataFile << "/*!\n";
    this->behaviourDataFile << "* \\class " << this->mb.getClassName()
                            << "BehaviourData\n";
    this->behaviourDataFile << "* \\brief This class implements the "
                            << this->mb.getClassName() << "BehaviourData"
                            << " .\n";
    this->behaviourDataFile << "* \\param H, modelling hypothesis.\n";
    this->behaviourDataFile << "* \\param typename Type, numerical type.\n";
    this->behaviourDataFile
        << "* \\param bool use_qt, conditional saying if quantities are use.\n";
    if (!this->authorName.empty()) {
      this->behaviourDataFile << "* \\author " << this->authorName << '\n';
    }
    if (!this->date.empty()) {
      this->behaviourDataFile << "* \\date   " << this->date << '\n';
    }
    this->behaviourDataFile << "*/\n";
  }

  void BehaviourDSLCommon::writeBehaviourDataForwardDeclarations() {
    using namespace std;
    this->checkBehaviourDataFile();
    this->behaviourDataFile << "// Forward Declaration\n";
    this->behaviourDataFile << "template<ModellingHypothesis::Hypothesis "
                               "hypothesis,typename Type,bool use_qt>\n";
    this->behaviourDataFile << "class " << this->mb.getClassName()
                            << "BehaviourData;\n\n";
    this->behaviourDataFile << "// Forward Declaration\n";
    this->behaviourDataFile << "template<ModellingHypothesis::Hypothesis "
                               "hypothesis,typename Type,bool use_qt>\n";
    this->behaviourDataFile << "class " << this->mb.getClassName()
                            << "IntegrationData;\n\n";
    if (this->mb.useQt()) {
      this->behaviourDataFile << "// Forward Declaration\n";
      this->behaviourDataFile << "template<ModellingHypothesis::Hypothesis "
                                 "hypothesis,typename Type,bool use_qt>\n";
      this->behaviourDataFile << "std::ostream&\n operator <<(std::ostream&,";
      this->behaviourDataFile << "const " << this->mb.getClassName()
                              << "BehaviourData<hypothesis,Type,use_qt>&);\n\n";
    } else {
      this->behaviourDataFile << "// Forward Declaration\n";
      this->behaviourDataFile << "template<ModellingHypothesis::Hypothesis "
                                 "hypothesis,typename Type>\n";
      this->behaviourDataFile << "std::ostream&\n operator <<(std::ostream&,";
      this->behaviourDataFile << "const " << this->mb.getClassName()
                              << "BehaviourData<hypothesis,Type,false>&);\n\n";
    }
    // maintenant, il faut déclarer toutes les spécialisations partielles...
    for (const auto& h : this->mb.getModellingHypotheses()) {
      if (this->mb.hasSpecialisedMechanicalData(h)) {
        if (this->mb.useQt()) {
          this->behaviourDataFile
              << "// Forward Declaration\n"
              << "template<typename Type,bool use_qt>\n"
              << "std::ostream&\n operator <<(std::ostream&,"
              << "const " << this->mb.getClassName()
              << "BehaviourData<ModellingHypothesis::"
              << ModellingHypothesis::toUpperCaseString(h)
              << ",Type,use_qt>&);\n\n";
        } else {
          this->behaviourDataFile
              << "// Forward Declaration\n"
              << "template<typename Type>\n"
              << "std::ostream&\n operator <<(std::ostream&,"
              << "const " << this->mb.getClassName()
              << "BehaviourData<ModellingHypothesis::"
              << ModellingHypothesis::toUpperCaseString(h)
              << ",Type,false>&);\n\n";
        }
      }
    }
  }

  void BehaviourDSLCommon::writeBehaviourDataClassBegin(const Hypothesis h) {
    using namespace std;
    this->checkBehaviourDataFile();
    if (h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      if (this->mb.useQt()) {
        this->behaviourDataFile << "template<ModellingHypothesis::Hypothesis "
                                   "hypothesis,typename Type,bool use_qt>\n";
        this->behaviourDataFile << "class " << this->mb.getClassName()
                                << "BehaviourData\n";
      } else {
        this->behaviourDataFile << "template<ModellingHypothesis::Hypothesis "
                                   "hypothesis,typename Type>\n";
        this->behaviourDataFile << "class " << this->mb.getClassName()
                                << "BehaviourData<hypothesis,Type,false>\n";
      }
    } else {
      if (this->mb.useQt()) {
        this->behaviourDataFile << "template<typename Type,bool use_qt>\n";
        this->behaviourDataFile << "class " << this->mb.getClassName()
                                << "BehaviourData<ModellingHypothesis::"
                                << ModellingHypothesis::toUpperCaseString(h)
                                << ",Type,use_qt>\n";
      } else {
        this->behaviourDataFile << "template<typename Type>\n";
        this->behaviourDataFile << "class " << this->mb.getClassName()
                                << "BehaviourData<ModellingHypothesis::"
                                << ModellingHypothesis::toUpperCaseString(h)
                                << ",Type,false>\n";
      }
    }
    this->behaviourDataFile << "{\n\n";
    if (h != ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      this->behaviourDataFile
          << "static " << constexpr_c
          << " ModellingHypothesis::Hypothesis hypothesis = "
          << "ModellingHypothesis::"
          << ModellingHypothesis::toUpperCaseString(h) << ";\n";
    }
    this->behaviourDataFile
        << "static " << constexpr_c
        << " unsigned short N = "
           "ModellingHypothesisToSpaceDimension<hypothesis>::value;\n";
    this->behaviourDataFile << "TFEL_STATIC_ASSERT(N==1||N==2||N==3);\n";
    this->behaviourDataFile << "TFEL_STATIC_ASSERT(tfel::typetraits::"
                            << "IsFundamentalNumericType<Type>::cond);\n";
    this->behaviourDataFile
        << "TFEL_STATIC_ASSERT(tfel::typetraits::IsReal<Type>::cond);\n\n";
    this->behaviourDataFile
        << "friend std::ostream& operator<< <>(std::ostream&,const ";
    this->behaviourDataFile << this->mb.getClassName()
                            << "BehaviourData&);\n\n";
    this->behaviourDataFile
        << "/* integration data is declared friend to access"
        << "   driving variables at the beginning of the time step */\n";
    if (this->mb.useQt()) {
      this->behaviourDataFile << "friend class " << this->mb.getClassName()
                              << "IntegrationData<hypothesis,Type,use_qt>;\n\n";
    } else {
      this->behaviourDataFile << "friend class " << this->mb.getClassName()
                              << "IntegrationData<hypothesis,Type,false>;\n\n";
    }
  }

  void BehaviourDSLCommon::writeBehaviourDataClassEnd() {
    this->checkBehaviourDataFile();
    this->behaviourDataFile << "}; // end of " << this->mb.getClassName()
                            << "BehaviourData"
                            << "class\n\n";
  }

  void BehaviourDSLCommon::writeBehaviourDataMaterialProperties(
      const Hypothesis h) {
    this->checkBehaviourDataFile();
    this->writeVariablesDeclarations(
        this->behaviourDataFile,
        this->mb.getBehaviourData(h).getMaterialProperties(), "", "",
        this->fileName, false);
    this->behaviourDataFile << '\n';
  }

  void BehaviourDSLCommon::writeBehaviourDataStateVariables(
      const Hypothesis h) {
    this->checkBehaviourDataFile();
    const auto& d = this->mb.getBehaviourData(h);
    this->writeVariablesDeclarations(this->behaviourDataFile,
                                     d.getStateVariables(), "", "",
                                     this->fileName, false);
    this->writeVariablesDeclarations(this->behaviourDataFile,
                                     d.getAuxiliaryStateVariables(), "", "",
                                     this->fileName, false);
    this->writeVariablesDeclarations(this->behaviourDataFile,
                                     d.getExternalStateVariables(), "", "",
                                     this->fileName, false);
    this->behaviourDataFile << '\n';
  }

  void BehaviourDSLCommon::writeBehaviourDataOutputOperator(
      const Hypothesis h) {
    this->checkBehaviourFile();
    const auto& d = this->mb.getBehaviourData(h);
    if (h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      if (this->mb.useQt()) {
        this->behaviourDataFile << "template<ModellingHypothesis::Hypothesis "
                                   "hypothesis,typename Type,bool use_qt>\n";
        this->behaviourDataFile << "std::ostream&\n";
        this->behaviourDataFile << "operator <<(std::ostream& os,";
        this->behaviourDataFile
            << "const " << this->mb.getClassName()
            << "BehaviourData<hypothesis,Type,use_qt>& b)\n";
      } else {
        this->behaviourDataFile << "template<ModellingHypothesis::Hypothesis "
                                   "hypothesis,typename Type>\n";
        this->behaviourDataFile << "std::ostream&\n";
        this->behaviourDataFile << "operator <<(std::ostream& os,";
        this->behaviourDataFile << "const " << this->mb.getClassName()
                                << "BehaviourData<hypothesis,Type,false>& b)\n";
      }
    } else {
      if (this->mb.useQt()) {
        this->behaviourDataFile << "template<typename Type,bool use_qt>\n";
        this->behaviourDataFile << "std::ostream&\n";
        this->behaviourDataFile << "operator <<(std::ostream& os,";
        this->behaviourDataFile << "const " << this->mb.getClassName()
                                << "BehaviourData<ModellingHypothesis::"
                                << ModellingHypothesis::toUpperCaseString(h)
                                << ",Type,use_qt>& b)\n";
      } else {
        this->behaviourDataFile << "template<typename Type>\n";
        this->behaviourDataFile << "std::ostream&\n";
        this->behaviourDataFile << "operator <<(std::ostream& os,";
        this->behaviourDataFile << "const " << this->mb.getClassName()
                                << "BehaviourData<ModellingHypothesis::"
                                << ModellingHypothesis::toUpperCaseString(h)
                                << ",Type,false>& b)\n";
      }
    }
    this->behaviourDataFile << "{\n";
    this->behaviourDataFile << "using namespace std;\n";
    for (const auto& v : this->mb.getMainVariables()) {
      if (v.first.increment_known) {
        this->behaviourDataFile << "os << \"" << v.first.name << " : \" << b."
                                << v.first.name << " << '\\n';\n";
      } else {
        this->behaviourDataFile << "os << \"" << v.first.name << "0 : \" << b."
                                << v.first.name << "0 << endl;\n";
      }
      this->behaviourDataFile << "os << \"" << v.second.name << " : \" << b."
                              << v.second.name << " << '\\n';\n";
    }
    this->behaviourDataFile << "os << \"T : \" << b.T << endl;\n";
    for (const auto& v : d.getMaterialProperties()) {
      this->behaviourDataFile << "os << \"" << v.name << " : \" << b." << v.name
                              << " << '\\n';\n";
    }
    for (const auto& v : d.getStateVariables()) {
      this->behaviourDataFile << "os << \"" << v.name << " : \" << b." << v.name
                              << " << '\\n';\n";
    }
    for (const auto& v : d.getAuxiliaryStateVariables()) {
      this->behaviourDataFile << "os << \"" << v.name << " : \" << b." << v.name
                              << " << '\\n';\n";
    }
    for (const auto& v : d.getExternalStateVariables()) {
      this->behaviourDataFile << "os << \"" << v.name << " : \" << b." << v.name
                              << " << '\\n';\n";
    }
    this->behaviourDataFile << "return os;\n";
    this->behaviourDataFile << "}\n\n";
  }  //  BehaviourDSLCommon::writeBehaviourDataOutputOperator

  void BehaviourDSLCommon::writeBehaviourDataFileBegin() {
    this->checkBehaviourDataFile();
    this->writeBehaviourDataFileHeader();
    this->writeBehaviourDataFileHeaderBegin();
    this->writeBehaviourDataStandardTFELIncludes();
    this->writeIncludes(this->behaviourDataFile);
    // includes specific to interfaces
    for (const auto& i : this->interfaces) {
      i.second->writeInterfaceSpecificIncludes(this->behaviourDataFile,
                                               this->mb);
    }
    this->writeNamespaceBegin(this->behaviourDataFile);
    this->writeBehaviourDataForwardDeclarations();
  }  // end of BehaviourDSLCommon::writeBehaviourDataFile

  void BehaviourDSLCommon::writeBehaviourDataClass(const Hypothesis h) {
    this->checkBehaviourDataFile();
    this->writeBehaviourDataClassBegin(h);
    this->writeBehaviourDataStandardTFELTypedefs();
    this->behaviourDataFile << "protected:\n\n";
    this->writeBehaviourDataDefaultMembers();
    this->writeBehaviourDataMaterialProperties(h);
    this->writeBehaviourDataStateVariables(h);
    this->behaviourDataFile << "public:\n\n";
    this->writeBehaviourDataDisabledConstructors();
    this->writeBehaviourDataConstructors(h);
    this->writeBehaviourDataMainVariablesSetters();
    this->writeBehaviourDataPublicMembers();
    this->writeBehaviourDataAssignementOperator(h);
    this->writeBehaviourDataExport(h);
    this->writeBehaviourDataClassEnd();
    this->writeBehaviourDataOutputOperator(h);
  }

  void BehaviourDSLCommon::writeBehaviourDataFileEnd() {
    this->writeNamespaceEnd(this->behaviourDataFile);
    this->writeBehaviourDataFileHeaderEnd();
  }  // end of BehaviourDSLCommon::writeBehaviourDataFileEnd

  void BehaviourDSLCommon::checkBehaviourFile() const {
    if ((!this->behaviourFile) || (!this->behaviourFile.good())) {
      this->throwRuntimeError(
          "BehaviourDSLCommon::checkBehaviourDataOutputFile",
          "ouput file is not valid");
    }
  }

  void BehaviourDSLCommon::writeBehaviourForwardDeclarations() {
    this->checkBehaviourFile();
    this->behaviourFile << "// Forward Declaration\n"
                        << "template<ModellingHypothesis::Hypothesis,typename "
                           "Type,bool use_qt>\n"
                        << "class " << this->mb.getClassName() << ";\n\n";
    if (this->mb.useQt()) {
      this->behaviourFile << "// Forward Declaration\n"
                          << "template<ModellingHypothesis::Hypothesis "
                             "hypothesis,typename Type,bool use_qt>\n"
                          << "std::ostream&\n operator <<(std::ostream&,"
                          << "const " << this->mb.getClassName()
                          << "<hypothesis,Type,use_qt>&);\n\n";
    } else {
      this->behaviourFile << "// Forward Declaration\n"
                          << "template<ModellingHypothesis::Hypothesis "
                             "hypothesis,typename Type>\n"
                          << "std::ostream&\n operator <<(std::ostream&,"
                          << "const " << this->mb.getClassName()
                          << "<hypothesis,Type,false>&);\n\n";
    }
    // maintenant, il faut déclarer toutes les spécialisations partielles...
    const auto& mh = this->mb.getModellingHypotheses();
    for (const auto& h : mh) {
      if (this->mb.hasSpecialisedMechanicalData(h)) {
        if (this->mb.useQt()) {
          this->behaviourFile << "// Forward Declaration\n"
                              << "template<typename Type,bool use_qt>\n"
                              << "std::ostream&\n operator <<(std::ostream&,"
                              << "const " << this->mb.getClassName()
                              << "<ModellingHypothesis::"
                              << ModellingHypothesis::toUpperCaseString(h)
                              << ",Type,use_qt>&);\n\n";
        } else {
          this->behaviourFile << "// Forward Declaration\n"
                              << "template<typename Type>\n"
                              << "std::ostream&\n operator <<(std::ostream&,"
                              << "const " << this->mb.getClassName()
                              << "<ModellingHypothesis::"
                              << ModellingHypothesis::toUpperCaseString(h)
                              << ",Type,false>&);\n\n";
        }
      }
    }
  }  // end of BehaviourDSLCommon::writeBehaviourClassForwardDeclarations

  void BehaviourDSLCommon::writeBehaviourClassBegin(const Hypothesis h) {
    using namespace std;
    this->checkBehaviourFile();
    this->behaviourFile << "/*!\n";
    this->behaviourFile << "* \\class " << this->mb.getClassName() << '\n';
    this->behaviourFile << "* \\brief This class implements the "
                        << this->mb.getClassName() << " behaviour.\n";
    this->behaviourFile << "* \\param hypothesis, modelling hypothesis.\n";
    this->behaviourFile << "* \\param Type, numerical type.\n";
    if (this->mb.useQt()) {
      this->behaviourFile << "* \\param use_qt, conditional "
                          << "saying if quantities are use.\n";
    }
    if (!this->authorName.empty()) {
      this->behaviourFile << "* \\author " << this->authorName << '\n';
    }
    if (!this->date.empty()) {
      this->behaviourFile << "* \\date   " << this->date << '\n';
    }
    if (!this->description.empty()) {
      this->behaviourFile << this->description << '\n';
    }
    this->behaviourFile << "*/\n";
    const string btype = this->mb.getBehaviourTypeFlag();
    if (h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      if (this->mb.useQt()) {
        this->behaviourFile << "template<ModellingHypothesis::Hypothesis "
                               "hypothesis,typename Type,bool use_qt>\n";
        this->behaviourFile << "class " << this->mb.getClassName() << '\n';
        this->behaviourFile << ": public MechanicalBehaviour<" << btype
                            << ",hypothesis,Type,use_qt>,\n";
        if (this->mb.getAttribute(BehaviourData::profiling, false)) {
          this->behaviourFile << "public " << this->mb.getClassName()
                              << "Profiler,\n";
        }
        this->behaviourFile << "public " << this->mb.getClassName()
                            << "BehaviourData<hypothesis,Type,use_qt>,\n";
        this->behaviourFile << "public " << this->mb.getClassName()
                            << "IntegrationData<hypothesis,Type,use_qt>";
        this->writeBehaviourParserSpecificInheritanceRelationship();
      } else {
        this->behaviourFile << "template<ModellingHypothesis::Hypothesis "
                               "hypothesis,typename Type>\n";
        this->behaviourFile << "class " << this->mb.getClassName()
                            << "<hypothesis,Type,false>\n";
        this->behaviourFile << ": public MechanicalBehaviour<" << btype
                            << ",hypothesis,Type,false>,\n";
        if (this->mb.getAttribute(BehaviourData::profiling, false)) {
          this->behaviourFile << "public " << this->mb.getClassName()
                              << "Profiler,\n";
        }
        this->behaviourFile << "public " << this->mb.getClassName()
                            << "BehaviourData<hypothesis,Type,false>,\n";
        this->behaviourFile << "public " << this->mb.getClassName()
                            << "IntegrationData<hypothesis,Type,false>";
        this->writeBehaviourParserSpecificInheritanceRelationship();
      }
    } else {
      if (this->mb.useQt()) {
        this->behaviourFile << "template<typename Type,bool use_qt>\n";
        this->behaviourFile
            << "class " << this->mb.getClassName() << "<ModellingHypothesis::"
            << ModellingHypothesis::toUpperCaseString(h) << ",Type,use_qt>\n";
        this->behaviourFile << ": public MechanicalBehaviour<" << btype
                            << ",ModellingHypothesis::"
                            << ModellingHypothesis::toUpperCaseString(h)
                            << ",Type,use_qt>,\n";
        if (this->mb.getAttribute(BehaviourData::profiling, false)) {
          this->behaviourFile << "public " << this->mb.getClassName()
                              << "Profiler,\n";
        }
        this->behaviourFile << "public " << this->mb.getClassName()
                            << "BehaviourData<ModellingHypothesis::"
                            << ModellingHypothesis::toUpperCaseString(h)
                            << ",Type,use_qt>,\n";
        this->behaviourFile << "public " << this->mb.getClassName()
                            << "IntegrationData<ModellingHypothesis::"
                            << ModellingHypothesis::toUpperCaseString(h)
                            << ",Type,use_qt>";
        this->writeBehaviourParserSpecificInheritanceRelationship();
      } else {
        this->behaviourFile << "template<typename Type>\n";
        this->behaviourFile
            << "class " << this->mb.getClassName() << "<ModellingHypothesis::"
            << ModellingHypothesis::toUpperCaseString(h) << ",Type,false>\n";
        this->behaviourFile << ": public MechanicalBehaviour<" << btype
                            << ",ModellingHypothesis::"
                            << ModellingHypothesis::toUpperCaseString(h)
                            << ",Type,false>,\n";
        if (this->mb.getAttribute(BehaviourData::profiling, false)) {
          this->behaviourFile << "public " << this->mb.getClassName()
                              << "Profiler,\n";
        }
        this->behaviourFile << "public " << this->mb.getClassName()
                            << "BehaviourData<ModellingHypothesis::"
                            << ModellingHypothesis::toUpperCaseString(h)
                            << ",Type,false>,\n";
        this->behaviourFile << "public " << this->mb.getClassName()
                            << "IntegrationData<ModellingHypothesis::"
                            << ModellingHypothesis::toUpperCaseString(h)
                            << ",Type,false>";
      }
    }
    this->behaviourFile << "{\n\n";
    if (h != ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      this->behaviourFile << "static " << constexpr_c
                          << " ModellingHypothesis::Hypothesis hypothesis = "
                          << "ModellingHypothesis::"
                          << ModellingHypothesis::toUpperCaseString(h) << ";\n";
    }
    this->behaviourFile
        << "static " << constexpr_c
        << " unsigned short N = "
           "ModellingHypothesisToSpaceDimension<hypothesis>::value;\n\n";
    this->behaviourFile << "TFEL_STATIC_ASSERT(N==1||N==2||N==3);\n";
    this->behaviourFile << "TFEL_STATIC_ASSERT(tfel::typetraits::"
                        << "IsFundamentalNumericType<Type>::cond);\n";
    this->behaviourFile
        << "TFEL_STATIC_ASSERT(tfel::typetraits::IsReal<Type>::cond);\n\n";
    this->behaviourFile
        << "friend std::ostream& operator<< <>(std::ostream&,const ";
    this->behaviourFile << this->mb.getClassName() << "&);\n\n";
  }

  void BehaviourDSLCommon::writeBehaviourFileHeader() {
    this->checkBehaviourFile();
    this->behaviourFile << "/*!\n"
                        << "* \\file   " << this->behaviourFileName << '\n'
                        << "* \\brief  "
                        << "this file implements the "
                        << this->mb.getClassName() << " Behaviour.\n"
                        << "*         File generated by "
                        << MFrontHeader::getVersionName() << " "
                        << "version " << MFrontHeader::getVersionNumber()
                        << '\n';
    if (!this->authorName.empty()) {
      this->behaviourFile << "* \\author " << this->authorName << '\n';
    }
    if (!this->date.empty()) {
      this->behaviourFile << "* \\date   " << this->date << '\n';
    }
    this->behaviourFile << " */\n\n";
  }

  void BehaviourDSLCommon::writeBehaviourFileHeaderBegin() {
    this->checkBehaviourFile();
    this->behaviourFile << "#ifndef LIB_TFELMATERIAL_"
                        << makeUpperCase(this->mb.getClassName()) << "_HXX\n"
                        << "#define LIB_TFELMATERIAL_"
                        << makeUpperCase(this->mb.getClassName()) << "_HXX\n\n";
  }

  void BehaviourDSLCommon::writeBehaviourFileHeaderEnd() {
    this->checkBehaviourFile();
    this->behaviourFile << "#endif /* LIB_TFELMATERIAL_"
                        << makeUpperCase(this->mb.getClassName())
                        << "_HXX */\n";
  }

  void BehaviourDSLCommon::writeBehaviourClassEnd() {
    this->checkBehaviourFile();
    this->behaviourFile << "}; // end of " << this->mb.getClassName()
                        << " class\n\n";
  }

  void BehaviourDSLCommon::treatUpdateAuxiliaryStateVariables() {
    this->readCodeBlock(*this, BehaviourData::UpdateAuxiliaryStateVariables,
                        &BehaviourDSLCommon::standardModifier, true, true);
  }  // end of BehaviourDSLCommon::treatUpdateAuxiliaryStateVarBase

  void BehaviourDSLCommon::treatComputeStressFreeExpansion() {
    this->readCodeBlock(*this, BehaviourData::ComputeStressFreeExpansion,
                        &BehaviourDSLCommon::standardModifier, true, true);
  }  // end of BehaviourDSLCommon::treatComputeStressFreeExpansion

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
              BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) &&
                 (this->mb.getBehaviourType() !=
                  BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR),
             "the @Swelling keyword is only valid for small or "
             "finite strain behaviours");
    this->checkNotEndOfFile("DSLBase::treatSwelling");
    if (this->current->value == "<") {
      auto options = std::vector<tfel::utilities::Token>{};
      this->readList(options, "BehaviourDSLCommon::readCodeBlockOptions", "<",
                     ">", true);
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
  }  // end of BehaviourDSLCommon::treatSwelling

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
  }  // end of BehaviourDSLCommon::readStressFreeExpansionHandler

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
  }  // end of BehaviourDSLCommon::readStressFreeExpansionHandler

  void BehaviourDSLCommon::treatAxialGrowth() {
    using AxialGrowth = BehaviourData::AxialGrowth;
    const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    auto throw_if = [this](const bool b, const std::string& m) {
      if (b) {
        this->throwRuntimeError("BehaviourDSLCommon::treatAxialGrowth", m);
      }
    };
    throw_if((this->mb.getBehaviourType() !=
              BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) &&
                 (this->mb.getBehaviourType() !=
                  BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR),
             "the @AxialGrowth keyword is only valid for small or "
             "finite strain behaviours");
    throw_if(this->mb.getSymmetryType() != mfront::ORTHOTROPIC,
             "@AxialGrowth is only valid for orthotropic behaviour");
    this->checkNotEndOfFile("BehaviourDSLCommon::treatAxialGrowth");
    auto s = this->readStressFreeExpansionHandler(*(this->current));
    ++(this->current);
    this->readSpecifiedToken("BehaviourDSLCommon::treatAxialGrowth", ";");
    this->mb.addStressFreeExpansion(uh, AxialGrowth{s});
  }  // end of BehaviourDSLCommon::treatAxialGrowth

  void BehaviourDSLCommon::treatRelocation() {
    using Relocation = BehaviourData::Relocation;
    auto throw_if = [this](const bool b, const std::string& m) {
      if (b) {
        this->throwRuntimeError("BehaviourDSLCommon::treatRelocation", m);
      }
    };
    throw_if((this->mb.getBehaviourType() !=
              BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) &&
                 (this->mb.getBehaviourType() !=
                  BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR),
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
  }  // end of BehaviourDSLCommon::treatRelocation

  void BehaviourDSLCommon::writeBehaviourUpdateIntegrationVariables(
      const Hypothesis h) {
    const auto& d = this->mb.getBehaviourData(h);
    this->checkBehaviourFile();
    this->behaviourFile
        << "/*!\n"
        << "* \\brief Update internal variables at end of integration\n"
        << "*/\n"
        << "void\n"
        << "updateIntegrationVariables(void)";
    if (!d.getIntegrationVariables().empty()) {
      this->behaviourFile << "{\n";
      for (const auto& v : d.getIntegrationVariables()) {
        if (!d.isStateVariableName(v.name)) {
          if (d.isMemberUsedInCodeBlocks(v.name)) {
            this->behaviourFile << "this->" << v.name << " += "
                                << "this->d" << v.name << ";\n";
          }
        }
      }
      this->behaviourFile << "}\n\n";
    } else {
      this->behaviourFile << "\n{}\n\n";
    }
  }  // end of BehaviourDSLCommon::writeBehaviourUpdateIntegrationVariables

  void BehaviourDSLCommon::writeBehaviourUpdateStateVariables(
      const Hypothesis h) {
    const auto& d = this->mb.getBehaviourData(h);
    this->checkBehaviourFile();
    this->behaviourFile
        << "/*!\n"
        << "* \\brief Update internal variables at end of integration\n"
        << "*/\n"
        << "void\n"
        << "updateStateVariables(void)";
    if (!d.getStateVariables().empty()) {
      this->behaviourFile << "{\n";
      for (const auto& v : d.getStateVariables()) {
        this->behaviourFile << "this->" << v.name << " += "
                            << "this->d" << v.name << ";\n";
      }
      this->behaviourFile << "}\n\n";
    } else {
      this->behaviourFile << "\n{}\n\n";
    }
  }  // end of BehaviourDSLCommon::writeBehaviourUpdateStateVariables

  void BehaviourDSLCommon::writeBehaviourUpdateAuxiliaryStateVariables(
      const Hypothesis h) {
    this->behaviourFile
        << "/*!\n"
        << "* \\brief Update auxiliary state variables at end of integration\n"
        << "*/\n"
        << "void\n"
        << "updateAuxiliaryStateVariables(void)";
    const auto& em = this->mb.getModelsDescriptions();
    if ((this->mb.hasCode(h, BehaviourData::UpdateAuxiliaryStateVariables)) ||
        (!em.empty())) {
      this->behaviourFile << "{\n"
                          << "using namespace std;\n"
                          << "using namespace tfel::math;\n";
      for (const auto& m : em) {
        if (m.outputs.size() == 1) {
          const auto vn = m.outputs[0].name;
          this->behaviourFile << "this->" << vn << " += this->d" << vn << ";\n";
        } else {
          this->throwRuntimeError(
              "BehaviourDSLCommon::writeBehaviourUpdateAuxiliaryStateVariables",
              "only models with one output are supported");
        }
      }
      if (this->mb.hasCode(h, BehaviourData::UpdateAuxiliaryStateVariables)) {
        writeMaterialLaws(
            "BehaviourDSLCommon::writeBehaviourUpdateAuxiliaryStateVariables",
            this->behaviourFile, this->mb.getMaterialLaws());
        this->behaviourFile
            << this->mb.getCode(h, BehaviourData::UpdateAuxiliaryStateVariables)
            << "\n";
      }
      this->behaviourFile << "}\n\n";
    } else {
      this->behaviourFile << "\n{}\n\n";
    }
  }  // end of  BehaviourDSLCommon::writeBehaviourUpdateAuxiliaryStateVariables

  void BehaviourDSLCommon::writeBehaviourComputeInternalEnergy(
      const Hypothesis h) {
    this->behaviourFile
        << "/*!\n"
        << "* \\brief Update the internal energy at end of the time step\n"
        << "* \\param[in] Psi_s: internal energy at end of the time step\n"
        << "*/\n"
        << "void computeInternalEnergy(real& Psi_s) const";
    if (this->mb.hasCode(h, BehaviourData::ComputeInternalEnergy)) {
      this->behaviourFile << "{\n"
                          << "using namespace std;\n"
                          << "using namespace tfel::math;\n";
      writeMaterialLaws(
          "BehaviourDSLCommon::writeBehaviourComputeInternalEnergy",
          this->behaviourFile, this->mb.getMaterialLaws());
      this->behaviourFile << this->mb.getCode(
                                 h, BehaviourData::ComputeInternalEnergy)
                          << "\n}\n\n";
    } else {
      this->behaviourFile << "\n{\nPsi_s=0;\n}\n\n";
    }
  }  // end of BehaviourDSLCommon::writeBehaviourComputeInternalEnergy

  void BehaviourDSLCommon::writeBehaviourComputeDissipatedEnergy(
      const Hypothesis h) {
    this->behaviourFile
        << "/*!\n"
        << "* \\brief Update the dissipated energy at end of the time step\n"
        << "* \\param[in] Psi_d: dissipated energy at end of the time step\n"
        << "*/\n"
        << "void computeDissipatedEnergy(real& Psi_d) const";
    if (this->mb.hasCode(h, BehaviourData::ComputeDissipatedEnergy)) {
      this->behaviourFile << "{\n"
                          << "using namespace std;\n"
                          << "using namespace tfel::math;\n";
      writeMaterialLaws(
          "BehaviourDSLCommon::writeBehaviourComputeDissipatedEnergy",
          this->behaviourFile, this->mb.getMaterialLaws());
      this->behaviourFile << this->mb.getCode(
                                 h, BehaviourData::ComputeDissipatedEnergy)
                          << "\n}\n\n";
    } else {
      this->behaviourFile << "\n{\nPsi_d=0;\n}\n\n";
    }
  }  // end of BehaviourDSLCommon::writeBehaviourComputeDissipatedEnergy

  bool BehaviourDSLCommon::hasUserDefinedTangentOperatorCode(
      const Hypothesis h) const {
    using tfel::material::getFiniteStrainBehaviourTangentOperatorFlags;
    if (this->mb.getBehaviourType() ==
        BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR) {
      // all available tangent operators for finite strain behaviours
      const auto tos = getFiniteStrainBehaviourTangentOperatorFlags();
      // search tangent operators defined by the user
      for (const auto& t : tos) {
        const auto ktype =
            convertFiniteStrainBehaviourTangentOperatorFlagToString(t);
        if (this->mb.hasCode(
                h, BehaviourData::ComputeTangentOperator + '-' + ktype)) {
          return true;
        }
      }
    } else {
      if (this->mb.hasCode(h, BehaviourData::ComputeTangentOperator)) {
        return true;
      }
    }
    return false;
  }  // end of BehaviourDSLCommon::hasUserDefinedTangentOperatorCode

  void BehaviourDSLCommon::writeBehaviourIntegrator(const Hypothesis h) {
    const auto btype = this->mb.getBehaviourTypeFlag();
    const auto& md = this->mb.getBehaviourData(h);
    this->checkBehaviourFile();
    this->behaviourFile
        << "/*!\n"
        << "* \\brief Integrate behaviour  over the time step\n"
        << "*/\n"
        << "IntegrationResult\n"
        << "integrate(const SMFlag smflag, const SMType smt) override{\n"
        << "using namespace std;\n"
        << "using namespace tfel::math;\n";
    writeMaterialLaws("BehaviourDSLCommon::writeBehaviourIntegrator",
                      this->behaviourFile, this->mb.getMaterialLaws());
    if ((this->mb.getBehaviourType() ==
         BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) ||
        (this->mb.getBehaviourType() ==
         BehaviourDescription::COHESIVEZONEMODEL)) {
      if (this->mb.useQt()) {
        this->behaviourFile
            << "if(smflag!=MechanicalBehaviour<" << btype
            << ",hypothesis,Type,use_qt>::STANDARDTANGENTOPERATOR){\n"
            << "throw(runtime_error(\"invalid tangent operator flag\"));\n"
            << "}\n";
      } else {
        this->behaviourFile
            << "if(smflag!=MechanicalBehaviour<" << btype
            << ",hypothesis,Type,false>::STANDARDTANGENTOPERATOR){\n"
            << "throw(runtime_error(\"invalid tangent operator flag\"));\n"
            << "}\n";
      }
    }
    this->behaviourFile
        << "bool computeTangentOperator_ = smt!=NOSTIFFNESSREQUESTED;\n";
    if (this->mb.hasCode(h, BehaviourData::ComputePredictor)) {
      this->behaviourFile << this->mb.getCode(h,
                                              BehaviourData::ComputePredictor)
                          << '\n';
    }
    if (this->mb.hasCode(h, BehaviourData::Integrator)) {
      this->behaviourFile << this->mb.getCode(h, BehaviourData::Integrator)
                          << '\n';
    }
    this->behaviourFile << "this->updateIntegrationVariables();\n"
                        << "this->updateStateVariables();\n"
                        << "this->updateAuxiliaryStateVariables();\n";
    for (const auto& b : md.getBounds()) {
      if (b.varCategory == BoundsDescription::StateVariable) {
        b.writeBoundsChecks(this->behaviourFile);
      }
    }
    if (this->hasUserDefinedTangentOperatorCode(h)) {
      this->behaviourFile << "if(computeTangentOperator_){\n";
      if (this->mb.getBehaviourType() ==
          BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR) {
        this->behaviourFile
            << "if(!this->computeConsistentTangentOperator(smflag,smt)){\n";
      } else {
        this->behaviourFile
            << "if(!this->computeConsistentTangentOperator(smt)){\n";
      }
      if (this->mb.useQt()) {
        this->behaviourFile << "return MechanicalBehaviour<" << btype
                            << ",hypothesis,Type,use_qt>::FAILURE;\n";
      } else {
        this->behaviourFile << "return MechanicalBehaviour<" << btype
                            << ",hypothesis,Type,false>::FAILURE;\n";
      }
      this->behaviourFile << "}\n";
      this->behaviourFile << "}\n";
    }
    if (this->mb.useQt()) {
      this->behaviourFile << "return MechanicalBehaviour<" << btype
                          << ",hypothesis,Type,use_qt>::SUCCESS;\n";
    } else {
      this->behaviourFile << "return MechanicalBehaviour<" << btype
                          << ",hypothesis,Type,false>::SUCCESS;\n";
    }
    this->behaviourFile << "}\n\n";
  }

  void BehaviourDSLCommon::writeBehaviourDisabledConstructors() {
    this->checkBehaviourFile();
    this->behaviourFile << "//! \\brief Default constructor (disabled)\n"
                        << this->mb.getClassName() << "() =delete ;\n"
                        << "//! \\brief Copy constructor (disabled)\n"
                        << this->mb.getClassName() << "(const "
                        << this->mb.getClassName() << "&) = delete;\n"
                        << "//! \\brief Assignement operator (disabled)\n"
                        << this->mb.getClassName() << "& operator = (const "
                        << this->mb.getClassName() << "&) = delete;\n\n";
  }

  void BehaviourDSLCommon::writeBehaviourSetOutOfBoundsPolicy() {
    this->checkBehaviourFile();
    this->behaviourFile
        << "/*!\n"
        << "* \\brief set the policy for \"out of bounds\" conditions\n"
        << "*/\n"
        << "void\nsetOutOfBoundsPolicy(const OutOfBoundsPolicy policy_value){\n"
        << "this->policy = policy_value;\n"
        << "} // end of setOutOfBoundsPolicy\n\n";
  }  // end of BehaviourDSLCommon::writeBehaviourOutOfBoundsEnumeration(void)

  void BehaviourDSLCommon::writeBehaviourCheckBounds(const Hypothesis h) {
    const auto& md = this->mb.getBehaviourData(h);
    this->checkBehaviourFile();
    this->behaviourFile << "/*!\n"
                        << "* \\brief check bounds\n"
                        << "*/\n"
                        << "void\ncheckBounds(void) const{\n";
    for (const auto& b : md.getBounds()) {
      b.writeBoundsChecks(this->behaviourFile);
    }
    this->behaviourFile << "} // end of checkBounds\n\n";
  }  // end of BehaviourDSLCommon::writeBehaviourCheckBounds(void)

  std::string BehaviourDSLCommon::getBehaviourConstructorsInitializers(
      const Hypothesis h) {
    const auto& md = this->mb.getBehaviourData(h);
    // variable initialisation
    auto init = this->getIntegrationVariablesIncrementsInitializers(
        md.getIntegrationVariables(), this->useStateVarTimeDerivative);
    if (!this->localVariablesInitializers.empty()) {
      if (!init.empty()) {
        init += ",\n";
      }
      init += this->localVariablesInitializers;
    }
    return init;
  }  // end of BehaviourDSLCommon::getBehaviourConstructorsInitializers

  void BehaviourDSLCommon::writeBehaviourConstructors(const Hypothesis h) {
    auto write_body = [this, h]() {
      this->behaviourFile << "\n{\n"
                          << "using namespace std;\n"
                          << "using namespace tfel::math;\n"
                          << "using std::vector;\n";
      writeMaterialLaws("BehaviourDSLCommon::writeBehaviourConstructors",
                        this->behaviourFile, this->mb.getMaterialLaws());
      this->writeBehaviourParameterInitialisation(h);
      // calling models
      for (const auto& m : this->mb.getModelsDescriptions()) {
        if (m.outputs.size() == 1) {
          const auto vn = m.outputs[0].name;
          this->writeModelCall(this->behaviourFile, h, m, "d" + vn, vn, "em");
          this->behaviourFile << "this->d" << vn << " -= this->" << vn << ";\n";
        } else {
          this->throwRuntimeError(
              "BehaviourDSLCommon::writeBehaviourInitializeMethod",
              "only models with one output are supported yet");
        }
      }
      this->writeBehaviourLocalVariablesInitialisation(h);
      this->behaviourFile << "}\n\n";
    };
    this->checkBehaviourFile();
    // initializers
    const auto& init = this->getBehaviourConstructorsInitializers(h);
    ;
    // writing constructors
    this->behaviourFile << "/*!\n"
                        << "* \\brief Constructor\n"
                        << "*/\n";
    if (this->mb.useQt()) {
      this->behaviourFile << this->mb.getClassName() << "("
                          << "const " << this->mb.getClassName()
                          << "BehaviourData<hypothesis,Type,use_qt>& src1,\n"
                          << "const " << this->mb.getClassName()
                          << "IntegrationData<hypothesis,Type,use_qt>& src2)\n"
                          << ": " << this->mb.getClassName()
                          << "BehaviourData<hypothesis,Type,use_qt>(src1),\n"
                          << this->mb.getClassName()
                          << "IntegrationData<hypothesis,Type,use_qt>(src2)";
    } else {
      this->behaviourFile << this->mb.getClassName() << "("
                          << "const " << this->mb.getClassName()
                          << "BehaviourData<hypothesis,Type,false>& src1,\n"
                          << "const " << this->mb.getClassName()
                          << "IntegrationData<hypothesis,Type,false>& src2)\n"
                          << ": " << this->mb.getClassName()
                          << "BehaviourData<hypothesis,Type,false>(src1),\n"
                          << this->mb.getClassName()
                          << "IntegrationData<hypothesis,Type,false>(src2)";
    }
    if (!init.empty()) {
      this->behaviourFile << ",\n" << init;
    }
    write_body();
    // constructor specific to interfaces
    for (const auto& i : this->interfaces) {
      if (i.second->isModellingHypothesisHandled(h, this->mb)) {
        i.second->writeBehaviourConstructor(this->behaviourFile, this->mb,
                                            init);
        write_body();
      }
    }
  }

  void BehaviourDSLCommon::writeHillTensorComputation(
      std::ostream& out,
      const std::string& H,
      const BehaviourDescription::HillTensor& h,
      std::function<std::string(const MaterialPropertyInput&)>& f) {
    auto throw_if = [this](const bool b, const std::string& m) {
      if (b) {
        this->throwRuntimeError(
            "BehaviourDSLCommon::writeHillTensorComputation", m);
      }
    };
    throw_if(this->mb.getSymmetryType() == mfront::ISOTROPIC,
             "material is not orthotropic");
    for (decltype(h.c.size()) i = 0; i != h.c.size(); ++i) {
      this->writeMaterialPropertyCheckBoundsEvaluation(out, h.c[i], f);
    }
    if (this->mb.getOrthotropicAxesConvention() ==
        OrthotropicAxesConvention::PIPE) {
      out << H << " = tfel::material::computeHillTensor<hypothesis,"
          << "OrthotropicAxesConvention::PIPE,stress>(";
    } else {
      out << H << " = tfel::material::computeHillTensor<hypothesis,"
          << "OrthotropicAxesConvention::DEFAULT,stress>(";
    }
    for (decltype(h.c.size()) i = 0; i != h.c.size();) {
      this->writeMaterialPropertyEvaluation(out, h.c[i], f);
      if (++i != h.c.size()) {
        out << ",\n";
      }
    }
    out << ");\n";
  }  // end of BehaviourDSLCommon::writeHillTensorComputation

  void BehaviourDSLCommon::writeStiffnessTensorComputation(
      std::ostream& out,
      const std::string& D,
      std::function<std::string(const MaterialPropertyInput&)>& f) {
    const auto& emps = this->mb.getElasticMaterialProperties();
    if ((this->mb.getSymmetryType() == mfront::ISOTROPIC) &&
        (this->mb.getElasticSymmetryType() != mfront::ISOTROPIC)) {
      this->throwRuntimeError(
          "BehaviourDSLCommon::writeStiffnessTensorComputation",
          "inconsistent symmetry type for the material and "
          "the elastic behaviour.");
    }
    bool ua = true;
    if (!this->mb.hasAttribute(
            BehaviourDescription::requiresUnAlteredStiffnessTensor)) {
      const auto& h = this->mb.getModellingHypotheses();
      if ((h.find(ModellingHypothesis::PLANESTRESS) != h.end()) ||
          (h.find(ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS) !=
           h.end())) {
        this->throwRuntimeError(
            "BehaviourDSLCommon::writeStiffnessTensorComputation",
            "For plane stress hypotheses, it is required to precise whether "
            "the expected stiffness tensor is 'Altered' (the plane stress "
            "hypothesis is taken into account) or 'UnAltered' (the stiffness "
            "tensor is the same as in plane strain). "
            "See the '@ComputeStiffnessTensor' documentation");
      }
    } else {
      ua = this->mb.getAttribute<bool>(
          BehaviourDescription::requiresUnAlteredStiffnessTensor);
    }
    if (this->mb.getElasticSymmetryType() == mfront::ISOTROPIC) {
      if (emps.size() != 2u) {
        this->throwRuntimeError(
            "BehaviourDSLCommon::writeStiffnessTensorComputation",
            "invalid number of material properties");
      }
      this->writeMaterialPropertyCheckBoundsEvaluation(out, emps[0], f);
      this->writeMaterialPropertyCheckBoundsEvaluation(out, emps[1], f);
      if (ua) {
        out << "tfel::material::computeIsotropicStiffnessTensor<hypothesis,"
               "StiffnessTensorAlterationCharacteristic::UNALTERED"
            << ">(" << D << ",";
      } else {
        out << "tfel::material::computeIsotropicStiffnessTensor<hypothesis,"
               "StiffnessTensorAlterationCharacteristic::ALTERED"
            << ">(" << D << ",";
      }
      this->writeMaterialPropertyEvaluation(out, emps[0], f);
      out << ",\n";
      this->writeMaterialPropertyEvaluation(out, emps[1], f);
      out << ");\n";
    } else if (this->mb.getElasticSymmetryType() == mfront::ORTHOTROPIC) {
      if (emps.size() != 9u) {
        this->throwRuntimeError(
            "BehaviourDSLCommon::writeStiffnessTensorComputation",
            "invalid number of material properties");
      }
      for (decltype(emps.size()) i = 0; i != emps.size(); ++i) {
        this->writeMaterialPropertyCheckBoundsEvaluation(out, emps[i], f);
      }
      if (ua) {
        if (this->mb.getOrthotropicAxesConvention() ==
            OrthotropicAxesConvention::PIPE) {
          out << "tfel::material::computeOrthotropicStiffnessTensor<hypothesis,"
              << "StiffnessTensorAlterationCharacteristic::UNALTERED,"
              << "OrthotropicAxesConvention::PIPE>(" << D << ",";
        } else {
          out << "tfel::material::computeOrthotropicStiffnessTensor<hypothesis,"
              << "StiffnessTensorAlterationCharacteristic::UNALTERED,"
              << "OrthotropicAxesConvention::DEFAULT>(" << D << ",";
        }
      } else {
        if (this->mb.getOrthotropicAxesConvention() ==
            OrthotropicAxesConvention::PIPE) {
          out << "tfel::material::computeOrthotropicStiffnessTensor<hypothesis,"
              << "StiffnessTensorAlterationCharacteristic::ALTERED,"
              << "OrthotropicAxesConvention::PIPE>(" << D << ",";
        } else {
          out << "tfel::material::computeOrthotropicStiffnessTensor<hypothesis,"
              << "StiffnessTensorAlterationCharacteristic::ALTERED,"
              << "OrthotropicAxesConvention::DEFAULT>(" << D << ",";
        }
      }
      for (decltype(emps.size()) i = 0; i != emps.size();) {
        this->writeMaterialPropertyEvaluation(out, emps[i], f);
        if (++i != emps.size()) {
          out << ",\n";
        }
      }
      out << ");\n";
    } else {
      this->throwRuntimeError(
          "BehaviourDSLCommon::writeStiffnessTensorComputation",
          "unsupported elastic symmetry type");
    }
  }  // end of BehaviourDSLCommon::writeStiffnessTensorComputation

  void BehaviourDSLCommon::writeMaterialPropertyArguments(
      std::ostream& out,
      const BehaviourDescription::ComputedMaterialProperty& cmp,
      std::function<std::string(const MaterialPropertyInput&)>& f) {
    const auto& mpd = *(cmp.mpd);
    out << '(';
    if (!mpd.inputs.empty()) {
      const auto& inputs = this->mb.getMaterialPropertyInputs(mpd);
      auto pi = std::begin(inputs);
      const auto pie = std::end(inputs);
      while (pi != pie) {
        out << f(*pi);
        if (++pi != pie) {
          out << ",";
        }
      }
    }
    out << ")";
  }

  void BehaviourDSLCommon::writeMaterialPropertyCheckBoundsEvaluation(
      std::ostream& out,
      const BehaviourDescription::MaterialProperty& m,
      std::function<std::string(const MaterialPropertyInput&)>& f) {
    if (m.is<BehaviourDescription::ComputedMaterialProperty>()) {
      const auto& cmp = m.get<BehaviourDescription::ComputedMaterialProperty>();
      const auto& mpd = *(cmp.mpd);
      if ((mpd.bounds.empty()) && (mpd.physicalBounds.empty())) {
        return;
      }
      const auto& n = MFrontMaterialPropertyInterface().getFunctionName(
          mpd.material, mpd.law);
      out << "{\n // check bounds for material property '" << n << "'\n"
          << "const auto " << n << "_bounds_check_status = " << n
          << "_checkBounds";
      this->writeMaterialPropertyArguments(out, cmp, f);
      out << ";\n"
          << "if(" << n << "_bounds_check_status!=0){\n"
          << "if(" << n << "_bounds_check_status<0){\n"
          << "// physical bounds\n"
          << "throw(OutOfBoundsException(\"" << this->mb.getClassName() << ": "
          << "a variable is out of its physical bounds \"\n"
          << "                           \"when calling the material property '"
          << n << "'\"));\n"
          << "} else {\n"
          << "// standard bounds\n"
          << "if(this->policy==Strict){\n"
          << "throw(OutOfBoundsException(\"" << this->mb.getClassName() << ": "
          << "a variable is out of its bounds \"\n"
          << "                           \"when calling the material property '"
          << n << "'\"));\n"
          << "} else if(this->policy==Warning){\n"
          << "std::cerr << \"" << this->mb.getClassName() << ": "
          << "a variable is out of its bounds \"\n"
          << "             \"when calling the material property '" << n
          << "'\\n\";\n"
          << "}\n"
          << "}\n"
          << "}\n"
          << "}\n";
    } else if (!m.is<BehaviourDescription::ConstantMaterialProperty>()) {
      this->throwRuntimeError(
          "BehaviourDSLCommon::writeMaterialPropertyEvaluation",
          "unsupported material property type");
    }
  }  // end of BehaviourDSLCommon::writeMaterialPropertyEvaluation

  void BehaviourDSLCommon::writeMaterialPropertyEvaluation(
      std::ostream& out,
      const BehaviourDescription::MaterialProperty& m,
      std::function<std::string(const MaterialPropertyInput&)>& f) {
    if (m.is<BehaviourDescription::ConstantMaterialProperty>()) {
      const auto& cmp = m.get<BehaviourDescription::ConstantMaterialProperty>();
      if (!cmp.name.empty()) {
        out << "this->" << cmp.name;
      } else {
        out << cmp.value;
      }
    } else if (m.is<BehaviourDescription::ComputedMaterialProperty>()) {
      const auto& cmp = m.get<BehaviourDescription::ComputedMaterialProperty>();
      const auto& mpd = *(cmp.mpd);
      out << MFrontMaterialPropertyInterface().getFunctionName(mpd.material,
                                                               mpd.law);
      this->writeMaterialPropertyArguments(out, cmp, f);
    } else {
      this->throwRuntimeError(
          "BehaviourDSLCommon::writeMaterialPropertyEvaluation",
          "unsupported material property type");
    }
  }  // end of BehaviourDSLCommon::writeMaterialPropertyEvaluation

  void BehaviourDSLCommon::writeThermalExpansionCoefficientComputation(
      std::ostream& out,
      const BehaviourDescription::MaterialProperty& a,
      const std::string& T,
      const std::string& i,
      const std::string& s) {
    out << "const thermalexpansion alpha" << s;
    if (!i.empty()) {
      out << "_" << i;
    }
    out << " = ";
    if (a.is<BehaviourDescription::ConstantMaterialProperty>()) {
      const auto& cmp = a.get<BehaviourDescription::ConstantMaterialProperty>();
      if (cmp.name.empty()) {
        out << cmp.value << ";\n";
      } else {
        out << "this->" << cmp.name << ";\n";
      }
    } else if (a.is<BehaviourDescription::ComputedMaterialProperty>()) {
      const auto& mpd =
          *(a.get<BehaviourDescription::ComputedMaterialProperty>().mpd);
      const auto inputs = this->mb.getMaterialPropertyInputs(mpd);
      out << MFrontMaterialPropertyInterface().getFunctionName(mpd.material,
                                                               mpd.law)
          << '(';
      if (!inputs.empty()) {
        if (inputs.size() != 1u) {
          this->throwRuntimeError(
              "BehaviourDSLCommon::writeThermalExpansionCoefficientComputation",
              "thermal expansion coefficients must depend on the temperature "
              "only");
        }
        if (inputs.front().ename != tfel::glossary::Glossary::Temperature) {
          this->throwRuntimeError(
              "BehaviourDSLCommon::writeThermalExpansionCoefficientComputation",
              "thermal expansion coefficients must depend on the temperature "
              "only");
        }
        out << T;
      }
      out << ");\n";
    } else {
      this->throwRuntimeError(
          "BehaviourDSLCommon::writeMaterialPropertyEvaluation",
          "unsupported material property type");
    }
  }  // end of BehaviourDSLCommon::writeThermalExpansionCoefficientComputation

  void BehaviourDSLCommon::writeThermalExpansionCoefficientsComputations(
      std::ostream& out,
      const BehaviourDescription::MaterialProperty& a,
      const std::string& suffix) {
    this->writeThermalExpansionCoefficientComputation(
        out, a, "this->referenceTemperatureForThermalExpansion", "",
        suffix + "_Ti");
    this->writeThermalExpansionCoefficientComputation(out, a, "this->T", "t",
                                                      suffix + "_T");
    this->writeThermalExpansionCoefficientComputation(
        out, a, "this->T+this->dT", "t_dt", suffix + "_T");
  }  // end of writeThermalExpansionCoefficientComputation

  void BehaviourDSLCommon::writeThermalExpansionComputation(
      std::ostream& out,
      const BehaviourDescription::MaterialProperty& a,
      const std::string& t,
      const std::string& c,
      const std::string& suffix) {
    const auto Tref = [&a]() -> std::string {
      if (a.is<BehaviourDescription::ConstantMaterialProperty>()) {
        return "293.15";
      } else if (a.is<BehaviourDescription::ComputedMaterialProperty>()) {
        const auto& mpd =
            *(a.get<BehaviourDescription::ComputedMaterialProperty>().mpd);
        return mpd.staticVars.contains("ReferenceTemperature")
                   ? std::to_string(
                         mpd.staticVars.get("ReferenceTemperature").value)
                   : "293.15";
      }
      throw(std::runtime_error(
          "BehaviourDSLCommon::writeThermalExpansionComputation: "
          "unsupported material property type"));
    }();
    const auto T = (t == "t") ? "this->T" : "this->T+this->dT";
    const auto i = (t == "t") ? "0" : "1";
    out << "const auto mfront_thermal_expansion" << c << "_" << i << "  "
        << "= 1/(1+alpha" << suffix
        << "_Ti * (this->referenceTemperatureForThermalExpansion-" << Tref
        << "))*("
        << "alpha" << suffix << "_T_" << t << " * (" << T << "-" << Tref << ")-"
        << "alpha" << suffix
        << "_Ti * (this->referenceTemperatureForThermalExpansion-" << Tref
        << "));\n";
  }  // end of BehaviourDSLCommon::writeThermalExpansionComputation

  void BehaviourDSLCommon::writeBehaviourComputeStressFreeExpansion(
      const Hypothesis h) {
    const auto& bd = this->mb.getBehaviourData(h);
    auto throw_if = [this](const bool b, const std::string& m) {
      if (b) {
        this->throwRuntimeError(
            "BehaviourDSLCommon::writeBehaviourComputeStressFreeExpansion", m);
      }
    };
    auto eval = [](std::ostream& out,
                   const BehaviourDescription::MaterialProperty& mp,
                   const std::string& c, const bool b) {
      const auto& cmp =
          mp.get<BehaviourDescription::ConstantMaterialProperty>();
      const auto Tref = "293.15";
      const auto i = b ? "1" : "0";
      const auto T = b ? "this->T+this->dT" : "this->T";
      if (cmp.name.empty()) {
        out << "const auto mfront_thermal_expansion" << c << "_" << i << " "
            << "= " << cmp.value << "/(1+" << cmp.value
            << "*(this->referenceTemperatureForThermalExpansion-" << Tref
            << "))"
            << "*(" << T
            << "-this->referenceTemperatureForThermalExpansion);\n";
      } else {
        out << "const auto mfront_thermal_expansion" << c << "_" << i << " "
            << "= (this->" << cmp.name << ")/(1+(this->" << cmp.name
            << ")*(this->referenceTemperatureForThermalExpansion-" << Tref
            << "))"
            << "*(" << T
            << "-this->referenceTemperatureForThermalExpansion);\n";
      }
    };
    if (!this->mb.requiresStressFreeExpansionTreatment(h)) {
      return;
    }
    if (this->mb.areThermalExpansionCoefficientsDefined()) {
      throw_if((this->mb.getBehaviourType() !=
                BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) &&
                   (this->mb.getBehaviourType() !=
                    BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR),
               "only finite strain or small strain behaviour are supported");
      if (this->mb.getSymmetryType() == mfront::ORTHOTROPIC) {
        if ((this->mb.getOrthotropicAxesConvention() ==
             OrthotropicAxesConvention::DEFAULT) &&
            (this->mb.getThermalExpansionCoefficients().size() == 3u)) {
          // in this case, only tridimensional case is supported
          for (const auto mh : this->mb.getDistinctModellingHypotheses()) {
            throw_if(mh != ModellingHypothesis::TRIDIMENSIONAL,
                     "an orthotropic axes convention must be choosen when "
                     "using @ComputeThermalExpansion keyword in behaviours "
                     "which shall be valid in other modelling hypothesis "
                     "than 'Tridimensional'.\n"
                     "Either restrict the validity of the behaviour to "
                     "'Tridimensional' (see @ModellingHypothesis) or "
                     "choose and orthotropic axes convention as on option "
                     "to the @OrthotropicBehaviour keyword");
          }
        }
      }
    }
    this->checkBehaviourFile();
    this->behaviourFile
        << "void\n"
        << "computeStressFreeExpansion(std::pair<StressFreeExpansionType,"
           "StressFreeExpansionType>& dl01_l0)\n{\n";
    this->behaviourFile << "using namespace std;\n";
    this->behaviourFile << "using namespace tfel::math;\n";
    this->behaviourFile << "using std::vector;\n";
    writeMaterialLaws(
        "BehaviourDSLCommon::writeBehaviourComputeStressFreeExpansion",
        this->behaviourFile, this->mb.getMaterialLaws());
    this->behaviourFile << "auto& dl0_l0 = dl01_l0.first;\n";
    this->behaviourFile << "auto& dl1_l0 = dl01_l0.second;\n";
    this->behaviourFile << "dl0_l0 = StressFreeExpansionType(typename "
                           "StressFreeExpansionType::value_type(0));\n";
    this->behaviourFile << "dl1_l0 = StressFreeExpansionType(typename "
                           "StressFreeExpansionType::value_type(0));\n";
    if (this->mb.hasCode(h, BehaviourData::ComputeStressFreeExpansion)) {
      this->behaviourFile << this->mb.getCode(
                                 h, BehaviourData::ComputeStressFreeExpansion)
                          << '\n';
    }
    if (this->mb.areThermalExpansionCoefficientsDefined()) {
      const auto& acs = this->mb.getThermalExpansionCoefficients();
      if (acs.size() == 1u) {
        const auto& a = acs.front();
        if (a.is<BehaviourDescription::ConstantMaterialProperty>()) {
          eval(this->behaviourFile, a, "0", false);
        } else {
          this->writeThermalExpansionCoefficientsComputations(
              this->behaviourFile, acs.front());
          this->writeThermalExpansionComputation(this->behaviourFile,
                                                 acs.front(), "t", "0");
        }
        this->behaviourFile << "dl0_l0[0] += mfront_thermal_expansion0_0;\n"
                            << "dl0_l0[1] += mfront_thermal_expansion0_0;\n"
                            << "dl0_l0[2] += mfront_thermal_expansion0_0;\n";
        if (a.is<BehaviourDescription::ConstantMaterialProperty>()) {
          eval(this->behaviourFile, a, "0", true);
        } else {
          this->writeThermalExpansionComputation(this->behaviourFile,
                                                 acs.front(), "t_dt", "0");
        }
        this->behaviourFile << "dl1_l0[0] += mfront_thermal_expansion0_1;\n"
                            << "dl1_l0[1] += mfront_thermal_expansion0_1;\n"
                            << "dl1_l0[2] += mfront_thermal_expansion0_1;\n";
      } else if (acs.size() == 3u) {
        throw_if(this->mb.getSymmetryType() != mfront::ORTHOTROPIC,
                 "invalid number of thermal expansion coefficients");
        for (size_t i = 0; i != 3; ++i) {
          if (!acs[i].is<BehaviourDescription::ConstantMaterialProperty>()) {
            this->writeThermalExpansionCoefficientsComputations(
                this->behaviourFile, acs[i], std::to_string(i));
          }
        }
        for (size_t i = 0; i != 3; ++i) {
          const auto idx = std::to_string(i);
          if (acs[i].is<BehaviourDescription::ConstantMaterialProperty>()) {
            eval(this->behaviourFile, acs[i], idx, false);
            eval(this->behaviourFile, acs[i], idx, true);
          } else {
            this->writeThermalExpansionComputation(this->behaviourFile, acs[i],
                                                   "t", idx, idx);
            this->writeThermalExpansionComputation(this->behaviourFile, acs[i],
                                                   "t_dt", idx, idx);
          }
        }
        this->behaviourFile << "dl0_l0[0] += mfront_thermal_expansion0_0;\n"
                            << "dl0_l0[1] += mfront_thermal_expansion1_0;\n"
                            << "dl0_l0[2] += mfront_thermal_expansion2_0;\n";
        this->behaviourFile << "dl1_l0[0] += mfront_thermal_expansion0_1;\n"
                            << "dl1_l0[1] += mfront_thermal_expansion1_1;\n"
                            << "dl1_l0[2] += mfront_thermal_expansion2_1;\n";
      } else {
        throw_if(true, "unsupported behaviour symmetry");
      }
    }
    for (const auto& d : this->mb.getStressFreeExpansionDescriptions(h)) {
      if (d.is<BehaviourData::AxialGrowth>()) {
        throw_if((this->mb.getBehaviourType() !=
                  BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) &&
                     (this->mb.getBehaviourType() !=
                      BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR),
                 "only finite strain or small strain behaviour are supported");
        throw_if(this->mb.getSymmetryType() != mfront::ORTHOTROPIC,
                 "axial growth is only supported for orthotropic behaviours");
        const auto& s = d.get<BehaviourData::AxialGrowth>();
        throw_if(s.sfe.is<BehaviourData::NullExpansion>(),
                 "null swelling is not supported here");
        // The z-axis is supposed to be aligned with the second
        // direction of orthotropy.
        if (s.sfe.is<BehaviourData::SFED_ESV>()) {
          const auto ev = s.sfe.get<BehaviourData::SFED_ESV>().vname;
          this->behaviourFile
              << "dl0_l0[1]+=this->" << ev << ";\n"
              << "dl0_l0[0]+=real(1)/std::sqrt(1+this->" << ev << ")-real(1);\n"
              << "dl0_l0[2]+=real(1)/std::sqrt(1+this->" << ev << ")-real(1);\n"
              << "dl1_l0[1]+=this->" << ev << "+this->d" << ev << ";\n"
              << "dl1_l0[0]+=real(1)/std::sqrt(1+this->" << ev << "+this->d"
              << ev << ")-real(1);\n"
              << "dl1_l0[2]+=real(1)/std::sqrt(1+this->" << ev << "+this->d"
              << ev << ")-real(1);\n";
        } else if (s.sfe.is<std::shared_ptr<ModelDescription>>()) {
          const auto& md = *(s.sfe.get<std::shared_ptr<ModelDescription>>());
          throw_if(
              md.outputs.size() != 1u,
              "invalid number of outputs for model '" + md.className + "'");
          const auto vs = md.className + "_" + md.outputs[0].name;
          this->behaviourFile << "dl0_l0[1]+=this->" << vs << ";\n"
                              << "dl0_l0[0]+=real(1)/std::sqrt(1+this->" << vs
                              << ")-real(1);\n"
                              << "dl0_l0[2]+=real(1)/std::sqrt(1+this->" << vs
                              << ")-real(1);\n";
          this->writeModelCall(this->behaviourFile, h, md, vs, vs, "sfeh");
          this->behaviourFile << "dl1_l0[1]+=this->" << vs << ";\n"
                              << "dl1_l0[0]+=real(1)/std::sqrt(1+this->" << vs
                              << ")-real(1);\n"
                              << "dl1_l0[2]+=real(1)/std::sqrt(1+this->" << vs
                              << ")-real(1);\n";
        } else {
          throw_if(true, "internal error, unsupported stress free expansion");
        }
      } else if (d.is<BehaviourData::Relocation>()) {
        throw_if((this->mb.getBehaviourType() !=
                  BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) &&
                     (this->mb.getBehaviourType() !=
                      BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR),
                 "only finite strain or small strain behaviour are supported");
        const auto& s = d.get<BehaviourData::Relocation>();
        throw_if(s.sfe.is<BehaviourData::NullExpansion>(),
                 "null swelling is not supported here");
        if (s.sfe.is<BehaviourData::SFED_ESV>()) {
          const auto ev = s.sfe.get<BehaviourData::SFED_ESV>().vname;
          if ((h ==
               ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS) ||
              (h ==
               ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN)) {
            this->behaviourFile
                << "dl0_l0[0]+=this->" << ev << "/2;\n"
                << "dl0_l0[2]+=this->" << ev << "/2;\n"
                << "dl1_l0[0]+=(this->" << ev << "+this->d" << ev << ")/2;\n"
                << "dl1_l0[2]+=(this->" << ev << "+this->d" << ev << ")/2;\n";
          }
          if ((h == ModellingHypothesis::GENERALISEDPLANESTRAIN) ||
              (h == ModellingHypothesis::PLANESTRAIN) ||
              (h == ModellingHypothesis::PLANESTRESS)) {
            this->behaviourFile
                << "dl0_l0[0]+=this->" << ev << "/2;\n"
                << "dl0_l0[1]+=this->" << ev << "/2;\n"
                << "dl1_l0[0]+=(this->" << ev << "+this->d" << ev << ")/2;\n"
                << "dl1_l0[1]+=(this->" << ev << "+this->d" << ev << ")/2;\n";
          }
        } else if (s.sfe.is<std::shared_ptr<ModelDescription>>()) {
          const auto& md = *(s.sfe.get<std::shared_ptr<ModelDescription>>());
          throw_if(
              md.outputs.size() != 1u,
              "invalid number of outputs for model '" + md.className + "'");
          const auto vs = md.className + "_" + md.outputs[0].name;
          if ((h ==
               ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS) ||
              (h ==
               ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN)) {
            this->behaviourFile << "dl0_l0[0]+=(this->" << vs << ")/2;\n"
                                << "dl0_l0[2]+=(this->" << vs << ")/2;\n";
          }
          this->writeModelCall(this->behaviourFile, h, md, vs, vs, "sfeh");
          if ((h == ModellingHypothesis::GENERALISEDPLANESTRAIN) ||
              (h == ModellingHypothesis::PLANESTRAIN) ||
              (h == ModellingHypothesis::PLANESTRESS)) {
            this->behaviourFile << "dl0_l0[0]+=(this->" << vs << ")/2;\n"
                                << "dl0_l0[1]+=(this->" << vs << ")/2;\n";
          }
        } else {
          throw_if(true, "internal error, unsupported stress free expansion");
        }
      } else if (d.is<BehaviourData::OrthotropicStressFreeExpansion>()) {
        const auto& s = d.get<BehaviourData::OrthotropicStressFreeExpansion>();
        auto write = [this, throw_if, h](
                         const BehaviourData::StressFreeExpansionHandler& sfe,
                         const char* const c) {
          if (sfe.is<BehaviourData::SFED_ESV>()) {
            const auto& ev = sfe.get<BehaviourData::SFED_ESV>().vname;
            this->behaviourFile << "dl0_l0[" << c << "]+=this->" << ev << ";\n";
            this->behaviourFile << "dl1_l0[" << c << "]+=this->" << ev
                                << "+this->d" << ev << ";\n";
          } else if (sfe.is<std::shared_ptr<ModelDescription>>()) {
            const auto& md = *(sfe.get<std::shared_ptr<ModelDescription>>());
            throw_if(
                md.outputs.size() != 1u,
                "invalid number of outputs for model '" + md.className + "'");
            const auto vs = md.className + "_" + md.outputs[0].name;
            this->behaviourFile << "dl0_l0[" << c << "]+=this->" << vs << ";\n";
            this->writeModelCall(this->behaviourFile, h, md, vs, vs, "sfeh");
            this->behaviourFile << "dl1_l0[" << c << "]+=this->" << vs << ";\n";
          } else if (!sfe.is<BehaviourData::NullExpansion>()) {
            throw_if(true, "internal error, unsupported stress free expansion");
          }
        };
        throw_if((this->mb.getBehaviourType() !=
                  BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) &&
                     (this->mb.getBehaviourType() !=
                      BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR),
                 "only finite strain or small strain behaviour are supported");
        throw_if(this->mb.getSymmetryType() != mfront::ORTHOTROPIC,
                 "orthotropic stress free expansion is only supported "
                 "for orthotropic behaviours");
        throw_if(s.sfe0.is<BehaviourData::NullExpansion>() &&
                     s.sfe1.is<BehaviourData::NullExpansion>() &&
                     s.sfe2.is<BehaviourData::NullExpansion>(),
                 "null swelling is not supported here");
        write(s.sfe0, "0");
        write(s.sfe1, "1");
        write(s.sfe2, "2");
      } else if (d.is<BehaviourData::OrthotropicStressFreeExpansionII>()) {
        const auto& s =
            d.get<BehaviourData::OrthotropicStressFreeExpansionII>();
        const auto& ev = s.esv.vname;
        throw_if((this->mb.getBehaviourType() !=
                  BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) &&
                     (this->mb.getBehaviourType() !=
                      BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR),
                 "only finite strain or small strain behaviour are supported");
        throw_if(this->mb.getSymmetryType() != mfront::ORTHOTROPIC,
                 "orthotropic stress free expansion is only supported "
                 "for orthotropic behaviours");
        this->behaviourFile
            << "dl0_l0[0]+=this->" << ev << "[0];\n"
            << "dl0_l0[1]+=this->" << ev << "[1];\n"
            << "dl0_l0[2]+=this->" << ev << "[2];\n"
            << "dl1_l0[0]+=this->" << ev << "[0]+this->d" << ev << "[0];\n"
            << "dl1_l0[1]+=this->" << ev << "[1]+this->d" << ev << "[1];\n"
            << "dl1_l0[2]+=this->" << ev << "[2]+this->d" << ev << "[2];\n";
      } else if (d.is<BehaviourData::IsotropicStressFreeExpansion>()) {
        throw_if((this->mb.getBehaviourType() !=
                  BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) &&
                     (this->mb.getBehaviourType() !=
                      BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR),
                 "only finite strain or small strain behaviour are supported");
        const auto& s = d.get<BehaviourData::IsotropicStressFreeExpansion>();
        throw_if(s.sfe.is<BehaviourData::NullExpansion>(),
                 "null swelling is not supported here");
        if (s.sfe.is<BehaviourData::SFED_ESV>()) {
          const auto ev = s.sfe.get<BehaviourData::SFED_ESV>().vname;
          this->behaviourFile
              << "dl0_l0[0]+=this->" << ev << ";\n"
              << "dl0_l0[1]+=this->" << ev << ";\n"
              << "dl0_l0[2]+=this->" << ev << ";\n"
              << "dl1_l0[0]+=this->" << ev << "+this->d" << ev << ";\n"
              << "dl1_l0[1]+=this->" << ev << "+this->d" << ev << ";\n"
              << "dl1_l0[2]+=this->" << ev << "+this->d" << ev << ";\n";
        } else if (s.sfe.is<std::shared_ptr<ModelDescription>>()) {
          const auto& md = *(s.sfe.get<std::shared_ptr<ModelDescription>>());
          throw_if(
              md.outputs.size() != 1u,
              "invalid number of outputs for model '" + md.className + "'");
          const auto vs = md.className + "_" + md.outputs[0].name;
          this->behaviourFile << "dl0_l0[0]+=this->" << vs << ";\n"
                              << "dl0_l0[1]+=this->" << vs << ";\n"
                              << "dl0_l0[2]+=this->" << vs << ";\n";
          this->writeModelCall(this->behaviourFile, h, md, vs, vs, "sfeh");
          this->behaviourFile << "dl1_l0[0]+=this->" << vs << ";\n"
                              << "dl1_l0[1]+=this->" << vs << ";\n"
                              << "dl1_l0[2]+=this->" << vs << ";\n";
        } else {
          throw_if(true, "internal error, unsupported stress free expansion");
        }
      } else if (d.is<BehaviourData::VolumeSwellingStressFreeExpansion>()) {
        throw_if((this->mb.getBehaviourType() !=
                  BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) &&
                     (this->mb.getBehaviourType() !=
                      BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR),
                 "only finite strain or small strain behaviour are supported");
        const auto& s =
            d.get<BehaviourData::VolumeSwellingStressFreeExpansion>();
        throw_if(s.sfe.is<BehaviourData::NullExpansion>(),
                 "null swelling is not supported here");
        if (s.sfe.is<BehaviourData::SFED_ESV>()) {
          const auto ev = s.sfe.get<BehaviourData::SFED_ESV>().vname;
          this->behaviourFile
              << "dl0_l0[0]+=this->" << ev << "/3;\n"
              << "dl0_l0[1]+=this->" << ev << "/3;\n"
              << "dl0_l0[2]+=this->" << ev << "/3;\n"
              << "dl1_l0[0]+=(this->" << ev << "+this->d" << ev << ")/3;\n"
              << "dl1_l0[1]+=(this->" << ev << "+this->d" << ev << ")/3;\n"
              << "dl1_l0[2]+=(this->" << ev << "+this->d" << ev << ")/3;\n";
        } else if (s.sfe.is<std::shared_ptr<ModelDescription>>()) {
          const auto& md = *(s.sfe.get<std::shared_ptr<ModelDescription>>());
          throw_if(
              md.outputs.size() != 1u,
              "invalid number of outputs for model '" + md.className + "'");
          const auto vs = md.className + "_" + md.outputs[0].name;
          this->behaviourFile << "dl0_l0[0]+=this->" << vs << "/3;\n"
                              << "dl0_l0[1]+=this->" << vs << "/3;\n"
                              << "dl0_l0[2]+=this->" << vs << "/3;\n";
          this->writeModelCall(this->behaviourFile, h, md, vs, vs, "sfeh");
          this->behaviourFile << "dl1_l0[0]+=this->" << vs << "/3;\n"
                              << "dl1_l0[1]+=this->" << vs << "/3;\n"
                              << "dl1_l0[2]+=this->" << vs << "/3;\n";
        } else {
          throw_if(true, "internal error, unsupported stress free expansion");
        }
      } else {
        throw_if(true,
                 "internal error, unsupported stress "
                 "free expansion description");
      }
    }
    if (this->mb.getSymmetryType() == mfront::ORTHOTROPIC) {
      if (this->mb.getOrthotropicAxesConvention() ==
          OrthotropicAxesConvention::PIPE) {
        this->behaviourFile
            << "tfel::material::convertStressFreeExpansionStrain<hypothesis,"
               "tfel::material::OrthotropicAxesConvention::PIPE>(dl0_l0);\n"
            << "tfel::material::convertStressFreeExpansionStrain<hypothesis,"
               "tfel::material::OrthotropicAxesConvention::PIPE>(dl1_l0);\n";
      } else {
        throw_if(this->mb.getOrthotropicAxesConvention() !=
                     OrthotropicAxesConvention::DEFAULT,
                 "internal error, unsupported orthotropic axes convention");
        for (const auto mh : this->mb.getDistinctModellingHypotheses()) {
          throw_if(mh != ModellingHypothesis::TRIDIMENSIONAL,
                   "an orthotropic axes convention must be choosen when "
                   "defining a stress free expansion in behaviours "
                   "which shall be valid in other modelling hypothesis "
                   "than 'Tridimensional'.\n"
                   "Either restrict the validity of the behaviour to "
                   "'Tridimensional' (see @ModellingHypothesis) or "
                   "choose and orthotropic axes convention as on option "
                   "to the @OrthotropicBehaviour keyword");
        }
      }
    }
    this->behaviourFile << "}\n\n";
  }  // end of BehaviourDSLCommon::writeBehaviourComputeStressFreeExpansion

  void BehaviourDSLCommon::writeBehaviourInitializeMethod(const Hypothesis h) {
    this->checkBehaviourFile();
    this->behaviourFile
        << "/*!\n"
        << " * \\ brief initialize the behaviour with user code\n"
        << " */\n"
        << "void initialize(void){\n"
        << "using namespace std;\n"
        << "using namespace tfel::math;\n"
        << "using std::vector;\n";
    writeMaterialLaws("BehaviourDSLCommon::writeBehaviourInitializeMethod",
                      this->behaviourFile, this->mb.getMaterialLaws());
    if (this->mb.hasCode(h, BehaviourData::BeforeInitializeLocalVariables)) {
      if (this->mb.getAttribute(BehaviourData::profiling, false)) {
        writeStandardPerformanceProfilingBegin(
            this->behaviourFile, this->mb.getClassName(),
            BehaviourData::BeforeInitializeLocalVariables, "binit");
      }
      this->behaviourFile
          << this->mb
                 .getCodeBlock(h, BehaviourData::BeforeInitializeLocalVariables)
                 .code
          << '\n';
      if (this->mb.getAttribute(BehaviourData::profiling, false)) {
        writeStandardPerformanceProfilingEnd(this->behaviourFile);
      }
    }
    if (this->mb.hasCode(h, BehaviourData::InitializeLocalVariables)) {
      if (this->mb.getAttribute(BehaviourData::profiling, false)) {
        writeStandardPerformanceProfilingBegin(
            this->behaviourFile, this->mb.getClassName(),
            BehaviourData::InitializeLocalVariables, "init");
      }
      this->behaviourFile << this->mb
                                 .getCodeBlock(
                                     h, BehaviourData::InitializeLocalVariables)
                                 .code
                          << '\n';
      if (this->mb.getAttribute(BehaviourData::profiling, false)) {
        writeStandardPerformanceProfilingEnd(this->behaviourFile);
      }
    }
    if (this->mb.hasCode(h, BehaviourData::AfterInitializeLocalVariables)) {
      if (this->mb.getAttribute(BehaviourData::profiling, false)) {
        writeStandardPerformanceProfilingBegin(
            this->behaviourFile, this->mb.getClassName(),
            BehaviourData::AfterInitializeLocalVariables, "ainit");
      }
      this->behaviourFile
          << this->mb
                 .getCodeBlock(h, BehaviourData::AfterInitializeLocalVariables)
                 .code
          << '\n';
      if (this->mb.getAttribute(BehaviourData::profiling, false)) {
        writeStandardPerformanceProfilingEnd(this->behaviourFile);
      }
    }
    this->writeBehaviourParserSpecificInitializeMethodPart(h);
    this->behaviourFile << "}\n\n";
  }  // end of void BehaviourDSLCommon::writeBehaviourInitializeMethod

  void BehaviourDSLCommon::writeBehaviourLocalVariablesInitialisation(
      const Hypothesis h) {
    const auto& md = this->mb.getBehaviourData(h);
    this->checkBehaviourFile();
    for (const auto& v : md.getLocalVariables()) {
      if (this->useDynamicallyAllocatedVector(v.arraySize)) {
        this->behaviourFile << "this->" << v.name << ".resize(" << v.arraySize
                            << ");\n";
      }
    }
  }

  void BehaviourDSLCommon::writeBehaviourParameterInitialisation(
      const Hypothesis h) {
    this->checkBehaviourFile();
    const auto& d = this->mb.getBehaviourData(h);
    for (const auto& p : d.getParameters()) {
      if (h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
        this->behaviourFile
            << "this->" << p.name << " = " << this->mb.getClassName()
            << "ParametersInitializer::get()." << p.name << ";\n";
      } else {
        if (this->mb.hasParameter(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                                  p.name)) {
          this->behaviourFile
              << "this->" << p.name << " = " << this->mb.getClassName()
              << "ParametersInitializer::get()." << p.name << ";\n";
        } else {
          this->behaviourFile
              << "this->" << p.name << " = " << this->mb.getClassName()
              << ModellingHypothesis::toString(h)
              << "ParametersInitializer::get()." << p.name << ";\n";
        }
      }
    }
  }  // end of BehaviourDSLCommon::writeBehaviourParameterInitialisation

  void BehaviourDSLCommon::writeBehaviourDataMainVariablesSetters() {
    this->checkBehaviourDataFile();
    for (const auto& i : this->interfaces) {
      i.second->writeBehaviourDataMainVariablesSetters(this->behaviourDataFile,
                                                       this->mb);
      this->behaviourDataFile << '\n';
    }
  }  // end of BehaviourDSLCommon::writeBehaviourDataMainVariablesSetters

  void BehaviourDSLCommon::writeIntegrationDataMainVariablesSetters() {
    this->checkIntegrationDataFile();
    for (const auto& i : this->interfaces) {
      i.second->writeIntegrationDataMainVariablesSetters(
          this->integrationDataFile, this->mb);
      this->integrationDataFile << '\n';
    }
  }  // end of BehaviourDSLCommon::writeIntegrationDataMainVariablesSetters

  void BehaviourDSLCommon::writeBehaviourGetModellingHypothesis() {
    this->checkBehaviourFile();
    this->behaviourFile << "/*!\n"
                        << "* \\return the modelling hypothesis\n"
                        << "*/\n"
                        << "constexpr "
                           "ModellingHypothesis::"
                           "Hypothesis\ngetModellingHypothesis(void) const{\n"
                        << "return hypothesis;\n"
                        << "} // end of getModellingHypothesis\n\n";
  }  // end of BehaviourDSLCommon::writeBehaviourGetModellingHypothesis();

  void BehaviourDSLCommon::writeBehaviourLocalVariables(const Hypothesis h) {
    this->checkBehaviourFile();
    const auto& md = this->mb.getBehaviourData(h);
    this->writeVariablesDeclarations(this->behaviourFile,
                                     md.getLocalVariables(), "", "",
                                     this->fileName, false);
    this->behaviourFile << '\n';
  }

  void BehaviourDSLCommon::writeBehaviourIntegrationVariables(
      const Hypothesis h) {
    this->checkBehaviourFile();
    const auto& md = this->mb.getBehaviourData(h);
    for (const auto& v : md.getIntegrationVariables()) {
      if (!md.isStateVariableName(v.name)) {
        if (md.isMemberUsedInCodeBlocks(v.name)) {
          this->writeVariableDeclaration(this->behaviourFile, v, "", "",
                                         this->fileName, false);
        }
      }
    }
    this->behaviourFile << '\n';
  }  // end od BehaviourDSLCommon::writeBehaviourIntegrationVariables

  void BehaviourDSLCommon::writeBehaviourParameters(const Hypothesis h) {
    this->checkBehaviourFile();
    const auto& d = this->mb.getBehaviourData(h);
    for (const auto& v : d.getParameters()) {
      if (!getDebugMode()) {
        if (v.lineNumber != 0u) {
          this->behaviourFile << "#line " << v.lineNumber << " \""
                              << this->fileName << "\"\n";
        }
      }
      if (v.arraySize == 1) {
        this->behaviourFile << v.type << " " << v.name << ";\n";
      } else {
        this->behaviourFile << "tfel::math::tvector<" << v.arraySize << ","
                            << v.type << "> " << v.name << ";\n";
      }
    }
    this->behaviourFile << '\n';
  }

  void BehaviourDSLCommon::writeBehaviourPolicyVariable() {
    this->checkBehaviourFile();
    this->behaviourFile << "//! policy for treating out of bounds conditions\n"
                        << "OutOfBoundsPolicy policy = None;\n";
  }  // end of BehaviourDSLCommon::writeBehaviourPolicyVariable

  void BehaviourDSLCommon::writeBehaviourStaticVariables(const Hypothesis h) {
    const auto& md = this->mb.getBehaviourData(h);
    this->checkBehaviourFile();
    for (const auto& v : md.getStaticVariables()) {
      if (!getDebugMode()) {
        if (v.lineNumber != 0u) {
          this->behaviourFile << "#line " << v.lineNumber << " \""
                              << this->fileName << "\"\n";
        }
      }
      if (v.type == "int") {
        this->behaviourFile << "static constexpr " << v.type << " " << v.name
                            << " = " << v.value << ";\n";
      } else {
        this->behaviourFile << "static const " << v.type << " " << v.name
                            << ";\n";
      }
    }
    this->behaviourFile << '\n';
  }

  void BehaviourDSLCommon::writeBehaviourIntegrationVariablesIncrements(
      const Hypothesis h) {
    const auto& md = this->mb.getBehaviourData(h);
    this->checkBehaviourFile();
    this->writeVariablesDeclarations(
        this->behaviourFile, md.getIntegrationVariables(), "d", "",
        this->fileName, this->useStateVarTimeDerivative);
    this->behaviourFile << '\n';
  }

  void BehaviourDSLCommon::writeBehaviourOutputOperator(const Hypothesis h) {
    const auto& md = this->mb.getBehaviourData(h);
    this->checkBehaviourFile();
    if (h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      if (this->mb.useQt()) {
        this->behaviourFile << "template<ModellingHypothesis::Hypothesis "
                               "hypothesis,typename Type,bool use_qt>\n"
                            << "std::ostream&\n"
                            << "operator <<(std::ostream& os,"
                            << "const " << this->mb.getClassName()
                            << "<hypothesis,Type,use_qt>& b)\n";
      } else {
        this->behaviourFile << "template<ModellingHypothesis::Hypothesis "
                               "hypothesis,typename Type>\n"
                            << "std::ostream&\n"
                            << "operator <<(std::ostream& os,"
                            << "const " << this->mb.getClassName()
                            << "<hypothesis,Type,false>& b)\n";
      }
    } else {
      if (this->mb.useQt()) {
        this->behaviourFile << "template<typename Type,bool use_qt>\n"
                            << "std::ostream&\n"
                            << "operator <<(std::ostream& os,"
                            << "const " << this->mb.getClassName()
                            << "<ModellingHypothesis::"
                            << ModellingHypothesis::toUpperCaseString(h)
                            << ",Type,use_qt>& b)\n";
      } else {
        this->behaviourFile << "template<typename Type>\n"
                            << "std::ostream&\n"
                            << "operator <<(std::ostream& os,"
                            << "const " << this->mb.getClassName()
                            << "<ModellingHypothesis::"
                            << ModellingHypothesis::toUpperCaseString(h)
                            << ",Type,false>& b)\n";
      }
    }
    this->behaviourFile << "{\n"
                        << "using namespace std;\n";
    for (const auto& v : this->mb.getMainVariables()) {
      if (v.first.increment_known) {
        this->behaviourFile << "os << \"" << v.first.name << " : \" << b."
                            << v.first.name << " << '\\n';\n"
                            << "os << \"d" << v.first.name << " : \" << b.d"
                            << v.first.name << " << '\\n';\n";
      } else {
        this->behaviourFile << "os << \"" << v.first.name << "0 : \" << b."
                            << v.first.name << "0 << endl;\n"
                            << "os << \"" << v.first.name << "1 : \" << b."
                            << v.first.name << "1 << endl;\n";
      }
      this->behaviourFile << "os << \"" << v.second.name << " : \" << b."
                          << v.second.name << " << '\\n';\n";
    }
    this->behaviourFile << "os << \"dt : \" << b.dt << endl;\n"
                        << "os << \"T : \" << b.T << endl;\n"
                        << "os << \"dT : \" << b.dT << endl;\n";
    for (const auto& v : md.getMaterialProperties()) {
      this->behaviourFile << "os << \"" << v.name << " : \" << b." << v.name
                          << " << '\\n';\n";
    }
    for (const auto& v : md.getStateVariables()) {
      this->behaviourFile << "os << \"" << v.name << " : \" << b." << v.name
                          << " << '\\n';\n";
      this->behaviourFile << "os << \"d" << v.name << " : \" << b.d" << v.name
                          << " << '\\n';\n";
    }
    for (const auto& v : md.getAuxiliaryStateVariables()) {
      this->behaviourFile << "os << \"" << v.name << " : \" << b." << v.name
                          << " << '\\n';\n";
    }
    for (const auto& v : md.getExternalStateVariables()) {
      this->behaviourFile << "os << \"" << v.name << " : \" << b." << v.name
                          << " << '\\n';\n";
      this->behaviourFile << "os << \"d" << v.name << " : \" << b.d" << v.name
                          << " << '\\n';\n";
    }
    for (const auto& v : md.getLocalVariables()) {
#pragma message("BehaviourDSLCommon: handle LocalDataStructure properly")
      if ((v.type.size() >= 7) && (v.type.substr(0, 7) != "struct{")) {
        this->behaviourFile << "os << \"" << v.name << " : \" << b." << v.name
                            << " << '\\n';\n";
      }
    }
    for (const auto& v : md.getParameters()) {
      this->behaviourFile << "os << \"" << v.name << " : \" << b." << v.name
                          << " << '\\n';\n";
    }
    this->behaviourFile << "return os;\n"
                        << "}\n\n";
  }

  void BehaviourDSLCommon::writeBehaviourDestructor() {
    this->checkBehaviourFile();
    this->behaviourFile << "/*!\n"
                        << "* \\brief Destructor\n"
                        << "*/\n"
                        << "~" << this->mb.getClassName() << "()\n"
                        << "{}\n\n";
  }

  void BehaviourDSLCommon::writeBehaviourUpdateExternalStateVariables(
      const Hypothesis h) {
    const auto& md = this->mb.getBehaviourData(h);
    this->checkBehaviourFile();
    this->behaviourFile << "void updateExternalStateVariables(void){\n";
    for (const auto& v : this->mb.getMainVariables()) {
      if (v.first.increment_known) {
        this->behaviourFile << "this->" << v.first.name << "  += this->d"
                            << v.first.name << ";\n";
      } else {
        this->behaviourFile << "this->" << v.first.name << "0  = this->"
                            << v.first.name << "1;\n";
      }
    }
    this->behaviourFile << "this->T   += this->dT;\n";
    for (const auto& v : md.getExternalStateVariables()) {
      this->behaviourFile << "this->" << v.name << " += this->d" << v.name
                          << ";\n";
    }
    this->behaviourFile << "}\n\n";
  }

  void BehaviourDSLCommon::writeBehaviourIncludes() {
    this->checkBehaviourFile();
    this->behaviourFile
        << "#include<string>\n"
        << "#include<iostream>\n"
        << "#include<limits>\n"
        << "#include<stdexcept>\n"
        << "#include<algorithm>\n\n"
        << "#include\"TFEL/Config/TFELConfig.hxx\"\n"
        << "#include\"TFEL/Config/TFELTypes.hxx\"\n"
        << "#include\"TFEL/Metaprogramming/StaticAssert.hxx\"\n"
        << "#include\"TFEL/TypeTraits/IsFundamentalNumericType.hxx\"\n"
        << "#include\"TFEL/TypeTraits/IsReal.hxx\"\n"
        << "#include\"TFEL/Material/MaterialException.hxx\"\n"
        << "#include\"TFEL/Material/MechanicalBehaviour.hxx\"\n"
        << "#include\"TFEL/Material/MechanicalBehaviourTraits.hxx\"\n"
        << "#include\"TFEL/Material/OutOfBoundsPolicy.hxx\"\n"
        << "#include\"TFEL/Material/BoundsCheck.hxx\"\n"
        << "#include\"TFEL/Material/Lame.hxx\"\n";
    if (this->mb.getSymmetryType() == ORTHOTROPIC) {
      this->behaviourFile
          << "#include\"TFEL/Material/Hill.hxx\"\n"
          << "#include\"TFEL/Material/OrthotropicAxesConvention.hxx\"\n";
    }
    if (this->mb.getAttribute(BehaviourDescription::computesStiffnessTensor,
                              false)) {
      this->behaviourFile << "#include\"TFEL/Material/StiffnessTensor.hxx\"\n";
    }
    this->behaviourFile << "#include\"" << this->behaviourDataFileName << "\"\n"
                        << "#include\"" << this->integrationDataFileName
                        << "\"\n";
    if (this->mb.getAttribute<bool>(BehaviourData::profiling, false)) {
      this->behaviourFile << "#include\"MFront/BehaviourProfiler.hxx\"\n";
    }
    this->behaviourFile << '\n';
  }

  void BehaviourDSLCommon::writeBehaviourAdditionalMembers(const Hypothesis h) {
    this->checkBehaviourFile();
    const auto& m = this->mb.getMembers(h);
    if (!m.empty()) {
      this->behaviourFile << m << "\n\n";
    }
  }

  void BehaviourDSLCommon::writeBehaviourPrivate(const Hypothesis h) {
    this->checkBehaviourFile();
    const auto& c = this->mb.getPrivateCode(h);
    if (!c.empty()) {
      this->behaviourFile << c << "\n\n";
    }
  }  // end of void BehaviourDSLCommon::writeBehaviourPrivate

  void BehaviourDSLCommon::writeBehaviourStandardTFELTypedefs() {
    using namespace tfel::material;
    this->checkBehaviourFile();
    const auto btype = this->mb.getBehaviourTypeFlag();
    this->behaviourFile
        << "static " << constexpr_c << " unsigned short TVectorSize = N;\n"
        << "typedef tfel::math::StensorDimeToSize<N> StensorDimeToSize;\n"
        << "static " << constexpr_c << " unsigned short StensorSize = "
        << "StensorDimeToSize::value;\n"
        << "typedef tfel::math::TensorDimeToSize<N> TensorDimeToSize;\n"
        << "static " << constexpr_c << " unsigned short TensorSize = "
        << "TensorDimeToSize::value;\n\n";
    this->writeStandardTFELTypedefs(this->behaviourFile);
    this->behaviourFile << '\n' << "public :\n\n";
    const auto qt = this->mb.useQt() ? "use_qt" : "false";
    this->behaviourFile << "typedef " << this->mb.getClassName()
                        << "BehaviourData<hypothesis,Type," << qt
                        << "> BehaviourData;\n"
                        << "typedef " << this->mb.getClassName()
                        << "IntegrationData<hypothesis,Type," << qt
                        << "> IntegrationData;\n"
                        << "typedef typename MechanicalBehaviour<" << btype
                        << ",hypothesis,Type," << qt << ">::SMFlag SMFlag;\n"
                        << "typedef typename MechanicalBehaviour<" << btype
                        << ",hypothesis,Type," << qt << ">::SMType SMType;\n"
                        << "using MechanicalBehaviour<" << btype
                        << ",hypothesis,Type," << qt << ">::ELASTIC;\n"
                        << "using MechanicalBehaviour<" << btype
                        << ",hypothesis,Type," << qt << ">::SECANTOPERATOR;\n"
                        << "using MechanicalBehaviour<" << btype
                        << ",hypothesis,Type," << qt << ">::TANGENTOPERATOR;\n"
                        << "using MechanicalBehaviour<" << btype
                        << ",hypothesis,Type," << qt
                        << ">::CONSISTENTTANGENTOPERATOR;\n"
                        << "using MechanicalBehaviour<" << btype
                        << ",hypothesis,Type," << qt
                        << ">::NOSTIFFNESSREQUESTED;\n";
    if ((this->mb.getBehaviourType() ==
         BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) ||
        (this->mb.getBehaviourType() ==
         BehaviourDescription::COHESIVEZONEMODEL)) {
      this->behaviourFile << "using MechanicalBehaviour<" << btype
                          << ",hypothesis,Type," << qt
                          << ">::STANDARDTANGENTOPERATOR;\n";
    } else if (this->mb.getBehaviourType() ==
               BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR) {
      for (const auto& toflag :
           getFiniteStrainBehaviourTangentOperatorFlags()) {
        this->behaviourFile
            << "using MechanicalBehaviour<" << btype << ",hypothesis,Type,"
            << qt << ">::"
            << convertFiniteStrainBehaviourTangentOperatorFlagToString(toflag)
            << ";\n";
      }
    }
    this->behaviourFile
        << "using IntegrationResult = typename MechanicalBehaviour<" << btype
        << ",hypothesis,Type," << qt << ">::IntegrationResult;\n\n"
        << "using MechanicalBehaviour<" << btype << ",hypothesis,Type," << qt
        << ">::SUCCESS;\n"
        << "using MechanicalBehaviour<" << btype << ",hypothesis,Type," << qt
        << ">::FAILURE;\n"
        << "using MechanicalBehaviour<" << btype << ",hypothesis,Type," << qt
        << ">::UNRELIABLE_RESULTS;\n\n";
    if ((this->mb.getBehaviourType() ==
         BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) ||
        (this->mb.getBehaviourType() ==
         BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR)) {
      this->behaviourFile << "using StressFreeExpansionType = "
                          << this->mb.getStressFreeExpansionType() << ";\n\n";
    }
  }  // end of BehaviourDSLCommon::writeBehaviourStandardTFELTypedefs

  void BehaviourDSLCommon::writeBehaviourTraits() {
    this->checkBehaviourFile();
    const auto& ah = ModellingHypothesis::getModellingHypotheses();
    // writing partial specialisations
    if (this->mb.getModellingHypotheses().size() >= 4u) {
      // on définit toutes les hypothèses par défaut
      this->writeBehaviourTraitsSpecialisation(
          ModellingHypothesis::UNDEFINEDHYPOTHESIS, true);
      // unsupported hypothesis
      for (const auto h : ah) {
        if (this->mb.isModellingHypothesisSupported(h)) {
          if (this->mb.hasSpecialisedMechanicalData(h)) {
            this->writeBehaviourTraitsSpecialisation(h, true);
          }
        } else {
          this->writeBehaviourTraitsSpecialisation(h, false);
        }
      }
    } else {
      // on exclut toutes les hypothèses par défaut
      this->writeBehaviourTraitsSpecialisation(
          ModellingHypothesis::UNDEFINEDHYPOTHESIS, false);
      // unsupported hypothesis
      for (const auto h : this->mb.getModellingHypotheses()) {
        this->writeBehaviourTraitsSpecialisation(h, true);
      }
    }
  }

  void BehaviourDSLCommon::writeBehaviourTraitsSpecialisation(
      const Hypothesis h, const bool b) {
    SupportedTypes::TypeSize coefSize;
    SupportedTypes::TypeSize stateVarsSize;
    SupportedTypes::TypeSize externalStateVarsSize;
    if (b) {
      const auto& d = this->mb.getBehaviourData(h);
      for (const auto& m : d.getMaterialProperties()) {
        coefSize += this->getTypeSize(m.type, m.arraySize);
      }
      for (const auto& v : d.getPersistentVariables()) {
        stateVarsSize += this->getTypeSize(v.type, v.arraySize);
      }
      for (const auto& v : d.getExternalStateVariables()) {
        externalStateVarsSize += this->getTypeSize(v.type, v.arraySize);
      }
    }
    this->behaviourFile << "/*!\n"
                        << "* Partial specialisation for "
                        << this->mb.getClassName() << ".\n"
                        << "*/\n";
    if (h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      if (this->mb.useQt()) {
        this->behaviourFile << "template<ModellingHypothesis::Hypothesis "
                               "hypothesis,typename Type,bool use_qt>\n"
                            << "class MechanicalBehaviourTraits<"
                            << this->mb.getClassName()
                            << "<hypothesis,Type,use_qt> >\n";
      } else {
        this->behaviourFile << "template<ModellingHypothesis::Hypothesis "
                               "hypothesis,typename Type>\n"
                            << "class MechanicalBehaviourTraits<"
                            << this->mb.getClassName()
                            << "<hypothesis,Type,false> >\n";
      }
    } else {
      if (this->mb.useQt()) {
        this->behaviourFile
            << "template<typename Type,bool use_qt>\n"
            << "class MechanicalBehaviourTraits<" << this->mb.getClassName()
            << "<ModellingHypothesis::"
            << ModellingHypothesis::toUpperCaseString(h) << ",Type,use_qt> >\n";
      } else {
        this->behaviourFile
            << "template<typename Type>\n"
            << "class MechanicalBehaviourTraits<" << this->mb.getClassName()
            << "<ModellingHypothesis::"
            << ModellingHypothesis::toUpperCaseString(h) << ",Type,false> >\n";
      }
    }
    this->behaviourFile << "{\n";
    if (b) {
      if (h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
        this->behaviourFile
            << "static " << constexpr_c
            << " unsigned short N = "
               "ModellingHypothesisToSpaceDimension<hypothesis>::value;\n";
      } else {
        this->behaviourFile
            << "static " << constexpr_c
            << " unsigned short N = ModellingHypothesisToSpaceDimension<"
            << "ModellingHypothesis::"
            << ModellingHypothesis::toUpperCaseString(h) << ">::value;\n";
      }
      this->behaviourFile
          << "static " << constexpr_c << " unsigned short TVectorSize = N;\n"
          << "typedef tfel::math::StensorDimeToSize<N> StensorDimeToSize;\n"
          << "static " << constexpr_c << " unsigned short StensorSize = "
          << "StensorDimeToSize::value;\n"
          << "typedef tfel::math::TensorDimeToSize<N> TensorDimeToSize;\n"
          << "static " << constexpr_c << " unsigned short TensorSize = "
          << "TensorDimeToSize::value;\n";
    }
    this->behaviourFile << "public:\n";
    if (b) {
      this->behaviourFile << "static " << constexpr_c
                          << " bool is_defined = true;\n";
    } else {
      this->behaviourFile << "static " << constexpr_c
                          << " bool is_defined = false;\n";
    }
    if (this->mb.useQt()) {
      this->behaviourFile << "static " << constexpr_c
                          << " bool use_quantities = use_qt;\n";
    } else {
      this->behaviourFile << "static " << constexpr_c
                          << " bool use_quantities = false;\n";
    }
    if (this->mb.getSymmetryType() == mfront::ORTHOTROPIC) {
      this->behaviourFile << "//! orthotropic axes convention\n";
      if (this->mb.getOrthotropicAxesConvention() ==
          OrthotropicAxesConvention::DEFAULT) {
        this->behaviourFile << "static " << constexpr_c
                            << " OrthotropicAxesConvention oac = "
                               "OrthotropicAxesConvention::DEFAULT;\n";
      } else if (this->mb.getOrthotropicAxesConvention() ==
                 OrthotropicAxesConvention::PIPE) {
        this->behaviourFile << "static " << constexpr_c
                            << " OrthotropicAxesConvention oac = "
                               "OrthotropicAxesConvention::PIPE;\n";
      } else {
        this->throwRuntimeError(
            "BehaviourDSLCommon::writeBehaviourTraitsSpecialisation",
            "internal error : unsupported orthotropic axes convention");
      }
    }
    if ((b) && (this->mb.requiresStressFreeExpansionTreatment(h))) {
      this->behaviourFile << "static " << constexpr_c
                          << " bool hasStressFreeExpansion = true;\n";
    } else {
      this->behaviourFile << "static " << constexpr_c
                          << " bool hasStressFreeExpansion = false;\n";
    }
    if (this->mb.areThermalExpansionCoefficientsDefined()) {
      this->behaviourFile << "static " << constexpr_c
                          << " bool handlesThermalExpansion = true;\n";
    } else {
      this->behaviourFile << "static " << constexpr_c
                          << " bool handlesThermalExpansion = false;\n";
    }
    if (b) {
      this->behaviourFile << "static " << constexpr_c
                          << " unsigned short dimension = N;\n";
    } else {
      this->behaviourFile << "static " << constexpr_c
                          << " unsigned short dimension = 0u;\n";
    }
    this->behaviourFile << "typedef Type NumType;\n"
                        << "static " << constexpr_c
                        << " unsigned short material_properties_nb = "
                        << coefSize << ";\n"
                        << "static " << constexpr_c
                        << " unsigned short internal_variables_nb  = "
                        << stateVarsSize << ";\n"
                        << "static " << constexpr_c
                        << " unsigned short external_variables_nb  = "
                        << externalStateVarsSize << ";\n"
                        << "static " << constexpr_c
                        << " bool hasConsistentTangentOperator = ";
    if (b) {
      if (this->mb.getAttribute<bool>(
              h, BehaviourData::hasConsistentTangentOperator, false)) {
        this->behaviourFile << "true;\n";
      } else {
        this->behaviourFile << "false;\n";
      }
    } else {
      this->behaviourFile << "false;\n";
    }
    this->behaviourFile << "static " << constexpr_c
                        << " bool isConsistentTangentOperatorSymmetric = ";
    if (b) {
      if (this->mb.getAttribute<bool>(
              h, BehaviourData::isConsistentTangentOperatorSymmetric, false)) {
        this->behaviourFile << "true;\n";
      } else {
        this->behaviourFile << "false;\n";
      }
    } else {
      this->behaviourFile << "false;\n";
    }
    this->behaviourFile << "static " << constexpr_c
                        << " bool hasPredictionOperator = ";
    if (b) {
      if (this->mb.getAttribute<bool>(h, BehaviourData::hasPredictionOperator,
                                      false)) {
        this->behaviourFile << "true;\n";
      } else {
        this->behaviourFile << "false;\n";
      }
    } else {
      this->behaviourFile << "false;\n";
    }
    this->behaviourFile << "static " << constexpr_c
                        << " bool hasAPrioriTimeStepScalingFactor = ";
    if (b) {
      if (this->mb.getAttribute<bool>(
              h, BehaviourData::hasAPrioriTimeStepScalingFactor, false)) {
        this->behaviourFile << "true;\n";
      } else {
        this->behaviourFile << "false;\n";
      }
    } else {
      this->behaviourFile << "false;\n";
    }
    // internal enery
    this->behaviourFile << "static " << constexpr_c
                        << " bool hasComputeInternalEnergy = ";
    if (b) {
      if (this->mb.hasCode(h, BehaviourData::ComputeInternalEnergy)) {
        this->behaviourFile << "true;\n";
      } else {
        this->behaviourFile << "false;\n";
      }
    } else {
      this->behaviourFile << "false;\n";
    }
    // dissipated energy
    this->behaviourFile << "static " << constexpr_c
                        << " bool hasComputeDissipatedEnergy = ";
    if (b) {
      if (this->mb.hasCode(h, BehaviourData::ComputeDissipatedEnergy)) {
        this->behaviourFile << "true;\n";
      } else {
        this->behaviourFile << "false;\n";
      }
    } else {
      this->behaviourFile << "false;\n";
    }
    // name of the class
    this->behaviourFile << "/*!\n"
                        << "* \\return the name of the class.\n"
                        << "*/\n"
                        << "static const char*\n"
                        << "getName(void){\n"
                        << "return \"" << this->mb.getClassName() << "\";\n"
                        << "}\n\n"
                        << "};\n\n";
  }

  void
  BehaviourDSLCommon::writeBehaviourParserSpecificInheritanceRelationship() {
    this->behaviourFile << '\n';
  }

  void BehaviourDSLCommon::writeBehaviourParserSpecificTypedefs() {
    // Empty member meant to be overriden in Child if necessary
  }

  void BehaviourDSLCommon::writeBehaviourParserSpecificMembers(
      const Hypothesis) {
    // Empty member meant to be overriden in Child if necessary
  }

  void BehaviourDSLCommon::writeBehaviourParserSpecificIncludes() {
    // Empty member meant to be overriden in Child if necessary
  }

  void BehaviourDSLCommon::writeBehaviourParametersInitializers() {
    if (!this->mb.hasParameters()) {
      return;
    }
    auto mh = this->mb.getDistinctModellingHypotheses();
    mh.insert(ModellingHypothesis::UNDEFINEDHYPOTHESIS);
    for (const auto h : mh) {
      if (this->mb.hasParameters(h)) {
        this->writeBehaviourParametersInitializer(h);
      }
    }
  }  // end of BehaviourDSLCommon::writeBehaviourParametersInitializers

  void BehaviourDSLCommon::writeBehaviourParametersInitializer(
      const Hypothesis h) {
    const auto& md = this->mb.getBehaviourData(h);
    const auto& params = md.getParameters();
    std::string cname(this->mb.getClassName());
    if (h != ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      cname += ModellingHypothesis::toString(h);
    }
    cname += "ParametersInitializer";
    bool rp = false;
    bool ip = false;
    bool up = false;
    bool rp2 = false;
    bool ip2 = false;
    bool up2 = false;
    this->checkBehaviourFile();
    this->behaviourFile << "struct " << cname << '\n'
                        << "{\n"
                        << "static " << cname << "&\n"
                        << "get();\n\n";
    for (const auto& p : params) {
      if (p.type == "real") {
        rp = true;
        if ((h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) ||
            ((h != ModellingHypothesis::UNDEFINEDHYPOTHESIS) &&
             (!this->mb.hasParameter(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                                     p.name)))) {
          rp2 = true;
          if (p.arraySize == 1) {
            this->behaviourFile << "double " << p.name << ";\n";
          } else {
            this->behaviourFile << "tfel::math::tvector<" << p.arraySize
                                << ",double> " << p.name << ";\n";
          }
        }
      } else if (p.type == "int") {
        ip = true;
        if ((h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) ||
            ((h != ModellingHypothesis::UNDEFINEDHYPOTHESIS) &&
             (!this->mb.hasParameter(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                                     p.name)))) {
          ip2 = true;
          this->behaviourFile << "int " << p.name << ";\n";
        }
      } else if (p.type == "ushort") {
        up = true;
        if ((h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) ||
            ((h != ModellingHypothesis::UNDEFINEDHYPOTHESIS) &&
             (!this->mb.hasParameter(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                                     p.name)))) {
          up2 = true;
          this->behaviourFile << "unsigned short " << p.name << ";\n";
        }
      } else {
        this->throwRuntimeError(
            "BehaviourDSLCommon::writeBehaviourParametersInitializer",
            "invalid type for parameter '" + p.name + "' ('" + p.type + "')");
      }
    }
    if (!params.empty()) {
      this->behaviourFile << '\n';
    }
    if (rp) {
      this->behaviourFile << "void set(const char* const,const double);\n\n";
    }
    if (ip) {
      this->behaviourFile << "void set(const char* const,const int);\n\n";
    }
    if (up) {
      this->behaviourFile
          << "void set(const char* const,const unsigned short);\n\n";
    }
    if (rp2) {
      this->behaviourFile << "/*!\n"
                          << " * \\brief convert a string to double\n"
                          << " * \\param[in] p : parameter\n"
                          << " * \\param[in] v : value\n"
                          << " */\n"
                          << "static double getDouble(const std::string&,const "
                             "std::string&);\n";
    }
    if (ip2) {
      this->behaviourFile
          << "/*!\n"
          << " * \\brief convert a string to int\n"
          << " * \\param[in] p : parameter\n"
          << " * \\param[in] v : value\n"
          << " */\n"
          << "static int getInt(const std::string&,const std::string&);\n";
    }
    if (up2) {
      this->behaviourFile << "/*!\n"
                          << " * \\brief convert a string to unsigned short\n"
                          << " * \\param[in] p : parameter\n"
                          << " * \\param[in] v : value\n"
                          << " */\n"
                          << "static unsigned short getUnsignedShort(const "
                             "std::string&,const std::string&);\n";
    }
    this->behaviourFile
        << "private :\n\n"
        << cname << "();\n\n"
        << cname << "(const " << cname << "&);\n\n"
        << cname << "&\n"
        << "operator=(const " << cname << "&);\n"
        << "/*!\n"
        << " * \\brief read the parameters from the given file\n"
        << " * \\param[out] pi : parameters initializer\n"
        << " * \\param[in]  fn : file name\n"
        << " */\n"
        << "static void readParameters(" << cname << "&,const char* const);\n"
        << "};\n\n";
  }  // end of BehaviourDSLCommon::writeBehaviourParametersInitializer

  void BehaviourDSLCommon::writeBehaviourParserSpecificInitializeMethodPart(
      const Hypothesis) {
    // Empty member meant to be overriden in Child if necessary
  }

  void BehaviourDSLCommon::writeBehaviourFileBegin() {
    this->checkBehaviourFile();
    this->writeBehaviourFileHeader();
    this->writeBehaviourFileHeaderBegin();
    this->writeBehaviourIncludes();
    this->writeBehaviourParserSpecificIncludes();
    this->writeIncludes(this->behaviourFile);
    // includes specific to interfaces
    for (const auto& i : this->interfaces) {
      i.second->writeInterfaceSpecificIncludes(this->behaviourFile, this->mb);
    }
    this->writeNamespaceBegin(this->behaviourFile);
    this->writeBehaviourParametersInitializers();
    this->writeBehaviourForwardDeclarations();
    this->writeBehaviourProfiler();
  }  // end of BehaviourDSLCommon::writeBehaviourFileBegin

  void BehaviourDSLCommon::writeBehaviourProfiler() {
    if (this->mb.getAttribute(BehaviourData::profiling, false)) {
      this->checkBehaviourFile();
      this->behaviourFile
          << "/*!\n"
          << " * " << this->mb.getClassName() << " profiler\n"
          << " */\n"
          << "struct " << this->mb.getClassName() << "Profiler\n"
          << "{\n"
          << "//! return the profiler associated with the behaviour\n"
          << "static mfront::BehaviourProfiler&\n"
          << "getProfiler(void);\n"
          << "}; // end of struct " << this->mb.getClassName()
          << "Profiler\n\n";
    }
  }  // end of BehaviourDSLCommon::writeBehaviourProfiler

  void BehaviourDSLCommon::writeBehaviourClass(const Hypothesis h) {
    this->checkBehaviourFile();
    this->writeBehaviourClassBegin(h);
    this->writeBehaviourStandardTFELTypedefs();
    this->behaviourFile << "private :\n\n";
    this->writeBehaviourParserSpecificTypedefs();
    this->writeBehaviourStaticVariables(h);
    this->writeBehaviourIntegrationVariables(h);
    this->writeBehaviourIntegrationVariablesIncrements(h);
    this->writeBehaviourLocalVariables(h);
    this->writeBehaviourParameters(h);
    this->writeBehaviourParserSpecificMembers(h);
    this->writeBehaviourUpdateIntegrationVariables(h);
    this->writeBehaviourUpdateStateVariables(h);
    this->writeBehaviourUpdateAuxiliaryStateVariables(h);
    this->writeBehaviourAdditionalMembers(h);
    this->writeBehaviourPrivate(h);
    this->writeBehaviourDisabledConstructors();
    // from this point, all is public
    this->behaviourFile << "public:\n\n";
    this->writeBehaviourConstructors(h);
    this->writeBehaviourComputeStressFreeExpansion(h);
    this->writeBehaviourInitializeMethod(h);
    this->writeBehaviourSetOutOfBoundsPolicy();
    this->writeBehaviourGetModellingHypothesis();
    this->writeBehaviourCheckBounds(h);
    this->writeBehaviourComputePredictionOperator(h);
    this->writeBehaviourGetTimeStepScalingFactor();
    this->writeBehaviourComputeAPrioriTimeStepScalingFactor();
    this->writeBehaviourIntegrator(h);
    this->writeBehaviourComputeAPosterioriTimeStepScalingFactor();
    this->writeBehaviourComputeInternalEnergy(h);
    this->writeBehaviourComputeDissipatedEnergy(h);
    this->writeBehaviourComputeTangentOperator(h);
    this->writeBehaviourGetTangentOperator();
    this->writeBehaviourUpdateExternalStateVariables(h);
    this->writeBehaviourDestructor();
    this->checkBehaviourFile();
    this->behaviourFile << "private:\n\n";
    this->writeBehaviourComputeAPrioriTimeStepScalingFactorII(h);
    this->writeBehaviourComputeAPosterioriTimeStepScalingFactorII(h);
    this->writeBehaviourTangentOperator();
    this->writeBehaviourPolicyVariable();
    this->writeBehaviourClassEnd();
    this->writeBehaviourOutputOperator(h);
  }

  void BehaviourDSLCommon::writeBehaviourFileEnd() {
    this->checkBehaviourFile();
    this->writeBehaviourTraits();
    this->writeNamespaceEnd(this->behaviourFile);
    this->writeBehaviourFileHeaderEnd();
  }  // end of BehaviourDSLCommon::writeBehaviourFileBegin

  static bool hasUserDefinedPredictionOperatorCode(
      const BehaviourDescription& mb,
      const tfel::material::ModellingHypothesis::Hypothesis h) {
    using tfel::material::getFiniteStrainBehaviourTangentOperatorFlags;
    if (mb.getBehaviourType() ==
        BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR) {
      // all available tangent operators for finite strain behaviours
      const auto tos = getFiniteStrainBehaviourTangentOperatorFlags();
      // search tangent operators defined by the user
      for (const auto& t : tos) {
        const auto ktype =
            convertFiniteStrainBehaviourTangentOperatorFlagToString(t);
        if (mb.hasCode(
                h, BehaviourData::ComputePredictionOperator + '-' + ktype)) {
          return true;
        }
      }
    } else {
      if (mb.hasCode(h, BehaviourData::ComputePredictionOperator)) {
        return true;
      }
    }
    return false;
  }  // end of BehaviourDSLCommon::hasUserDefinedTangentOperatorCode

  void BehaviourDSLCommon::writeBehaviourComputePredictionOperator(
      const Hypothesis h) {
    using namespace std;
    using namespace tfel::material;
    const auto btype = this->mb.getBehaviourTypeFlag();
    if ((!this->mb.getAttribute<bool>(h, BehaviourData::hasPredictionOperator,
                                      false)) &&
        (this->mb.hasCode(h, BehaviourData::ComputePredictionOperator))) {
      this->throwRuntimeError(
          "BehaviourDSLCommon::writeBehaviourComputePredictionOperator : ",
          "attribute 'hasPredictionOperator' is set but no associated code "
          "defined");
    }
    if (!hasUserDefinedPredictionOperatorCode(this->mb, h)) {
      this->behaviourFile
          << "IntegrationResult computePredictionOperator(const SMFlag,const "
             "SMType) override{\n"
          << "throw(std::runtime_error(\"" << this->mb.getClassName()
          << "::computePredictionOperator: \"\n"
          << "\"unsupported prediction operator flag\"));\n"
          << "}\n\n";
      return;
    }
    if (this->mb.getBehaviourType() ==
        BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR) {
      // all available tangent operators for finite strain behaviours
      const auto tos(getFiniteStrainBehaviourTangentOperatorFlags());
      // all known converters
      const auto converters = FiniteStrainBehaviourTangentOperatorConversion::
          getAvailableFiniteStrainBehaviourTangentOperatorConversions();
      // tangent operators defined by the user
      vector<FiniteStrainBehaviourTangentOperatorBase::Flag> ktos;
      for (const auto& t : tos) {
        const auto ktype =
            convertFiniteStrainBehaviourTangentOperatorFlagToString(t);
        if (this->mb.hasCode(
                h, BehaviourData::ComputePredictionOperator + '-' + ktype)) {
          ktos.push_back(t);
        }
      }
      if (!ktos.empty()) {
        // computing all the conversion paths starting from user defined ones
        vector<FiniteStrainBehaviourTangentOperatorConversionPath> paths;
        for (const auto& k : ktos) {
          const auto kpaths =
              FiniteStrainBehaviourTangentOperatorConversionPath::
                  getConversionsPath(k, ktos, converters);
          paths.insert(paths.end(), kpaths.begin(), kpaths.end());
        }
        for (const auto& t : tos) {
          const auto ktype =
              convertFiniteStrainBehaviourTangentOperatorFlagToString(t);
          if (find(ktos.begin(), ktos.end(), t) != ktos.end()) {
            this->behaviourFile
                << "IntegrationResult\ncomputePredictionOperator_" << ktype
                << "(const SMType smt){\n"
                << "using namespace std;\n"
                << "using namespace tfel::math;\n"
                << "using std::vector;\n";
            writeMaterialLaws(
                "BehaviourDSLCommon::writeBehaviourComputePredictionOperator",
                this->behaviourFile, this->mb.getMaterialLaws());
            this->behaviourFile
                << this->mb.getCode(
                       h,
                       BehaviourData::ComputePredictionOperator + "-" + ktype)
                << '\n'
                << "return SUCCESS;\n"
                << "}\n\n";
          } else {
            const auto path =
                FiniteStrainBehaviourTangentOperatorConversionPath::
                    getShortestPath(paths, t);
            if (path.empty()) {
              this->behaviourFile
                  << "IntegrationResult computePredictionOperator_" << ktype
                  << "(const SMType){\n"
                  << "throw(std::runtime_error(\"" << this->mb.getClassName()
                  << "::computePredictionOperator_" << ktype << ": \"\n"
                  << "\"computing the prediction operator '" << ktype
                  << "' is not supported\"));\n"
                  << "}\n\n";
            } else {
              this->behaviourFile
                  << "IntegrationResult computePredictionOperator_" << ktype
                  << "(const SMType smt){\n";
              auto pc = path.begin();
              this->behaviourFile << "using namespace tfel::math;\n";
              this->behaviourFile
                  << "// computing "
                  << convertFiniteStrainBehaviourTangentOperatorFlagToString(
                         pc->from())
                  << '\n';
              const auto k =
                  convertFiniteStrainBehaviourTangentOperatorFlagToString(
                      pc->from());
              this->behaviourFile
                  << "this->computePredictionOperator_" << k << "(smt);\n"
                  << "const "
                  << getFiniteStrainBehaviourTangentOperatorFlagType(pc->from())
                  << "<N,stress>"
                  << " tangentOperator_"
                  << convertFiniteStrainBehaviourTangentOperatorFlagToString(
                         pc->from())
                  << " = this->Dt.template get<"
                  << getFiniteStrainBehaviourTangentOperatorFlagType(pc->from())
                  << "<N,stress> >();\n";
              for (; pc != path.end();) {
                const auto converter = *pc;
                if (++pc == path.end()) {
                  this->behaviourFile << converter.getFinalConversion() << '\n';
                } else {
                  this->behaviourFile << converter.getIntermediateConversion()
                                      << '\n';
                }
              }
              this->behaviourFile << "return SUCCESS;\n"
                                  << "}\n\n";
            }
          }
        }
        this->behaviourFile
            << "IntegrationResult computePredictionOperator(const SMFlag "
               "smflag,const SMType smt) override{\n"
            << "using namespace std;\n"
            << "switch(smflag){\n";
        for (const auto& t : tos) {
          const auto ktype =
              convertFiniteStrainBehaviourTangentOperatorFlagToString(t);
          this->behaviourFile << "case " << ktype << ":\n"
                              << "return this->computePredictionOperator_"
                              << ktype << "(smt);\n";
        }
        this->behaviourFile << "}\n"
                            << "throw(runtime_error(\""
                            << this->mb.getClassName()
                            << "::computePredictionOperator: \"\n"
                            << "\"unsupported prediction operator flag\"));\n"
                            << "}\n\n";
      }
    } else {
      this->behaviourFile << "IntegrationResult\n"
                          << "computePredictionOperator(const SMFlag "
                             "smflag,const SMType smt) override{\n"
                          << "using namespace std;\n"
                          << "using namespace tfel::math;\n"
                          << "using std::vector;\n";
      writeMaterialLaws(
          "BehaviourDSLCommon::writeBehaviourComputePredictionOperator",
          this->behaviourFile, this->mb.getMaterialLaws());
      if ((this->mb.getBehaviourType() ==
           BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) ||
          (this->mb.getBehaviourType() ==
           BehaviourDescription::COHESIVEZONEMODEL)) {
        if (mb.useQt()) {
          this->behaviourFile
              << "if(smflag!=MechanicalBehaviour<" << btype
              << ",hypothesis,Type,use_qt>::STANDARDTANGENTOPERATOR){\n"
              << "throw(runtime_error(\"invalid prediction operator flag\"));\n"
              << "}\n";
        } else {
          this->behaviourFile
              << "if(smflag!=MechanicalBehaviour<" << btype
              << ",hypothesis,Type,false>::STANDARDTANGENTOPERATOR){\n"
              << "throw(runtime_error(\"invalid prediction operator flag\"));\n"
              << "}\n";
        }
      }
      this->behaviourFile << this->mb.getCode(
                                 h, BehaviourData::ComputePredictionOperator)
                          << "return SUCCESS;\n"
                          << "}\n\n";
    }
  }  // end of BehaviourDSLCommon::writeBehaviourComputePredictionOperator(void)

  void BehaviourDSLCommon::writeBehaviourComputeTangentOperator(
      const Hypothesis h) {
    using namespace std;
    using namespace tfel::material;
    if (this->mb.getBehaviourType() ==
        BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR) {
      // all available tangent operators for finite strain behaviours
      const auto tos(getFiniteStrainBehaviourTangentOperatorFlags());
      // all known converters
      const auto converters = FiniteStrainBehaviourTangentOperatorConversion::
          getAvailableFiniteStrainBehaviourTangentOperatorConversions();
      // tangent operators defined by the user
      vector<FiniteStrainBehaviourTangentOperatorBase::Flag> ktos;
      for (const auto& t : tos) {
        const auto ktype =
            convertFiniteStrainBehaviourTangentOperatorFlagToString(t);
        if (this->mb.hasCode(
                h, BehaviourData::ComputeTangentOperator + '-' + ktype)) {
          ktos.push_back(t);
        }
      }
      if (!ktos.empty()) {
        // computing all the conversion paths starting from user defined ones
        vector<FiniteStrainBehaviourTangentOperatorConversionPath> paths;
        for (const auto& k : ktos) {
          const auto kpaths =
              FiniteStrainBehaviourTangentOperatorConversionPath::
                  getConversionsPath(k, ktos, converters);
          paths.insert(paths.end(), kpaths.begin(), kpaths.end());
        }
        for (const auto& t : tos) {
          const auto ktype =
              convertFiniteStrainBehaviourTangentOperatorFlagToString(t);
          if (find(ktos.begin(), ktos.end(), t) != ktos.end()) {
            this->behaviourFile << "bool computeConsistentTangentOperator_"
                                << ktype << "(const SMType smt){\n"
                                << "using namespace std;\n"
                                << "using namespace tfel::math;\n"
                                << "using std::vector;\n";
            writeMaterialLaws(
                "BehaviourDSLCommon::writeBehaviourComputeTangentOperator",
                this->behaviourFile, this->mb.getMaterialLaws());
            this->behaviourFile
                << this->mb.getCode(
                       h, BehaviourData::ComputeTangentOperator + "-" + ktype)
                << '\n'
                << "return true;\n"
                << "}\n\n";
          } else {
            const auto path =
                FiniteStrainBehaviourTangentOperatorConversionPath::
                    getShortestPath(paths, t);
            if (path.empty()) {
              this->behaviourFile << "bool computeConsistentTangentOperator_"
                                  << ktype << "(const SMType){\n"
                                  << "using namespace std;\n"
                                  << "string msg(\"" << this->mb.getClassName()
                                  << "::computeConsistentTangentOperator_"
                                  << ktype << " : \");\n"
                                  << "msg += \"computing the tangent operator '"
                                  << ktype << "' is not supported\";\n"
                                  << "throw(runtime_error(msg));\n"
                                  << "}\n\n";
            } else {
              this->behaviourFile << "bool computeConsistentTangentOperator_"
                                  << ktype << "(const SMType smt){\n";
              auto pc = path.begin();
              this->behaviourFile << "using namespace tfel::math;\n";
              this->behaviourFile
                  << "// computing "
                  << convertFiniteStrainBehaviourTangentOperatorFlagToString(
                         pc->from())
                  << '\n';
              const auto k =
                  convertFiniteStrainBehaviourTangentOperatorFlagToString(
                      pc->from());
              this->behaviourFile
                  << "this->computeConsistentTangentOperator_" << k
                  << "(smt);\n"
                  << "const "
                  << getFiniteStrainBehaviourTangentOperatorFlagType(pc->from())
                  << "<N,stress>"
                  << " tangentOperator_"
                  << convertFiniteStrainBehaviourTangentOperatorFlagToString(
                         pc->from())
                  << " = this->Dt.template get<"
                  << getFiniteStrainBehaviourTangentOperatorFlagType(pc->from())
                  << "<N,stress> >();\n";
              for (; pc != path.end();) {
                const auto converter = *pc;
                if (++pc == path.end()) {
                  this->behaviourFile << converter.getFinalConversion() << '\n';
                } else {
                  this->behaviourFile << converter.getIntermediateConversion()
                                      << '\n';
                }
              }
              this->behaviourFile << "return true;\n"
                                  << "}\n\n";
            }
          }
        }
        this->behaviourFile << "bool computeConsistentTangentOperator(const "
                               "SMFlag smflag,const SMType smt){\n"
                            << "using namespace std;\n"
                            << "switch(smflag){\n";
        for (const auto& t : tos) {
          const auto ktype =
              convertFiniteStrainBehaviourTangentOperatorFlagToString(t);
          this->behaviourFile
              << "case " << ktype << ":\n"
              << "return this->computeConsistentTangentOperator_" << ktype
              << "(smt);\n";
        }
        this->behaviourFile << "}\n"
                            << "throw(std::runtime_error(\""
                            << this->mb.getClassName()
                            << "::computeConsistentTangentOperator : \"\n"
                            << "\"unsupported tangent operator flag\"));\n"
                            << "}\n\n";
      }
    } else {
      if (this->mb.hasCode(h, BehaviourData::ComputeTangentOperator)) {
        this->behaviourFile
            << "bool computeConsistentTangentOperator(const SMType smt){\n"
            << "using namespace std;\n"
            << "using namespace tfel::math;\n"
            << "using std::vector;\n";
        writeMaterialLaws(
            "BehaviourDSLCommon::writeBehaviourComputeTangentOperator",
            this->behaviourFile, this->mb.getMaterialLaws());
        this->behaviourFile
            << this->mb.getCode(h, BehaviourData::ComputeTangentOperator)
            << '\n'
            << "return true;\n"
            << "}\n\n";
      }
    }
  }  // end of BehaviourDSLCommon::writeBehaviourComputeTangentOperator

  void BehaviourDSLCommon::writeBehaviourGetTangentOperator() {
    this->checkBehaviourFile();
    this->behaviourFile << "const TangentOperator&\n"
                        << "getTangentOperator(void) const{\n"
                        << "return this->Dt;\n"
                        << "}\n\n";
  }  // end of BehaviourDSLCommon::writeBehaviourComputeTangentOperator(void)

  void BehaviourDSLCommon::writeBehaviourGetTimeStepScalingFactor() {
    this->checkBehaviourFile();
    this->behaviourFile
        << "real\n"
           "getMinimalTimeStepScalingFactor(void) const override{\n"
           "  return this->minimal_time_step_scaling_factor;\n"
           "}\n\n";
  }

  void BehaviourDSLCommon::writeBehaviourComputeAPrioriTimeStepScalingFactor() {
    this->checkBehaviourFile();
    this->behaviourFile
        << "std::pair<bool,real>\n"
           "computeAPrioriTimeStepScalingFactor(const real "
           "current_time_step_scaling_factor) const override{\n"
           "const auto time_scaling_factor = "
           "this->computeAPrioriTimeStepScalingFactorII();\n"
           "return {time_scaling_factor.first,\n"
           "        std::min(std::min(std::max(time_scaling_factor.second,\n"
           "                                   "
           "this->minimal_time_step_scaling_factor),\n"
           "                          "
           "this->maximal_time_step_scaling_factor),\n"
           "                  current_time_step_scaling_factor)};\n"
           "}\n\n";
  }  // end of
     // BehaviourDSLCommon::writeBehaviourComputeAPrioriTimeStepScalingFactor(void)

  void BehaviourDSLCommon::writeBehaviourComputeAPrioriTimeStepScalingFactorII(
      const Hypothesis h) {
    this->checkBehaviourFile();
    this->behaviourFile
        << "std::pair<bool,real>\n"
        << "computeAPrioriTimeStepScalingFactorII(void) const{\n";
    if (this->mb.hasCode(h, BehaviourData::APrioriTimeStepScalingFactor)) {
      this->behaviourFile << "using namespace std;\n"
                          << "using namespace tfel::math;\n"
                          << "using std::vector;\n";
      writeMaterialLaws(
          "BehaviourDSLCommon::"
          "writeBehaviourComputeAPrioriTimeStepScalingFactorII",
          this->behaviourFile, this->mb.getMaterialLaws());
      this->behaviourFile << this->mb.getCode(
                                 h, BehaviourData::APrioriTimeStepScalingFactor)
                          << '\n';
    }
    this->behaviourFile
        << "return {true,this->maximal_time_step_scaling_factor};\n"
        << "}\n\n";
  }

  void
  BehaviourDSLCommon::writeBehaviourComputeAPosterioriTimeStepScalingFactor() {
    this->checkBehaviourFile();
    this->behaviourFile
        << "std::pair<bool,real>\n"
           "computeAPosterioriTimeStepScalingFactor(const real "
           "current_time_step_scaling_factor) const override{\n"
           "const auto time_scaling_factor = "
           "this->computeAPosterioriTimeStepScalingFactorII();\n"
           "return {time_scaling_factor.first,\n"
           "        std::min(std::min(std::max(time_scaling_factor.second,\n"
           "                                   "
           "this->minimal_time_step_scaling_factor),\n"
           "                          "
           "this->maximal_time_step_scaling_factor),\n"
           "                 current_time_step_scaling_factor)};\n"
           "}\n\n";
  }  // end of
     // BehaviourDSLCommon::writeBehaviourComputeAPosterioriTimeStepScalingFactor(void)

  void
  BehaviourDSLCommon::writeBehaviourComputeAPosterioriTimeStepScalingFactorII(
      const Hypothesis h) {
    this->checkBehaviourFile();
    this->behaviourFile
        << "std::pair<bool,real>\n"
        << "computeAPosterioriTimeStepScalingFactorII(void) const{\n";
    if (this->mb.hasCode(h, BehaviourData::APosterioriTimeStepScalingFactor)) {
      this->behaviourFile << "using namespace std;\n"
                          << "using namespace tfel::math;\n"
                          << "using std::vector;\n";
      writeMaterialLaws(
          "BehaviourDSLCommon::"
          "writeBehaviourComputeAPosterioriTimeStepScalingFactorII",
          this->behaviourFile, this->mb.getMaterialLaws());
      this->behaviourFile
          << this->mb.getCode(h,
                              BehaviourData::APosterioriTimeStepScalingFactor)
          << '\n';
    }
    this->behaviourFile
        << "return {true,this->maximal_time_step_scaling_factor};\n"
        << "}\n\n";
  }  // end of
     // BehaviourDSLCommon::writeBehaviourComputeAPosterioriTimeStepScalingFactor(void)

  void BehaviourDSLCommon::writeBehaviourTangentOperator() {
    this->checkBehaviourFile();
    this->behaviourFile << "//! Tangent operator;\n"
                        << "TangentOperator Dt;\n";
  }  // end of BehaviourDSLCommon::writeBehaviourTangentOperator()

  void BehaviourDSLCommon::checkIntegrationDataFile() const {
    if ((!this->integrationDataFile) || (!this->integrationDataFile.good())) {
      this->throwRuntimeError(
          "BehaviourDSLCommon::checkIntegrationDataOutputFile",
          "ouput file is not valid");
    }
  }

  void BehaviourDSLCommon::writeIntegrationDataFileHeader() {
    this->checkIntegrationDataFile();
    this->integrationDataFile << "/*!\n";
    this->integrationDataFile << "* \\file   " << this->integrationDataFileName
                              << '\n';
    this->integrationDataFile << "* \\brief  "
                              << "this file implements the "
                              << this->mb.getClassName() << "IntegrationData"
                              << " class.\n";
    this->integrationDataFile << "*         File generated by ";
    this->integrationDataFile << MFrontHeader::getVersionName() << " ";
    this->integrationDataFile << "version " << MFrontHeader::getVersionNumber();
    this->integrationDataFile << '\n';
    if (!this->authorName.empty()) {
      this->integrationDataFile << "* \\author " << this->authorName << '\n';
    }
    if (!this->date.empty()) {
      this->integrationDataFile << "* \\date   " << this->date << '\n';
    }
    this->integrationDataFile << " */\n\n";
  }

  void BehaviourDSLCommon::writeIntegrationDataFileHeaderBegin() {
    this->checkIntegrationDataFile();
    this->integrationDataFile
        << "#ifndef LIB_TFELMATERIAL_" << makeUpperCase(this->mb.getClassName())
        << "_INTEGRATION_DATA_HXX\n"
        << "#define LIB_TFELMATERIAL_" << makeUpperCase(this->mb.getClassName())
        << "_INTEGRATION_DATA_HXX\n\n";
  }

  void BehaviourDSLCommon::writeIntegrationDataFileHeaderEnd() {
    this->checkIntegrationDataFile();
    this->integrationDataFile << "#endif /* LIB_TFELMATERIAL_"
                              << makeUpperCase(this->mb.getClassName())
                              << "_INTEGRATION_DATA_HXX */\n";
  }

  void BehaviourDSLCommon::writeIntegrationDataStandardTFELIncludes() {
    bool b1 = false;
    bool b2 = false;
    this->checkIntegrationDataFile();
    this->integrationDataFile
        << "#include<string>\n"
        << "#include<iostream>\n"
        << "#include<limits>\n"
        << "#include<stdexcept>\n"
        << "#include<algorithm>\n\n"
        << "#include\"TFEL/Config/TFELConfig.hxx\"\n"
        << "#include\"TFEL/Config/TFELTypes.hxx\"\n"
        << "#include\"TFEL/Metaprogramming/StaticAssert.hxx\"\n"
        << "#include\"TFEL/TypeTraits/IsFundamentalNumericType.hxx\"\n"
        << "#include\"TFEL/TypeTraits/IsScalar.hxx\"\n"
        << "#include\"TFEL/TypeTraits/IsReal.hxx\"\n"
        << "#include\"TFEL/TypeTraits/Promote.hxx\"\n";
    this->mb.requiresTVectorOrVectorIncludes(b1, b2);
    if (b1) {
      this->integrationDataFile
          << "#include\"TFEL/Math/tvector.hxx\"\n"
          << "#include\"TFEL/Math/Vector/tvectorIO.hxx\"\n";
    }
    if (b2) {
      this->integrationDataFile << "#include\"TFEL/Math/vector.hxx\"\n";
    }
    this->integrationDataFile << "#include\"TFEL/Math/stensor.hxx\"\n"
                              << "#include\"TFEL/Math/st2tost2.hxx\"\n";
    if (this->mb.getBehaviourType() ==
        BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR) {
      this->integrationDataFile << "#include\"TFEL/Math/tensor.hxx\"\n"
                                << "#include\"TFEL/Math/t2tot2.hxx\"\n"
                                << "#include\"TFEL/Math/t2tost2.hxx\"\n"
                                << "#include\"TFEL/Math/st2tot2.hxx\"\n";
    }
  }

  void BehaviourDSLCommon::writeIntegrationDataDefaultMembers() {
    this->checkIntegrationDataFile();
    this->integrationDataFile << "protected: \n\n";
    for (const auto& v : this->mb.getMainVariables()) {
      if (v.first.increment_known) {
        this->integrationDataFile
            << "/*!\n"
            << " * \\brief " << v.first.name << " increment\n"
            << " */\n"
            << v.first.type << " d" << v.first.name << ";\n\n";
      } else {
        this->integrationDataFile
            << "/*!\n"
            << " * \\brief " << v.first.name << " at the end of the time step\n"
            << " */\n"
            << v.first.type << " " << v.first.name << "1;\n\n";
      }
    }
    this->integrationDataFile << "/*!\n"
                              << " * \\brief time increment\n"
                              << " */\n"
                              << "time dt;\n"
                              << "/*!\n"
                              << " * \\brief temperature increment\n"
                              << " */\n"
                              << "temperature dT;\n\n";
  }

  void BehaviourDSLCommon::writeIntegrationDataStandardTFELTypedefs() {
    this->checkIntegrationDataFile();
    this->integrationDataFile
        << "static " << constexpr_c << " unsigned short TVectorSize = N;\n"
        << "typedef tfel::math::StensorDimeToSize<N> StensorDimeToSize;\n"
        << "static " << constexpr_c << " unsigned short StensorSize = "
        << "StensorDimeToSize::value;\n"
        << "typedef tfel::math::TensorDimeToSize<N> TensorDimeToSize;\n"
        << "static " << constexpr_c << " unsigned short TensorSize = "
        << "TensorDimeToSize::value;\n\n";
    this->writeStandardTFELTypedefs(this->integrationDataFile);
    this->integrationDataFile << '\n';
  }

  void BehaviourDSLCommon::writeIntegrationDataDisabledConstructors() {
    this->checkIntegrationDataFile();
  }

  void BehaviourDSLCommon::writeIntegrationDataConstructors(
      const Hypothesis h) {
    const auto& md = this->mb.getBehaviourData(h);
    this->checkIntegrationDataFile();
    this->integrationDataFile
        << "/*!\n"
        << "* \\brief Default constructor\n"
        << "*/\n"
        << this->mb.getClassName() << "IntegrationData()\n"
        << "{}\n\n"
        << "/*!\n"
        << "* \\brief Copy constructor\n"
        << "*/\n"
        << this->mb.getClassName() << "IntegrationData(const "
        << this->mb.getClassName() << "IntegrationData& src)\n"
        << ": ";
    for (const auto& v : this->mb.getMainVariables()) {
      if (v.first.increment_known) {
        this->integrationDataFile << "d" << v.first.name << "(src.d"
                                  << v.first.name << "),\n";
      } else {
        this->integrationDataFile << v.first.name << "1(src." << v.first.name
                                  << "1),\n";
      }
    }
    this->integrationDataFile << "dt(src.dt),\n"
                              << "dT(src.dT)";
    for (const auto& v : md.getExternalStateVariables()) {
      this->integrationDataFile << ",\nd" << v.name << "(src.d" << v.name
                                << ")";
    }
    this->integrationDataFile << "\n{}\n\n";
    // Creating constructor for external interfaces
    for (const auto& i : this->interfaces) {
      if (i.second->isModellingHypothesisHandled(h, this->mb)) {
        i.second->writeIntegrationDataConstructor(this->integrationDataFile, h,
                                                  this->mb);
      }
    }
  }

  void BehaviourDSLCommon::writeIntegrationDataScaleOperators(
      const Hypothesis h) {
    const auto& md = this->mb.getBehaviourData(h);
    bool iknown = true;
    for (const auto& v : this->mb.getMainVariables()) {
      iknown = v.first.increment_known;
    }
    this->checkIntegrationDataFile();
    this->integrationDataFile
        << "/*\n"
        << "* Multiplication by a scalar.\n"
        << "*/\n"
        << "template<typename Scal>\n"
        << "typename std::enable_if<\n"
        << "tfel::typetraits::IsFundamentalNumericType<Scal>::cond&&\n"
        << "tfel::typetraits::IsScalar<Scal>::cond&&\n"
        << "tfel::typetraits::IsReal<Scal>::cond&&\n"
        << "std::is_same<Type,"
        << "typename tfel::typetraits::Promote"
        << "<Type,Scal>::type>::value,\n"
        << this->mb.getClassName() << "IntegrationData&\n"
        << ">::type\n";
    if (!iknown) {
      if (this->mb.useQt()) {
        this->integrationDataFile
            << "scale(const " << this->mb.getClassName()
            << "BehaviourData<hypothesis,Type,use_qt>& behaviourData, const "
               "Scal time_scaling_factor){\n";
      } else {
        this->integrationDataFile
            << "scale(const " << this->mb.getClassName()
            << "BehaviourData<hypothesis,Type,false>& behaviourData, const "
               "Scal time_scaling_factor){\n";
      }
    } else {
      if (this->mb.useQt()) {
        this->integrationDataFile << "scale(const " << this->mb.getClassName()
                                  << "BehaviourData<hypothesis,Type,use_qt>&, "
                                     "const Scal time_scaling_factor){\n";
      } else {
        this->integrationDataFile << "scale(const " << this->mb.getClassName()
                                  << "BehaviourData<hypothesis,Type,false>&, "
                                     "const Scal time_scaling_factor){\n";
      }
    }
    this->integrationDataFile << "this->dt   *= time_scaling_factor;\n";
    for (const auto& v : this->mb.getMainVariables()) {
      if (v.first.increment_known) {
        this->integrationDataFile << "this->d" << v.first.name
                                  << " *= time_scaling_factor;\n";
      } else {
        this->integrationDataFile
            << "this->" << v.first.name
            << "1 = (1-time_scaling_factor)*(behaviourData." << v.first.name
            << "0)+time_scaling_factor*(this->" << v.first.name << "1);\n";
      }
    }
    this->integrationDataFile << "this->dT   *= time_scaling_factor;\n";
    for (const auto& v : md.getExternalStateVariables()) {
      this->integrationDataFile << "this->d" << v.name
                                << " *= time_scaling_factor;\n";
    }
    this->integrationDataFile << "return *this;\n"
                              << "}\n\n";
  }  // end of BehaviourDSLCommon::writeIntegrationDataScaleOpeartors

  void BehaviourDSLCommon::writeIntegrationDataUpdateDrivingVariablesMethod() {
    bool iknown = true;
    for (const auto& v : this->mb.getMainVariables()) {
      iknown = v.first.increment_known;
    }
    this->checkIntegrationDataFile();
    this->integrationDataFile
        << "/*!\n"
        << "* \\brief update the driving variable in case of substepping.\n"
        << "*/\n"
        << this->mb.getClassName() << "IntegrationData&\n";
    if (!iknown) {
      if (this->mb.useQt()) {
        this->integrationDataFile
            << "updateDrivingVariables(const " << this->mb.getClassName()
            << "BehaviourData<hypothesis,Type,use_qt>& behaviourData){\n";
      } else {
        this->integrationDataFile
            << "updateDrivingVariables(const " << this->mb.getClassName()
            << "BehaviourData<hypothesis,Type,false>& behaviourData){\n";
      }
    } else {
      if (this->mb.useQt()) {
        this->integrationDataFile
            << "updateDrivingVariables(const " << this->mb.getClassName()
            << "BehaviourData<hypothesis,Type,use_qt>&){\n";
      } else {
        this->integrationDataFile
            << "updateDrivingVariables(const " << this->mb.getClassName()
            << "BehaviourData<hypothesis,Type,false>&){\n";
      }
    }
    for (const auto& v : this->mb.getMainVariables()) {
      if (!v.first.increment_known) {
        this->integrationDataFile << "this->" << v.first.name << "1 += "
                                  << "this->" << v.first.name
                                  << "1 - (behaviourData." << v.first.name
                                  << "0);\n";
      }
    }
    this->integrationDataFile << "return *this;\n"
                              << "}\n\n";
  }  // end of BehaviourDSLCommon::writeIntegrationUpdateDrivingVariablesMethod

  void BehaviourDSLCommon::writeIntegrationDataClassHeader() {
    this->checkIntegrationDataFile();
    this->integrationDataFile
        << "/*!\n"
        << "* \\class " << this->mb.getClassName() << "IntegrationData\n"
        << "* \\brief This class implements the " << this->mb.getClassName()
        << "IntegrationData"
        << " behaviour.\n"
        << "* \\param unsigned short N, space dimension.\n"
        << "* \\param typename Type, numerical type.\n"
        << "* \\param bool use_qt, conditional saying if quantities are use.\n";
    if (!this->authorName.empty()) {
      this->integrationDataFile << "* \\author " << this->authorName << '\n';
    }
    if (!this->date.empty()) {
      this->integrationDataFile << "* \\date   " << this->date << '\n';
    }
    this->integrationDataFile << "*/\n";
  }

  void BehaviourDSLCommon::writeIntegrationDataForwardDeclarations() {
    this->checkIntegrationDataFile();
    this->integrationDataFile << "// Forward Declaration\n"
                              << "template<ModellingHypothesis::Hypothesis "
                                 "hypothesis,typename Type,bool use_qt>\n"
                              << "class " << this->mb.getClassName()
                              << "IntegrationData;\n\n";
    if (this->mb.useQt()) {
      this->integrationDataFile
          << "// Forward Declaration\n"
          << "template<ModellingHypothesis::Hypothesis hypothesis,typename "
             "Type,bool use_qt>\n"
          << "std::ostream&\n operator <<(std::ostream&,"
          << "const " << this->mb.getClassName()
          << "IntegrationData<hypothesis,Type,use_qt>&);\n\n";
    } else {
      this->integrationDataFile
          << "// Forward Declaration\n"
          << "template<ModellingHypothesis::Hypothesis hypothesis,typename "
             "Type>\n"
          << "std::ostream&\n operator <<(std::ostream&,"
          << "const " << this->mb.getClassName()
          << "IntegrationData<hypothesis,Type,false>&);\n\n";
    }
    // maintenant, il faut déclarer toutes les spécialisations partielles...
    const auto& mh = this->mb.getModellingHypotheses();
    for (const auto& h : mh) {
      if (this->mb.hasSpecialisedMechanicalData(h)) {
        if (this->mb.useQt()) {
          this->integrationDataFile
              << "// Forward Declaration\n"
              << "template<typename Type,bool use_qt>\n"
              << "std::ostream&\n operator <<(std::ostream&,"
              << "const " << this->mb.getClassName()
              << "IntegrationData<ModellingHypothesis::"
              << ModellingHypothesis::toUpperCaseString(h)
              << ",Type,use_qt>&);\n\n";
        } else {
          this->integrationDataFile
              << "// Forward Declaration\n"
              << "template<typename Type>\n"
              << "std::ostream&\n operator <<(std::ostream&,"
              << "const " << this->mb.getClassName()
              << "IntegrationData<ModellingHypothesis::"
              << ModellingHypothesis::toUpperCaseString(h)
              << ",Type,false>&);\n\n";
        }
      }
    }
  }

  void BehaviourDSLCommon::writeIntegrationDataClassBegin(const Hypothesis h) {
    using namespace std;
    this->checkIntegrationDataFile();
    if (h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      if (this->mb.useQt()) {
        this->integrationDataFile << "template<ModellingHypothesis::Hypothesis "
                                     "hypothesis,typename Type,bool use_qt>\n";
        this->integrationDataFile << "class " << this->mb.getClassName()
                                  << "IntegrationData\n";
      } else {
        this->integrationDataFile << "template<ModellingHypothesis::Hypothesis "
                                     "hypothesis,typename Type>\n";
        this->integrationDataFile << "class " << this->mb.getClassName()
                                  << "IntegrationData<hypothesis,Type,false>\n";
      }
    } else {
      if (this->mb.useQt()) {
        this->integrationDataFile << "template<typename Type,bool use_qt>\n";
        this->integrationDataFile << "class " << this->mb.getClassName()
                                  << "IntegrationData<ModellingHypothesis::"
                                  << ModellingHypothesis::toUpperCaseString(h)
                                  << ",Type,use_qt>\n";
      } else {
        this->integrationDataFile << "template<typename Type>\n";
        this->integrationDataFile << "class " << this->mb.getClassName()
                                  << "IntegrationData<ModellingHypothesis::"
                                  << ModellingHypothesis::toUpperCaseString(h)
                                  << ",Type,false>\n";
      }
    }
    this->integrationDataFile << "{\n\n";
    if (h != ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      this->integrationDataFile
          << "static " << constexpr_c
          << " ModellingHypothesis::Hypothesis hypothesis = "
          << "ModellingHypothesis::"
          << ModellingHypothesis::toUpperCaseString(h) << ";\n";
    }
    this->integrationDataFile
        << "static " << constexpr_c
        << " unsigned short N = "
           "ModellingHypothesisToSpaceDimension<hypothesis>::value;\n";
    this->integrationDataFile << "TFEL_STATIC_ASSERT(N==1||N==2||N==3);\n";
    this->integrationDataFile << "TFEL_STATIC_ASSERT(tfel::typetraits::"
                              << "IsFundamentalNumericType<Type>::cond);\n";
    this->integrationDataFile
        << "TFEL_STATIC_ASSERT(tfel::typetraits::IsReal<Type>::cond);\n\n";
    this->integrationDataFile
        << "friend std::ostream& operator<< <>(std::ostream&,const ";
    this->integrationDataFile << this->mb.getClassName()
                              << "IntegrationData&);\n\n";
  }

  void BehaviourDSLCommon::writeIntegrationDataOutputOperator(
      const Hypothesis h) {
    using namespace std;
    const auto& md = this->mb.getBehaviourData(h);
    VariableDescriptionContainer::const_iterator p;
    map<DrivingVariable, ThermodynamicForce>::const_iterator p2;
    this->checkBehaviourFile();
    if (h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      if (this->mb.useQt()) {
        this->integrationDataFile << "template<ModellingHypothesis::Hypothesis "
                                     "hypothesis,typename Type,bool use_qt>\n";
        this->integrationDataFile << "std::ostream&\n";
        this->integrationDataFile << "operator <<(std::ostream& os,";
        this->integrationDataFile
            << "const " << this->mb.getClassName()
            << "IntegrationData<hypothesis,Type,use_qt>& b)\n";
      } else {
        this->integrationDataFile << "template<ModellingHypothesis::Hypothesis "
                                     "hypothesis,typename Type>\n";
        this->integrationDataFile << "std::ostream&\n";
        this->integrationDataFile << "operator <<(std::ostream& os,";
        this->integrationDataFile
            << "const " << this->mb.getClassName()
            << "IntegrationData<hypothesis,Type,false>& b)\n";
      }
    } else {
      if (this->mb.useQt()) {
        this->integrationDataFile << "template<typename Type,bool use_qt>\n";
        this->integrationDataFile << "std::ostream&\n";
        this->integrationDataFile << "operator <<(std::ostream& os,";
        this->integrationDataFile << "const " << this->mb.getClassName()
                                  << "IntegrationData<ModellingHypothesis::"
                                  << ModellingHypothesis::toUpperCaseString(h)
                                  << ",Type,use_qt>& b)\n";
      } else {
        this->integrationDataFile << "template<typename Type>\n";
        this->integrationDataFile << "std::ostream&\n";
        this->integrationDataFile << "operator <<(std::ostream& os,";
        this->integrationDataFile << "const " << this->mb.getClassName()
                                  << "IntegrationData<ModellingHypothesis::"
                                  << ModellingHypothesis::toUpperCaseString(h)
                                  << ",Type,false>& b)\n";
      }
    }
    this->integrationDataFile << "{\n";
    this->integrationDataFile << "using namespace std;\n";
    for (p2 = this->mb.getMainVariables().begin();
         p2 != this->mb.getMainVariables().end(); ++p2) {
      if (p2->first.increment_known) {
        this->integrationDataFile << "os << \"d" << p2->first.name
                                  << " : \" << b.d" << p2->first.name
                                  << " << '\\n';\n";
      } else {
        this->integrationDataFile << "os << \"" << p2->first.name
                                  << "1 : \" << b." << p2->first.name
                                  << "1 << endl;\n";
      }
    }
    this->integrationDataFile << "os << \"dt : \" << b.dt << endl;\n";
    this->integrationDataFile << "os << \"dT : \" << b.dT << endl;\n";
    for (p = md.getExternalStateVariables().begin();
         p != md.getExternalStateVariables().end(); ++p) {
      this->integrationDataFile << "os << \"d" << p->name << " : \" << b.d"
                                << p->name << " << '\\n';\n";
    }
    this->integrationDataFile << "return os;\n";
    this->integrationDataFile << "}\n\n";
  }  // end of BehaviourDSLCommon::writeIntegrationDataOutputOperator()

  void BehaviourDSLCommon::writeIntegrationDataClassEnd() {
    this->checkIntegrationDataFile();
    this->integrationDataFile << "}; // end of " << this->mb.getClassName()
                              << "IntegrationData"
                              << "class\n\n";
  }

  void BehaviourDSLCommon::writeIntegrationDataExternalStateVariables(
      const Hypothesis h) {
    const auto& md = this->mb.getBehaviourData(h);
    this->checkIntegrationDataFile();
    this->writeVariablesDeclarations(this->integrationDataFile,
                                     md.getExternalStateVariables(), "d", "",
                                     this->fileName, false);
  }

  void BehaviourDSLCommon::writeIntegrationDataFileBegin() {
    this->checkIntegrationDataFile();
    this->writeIntegrationDataFileHeader();
    this->writeIntegrationDataFileHeaderBegin();
    this->writeIntegrationDataStandardTFELIncludes();
    this->writeIncludes(this->integrationDataFile);
    // includes specific to interfaces
    for (const auto& i : this->interfaces) {
      i.second->writeInterfaceSpecificIncludes(this->integrationDataFile,
                                               this->mb);
    }
    this->writeNamespaceBegin(this->integrationDataFile);
    this->writeIntegrationDataForwardDeclarations();
  }  // end of BehaviourDSLCommon::writeIntegrationDataFile

  void BehaviourDSLCommon::writeIntegrationDataClass(const Hypothesis h) {
    this->checkIntegrationDataFile();
    this->writeIntegrationDataClassBegin(h);
    this->writeIntegrationDataStandardTFELTypedefs();
    this->writeIntegrationDataDefaultMembers();
    this->writeIntegrationDataExternalStateVariables(h);
    this->writeIntegrationDataDisabledConstructors();
    this->integrationDataFile << "public:\n\n";
    this->writeIntegrationDataConstructors(h);
    this->writeIntegrationDataMainVariablesSetters();
    this->writeIntegrationDataScaleOperators(h);
    this->writeIntegrationDataUpdateDrivingVariablesMethod();
    this->writeIntegrationDataClassEnd();
    this->writeIntegrationDataOutputOperator(h);
  }

  void BehaviourDSLCommon::writeIntegrationDataFileEnd() {
    this->checkIntegrationDataFile();
    this->writeNamespaceEnd(this->integrationDataFile);
    this->writeIntegrationDataFileHeaderEnd();
  }  // end of BehaviourDSLCommon::writeIntegrationDataFileEnd

  void BehaviourDSLCommon::checkSrcFile() const {
    using namespace std;
    if ((!this->integrationDataFile) || (!this->integrationDataFile.good())) {
      string msg("BehaviourDSLCommon::checkSrcOutputFile : ");
      msg += "ouput file is not valid";
      throw(runtime_error(msg));
    }
  }

  void BehaviourDSLCommon::writeSrcFileHeader() {
    this->checkSrcFile();
    this->srcFile << "/*!\n"
                  << "* \\file   " << this->srcFileName << '\n'
                  << "* \\brief  "
                  << "this file implements the " << this->mb.getClassName()
                  << " Behaviour.\n"
                  << "*         File generated by "
                  << MFrontHeader::getVersionName() << " "
                  << "version " << MFrontHeader::getVersionNumber() << '\n';
    if (!this->authorName.empty()) {
      this->srcFile << "* \\author " << this->authorName << '\n';
    }
    if (!this->date.empty()) {
      this->srcFile << "* \\date   " << this->date << '\n';
    }
    this->srcFile << " */\n\n";
    if (this->mb.hasParameters()) {
      this->srcFile << "#include<string>\n"
                    << "#include<cstring>\n"
                    << "#include<sstream>\n"
                    << "#include<fstream>\n"
                    << "#include<stdexcept>\n\n";
    }
    this->srcFile << "#include\"" << this->behaviourDataFileName << "\"\n"
                  << "#include\"" << this->integrationDataFileName << "\"\n"
                  << "#include\"" << this->behaviourFileName << "\"\n\n";
  }  // end of BehaviourDSLCommon::writeSrcFileHeader()

  void BehaviourDSLCommon::writeSrcFileStaticVariables(const Hypothesis h) {
    const auto& md = this->mb.getBehaviourData(h);
    const auto m = "tfel::material::ModellingHypothesis::" +
                   ModellingHypothesis::toUpperCaseString(h);
    this->checkSrcFile();
    for (const auto& v : md.getStaticVariables()) {
      if (v.type == "int") {
        continue;
      }
      if (this->mb.useQt()) {
        this->srcFile << "template<>\n";
        this->srcFile << "const " << this->mb.getClassName() << "<" << m
                      << ",float,true>::" << v.type << '\n'
                      << this->mb.getClassName() << "<" << m
                      << ",float,true>::" << v.name << " = "
                      << this->mb.getClassName() << "<" << m
                      << ",float,true>::" << v.type << "(static_cast<float>("
                      << v.value << "));\n\n";
      }
      this->srcFile << "template<>\n";
      this->srcFile << "const " << this->mb.getClassName() << "<" << m
                    << ",float,false>::" << v.type << '\n'
                    << this->mb.getClassName() << "<" << m
                    << ",float,false>::" << v.name << " = "
                    << this->mb.getClassName() << "<" << m
                    << ",float,false>::" << v.type << "(static_cast<float>("
                    << v.value << "));\n\n";
      if (this->mb.useQt()) {
        this->srcFile << "template<>\n";
        this->srcFile << "const " << this->mb.getClassName() << "<" << m
                      << ",double,true>::" << v.type << '\n'
                      << this->mb.getClassName() << "<" << m
                      << ",double,true>::" << v.name << " = "
                      << this->mb.getClassName() << "<" << m
                      << ",double,true>::" << v.type << "(static_cast<double>("
                      << v.value << "));\n\n";
      }
      this->srcFile << "template<>\n";
      this->srcFile << "const " << this->mb.getClassName() << "<" << m
                    << ",double,false>::" << v.type << '\n'
                    << this->mb.getClassName() << "<" << m
                    << ",double,false>::" << v.name << " = "
                    << this->mb.getClassName() << "<" << m
                    << ",double,false>::" << v.type << "(static_cast<double>("
                    << v.value << "));\n\n";
      if (this->mb.useQt()) {
        this->srcFile << "template<>\n";
        this->srcFile << "const " << this->mb.getClassName() << "<" << m
                      << ",long double,true>::" << v.type << '\n'
                      << this->mb.getClassName() << "<" << m
                      << ",long double,true>::" << v.name << " = "
                      << this->mb.getClassName() << "<" << m
                      << ",long double,true>::" << v.type
                      << "(static_cast<long double>(" << v.value << "));\n\n";
      }
      this->srcFile << "template<>\n";
      this->srcFile << "const " << this->mb.getClassName() << "<" << m
                    << ",long double,false>::" << v.type << '\n'
                    << this->mb.getClassName() << "<" << m
                    << ",long double,false>::" << v.name << " = "
                    << this->mb.getClassName() << "<" << m
                    << ",long double,false>::" << v.type
                    << "(static_cast<long double>(" << v.value << "));\n\n";
    }
  }  // end of BehaviourDSLCommon::writeSrcFileStaticVariables

  void BehaviourDSLCommon::writeSrcFileUserDefinedCode() {
    const auto& s = this->mb.getSources();
    this->checkBehaviourFile();
    if (!s.empty()) {
      this->srcFile << s << "\n\n";
    }
  }  // end of BehaviourDSLCommon::writeSrcFileUserDefinedCode

  void BehaviourDSLCommon::writeSrcFileParametersInitializers() {
    if (!this->mb.hasParameters()) {
      return;
    }
    auto hs = this->mb.getDistinctModellingHypotheses();
    hs.insert(ModellingHypothesis::UNDEFINEDHYPOTHESIS);
    for (const auto& h : hs) {
      if (this->mb.hasParameters(h)) {
        this->writeSrcFileParametersInitializer(h);
      }
    }
  }  // end of BehaviourDSLCommon::writeSrcFileParametersInitializer

  static void BehaviourDSLCommon_writeConverter(std::ostream& f,
                                                const std::string& cname,
                                                const std::string& type,
                                                const std::string& type2) {
    f << type << '\n'
      << cname << "::get" << type2 << "(const std::string& n,\n"
      << "const std::string& v)\n"
      << "{\n"
      << "using namespace std;\n"
      << type << " value;\n"
      << "istringstream converter(v);\n"
      << "converter >> value;\n"
      << "if(!converter||(!converter.eof())){\n"
      << "string msg(\"" << cname << "::get" << type2 << " : \"\n"
      << "           \"can't convert '\"+v+\"' to " << type
      << " for parameter '\"+ n+\"'\");\n"
      << "throw(runtime_error(msg));\n"
      << "}\n"
      << "return value;\n"
      << "}\n\n";
  }

  void BehaviourDSLCommon::writeSrcFileParametersInitializer(
      const Hypothesis h) {
    using namespace std;
    this->checkBehaviourFile();
    // treating the default case
    bool rp = false;   // real    parameter found
    bool ip = false;   // integer parameter found
    bool up = false;   // unsigned short parameter found
    bool rp2 = false;  // real    parameter found
    bool ip2 = false;  // integer parameter found
    bool up2 = false;  // unsigned short parameter found
    VariableDescriptionContainer::const_iterator p;
    string cname(this->mb.getClassName());
    if (h != ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      cname += ModellingHypothesis::toString(h);
    }
    cname += "ParametersInitializer";
    string dcname(this->mb.getClassName() + "ParametersInitializer");
    this->srcFile << cname << "&\n"
                  << cname << "::get()\n"
                  << "{\n"
                  << "static " << cname << " i;\n"
                  << "return i;\n"
                  << "}\n\n";
    this->srcFile << cname << "::" << cname << "()\n"
                  << "{\n";
    const VariableDescriptionContainer& params =
        this->mb.getBehaviourData(h).getParameters();
    for (p = params.begin(); p != params.end(); ++p) {
      if (p->type == "real") {
        rp = true;
        if ((h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) ||
            ((h != ModellingHypothesis::UNDEFINEDHYPOTHESIS) &&
             (!this->mb.hasParameter(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                                     p->name)))) {
          rp2 = true;
          const auto prec = this->srcFile.precision();
          this->srcFile.precision(14);
          if (p->arraySize == 1u) {
            this->srcFile << "this->" << p->name << " = "
                          << this->mb.getFloattingPointParameterDefaultValue(
                                 h, p->name)
                          << ";\n";
          } else {
            for (unsigned short i = 0; i != p->arraySize; ++i) {
              this->srcFile << "this->" << p->name << "[" << i << "] = "
                            << this->mb.getFloattingPointParameterDefaultValue(
                                   h, p->name, i)
                            << ";\n";
            }
          }
          this->srcFile.precision(prec);
        }
      } else if (p->type == "int") {
        ip = true;
        if ((h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) ||
            ((h != ModellingHypothesis::UNDEFINEDHYPOTHESIS) &&
             (!this->mb.hasParameter(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                                     p->name)))) {
          ip2 = true;
          this->srcFile << "this->" << p->name << " = "
                        << this->mb.getIntegerParameterDefaultValue(h, p->name)
                        << ";\n";
        }
      } else if (p->type == "ushort") {
        up = true;
        if ((h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) ||
            ((h != ModellingHypothesis::UNDEFINEDHYPOTHESIS) &&
             (!this->mb.hasParameter(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                                     p->name)))) {
          up2 = true;
          this->srcFile << "this->" << p->name << " = "
                        << this->mb.getUnsignedShortParameterDefaultValue(
                               h, p->name)
                        << ";\n";
        }
      }
    }
    this->srcFile << "// Reading parameters from a file\n";
    this->srcFile << cname << "::readParameters(*this,\""
                  << this->mb.getClassName() << "-parameters.txt\");\n";
    if (h != ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      this->srcFile << cname << "::readParameters(*this,\""
                    << this->mb.getClassName()
                    << ModellingHypothesis::toString(h)
                    << "-parameters.txt\");\n";
    }
    this->srcFile << "}\n\n";
    auto write_if = [](std::ostream& os, bool& b) {
      if (b) {
        os << "if(";
        b = false;
      } else {
        os << "} else if(";
      }
    };
    if (rp) {
      this->srcFile << "void\n"
                    << cname << "::set(const char* const key,\nconst double v)"
                    << "{\n"
                    << "using namespace std;\n";
      bool first = true;
      for (p = params.begin(); p != params.end(); ++p) {
        if (p->type == "real") {
          if (p->arraySize == 1u) {
            write_if(this->srcFile, first);
            this->srcFile << "::strcmp(\"" +
                                 this->mb.getExternalName(h, p->name) +
                                 "\",key)==0){\n";
            if ((h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) ||
                ((h != ModellingHypothesis::UNDEFINEDHYPOTHESIS) &&
                 (!this->mb.hasParameter(
                     ModellingHypothesis::UNDEFINEDHYPOTHESIS, p->name)))) {
              this->srcFile << "this->" << p->name << " = v;\n";
            } else {
              this->srcFile << dcname << "::get().set(\""
                            << this->mb.getExternalName(h, p->name)
                            << "\",v);\n";
            }
          } else {
            for (unsigned short i = 0; i != p->arraySize; ++i) {
              write_if(this->srcFile, first);
              const auto vn = p->name + '[' + to_string(i) + ']';
              const auto en = this->mb.getExternalName(h, p->name) + '[' +
                              to_string(i) + ']';
              this->srcFile << "::strcmp(\"" + en + "\",key)==0){\n";
              if ((h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) ||
                  ((h != ModellingHypothesis::UNDEFINEDHYPOTHESIS) &&
                   (!this->mb.hasParameter(
                       ModellingHypothesis::UNDEFINEDHYPOTHESIS, p->name)))) {
                this->srcFile << "this->" << vn << " = v;\n";
              } else {
                this->srcFile << dcname << "::get().set(\"" << en << "\",v);\n";
              }
            }
          }
        }
      }
      this->srcFile << "} else {\n";
      this->srcFile << "string msg(\"" << cname << "::set : \");\n"
                    << "msg += \" no parameter named '\";\n"
                    << "msg += key;\n"
                    << "msg += \"'\";\n"
                    << "throw(runtime_error(msg));\n"
                    << "}\n"
                    << "}\n\n";
    }
    if (ip) {
      this->srcFile << "void\n"
                    << cname << "::set(const char* const key,\nconst int v)"
                    << "{\n"
                    << "using namespace std;\n";
      bool first = true;
      for (p = params.begin(); p != params.end(); ++p) {
        if (p->type == "int") {
          write_if(this->srcFile, first);
          this->srcFile << "::strcmp(\"" +
                               this->mb.getExternalName(h, p->name) +
                               "\",key)==0){\n";
          if ((h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) ||
              ((h != ModellingHypothesis::UNDEFINEDHYPOTHESIS) &&
               (!this->mb.hasParameter(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                                       p->name)))) {
            this->srcFile << "this->" << p->name << " = v;\n";
          } else {
            this->srcFile << dcname << "::get().set(\""
                          << this->mb.getExternalName(h, p->name) << "\",v);\n";
          }
        }
      }
      this->srcFile << "} else {\n";
      this->srcFile << "string msg(\"" << cname << "::set : \");\n"
                    << "msg += \" no parameter named '\";\n"
                    << "msg += key;\n"
                    << "msg += \"'\";\n"
                    << "throw(runtime_error(msg));\n"
                    << "}\n"
                    << "}\n\n";
    }
    if (up) {
      this->srcFile << "void\n"
                    << cname
                    << "::set(const char* const key,\nconst unsigned short v)"
                    << "{\n"
                    << "using namespace std;\n";
      bool first = true;
      for (p = params.begin(); p != params.end(); ++p) {
        if (p->type == "ushort") {
          write_if(this->srcFile, first);
          this->srcFile << "::strcmp(\"" +
                               this->mb.getExternalName(h, p->name) +
                               "\",key)==0){\n";
          if ((h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) ||
              ((h != ModellingHypothesis::UNDEFINEDHYPOTHESIS) &&
               (!this->mb.hasParameter(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                                       p->name)))) {
            this->srcFile << "this->" << p->name << " = v;\n";
          } else {
            this->srcFile << dcname << "::get().set(\""
                          << this->mb.getExternalName(h, p->name) << "\",v);\n";
          }
        }
      }
      this->srcFile << "} else {\n";
      this->srcFile << "string msg(\"" << cname << "::set : \");\n"
                    << "msg += \" no parameter named '\";\n"
                    << "msg += key;\n"
                    << "msg += \"'\";\n"
                    << "throw(runtime_error(msg));\n"
                    << "}\n"
                    << "}\n\n";
    }
    if (rp2) {
      BehaviourDSLCommon_writeConverter(this->srcFile, cname, "double",
                                        "Double");
    }
    if (ip2) {
      BehaviourDSLCommon_writeConverter(this->srcFile, cname, "int", "Int");
    }
    if (up2) {
      BehaviourDSLCommon_writeConverter(this->srcFile, cname, "unsigned short",
                                        "UnsignedShort");
    }
    this->srcFile << "void\n" << cname << "::readParameters(" << cname << "&";
    if (rp2 || ip2 || up2) {
      this->srcFile << " pi";
    }
    this->srcFile
        << ",const char* const fn)"
        << "{\n"
        << "auto tokenize = [](const std::string& line){\n"
        << "std::istringstream tokenizer(line);\n"
        << "std::vector<std::string> tokens;\n"
        << "std::copy(std::istream_iterator<std::string>(tokenizer),\n"
        << "std::istream_iterator<std::string>(),\n"
        << "std::back_inserter(tokens));\n"
        << "return tokens;\n"
        << "};\n"
        << "std::ifstream f(fn);\n"
        << "if(!f){\n"
        << "return;\n"
        << "}\n"
        << "size_t ln = 1u;\n"
        << "while(!f.eof()){\n"
        << "auto line = std::string{};\n"
        << "std::getline(f,line);\n"
        << "auto tokens = tokenize(line);\n"
        << "auto throw_if = [ln,line,fn](const bool c,const std::string& m){\n"
        << "if(c){throw(std::runtime_error(\"" << cname
        << "::readParameters: \"\n"
        << "\"error at line '\"+std::to_string(ln)+\"' \"\n"
        << "\"while reading parameter file '\"+std::string(fn)+\"'\"\n"
        << "\"(\"+m+\")\"));}\n"
        << "};\n"
        << "if(tokens.empty()){\n"
        << "continue;\n"
        << "}\n"
        << "if(tokens[0][0]=='#'){\n"
        << "continue;\n"
        << "}\n"
        << "throw_if(tokens.size()!=2u,\"invalid number of tokens\");\n";
    bool first = true;
    for (const auto& v : params) {
      const auto b = ((h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) ||
                      ((h != ModellingHypothesis::UNDEFINEDHYPOTHESIS) &&
                       (!this->mb.hasParameter(
                           ModellingHypothesis::UNDEFINEDHYPOTHESIS, v.name))));
      auto write = [&v, &write_if, &b, &dcname, &cname](std::ostream& os,
                                                        const std::string& vn,
                                                        const std::string& en) {
        os << "\"" << en << "\"==tokens[0]){\n";
        if (b) {
          os << "pi." << vn << " = ";
          if (v.type == "real") {
            os << cname << "::getDouble(tokens[0],tokens[1]);\n";
          } else if (v.type == "int") {
            os << cname << "::getInt(tokens[0],tokens[1]);\n";
          } else if (v.type == "ushort") {
            os << cname << "::getUnsignedShort(tokens[0],tokens[1]);\n";
          } else {
            throw(std::runtime_error(
                "BehaviourDSLCommon::writeSrcFileParametersInitializer: "
                "invalid parameter type '" +
                v.type + "'"));
          }
        } else {
          os << dcname << "::get().set(\"" << en << "\",\n";
          if (v.type == "real") {
            os << dcname << "::getDouble(tokens[0],tokens[1])";
          } else if (v.type == "int") {
            os << dcname << "::getInt(tokens[0],tokens[1])";
          } else if (v.type == "ushort") {
            os << dcname << "::getUnsignedShort(tokens[0],tokens[1])";
          } else {
            throw(std::runtime_error(
                "BehaviourDSLCommon::writeSrcFileParametersInitializer: "
                "invalid parameter type '" +
                v.type + "'"));
          }
          os << ");\n";
        }
      };
      if (v.arraySize == 1u) {
        write_if(this->srcFile, first);
        write(this->srcFile, v.name, this->mb.getExternalName(h, v.name));
      } else {
        for (unsigned short i = 0; i != v.arraySize; ++i) {
          const auto vn = v.name + '[' + to_string(i) + ']';
          const auto en =
              this->mb.getExternalName(h, v.name) + '[' + to_string(i) + ']';
          write_if(this->srcFile, first);
          write(this->srcFile, vn, en);
        }
      }
    }
    this->srcFile << "} else {\n"
                  << "throw_if(true,\"invalid parameter '\"+tokens[0]+\"'\");\n"
                  << "}\n"
                  << "}\n"
                  << "}\n\n";
  }  // end of BehaviourDSLCommon::writeSrcFileParametersInitializer

  void BehaviourDSLCommon::writeSrcFileBehaviourProfiler() {
    if (this->mb.getAttribute(BehaviourData::profiling, false)) {
      this->checkSrcFile();
      this->srcFile << "mfront::BehaviourProfiler&\n"
                    << this->mb.getClassName() << "Profiler::getProfiler()\n"
                    << "{\n"
                    << "static mfront::BehaviourProfiler profiler(\""
                    << this->mb.getClassName() << "\");\n;"
                    << "return profiler;\n"
                    << "}\n\n";
    }
  }  // end of BehaviourDSLCommon::writeSrcFileBehaviourProfiler

  void BehaviourDSLCommon::writeSrcFile() {
    this->writeSrcFileHeader();
    this->writeSrcFileUserDefinedCode();
    this->writeNamespaceBegin(this->srcFile);
    this->writeSrcFileBehaviourProfiler();
    this->writeSrcFileParametersInitializers();
    // modelling hypotheses handled by the behaviour
    const auto& mh = this->mb.getModellingHypotheses();
    for (const auto& h : mh) {
      this->writeSrcFileStaticVariables(h);
    }
    this->writeNamespaceEnd(this->srcFile);
  }  // end of BehaviourDSLCommon::writeSrcFile(void)

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
  }  // end of BehaviourDSLCommon::predictionOperatorVariableModifier

  void BehaviourDSLCommon::treatProfiling() {
    const auto b = this->readBooleanValue("BehaviourDSLCommon::treatProfiling");
    this->readSpecifiedToken("BehaviourDSLCommon::treatProfiling", ";");
    this->mb.setAttribute(BehaviourData::profiling, b, false);
  }  // end of BehaviourDSLCommon::treatProfiling

  void BehaviourDSLCommon::treatPredictionOperator() {
    using namespace std;
    using namespace tfel::material;
    using namespace tfel::utilities;
    CodeBlockOptions o;
    this->readCodeBlockOptions(o, true);
    if (this->mb.getBehaviourType() ==
        BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR) {
      bool found = false;
      if (o.untreated.size() != 1u) {
        ostringstream msg;
        msg << "BehaviourDSLCommon::treatPredictionOperator : "
            << "tangent operator type is undefined. Valid tanget operator type "
               "are :\n";
        for (const auto& to : getFiniteStrainBehaviourTangentOperatorFlags()) {
          msg << "- "
              << convertFiniteStrainBehaviourTangentOperatorFlagToString(to)
              << " : " << getFiniteStrainBehaviourTangentOperatorDescription(to)
              << '\n';
        }
        throw(runtime_error(msg.str()));
      }
      if (o.untreated[0].flag != Token::Standard) {
        throw(
            runtime_error("BehaviourDSLCommon::treatPredictionOperator : "
                          "invalid option '" +
                          o.untreated[0].value + "'"));
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
        ostringstream msg;
        msg << "BehaviourDSLCommon::treatPredictionOperator : "
            << "invalid tangent operator type '" + ktype +
                   "'. Valid tanget operator type are :\n";
        for (const auto& to : getFiniteStrainBehaviourTangentOperatorFlags()) {
          msg << "- "
              << convertFiniteStrainBehaviourTangentOperatorFlagToString(to)
              << " : " << getFiniteStrainBehaviourTangentOperatorDescription(to)
              << '\n';
        }
        throw(runtime_error(msg.str()));
      }
      this->readCodeBlock(
          *this, o, BehaviourData::ComputePredictionOperator + "-" + ktype,
          &BehaviourDSLCommon::predictionOperatorVariableModifier, true);
      for (const auto& h : o.hypotheses) {
        if (!this->mb.hasAttribute(h, BehaviourData::hasPredictionOperator)) {
          this->mb.setAttribute(h, BehaviourData::hasPredictionOperator, true);
        }
      }
    } else {
      this->treatUnsupportedCodeBlockOptions(o);
      this->readCodeBlock(
          *this, o, BehaviourData::ComputePredictionOperator,
          &BehaviourDSLCommon::predictionOperatorVariableModifier, true);
      for (const auto& h : o.hypotheses) {
        this->mb.setAttribute(h, BehaviourData::hasPredictionOperator, true);
      }
    }
  }  // end of BehaviourDSLCommon::treatPredictionOperator

  void BehaviourDSLCommon::treatParameter() {
    std::set<Hypothesis> mh;
    this->readHypothesesList(mh);
    bool endOfTreatment = false;
    while ((this->current != this->tokens.end()) && (!endOfTreatment)) {
      if (!isValidIdentifier(this->current->value)) {
        this->throwRuntimeError("BehaviourDSLCommon::treatParameter : ",
                                "variable given is not valid (read "
                                "'" +
                                    this->current->value + "').");
      }
      const auto n = this->current->value;
      const auto lineNumber = this->current->line;
      ++(this->current);
      this->checkNotEndOfFile("BehaviourDSLCommon::treatParameter");
      unsigned short arraySize = 1u;
      if (this->current->value == "[") {
        ++(this->current);
        this->checkNotEndOfFile("BehaviourDSLCommon::treatParameter");
        arraySize =
            this->readUnsignedShort("BehaviourDSLCommon::treatParameter");
        if (arraySize == 0u) {
          this->throwRuntimeError("BehaviourDSLCommon::treatParameter",
                                  "invalid array size");
        }
        this->checkNotEndOfFile("BehaviourDSLCommon::treatParameter");
        this->readSpecifiedToken("BehaviourDSLCommon::treatParameter", "]");
      }
      this->checkNotEndOfFile("BehaviourDSLCommon::treatParameter");
      if ((this->current->value == "=") || (this->current->value == "{") ||
          (this->current->value == "(")) {
        std::string ci;  // closing initializer
        if (this->current->value == "{") {
          ci = "}";
        }
        if (this->current->value == "(") {
          if (arraySize != 1u) {
            this->throwRuntimeError(
                "BehaviourDSLCommon::treatParameter",
                "invalid initalisation syntax for the default values "
                "of an array of parameters.\n"
                "Unexpected token '" +
                    current->value + "'");
          }
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
          if (r.size() != arraySize) {
            this->throwRuntimeError("BehaviourDSLCommon::treatParameter",
                                    "number of values given does not match the "
                                    "number of parameters "
                                    "(" +
                                        std::to_string(r.size()) + " vs +" +
                                        std::to_string(arraySize) + ").\n");
          }
          for (const auto& h : mh) {
            this->mb.addParameter(
                h, VariableDescription("real", n, arraySize, lineNumber));
            for (decltype(r.size()) i = 0; i != r.size(); ++i) {
              this->mb.setParameterDefaultValue(h, n, i, r[i]);
            }
          }
        } else {
          double value;
          std::istringstream converter(this->current->value);
          converter >> value;
          if (!converter || (!converter.eof())) {
            this->throwRuntimeError(
                "BehaviourDSLCommon::treatParameter",
                "could not read default value for parameter '" + n + "'");
          }
          ++(this->current);
          this->checkNotEndOfFile("BehaviourDSLCommon::treatParameter");
          if (!ci.empty()) {
            this->readSpecifiedToken("BehaviourDSLCommon::treatParameter", ci);
          }
          for (const auto& h : mh) {
            this->mb.addParameter(
                h, VariableDescription("real", n, 1u, lineNumber));
            this->mb.setParameterDefaultValue(h, n, value);
          }
        }
      } else {
        if (arraySize != 1) {
          this->throwRuntimeError("BehaviourDSLCommon::treatParameter",
                                  "default values of parameters array "
                                  "must be defined with the array. "
                                  "Unexpected token '" +
                                      current->value + "'");
        }
        for (const auto& h : mh) {
          this->mb.addParameter(h,
                                VariableDescription("real", n, 1u, lineNumber));
        }
      }
      if (this->current->value == ",") {
        ++(this->current);
      } else if (this->current->value == ";") {
        endOfTreatment = true;
        ++(this->current);
      } else {
        this->throwRuntimeError("BehaviourDSLCommon::treatParameter",
                                ", or ; expected after '" + n + "', read '" +
                                    this->current->value + "'");
      }
    }
    if (!endOfTreatment) {
      --(this->current);
      this->throwRuntimeError("BehaviourDSLCommon::treatParameter",
                              "Expected ';' before end of file");
    }
  }  // end of BehaviourDSLCommon::treatParameter

  void BehaviourDSLCommon::treatInitLocalVariables() {
    this->readCodeBlock(*this, BehaviourData::InitializeLocalVariables,
                        &BehaviourDSLCommon::standardModifier, true, true);
  }  // end of BehaviourDSLCommon:treatInitLocalVariables

  void BehaviourDSLCommon::treatMinimalTimeStepScalingFactor() {
    const Hypothesis h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    double r_dt;
    this->checkNotEndOfFile(
        "ImplicitDSLBase::treatMinimalTimeStepScalingFactor",
        "Cannot read value.");
    std::istringstream flux(current->value);
    flux >> r_dt;
    if ((flux.fail()) || (!flux.eof())) {
      this->throwRuntimeError(
          "ImplicitDSLBase::treatMinimalTimeStepScalingFactor",
          "Failed to read value.");
    }
    if (r_dt < 10 * std::numeric_limits<double>::min()) {
      this->throwRuntimeError(
          "ImplicitDSLBase::treatMinimalTimeStepScalingFactor",
          "minimal time step scaling factor either too "
          "low value or negative.");
    }
    ++(this->current);
    this->readSpecifiedToken(
        "ImplicitDSLBase::treatMinimalTimeStepScalingFactor", ";");
    VariableDescription e("real", "minimal_time_step_scaling_factor", 1u, 0u);
    e.description = "minimal value for the time step scaling factor";
    this->mb.addParameter(h, e, BehaviourData::ALREADYREGISTRED);
    this->mb.setParameterDefaultValue(h, "minimal_time_step_scaling_factor",
                                      r_dt);
    this->mb.setEntryName(h, "minimal_time_step_scaling_factor",
                          "minimal_time_step_scaling_factor");
  }  // end of BehaviourDSLCommon::treatMinimalTimeStepScalingFactor

  void BehaviourDSLCommon::treatMaximalTimeStepScalingFactor() {
    const Hypothesis h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    double r_dt;
    this->checkNotEndOfFile(
        "ImplicitDSLBase::treatMaximalTimeStepScalingFactor",
        "Cannot read value.");
    std::istringstream flux(current->value);
    flux >> r_dt;
    if ((flux.fail()) || (!flux.eof())) {
      this->throwRuntimeError(
          "ImplicitDSLBase::treatMaximalTimeStepScalingFactor",
          "Failed to read value.");
    }
    if (r_dt < 1) {
      this->throwRuntimeError(
          "ImplicitDSLBase::treatMaximalTimeStepScalingFactor",
          "maximal time step scaling factor value either too "
          "low or negative.");
    }
    ++(this->current);
    this->readSpecifiedToken(
        "ImplicitDSLBase::treatMaximalTimeStepScalingFactor", ";");
    VariableDescription e("real", "maximal_time_step_scaling_factor", 1u, 0u);
    e.description = "maximal value for the time step scaling factor";
    this->mb.addParameter(h, e, BehaviourData::ALREADYREGISTRED);
    this->mb.setParameterDefaultValue(h, "maximal_time_step_scaling_factor",
                                      r_dt);
    this->mb.setEntryName(h, "maximal_time_step_scaling_factor",
                          "maximal_time_step_scaling_factor");
  }  // end of BehaviourDSLCommon::treatMaximalTimeStepScalingFactor

  void BehaviourDSLCommon::disableVariableDeclaration(const Hypothesis h) {
    if (!this->mb.hasAttribute(h,
                               BehaviourData::allowsNewUserDefinedVariables)) {
      this->mb.setAttribute(h, BehaviourData::allowsNewUserDefinedVariables,
                            false);
    }
  }

  void BehaviourDSLCommon::setMinimalTangentOperator() {
    if (this->mb.getBehaviourType() !=
        BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR) {
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
            BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) {
          const Hypothesis h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
          // if the user provided a tangent operator, it won't be
          // overriden
          CodeBlock tangentOperator;
          std::ostringstream code;
          code << "if(smt==ELASTIC){\n"
               << "this->Dt = this->D;\n"
               << "} else {\n"
               << "return false;\n"
               << "}\n";
          tangentOperator.code = code.str();
          this->mb.setCode(h, BehaviourData::ComputeTangentOperator,
                           tangentOperator, BehaviourData::CREATEBUTDONTREPLACE,
                           BehaviourData::BODY);
          this->mb.setAttribute(h, BehaviourData::hasConsistentTangentOperator,
                                true, true);
        }
      }
    }
  }  // end of BehaviourDSLCommon::setMinimalTangentOperator

  void BehaviourDSLCommon::treatInternalEnergy() {
    this->readCodeBlock(*this, BehaviourData::ComputeInternalEnergy,
                        &BehaviourDSLCommon::standardModifier, true, true);
  }  // end of BehaviourDSLCommon::treatInternalEnergy

  void BehaviourDSLCommon::treatDissipatedEnergy() {
    this->readCodeBlock(*this, BehaviourData::ComputeDissipatedEnergy,
                        &BehaviourDSLCommon::standardModifier, true, true);
  }  // end of BehaviourDSLCommon::treatDissipatedEnergy

  void BehaviourDSLCommon::
      setComputeFinalStressFromComputeFinalStressCandidateIfNecessary() {
    // first treating specialised mechanical data
    for (const auto& h : this->mb.getDistinctModellingHypotheses()) {
      if (h != ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
        if (!this->mb.hasCode(h, BehaviourData::ComputeFinalStress)) {
          if (this->mb.hasCode(h, BehaviourData::ComputeFinalStressCandidate)) {
            this->mb.setCode(h, BehaviourData::ComputeFinalStress,
                             this->mb.getCodeBlock(
                                 h, BehaviourData::ComputeFinalStressCandidate),
                             BehaviourData::CREATE, BehaviourData::BODY);
          }
        }
      }
    }
    // now treating the default hypothesis case
    if (!this->mb.areAllMechanicalDataSpecialised()) {
      const Hypothesis h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      if (!this->mb.hasCode(h, BehaviourData::ComputeFinalStress)) {
        if (this->mb.hasCode(h, BehaviourData::ComputeFinalStressCandidate)) {
          this->mb.setCode(h, BehaviourData::ComputeFinalStress,
                           this->mb.getCodeBlock(
                               h, BehaviourData::ComputeFinalStressCandidate),
                           BehaviourData::CREATEBUTDONTREPLACE,
                           BehaviourData::BODY);
        }
      }
    }
  }  // end of
     // BehaviourDSLCommon::setComputeFinalStressFromComputeFinalStressCandidateIfNecessary

}  // end of namespace mfront
