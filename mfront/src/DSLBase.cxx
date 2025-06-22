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
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <algorithm>

#include "TFEL/Raise.hxx"
#include "TFEL/Glossary/Glossary.hxx"
#include "TFEL/Math/IntegerEvaluator.hxx"
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

  static bool isInteger(const std::string& s) {
    const auto s2 = [&s]() -> std::string {
      if (s.empty()) {
        return s;
      }
      if ((s.back() == 'u') || (s.back() == 'U') || (s.back() == 'l') ||
          (s.back() == 'L')) {
        return s.substr(0, s.size() - 1);
      } else if ((tfel::utilities::ends_with(s, "ul")) ||
                 (tfel::utilities::ends_with(s, "uL")) ||
                 (tfel::utilities::ends_with(s, "lu")) ||
                 (tfel::utilities::ends_with(s, "Lu")) ||
                 (tfel::utilities::ends_with(s, "Ul")) ||
                 (tfel::utilities::ends_with(s, "UL")) ||
                 (tfel::utilities::ends_with(s, "lU")) ||
                 (tfel::utilities::ends_with(s, "LU"))) {
        return s.substr(0, s.size() - 2);
      }
      return s;
    }();
    if (s2.empty()) {
      return false;
    }
    for (const auto c : s2) {
      if (!std::isdigit(c)) {
        return false;
      }
    }
    return true;
  }

  DSLBase::VariableModifier::~VariableModifier() = default;

  DSLBase::WordAnalyser::~WordAnalyser() = default;

  DSLBase::CodeBlockParserOptions::CodeBlockParserOptions() = default;

  DSLBase::CodeBlockParserOptions::~CodeBlockParserOptions() noexcept = default;

  DSLBase::DSLBase() = default;

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
    for (const auto& v : SupportedTypes::getTypeFlags()) {
      if (!g.contains(v.first)) {
        names.push_back(v.first);
      }
    }
    names.insert(names.end(),
                 {"policy", "errno", "mfront_errno", "mfront_errno_old"});
    return names;
  }

  std::string DSLBase::getTemporaryVariableName(
      std::vector<std::string>& tmpnames, const std::string& p) const {
    if (!this->isValidIdentifier(p)) {
      this->throwRuntimeError("DSLBase::getTemporaryVariableName",
                              "invalid variable prefix '" + p + "'");
    }
    for (size_type i = 0; i != std::numeric_limits<size_type>::max(); ++i) {
      const auto c = p + std::to_string(i);
      if (!this->isNameReserved(c)) {
        if (std::find(tmpnames.begin(), tmpnames.end(), c) == tmpnames.end()) {
          tmpnames.push_back(c);
          return c;
        }
      }
    }
    this->throwRuntimeError("DSLBase::getTemporaryVariableName",
                            "unable to find a temporary variable");
  }

  void DSLBase::openFile(const std::string& f,
                         const std::vector<std::string>& ecmds,
                         const std::map<std::string, std::string>& s) {
    CxxTokenizer::openFile(f);
    // substitutions
    const auto pe = s.end();
    for (auto& t : this->tokens) {
      if (t.flag == tfel::utilities::Token::String) {
        auto delim = t.value.at(0);
        auto contents = t.value.substr(1, t.value.size() - 2);
        for (const auto& kv : s) {
          contents =
              tfel::utilities::replace_all(contents, kv.first, kv.second);
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
  }  // end of DSLBase::openFile

  const FileDescription& DSLBase::getFileDescription() const {
    return this->fd;
  }  // end of DSLBase::getFileDescription

  const TargetsDescription& DSLBase::getTargetsDescription() const {
    return this->td;
  }  // end of DSLBase::getTargetsDescription

  DSLBase::~DSLBase() = default;

  void DSLBase::readNextBlock(CodeBlock& res1,
                              CodeBlock& res2,
                              const CodeBlockParserOptions& o1,
                              const CodeBlockParserOptions& o2) {
    auto pb = this->current;
    res1 = this->readNextBlock(o1);
    this->current = pb;
    res2 = this->readNextBlock(o2);
  }  // end of DSLBase::readNextBlock

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
    if (analyser != nullptr) {
      analyser->exe(this->current->value);
    }
    if (smn.find(this->current->value) != smn.end()) {
      b.staticMembers.insert(this->current->value);
      const auto previous = std::prev(this->current);
      if ((previous->value != "->") && (previous->value != ".") &&
          (previous->value != "::")) {
        if (addClassName) {
          res += this->getClassName();
          res += "::";
        }
      }
      res += this->current->value;
    } else if (mn.find(this->current->value) != mn.end()) {
      b.members.insert(this->current->value);
      auto currentValue = std::string{};
      auto previous = std::prev(this->current);
      if ((previous->value == "->") || (previous->value == ".") ||
          (previous->value == "::")) {
        currentValue = this->current->value;
      } else {
        if (modifier != nullptr) {
          currentValue = modifier->exe(this->current->value, addThisPtr);
        } else {
          if (addThisPtr) {
            currentValue = "this->" + this->current->value;
          } else {
            currentValue = this->current->value;
          }
        }
      }
      previous = std::prev(this->current);
      if (previous->value == "*") {
        res += "(" + currentValue + ")";
      } else {
        res += currentValue;
      }
    } else {
      res += this->current->value;
    }
    addSpaceBetweenToken(res, this->current, std::next(this->current));
    ++(this->current);
    while ((this->current != this->tokens.end()) &&
           (!((this->current->value == delim2) && (openedBlock == 0)))) {
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
      if ((this->current->value == ";") && (!allowSemiColon)) {
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
        analyser->exe(this->current->value);
      }
      if (smn.find(this->current->value) != smn.end()) {
        b.staticMembers.insert(this->current->value);
        const auto previous = std::prev(this->current);
        if ((previous->value != "->") && (previous->value != ".") &&
            (previous->value != "::")) {
          if (addClassName) {
            res += this->getClassName();
            res += "::";
          }
        }
        res += this->current->value;
      } else if (mn.find(this->current->value) != mn.end()) {
        b.members.insert(this->current->value);
        auto currentValue = std::string{};
        const auto previous = std::prev(this->current);
        if ((previous->value == "->") || (previous->value == ".") ||
            (previous->value == "::")) {
          currentValue = this->current->value;
        } else {
          if (modifier != nullptr) {
            currentValue = modifier->exe(this->current->value, addThisPtr);
          } else {
            if (addThisPtr) {
              if (previous->value == "*") {
                currentValue = "(this->" + this->current->value + ')';
              } else {
                currentValue = "this->" + this->current->value;
              }
            } else {
              currentValue = this->current->value;
            }
          }
        }
        res += currentValue;
      } else {
        res += this->current->value;
      }
      addSpaceBetweenToken(res, this->current, std::next(this->current));
      if (this->current->value == delim1) {
        ++openedBlock;
      }
      if (this->current->value == delim2) {
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
  }  // end of DSLBase::readNextBlock

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
  }  // end of DSLBase::throwRuntimeError

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
  }  // end of DSLBase::checkNotEndOfFile

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
  }  // end of DSLBase::readUnsignedShort

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
  }  // end of DSLBase::readSpecifiedToken

  std::string DSLBase::readUntilEndOfInstruction() {
    auto res = std::string{};
    while ((this->current != this->tokens.end()) &&
           (this->current->value != ";")) {
      if (!this->current->value.empty()) {
        if (this->current->value[0] == '@') {
          this->throwRuntimeError(
              "DSLBase::readUntilEndOfInstruction",
              "no word beginning with '@' are allowed here");
        }
        res += this->current->value;
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
  }  // end of DSLBase::readOnlyOneToken

  std::pair<std::string, VariableBoundsDescription>
  DSLBase::readVariableBounds() {
    return mfront::readVariableBounds(this->current, this->end());
  }  // end of DSLBase::readVariableBounds

  void DSLBase::registerIntegerConstant(const std::string& n,
                                        const size_t l,
                                        const int v) {
    if (!this->isValidIdentifier(n)) {
      this->throwRuntimeError("DSLBase::registerIntegerConstant",
                              "Variable name '" + n + "' is not valid.");
    }
    this->addStaticVariableDescription(
        StaticVariableDescription("int", n, l, v));
  }  // end of DSLBase::registerIntegerConstant

  void DSLBase::treatIntegerConstant() {
    this->checkNotEndOfFile("DSLBase::treatIntegerConstant",
                            "Cannot read type of static variable.");
    const auto name = this->current->value;
    const auto line = this->current->line;
    ++(this->current);
    const auto value = this->readInitialisationValue<int>(name, true);
    this->readSpecifiedToken("DSLBase::treatIntegerConstant", ";");
    this->registerIntegerConstant(name, line, value.second);
  }  // end of DSLBase::treatIntegerConstant

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
      this->checkNotEndOfFile("DSLBase::readVarList");
      while (this->current->value != "]") {
        throw_if(((this->current->flag != tfel::utilities::Token::Standard) &&
                  (this->current->flag != tfel::utilities::Token::Number)) ||
                     (this->current->value == ";"),
                 "invalid array size for '" + n + "'");
        array_size += this->current->value;
        ++(this->current);
        this->checkNotEndOfFile("DSLBase::readVarList");
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
      this->readSpecifiedToken("DSLBase::readVarList", "]");
      this->checkNotEndOfFile("DSLBase::readVarList");
    }
    return asize;
  }  // end of DSLBase::readArrayOfVariablesSize

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
      const auto& varName = this->current->value;
      throw_if(
          !this->isValidIdentifier(this->current->value),
          "variable given is not valid (read '" + this->current->value + "').");
      auto lineNumber = this->current->line;
      ++(this->current);
      this->checkNotEndOfFile("DSLBase::readVarList");
      const auto asize = this->readArrayOfVariablesSize(varName, allowArray);
      if (this->current->value == ",") {
        ++(this->current);
      } else if (this->current->value == ";") {
        endOfTreatment = true;
        endComment = this->current->comment;
        ++(this->current);
      } else {
        throw_if(true, "',' or ';' expected after '" + varName + "'");
      }
      cont.push_back(VariableDescription(type, varName, asize, lineNumber));
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

  std::pair<std::string, bool> DSLBase::readType() {
    auto throw_if = [this](const bool b, const std::string& m) {
      if (b) {
        this->throwRuntimeError("DSLBase::readType", m);
      }
    };
    auto type = this->current->value;
    throw_if(!this->isValidIdentifier(type, false),
             "given type '" + type + "' is not valid.");
    ++(this->current);
    this->checkNotEndOfFile("DSLBase::readType");
    while (this->current->value == "::") {
      ++(this->current);
      this->checkNotEndOfFile("DSLBase::readType");
      const auto t = this->current->value;
      throw_if(!this->isValidIdentifier(t, false),
               "given type '" + t + "' is not valid.");
      type += "::" + t;
      ++(this->current);
      this->checkNotEndOfFile("DSLBase::readType");
    }
    if (this->current->value == "<") {
      bool b = false;
      type += "<";
      this->checkNotEndOfFile("DSLBase::readType");
      ++(this->current);
      this->checkNotEndOfFile("DSLBase::readType");
      bool c = true;
      while (c) {
        if (isInteger(this->current->value)) {
          type += this->current->value;
          ++(this->current);
        } else {
          const auto r = this->readType();
          type += r.first;
          b = r.second;
          if (!b) {
            c = false;
          }
        }
        if (c) {
          this->checkNotEndOfFile("DSLBase::readType");
          if (this->current->value == ",") {
            this->readSpecifiedToken("DSLBase::readType", ",");
            type += ",";
          } else {
            c = false;
          }
        }
      }
      if (b) {
        if (this->current->value == ">>") {
          ++(this->current);
          return {type + '>', false};
        } else {
          this->readSpecifiedToken("DSLBase::readType", ">");
        }
        type += ">";
      }
    }
    return {type, true};
  }  // end of DSLBase::readType

  void DSLBase::readVarList(VariableDescriptionContainer& cont,
                            const bool allowArray) {
    this->checkNotEndOfFile("DSLBase::readVarList",
                            "Cannot read type of varName.\n");
    const auto r = this->readType();
    if (!r.second) {
      this->throwRuntimeError("DSLBase::readVarList", "unbalanced '>'");
    }
    this->readVarList(cont, r.first, allowArray);
  }  // end of DSLBase::readVarList

  std::vector<tfel::utilities::Token> DSLBase::readList(const std::string& m,
                                                        const std::string& db,
                                                        const std::string& de,
                                                        const bool b) {
    std::vector<tfel::utilities::Token> t;
    this->readList(t, m, db, de, b);
    return t;
  }  // end of DSLBase::readList

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
  }  // end of DSLBase::readList

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
  }  // end of DSLBase::readArrayOfString

  std::vector<double> DSLBase::readArrayOfDouble(const std::string& m) {
    auto r = std::vector<double>{};
    auto as = std::vector<tfel::utilities::Token>{};
    this->readList(as, m, "{", "}", false);
    r.reserve(as.size());
    for (const auto& t : as) {
      r.push_back(tfel::utilities::convert<double>(t.value));
    }
    return r;
  }  // end of DSLBase::readArrayOfDouble

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
  }  // end of DSLBase::readBooleanValue

  std::string DSLBase::readString(const std::string& m) {
    this->checkNotEndOfFile(m, "Expected a string or '{'");
    if (this->current->flag != tfel::utilities::Token::String) {
      this->throwRuntimeError(m, "Expected a string");
    }
    const auto& r =
        this->current->value.substr(1, this->current->value.size() - 2);
    ++(this->current);
    return r;
  }  // end of DSLBase::readString

  std::vector<std::string> DSLBase::readStringOrArrayOfString(
      const std::string& m) {
    this->checkNotEndOfFile(m, "Expected a string or '{'");
    if (this->current->value == "{") {
      return this->readArrayOfString(m);
    }
    return {1u, this->readString(m)};
  }  // end of DSLBase::readStringOrArrayOfString

  void DSLBase::treatLink() {
    const auto nlink = readStringOrArrayOfString("DSLBase::treatLink");
    this->readSpecifiedToken("DSLBase::treatLink", ";");
    for (const auto& l : nlink) {
      insert_if(this->ldflags, l);
    }
  }  // end of DSLBase::treatLink

  void DSLBase::callMFront(const std::vector<std::string>& interfaces,
                           const std::vector<std::string>& files) {
    MFront m;
    for (const auto& i : interfaces) {
      m.setInterface(i);
    }
    for (const auto& f : files) {
      mergeTargetsDescription(this->td, m.treatFile(f), false);
    }
  }  // end of DSLBase::callMFront

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
      this->externalMFrontFiles.insert({f, vinterfaces});
    }
  }  // end of DSLBase::treatMfront

  std::string DSLBase::readSpecifiedValue(const std::string& file,
                                          const std::string& value) {
    std::vector<std::string> values(1, value);
    return this->readSpecifiedValues(file, values)[1];
  }  // end of DSLBase::readSpecifiedValue

  std::vector<std::string> DSLBase::readSpecifiedValues(
      const std::string& file,
      const std::string& value1,
      const std::string& value2) {
    return this->readSpecifiedValues(file, {value1, value2});
  }  // end of DSLBase::readSpecifiedValues

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
            ++pt == pte,
            "unexepected end of file '" + file + "' (expected " + *p + ").\n",
            (--pt)->line);
        const auto value = pt->value;
        throw_if(pt->value == ";",
                 "unexepected token ';' (exepected " + *p + ")", pt->line);
        throw_if(++pt == pte,
                 "unexepected end of file '" + file + "' (expected ';').\n",
                 (--pt)->line);
        throw_if(pt->value != ";",
                 "unexepected token '" + pt->value + "' (exepected ';')",
                 pt->line);
        res[static_cast<std::vector<std::string>::size_type>(
            p - values.begin())] = value;
      }
      ++pt;
    }
    return res;
  }  // end of DSLBase::readSpecifiedValues

  std::shared_ptr<MaterialPropertyDescription>
  DSLBase::handleMaterialPropertyDescription(const std::string& f) {
    // getting informations the source files
    MaterialPropertyDSL mp;
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
      this->externalMFrontFiles.insert({path, {"mfront"}});
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
  }  // end of DSLBase::handleMaterialLaw

  void DSLBase::treatMaterialLaw() {
    const auto vfiles =
        this->readStringOrArrayOfString("DSLBase::treatMaterialLaw");
    this->readSpecifiedToken("DSLBase::treatMaterialLaw", ";");
    for (const auto& f : vfiles) {
      this->handleMaterialPropertyDescription(f);
    }
  }  // end of DSLBase::treatMaterialLaw

  void DSLBase::treatDescription() {
    this->readSpecifiedToken("DSLBase::treatDescription", "{");
    this->checkNotEndOfFile("DSLBase::treatDescription");
    this->fd.description += "* ";
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
          this->fd.description += "\n* ";
          ++currentLine;
        }
      }
      if (this->current->flag == tfel::utilities::Token::String) {
        this->fd.description +=
            this->current->value.substr(1, this->current->value.size() - 2u);
      } else {
        this->fd.description += this->current->value;
      }
      this->fd.description += " ";
      ++(this->current);
    }
    if (this->current == this->tokens.end()) {
      --(this->current);
      this->throwRuntimeError("DSLBase::treatDescription",
                              "File ended before the end of description.");
    }
    ++(this->current);
  }  // end of DSLBase::treatDescription

  void DSLBase::treatLonelySeparator() {
    if (getPedanticMode()) {
      getLogStream() << this->fd.fileName << ":" << this->current->line << ":"
                     << this->current->offset
                     << ": warning: extra ‘;’ [-pedantic]\n";
    }
  }  // end of DSLBase::treatLonelySperator

  void DSLBase::treatAuthor() {
    this->fd.authorName = this->readUntilEndOfInstruction();
  }  // end of DSLBase::treatAuthor

  void DSLBase::treatDate() {
    this->fd.date = this->readUntilEndOfInstruction();
  }  // end of DSLBase::treatDate

  void DSLBase::treatUnknownKeyword() {
    --(this->current);
    this->throwRuntimeError(
        "DSLBase::treatUnknownKeyword",
        "unknown keyword (read '" + this->current->value + "')");
  }  // end of DSLBase::treatUnknownKeyword

  void DSLBase::treatIncludes() {
    CodeBlockParserOptions options;
    this->appendToIncludes(this->readNextBlock(options).code);
  }

  void DSLBase::treatSources() {
    CodeBlockParserOptions options;
    this->appendToSources(this->readNextBlock(options).code);
  }  // end of DSLBase::treatSources

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
  }  // end of DSLBase::treatPrivate

  void DSLBase::treatParser() {
    this->readUntilEndOfInstruction();
  }  // end of DSLBase::treatParser

  void DSLBase::treatStaticVar() {
    this->checkNotEndOfFile("DSLBase::treatStaticVar",
                            "Cannot read type of static variable.");
    const auto type = this->current->value;
    if (!this->isValidIdentifier(type, false)) {
      --(this->current);
      this->throwRuntimeError("DSLBase::treatStaticVar",
                              "type given is not valid.");
    }
    ++(this->current);
    this->checkNotEndOfFile("DSLBase::treatStaticVar",
                            "Cannot read variable name.");
    const auto name = this->current->value;
    if (!this->isValidIdentifier(name)) {
      this->throwRuntimeError("DSLBase::treatStaticVar",
                              "Variable name '" + name + "' is not valid.");
    }
    const auto line = this->current->line;
    ++(this->current);
    this->checkNotEndOfFile("DSLBase::treatStaticVar",
                            "Expected to read value of variable.");
    const auto value = this->readInitialisationValue<long double>(name, true);
    this->readSpecifiedToken("DSLBase::treatStaticVar", ";");
    this->addStaticVariableDescription(
        StaticVariableDescription(type, name, line, value.second));
  }  // end of DSLBase::treatStaticVar

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
  }  // end of DSLBase::ignoreKeyWord

  double DSLBase::readDouble() {
    this->checkNotEndOfFile("DSLBase::readDouble");
    return CxxTokenizer::readDouble(this->current, this->tokens.end());
  }  // end of DSLBase::readDouble

  void DSLBase::completeTargetsDescription() {
    for (auto& l : this->td) {
      l.ldflags.insert(l.ldflags.end(), this->ldflags.begin(),
                       this->ldflags.end());
    }
    for (const auto& t : this->atds) {
      for (const auto& al : t) {
        for (auto& l : this->td) {
          if (l.name != al.name) {
            insert_if(l.deps, al.name);
          }
        }
      }
    }
    for (const auto& t : this->atds) {
      mergeTargetsDescription(this->td, t, false);
    }
    this->ldflags.clear();
    this->atds.clear();
  }  // end of DSLBase::completeTargetsDescription()

}  // end of namespace mfront
