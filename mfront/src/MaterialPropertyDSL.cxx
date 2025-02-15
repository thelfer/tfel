/*!
 * \file   mfront/src/MaterialPropertyDSL.cxx
 * \brief
 * \author Thomas Helfer
 * \date   10 Nov 2006
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <string>
#include <cctype>
#include <sstream>
#include <cstdlib>
#include <iterator>
#include <algorithm>
#include <stdexcept>

#include "TFEL/Raise.hxx"
#include "TFEL/Config/GetInstallPath.hxx"
#include "TFEL/Utilities/Data.hxx"
#include "TFEL/Utilities/Token.hxx"
#include "TFEL/System/System.hxx"
#include "TFEL/Glossary/Glossary.hxx"
#include "TFEL/Glossary/GlossaryEntry.hxx"
#include "TFEL/UnicodeSupport/UnicodeSupport.hxx"

#include "MFront/MFrontHeader.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MFront/DSLUtilities.hxx"
#include "MFront/MFrontUtilities.hxx"
#include "MFront/PedanticMode.hxx"
#include "MFront/MFrontDebugMode.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MFront/DSLFactory.hxx"
#include "MFront/MaterialPropertyDSL.hxx"
#include "MFront/TargetsDescription.hxx"
#include "MFront/DataInterpolationUtilities.hxx"
#include "MFront/MaterialPropertyInterfaceFactory.hxx"

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

  bool isValidMaterialPropertyName(const std::string& n) {
    return tfel::utilities::CxxTokenizer::isValidIdentifier(n, false);
  }

  MaterialPropertyDSL::MaterialPropertyDSL(const DSLOptions& opts)
      : DSLBase(opts) {
    //
    DSLBase::handleDSLOptions(this->md, opts);
    const auto oparameters =
        tfel::utilities::convert<std::map<std::string, double>>(
            tfel::utilities::get_if<tfel::utilities::DataMap>(
                opts, DSLBase::overridingParameters,
                tfel::utilities::DataMap{}));
    for (const auto& op : oparameters) {
      this->overrideByAParameter(op.first, op.second);
    }
    // Call Back
    this->registerNewCallBack(";", &MaterialPropertyDSL::treatLonelySeparator);
    this->registerNewCallBack("@UseQt", &MaterialPropertyDSL::treatUseQt);
    this->registerNewCallBack("@Link", &MaterialPropertyDSL::treatLink);
    this->registerNewCallBack("@TFELLibraries",
                              &MaterialPropertyDSL::treatTFELLibraries);
    this->registerNewCallBack("@MaterialLaw",
                              &MaterialPropertyDSL::treatMaterialLaw);
    this->registerNewCallBack("@MFront", &MaterialPropertyDSL::treatMFront);
    this->registerNewCallBack("@Material", &MaterialPropertyDSL::treatMaterial);
    this->registerNewCallBack("@Library", &MaterialPropertyDSL::treatLibrary);
    this->registerNewCallBack("@DSL", &MaterialPropertyDSL::treatDSL);
    this->registerNewCallBack("@Parser", &MaterialPropertyDSL::treatDSL);
    this->registerNewCallBack("@Law", &MaterialPropertyDSL::treatLaw);
    this->registerNewCallBack("@Author", &MaterialPropertyDSL::treatAuthor);
    this->registerNewCallBack("@Date", &MaterialPropertyDSL::treatDate);
    this->registerNewCallBack("@UnitSystem",
                              &MaterialPropertyDSL::treatUnitSystem);
    this->registerNewCallBack("@Includes", &MaterialPropertyDSL::treatIncludes);
    this->registerNewCallBack("@StaticVar",
                              &MaterialPropertyDSL::treatStaticVar);
    this->registerNewCallBack("@StaticVariable",
                              &MaterialPropertyDSL::treatStaticVar);
    this->registerNewCallBack("@Parameter",
                              &MaterialPropertyDSL::treatParameter);
    this->registerNewCallBack("@Constant", &MaterialPropertyDSL::treatConstant);
    this->registerNewCallBack("@Description",
                              &MaterialPropertyDSL::treatDescription);
    this->registerNewCallBack("@Input", &MaterialPropertyDSL::treatInput);
    this->registerNewCallBack("@StateVariable",
                              &MaterialPropertyDSL::treatInput);
    this->registerNewCallBack("@Output", &MaterialPropertyDSL::treatOutput);
    this->registerNewCallBack("@Function", &MaterialPropertyDSL::treatFunction);
    this->registerNewCallBack("@Data", &MaterialPropertyDSL::treatData);
    this->registerNewCallBack("@Import", &MaterialPropertyDSL::treatImport);
    this->registerNewCallBack("@Interface",
                              &MaterialPropertyDSL::treatInterface);
    this->registerNewCallBack("@Bounds", &MaterialPropertyDSL::treatBounds);
    this->registerNewCallBack("@PhysicalBounds",
                              &MaterialPropertyDSL::treatPhysicalBounds);
    // reverse names
    for (const auto& v : DSLBase::getDefaultReservedNames()) {
      this->reserveName(v);
    }
    this->reserveName("params");
  }  // end of MaterialPropertyDSL()

  AbstractDSL::DSLOptions MaterialPropertyDSL::buildDSLOptions() const {
    return DSLBase::buildCommonDSLOptions(this->md);
  }  // end of buildDSLOptions

  AbstractDSL::DSLTarget MaterialPropertyDSL::getTargetType() const {
    return MATERIALPROPERTYDSL;
  }

  std::string MaterialPropertyDSL::getMaterialKnowledgeIdentifier() const {
    return this->md.law;
  }  // end of getMaterialKnowledgeIdentifier

  std::string MaterialPropertyDSL::getMaterialName() const {
    return this->md.material;
  }  // end of getMaterialName(

  std::string MaterialPropertyDSL::getOverridableVariableNameByExternalName(
      const std::string& en) const {
    const auto pi = findByExternalName(this->md.inputs, en);
    if (pi != this->md.inputs.end()) {
      return pi->name;
    }
    const auto pp = findByExternalName(this->md.parameters, en);
    if (pp == this->md.parameters.end()) {
      tfel::raise(
          "MaterialPropertyDSL::getOverridableVariableNameByExternalName: "
          "no overridable variable associated with external name '" +
          en + "'");
    }
    return pp->name;
  }  // end of getOverridableVariableNameByExternalName

  void MaterialPropertyDSL::overrideByAParameter(const std::string& n,
                                                 const double v) {
    tfel::raise_if(!this->md.f.body.empty(),
                   "MaterialPropertyDSL::overrideByAParameter: "
                   "overriding parameters can't be declared at "
                   "this stage (function is already defined)");
    this->overriding_parameters[n] = v;
  }  // end of overrideByAParameter

  std::map<std::string, double> MaterialPropertyDSL::getOverridenParameters()
      const {
    return this->overriding_parameters;
  }  // end of getOverridenParameters

  /*!
   * \brief various checks on variables
   * \param[in] v  : variables
   */
  static void performPedanticChecks(const VariableDescriptionContainer& vc,
                                    const std::set<std::string>& m,
                                    const std::string& t) {
    auto& log = getLogStream();
    auto& glossary = tfel::glossary::Glossary::getGlossary();
    // boucle sur md.inputs
    for (const auto& v : vc) {
      if (m.find(v.name) == m.end()) {
        log << "- " << t << " '" << v.name << "' is unused.\n";
      } else {
        if ((!v.hasGlossaryName()) && (!v.hasEntryName())) {
          log << "- " << t << " '" << v.name << "' has no external name.\n";
        }
        if (!v.hasPhysicalBounds()) {
          log << "- " << t << " '" << v.name << "' has no physical bounds.\n";
        }
        if (!v.hasBounds()) {
          log << "- " << t << " '" << v.name << "' has no bounds.\n";
        }
        if (v.description.empty()) {
          auto hasDoc = false;
          if (v.hasGlossaryName()) {
            const auto& e = glossary.getGlossaryEntry(v.getExternalName());
            hasDoc = (!e.getShortDescription().empty()) ||
                     (!e.getDescription().empty());
          }
          if (!hasDoc) {
            log << "- " << t << " '" << v.name << "' has no description.\n";
          }
        }
      }
    }
  }  // end of performPedanticChecks

  /*!
   * \brief various checks on variables
   * \param[in] v  : variables
   */
  static void performPedanticChecks(const VariableDescription& vd,
                                    const std::string& t) {
    auto& log = getLogStream();
    auto& glossary = tfel::glossary::Glossary::getGlossary();
    if ((!vd.hasGlossaryName()) && (!vd.hasEntryName())) {
      log << "- " << t << " '" << vd.name << "' has no external name.\n";
    }
    if (!vd.hasPhysicalBounds()) {
      log << "- " << t << " '" << vd.name << "' has no physical bounds.\n";
    }
    if (!vd.hasBounds()) {
      log << "- " << t << " '" << vd.name << "' has no bounds.\n";
    }
    if (vd.description.empty()) {
      auto hasDoc = false;
      if (vd.hasGlossaryName()) {
        const auto& e = glossary.getGlossaryEntry(vd.getExternalName());
        hasDoc =
            (!e.getShortDescription().empty()) || (!e.getDescription().empty());
      }
      if (!hasDoc) {
        log << "- " << t << " '" << vd.name << "' has no description.\n";
      }
    }
  }  // end of performPedanticChecks

  void MaterialPropertyDSL::doPedanticChecks() const {
    auto& log = getLogStream();
    log << "\n* Pedantic checks of " << this->fd.fileName << "\n\n";
    performPedanticChecks(this->fd);
    performPedanticChecks(this->md.inputs, this->md.f.used_inputs, "input");
    performPedanticChecks(this->md.parameters, md.f.used_parameters,
                          "parameter");
    performPedanticChecks(this->md.output, "output");
    log << "\n# End of pedantic checks\n";
  }  // end of doPedanticChecks

  void MaterialPropertyDSL::endsInputFileProcessing() {
  }  // end of endsInputFileProcessing

  void MaterialPropertyDSL::makeConsistencyChecks() const {
    if (getPedanticMode()) {
      this->doPedanticChecks();
    }
  }  // end of makeConsistencyChecks

  void MaterialPropertyDSL::registerNewCallBack(const std::string& keyword,
                                                const MemberFuncPtr ptr) {
    this->callBacks.insert({keyword, ptr});
    this->registredKeyWords.insert(keyword);
  }  // end of registerNewCall

  void MaterialPropertyDSL::getKeywordsList(std::vector<std::string>& k) const {
    for (const auto& c : this->callBacks) {
      k.push_back(c.first);
    }
  }

  std::string MaterialPropertyDSL::getClassName() const {
    return this->md.className;
  }  // end of getClassName

  void MaterialPropertyDSL::addStaticVariableDescription(
      const StaticVariableDescription& v) {
    this->reserveName(v.name);
    this->md.staticVars.push_back(v);
  }  // end of addStaticVariableDescription

  std::map<std::string, int> MaterialPropertyDSL::getIntegerConstants() const {
    auto r = std::map<std::string, int>{};
    for (const auto& v : this->md.staticVars) {
      if (v.type == "int") {
        r.insert({v.name, v.value});
      }
    }
    return r;
  }  // end of getIntegerConstants

  int MaterialPropertyDSL::getIntegerConstant(const std::string& n) const {
    for (const auto& v : this->md.staticVars) {
      if (v.name == n) {
        if (v.type != "int") {
          this->throwRuntimeError("MaterialPropertyDSL::getIntegerConstant",
                                  "invalid type for variable '" + n + "'");
        }
        return v.value;
      }
    }
    this->throwRuntimeError("MaterialPropertyDSL::getIntegerConstant",
                            "unknown variable '" + n + "'");
  }  // end of getIntegerConstant

  std::string MaterialPropertyDSL::getDescription() {
    using MLIF = MaterialPropertyInterfaceFactory;
    auto& mlif = MLIF::getMaterialPropertyInterfaceFactory();
    const auto& ai = mlif.getRegistredInterfaces();
    auto p2 = ai.cbegin();
    const auto p2e = ai.cend();
    std::string msg("this parser is used to define material properties. ");
    msg += "Available interfaces are ";
    while (p2 != p2e) {
      msg += "'" + *p2 + "'";
      if (++p2 != p2e) {
        msg += ",";
      } else {
        msg += " and ";
      }
    }
    return msg;
  }  // end of getDescription

  std::string MaterialPropertyDSL::getName() {
    return "MaterialLaw";
  }  // end of getName

  void MaterialPropertyDSL::setUnitSystem(const std::string_view s) {
    this->md.setUnitSystem(s);
  }  // end of setUnitSystem

  void MaterialPropertyDSL::setMaterial(const std::string& m) {
    if (!this->md.material.empty()) {
      this->throwRuntimeError("MaterialPropertyDSL::setMaterial",
                              "material name alreay defined");
    }
    if (!isValidMaterialName(m)) {
      this->throwRuntimeError("MaterialPropertyDSL::setMaterial",
                              "invalid material name '" + m + "'");
    }
    this->md.material = m;
  }  // end of setMaterial

  void MaterialPropertyDSL::treatUseQt() {
    this->checkNotEndOfFile("MaterialPropertyDSL::treatUseQt",
                            "Expected 'true' or 'false'.");
    if (this->md.use_qt.has_value()) {
      this->throwRuntimeError("MaterialPropertyDSL::treatUseQt",
                              "quantity usage has already been specified");
    }
    this->md.use_qt = this->readBooleanValue("MaterialPropertyDSL::treatUseQt");
    this->readSpecifiedToken("MaterialPropertyDSL::treatUseQt", ";");
  }  // end of treatUseQt

  void MaterialPropertyDSL::treatLibrary() {
    if (!this->md.library.empty()) {
      this->throwRuntimeError("MaterialPropertyDSL::treatLibrary",
                              "material name alreay defined");
    }
    const auto& l = this->readOnlyOneToken();
    if (!isValidLibraryName(l)) {
      this->throwRuntimeError("MaterialPropertyDSL::treatLibrary",
                              "invalid library name '" + l + "'");
    }
    this->md.library = l;
  }  // end of MFrontLibraryLawParser::treatLibrary

  void MaterialPropertyDSL::treatConstant() {
    this->checkNotEndOfFile("MaterialPropertyDSL::treatConstant",
                            "Cannot read variable name.");
    const auto name = this->current->value;
    if (!isValidIdentifier(name)) {
      this->throwRuntimeError("DSLBase::treatConstant",
                              "constant name '" + name + "' is not valid.");
    }
    const auto line = this->current->line;
    ++(this->current);
    const auto value = this->readInitialisationValue<long double>(name, true);
    this->readSpecifiedToken("MaterialPropertyDSL::treatConstant", ";");
    this->addStaticVariableDescription(
        StaticVariableDescription("real", name, line, value.second));
  }  // end of treatConstant

  void MaterialPropertyDSL::treatParameter() {
    this->checkNotEndOfFile("MaterialPropertyDSL::treatParameter",
                            "Expected parameter name.");
    tfel::raise_if(
        !this->md.f.body.empty(),
        "MaterialPropertyDSL::Parameter: parameters can't be declared at "
        "this stage (function is already defined)");
    const auto type = [this]() -> std::string {
      const auto otype = this->readVariableTypeIfPresent();
      if (!otype) {
        return "real";
      }
      return *otype;
    }();
    if (SupportedTypes::getTypeFlag(type) != SupportedTypes::SCALAR) {
      this->throwRuntimeError("DSLBase::treatParameter",
                              "parameter type'" + type + "' is not valid.");
    }
    //
    const auto sname = this->current->value;
    const auto name = tfel::unicode::getMangledString(sname);
    if (!this->isValidIdentifier(name)) {
      this->throwRuntimeError("DSLBase::treatParameter",
                              "parameter name '" + name + "' is not valid.");
    }
    ++(this->current);
    this->checkNotEndOfFile("MaterialPropertyDSL::treatParameter",
                            "Expected parameter name.");
    auto p = [this, type, sname, name] {
      if (sname != name) {
        return VariableDescription(type, sname, name, 1u, this->current->line);
      }
      return VariableDescription(type, name, 1u, this->current->line);
    }();
    const auto value = this->readInitialisationValue<double>(p.name, false);
    if (value.first) {
      p.setAttribute(VariableDescription::defaultValue, value.second, false);
    }
    this->readSpecifiedToken("MaterialPropertyDSL::treatParameter", ";");
    this->reserveName(p.name);
    this->md.parameters.push_back(p);
  }  // end of treatParameter

  void MaterialPropertyDSL::setMaterialKnowledgeIdentifier(
      const std::string& i) {
    auto throw_if = [this](const bool b, const std::string& m) {
      if (b) {
        this->throwRuntimeError(
            "MaterialPropertyDSL::setMaterialKnowledgeIdentifier", m);
      }
    };
    throw_if(!isValidMaterialPropertyName(i),
             "'" + i + "' is not a valid law name");
    throw_if(!this->md.className.empty(),
             "class name has already been declared");
    throw_if(!this->md.law.empty(), "law name has already been declared");
    this->md.law = i;
    this->md.className = i;
  }  // end of setMaterialKnowledgeIdentifier

  void MaterialPropertyDSL::treatLaw() {
    const auto& l = this->readOnlyOneToken();
    if (!isValidMaterialPropertyName(l)) {
      this->throwRuntimeError("MaterialPropertyDSL::treatLaw",
                              "'" + l + "' is not a valid law name");
    }
    if (this->overriden_implementation_name.empty()) {
      this->setMaterialKnowledgeIdentifier(l);
    }
  }  // end of treatLaw

  void MaterialPropertyDSL::addInterface(const std::string& i) {
    using MLIF = mfront::MaterialPropertyInterfaceFactory;
    if (this->interfaces.find(i) == this->interfaces.end()) {
      auto& mlif = MLIF::getMaterialPropertyInterfaceFactory();
      this->interfaces.insert({i, mlif.getInterface(i)});
    }
  }  // end of addInterface

  void MaterialPropertyDSL::setInterfaces(const std::set<std::string>& inames) {
    using MLIF = mfront::MaterialPropertyInterfaceFactory;
    auto& mlif = MLIF::getMaterialPropertyInterfaceFactory();
    // searching i2 depedencies
    for (const auto& i : inames) {
      for (const auto& d : mlif.getInterfaceDependencies(i)) {
        this->addInterface(d);
      }
      this->addInterface(i);
    }
  }  // end of setInterface

  void MaterialPropertyDSL::treatInterface() {
    this->checkNotEndOfFile("MaterialPropertyDSL::treatInterface",
                            "Expected interface name.");
    if (this->current->flag == tfel::utilities::Token::String) {
      this->addInterface(
          this->current->value.substr(1, this->current->value.size() - 2));
    } else {
      this->addInterface(this->current->value);
    }
    ++(this->current);
    this->readSpecifiedToken("MaterialPropertyDSL::treatInterface", ";");
  }  // end of treatInterface

  void MaterialPropertyDSL::finalizeVariablesDeclaration() {
    for (const auto& op : this->overriding_parameters) {
      auto match = [&op](const VariableDescription& v) {
        return (v.symbolic_form == op.first) || (v.name == op.first) ||
               (v.getExternalName() == op.first);
      };
      auto pi =
          std::find_if(this->md.inputs.begin(), this->md.inputs.end(), match);
      auto pp = std::find_if(this->md.parameters.begin(),
                             this->md.parameters.end(), match);
      if ((pi == this->md.inputs.end()) && (pp == this->md.parameters.end())) {
        tfel::raise(
            "MaterialPropertyDSL::finalizeVariablesDeclaration: "
            "no variable named '" +
            op.first + "' to be overriden");
      }
      {
        // checks
        const auto& v = (pi != this->md.inputs.end()) ? *pi : *pp;
        if (v.getTypeFlag() != SupportedTypes::SCALAR) {
          tfel::raise(
              "MaterialPropertyDSL::finalizeVariablesDeclaration: "
              "only scalar variables can be overriden by a parameter");
        }
        if (v.arraySize != 1u) {
          tfel::raise(
              "MaterialPropertyDSL::finalizeVariablesDeclaration: "
              "overriding array of parameters is not supported yet");
        }
      }
      if (pi != this->md.inputs.end()) {
        auto v = *pi;
        v.setAttribute(VariableDescription::defaultValue, op.second, true);
        this->md.inputs.erase(pi);
        this->md.parameters.push_back(v);
      } else {
        pp->setAttribute(VariableDescription::defaultValue, op.second, true);
      }
    }
    this->md.checkAndCompletePhysicalBoundsDeclaration();
  }  // end of finalizeVariablesDeclaration

  static std::string getFunctionAssociatedWithAValue(
      const VariableDescription& output, const double value) {
    auto body = std::string{};
    std::ostringstream os;
    os.precision(14);
    os << value;
    body += output.name + " = ";
    body += output.type + "{" + os.str() + "};\n";
    return body;
  }  // end of getFunctionAssociatedWithAValue

  void MaterialPropertyDSL::treatDataWithoutInput() {
    using namespace tfel::utilities;
    const auto opts = read<DataMap>(this->current, this->tokens.end());
    for (const auto& [k, v] : opts) {
      if (k != "value") {
        this->throwRuntimeError("MaterialPropertyDSL::treatData",
                                "unexpected option '" + k + "'");
      }
      if (!v.is<double>()) {
        this->throwRuntimeError("MaterialPropertyDSL::treatData",
                                "invalid type for option '" + k + "'");
      }
    }
    if (opts.empty()) {
      this->throwRuntimeError("MaterialPropertyDSL::treatData",
                              "no option 'value' defined");
    }
    const auto v = opts.at("value");
    this->md.f.body +=
        getFunctionAssociatedWithAValue(this->md.output, v.get<double>());
  }  // end of treatDataWithoutInput

  void MaterialPropertyDSL::treatDataWithOneInput() {
    using namespace tfel::utilities;
    const auto tfel_config = tfel::getTFELConfigExecutableName();
    auto& body = this->md.f.body;
    const auto& v = this->md.inputs[0];
    const auto idata = SingleVariableInterpolatedData::extract(
        read<DataMap>(this->current, this->tokens.end()));
    if (idata.itype == SingleVariableInterpolatedData::LINEAR_INTERPOLATION) {
      if (idata.values.size() == 1) {
        const auto value = idata.values.begin()->second;
        body += getFunctionAssociatedWithAValue(this->md.output, value);
      } else {
        this->md.f.used_inputs.insert(v.name);
        this->md.appendToIncludes(
            "#include \"TFEL/Math/LinearInterpolation.hxx\"");
        const auto args = SingleVariableInterpolatedData::
            WriteLinearInterpolationValuesArguments{
                .abscissae_name = "mfront_" + v.name + "_values",
                .abscissae_type = v.type,
                .ordinates_name = "mfront_" + this->md.output.name + "_values",
                .ordinates_type = this->md.output.type};
        const auto etype_value = idata.etype ? "true" : "false";
        body += writeLinearInterpolationValues(idata, args);
        body += this->md.output.name + " = ";
        body += "tfel::math::computeLinearInterpolation<";
        body += etype_value;
        body += ">(";
        body += args.abscissae_name + ", ";
        body += args.ordinates_name + ", ";
        body += v.name + ");\n";
      }
    } else {
      if (idata.values.size() == 1) {
        const auto value = idata.values.begin()->second;
        body += getFunctionAssociatedWithAValue(this->md.output, value);
      } else {
        this->md.appendToIncludes("#include \"TFEL/Math/CubicSpline.hxx\"");
        insert_if(this->link_directories,
                  "$(shell " + tfel_config + " --library-path)");
        insert_if(this->link_libraries,
                  "$(shell " + tfel_config +
                      " --library-dependency --math-cubic-spline)");
        this->md.f.used_inputs.insert(v.name);
        const auto etype_value = idata.etype ? "true" : "false";
        const auto args =
            SingleVariableInterpolatedData::WriteCollocationPointsArguments{
                .collocation_points_name = "mfront_collocation_points",
                .abscissae_type = v.type,
                .ordinates_type = this->md.output.type,
                .ordinates_derivatives_type = "tfel::math::derivative_type<" +
                                              this->md.output.type + ", " +
                                              v.type + ">"};
        body += writeCollocationPoints(idata, args);
        body += this->md.output.name + " = ";
        body += "tfel::math::computeCubicSplineInterpolation<";
        body += etype_value;
        body += ">(";
        body += args.collocation_points_name + ", ";
        body += v.name + ");\n";
      }
    }
  }  // end of treatDataWithOneInput

  void MaterialPropertyDSL::treatData() {
    auto throw_if = [this](const bool b, const std::string& m) {
      if (b) {
        this->throwRuntimeError("MaterialPropertyDSL::treatData", m);
      }
    };
    this->finalizeVariablesDeclaration();
    if (this->md.output.name.empty()) {
      this->reserveName("res");
      this->md.output = VariableDescription{"real", "res", 1u, 0u};
    }
    throw_if(!this->md.f.body.empty(), "function already defined");
    if (this->md.inputs.empty()) {
      this->treatDataWithoutInput();
    } else if (this->md.inputs.size() == 1) {
      this->treatDataWithOneInput();
    } else {
      this->throwRuntimeError(
          "MaterialPropertyDSL::treatData",
          "data with multiple inputs are not supported yet");
    }
    this->md.f.modified = true;
  }

  void MaterialPropertyDSL::treatFunction() {
    auto throw_if = [this](const bool b, const std::string& m) {
      if (b) {
        this->throwRuntimeError("MaterialPropertyDSL::treatFunction", m);
      }
    };
    this->finalizeVariablesDeclaration();
    unsigned int openedBrackets = 0;
    unsigned int openedParenthesis = 0;
    unsigned int currentLine;
    bool newLine;
    bool newInstruction;
    bool treated;
    throw_if(!this->md.f.body.empty(), "function already defined");
    if (this->md.output.name.empty()) {
      this->reserveName("res");
      this->md.output = VariableDescription{"real", "res", 1u, 0u};
    }
    this->md.f.modified = false;
    this->md.f.line = this->current->line;
    this->readSpecifiedToken("MaterialPropertyDSL::treatFunction", "{");
    // a simple check to see if this function is not empty
    this->checkNotEndOfFile("MaterialPropertyDSL::treatFunction");
    throw_if(this->current->value == "}", "function is empty.");
    --(this->current);
    ++openedBrackets;
    newInstruction = true;
    ++(this->current);
    this->checkNotEndOfFile("MaterialPropertyDSL::treatFunction",
                            "Expected body of function.");
    currentLine = this->current->line;
    newLine = true;
    if (!getDebugMode()) {
      this->md.f.body += "#line " + std::to_string(currentLine) + " \"" +
                         this->fd.fileName + "\"\n";
    }
    for (; (this->current != this->tokens.end()) && (openedBrackets != 0);
         ++(this->current)) {
      if (this->current->line != currentLine) {
        currentLine = this->current->line;
        this->md.f.body += "\n";
        if (!getDebugMode()) {
          this->md.f.body += "#line " + std::to_string(currentLine) + " \"" +
                             this->fd.fileName + "\"\n";
        }
        newLine = true;
      }
      if (this->current->value == "{") {
        ++openedBrackets;
        this->md.f.body += "{";
        newInstruction = true;
      } else if (this->current->value == "}") {
        --openedBrackets;
        if (openedBrackets != 0) {
          this->md.f.body += "}";
        }
      } else if (this->current->value == "(") {
        ++openedParenthesis;
        this->md.f.body += "(";
      } else if (this->current->value == ")") {
        throw_if(openedParenthesis == 0, "unbalanced parenthesis");
        --openedParenthesis;
        this->md.f.body += ")";
      } else if (this->current->value == ";") {
        this->md.f.body += ";";
        newInstruction = true;
      } else {
        if (!newLine) {
          this->md.f.body += " ";
        }
        const auto var = tfel::unicode::getMangledString(this->current->value);
        if (this->md.inputs.contains(var)) {
          this->md.f.used_inputs.insert(var);
        }
        if (this->md.parameters.contains(var)) {
          this->md.f.used_parameters.insert(var);
        }
        if (this->md.staticVars.contains(var)) {
          this->md.f.used_static_variables.insert(var);
        }
        if ((var == this->md.output.name) ||  //
            (this->md.inputs.contains(var)) ||
            (this->md.parameters.contains(var)) ||
            (this->md.staticVars.contains(var))) {
          treated = false;
          if (newInstruction) {
            ++(this->current);
            throw_if(this->current == tokens.end(),
                     "unexpected end of file while reading "
                     "body of function ");
            if ((this->current->value == "=") ||
                (this->current->value == "+=") ||
                (this->current->value == "-=") ||
                (this->current->value == "*=") ||
                (this->current->value == "/=")) {
              throw_if(var != this->md.output.name,
                       "trying to modify variable '" + var + "'.\n");
              this->md.f.modified = true;
              this->md.f.body += var + " " + this->current->value + " ";
              treated = true;
            } else {
              --(this->current);
            }
          }
          if (!treated) {
            this->md.f.body +=
                tfel::unicode::getMangledString(this->current->value);
          }
        } else {
          this->md.f.body +=
              tfel::unicode::getMangledString(this->current->value);
        }
        newInstruction = false;
      }
      newLine = false;
    }
    throw_if((this->current == tokens.end()) && (openedBrackets != 0),
             "unexpected end of file while reading body of function");
    throw_if(openedBrackets != 0,
             "parenthesis still opened at the end of function");
    throw_if(this->md.f.body.empty(), "empty function");
    throw_if(!this->md.f.modified, "function does not modify output.");
  }  // end of treatFunction

  void MaterialPropertyDSL::treatMethod() {
    using namespace tfel::utilities;
    using namespace tfel::glossary;
    auto throw_if = [this](const bool b, const std::string& m) {
      if (b) {
        this->throwRuntimeError("MaterialPropertyDSL::analyse", m);
      }
    };
    this->readSpecifiedToken("MaterialPropertyDSL::treatMethod", ".");
    this->checkNotEndOfFile("MaterialPropertyDSL::treatMethod",
                            "Expected method name.");
    const auto methodName = this->current->value;
    throw_if((methodName != "setGlossaryName") &&
                 (methodName != "setEntryName") &&
                 (methodName != "setDefaultValue"),
             "unknown method '" + methodName +
                 "' "
                 "valid methods are 'setGlossaryName', "
                 "'setEntryName' and 'setDefaultValue'");
    ++(this->current);
    this->readSpecifiedToken("MaterialPropertyDSL::treatMethod", "(");
    throw_if(!this->md.f.body.empty(),
             "MaterialPropertyDSL::treatMethod: variables methods "
             "can't be called at this stage (function is already defined)");
    if (methodName == "setGlossaryName") {
      const auto& glossary = Glossary::getGlossary();
      this->checkNotEndOfFile("MaterialPropertyDSL::treatMethod",
                              "Expected glossary name");
      throw_if(this->current->flag != Token::String,
               "expected a string as glossary name");
      throw_if(this->current->value.size() < 3, "glossary name too short");
      const auto gn =
          this->current->value.substr(1, this->current->value.size() - 2);
      throw_if(!glossary.contains(gn),
               "'" + gn + "' is not a valid glossary name");
      this->md.setGlossaryName(this->currentVar, gn);
    } else if (methodName == "setEntryName") {
      const auto& glossary = Glossary::getGlossary();
      this->checkNotEndOfFile("MaterialPropertyDSL::treatMethod",
                              "Expected entry file name.");
      throw_if(this->current->flag != Token::String,
               "expected a string as entry file name");
      throw_if(this->current->value.size() < 3, "entry file name too short");
      const auto en =
          this->current->value.substr(1, this->current->value.size() - 2);
      if (glossary.contains(en)) {
        std::ostringstream msg;
        msg << "'" << en << "' is a glossary name. Please use "
            << "the 'setGlossaryName' method or choose another entry name.";
        displayGlossaryEntryCompleteDescription(msg,
                                                glossary.getGlossaryEntry(en));
        this->throwRuntimeError("MaterialPropertyDSL::treatMethod", msg.str());
      }
      throw_if(!CxxTokenizer::isValidIdentifier(en),
               "invalid entry name '" + en + "'");
      this->md.setEntryName(this->currentVar, en);
    } else if (methodName == "setDefaultValue") {
      throw_if(!this->md.isParameterName(this->currentVar),
               "method 'setDefaultValue' is reserved for paramaters");
      this->checkNotEndOfFile(
          "MaterialPropertyDSL::treatMethod",
          "Expected to read value of variable '" + this->currentVar + "'");
      auto& p = this->md.parameters.getVariable(this->currentVar);
      const auto v = this->readDouble();
      p.setAttribute(VariableDescription::defaultValue, v, false);
      --(this->current);
    } else {
      throw_if(true, "internal error (untreated method '" + methodName + "'");
    }
    ++(this->current);
    this->readSpecifiedToken("MaterialPropertyDSL::treatMethod", ")");
    this->readSpecifiedToken("MaterialPropertyDSL::treatMethod", ";");
  }  // end of treatMethod

  void MaterialPropertyDSL::importFile(
      const std::string& fn,
      const std::vector<std::string>& ecmds,
      const std::map<std::string, std::string>& s) {
    this->fd.fileName = fn;
    this->openFile(this->fd.fileName, ecmds, s);
    this->analyse();
  }

  void MaterialPropertyDSL::analyseString(const std::string& s) {
    this->fd.fileName = "user defined string";
    this->parseString(s);
    this->analyse();
  }

  void MaterialPropertyDSL::analyse() {
    auto throw_if = [this](const bool b, const std::string& m) {
      if (b) {
        this->throwRuntimeError("MaterialPropertyDSL::analyse", m);
      }
    };
    // strip comments from file
    this->stripComments();
    // begin treatement
    this->current = this->tokens.begin();
    while (this->current != this->tokens.end()) {
      const auto p = this->callBacks.find(this->current->value);
      if (p == this->callBacks.end()) {
        MemberFuncPtr handler = nullptr;
        const auto c = tfel::unicode::getMangledString(this->current->value);
        if ((this->md.output.name == c) ||
            (this->md.output.symbolic_form == c)) {
          this->currentVar = this->md.output.name;
          handler = &MaterialPropertyDSL::treatMethod;
        }
        for (auto p2 = this->md.inputs.begin();
             (p2 != this->md.inputs.end()) && (handler == nullptr);) {
          if ((p2->name == c) || (p2->symbolic_form == c)) {
            this->currentVar = p2->name;
            handler = &MaterialPropertyDSL::treatMethod;
          } else {
            ++p2;
          }
        }
        for (auto p3 = this->md.parameters.begin();
             (p3 != this->md.parameters.end()) && (handler == nullptr);) {
          if ((p3->name == c) || (p3->symbolic_form == c)) {
            this->currentVar = p3->name;
            handler = &MaterialPropertyDSL::treatMethod;
          } else {
            ++p3;
          }
        }
        if (handler == nullptr) {
          handler = &MaterialPropertyDSL::treatUnknownKeyword;
        }
        ++(this->current);
        (this->*handler)();
      } else {
        auto handler = p->second;
        auto key = this->current->value;
        this->currentComment = this->current->comment;
        ++(this->current);
        try {
          (this->*handler)();
        } catch (const std::exception& e) {
          this->currentComment.clear();
          throw_if(true, "error while treating keyword '" + key + "'.\n" +
                             std::string(e.what()));
        } catch (...) {
          this->currentComment.clear();
          throw_if(true, "error while treating keyword '" + key + '\'');
        }
        this->currentComment.clear();
      }
    }
  }

  void MaterialPropertyDSL::analyseFile(
      const std::string& fileName_,
      const std::vector<std::string>& ecmds,
      const std::map<std::string, std::string>& s) {
    this->importFile(fileName_, ecmds, s);
    this->endsInputFileProcessing();
    this->makeConsistencyChecks();
    for (const auto& i : this->interfaces) {
      i.second->getTargetsDescription(this->td, this->md);
    }
    this->completeTargetsDescription();
  }

  bool MaterialPropertyDSL::useQt() const {
    if (!this->md.use_qt.has_value()) {
      return false;
    }
    return *(this->md.use_qt);
  }  // end of useQt

  void MaterialPropertyDSL::disableQuantitiesUsageIfNotAlreadySet() {
    if (!this->md.use_qt.has_value()) {
      this->md.use_qt = false;
    }
  }  // end of disableQuantitiesUsageIfNotAlreadySet

  void MaterialPropertyDSL::addExternalMFrontFile(
      const std::string& f,
      const std::vector<std::string>& vinterfaces,
      const tfel::utilities::DataMap& dsl_options) {
    this->md.addExternalMFrontFile(f, vinterfaces, dsl_options);
  }  // end of addExternalMFrontFile

  const MaterialKnowledgeDescription&
  MaterialPropertyDSL::getMaterialKnowledgeDescription() const {
    return this->md;
  }  // end of getMaterialKnowledgeDescription

  void MaterialPropertyDSL::reserveName(const std::string& n) {
    this->md.reserveName(n);
  }

  bool MaterialPropertyDSL::isNameReserved(const std::string& n) const {
    return this->md.isNameReserved(n);
  }

  void MaterialPropertyDSL::generateOutputFiles() {
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(b, "MaterialPropertyDSL::generateOutputFiles: " + m);
    };
    throw_if(this->md.className.empty(), "no material property name defined.");
    throw_if(this->md.f.body.empty(), "no function defined.");
    // creating directories
    if (!this->interfaces.empty()) {
      tfel::system::systemCall::mkdir("include");
      tfel::system::systemCall::mkdir("src");
    }
    //! generating sources du to external material properties and models
    for (const auto& em : this->md.getExternalMFrontFiles()) {
      this->callMFront({em.first}, std::get<0>(em.second),
                       std::get<1>(em.second));
    }
    // calling interfaces
    for (const auto& i : this->interfaces) {
      if (getVerboseMode() >= VERBOSE_LEVEL2) {
        getLogStream() << "calling interface " << i.first << '\n';
      }
      i.second->writeOutputFiles(this->md, this->fd);
    }
  }  // end of generateOutputFiles

  void MaterialPropertyDSL::treatInput() {
    tfel::raise_if(
        !this->md.f.body.empty(),
        "MaterialPropertyDSL::treatInput: inputs can't be declared at "
        "this stage (function is already defined)");
    this->checkNotEndOfFile("MaterialPropertyDSL::treatInput");
    const auto type = [this]() -> std::string {
      const auto otype = this->readVariableTypeIfPresent();
      if (!otype) {
        return "real";
      }
      return *otype;
    }();
    auto ninputs = VariableDescriptionContainer{};
    this->readVarList(ninputs, type, false);
    for (const auto& i : ninputs) {
      this->reserveName(i.name);
      this->md.inputs.push_back(i);
    }
  }  // end of treatInput

  void MaterialPropertyDSL::treatOutput() {
    tfel::raise_if(
        !this->md.f.body.empty(),
        "MaterialPropertyDSL::treatOutput: output can't be declared at "
        "this stage (function is already defined)");
    if (!this->md.output.name.empty()) {
      this->throwRuntimeError("MaterialPropertyDSL::treatOutput",
                              "Output already defined.");
    }
    this->checkNotEndOfFile("MaterialPropertyDSL::treatOutput");
    const auto type = [this]() -> std::string {
      const auto otype = this->readVariableTypeIfPresent();
      if (!otype) {
        return "real";
      }
      return *otype;
    }();
    const auto s = this->readOnlyOneToken();
    if (SupportedTypes::getTypeFlag(type) != SupportedTypes::SCALAR) {
      this->throwRuntimeError("DSLBase::treatOutput",
                              "output type'" + type + "' is not valid.");
    }
    const auto n = tfel::unicode::getMangledString(s);
    if (!tfel::utilities::CxxTokenizer::isValidIdentifier(n, false)) {
      this->throwRuntimeError("MaterialPropertyDSL::treatOutput",
                              "invalid output name.");
    }
    this->reserveName(n);
    if (s != n) {
      this->reserveName(s);
      this->md.output = VariableDescription{type, s, n, 1u, 0u};
    } else {
      this->md.output = VariableDescription{type, n, 1u, 0u};
    }
  }  // end of treatOutput

  void MaterialPropertyDSL::treatBounds() {
    const auto [n, bounds] = this->readVariableBounds();
    this->readSpecifiedToken("MaterialPropertyDSL::treatBounds", ";");
    if (n == this->md.output.name) {
      this->md.output.setBounds(bounds);
      return;
    }
    auto& v = this->md.inputs.getVariable(n);
    v.setBounds(bounds);
  }  // end of treatBounds

  void MaterialPropertyDSL::treatPhysicalBounds() {
    const auto [n, bounds] = this->readVariableBounds();
    this->readSpecifiedToken("MaterialPropertyDSL::treatPhysicalBounds", ";");
    if (n == this->md.output.name) {
      this->md.output.setPhysicalBounds(bounds);
      return;
    }
    auto& v = this->md.inputs.getVariable(n);
    v.setPhysicalBounds(bounds);
  }  // end of treatPhysicalBounds

  void MaterialPropertyDSL::treatUnknownKeyword() {
    TokensContainer::const_iterator p2;
    auto treated = false;
    --(this->current);
    const auto key = this->current->value;
    ++(this->current);
    this->checkNotEndOfFile("MaterialPropertyDSL::treatUnknownKeyword");
    if (this->current->value == "[") {
      ++(this->current);
      this->checkNotEndOfFile("MaterialPropertyDSL::treatUnknownKeyword");
      auto s = std::vector<std::string>{};
      while (this->current->value != "]") {
        this->checkNotEndOfFile("MaterialPropertyDSL::treatUnknownKeyword");
        const auto t = [this]() -> std::string {
          if (this->current->flag == tfel::utilities::Token::String) {
            return this->current->value.substr(1,
                                               this->current->value.size() - 2);
          }
          return this->current->value;
        }();
        ++(this->current);
        this->checkNotEndOfFile("MaterialPropertyDSL::treatUnknownKeyword");
        if (std::find(s.begin(), s.end(), t) == s.end()) {
          s.push_back(t);
        }
        if (this->current->value != "]") {
          this->readSpecifiedToken("MaterialPropertyDSL::treatUnknownKeyword",
                                   ",");
          this->checkNotEndOfFile("MaterialPropertyDSL::treatUnknownKeyword");
          if (this->current->value == "]") {
            this->throwRuntimeError("MaterialPropertyDSL::treatUnknownKeyword",
                                    "unexpected token ']'");
          }
        }
      }
      ++(this->current);
      for (auto& i : this->interfaces) {
        auto p =
            i.second->treatKeyword(key, s, this->current, this->tokens.end());
        if (p.first) {
          if (treated) {
            if (p2 != p.second) {
              this->throwRuntimeError(
                  "MaterialPropertyDSL::treatUnknownKeyword",
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
        auto p =
            i.second->treatKeyword(key, {}, this->current, this->tokens.end());
        if (p.first) {
          if (treated) {
            if (p2 != p.second) {
              this->throwRuntimeError(
                  "MaterialPropertyDSL::treatUnknownKeyword",
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
  }  // end of treatUnknownKeyword

  const MaterialPropertyDescription&
  MaterialPropertyDSL::getMaterialPropertyDescription() const {
    return this->md;
  }  // end of getMaterialPropertyDescription

  void MaterialPropertyDSL::appendToIncludes(const std::string& c) {
    this->md.appendToIncludes(c);
  }  // end of appendToIncludes

  void MaterialPropertyDSL::appendToMembers(const std::string& c) {
    this->md.appendToMembers(c);
  }  // end of appendToMembers

  void MaterialPropertyDSL::appendToPrivateCode(const std::string& c) {
    this->md.appendToPrivateCode(c);
  }  // end of appendToPrivateCode

  void MaterialPropertyDSL::appendToSources(const std::string& c) {
    this->md.appendToSources(c);
  }  // end of appendToSources

  void MaterialPropertyDSL::addMaterialLaw(const std::string& m) {
    this->md.addMaterialLaw(m);
  }  // end of addMaterialLaw

  MaterialPropertyDSL::~MaterialPropertyDSL() = default;

}  // end of namespace mfront
