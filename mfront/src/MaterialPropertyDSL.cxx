/*!
 * \file   mfront/src/MaterialPropertyDSL.cxx
 * \brief
 * \author Helfer Thomas
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

#include "TFEL/Utilities/Token.hxx"
#include "TFEL/System/System.hxx"
#include "TFEL/Glossary/Glossary.hxx"
#include "TFEL/Glossary/GlossaryEntry.hxx"

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

  void MaterialPropertyDSL::registerNewCallBack(const std::string& keyword,
                                                const MemberFuncPtr ptr) {
    using namespace std;
    this->callBacks.insert(make_pair(keyword, ptr));
    this->registredKeyWords.insert(keyword);
  }  // end of MaterialPropertyDSL::registerNewCall

  void MaterialPropertyDSL::getKeywordsList(std::vector<std::string>& k) const {
    CallBackContainer::const_iterator p;
    for (p = this->callBacks.begin(); p != this->callBacks.end(); ++p) {
      k.push_back(p->first);
    }
  }

  std::string MaterialPropertyDSL::getClassName() const {
    return this->className;
  }  // end of MaterialPropertyDSL::getClassName

  void MaterialPropertyDSL::addStaticVariableDescription(
      const StaticVariableDescription& v) {
    this->reserveName(v.name);
    this->staticVars.push_back(v);
  }  // end of MaterialPropertyDSL::addStaticVariableDescription

  std::string MaterialPropertyDSL::getDescription() {
    using MLIF = mfront::MaterialPropertyInterfaceFactory;
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
  }  // end of MaterialPropertyDSL::getName(void)

  void MaterialPropertyDSL::treatMaterial() {
    if (!this->material.empty()) {
      this->throwRuntimeError("MaterialPropertyDSL::treatMaterial",
                              "material name alreay defined");
    }
    this->material = this->readOnlyOneToken();
    if (!CxxTokenizer::isValidIdentifier(this->material, true)) {
      this->throwRuntimeError("MaterialPropertyDSL::treatMaterial",
                              "invalid material name '" + this->material + "'");
    }
  }  // end of MaterialPropertyDSL::treatMaterial

  void MaterialPropertyDSL::treatLibrary() {
    if (!this->library.empty()) {
      this->throwRuntimeError("MaterialPropertyDSL::treatLibrary",
                              "material name alreay defined");
    }
    const auto& l = this->readOnlyOneToken();
    if (!CxxTokenizer::isValidIdentifier(l, true)) {
      this->throwRuntimeError("MaterialPropertyDSL::treatLibrary",
                              "invalid library name '" + l + "'");
    }
    this->library = l;
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
    const auto p = this->current->value;
    if (!isValidIdentifier(p)) {
      this->throwRuntimeError("DSLBase::treatParameter",
                              "parameter name '" + p + "' is not valid.");
    }
    ++(this->current);
    const auto value = this->readInitialisationValue<double>(p, false);
    if (value.first) {
      this->parametersValues.insert({p, value.second});
    }
    this->readSpecifiedToken("MaterialPropertyDSL::treatParameter", ";");
    this->reserveName(p);
    this->parameters.push_back(p);
  }  // MaterialPropertyDSL::treatParameter

  void MaterialPropertyDSL::treatLaw() {
    if (!this->className.empty()) {
      this->throwRuntimeError("MaterialPropertyDSL::treatLaw",
                              "Law name has already been declared.");
    }
    if (!this->law.empty()) {
      this->throwRuntimeError("MaterialPropertyDSL::treatLaw",
                              "Law name has already been declared.");
    }
    this->law = this->readOnlyOneToken();
    this->className = this->law;
    if (!isValidIdentifier(this->className)) {
      --(this->current);
      this->throwRuntimeError("MaterialPropertyDSL::treatLaw",
                              this->className + "is not a valid law name");
    }
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
    using namespace std;
    unsigned int openedBrackets = 0;
    unsigned int openedParenthesis = 0;
    unsigned int currentLine;
    bool newLine;
    bool newInstruction;
    bool treated;
    if (!this->f.body.empty()) {
      this->throwRuntimeError("MaterialPropertyDSL::treatFunction",
                              "function already defined");
    }
    if (this->output.empty()) {
      this->reserveName("res");
      this->output = "res";
    }
    this->f.modified = false;
    this->f.line = this->current->line;
    this->readSpecifiedToken("MaterialPropertyDSL::treatFunction", "{");
    // a simple check to see if this function is not empty
    this->checkNotEndOfFile("MaterialPropertyDSL::treatFunction");
    if (this->current->value == "}") {
      this->throwRuntimeError("MaterialPropertyDSL::treatFunction",
                              "Function is empty.");
    }
    --(this->current);
    ++openedBrackets;
    newInstruction = true;
    ++(this->current);
    this->checkNotEndOfFile("MaterialPropertyDSL::treatFunction",
                            "Expected body of function.");
    currentLine = this->current->line;
    newLine = true;
    if (!getDebugMode()) {
      this->f.body +=
          "#line " + to_string(currentLine) + " \"" + this->fileName + "\"\n";
    }
    for (; (this->current != this->tokens.end()) && (openedBrackets != 0);
         ++(this->current)) {
      if (this->current->line != currentLine) {
        currentLine = this->current->line;
        f.body += "\n";
        if (!getDebugMode()) {
          this->f.body += "#line " + to_string(currentLine) + " \"" +
                          this->fileName + "\"\n";
        }
        newLine = true;
      }
      if (this->current->value == "{") {
        ++openedBrackets;
        this->f.body += "{";
        newInstruction = true;
      } else if (this->current->value == "}") {
        --openedBrackets;
        if (openedBrackets != 0) {
          this->f.body += "}";
        }
      } else if (this->current->value == "(") {
        ++openedParenthesis;
        this->f.body += "(";
      } else if (this->current->value == ")") {
        if (openedParenthesis == 0) {
          this->throwRuntimeError("MaterialPropertyDSL::treatFunction",
                                  "unbalanced parenthesis");
        }
        --openedParenthesis;
        this->f.body += ")";
      } else if (this->current->value == ";") {
        this->f.body += ";";
        newInstruction = true;
      } else {
        if (!newLine) {
          this->f.body += " ";
        }
        if ((this->current->value == output) ||
            (this->inputs.contains(this->current->value))) {
          treated = false;
          if (newInstruction) {
            const auto var = this->current->value;
            ++(this->current);
            if (this->current == tokens.end()) {
              this->throwRuntimeError("MaterialPropertyDSL::treatFunction",
                                      "unexpected end of file while reading "
                                      "body of function ");
            }
            if ((this->current->value == "=") ||
                (this->current->value == "+=") ||
                (this->current->value == "-=") ||
                (this->current->value == "*=") ||
                (this->current->value == "/=")) {
              if (var != this->output) {
                this->throwRuntimeError(
                    "MaterialPropertyDSL::treatFunction",
                    "Trying to modify variable '" + var + "'.\n");
              }
              this->f.modified = true;
              this->f.body += var + " " + this->current->value + " ";
              treated = true;
            } else {
              --(this->current);
            }
          }
          if (!treated) {
            this->f.body += this->current->value;
          }
        } else {
          this->f.body += this->current->value;
        }
        newInstruction = false;
      }
      newLine = false;
    }
    if ((this->current == tokens.end()) && (openedBrackets != 0)) {
      this->throwRuntimeError(
          "MaterialPropertyDSL::treatFunction",
          "unexpected end of file while reading body of function");
    }
    if (openedBrackets != 0) {
      this->throwRuntimeError(
          "MaterialPropertyDSL::treatFunction",
          "parenthesis still opened at the end of function");
    }
    if (this->f.body.empty()) {
      this->throwRuntimeError("MaterialPropertyDSL::treatFunction",
                              "empty function");
    }
    if (!this->f.modified) {
      this->throwRuntimeError("MaterialPropertyDSL::treatFunction",
                              "Function does not modifiy output.");
    }
  }  // end of MaterialPropertyDSL::treatFunction(void)

  static void MFrontLawParserCheckIfNameIsAnEntryNameOrAGlossaryName(
      const std::map<std::string, std::string>& g,
      const std::map<std::string, std::string>& e,
      const std::string& n) {
    for (const auto& gn : g) {
      if (gn.second == n) {
        throw(std::runtime_error(
            "MFrontLawParserCheckIfNameIsAnEntryNameOrAGlossaryName : "
            "name '" +
            n + "' is already used as a glossary name"));
      }
    }
    for (const auto& en : e) {
      if (en.second == n) {
        throw(std::runtime_error(
            "MFrontLawParserCheckIfNameIsAnEntryNameOrAGlossaryName : "
            "name '" +
            n + "' is already used as a entry name"));
      }
    }
  }

  void MaterialPropertyDSL::treatMethod() {
    using namespace std;
    using namespace tfel::utilities;
    using namespace tfel::glossary;
    this->readSpecifiedToken("MaterialPropertyDSL::treatMethod", ".");
    this->checkNotEndOfFile("MaterialPropertyDSL::treatMethod",
                            "Expected method name.");
    const auto methodName = this->current->value;
    if ((methodName != "setGlossaryName") && (methodName != "setEntryName") &&
        (methodName != "setDefaultValue")) {
      this->throwRuntimeError("MaterialPropertyDSL::treatMethod",
                              "unknown method '" + methodName +
                                  "' "
                                  "valid methods are 'setGlossaryName', "
                                  "'setEntryName' and 'setDefaultValue'");
    }
    ++(this->current);
    this->readSpecifiedToken("MaterialPropertyDSL::treatMethod", "(");
    if (methodName == "setGlossaryName") {
      const auto& glossary = Glossary::getGlossary();
      this->checkNotEndOfFile("MaterialPropertyDSL::treatMethod",
                              "Expected glossary name.");
      if ((this->glossaryNames.find(this->currentVar) !=
           this->glossaryNames.end()) ||
          (this->entryNames.find(this->currentVar) != this->entryNames.end())) {
        this->throwRuntimeError("MaterialPropertyDSL::treatMethod",
                                "A glossary or an entry name has already "
                                "been defined for field '" +
                                    this->currentVar + "'");
      }
      if (this->current->flag != Token::String) {
        this->throwRuntimeError("MaterialPropertyDSL::treatMethod",
                                "Expected a string as glossary name.");
      }
      if (this->current->value.size() < 3) {
        this->throwRuntimeError("MaterialPropertyDSL::treatMethod",
                                "Glossary name too short.");
      }
      const auto glossaryName =
          this->current->value.substr(1, this->current->value.size() - 2);
      MFrontLawParserCheckIfNameIsAnEntryNameOrAGlossaryName(
          this->glossaryNames, this->entryNames, glossaryName);
      if (!glossary.contains(glossaryName)) {
        throw(
            runtime_error("MaterialPropertyDSL::treatMethod : "
                          "'" +
                          glossaryName + "' is not a valid glossary name"));
      }
      const auto& k = glossary.getGlossaryEntry(glossaryName).getKey();
      if (!this->glossaryNames.insert({this->currentVar, k}).second) {
        this->throwRuntimeError("MaterialPropertyDSL::treatMethod",
                                "glossary name for field '" + this->currentVar +
                                    "' already defined.");
      }
    } else if (methodName == "setEntryName") {
      const auto& glossary = Glossary::getGlossary();
      this->checkNotEndOfFile("MaterialPropertyDSL::treatMethod",
                              "Expected entry file name.");
      if ((this->glossaryNames.find(this->currentVar) !=
           this->glossaryNames.end()) ||
          (this->entryNames.find(this->currentVar) != this->entryNames.end())) {
        this->throwRuntimeError("MaterialPropertyDSL::treatMethod",
                                "A glossary or an entry name has already been "
                                "defined for field '" +
                                    this->currentVar + "'");
      }
      if (this->current->flag != Token::String) {
        this->throwRuntimeError("MaterialPropertyDSL::treatMethod",
                                "Expected a string as entry file name.");
      }
      if (this->current->value.size() < 3) {
        this->throwRuntimeError("MaterialPropertyDSL::treatMethod",
                                "Entry file name too short.");
      }
      const auto entryName =
          this->current->value.substr(1, this->current->value.size() - 2);
      MFrontLawParserCheckIfNameIsAnEntryNameOrAGlossaryName(
          this->glossaryNames, this->entryNames, entryName);
      if (glossary.contains(entryName)) {
        std::ostringstream msg;
        msg << "MaterialPropertyDSL::treatMethod : "
            << "'" << entryName << "' is a glossary name. Please use "
            << "the 'setGlossaryName' method or choose another entry name.";
        displayGlossaryEntryCompleteDescription(
            msg, glossary.getGlossaryEntry(entryName));
        throw(runtime_error(msg.str()));
      }
      if (!CxxTokenizer::isValidIdentifier(entryName)) {
        this->throwRuntimeError("MaterialPropertyDSL::treatMethod",
                                "invalid entry name '" + entryName + "'");
      }
      if (entryName != this->currentVar) {
        this->reserveName(entryName);
      }
      if (!this->entryNames.insert({this->currentVar, entryName}).second) {
        this->throwRuntimeError("MaterialPropertyDSL::treatMethod",
                                "Entry file name for field '" +
                                    this->currentVar + "' already defined.");
      }
    } else if (methodName == "setDefaultValue") {
      if (find(this->parameters.begin(), this->parameters.end(),
               this->currentVar) == this->parameters.end()) {
        this->throwRuntimeError(
            "MaterialPropertyDSL::treatMethod",
            "method setDefaultValue is reserved for paramaters.");
      }
      this->checkNotEndOfFile(
          "MaterialPropertyDSL::treatMethod",
          "Expected to read value of variable '" + this->currentVar + "'");
      const auto value = this->readDouble();
      if (!this->parametersValues.insert(make_pair(this->currentVar, value))
               .second) {
        this->throwRuntimeError("MaterialPropertyDSL::treatMethod",
                                "default value already defined for variable '" +
                                    this->currentVar + "'.");
      }
      --(this->current);
    } else {
      this->throwRuntimeError(
          "MaterialPropertyDSL::treatMethod",
          "Internal error (untreated method '" + methodName + "'");
    }
    ++(this->current);
    this->readSpecifiedToken("MaterialPropertyDSL::treatMethod", ")");
    this->readSpecifiedToken("MaterialPropertyDSL::treatMethod", ";");
  }  // end of MaterialPropertyDSL::treatMethod

  void MaterialPropertyDSL::importFile(
      const std::string& fileName_,
      const std::vector<std::string>& ecmds,
      const std::map<std::string, std::string>& s) {
    this->fileName = fileName_;
    this->openFile(this->fileName, ecmds, s);
    this->analyse();
  }

  void MaterialPropertyDSL::analyseString(const std::string& s) {
    this->fileName = "user defined string";
    this->parseString(s);
    this->analyse();
  }

  void MaterialPropertyDSL::analyse() {
    auto throw_if = [](const bool b, const std::string& m) {
      if (b) {
        throw(std::runtime_error("MaterialPropertyDSL::analyse: " + m));
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
        if (this->output == this->current->value) {
          this->currentVar = this->output;
          handler = &MaterialPropertyDSL::treatMethod;
        }
        for (auto p2 = this->inputs.begin();
             (p2 != this->inputs.end()) && (handler == nullptr);) {
          if (p2->name == this->current->value) {
            this->currentVar = this->current->value;
            handler = &MaterialPropertyDSL::treatMethod;
          } else {
            ++p2;
          }
        }
        for (auto p3 = this->parameters.begin();
             (p3 != this->parameters.end()) && (handler == nullptr);) {
          if (*p3 == this->current->value) {
            this->currentVar = this->current->value;
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
      i.second->getTargetsDescription(this->td, *this);
    }
    this->completeTargetsDescription();
  }

  void MaterialPropertyDSL::reserveName(const std::string& n) {
    if (!this->reservedNames.insert(n).second) {
      this->throwRuntimeError("MaterialPropertyDSL::reserveName",
                              "name '" + n + "' already reserved");
    }
  }

  bool MaterialPropertyDSL::isNameReserved(const std::string& n) const {
    return this->reservedNames.count(n) != 0;
  }

  void MaterialPropertyDSL::generateOutputFiles() {
    using namespace tfel::system;
    auto throw_if = [](const bool b, const std::string& m) {
      if (b) {
        throw(std::runtime_error("MaterialPropertyDSL::generateOutputFiles: " +
                                 m));
      }
    };
    throw_if(this->className.empty(), "no material property name defined.");
    throw_if(this->f.body.empty(), "no function defined.");
    throw_if(this->interfaces.empty(), "no interface defined.");
    // creating directories
    systemCall::mkdir("include");
    systemCall::mkdir("src");
    // calling interfaces
    for (const auto& i : this->interfaces) {
      if (getVerboseMode() >= VERBOSE_LEVEL2) {
        getLogStream() << "calling interface " << i.first << '\n';
      }
      i.second->writeOutputFiles(*this, *this);
    }
  }  // end of MaterialPropertyDSL::generateOutputFiles

  void MaterialPropertyDSL::treatInput() {
    VariableDescriptionContainer ninputs;
    this->readVarList(ninputs, "real", false);
    for (const auto& i : ninputs) {
      this->reserveName(i.name);
      this->inputs.push_back(i);
    }
  }  // end of MaterialPropertyDSL::treatInput

  void MaterialPropertyDSL::treatOutput() {
    if (!this->output.empty()) {
      this->throwRuntimeError("MaterialPropertyDSL::treatOutput",
                              "Output already defined.");
    }
    this->output = this->readOnlyOneToken();
    this->reserveName(this->output);
  }  // end of MaterialPropertyDSL::treatOutput

  void MaterialPropertyDSL::treatBounds() {
    this->registerBounds(this->bounds);
  }  // end of MaterialPropertyDSL::treatBounds

  void MaterialPropertyDSL::treatPhysicalBounds() {
    this->registerBounds(this->physicalBounds);
  }  // end of MaterialPropertyDSL::treatPhysicalBounds

  void MaterialPropertyDSL::registerBounds(
      std::vector<VariableBoundsDescription>& container) {
    using namespace std;
    VariableDescriptionContainer::const_iterator p;
    vector<VariableBoundsDescription>::const_iterator p3;
    VariableBoundsDescription boundsDescription;
    unsigned short i;
    bool found;
    this->checkNotEndOfFile("MaterialPropertyDSL::registerBounds");
    boundsDescription.lineNumber = this->current->line;
    boundsDescription.varName = this->current->value;
    found = false;
    for (i = 1u, p = this->inputs.begin();
         (p != this->inputs.end()) && (!found); ++p, ++i) {
      if (p->name == boundsDescription.varName) {
        found = true;
        boundsDescription.varNbr = i;
      }
    }
    if (!found) {
      this->throwRuntimeError(
          "MaterialPropertyDSL::registerBounds",
          "'" + this->current->value + "' is not a valid identifier.");
    }
    found = false;
    for (p3 = container.begin(); (p3 != container.end()) && (!found); ++p3) {
      if (p3->varName == boundsDescription.varName) {
        found = true;
      }
    }
    if (found) {
      this->throwRuntimeError("MaterialPropertyDSL::registerBounds",
                              "bounds for variable '" + this->current->value +
                                  "' already declared.");
    }
    ++(this->current);
    this->readSpecifiedToken("MaterialPropertyDSL::registerBounds", "in");
    this->checkNotEndOfFile("MaterialPropertyDSL::registerBounds ",
                            "Expected ']' or '['.");
    if (this->current->value == "]") {
      ++(this->current);
      this->checkNotEndOfFile("MaterialPropertyDSL::registerBounds ",
                              "Expected '*'.");
      if (this->current->value != "*") {
        this->throwRuntimeError(
            "MaterialPropertyDSL::registerBounds",
            "Expected '*' (read '" + this->current->value + "')");
      }
      boundsDescription.boundsType = VariableBoundsDescription::Upper;
      ++(this->current);
    } else if (this->current->value == "[") {
      ++(this->current);
      this->checkNotEndOfFile("MaterialPropertyDSL::registerBounds ",
                              "Expected lower bound value for variable " +
                                  boundsDescription.varName);
      boundsDescription.lowerBound = this->readDouble();
      boundsDescription.boundsType = VariableBoundsDescription::LowerAndUpper;
    } else {
      this->throwRuntimeError(
          "MaterialPropertyDSL::registerBounds",
          "Expected ']' or '[' (read '" + this->current->value + "')");
    }
    this->readSpecifiedToken("MaterialPropertyDSL::registerBounds", ":");
    this->checkNotEndOfFile("MaterialPropertyDSL::registerBounds",
                            "Could not read upper bound value for variable " +
                                boundsDescription.varName);
    if (this->current->value == "*") {
      if (boundsDescription.boundsType == VariableBoundsDescription::Upper) {
        this->throwRuntimeError("MaterialPropertyDSL::registerBounds",
                                "Upper and lower values bounds are both "
                                "infinity. This is inconsistent.");
      }
      boundsDescription.boundsType = VariableBoundsDescription::Lower;
      ++(this->current);
      this->checkNotEndOfFile("MaterialPropertyDSL::registerBounds",
                              "Expected '['.");
      if (this->current->value != "[") {
        this->throwRuntimeError(
            "MaterialPropertyDSL::registerBounds",
            "Expected '[' (read '" + this->current->value + "')");
      }
    } else {
      boundsDescription.upperBound = this->readDouble();
      if (boundsDescription.boundsType ==
          VariableBoundsDescription::LowerAndUpper) {
        if (boundsDescription.lowerBound > boundsDescription.upperBound) {
          this->throwRuntimeError("MaterialPropertyDSL::registerBounds",
                                  "Lower bound value is greater than upper "
                                  "bound value for variable '" +
                                      boundsDescription.varName + "'");
        }
      }
      this->checkNotEndOfFile("MaterialPropertyDSL::registerBounds",
                              "Expected ']'");
      if (this->current->value != "]") {
        this->throwRuntimeError(
            "MaterialPropertyDSL::registerBounds",
            "Expected ']' (read '" + this->current->value + "'");
      }
    }
    ++(this->current);
    this->readSpecifiedToken("MaterialPropertyDSL::registerBounds", ";");
    container.push_back(boundsDescription);
  }  // end of MaterialPropertyDSL::registerBounds

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
        auto p = i.second->treatKeyword(key, {i.first}, this->current,
                                        this->tokens.end());
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
    return *this;
  }  // end of MaterialPropertyDSL::getMaterialPropertyDescription

  void MaterialPropertyDSL::appendToIncludes(const std::string& c) {
    this->includes += c;
    if (!this->includes.empty()) {
      if (*(this->includes.rbegin()) != '\n') {
        this->includes += '\n';
      }
    }
  }  // end of MaterialPropertyDSL::appendToIncludes

  void MaterialPropertyDSL::appendToMembers(const std::string& c) {
    this->members += c;
    if (!this->members.empty()) {
      if (*(this->members.rbegin()) != '\n') {
        this->members += '\n';
      }
    }
  }  // end of MaterialPropertyDSL::appendToMembers

  void MaterialPropertyDSL::appendToPrivateCode(const std::string& c) {
    this->privateCode += c;
    if (!this->privateCode.empty()) {
      if (*(this->privateCode.rbegin()) != '\n') {
        this->privateCode += '\n';
      }
    }
  }  // end of MaterialPropertyDSL::appendToPrivateCode

  void MaterialPropertyDSL::appendToSources(const std::string& c) {
    this->sources += c;
    if (!this->sources.empty()) {
      if (*(this->sources.rbegin()) != '\n') {
        this->sources += '\n';
      }
    }
  }  // end of MaterialPropertyDSL::appendToSources

  void MaterialPropertyDSL::addMaterialLaw(const std::string& m) {
    if (std::find(this->materialLaws.begin(), this->materialLaws.end(), m) ==
        this->materialLaws.end()) {
      this->materialLaws.push_back(m);
    }
  }  // end of MaterialPropertyDSL::addMaterialLaw

  MaterialPropertyDSL::~MaterialPropertyDSL() = default;

}  // end of namespace mfront
