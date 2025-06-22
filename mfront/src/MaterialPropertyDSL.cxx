/*!
 * \file   mfront/src/MaterialPropertyDSL.cxx
 * \brief
 * \author Thomas Helfer
 * \date   10 Nov 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <string>
#include <stdexcept>
#include <sstream>
#include <cctype>
#include <algorithm>
#include <iterator>
#include <cstdlib>

#include "TFEL/Raise.hxx"
#include "TFEL/Utilities/Token.hxx"
#include "TFEL/System/System.hxx"
#include "TFEL/Glossary/Glossary.hxx"
#include "TFEL/Glossary/GlossaryEntry.hxx"
#include "TFEL/UnicodeSupport/UnicodeSupport.hxx"

#include "MFront/MFrontHeader.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MFront/DSLUtilities.hxx"
#include "MFront/MFrontUtilities.hxx"
#include "MFront/MFrontDebugMode.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MFront/DSLFactory.hxx"
#include "MFront/MaterialPropertyDSL.hxx"
#include "MFront/TargetsDescription.hxx"
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

  MaterialPropertyDSL::MaterialPropertyDSL() {
    // Call Back
    this->registerNewCallBack(";", &MaterialPropertyDSL::treatLonelySeparator);
    this->registerNewCallBack("@Link", &MaterialPropertyDSL::treatLink);
    this->registerNewCallBack("@MaterialLaw",
                              &MaterialPropertyDSL::treatMaterialLaw);
    this->registerNewCallBack("@MFront", &MaterialPropertyDSL::treatMFront);
    this->registerNewCallBack("@Material", &MaterialPropertyDSL::treatMaterial);
    this->registerNewCallBack("@Library", &MaterialPropertyDSL::treatLibrary);
    this->registerNewCallBack("@DSL", &MaterialPropertyDSL::treatParser);
    this->registerNewCallBack("@Parser", &MaterialPropertyDSL::treatParser);
    this->registerNewCallBack("@Law", &MaterialPropertyDSL::treatLaw);
    this->registerNewCallBack("@Author", &MaterialPropertyDSL::treatAuthor);
    this->registerNewCallBack("@Date", &MaterialPropertyDSL::treatDate);
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
    this->registerNewCallBack("@Output", &MaterialPropertyDSL::treatOutput);
    this->registerNewCallBack("@Function", &MaterialPropertyDSL::treatFunction);
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
  }  // end of MaterialPropertyDSL::MaterialPropertyDSL()

  AbstractDSL::DSLTarget MaterialPropertyDSL::getTargetType() const {
    return MATERIALPROPERTYDSL;
  }

  void MaterialPropertyDSL::endsInputFileProcessing() {
  }  // end of MaterialPropertyDSL::endsInputFileProcessing

  void MaterialPropertyDSL::registerNewCallBack(const std::string& keyword,
                                                const MemberFuncPtr ptr) {
    this->callBacks.insert({keyword, ptr});
    this->registredKeyWords.insert(keyword);
  }  // end of MaterialPropertyDSL::registerNewCall

  void MaterialPropertyDSL::getKeywordsList(std::vector<std::string>& k) const {
    for (const auto& c : this->callBacks) {
      k.push_back(c.first);
    }
  }

  std::string MaterialPropertyDSL::getClassName() const {
    return this->md.className;
  }  // end of MaterialPropertyDSL::getClassName

  void MaterialPropertyDSL::addStaticVariableDescription(
      const StaticVariableDescription& v) {
    this->reserveName(v.name);
    this->md.staticVars.push_back(v);
  }  // end of MaterialPropertyDSL::addStaticVariableDescription

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
  }  // end of MaterialPropertyDSL::getIntegerConstant

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
  }  // end of MaterialPropertyDSL::getDescription

  std::string MaterialPropertyDSL::getName() {
    return "MaterialLaw";
  }  // end of MaterialPropertyDSL::getName()

  void MaterialPropertyDSL::treatMaterial() {
    if (!this->md.material.empty()) {
      this->throwRuntimeError("MaterialPropertyDSL::treatMaterial",
                              "material name alreay defined");
    }
    const auto& m = this->readOnlyOneToken();
    if (!isValidMaterialName(m)) {
      this->throwRuntimeError("MaterialPropertyDSL::treatMaterial",
                              "invalid material name '" + m + "'");
    }
    this->md.material = m;
  }  // end of MaterialPropertyDSL::treatMaterial

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
  }  // end of MaterialPropertyDSL::treatConstant

  void MaterialPropertyDSL::treatParameter() {
    this->checkNotEndOfFile("MaterialPropertyDSL::treatParameter",
                            "Expected parameter name.");
    VariableDescription p("real", this->current->value, 1u,
                          this->current->line);
    if (!isValidIdentifier(p.name)) {
      this->throwRuntimeError("DSLBase::treatParameter",
                              "parameter name '" + p.name + "' is not valid.");
    }
    ++(this->current);
    const auto value = this->readInitialisationValue<double>(p.name, false);
    if (value.first) {
      p.setAttribute(VariableDescription::defaultValue, value.second, false);
    }
    this->readSpecifiedToken("MaterialPropertyDSL::treatParameter", ";");
    this->reserveName(p.name);
    this->md.parameters.push_back(p);
  }  // MaterialPropertyDSL::treatParameter

  void MaterialPropertyDSL::treatLaw() {
    auto throw_if = [this](const bool b, const std::string& m) {
      if (b) {
        this->throwRuntimeError("MaterialPropertyDSL::treatLaw", m);
      }
    };
    throw_if(!this->md.className.empty(), "law name has already been declared");
    throw_if(!this->md.law.empty(), "law name has already been declared");
    const auto& l = this->readOnlyOneToken();
    throw_if(!isValidMaterialPropertyName(l),
             "'" + l + "' is not a valid law name");
    this->md.law = l;
    this->md.className = l;
  }  // end of MaterialPropertyDSL::treatLaw

  void MaterialPropertyDSL::addInterface(const std::string& i) {
    using MLIF = mfront::MaterialPropertyInterfaceFactory;
    if (this->interfaces.find(i) == this->interfaces.end()) {
      auto& mlif = MLIF::getMaterialPropertyInterfaceFactory();
      this->interfaces.insert({i, mlif.getInterface(i)});
    }
  }  // end of MaterialPropertyDSL::addInterface

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
  }  // end of MaterialPropertyDSL::setInterface

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
  }  // end of MaterialPropertyDSL::treatInterface

  void MaterialPropertyDSL::treatFunction() {
    auto throw_if = [this](const bool b, const std::string& m) {
      if (b) {
        this->throwRuntimeError("MaterialPropertyDSL::treatFunction", m);
      }
    };
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
        if ((var == this->md.output.name) || (this->md.inputs.contains(var))) {
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
    throw_if(!this->md.f.modified, "function does not modifiy output.");
  }  // end of MaterialPropertyDSL::treatFunction()

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
      p.setAttribute(VariableDescription::defaultValue, this->readDouble(),
                     false);
      --(this->current);
    } else {
      throw_if(true, "internal error (untreated method '" + methodName + "'");
    }
    ++(this->current);
    this->readSpecifiedToken("MaterialPropertyDSL::treatMethod", ")");
    this->readSpecifiedToken("MaterialPropertyDSL::treatMethod", ";");
  }  // end of MaterialPropertyDSL::treatMethod

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
    for (const auto& i : this->interfaces) {
      i.second->getTargetsDescription(this->td, this->md);
    }
    this->completeTargetsDescription();
  }

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
    throw_if(this->interfaces.empty(), "no interface defined.");
    // creating directories
    tfel::system::systemCall::mkdir("include");
    tfel::system::systemCall::mkdir("src");
    //! generating sources du to external material properties and models
    for (const auto& em : this->externalMFrontFiles) {
      this->callMFront(em.second, {em.first});
    }
    // calling interfaces
    for (const auto& i : this->interfaces) {
      if (getVerboseMode() >= VERBOSE_LEVEL2) {
        getLogStream() << "calling interface " << i.first << '\n';
      }
      i.second->writeOutputFiles(this->md, this->fd);
    }
  }  // end of MaterialPropertyDSL::generateOutputFiles

  void MaterialPropertyDSL::treatInput() {
    VariableDescriptionContainer ninputs;
    this->readVarList(ninputs, "real", false);
    for (const auto& i : ninputs) {
      this->reserveName(i.name);
      this->md.inputs.push_back(i);
    }
  }  // end of MaterialPropertyDSL::treatInput

  void MaterialPropertyDSL::treatOutput() {
    if (!this->md.output.name.empty()) {
      this->throwRuntimeError("MaterialPropertyDSL::treatOutput",
                              "Output already defined.");
    }
    const auto s = this->readOnlyOneToken();
    const auto n = tfel::unicode::getMangledString(s);
    if (!tfel::utilities::CxxTokenizer::isValidIdentifier(n, false)) {
      this->throwRuntimeError("MaterialPropertyDSL::treatOutput",
                              "invalid output name.");
    }
    this->reserveName(n);
    if (s != n) {
      this->reserveName(s);
      this->md.output = VariableDescription{"real", s, n, 1u, 0u};
    } else {
      this->md.output = VariableDescription{"real", n, 1u, 0u};
    }
  }  // end of MaterialPropertyDSL::treatOutput

  void MaterialPropertyDSL::treatBounds() {
    const auto b = this->readVariableBounds();
    this->readSpecifiedToken("MaterialPropertyDSL::treatBounds", ";");
    auto& v = this->md.inputs.getVariable(std::get<0>(b));
    v.setBounds(std::get<1>(b));
  }  // end of MaterialPropertyDSL::treatBounds

  void MaterialPropertyDSL::treatPhysicalBounds() {
    const auto b = this->readVariableBounds();
    this->readSpecifiedToken("MaterialPropertyDSL::treatPhysicalBounds", ";");
    auto& v = this->md.inputs.getVariable(std::get<0>(b));
    v.setPhysicalBounds(std::get<1>(b));
  }  // end of MaterialPropertyDSL::treatPhysicalBounds

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
  }  // end of MaterialPropertyDSL::treatUnknownKeyword

  const MaterialPropertyDescription&
  MaterialPropertyDSL::getMaterialPropertyDescription() const {
    return this->md;
  }  // end of MaterialPropertyDSL::getMaterialPropertyDescription

  void MaterialPropertyDSL::appendToIncludes(const std::string& c) {
    this->md.appendToIncludes(c);
  }  // end of MaterialPropertyDSL::appendToIncludes

  void MaterialPropertyDSL::appendToMembers(const std::string& c) {
    this->md.appendToMembers(c);
  }  // end of MaterialPropertyDSL::appendToMembers

  void MaterialPropertyDSL::appendToPrivateCode(const std::string& c) {
    this->md.appendToPrivateCode(c);
  }  // end of MaterialPropertyDSL::appendToPrivateCode

  void MaterialPropertyDSL::appendToSources(const std::string& c) {
    this->md.appendToSources(c);
  }  // end of MaterialPropertyDSL::appendToSources

  void MaterialPropertyDSL::addMaterialLaw(const std::string& m) {
    this->md.addMaterialLaw(m);
  }  // end of MaterialPropertyDSL::addMaterialLaw

  MaterialPropertyDSL::~MaterialPropertyDSL() = default;

}  // end of namespace mfront
