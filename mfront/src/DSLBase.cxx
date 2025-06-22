/*!
 * \file   mfront/src/DSLBase.cxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   04 jun 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cctype>
#include <sstream>
#include <iterator>
#include <stdexcept>
#include <algorithm>

#ifdef MFRONT_HAVE_MADNEX
#include "Madnex/MFrontImplementation.hxx"
#endif /* MFRONT_HAVE_MADNEX */

#include "TFEL/Raise.hxx"
#include "TFEL/Glossary/Glossary.hxx"
#include "TFEL/Math/IntegerEvaluator.hxx"
#include "TFEL/UnicodeSupport/UnicodeSupport.hxx"
#include "TFEL/Utilities/Data.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"

#include "MFront/MFront.hxx"
#include "MFront/PedanticMode.hxx"
#include "MFront/SupportedTypes.hxx"
#include "MFront/DSLBase.hxx"
#include "MFront/SearchPathsHandler.hxx"
#include "MFront/DSLUtilities.hxx"
#include "MFront/MFrontUtilities.hxx"
#include "MFront/MFrontDebugMode.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MFront/MFrontMaterialPropertyInterface.hxx"
#include "MFront/StaticVariableDescription.hxx"
#include "MFront/GlobalDomainSpecificLanguageOptionsManager.hxx"
#include "MFront/MaterialPropertyDSL.hxx"

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

  const char* const DSLBase::defaultOutOfBoundsPolicyOption =
      "default_out_of_bounds_policy";
  const char* const DSLBase::runtimeModificationOfTheOutOfBoundsPolicyOption =
      "out_of_bounds_policy_runtime_modification";
  const char* const DSLBase::parametersAsStaticVariablesOption =
      "parameters_as_static_variables";
  const char* const DSLBase::initializationFromFileOption =
      "parameters_initialization_from_file";
  const char* const DSLBase::overridingParameters = "overriding_parameters";
  const char* const DSLBase::validatorOption = "validator";
  const char* const DSLBase::buildIdentifierOption = "build_identifier";
  const char* const DSLBase::disableRuntimeChecksOption =
      "disable_runtime_checks";

  bool isValidMaterialName(std::string_view n) {
    return tfel::utilities::CxxTokenizer::isValidIdentifier(n, true);
  }

  bool isValidLibraryName(std::string_view n) {
    return tfel::utilities::CxxTokenizer::isValidIdentifier(n, true);
  }

  bool isValidUserDefinedVariableName(std::string_view n) {
    return (tfel::utilities::CxxTokenizer::isValidIdentifier(n, true)) &&
           (!tfel::utilities::starts_with(n, "mfront_"));
  }  // end of isValidUserDefinedVariableName

  tfel::utilities::DataMapValidator DSLBase::getDSLOptionsValidator() {
    auto v = tfel::utilities::DataMapValidator{};
    v.addDataTypeValidator<std::string>(DSLBase::defaultOutOfBoundsPolicyOption)
        .addDataTypeValidator<bool>(
            DSLBase::runtimeModificationOfTheOutOfBoundsPolicyOption)
        .addDataTypeValidator<bool>(DSLBase::parametersAsStaticVariablesOption)
        .addDataTypeValidator<bool>(DSLBase::initializationFromFileOption)
        .addDataTypeValidator<bool>(DSLBase::disableRuntimeChecksOption)
        .addDataTypeValidator<std::string>(DSLBase::validatorOption)
        .addDataTypeValidator<std::string>(DSLBase::buildIdentifierOption)
        .addDataTypeValidator<tfel::utilities::DataMap>(
            DSLBase::overridingParameters);
    return v;
  }  // end of getDSLOptionsValidator

  DSLBase::VariableModifier::~VariableModifier() = default;

  DSLBase::WordAnalyser::~WordAnalyser() = default;

  DSLBase::CodeBlockParserOptions::CodeBlockParserOptions() = default;

  DSLBase::CodeBlockParserOptions::~CodeBlockParserOptions() noexcept = default;

  void DSLBase::handleDSLOptions(MaterialKnowledgeDescription& d,
                                 const DSLOptions& opts) {
    if (opts.count(DSLBase::disableRuntimeChecksOption) != 0) {
      const auto opt = opts.at(DSLBase::disableRuntimeChecksOption).get<bool>();
      setDisableRuntimeChecks(d, opt);
    }
    if (opts.count(DSLBase::defaultOutOfBoundsPolicyOption) != 0) {
      const auto opt =
          opts.at(DSLBase::defaultOutOfBoundsPolicyOption).get<std::string>();
      setDefaultOutOfBoundsPolicy(d, opt);
    }
    if (opts.count(DSLBase::runtimeModificationOfTheOutOfBoundsPolicyOption) !=
        0) {
      const auto b =
          opts.at(DSLBase::runtimeModificationOfTheOutOfBoundsPolicyOption)
              .get<bool>();
      d.setAttribute(MaterialKnowledgeDescription::
                         runtimeModificationOfTheOutOfBoundsPolicy,
                     b, false);
    }
    if (opts.count(DSLBase::parametersAsStaticVariablesOption) != 0) {
      const auto b =
          opts.at(DSLBase::parametersAsStaticVariablesOption).get<bool>();
      d.setAttribute(MaterialKnowledgeDescription::parametersAsStaticVariables,
                     b, false);
    }
    //
    if (opts.count(DSLBase::initializationFromFileOption) != 0) {
      const auto b = opts.at(DSLBase::initializationFromFileOption).get<bool>();
      d.setAttribute(MaterialKnowledgeDescription::initializeParametersFromFile,
                     b, false);
    }
    //
    if (opts.count(DSLBase::validatorOption) != 0) {
      const auto id = opts.at(DSLBase::validatorOption).get<std::string>();
      d.setAttribute(MaterialKnowledgeDescription::validator, id, false);
    }
    //
    if (opts.count(DSLBase::buildIdentifierOption) != 0) {
      const auto id =
          opts.at(DSLBase::buildIdentifierOption).get<std::string>();
      d.setAttribute(MaterialKnowledgeDescription::buildIdentifier, id, false);
    }
  }  // end of handleDSLOptions

  AbstractDSL::DSLOptions DSLBase::buildCommonDSLOptions(
      const MaterialKnowledgeDescription& d) {
    const auto policy = d.getAttribute<std::string>(
        MaterialKnowledgeDescription::defaultOutOfBoundsPolicy, "None");
    const auto build_id = d.getAttribute<std::string>(
        MaterialKnowledgeDescription::buildIdentifier, "");
    const auto validator = d.getAttribute<std::string>(
        MaterialKnowledgeDescription::validator, "");
    return {{DSLBase::disableRuntimeChecksOption, areRuntimeChecksDisabled(d)},
            {DSLBase::defaultOutOfBoundsPolicyOption, policy},
            {DSLBase::runtimeModificationOfTheOutOfBoundsPolicyOption,
             allowRuntimeModificationOfTheOutOfBoundsPolicy(d)},
            {DSLBase::parametersAsStaticVariablesOption,
             areParametersTreatedAsStaticVariables(d)},
            {DSLBase::initializationFromFileOption,
             allowsParametersInitializationFromFile(d)},
            {DSLBase::validatorOption, validator},
            {DSLBase::buildIdentifierOption, build_id},
            {DSLBase::overridingParameters, tfel::utilities::DataMap{}}};
  }  // end of buildCommonOptions

  DSLBase::DSLBase(const DSLOptions& /* opts */) {
    this->addSeparator("\u2297");
    this->addSeparator("\u22C5");
  }  // end of DSLBase

  std::vector<AbstractDSL::DSLOptionDescription> DSLBase::getDSLOptions()
      const {
    return {{DSLBase::disableRuntimeChecksOption,
             "boolean stating if interfaces shall remove as much runtime "
             "checks as possible. Those checks include the treatment of "
             "standard and physical bounds for instance."},
            {DSLBase::defaultOutOfBoundsPolicyOption,
             "string specifying the default out of bounds policy. Allowed "
             "values are `None`, `Warning`, `Strict`"},
            {DSLBase::runtimeModificationOfTheOutOfBoundsPolicyOption,
             "boolean stating if the runtime modification of the out of bounds "
             "policy is allowed"},
            {DSLBase::parametersAsStaticVariablesOption,
             "boolean stating if parameters shall be treated as static "
             "variables"},
            {DSLBase::initializationFromFileOption,
             "boolean stating if the values of parameters can be changed from "
             "an external file"},
            {DSLBase::buildIdentifierOption,
             "string specifying a build identifier. This option shall only be "
             "specified on the command line"},
            {DSLBase::overridingParameters,
             "map allowing to override material properties, parameters and "
             "external state variables by parameters"}};
  }  // end of getDSLOptions

  std::vector<std::string> DSLBase::getDefaultReservedNames() {
    const auto& g = tfel::glossary::Glossary::getGlossary();
    auto names = std::vector<std::string>{};
    // names of the c++ standard
    names.insert(
        names.end(),
        {"std",           "cout",    "cerr",   "endl",  "cos",    "sin",
         "tan",           "acos",    "asin",   "atan",  "atan2",  "cosh",
         "sinh",          "tanh",    "acosh",  "asinh", "atanh",  "exp",
         "frexp",         "ldexp",   "log",    "log10", "modf",   "exp2",
         "expm1",         "ilogb",   "log1p",  "log2",  "logb",   "scalbn",
         "scalbln",       "pow",     "sqrt",   "cbrt",  "hypot",  "erf",
         "erfc",          "tgamma",  "lgamma", "abs",   "string", "ofstream",
         "ostringstream", "ifstream"});
    // tfel namespaces
    names.insert(names.end(), {"tfel", "math", "material", "utilities",
                               "exception", "glossary"});
    names.insert(names.end(),
                 {"policy", "errno", "mfront_errno", "mfront_errno_old"});
    // standard aliases
    for (const auto& a : getTypeAliases()) {
      if (!g.contains(a)) {
        names.push_back(a);
      }
    }
    return names;
  }

  void DSLBase::openFile(const std::string& f,
                         const std::vector<std::string>& ecmds,
                         const std::map<std::string, std::string>& s) {
    if ((tfel::utilities::starts_with(f, "madnex:")) ||
        (tfel::utilities::starts_with(f, "mdnx:")) ||
        (tfel::utilities::starts_with(f, "edf:"))) {
#ifdef MFRONT_HAVE_MADNEX
      const auto path = decomposeImplementationPathInMadnexFile(f);
      const auto& material = std::get<2>(path);
      const auto& name = std::get<3>(path);
      const auto impl = madnex::getMFrontImplementation(
          std::get<0>(path), std::get<1>(path), material, name);
      this->overrideMaterialKnowledgeIdentifier(name);
      if (!material.empty()) {
        this->overrideMaterialName(material);
      }
      if (!impl.metadata.author.empty()) {
        this->overrideAuthorName(impl.metadata.author);
      }
      if (!impl.metadata.date.empty()) {
        this->overrideDate(impl.metadata.date);
      }
      if (!impl.metadata.description.empty()) {
        this->overrideDescription(impl.metadata.description);
      }
      if (!impl.metadata.unit_system.empty()) {
        this->overrideUnitSystem(impl.metadata.unit_system);
      }
      for (const auto& p : impl.parameters) {
        this->overrideByAParameter(p.first, p.second);
      }
      CxxTokenizer::parseString(impl.source);
#else  /* MFRONT_HAVE_MADNEX */
      tfel::raise("DSLBase::openFile: madnex support was not enabled");
#endif /* MFRONT_HAVE_MADNEX */
    } else {
      CxxTokenizer::openFile(f);
    }
    // substitutions
    const auto pe = s.end();
    for (auto& t : this->tokens) {
      if (t.flag == tfel::utilities::Token::String) {
        auto delim = t.value.at(0);
        auto contents = t.value.substr(1, t.value.size() - 2);
        for (const auto& [k, v] : s) {
          contents = tfel::utilities::replace_all(contents, k, v);
        }
        t.value = delim + contents + delim;
      } else {
        auto p = s.find(t.value);
        if (p != pe) {
          t.value = p->second;
        }
      }
    }
    // treating external commands
    for (const auto& c : ecmds) {
      CxxTokenizer t;
      try {
        t.parseString(c);
      } catch (std::exception& e) {
        tfel::raise(
            "DSLBase::openFile : "
            "error while parsing external command "
            "'" +
            c + "'\n" + std::string(e.what()));
      }
      this->tokens.insert(this->tokens.begin(), t.begin(), t.end());
    }
  }  // end of openFile

  const FileDescription& DSLBase::getFileDescription() const {
    return this->fd;
  }  // end of getFileDescription

  const TargetsDescription& DSLBase::getTargetsDescription() const {
    return this->td;
  }  // end of getTargetsDescription

  DSLBase::~DSLBase() = default;

  void DSLBase::readNextBlock(CodeBlock& res1,
                              CodeBlock& res2,
                              const CodeBlockParserOptions& o1,
                              const CodeBlockParserOptions& o2) {
    auto pb = this->current;
    res1 = this->readNextBlock(o1);
    this->current = pb;
    res2 = this->readNextBlock(o2);
  }  // end of readNextBlock

  CodeBlock DSLBase::readNextBlock(const CodeBlockParserOptions& options) {
    using tfel::utilities::Token;
    auto addSpaceBetweenToken =
        [this](std::string& r, const TokensContainer::const_iterator c,
               const TokensContainer::const_iterator n) {
          if ((n == this->tokens.end()) || (n->line != c->line)) {
            return;
          }
          const auto csize = [&c] {
            if ((c->flag == Token::String) || (c->flag == Token::Char)) {
              return c->value.size() + 2;
            }
            return c->value.size();
          }();
          if (n->offset > c->offset + csize) {
            const auto d = n->offset - csize - c->offset;
            r += std::string(d, ' ');
          }
        };
    const auto& smn = options.smn;
    const auto& mn = options.mn;
    const auto& delim1 = options.delim1;
    const auto& delim2 = options.delim2;
    const auto addThisPtr = options.qualifyMemberVariables;
    const auto addClassName = options.qualifyStaticVariables;
    const auto allowSemiColon = options.allowSemiColon;
    const auto registerLine = options.registerLine;
    auto demangle = [&options](const Token& t) {
      if (t.flag != Token::Standard) {
        return t.value;
      }
      const auto p = options.symbols.find(t.value);
      if (p != options.symbols.end()) {
        return p->second;
      }
      return tfel::unicode::getMangledString(t.value);
    };
    auto modifier = options.modifier;
    auto analyser = options.analyser;
    CodeBlock b;
    if (!this->currentComment.empty()) {
      b.description += this->currentComment;
    }
    auto& res = b.code;
    unsigned int openedBlock = 0;
    this->readSpecifiedToken("DSLBase::readNextBlock", delim1);
    this->checkNotEndOfFile("DSLBase::readNextBlock",
                            "Expected a '" + delim2 + "'.");
    if ((this->current->value == ";") && (!allowSemiColon)) {
      this->throwRuntimeError("DSLBase::readNextBlock",
                              "read ';' before the end of block.\n"
                              "Number of block opened : " +
                                  std::to_string(openedBlock));
    }
    if (this->current->value == delim1) {
      ++openedBlock;
    }
    if (this->current->value == delim2) {
      ++(this->current);
      return b;
    }
    auto currentLine = this->current->line;
    if ((registerLine) && (!getDebugMode())) {
      res = "#line ";
      res += std::to_string(currentLine);
      res += " \"";
      res += this->fd.fileName;
      res += "\"\n";
    }
    if (!this->current->comment.empty()) {
      if (!b.description.empty()) {
        b.description += '\n';
      }
      b.description += this->current->comment;
    }
    auto currentValue = demangle(*(this->current));
    if (analyser != nullptr) {
      analyser->exe(b, currentValue);
    }
    if (smn.find(currentValue) != smn.end()) {
      b.staticMembers.insert(currentValue);
      const auto previous = std::prev(this->current);
      if ((previous->value != "->") && (previous->value != ".") &&
          (previous->value != "::")) {
        if (addClassName) {
          res += this->getClassName();
          res += "::";
        }
      }
      res += currentValue;
    } else if (mn.find(currentValue) != mn.end()) {
      b.members.insert(currentValue);
      auto cv = std::string{};
      auto previous = std::prev(this->current);
      if ((previous->value == "->") || (previous->value == ".") ||
          (previous->value == "::")) {
        cv = currentValue;
      } else {
        if (modifier != nullptr) {
          cv = modifier->exe(currentValue, addThisPtr);
        } else {
          if (addThisPtr) {
            cv = "this->" + currentValue;
          } else {
            cv = currentValue;
          }
        }
      }
      previous = std::prev(this->current);
      if (previous->value == "*") {
        res += "(" + cv + ")";
      } else {
        res += cv;
      }
    } else {
      res += currentValue;
    }
    addSpaceBetweenToken(res, this->current, std::next(this->current));
    ++(this->current);
    while ((this->current != this->tokens.end()) &&
           (!((this->current->value == delim2) && (openedBlock == 0)))) {
      currentValue = demangle(*(this->current));
      if (currentLine != this->current->line) {
        currentLine = this->current->line;
        if ((registerLine) && (!getDebugMode())) {
          res += "\n";
          res += "#line ";
          res += std::to_string(currentLine);
          res += " \"";
          res += this->fd.fileName;
          res += "\"\n";
        } else {
          res += "\n";
        }
      }
      if ((currentValue == ";") && (!allowSemiColon)) {
        this->throwRuntimeError("DSLBase::readNextBlock",
                                "read ';' before the end of block.\n"
                                "Number of block opened : " +
                                    std::to_string(openedBlock));
      }
      if (!this->current->comment.empty()) {
        if (!b.description.empty()) {
          b.description += '\n';
        }
        b.description += this->current->comment;
      }
      if (analyser != nullptr) {
        analyser->exe(b, currentValue);
      }
      if (smn.find(currentValue) != smn.end()) {
        b.staticMembers.insert(currentValue);
        const auto previous = std::prev(this->current);
        if ((previous->value != "->") && (previous->value != ".") &&
            (previous->value != "::")) {
          if (addClassName) {
            res += this->getClassName();
            res += "::";
          }
        }
        res += currentValue;
      } else if (mn.find(currentValue) != mn.end()) {
        b.members.insert(currentValue);
        auto cv = std::string{};
        const auto previous = std::prev(this->current);
        if ((previous->value == "->") || (previous->value == ".") ||
            (previous->value == "::")) {
          cv = currentValue;
        } else {
          if (modifier != nullptr) {
            cv = modifier->exe(currentValue, addThisPtr);
          } else {
            if (addThisPtr) {
              if (previous->value == "*") {
                cv = "(this->" + currentValue + ')';
              } else {
                cv = "this->" + currentValue;
              }
            } else {
              cv = currentValue;
            }
          }
        }
        res += cv;
      } else {
        res += currentValue;
      }
      addSpaceBetweenToken(res, this->current, std::next(this->current));
      if (currentValue == delim1) {
        ++openedBlock;
      }
      if (currentValue == delim2) {
        --openedBlock;
      }
      ++(this->current);
    }
    if (this->current == this->tokens.end()) {
      --(this->current);
      this->throwRuntimeError("DSLBase::readNextBlock",
                              "Expected the end of a block.\n"
                              "Number of block opened : " +
                                  std::to_string(openedBlock));
    }
    ++(this->current);
    return b;
  }  // end of readNextBlock

  void DSLBase::throwRuntimeError(const std::string& m,
                                  const std::string& e) const {
    auto msg = m;
    if (!e.empty()) {
      msg += ": " + e;
    }
    if (!this->tokens.empty()) {
      auto t = this->current;
      if (t == this->tokens.end()) {
        --t;
      }
      msg += "\nError at line " + std::to_string(t->line);
    }
    tfel::raise(msg);
  }  // end of throwRuntimeError

  void DSLBase::treatImport() {
    const auto m = "DSLBase::treatImport";
    const auto oFileName = this->fd.fileName;
    this->checkNotEndOfFile(m);
    const auto& files = this->readStringOrArrayOfString(m);
    this->checkNotEndOfFile(m);
    this->readSpecifiedToken(m, ";");
    TokensContainer oFileTokens;
    oFileTokens.swap(this->tokens);
    const auto ocurrent = this->current;
    for (const auto& f : files) {
      this->importFile(SearchPathsHandler::search(f),
                       std::vector<std::string>(), {});
    }
    this->fd.fileName = oFileName;
    this->tokens.swap(oFileTokens);
    this->current = ocurrent;
  }

  void DSLBase::checkNotEndOfFile(const std::string& m,
                                  const std::string& e) const {
    if (this->current == this->tokens.end()) {
      auto msg = std::string{};
      msg += "unexpected end of file.";
      if (!e.empty()) {
        msg += "\n" + e;
      }
      if (!this->tokens.empty()) {
        const auto previous = std::prev(this->current);
        msg += "\nError at line " + std::to_string(previous->line);
      }
      this->throwRuntimeError(m, msg);
    }
  }  // end of checkNotEndOfFile

  unsigned short DSLBase::readUnsignedShort(const std::string& m) {
    this->checkNotEndOfFile(m, "Cannot read unsigned short value.");
    unsigned short value;
    std::istringstream flux(current->value);
    flux >> value;
    if ((flux.fail()) || (!flux.eof())) {
      this->throwRuntimeError(m, "Failed to read unsigned short value.");
    }
    ++(this->current);
    return value;
  }  // end of readUnsignedShort

  void DSLBase::readSpecifiedToken(const std::string& m, const std::string& v) {
    this->checkNotEndOfFile(m, "expected '" + v + "'.");
    if (this->current->value != v) {
      this->throwRuntimeError(m, "expected '" + v +
                                     "', "
                                     "read '" +
                                     this->current->value +
                                     "'.\n"
                                     "Error at line: " +
                                     std::to_string(this->current->line));
    }
    ++(this->current);
  }  // end of readSpecifiedToken

  std::string DSLBase::readUntilEndOfInstruction() {
    using tfel::utilities::Token;
    auto res = std::string{};
    while ((this->current != this->tokens.end()) &&
           (this->current->value != ";")) {
      const auto value = [this] {
        if (this->current->flag == Token::String) {
          const auto s = this->current->value.size() - 2;
          return this->current->value.substr(1, s);
        }
        return this->current->value;
      }();
      if (!value.empty()) {
        if (value[0] == '@') {
          this->throwRuntimeError(
              "DSLBase::readUntilEndOfInstruction",
              "no word beginning with '@' are allowed here");
        }
        res += value;
        res += " ";
      }
      ++(this->current);
    }
    this->checkNotEndOfFile("DSLBase::readUntilEndOfInstruction",
                            "Missing ';' delimiter.");
    if (!res.empty()) {
      res.erase(res.length() - 1);
    }
    ++(this->current);
    return res;
  }

  std::string DSLBase::readOnlyOneToken() {
    this->checkNotEndOfFile("DSLBase::readOnlyOneToken", "Expected a word.");
    if (this->current->value == ";") {
      this->throwRuntimeError("DSLBase::readOnlyOneToken", "no word read");
    }
    const auto res = this->current->value;
    ++(this->current);
    this->readSpecifiedToken("DSLBase::readOnlyOneToken", ";");
    return res;
  }  // end of readOnlyOneToken

  std::pair<std::string, VariableBoundsDescription>
  DSLBase::readVariableBounds() {
    return mfront::readVariableBounds(this->current, this->end());
  }  // end of readVariableBounds

  void DSLBase::registerIntegerConstant(const std::string& n,
                                        const size_t l,
                                        const int v) {
    if (!this->isValidIdentifier(n)) {
      this->throwRuntimeError("DSLBase::registerIntegerConstant",
                              "Variable name '" + n + "' is not valid.");
    }
    this->addStaticVariableDescription(
        StaticVariableDescription("int", n, l, v));
  }  // end of registerIntegerConstant

  void DSLBase::treatIntegerConstant() {
    this->checkNotEndOfFile("DSLBase::treatIntegerConstant",
                            "Cannot read type of static variable.");
    const auto name = this->current->value;
    const auto line = this->current->line;
    ++(this->current);
    const auto value = this->readInitialisationValue<int>(name, true);
    this->readSpecifiedToken("DSLBase::treatIntegerConstant", ";");
    this->registerIntegerConstant(name, line, value.second);
  }  // end of treatIntegerConstant

  unsigned int DSLBase::readArrayOfVariablesSize(const std::string& n,
                                                 const bool b) {
    auto throw_if = [this](const bool c, const std::string& m) {
      if (c) {
        this->throwRuntimeError("DSLBase::readArrayOfVariablesSize", m);
      }
    };
    this->checkNotEndOfFile("DSLBase::readArrayOfVariablesSize");
    unsigned int asize = 1u;
    if (this->current->value == "[") {
      throw_if(!b, "variable '" + n + "' can't be declared an array");
      auto array_size = std::string{};
      ++(this->current);
      this->checkNotEndOfFile("DSLBase::readArrayOfVariablesSize");
      while (this->current->value != "]") {
        throw_if(((this->current->flag != tfel::utilities::Token::Standard) &&
                  (this->current->flag != tfel::utilities::Token::Number)) ||
                     (this->current->value == ";"),
                 "invalid array size for '" + n + "'");
        array_size += this->current->value;
        ++(this->current);
        this->checkNotEndOfFile("DSLBase::readArrayOfVariablesSize");
      }
      throw_if(array_size.empty(), "empty array size for '" + n + "'");
      tfel::math::IntegerEvaluator ev(array_size);
      const auto& vars = ev.getVariablesNames();
      for (const auto& v : vars) {
        ev.setVariableValue(v, this->getIntegerConstant(v));
      }
      const auto iv = ev.getValue();
      throw_if(iv <= 0, "invalid array size for '" + n + "'");
      asize = static_cast<unsigned int>(iv);
      this->readSpecifiedToken("DSLBase::readArrayOfVariablesSize", "]");
      this->checkNotEndOfFile("DSLBase::readArrayOfVariablesSize");
    }
    return asize;
  }  // end of readArrayOfVariablesSize

  void DSLBase::readVarList(VariableDescriptionContainer& cont,
                            const std::string& type,
                            const bool allowArray) {
    auto throw_if = [this](const bool b, const std::string& m) {
      if (b) {
        this->throwRuntimeError("DSLBase::readVarList", m);
      }
    };
    auto endComment = std::string{};
    auto endOfTreatment = false;
    while ((this->current != this->tokens.end()) && (!endOfTreatment)) {
      const auto sname = this->current->value;
      const auto vname = tfel::unicode::getMangledString(sname);
      throw_if(!isValidUserDefinedVariableName(vname),
               "variable given is not valid (read '" + sname + "').");
      auto lineNumber = this->current->line;
      ++(this->current);
      this->checkNotEndOfFile("DSLBase::readVarList");
      const auto asize = this->readArrayOfVariablesSize(sname, allowArray);
      if (this->current->value == ",") {
        ++(this->current);
      } else if (this->current->value == ";") {
        endOfTreatment = true;
        endComment = this->current->comment;
        ++(this->current);
      } else {
        throw_if(true, "',' or ';' expected after '" + sname + "'");
      }
      if (vname == sname) {
        cont.push_back(VariableDescription(type, vname, asize, lineNumber));
      } else {
        cont.push_back(
            VariableDescription(type, sname, vname, asize, lineNumber));
      }
      if (!this->currentComment.empty()) {
        cont.back().description = this->currentComment;
      }
    }
    if (!endComment.empty()) {
      for (auto& c : cont) {
        if (!c.description.empty()) {
          c.description += ' ';
        }
        c.description += endComment;
      }
    }
    if (!endOfTreatment) {
      --(this->current);
      throw_if(true, "expected ';' before end of file");
    }
  }

  SupportedTypes::TypeParsingOptions DSLBase::getTypeParsingOptions() const {
    auto opts = SupportedTypes::TypeParsingOptions{};
    opts.integer_constants = this->getIntegerConstants();
    opts.use_qt = this->useQt();
    return opts;
  }  // end of getTypeParsingOptions

  std::optional<std::string> DSLBase::readVariableTypeIfPresent() {
    this->disableQuantitiesUsageIfNotAlreadySet();
    if (this->current->value == "::") {
      return this->readType();
    }
    const auto n = std::next(this->current);
    if (n == this->tokens.end()) {
      this->throwRuntimeError("DSLBase::readVariableTypeIfPresent",
                              "unexpected end of file");
    }
    if ((n->value == ";") || (n->value == ",") || (n->value == "=") ||
        (n->value == "[") || (n->value == "{") || (n->value == "(")) {
      return {};
    }
    return this->readType();
  }  // end of readVariableTypeIfPresent

  std::string DSLBase::readType() {
    this->disableQuantitiesUsageIfNotAlreadySet();
    return SupportedTypes::extractType(this->current, this->tokens.end(),
                                       this->getTypeParsingOptions());
  }  // end of readType

  void DSLBase::readVarList(VariableDescriptionContainer& cont,
                            const bool allowArray) {
    this->checkNotEndOfFile("DSLBase::readVarList",
                            "Cannot read type of varName.\n");
    const auto r = this->readType();
    this->readVarList(cont, r, allowArray);
  }  // end of readVarList

  std::vector<tfel::utilities::Token> DSLBase::readList(const std::string& m,
                                                        const std::string& db,
                                                        const std::string& de,
                                                        const bool b) {
    std::vector<tfel::utilities::Token> t;
    this->readList(t, m, db, de, b);
    return t;
  }  // end of readList

  void DSLBase::readList(std::vector<tfel::utilities::Token>& l,
                         const std::string& m,
                         const std::string& db,
                         const std::string& de,
                         const bool b) {
    l.clear();
    this->checkNotEndOfFile(m, "Expected '" + db + "'");
    if (this->current == this->tokens.end()) {
      if (b) {
        return;
      }
    }
    this->checkNotEndOfFile(m, "Expected '" + db + "'");
    if (this->current->value != db) {
      return;
    }
    this->readSpecifiedToken(m, db);
    this->checkNotEndOfFile(m, "Expected '" + de + "'");
    while (this->current->value != de) {
      l.push_back(*(this->current));
      ++(this->current);
      this->checkNotEndOfFile(m, "Expected '" + de + "'");
      if (!((this->current->value == de) || (this->current->value == ","))) {
        this->throwRuntimeError(m, "Expected ',' or '" + de +
                                       "',"
                                       " read '" +
                                       this->current->value + "'");
      }
      if (this->current->value == ",") {
        ++(this->current);
        this->checkNotEndOfFile(m, "Expected '" + de + "'");
        if (this->current->value == de) {
          this->throwRuntimeError(m, "Expected a new item");
        }
      }
    }
    ++(this->current);
  }  // end of readList

  std::vector<std::string> DSLBase::readArrayOfString(const std::string& m) {
    auto r = std::vector<std::string>{};
    auto as = std::vector<tfel::utilities::Token>{};
    this->readList(as, m, "{", "}", false);
    r.reserve(as.size());
    for (const auto& t : as) {
      if (t.flag != tfel::utilities::Token::String) {
        this->throwRuntimeError(m, "Expected a string");
      }
      r.push_back(t.value.substr(1, t.value.size() - 2));
    }
    return r;
  }  // end of readArrayOfString

  std::vector<double> DSLBase::readArrayOfDouble(const std::string& m) {
    auto r = std::vector<double>{};
    auto as = std::vector<tfel::utilities::Token>{};
    this->readList(as, m, "{", "}", false);
    r.reserve(as.size());
    for (const auto& t : as) {
      r.push_back(tfel::utilities::convert<double>(t.value));
    }
    return r;
  }  // end of readArrayOfDouble

  bool DSLBase::readBooleanValue(const std::string& m) {
    bool b = false;
    this->checkNotEndOfFile(m, "Expected a boolean value");
    if (this->current->value == "true") {
      b = true;
    } else if (this->current->value == "false") {
      b = false;
    } else {
      this->throwRuntimeError(m,
                              "Expected to read 'true' or 'false' "
                              "(read '" +
                                  this->current->value + "')");
    }
    ++(this->current);
    return b;
  }  // end of readBooleanValue

  std::string DSLBase::readString(const std::string& m) {
    this->checkNotEndOfFile(m, "Expected a string or '{'");
    if (this->current->flag != tfel::utilities::Token::String) {
      this->throwRuntimeError(m, "Expected a string");
    }
    const auto& r =
        this->current->value.substr(1, this->current->value.size() - 2);
    ++(this->current);
    return r;
  }  // end of readString

  std::vector<std::string> DSLBase::readStringOrArrayOfString(
      const std::string& m) {
    this->checkNotEndOfFile(m, "Expected a string or '{'");
    if (this->current->value == "{") {
      return this->readArrayOfString(m);
    }
    return {1u, this->readString(m)};
  }  // end of readStringOrArrayOfString

  void DSLBase::treatLink() {
    const auto nlink = readStringOrArrayOfString("DSLBase::treatLink");
    this->readSpecifiedToken("DSLBase::treatLink", ";");
    for (const auto& l : nlink) {
      insert_if(this->ldflags, l);
    }
  }  // end of treatLink

  void DSLBase::callMFront(const std::vector<std::string>& interfaces,
                           const std::vector<std::string>& files) {
    MFront m;
    for (const auto& i : interfaces) {
      m.setInterface(i);
    }
    for (const auto& f : files) {
      mergeTargetsDescription(this->td, m.treatFile(f), false);
    }
  }  // end of callMFront

  void DSLBase::treatMFront() {
    this->readSpecifiedToken("DSLBase::treatMfront", "{");
    const auto vfiles = this->readStringOrArrayOfString("DSLBase::treatMfront");
    auto vinterfaces = std::vector<std::string>{};
    this->checkNotEndOfFile("DSLBase::treatMfront", "Expected '}'");
    if (!((this->current->value == "}") || (this->current->value == ","))) {
      this->throwRuntimeError(
          "DSLBase::treatMfront",
          "Expected ',' or '}', read '" + this->current->value + "'");
    }
    if (this->current->value == ",") {
      ++(this->current);
      vinterfaces = this->readStringOrArrayOfString("DSLBase::treatMfront");
    }
    this->readSpecifiedToken("DSLBase::treatMfront", "}");
    this->readSpecifiedToken("DSLBase::treatMfront", ";");
    for (const auto& f : vfiles) {
      this->addExternalMFrontFile(f, vinterfaces);
    }
  }  // end of treatMfront

  std::string DSLBase::readSpecifiedValue(const std::string& file,
                                          const std::string& value) {
    std::vector<std::string> values(1, value);
    return this->readSpecifiedValues(file, values)[1];
  }  // end of readSpecifiedValue

  std::vector<std::string> DSLBase::readSpecifiedValues(
      const std::string& file,
      const std::string& value1,
      const std::string& value2) {
    return this->readSpecifiedValues(file, {value1, value2});
  }  // end of readSpecifiedValues

  std::vector<std::string> DSLBase::readSpecifiedValues(
      const std::string& file, const std::vector<std::string>& values) {
    auto throw_if = [](const bool b, const std::string& m,
                       const unsigned int l) {
      tfel::raise_if(b, "DSLBase::readSpecifiedValues : " + m +
                            "\n"
                            "Error at line " +
                            std::to_string(l));
    };
    tfel::utilities::CxxTokenizer cfile;
    auto res = std::vector<std::string>(values.size());
    cfile.openFile(file);
    cfile.stripComments();
    auto pt = cfile.begin();
    const auto pte = cfile.end();
    while (pt != pte) {
      const auto p = find(values.begin(), values.end(), pt->value);
      if (p != values.end()) {
        if (pt != cfile.begin()) {
          auto ptp = std::prev(pt);
          throw_if(ptp->value != ";",
                   "the keyword '" + *p +
                       "' does not "
                       "begin a new instruction.",
                   pt->line);
        }
        throw_if(
            std::next(pt) == pte,
            "unexepected end of file '" + file + "' (expected " + *p + ").\n",
            (--pt)->line);
        ++pt;
        const auto value = pt->value;
        throw_if(pt->value == ";",
                 "unexepected token ';' (exepected " + *p + ")", pt->line);
        throw_if(std::next(pt) == pte,
                 "unexepected end of file '" + file + "' (expected ';').\n",
                 (--pt)->line);
        ++pt;
        throw_if(pt->value != ";",
                 "unexepected token '" + pt->value + "' (exepected ';')",
                 pt->line);
        res[static_cast<std::vector<std::string>::size_type>(
            p - values.begin())] = value;
      }
      ++pt;
    }
    return res;
  }  // end of readSpecifiedValues

  std::shared_ptr<MaterialPropertyDescription>
  DSLBase::handleMaterialPropertyDescription(const std::string& f) {
    // getting informations the source file
    const auto& global_options =
        GlobalDomainSpecificLanguageOptionsManager::get();
    auto mp = MaterialPropertyDSL{
        tfel::utilities::merge(global_options.getMaterialPropertyDSLOptions(),
                               this->buildDSLOptions(), true)};
    try {
      MFrontMaterialPropertyInterface minterface;
      const auto& path = SearchPathsHandler::search(f);
      mp.setInterfaces({"mfront"});
      mp.analyseFile(path);
      const auto t = mp.getTargetsDescription();
      if (!t.specific_targets.empty()) {
        this->throwRuntimeError("DSLBase::handleMaterialPropertyDescription",
                                "error while treating file '" + f +
                                    "'.\n"
                                    "Specific targets are not supported");
      }
      const auto& mpd = mp.getMaterialPropertyDescription();
      const auto& mname = minterface.getFunctionName(mpd);
      this->reserveName(mname);
      this->reserveName(mname + "_checkBounds");
      this->reserveName(mname + "_bounds_check_status");
      this->appendToIncludes(
          "#include\"" + minterface.getHeaderFileName(mpd.material, mpd.law) +
          ".hxx\"");
      this->addMaterialLaw(mname);
      this->atds.push_back(std::move(t));
      this->addExternalMFrontFile(path, {"mfront"});
    } catch (std::exception& e) {
      this->throwRuntimeError(
          "DSLBase::handleMaterialPropertyDescription",
          "error while treating file '" + f + "'\n" + std::string(e.what()));
    } catch (...) {
      this->throwRuntimeError("DSLBase::handleMaterialPropertyDescription",
                              "error while treating file '" + f + "'");
    }
    const auto& m = mp.getMaterialPropertyDescription();
    return std::make_shared<MaterialPropertyDescription>(m);
  }  // end of handleMaterialPropertyDescription

  void DSLBase::treatMaterialLaw() {
    const auto vfiles =
        this->readStringOrArrayOfString("DSLBase::treatMaterialLaw");
    this->readSpecifiedToken("DSLBase::treatMaterialLaw", ";");
    for (const auto& f : vfiles) {
      this->handleMaterialPropertyDescription(f);
    }
  }  // end of treatMaterialLaw

  void DSLBase::treatLonelySeparator() {
    if (getPedanticMode()) {
      getLogStream() << this->fd.fileName << ":" << this->current->line << ":"
                     << this->current->offset
                     << ": warning: extra ‘;’ [-pedantic]\n";
    }
  }  // end of treatLonelySperator

  void DSLBase::overrideMaterialKnowledgeIdentifier(const std::string& i) {
    if (!this->overriden_implementation_name.empty()) {
      this->throwRuntimeError("DSLBase::overrideMaterialKnowledgeIdentifier",
                              "the implementation name is already overriden");
    }
    this->overriden_implementation_name = i;
    this->setMaterialKnowledgeIdentifier(i);
  }  // end of overrideMaterialKnowledgeIdentifier

  void DSLBase::overrideMaterialName(const std::string& m) {
    if (!this->overriden_material.empty()) {
      this->throwRuntimeError("DSLBase::overrideMaterialName",
                              "the material name is already overriden");
    }
    this->overriden_material = m;
    this->setMaterial(m);
  }  // end of overrideMaterialName

  void DSLBase::overrideAuthorName(const std::string& a) {
    if (!this->overriden_author.empty()) {
      this->throwRuntimeError("DSLBase::overrideAuthorName",
                              "the author is already overriden");
    }
    this->overriden_author = a;
    this->setAuthor(a);
  }  // end of overrideAuthorName

  void DSLBase::overrideDate(const std::string& d) {
    if (!this->overriden_date.empty()) {
      this->throwRuntimeError("DSLBase::overrideDate",
                              "the date is already overriden");
    }
    this->overriden_date = d;
    this->setDate(d);
  }  // end of overrideAuthorName

  void DSLBase::overrideDescription(const std::string& d) {
    if (!this->overriden_description.empty()) {
      this->throwRuntimeError("DSLBase::overrideDescription",
                              "the description is already overriden");
    }
    this->overriden_description = d;
    this->setDescription(d);
  }  // end of overrideAuthorName

  void DSLBase::overrideUnitSystem(const std::string& d) {
    if (!this->overriden_unit_system.empty()) {
      this->throwRuntimeError("DSLBase::overrideUnitSystem",
                              "the author is already overriden");
    }
    this->overriden_unit_system = d;
    this->setUnitSystem(d);
  }  // end of overrideUnitSystem

  void DSLBase::treatMaterial() {
    const auto& m = this->readOnlyOneToken();
    if (this->overriden_material.empty()) {
      this->setMaterial(m);
    }
  }  // end of treatMaterial

  void DSLBase::treatAuthor() {
    const auto author = this->readUntilEndOfInstruction();
    if (this->overriden_author.empty()) {
      this->setAuthor(author);
    }
  }  // end of treatAuthor

  void DSLBase::treatUnitSystem() {
    const auto& m = this->readOnlyOneToken();
    if (this->overriden_unit_system.empty()) {
      this->setUnitSystem(m);
    }
  }  // end of treatUnitSystem

  void DSLBase::setAuthor(const std::string& a) {
    if (!this->fd.authorName.empty()) {
      this->throwRuntimeError("DSLBase::setAuthor", "author already specified");
    }
    this->fd.authorName = a;
  }  // end of setAuthor

  void DSLBase::treatDate() {
    const auto date = this->readUntilEndOfInstruction();
    if (this->overriden_date.empty()) {
      this->setDate(date);
    }
  }  // end of treatDate

  void DSLBase::setDate(const std::string& d) {
    if (!this->fd.date.empty()) {
      this->throwRuntimeError("DSLBase::setDate", "date already specified");
    }
    this->fd.date = d;
  }  // end of setDate

  void DSLBase::treatDescription() {
    this->readSpecifiedToken("DSLBase::treatDescription", "{");
    this->checkNotEndOfFile("DSLBase::treatDescription");
    auto description = std::string{};
    description += "* ";
    auto currentLine = this->current->line;
    unsigned int openedBrackets = 1u;
    while ((this->current != this->tokens.end()) &&
           (!((this->current->value == "}") && (openedBrackets == 1u)))) {
      if (this->current->value == "{") {
        const auto previous = std::prev(this->current);
        if ((previous->value.size() > 0) &&
            (previous->value[previous->value.size() - 1] != '\\')) {
          ++openedBrackets;
        }
      }
      if (this->current->value == "}") {
        const auto previous = std::prev(this->current);
        if ((previous->value.size() > 0) &&
            (previous->value[previous->value.size() - 1] != '\\')) {
          --openedBrackets;
        }
      }
      if (currentLine != this->current->line) {
        while (currentLine != this->current->line) {
          description += "\n* ";
          ++currentLine;
        }
      }
      if (this->current->flag == tfel::utilities::Token::String) {
        description +=
            this->current->value.substr(1, this->current->value.size() - 2u);
      } else {
        description += this->current->value;
      }
      description += " ";
      ++(this->current);
    }
    if (this->current == this->tokens.end()) {
      --(this->current);
      this->throwRuntimeError("DSLBase::treatDescription",
                              "File ended before the end of description.");
    }
    ++(this->current);
    if (this->overriden_description.empty()) {
      this->setDescription(description);
    }
  }  // end of treatDescription

  void DSLBase::setDescription(const std::string& d) {
    if (!this->fd.description.empty()) {
      this->throwRuntimeError("DSLBase::setDescription",
                              "description already specified");
    }
    this->fd.description = d;
  }  // end of setDescription

  void DSLBase::treatUnknownKeyword() {
    --(this->current);
    this->throwRuntimeError(
        "DSLBase::treatUnknownKeyword",
        "unknown keyword (read '" + this->current->value + "')");
  }  // end of treatUnknownKeyword

  void DSLBase::treatIncludes() {
    CodeBlockParserOptions options;
    this->appendToIncludes(this->readNextBlock(options).code);
  }

  void DSLBase::treatSources() {
    CodeBlockParserOptions options;
    this->appendToSources(this->readNextBlock(options).code);
  }  // end of treatSources

  void DSLBase::treatMembers() {
    CodeBlockParserOptions options;
    options.qualifyStaticVariables = true;
    options.qualifyMemberVariables = true;
    this->appendToMembers(this->readNextBlock(options).code);
  }

  void DSLBase::treatPrivate() {
    CodeBlockParserOptions options;
    options.qualifyStaticVariables = true;
    options.qualifyMemberVariables = true;
    this->appendToPrivateCode(this->readNextBlock(options).code);
  }  // end of treatPrivate

  void DSLBase::treatDSL() {
    this->checkNotEndOfFile("DSLBase::treatDSL", "expected DSL' name");
    ++(this->current);
    this->checkNotEndOfFile("DSLBase::treatDSL", "expected ';'");
    if (this->current->value == "{") {
      const auto o = tfel::utilities::DataParsingOptions{};
      tfel::utilities::Data::read(this->current, this->tokens.end(), o);
    }
    this->readSpecifiedToken("DSLBase::treatDSL", ";");
  }  // end of treatDSL

  void DSLBase::treatStaticVar() {
    this->checkNotEndOfFile("DSLBase::treatStaticVar",
                            "Cannot read type of static variable.");
    const auto type = this->readType();
    this->checkNotEndOfFile("DSLBase::treatStaticVar",
                            "Cannot read variable name.");
    const auto sname = this->current->value;
    const auto vname = tfel::unicode::getMangledString(sname);
    if (!isValidUserDefinedVariableName(vname)) {
      this->throwRuntimeError("DSLBase::treatStaticVar",
                              "Variable name '" + sname + "' is not valid.");
    }
    const auto line = this->current->line;
    ++(this->current);
    this->checkNotEndOfFile("DSLBase::treatStaticVar",
                            "Expected to read value of variable.");
    const auto value = this->readInitialisationValue<long double>(sname, true);
    this->readSpecifiedToken("DSLBase::treatStaticVar", ";");
    this->addStaticVariableDescription(
        StaticVariableDescription(type, sname, vname, line, value.second));
  }  // end of treatStaticVar

  void DSLBase::ignoreKeyWord(const std::string& key) {
    this->checkNotEndOfFile("DSLBase::ignoreKeyWord",
                            "error while treating keyword '" + key + "' ");
    while ((this->current->value != "{") && (this->current->value != ";")) {
      ++(this->current);
      this->checkNotEndOfFile("DSLBase::ignoreKeyWord",
                              "error while treating keyword '" + key + "' ");
    }
    if (this->current->value == "{") {
      unsigned short openedBrackets = 1;
      while (!((this->current->value == "}") && (openedBrackets == 0))) {
        ++(this->current);
        this->checkNotEndOfFile("DSLBase::ignoreKeyWord",
                                "error while treating keyword '" + key + "' ");
        if (this->current->value == "{") {
          ++openedBrackets;
        }
        if (this->current->value == "}") {
          --openedBrackets;
        }
      }
      const auto next = std::next(this->current);
      if (next != this->tokens.end()) {
        if (next->value == ";") {
          current = next;
        }
      }
    }
    ++(this->current);
  }  // end of ignoreKeyWord

  double DSLBase::readDouble() {
    this->checkNotEndOfFile("DSLBase::readDouble");
    return CxxTokenizer::readDouble(this->current, this->tokens.end());
  }  // end of readDouble

  void DSLBase::completeTargetsDescription() {
    for (auto& l : this->td.libraries) {
      l.ldflags.insert(l.ldflags.end(), this->ldflags.begin(),
                       this->ldflags.end());
      l.link_libraries.insert(l.link_libraries.end(),
                              this->link_libraries.begin(),
                              this->link_libraries.end());
      l.link_directories.insert(l.link_directories.end(),
                                this->link_directories.begin(),
                                this->link_directories.end());
    }
    // merging auxiliary target description
    auto atd = TargetsDescription();
    for (const auto& t : this->atds) {
      mergeTargetsDescription(atd, t, false);
    }
    // adding dependencies to main targets
    for (const auto& al : atd.libraries) {
      for (auto& l : this->td.libraries) {
        if (l.name != al.name) {
          insert_if(l.deps, al.name);
        }
      }
      for (auto& tg : this->td.specific_targets) {
        if ((tg.first == "all") || (tg.first == "clean")) {
          continue;
        }
        insert_if(tg.second.libraries, al.name);
      }
    }
    mergeTargetsDescription(this->td, atd, false);
    this->ldflags.clear();
    this->link_libraries.clear();
    this->link_directories.clear();
    this->atds.clear();
  }  // end of completeTargetsDescription

}  // end of namespace mfront
