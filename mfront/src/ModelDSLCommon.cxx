/*!
 * \file  mfront/src/ModelDSLCommon.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 11 jun 2010
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <locale>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include "TFEL/Raise.hxx"
#include "TFEL/System/System.hxx"
#include "TFEL/Glossary/Glossary.hxx"
#include "TFEL/Glossary/GlossaryEntry.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"
#include "TFEL/UnicodeSupport/UnicodeSupport.hxx"
#include "MFront/PedanticMode.hxx"
#include "MFront/DSLUtilities.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MFront/MFrontDebugMode.hxx"
#include "MFront/ModelDSLCommon.hxx"
#include "MFront/TargetsDescription.hxx"
#include "MFront/ModelInterfaceFactory.hxx"

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

  static bool is(const VariableDescriptionContainer& vc,
                 const std::string& vn) {
    for (const auto& v : vc) {
      if (v.name == vn) {
        return true;
      }
      if (v.hasAttribute(VariableDescription::depth)) {
        const auto d =
            v.getAttribute<unsigned short>(VariableDescription::depth);
        for (unsigned short i = 1; i != d + 1; ++i) {
          if (v.name + '_' + std::to_string(i) == vn) {
            return true;
          }
        }
      }
    }
    return false;
  }  // end of is()

  bool isValidModelName(const std::string& n) {
    return tfel::utilities::CxxTokenizer::isValidIdentifier(n, false);
  }

  ModelDSLCommon::ModelDSLCommon(const DSLOptions& opts) : DSLBase(opts) {
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
    //
    this->reserveName("dt");
    this->reserveName("\u0394t");
    for (const auto& v : DSLBase::getDefaultReservedNames()) {
      this->reserveName(v);
    }
    //
    this->appendToIncludes("#include<cmath>");
    this->appendToIncludes("#include<limits>");
    this->appendToIncludes("#include<string>");
    this->appendToIncludes("#include<sstream>");
    this->appendToIncludes("#include<iostream>");
    this->appendToIncludes("#include<stdexcept>");
  }

  AbstractDSL::DSLOptions ModelDSLCommon::buildDSLOptions() const {
    return DSLBase::buildCommonDSLOptions(this->md);
  }  // end of buildDSLOptions

  std::string ModelDSLCommon::getMaterialKnowledgeIdentifier() const {
    return this->md.className;
  }  // end of getMaterialKnowledgeIdentifier

  std::string ModelDSLCommon::getMaterialName() const {
    return this->md.material;
  }  // end of getMaterialName(

  std::string ModelDSLCommon::getOverridableVariableNameByExternalName(
      const std::string&) const {
#pragma message("unimplemented")
    tfel::raise(
        "ModelDSLCommon::getOverridableVariableNameByExternalName: "
        "unimplemented feature");
  }  // end of getOverridableVariableNameByExternalName

  void ModelDSLCommon::overrideByAParameter(const std::string&, const double){
#pragma message("unimplemented")
  }  // end of overrideByAParameter

  std::map<std::string, double> ModelDSLCommon::getOverridenParameters() const {
    return this->overriding_parameters;
  }  // end of getOverridenParameters

  void ModelDSLCommon::endsInputFileProcessing() {
    if (this->md.modelName.empty()) {
      tfel::raise(
          "ModelDSLCommon::endsInputFileProcessing: "
          "model name undefined");
    }
    // complete the declaration of physical bounds
    this->md.checkAndCompletePhysicalBoundsDeclaration();

    if (getPedanticMode()) {
      this->doPedanticChecks();
    }
  }  // end of endsInputFileProcessing

  bool ModelDSLCommon::useQt() const {
    return this->md.use_qt.has_value() ? *(this->md.use_qt) : false;
  }

  void ModelDSLCommon::disableQuantitiesUsageIfNotAlreadySet() {
    if (!this->md.use_qt.has_value()) {
      this->md.use_qt = false;
    }
  }  // end of disableQuantitiesUsageIfNotAlreadySet

  void ModelDSLCommon::reserveName(const std::string& n) {
    this->md.reserveName(n);
  }

  bool ModelDSLCommon::isNameReserved(const std::string& n) const {
    return this->md.isNameReserved(n);
  }

  AbstractDSL::DSLTarget ModelDSLCommon::getTargetType() const {
    return MODELDSL;
  }

  std::string ModelDSLCommon::getClassName() const {
    return this->md.className;
  }  // end of getClassName

  void ModelDSLCommon::addStaticVariableDescription(
      const StaticVariableDescription& v) {
    this->md.staticVars.push_back(v);
  }  // end of addStaticVariableDescription

  std::map<std::string, int> ModelDSLCommon::getIntegerConstants() const {
    auto r = std::map<std::string, int>{};
    for (const auto& v : this->md.staticVars) {
      if (v.type == "int") {
        r.insert({v.name, v.value});
      }
    }
    return r;
  }  // end of getIntegerConstants

  int ModelDSLCommon::getIntegerConstant(const std::string& n) const {
    for (const auto& v : this->md.staticVars) {
      if (v.name == n) {
        if (v.type != "int") {
          this->throwRuntimeError("ModelDSLCommon::getIntegerConstant",
                                  "invalid type for variable '" + n + "'");
        }
        return v.value;
      }
    }
    this->throwRuntimeError("ModelDSLCommon::getIntegerConstant",
                            "unknown variable '" + n + "'");
  }  // end of getIntegerConstant

  void ModelDSLCommon::setUnitSystem(const std::string_view s) {
    this->md.setUnitSystem(s);
  }  // end of setUnitSystem

  void ModelDSLCommon::setMaterial(const std::string& m) {
    if (!this->md.material.empty()) {
      this->throwRuntimeError("ModelDSLCommon::setMaterial",
                              "material name alreay defined");
    }
    if (!isValidMaterialName(m)) {
      this->throwRuntimeError("ModelDSLCommon::setMaterial",
                              "invalid material name ('" + m + "')");
    }
    this->md.material = m;
    if (!this->md.modelName.empty()) {
      this->md.className = this->md.material + "_" + this->md.modelName;
    }
  }  // end of setMaterial

  void ModelDSLCommon::treatUseQt() {
    this->checkNotEndOfFile("ModelDSLCommon::treatUseQt",
                            "Expected 'true' or 'false'.");
    if (this->md.use_qt.has_value()) {
      this->throwRuntimeError("ModelDSLCommon::treatUseQt",
                              "quantity usage has already been specified");
    }
    this->md.use_qt = this->readBooleanValue("ModelDSLCommon::treatUseQt");
    this->readSpecifiedToken("ModelDSLCommon::treatUseQt", ";");
  }  // end of treatUseQt

  void ModelDSLCommon::treatLibrary() {
    const auto& l = this->readOnlyOneToken();
    if (!isValidLibraryName(l)) {
      this->throwRuntimeError("ModelDSLCommon::treatLibrary",
                              "invalid library name");
    }
    if (!this->md.library.empty()) {
      this->throwRuntimeError("ModelDSLCommon::treatLibrary",
                              "library name already registred");
    }
    this->md.library = l;
  }  // end of treatLibrary

  void ModelDSLCommon::setMaterialKnowledgeIdentifier(const std::string& i) {
    if (!this->md.className.empty()) {
      this->throwRuntimeError("ModelDSLCommon::setMaterialKnowledgeIdentifier",
                              "model name already defined");
    }
    if (!isValidModelName(i)) {
      this->throwRuntimeError("ModelDSLCommon::setMaterialKnowledgeIdentifier",
                              "invalid model name");
    }
    this->md.modelName = i;
    if (!this->md.material.empty()) {
      this->md.className = this->md.material + "_" + this->md.modelName;
    } else {
      this->md.className = i;
    }
  }  // end of setMaterialKnowledgeIdentifier

  void ModelDSLCommon::treatModel() {
    const auto& m = this->readOnlyOneToken();
    if (!isValidModelName(m)) {
      this->throwRuntimeError("ModelDSLCommon::treatModel",
                              "invalid model name");
    }
    if (this->overriden_implementation_name.empty()) {
      this->setMaterialKnowledgeIdentifier(m);
    }
  }  // end of treatModel

  void ModelDSLCommon::setInterfaces(const std::set<std::string>& inames) {
    auto& mbif = ModelInterfaceFactory::getModelInterfaceFactory();
    for (const auto& i : inames) {
      if (this->interfaces.count(i) == 0) {
        this->interfaces.insert({i, mbif.getInterface(i)});
      }
    }
  }  // end of setInterfaces

  void ModelDSLCommon::generateOutputFiles() {
    // creating directories
    if (!this->interfaces.empty()) {
      tfel::system::systemCall::mkdir("include");
      tfel::system::systemCall::mkdir("src");
    }
    //! generating sources du to external material properties and models
    for (const auto& em : this->md.getExternalMFrontFiles()) {
      this->callMFront(em.second, {em.first});
    }
    //! generating sources by the interfaces
    for (const auto& i : this->interfaces) {
      i.second->writeOutputFiles(this->fd, this->md);
    }
  }  // end of generateOutputFiles

  void ModelDSLCommon::treatUnknownKeyword() {
    TokensContainer::const_iterator p2;
    auto treated = false;
    --(this->current);
    const auto key = this->current->value;
    ++(this->current);
    this->checkNotEndOfFile("ModelDSLCommon::treatUnknownKeyword");
    if (this->current->value == "[") {
      ++(this->current);
      this->checkNotEndOfFile("ModelDSLCommon::treatUnknownKeyword");
      auto s = std::vector<std::string>{};
      while (this->current->value != "]") {
        this->checkNotEndOfFile("ModelDSLCommon::treatUnknownKeyword");
        const auto t = [this]() -> std::string {
          if (this->current->flag == tfel::utilities::Token::String) {
            return this->current->value.substr(1,
                                               this->current->value.size() - 2);
          }
          return this->current->value;
        }();
        ++(this->current);
        this->checkNotEndOfFile("ModelDSLCommon::treatUnknownKeyword");
        if (std::find(s.begin(), s.end(), t) == s.end()) {
          s.push_back(t);
        }
        if (this->current->value != "]") {
          this->readSpecifiedToken("ModelDSLCommon::treatUnknownKeyword", ",");
          this->checkNotEndOfFile("ModelDSLCommon::treatUnknownKeyword");
          if (this->current->value == "]") {
            this->throwRuntimeError("ModelDSLCommon::treatUnknownKeyword",
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
              this->throwRuntimeError("ModelDSLCommon::treatUnknownKeyword",
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
              this->throwRuntimeError("ModelDSLCommon::treatUnknownKeyword",
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

  void ModelDSLCommon::treatDomain() {
    auto throw_if = [this](const bool b, const std::string& m) {
      if (b) {
        this->throwRuntimeError("ModelDSLCommon::treatDomain", m);
      }
    };
    throw_if(!this->md.domains.empty(), "domain defined");
    this->checkNotEndOfFile("ModelDSLCommon::treatDomain");
    throw_if(
        this->current->flag != tfel::utilities::Token::String,
        "Expected to read a string (read '" + this->current->value + "').");
    throw_if(this->current->value.size() < 2, "domain name too short.");
    this->md.domains.insert(
        this->current->value.substr(1, this->current->value.size() - 2));
    ++(this->current);
    this->readSpecifiedToken("ModelDSLCommon::treatDomain", ";");
  }  // end of treatDomain()

  void ModelDSLCommon::treatDomains() {
    auto throw_if = [this](const bool b, const std::string& m) {
      if (b) {
        this->throwRuntimeError("ModelDSLCommon::treatDomains", m);
      }
    };
    throw_if(!this->md.domains.empty(), "domains defined");
    for (const auto& d :
         this->readArrayOfString("ModelDSLCommon::treatDomains")) {
      throw_if(!this->md.domains.insert(d).second,
               "domain " + d + " already defined.");
    }
    throw_if(this->md.domains.empty(), "@Domains does not set any domain");
    this->readSpecifiedToken("ModelDSLCommon::treatDomain", ";");
  }  // end of treatDomains()

  bool ModelDSLCommon::isInputVariable(const std::string& v) const {
    return is(this->md.inputs, v);
  }  // end of isInputVariable()

  bool ModelDSLCommon::isOutputVariable(const std::string& v) const {
    return is(this->md.outputs, v);
  }  // end of isInputVariable()

  std::map<std::string, std::string> ModelDSLCommon::getSymbols() {
    auto symbols = std::map<std::string, std::string>{};
    auto filtered_inputs = VariableDescriptionContainer{};
    std::copy_if(
        this->md.inputs.begin(), this->md.inputs.end(),
        std::back_inserter(filtered_inputs), [](const VariableDescription& v) {
          if (!v.hasAttribute(VariableDescription::depth)) {
            return false;
          }
          return v.getAttribute<unsigned short>(VariableDescription::depth) > 0;
        });
    mfront::getIncrementSymbols(symbols, filtered_inputs);
    mfront::addSymbol(symbols, "\u0394t", "dt");
    return symbols;
  }  // end of getSymbols

  void ModelDSLCommon::treatIntegrator() {
    this->readFunction("DefaultFunction");
  }  // end of treatIntegrator

  void ModelDSLCommon::readFunction(const std::string& fn) {
    auto throw_if = [this](const bool b, const std::string& m) {
      if (b) {
        this->throwRuntimeError("ModelDSLCommon::rreatFunction", m);
      }
    };
    auto isStaticMemberName = [](const ModelDescription& d,
                                 const std::string& n) -> bool {
      for (const auto& v : d.staticMemberNames) {
        if (v == n) {
          return true;
        }
      }
      return false;
    };
    auto isMemberName = [](const ModelDescription& d,
                           const std::string& n) -> bool {
      for (const auto& v : d.memberNames) {
        if (v == n) {
          return true;
        }
      }
      return false;
    };
    const auto symbols = this->getSymbols();
    auto demangle = [&symbols](const tfel::utilities::Token& t) {
      if (t.flag != tfel::utilities::Token::Standard) {
        return t.value;
      }
      const auto p = symbols.find(t.value);
      if (p != symbols.end()) {
        return p->second;
      }
      return tfel::unicode::getMangledString(t.value);
    };
    ModelDescription::Function f;
    unsigned int openedBrackets = 0;
    unsigned int openedParenthesis = 0;
    f.useTimeIncrement = false;
    throw_if(!this->isValidIdentifier(fn),
             "function name '" + fn + "' is not valid");
    f.name = fn;
    f.line = this->current->line;
    this->readSpecifiedToken("ModelDSLCommon::treatFunction", "{");
    ++openedBrackets;
    this->checkNotEndOfFile("ModelDSLCommon::treatFunction",
                            "expected body of function '" + f.name + "'.");
    auto currentLine = this->current->line;
    auto newInstruction = true;
    auto newLine = true;
    if (!getDebugMode()) {
      f.body += "#line " + std::to_string(currentLine) + " \"" +
                this->fd.fileName + "\"\n";
    }
    for (; (this->current != this->tokens.end()) && (openedBrackets != 0);
         ++(this->current)) {
      if (this->current->line != currentLine) {
        currentLine = this->current->line;
        f.body += "\n";
        if (!getDebugMode()) {
          f.body += "#line " + std::to_string(currentLine) + " \"" +
                    this->fd.fileName + "\"\n";
        }
        newLine = true;
      }
      if (this->current->value == "{") {
        ++openedBrackets;
        f.body += "{";
        newInstruction = true;
      } else if (this->current->value == "}") {
        --openedBrackets;
        if (openedBrackets != 0) {
          f.body += "}";
        }
      } else if (this->current->value == "(") {
        ++openedParenthesis;
        f.body += "(";
      } else if (this->current->value == ")") {
        throw_if(openedParenthesis == 0, "unbalanced parenthesis");
        --openedParenthesis;
        f.body += ")";
      } else if (this->current->value == ";") {
        f.body += ";";
        newInstruction = true;
      } else {
        if (!newLine) {
          f.body += " ";
        }
        const auto c = demangle(*(this->current));
        if (isStaticMemberName(this->md, c)) {
          f.body += "(" + this->md.className + ":: " + c + ")";
        } else if (isMemberName(this->md, c)) {
          bool treated = false;
          if (newInstruction) {
            auto op = std::string{};
            ++(this->current);
            throw_if(this->current == tokens.end(),
                     "unexpected end of file while reading "
                     "body of function '" +
                         f.name + "'");
            auto modifier = false;
            if (this->current->value == "=") {
              op = "=";
              modifier = true;
            } else if (this->current->value == "+=") {
              op = "+=";
              modifier = true;
            } else if (this->current->value == "-=") {
              op = "-=";
              modifier = true;
            } else if (this->current->value == "*=") {
              op = "*=";
              modifier = true;
            } else if (this->current->value == "/=") {
              op = "/=";
              modifier = true;
            }
            if (modifier) {
              bool found = false;
              for (auto p = this->md.outputs.begin();
                   (p != this->md.outputs.end()) && (!found);) {
                if (p->name == c) {
                  found = true;
                } else {
                  ++p;
                }
              }
              throw_if(!found, "trying to modify variable '" + c +
                                   "' in the body of function '" + f.name +
                                   "'");
              f.body += c + " " + op + " ";
              f.modifiedVariables.insert(c);
              auto p6 = f.depths.find(c);
              if (p6 == f.depths.end()) {
                f.depths[c] = 0;
              }
              treated = true;
            } else {
              --(this->current);
            }
          }
          if (!treated) {
            // treating the case of local parameters
            for (auto p = this->md.parameters.begin();
                 (p != this->md.parameters.end()) && (!treated); ++p) {
              if (p->name == c) {
                treated = true;
                f.parameters.insert(c);
                f.body += "(this->" + c + ")";
              }
            }
            // treating the case of local parameters
            for (auto p = this->md.constantMaterialProperties.begin();
                 (p != this->md.constantMaterialProperties.end()) && (!treated);
                 ++p) {
              if (p->name == c) {
                treated = true;
                f.constantMaterialProperties.insert(c);
                f.body += "(this->" + c + ")";
              }
            }
            if (!treated) {
              if (this->isInputVariable(c)) {
                f.usedVariables.insert(c);
                auto dv = this->md.decomposeVariableName(c);
                auto p6 = f.depths.find(dv.first);
                if (p6 != f.depths.end()) {
                  if (dv.second > p6->second) {
                    f.depths[dv.first] = dv.second;
                  }
                } else {
                  f.depths[dv.first] = dv.second;
                }
              } else if (this->isOutputVariable(c)) {
                f.usedVariables.insert(c);
                auto dv = this->md.decomposeVariableName(c);
                auto p6 = f.depths.find(dv.first);
                if (p6 != f.depths.end()) {
                  if (dv.second > p6->second) {
                    f.depths[dv.first] = dv.second;
                  }
                } else {
                  f.depths[dv.first] = dv.second;
                }
              }
              f.body += c;
            }
          }
        } else {
          if ((c == "dt") || (c == "\u0394t")) {
            f.useTimeIncrement = true;
            f.body += "dt";
          } else {
            f.body += c;
          }
        }
        newInstruction = false;
      }
      newLine = false;
    }
    throw_if((this->current == tokens.end()) && (openedBrackets != 0),
             "unexpected end of file while reading body of function '" +
                 f.name + "'");
    throw_if(
        openedBrackets != 0,
        "parenthesis still opened at the end of function '" + f.name + "'");
    throw_if(f.modifiedVariables.empty(),
             "function " + f.name + " does not change any variable.");
    for (const auto& df : this->md.functions) {
      throw_if(df.name == f.name, "function " + f.name + " already declared.");
    }
    for (auto p2 = f.modifiedVariables.begin(); p2 != f.modifiedVariables.end();
         ++p2) {
      auto p3 = f.usedVariables.find(*p2);
      if (p3 != f.usedVariables.end()) {
        f.usedVariables.erase(*p3);
      }
    }
    this->md.registerMemberName(f.name);
    this->reserveName(f.name + ".Domain");
    this->reserveName(f.name + ".Domains");
    this->md.registerMemberName("functor" +
                                std::to_string(this->md.functions.size()));
    this->md.functions.push_back(f);
  }  // end of readFunction

  void ModelDSLCommon::treatFunction() {
    this->checkNotEndOfFile("ModelDSLCommon::treatFunction");
    const auto fn = [this] {
      const auto symbols = this->getSymbols();
      if (this->current->flag != tfel::utilities::Token::Standard) {
        return this->current->value;
      }
      const auto p = symbols.find(this->current->value);
      if (p != symbols.end()) {
        return p->second;
      }
      return tfel::unicode::getMangledString(this->current->value);
    }();
    ++(this->current);
    this->readFunction(fn);
  }  // end of treatFunction

  void ModelDSLCommon::treatOutput() {
    if (!this->md.functions.empty()) {
      this->throwRuntimeError("ModelDSLCommon::treatOutput",
                              "outputs must be declared before "
                              "declaring functions");
    }
    VariableDescriptionContainer noutputs;
    const auto type = [this]() -> std::string {
      const auto otype = this->readVariableTypeIfPresent();
      if (!otype) {
        return "real";
      }
      return *otype;
    }();
    if (SupportedTypes::getTypeFlag(type) != SupportedTypes::SCALAR) {
      this->throwRuntimeError("DSLBase::treatOutput",
                              "output type'" + type + "' is not valid.");
    }
    this->readVarList(noutputs, type, false);
    for (const auto& v : noutputs) {
      if (!v.symbolic_form.empty()) {
        this->reserveName(v.symbolic_form);
      }
      this->md.registerMemberName(v.name);
      this->md.outputs.push_back(v);
    }
  }  // end of treatOutput

  void ModelDSLCommon::treatInput() {
    if (!this->md.functions.empty()) {
      this->throwRuntimeError("ModelDSLCommon::treatInput",
                              "inputs must be declared before "
                              "declaring functions");
    }
    VariableDescriptionContainer ninputs;
    const auto type = [this]() -> std::string {
      const auto otype = this->readVariableTypeIfPresent();
      if (!otype) {
        return "real";
      }
      return *otype;
    }();
    if (SupportedTypes::getTypeFlag(type) != SupportedTypes::SCALAR) {
      this->throwRuntimeError("DSLBase::treatInput",
                              "input type'" + type + "' is not valid.");
    }
    this->readVarList(ninputs, type, false);
    for (const auto& v : ninputs) {
      if (!v.symbolic_form.empty()) {
        this->reserveName(v.symbolic_form);
      }
      this->md.registerMemberName(v.name);
      this->md.inputs.push_back(v);
    }
  }  // end of treatInput

  void ModelDSLCommon::treatOutputMethod() {
    if (!this->md.functions.empty()) {
      this->throwRuntimeError("ModelDSLCommon::treatOutputMethod: ",
                              "variable methods must be called "
                              "before declaring functions");
    }
    this->readSpecifiedToken("ModelDSLCommon::treatOutputMethod", ".");
    this->checkNotEndOfFile("ModelDSLCommon::treatOutputMethod",
                            "Expected method name.");
    const auto mn = this->current->value;
    if ((mn != "setGlossaryName") && (mn != "setEntryName") &&
        (mn != "setDefaultInitialValue") && (mn != "setDepth")) {
      this->throwRuntimeError("ModelDSLCommon::treatOutputMethod",
                              "Unknown method (valid methods for fields are "
                              "setGlossaryName, setEntryName, setDepth and "
                              "setDefaultInitialValue,"
                              "read '" +
                                  mn + "').");
    }
    ++(this->current);
    this->readSpecifiedToken("ModelDSLCommon::treatOutputMethod", "(");
    if (mn == "setGlossaryName") {
      const auto gn = this->readString("ModelDSLCommon::treatOutputMethod");
      this->md.setGlossaryName(this->currentVar, gn);
    } else if (mn == "setEntryName") {
      const auto en = this->readString("ModelDSLCommon::treatOutputMethod");
      this->md.setEntryName(this->currentVar, en);
    } else if (mn == "setDefaultInitialValue") {
      this->checkNotEndOfFile("ModelDSLCommon::treatOutputMethod",
                              "Expected intial value.");
      const auto value = this->readDouble();
      auto& v = this->md.outputs.getVariable(this->currentVar);
      v.setAttribute(VariableDescription::initialValue, value, false);
    } else if (mn == "setDepth") {
      this->checkNotEndOfFile("ModelDSLCommon::treatOutputMethod",
                              "Expected depth value.");
      unsigned short value;
      std::istringstream converter(this->current->value);
      converter.imbue(std::locale::classic());
      converter >> value;
      if (!converter || (!converter.eof())) {
        this->throwRuntimeError(
            "ModelDSLCommon::treatOutputMethod",
            "Could not read depth value of field '" + this->currentVar + "'");
      }
      auto& v = this->md.outputs.getVariable(this->currentVar);
      v.setAttribute(VariableDescription::depth, value, false);
      for (unsigned short i = 1; i <= value; ++i) {
        const auto vn = this->currentVar + "_" + std::to_string(i);
        this->md.registerMemberName(vn);
        this->md.registerMemberName("f_" + vn);
        this->md.registerMemberName("ff_" + vn);
      }
      ++(this->current);
    } else {
      this->throwRuntimeError("ModelDSLCommon::treatOutputMethod",
                              "Internal error (untreated method '" + mn + "')");
    }
    this->readSpecifiedToken("ModelDSLCommon::treatOutputMethod", ")");
    this->readSpecifiedToken("ModelDSLCommon::treatOutputMethod", ";");
  }  // end of treatOutputMethod

  void ModelDSLCommon::treatInputMethod() {
    if (!this->md.functions.empty()) {
      this->throwRuntimeError("ModelDSLCommon::treatInputMethod",
                              "input methods must be called "
                              "before declaring functions");
    }
    this->readSpecifiedToken("ModelDSLCommon::treatInputMethod", ".");
    this->checkNotEndOfFile("ModelDSLCommon::treatInputMethod",
                            "Expected method name.");
    const auto mn = this->current->value;
    if ((mn != "setGlossaryName") && (mn != "setEntryName") &&
        (mn != "setDepth")) {
      this->throwRuntimeError(
          "ModelDSLCommon::treatInputMethod",
          "Unknown method (valid methods for input fields are "
          "setGlossaryName, setEntryName, setDepth"
          ", read '" +
              mn + "')");
    }
    ++(this->current);
    this->readSpecifiedToken("ModelDSLCommon::treatInputMethod", "(");
    if (mn == "setGlossaryName") {
      const auto gn = this->readString("ModelDSLCommon::treatInputMethod");
      this->md.setGlossaryName(this->currentVar, gn);
    } else if (mn == "setEntryName") {
      const auto en = this->readString("ModelDSLCommon::treatInputMethod");
      this->md.setEntryName(this->currentVar, en);
    } else if (mn == "setDepth") {
      this->checkNotEndOfFile("ModelDSLCommon::treatInputMethod",
                              "Expected depth value.");
      unsigned short value;
      std::istringstream converter(this->current->value);
      converter >> value;
      if (!converter || (!converter.eof())) {
        this->throwRuntimeError(
            "ModelDSLCommon::treatInputMethod",
            "Could not read initial value of field '" + this->currentVar + "'");
      }
      auto& v = this->md.inputs.getVariable(this->currentVar);
      v.setAttribute(VariableDescription::depth, value, false);
      for (unsigned short i = 1; i <= value; ++i) {
        const auto vn = this->currentVar + "_" + std::to_string(i);
        this->md.registerMemberName(vn);
        this->md.registerMemberName("f_" + vn);
        this->md.registerMemberName("ff_" + vn);
      }
      ++(this->current);
    } else {
      this->throwRuntimeError("ModelDSLCommon::treatInputMethod",
                              "Internal error (untreated method '" + mn + "')");
    }
    this->readSpecifiedToken("ModelDSLCommon::treatInputMethod", ")");
    this->readSpecifiedToken("ModelDSLCommon::treatInputMethod", ";");
  }  // end of treatInputMethod

  void ModelDSLCommon::treatParameter() {
    auto endOfTreatment = false;
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
    while ((this->current != this->tokens.end()) && (!endOfTreatment)) {
      if (!this->isValidIdentifier(this->current->value)) {
        this->throwRuntimeError("DSLBase::handleParameter : ",
                                "variable given is not valid (read '" +
                                    this->current->value + "').");
      }
      const auto n = this->current->value;
      const auto lineNumber = this->current->line;
      VariableDescription v(type, n, 1u, lineNumber);
      ++(this->current);
      this->checkNotEndOfFile("DSLBase::handleParameter");
      auto value = this->readInitialisationValue<double>(n, false);
      if (value.first) {
        const auto op = this->overriding_parameters.find(n);
        if (op == this->overriding_parameters.end()) {
          v.setAttribute(VariableDescription::defaultValue, value.second,
                         false);
        } else {
          v.setAttribute(VariableDescription::defaultValue, op->second, false);
        }
      }
      if (this->current->value == ",") {
        ++(this->current);
      } else if (this->current->value == ";") {
        endOfTreatment = true;
        ++(this->current);
      } else {
        this->throwRuntimeError("DSLBase::handleParameter",
                                ", or ; expected after '" + n + "'");
      }
      this->md.registerMemberName(v.name);
      this->md.parameters.push_back(v);
    }
    if (!endOfTreatment) {
      --(this->current);
      this->throwRuntimeError("DSLBase::handleParameter",
                              "Expected ';' before end of file");
    }
  }  // end of treatParameter

  void ModelDSLCommon::treatLocalParameter() {
    VariableDescriptionContainer gp;
    this->readVarList(gp, false);
    for (const auto& v : gp) {
      this->md.registerMemberName(v.name);
      this->md.parameters.push_back(v);
    }
  }  // end of treatLocalParameter

  void ModelDSLCommon::treatParameterMethod() {
    auto throw_if = [this](const bool b, const std::string& m) {
      if (b) {
        this->throwRuntimeError("ModelDSLCommon::treatParameterMethod", m);
      }
    };
    this->readSpecifiedToken("ModelDSLCommon::treatParameterMethod", ".");
    this->checkNotEndOfFile("ModelDSLCommon::treatParameterMethod",
                            "Expected method name.");
    const auto mn = this->current->value;
    throw_if((mn != "setGlossaryName") && (mn != "setEntryName") &&
                 (mn != "setDefaultValue"),
             "unknown method (valid methods for local parameters are "
             "setGlossaryName, setEntryName and setDefaultValue"
             ", read " +
                 mn + ").");
    ++(this->current);
    this->readSpecifiedToken("ModelDSLCommon::treatParameterMethod", "(");
    if (mn == "setGlossaryName") {
      const auto gn = this->readString("ModelDSLCommon::treatInputMethod");
      this->md.setGlossaryName(this->currentVar, gn);
    } else if (mn == "setEntryName") {
      const auto en = this->readString("ModelDSLCommon::treatInputMethod");
      this->md.setEntryName(this->currentVar, en);
    } else if (mn == "setDefaultValue") {
      this->readDefaultValue();
    }
    this->readSpecifiedToken("ModelDSLCommon::treatParameterMethod", ")");
    this->readSpecifiedToken("ModelDSLCommon::treatParameterMethod", ";");
  }  // end of treatParameterMethod

  void ModelDSLCommon::treatConstantMaterialProperty() {
    VariableDescriptionContainer cmp;
    const auto type = [this]() -> std::string {
      const auto otype = this->readVariableTypeIfPresent();
      if (!otype) {
        return "real";
      }
      return *otype;
    }();
    if (SupportedTypes::getTypeFlag(type) != SupportedTypes::SCALAR) {
      this->throwRuntimeError(
          "DSLBase::treatConstantMaterialProperty",
          "the type'" + type + "' is not valid for material properties.");
    }
    this->readVarList(cmp, type, false);
    for (const auto& mp : cmp) {
      this->md.registerMemberName(mp.name);
      this->md.constantMaterialProperties.push_back(mp);
    }
  }  // end of treatConstantMaterialProperty

  void ModelDSLCommon::treatConstantMaterialPropertyMethod() {
    auto throw_if = [this](const bool b, const std::string& m) {
      if (b) {
        this->throwRuntimeError(
            "ModelDSLCommon::treatConstantMaterialPropertyMethod", m);
      }
    };
    this->readSpecifiedToken(
        "ModelDSLCommon::treatConstantMaterialPropertyMethod", ".");
    this->checkNotEndOfFile(
        "ModelDSLCommon::treatConstantMaterialPropertyMethod",
        "Expected method name.");
    const auto mn = this->current->value;
    ++(this->current);
    this->readSpecifiedToken(
        "ModelDSLCommon::treatConstantMaterialPropertyMethod", "(");
    if (mn == "setGlossaryName") {
      const auto gn = this->readString(
          "ModelDSLCommon::treatConstantMaterialPropertyMethod");
      this->md.setGlossaryName(this->currentVar, gn);
    } else if (mn == "setEntryName") {
      const auto en = this->readString(
          "ModelDSLCommon::treatConstantMaterialPropertyMethod");
      this->md.setEntryName(this->currentVar, en);
    } else {
      throw_if(
          true,
          "unknown method (valid methods for constant material properties are "
          "setGlossaryName and setEntryName, read " +
              mn + ").");
    }
    this->readSpecifiedToken(
        "ModelDSLCommon::treatConstantMaterialPropertyMethod", ")");
    this->readSpecifiedToken(
        "ModelDSLCommon::treatConstantMaterialPropertyMethod", ";");
  }  // end of treatConstantMaterialPropertyMethod

  void ModelDSLCommon::readDefaultValue() {
    auto throw_if = [this](const bool b, const std::string& m) {
      if (b) {
        this->throwRuntimeError("ModelDSLCommon::readDefaultValue", m);
      }
    };
    auto& v = this->md.parameters.getVariable(this->currentVar);
    this->checkNotEndOfFile("ModelDSLCommon::readDefaultValue",
                            "Expected default value.");
    if (v.type == "DoubleArray") {
      const auto values =
          this->readArrayOfDouble("ModelDSLCommon::readDefaultValue");
      v.setAttribute(VariableDescription::defaultValue, values, false);
    } else if (v.type == "StringArray") {
      const auto values =
          this->readArrayOfDouble("ModelDSLCommon::readDefaultValue");
      v.setAttribute(VariableDescription::defaultValue, values, false);
    } else if ((v.type == "double") || (v.type == "real")) {
      const auto value = this->readDouble();
      const auto op = this->overriding_parameters.find(v.name);
      if (op == this->overriding_parameters.end()) {
        v.setAttribute(VariableDescription::defaultValue, value, false);
      } else {
        v.setAttribute(VariableDescription::defaultValue, op->second, false);
      }
    } else if (v.type == "string") {
      const auto value = this->readString("ModelDSLCommon::readDefaultValue");
      v.setAttribute(VariableDescription::defaultValue, value, false);
    } else if (v.type == "bool") {
      const auto b = this->readBooleanValue("ModelDSLCommon::readDefaultValue");
      v.setAttribute(VariableDescription::defaultValue, b, false);
    } else {
      throw_if(true, "type '" + v.type + "' is not supported.");
    }
  }  // end of readDefaultValue

  void ModelDSLCommon::treatBounds() {
    const auto b = mfront::readVariableBounds(this->current, this->end());
    if (this->md.outputs.contains(b.first)) {
      this->md.outputs.getVariable(b.first).setBounds(b.second);
    } else if (this->md.inputs.contains(b.first)) {
      this->md.inputs.getVariable(b.first).setBounds(b.second);
    } else {
      this->throwRuntimeError("ModelDSLCommon::treatBounds",
                              "no variable named '" + b.first + "'");
    }
    this->readSpecifiedToken("ModelDSLCommon::treatBounds", ";");
  }  // end of treatBounds

  void ModelDSLCommon::treatPhysicalBounds() {
    const auto b = mfront::readVariableBounds(this->current, this->end());
    if (this->md.outputs.contains(b.first)) {
      this->md.outputs.getVariable(b.first).setPhysicalBounds(b.second);
    } else if (this->md.inputs.contains(b.first)) {
      this->md.inputs.getVariable(b.first).setPhysicalBounds(b.second);
    } else {
      this->throwRuntimeError("ModelDSLCommon::treatPhysicalBounds",
                              "no variable named '" + b.first + "'");
    }
    this->readSpecifiedToken("ModelDSLCommon::treatPhysicalBounds", ";");
  }  // end of treatPhysicalBounds

  void ModelDSLCommon::addMaterialLaw(const std::string& m) {
    this->md.addMaterialLaw(m);
  }  // end of addMaterialLaw

  void ModelDSLCommon::appendToIncludes(const std::string& c) {
    this->md.appendToIncludes(c);
  }  // end of appendToIncludes

  void ModelDSLCommon::appendToMembers(const std::string& c) {
    this->md.appendToMembers(c);
  }  // end of appendToMembers

  void ModelDSLCommon::appendToPrivateCode(const std::string& c) {
    this->md.appendToPrivateCode(c);
  }  // end of appendToPrivateCode

  void ModelDSLCommon::appendToSources(const std::string& c) {
    this->md.appendToSources(c);
  }  // end of appendToSources

  void ModelDSLCommon::addExternalMFrontFile(
      const std::string& f, const std::vector<std::string>& vinterfaces) {
    this->md.addExternalMFrontFile(f, vinterfaces);
  }  // end of addExternalMFrontFile

  const MaterialKnowledgeDescription&
  ModelDSLCommon::getMaterialKnowledgeDescription() const {
    return this->md;
  }  // end of getMaterialKnowledgeDescription

  /*!
   * \brief various checks on variables
   * \param[in] v  : variables
   */
  static void performPedanticChecks(const VariableDescriptionContainer& vc,
                                    const std::map<std::string, std::size_t>& f,
                                    const std::string& t) {
    auto& log = getLogStream();
    auto& glossary = tfel::glossary::Glossary::getGlossary();
    for (const auto& v : vc) {
      if (f.find(v.name) == f.end()) {
        log << "- " << t << " '" << v.name << "' is unused.\n";
      } else {
        if (v.hasAttribute(VariableDescription::depth)) {
          const auto d =
              v.getAttribute<unsigned short>(VariableDescription::depth);
          if (d > 0) {
            for (int i = 1; i <= d; i = i + 1) {
              std::string dv = v.name + '_' + std::to_string(i);
              if (f.find(dv) == f.end()) {
                log << "- " << t << " '" << dv << "' is unused.\n";
              }
            }
          }
        }
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

  void ModelDSLCommon::doPedanticChecks() const {
    auto& log = getLogStream();
    log << "\n* Pedantic checks of " << this->fd.fileName << "\n\n";
    performPedanticChecks(this->fd);

    if (this->md.functions.size() > 1) {
      log << "- multiple @Function instances can be collected into a single "
             "one \n";
    }

    // getting all used variables
    // variable names and counts
    auto members = std::map<std::string, std::size_t>{};
    // static variable names and counts
    auto smembers = std::map<std::string, std::size_t>{};
    for (const auto& df : this->md.functions) {
      for (const auto& v : df.parameters) {
        if (members.count(v) == 0) {
          members[v] = 1;
        } else {
          ++(members[v]);
        }
      }
      for (const auto& v : df.usedVariables) {
        if (members.count(v) == 0) {
          members[v] = 1;
        } else {
          ++(members[v]);
        }
      }
      for (const auto& v : df.modifiedVariables) {
        if (members.count(v) == 0) {
          members[v] = 1;
        } else {
          ++(members[v]);
        }
      }
    }

    performPedanticChecks(this->md.outputs, members, "output");
    performPedanticChecks(this->md.parameters, members, "parameter");
    performPedanticChecks(this->md.inputs, members, "input");
    log << "\n# End of pedantic checks\n";
  }  // end of doPedanticChecks

  ModelDSLCommon::~ModelDSLCommon() = default;

}  // end of namespace mfront
