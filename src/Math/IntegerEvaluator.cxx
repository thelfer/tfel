/*!
 * \file   src/Math/IntegerEvaluator.cxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   02 oct 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cmath>
#include <cctype>
#include <cassert>
#include <utility>
#include <sstream>
#include <iterator>
#include <stdexcept>
#include <algorithm>
#include "TFEL/Raise.hxx"
#include "TFEL/UnicodeSupport/UnicodeSupport.hxx"
#include "TFEL/Math/IntegerEvaluator.hxx"
#include "TFEL/Math/Parser/IntegerEvaluatorTExpr.hxx"

namespace tfel::math {

  IntegerEvaluator::TExpr::~TExpr() = default;

  void IntegerEvaluator::checkNotEndOfExpression(
      const std::string& method,
      const std::string& error,
      const std::vector<std::string>::const_iterator p,
      const std::vector<std::string>::const_iterator pe) {
    raise_if(p == pe, method + " : unexpected end of line" +
                          (!error.empty() ? " (" + error + ")" : ""));
  }  // end of IntegerEvaluator::checkNotEndOfExpression

  void IntegerEvaluator::checkNotEndOfExpression(
      const std::string& method,
      const std::vector<std::string>::const_iterator p,
      const std::vector<std::string>::const_iterator pe) {
    IntegerEvaluator::checkNotEndOfExpression(method, "", p, pe);
  }  // end of IntegerEvaluator::checkNotEndOfExpression

  template <typename T>
  bool IntegerEvaluator::convert(const std::string& value) {
    T res;
    std::istringstream is(value);
    if ((value == "+") || (value == "-") || (value == "*") || (value == "/") ||
        (value == ".")) {
      return false;
    }
    is >> res;
    return !(!is && (!is.eof()));
  }  // end of convert

  bool IntegerEvaluator::isNumber(const std::string& value) {
    return convert<int>(value);
  }  // end of IntegerEvaluator::isNumber

  std::vector<std::string> IntegerEvaluator::analyseParameters(
      std::vector<std::string>::const_iterator& p,
      const std::vector<std::string>::const_iterator pe) {
    std::vector<std::string> params;
    checkNotEndOfExpression("IntegerEvaluator::analyseParameters", p, pe);
    while (*p != ">") {
      // check parameter validity
      for (const auto c : tfel::unicode::getMangledString(*p)) {
        raise_if(!(isalpha(c) || isdigit(c)) || (c == '_') || (c == '-'),
                 "IntegerEvaluator::analyseParameters: "
                 "invalid parameter '" +
                     *p + "'");
      }
      params.push_back(*p);
      ++p;
      IntegerEvaluator::checkNotEndOfExpression(
          "IntegerEvaluator::analyseParameters", p, pe);
      if (*p != ">") {
        if (*p == ",") {
          ++p;
          IntegerEvaluator::checkNotEndOfExpression(
              "IntegerEvaluator::analyseParameters", p, pe);
        } else {
          raise(
              "IntegerEvaluator::analyseParameters: "
              "unexpected token '" +
              *p +
              "' "
              "(expected ',' or '>')");
        }
      }
    }
    ++p;
    return params;
  }  // end of IntegerEvaluator::analyseParameters

  std::vector<std::shared_ptr<IntegerEvaluator::TExpr>>
  IntegerEvaluator::analyseArguments(
      const unsigned short nbr,
      std::vector<std::string>::const_iterator& p,
      const std::vector<std::string>::const_iterator pe,
      const bool b) {
    std::vector<std::shared_ptr<IntegerEvaluator::TExpr>> res;
    if (nbr > 0) {
      unsigned short i;
      for (i = 0; i != nbr - 1u; ++i) {
        res.push_back(this->treatGroup(p, pe, b, ","));
        ++p;
      }
      res.push_back(this->treatGroup(p, pe, b, ")"));
    }
    return res;
  }  // end of IntegerEvaluator::analyseArguments

  std::vector<std::shared_ptr<IntegerEvaluator::TExpr>>
  IntegerEvaluator::analyseArguments(
      std::vector<std::string>::const_iterator& p,
      const std::vector<std::string>::const_iterator pe,
      const bool b) {
    auto res = std::vector<std::shared_ptr<IntegerEvaluator::TExpr>>{};
    const auto nbr = this->countNumberOfArguments(p, pe);
    if (nbr > 0) {
      for (unsigned short i = 0; i != nbr - 1u; ++i) {
        res.push_back(this->treatGroup(p, pe, b, ","));
        ++p;
      }
      res.push_back(this->treatGroup(p, pe, b, ")"));
    }
    return res;
  }  // end of IntegerEvaluator::analyseArguments

  unsigned short IntegerEvaluator::countNumberOfArguments(
      std::vector<std::string>::const_iterator p,
      const std::vector<std::string>::const_iterator pe) {
    unsigned short opened = 1;
    unsigned short nbr = 1;
    IntegerEvaluator::checkNotEndOfExpression(
        "IntegerEvaluator::countNumberOfArguments", p, pe);
    if (*p == ")") {
      return 0;
    }
    while (p != pe) {
      if (*p == "(") {
        ++opened;
      } else if (*p == ")") {
        --opened;
        if (opened == 0) {
          return nbr;
        }
      } else if (*p == ",") {
        if (opened == 1) {
          ++nbr;
        }
      }
      ++p;
    }
    std::ostringstream msg;
    if (opened == 1) {
      msg << "IntegerEvaluator::countNumberOfArguments : group ended while "
          << "a parenthesis was still opened";
    } else {
      msg << "IntegerEvaluator::countNumberOfArguments : group ended while "
          << opened << " parenthesis were still opened";
    }
    raise(msg.str());
  }  // end of IntegerEvaluator::analyseArguments

  std::pair<bool, std::vector<std::string>::const_iterator>
  IntegerEvaluator::search(std::vector<std::string>::const_iterator p,
                           std::vector<std::string>::const_iterator pe,
                           const std::string& m,
                           const std::string& s) {
    unsigned short openedParenthesis = 0;
    bool test;
    if (s.empty()) {
      test = (p != pe);
    } else {
      test = (p != pe) && (*p != s);
    }
    while (test) {
      if (*p == "(") {
        ++openedParenthesis;
      }
      if (*p == ")") {
        raise_if(openedParenthesis == 0,
                 "Analyser::readNextGroup: "
                 "unbalanced parenthesis");
        --openedParenthesis;
      }
      if ((*p == m) && (openedParenthesis == 0)) {
        return {true, p};
      }
      ++p;
      if (s.empty()) {
        test = (p != pe);
      } else {
        test = (p != pe) && (*p != s);
      }
    }
    return {false, p};
  }  // end of IntegerEvaluator::search

  std::shared_ptr<IntegerEvaluator::TExpr> IntegerEvaluator::treatGroup(
      std::vector<std::string>::const_iterator& p,
      const std::vector<std::string>::const_iterator pe,
      const bool b,
      const std::string& s) {
    using namespace tfel::math::parser;
    auto throw_if = [](const bool c, const std::string& m) {
      raise_if(c, "IntegerEvaluator::treatGroup: " + m);
    };
    IntegerEvaluator::checkNotEndOfExpression("IntegerEvaluator::treatGroup", p,
                                              pe);
    assert(p != pe);
    auto g = std::make_shared<TGroup>();
    bool test;
    if (s.empty()) {
      test = (p != pe);
    } else {
      test = (p != pe) && (*p != s);
    }
    while (test) {
      if (isNumber(*p)) {
        // number
        std::istringstream converter(*p);
        int value;
        converter >> value;
        g->add(std::make_shared<TNumber>(value));
      } else if (*p == "(") {
        ++p;
        // begin group
        g->add(this->treatGroup(p, pe, b));
      } else if (*p == "+") {
        g->add(std::make_shared<TOperator>("+"));
      } else if (*p == "-") {
        g->add(std::make_shared<TOperator>("-"));
      } else if (*p == "*") {
        g->add(std::make_shared<TOperator>("*"));
      } else if (*p == "/") {
        g->add(std::make_shared<TOperator>("/"));
      } else {
        auto pt = p;
        ++pt;
        throw_if((pt != pe) && (*pt == "("), "unknown function '" + *p + "'");
        if (b) {
          // variable name is fixed
          throw_if(this->positions.find(*p) == this->positions.end(),
                   "unknown variable '" + *p + "'");
          g->add(std::make_shared<TVariable>(*p, *this));
        } else {
          g->add(std::make_shared<TVariable>(*p, *this));
        }
      }
      ++p;
      if (s.empty()) {
        test = (p != pe);
      } else {
        test = (p != pe) && (*p != s);
      }
    }
    throw_if((!s.empty()) && (p == pe),
             "unterminated group (group began with '" + s + "').");
    return g;
  }  // end of IntegerEvaluator::treatGroup2

  std::vector<double>::size_type IntegerEvaluator::registerVariable(
      const std::string& vname) {
    auto p = this->positions.find(vname);
    const auto pe = this->positions.end();
    if (p != pe) {
      return p->second;
    }
    const auto pos = this->variables.size();
    this->positions.insert({vname, pos});
    this->variables.resize(pos + 1u);
    return pos;
  }  // end of IntegerEvaluator::registerVariable

  IntegerEvaluator::~IntegerEvaluator() = default;

  void IntegerEvaluator::setVariableValue(const std::string& vname,
                                          const int value) {
    auto p = this->positions.find(vname);
    raise_if(p == this->positions.end(),
             "IntegerEvaluator::setVariableValue: "
             "variable '" +
                 vname + "' does not exist");
    this->variables[p->second] = value;
  }  // end of IntegerEvaluator::setVariableValue

  void IntegerEvaluator::setVariableValue(const std::vector<int>::size_type pos,
                                          const int value) {
    if (pos >= this->variables.size()) {
      std::ostringstream msg;
      msg << "IntegerEvaluator::setVariableValue : position '" << pos
          << "' is invalid ";
      if (this->variables.empty()) {
        msg << "(function has no variable).";
      } else if (this->variables.size() == 1) {
        msg << "(function has one variable).";
      } else {
        msg << "(function has only '" << this->variables.size()
            << "' variable(s)).";
      }
      raise(msg.str());
    }
    this->variables[pos] = value;
  }  // end of IntegerEvaluator::setVariableValue

  int IntegerEvaluator::getValue() const {
    raise_if(this->expr == nullptr,
             "IntegerEvaluator::getValue: "
             "uninitialized expression.");
    return this->expr->getValue();
  }  // end of IntegerEvaluator::getValue

  std::vector<std::string> IntegerEvaluator::getVariablesNames() const {
    auto res = std::vector<std::string>{};
    auto vnames = std::map<std::vector<int>::size_type, std::string>{};
    for (const auto& p : this->positions) {
      vnames.insert({p.second, p.first});
    }
    for (const auto& n : vnames) {
      res.push_back(n.second);
    }
    return res;
  }  // end of IntegerEvaluator::getVariablesNames

  std::vector<int>::size_type IntegerEvaluator::getNumberOfVariables() const {
    return this->variables.size();
  }  // end of IntegerEvaluator::getNumberOfVariables() const;

  void IntegerEvaluator::analyse(const std::string& f, const bool b) {
    EvaluatorBase::analyse(f);
    auto p = this->tokens.cbegin();
    const auto pe = this->tokens.cend();
    auto g = this->treatGroup(p, pe, b, "");
    g->reduce();
    this->expr = g->analyse();
  }

  IntegerEvaluator::IntegerEvaluator() {
  }  // end of IntegerEvaluator::IntegerEvaluator

  IntegerEvaluator::IntegerEvaluator(const IntegerEvaluator& src)
      : EvaluatorBase(src), variables(src.variables), positions(src.positions) {
    if (src.expr != nullptr) {
      this->expr = src.expr->clone(this->variables);
    }
  }  // end of IntegerEvaluator::IntegerEvaluator

  IntegerEvaluator& IntegerEvaluator::operator=(const IntegerEvaluator& src) {
    if (this != &src) {
      this->variables = src.variables;
      this->positions = src.positions;
      if (src.expr != nullptr) {
        this->expr = src.expr->clone(this->variables);
      } else {
        this->expr.reset();
      }
    }
    return *this;
  }  // end of IntegerEvaluator::IntegerEvaluator

  IntegerEvaluator::IntegerEvaluator(const std::string& f) {
    this->setFunction(f);
  }  // end of IntegerEvaluator::IntegerEvaluator

  IntegerEvaluator::IntegerEvaluator(const std::vector<std::string>& vars,
                                     const std::string& f) {
    this->setFunction(vars, f);
  }  // end of IntegerEvaluator::IntegerEvaluator

  void IntegerEvaluator::setFunction(const std::string& f) {
    this->variables.clear();
    this->positions.clear();
    this->expr.reset();
    this->analyse(f, false);
  }  // end of IntegerEvaluator::setFunction

  void IntegerEvaluator::setFunction(const std::vector<std::string>& vars,
                                     const std::string& f) {
    this->variables.clear();
    this->positions.clear();
    this->expr.reset();
    this->variables.resize(vars.size());
    std::vector<int>::size_type pos = 0u;
    for (const auto& v : vars) {
      raise_if(this->positions.find(v) != this->positions.end(),
               "IntegerEvaluator::setFunction: "
               "variable '" +
                   v + "' multiply defined.");
      this->positions[v] = pos;
      ++pos;
    }
    this->analyse(f, true);
  }  // end of IntegerEvaluator::setFunction

  std::vector<int>::size_type IntegerEvaluator::getVariablePosition(
      const std::string& name) const {
    const auto p = this->positions.find(name);
    raise_if(p == this->positions.end(),
             "IntegerEvaluator::getVariablePosition: "
             "unknown variable '" +
                 name + "'");
    return p->second;
  }  // end of IntegerEvaluator::getVariablePosition(const std::string&)

}  // end of namespace tfel::math
