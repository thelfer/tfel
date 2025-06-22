/*!
 * \file   src/Math/Evaluator.cxx
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
#include <cassert>
#include <stdexcept>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <cctype>

#include "TFEL/Raise.hxx"
#include "TFEL/PhysicalConstants.hxx"
#include "TFEL/Math/power.hxx"
#include "TFEL/Math/Evaluator.hxx"
#include "TFEL/Math/Parser/Function.hxx"
#include "TFEL/Math/Parser/BinaryFunction.hxx"

namespace tfel {

  namespace math {

    struct Evaluator::FunctionGeneratorManager {
      void insert(const std::string&, const Evaluator::FunctionGenerator);
      void insert(const std::string&, const Evaluator::BinaryFunctionGenerator);
      void insert(const std::string&,
                  const Evaluator::ExternalFunctionGenerator);

     private:
      friend struct Evaluator;
      FunctionGeneratorManager();
      TFEL_VISIBILITY_LOCAL void checkName(const std::string&) const;
      std::map<std::string, double> constants;
      std::map<std::string, Evaluator::FunctionGenerator> fctGenerators;
      std::map<std::string, Evaluator::BinaryFunctionGenerator> bFctGenerators;
      std::map<std::string, Evaluator::ExternalFunctionGenerator>
          extOpGenerators;
    };  // end of struct FunctionManager

    void Evaluator::checkNotEndOfExpression(
        const std::string& method,
        const std::string& error,
        const std::vector<std::string>::const_iterator p,
        const std::vector<std::string>::const_iterator pe) {
      raise_if(p == pe, method + ": unexpected end of line" +
                            ((!error.empty()) ? " (" + error + ")" : ""));
    }  // end of Evaluator::checkNotEndOfExpression

    void Evaluator::checkNotEndOfExpression(
        const std::string& method,
        const std::vector<std::string>::const_iterator p,
        const std::vector<std::string>::const_iterator pe) {
      Evaluator::checkNotEndOfExpression(method, "", p, pe);
    }  // end of Evaluator::checkNotEndOfExpression

    void Evaluator::readSpecifiedToken(
        const std::string& method,
        const std::string& value,
        std::vector<std::string>::const_iterator& p,
        const std::vector<std::string>::const_iterator pe) {
      Evaluator::checkNotEndOfExpression(method, "expected '" + value + "'", p,
                                         pe);
      raise_if(*p != value, method + ": unexpected token '" + *p +
                                "'"
                                " (expected '" +
                                value + "')");
      ++p;
    }  // end of Evaluator::readSpecifiedToken

    unsigned short Evaluator::convertToUnsignedShort(const std::string& method,
                                                     const std::string& value) {
      for (const auto c : value) {
        if (!isdigit(c)) {
          tfel::raise(method + " : invalid entry");
        }
      }
      std::istringstream converter(value);
      unsigned short u;
      converter >> u;
      if ((!converter) || (!converter.eof())) {
        tfel::raise(method + ": not read value from token '" + value + "'");
      }
      return u;
    }  // end of Evaluator::convertToUnsignedShort

    unsigned int Evaluator::convertToUnsignedInt(const std::string& method,
                                                 const std::string& value) {
      for (const auto c : value) {
        if (!isdigit(c)) {
          tfel::raise(method + ": invalid entry");
        }
      }
      std::istringstream converter(value);
      unsigned int u;
      converter >> u;
      if (!converter || (!converter.eof())) {
        tfel::raise(method + ": not read value from token '" + value + "'");
      }
      return u;
    }  // end of Evaluator::convertToUnsignedInt

    int Evaluator::convertToInt(const std::string& method,
                                const std::string& value) {
      std::istringstream converter(value);
      for (const auto c : value) {
        if (!isdigit(c)) {
          tfel::raise(method + ": invalid entry");
        }
      }
      int u;
      converter >> u;
      if (!converter || (!converter.eof())) {
        tfel::raise(method + ": not read value from token '" + value + "'");
      }
      return u;
    }  // end of Evaluator::convertToInt

    unsigned short Evaluator::readUnsignedShortValue(
        const std::string& method,
        std::vector<std::string>::const_iterator& p,
        const std::vector<std::string>::const_iterator pe) {
      Evaluator::checkNotEndOfExpression(
          method, "expected unsigned short value", p, pe);
      const auto nbr = convertToUnsignedShort(method, *p);
      ++p;
      return nbr;
    }  // end of Evaluator::readUnsignedShortValue

    void Evaluator::checkParameterNumber(
        const std::vector<double>::size_type n,
        const std::vector<double>::size_type p) {
      if (p != n) {
        std::string msg("Evaluator::checkParameterNumber: ");
        if (p == 0) {
          msg += "no parameter given";
        } else {
          msg += "too many parameters given";
        }
        raise(msg);
      }
    }  // end of Evaluator::checkParameterNubmer

    void Evaluator::checkVariableNumber(
        const std::vector<double>::size_type n,
        const std::vector<double>::size_type p) {
      if (p != n) {
        std::string msg("Evaluator::checkVariableNumber: ");
        if (p == 0) {
          msg += "no parameter given";
        } else {
          msg += "too many parameters given";
        }
        raise(msg);
      }
    }  // end of Evaluator::checkVariableNubmer

    struct TFEL_VISIBILITY_LOCAL Evaluator::ExternalFunctionRegister {
      ExternalFunctionRegister() = default;
    };  // end of struct Evaluator::ExternalFunctionRegister

    Evaluator::ExternalFunctionRegister Evaluator::externalFunctionRegister;

#if !(defined _WIN32 || defined _WIN64 || defined __CYGWIN__)
    template <unsigned short N>
    static std::shared_ptr<tfel::math::parser::Expr>
    EvaluatorPowerFunctionGenerator(
        const char* const n,
        const std::shared_ptr<tfel::math::parser::Expr> e) {
      using namespace tfel::math::parser;
      using namespace tfel::math::stdfunctions;
      return std::make_shared<StandardFunction<power<N>>>(n, e);
    }  // end of EvaluatorPowerFunctionGenerator
#endif

    static std::shared_ptr<tfel::math::parser::Expr> EvaluatorTreatPower(
        const std::vector<std::string>& params,
        std::vector<std::shared_ptr<tfel::math::parser::Expr>>& args) {
      using namespace tfel::math::parser;
      unsigned short nbr;
      Evaluator::checkParameterNumber(1, params.size());
      Evaluator::checkVariableNumber(1, args.size());
      nbr = Evaluator::convertToUnsignedShort("EvaluatorTreatPower", params[0]);
      switch (nbr) {
        case 0:
          return std::make_shared<Number>("1", 1.);
#if !(defined _WIN32 || defined _WIN64 || defined __CYGWIN__)
        case 1:
          return EvaluatorPowerFunctionGenerator<1>("tfel::math::power<1>",
                                                    args[0]);
        case 2:
          return EvaluatorPowerFunctionGenerator<2>("tfel::math::power<2>",
                                                    args[0]);
        case 3:
          return EvaluatorPowerFunctionGenerator<3>("tfel::math::power<3>",
                                                    args[0]);
        case 4:
          return EvaluatorPowerFunctionGenerator<4>("tfel::math::power<4>",
                                                    args[0]);
        case 5:
          return EvaluatorPowerFunctionGenerator<5>("tfel::math::power<5>",
                                                    args[0]);
        case 6:
          return EvaluatorPowerFunctionGenerator<6>("tfel::math::power<6>",
                                                    args[0]);
        case 7:
          return EvaluatorPowerFunctionGenerator<7>("tfel::math::power<7>",
                                                    args[0]);
        case 8:
          return EvaluatorPowerFunctionGenerator<8>("tfel::math::power<8>",
                                                    args[0]);
        case 9:
          return EvaluatorPowerFunctionGenerator<9>("tfel::math::power<9>",
                                                    args[0]);
        case 10:
          return EvaluatorPowerFunctionGenerator<10>("tfel::math::power<10>",
                                                     args[0]);
        case 11:
          return EvaluatorPowerFunctionGenerator<11>("tfel::math::power<11>",
                                                     args[0]);
        case 12:
          return EvaluatorPowerFunctionGenerator<12>("tfel::math::power<12>",
                                                     args[0]);
        case 13:
          return EvaluatorPowerFunctionGenerator<13>("tfel::math::power<13>",
                                                     args[0]);
        case 14:
          return EvaluatorPowerFunctionGenerator<14>("tfel::math::power<14>",
                                                     args[0]);
        case 15:
          return EvaluatorPowerFunctionGenerator<15>("tfel::math::power<15>",
                                                     args[0]);
        case 16:
          return EvaluatorPowerFunctionGenerator<16>("tfel::math::power<16>",
                                                     args[0]);
#endif
      }
      raise("EvaluatorTreatPower: only exponent below 16 are supported");
    }  // end of EvaluatorTreatPower

    void Evaluator::treatDiff(std::vector<std::string>::const_iterator& p,
                              const std::vector<std::string>::const_iterator pe,
                              Evaluator::TGroup* const g,
                              const bool b) {
      using namespace tfel::math::parser;
      using size_type = std::vector<double>::size_type;
      auto throw_if = [](const bool c, const std::string& m) {
        raise_if(c, "Evaluator::treatDiff: " + m);
      };
      unsigned short openedParenthesis = 0u;
      int diffNumber = -1;
      Evaluator::checkNotEndOfExpression("Evaluator::treatDiff", "(", p, pe);
      if (*p == "<") {
        ++p;
        Evaluator::checkNotEndOfExpression("Evaluator::treatDiff",
                                           "expected number", p, pe);
        auto ps = p->begin();
        const auto pse = p->end();
        while (ps != pse) {
          throw_if(!std::isdigit(*ps), "expected a number");
          ++ps;
        }
        std::istringstream converter(*p);
        converter >> diffNumber;
        throw_if(diffNumber <= 1,
                 "order of differentiation must be greater or egal than 1");
        ++p;
        Evaluator::readSpecifiedToken("Evaluator::treatDiff", ">", p, pe);
        Evaluator::checkNotEndOfExpression("Evaluator::treatDiff",
                                           "expected ')'", p, pe);
      }
      Evaluator::readSpecifiedToken("Evaluator::treatDiff", "(", p, pe);
      Evaluator::checkNotEndOfExpression("Evaluator::treatDiff", p, pe);
      auto f = std::string{};
      while (!((*p == ",") && (openedParenthesis == 0u))) {
        if (*p == "(") {
          ++openedParenthesis;
        }
        if (*p == ")") {
          throw_if(openedParenthesis == 0, "unbalanced parenthesis");
          --openedParenthesis;
        }
        f += *p;
        ++p;
        Evaluator::checkNotEndOfExpression("Evaluator::treatDiff", p, pe);
      }
      throw_if(f.empty(), "function definition is empty");
      ++p;
      Evaluator::checkNotEndOfExpression("Evaluator::treatDiff",
                                         "expected variable name", p, pe);
      // now reading var list
      auto var = std::vector<std::string>{};
      while (*p != ")") {
        var.push_back(*p);
        ++p;
        Evaluator::checkNotEndOfExpression("Evaluator::treatDiff",
                                           "expected  ',' or ')'", p, pe);
        if (*p != ")") {
          throw_if(*p != ",",
                   "unexpected token '" + *p + "' (expected ',' or ')')");
          ++p;
          Evaluator::readSpecifiedToken("Evaluator::treatDiff", ")", p, pe);
        }
      }
      throw_if(var.empty(), "empty var list");
      throw_if((var.size() > 1) && (diffNumber != -1),
               "only one variable name allowed when degree of differentiation "
               "specified");
      auto pev = [this, b, &f] {
        if (this->manager == nullptr) {
          if (b) {
            // variable names are fixed
            return std::make_shared<Evaluator>(this->getVariablesNames(), f);
          } else {
            return std::make_shared<Evaluator>(f);
          }
        }
        if (b) {
          // variable names are fixed
          return std::make_shared<Evaluator>(this->getVariablesNames(), f,
                                             this->manager);
        }
        return std::make_shared<Evaluator>(f, this->manager);
      }();
      const auto& fvars = pev->getVariablesNames();
      for (const auto& v : var) {
        throw_if(find(fvars.begin(), fvars.end(), v) == fvars.end(),
                 "expression '" + f + "'can't be differentiated by variable '" +
                     v + "'");
      }
      if (diffNumber != -1) {
        var.resize(static_cast<size_type>(diffNumber), var[0]);
      }
      auto args = std::vector<std::shared_ptr<Evaluator::TExpr>>{};
      for (const auto& v : fvars) {
        if (!b) {
          this->registerVariable(v);
        }
        const auto pos = this->getVariablePosition(v);
        args.push_back(
            std::make_shared<Evaluator::TVariable>(pos, this->variables));
      }
      auto pvar = std::vector<size_type>{};
      for (const auto& v : var) {
        pvar.push_back(static_cast<size_type>(
            std::find(fvars.begin(), fvars.end(), v) - fvars.begin()));
      }
      g->add(std::make_shared<Evaluator::TDifferentiatedFunctionExpr>(pev, args,
                                                                      pvar));
    }  // end of Evaluator::treatDiff

    void Evaluator::FunctionGeneratorManager::checkName(
        const std::string& name) const {
      auto throw_if = [](const bool b, const std::string& m) {
        raise_if(b, "Evaluator::FunctionGeneratorManager::checkName: " + m);
      };
      throw_if(this->constants.find(name) != this->constants.end(),
               "a constant named '" + name + "' has already been declared");
      throw_if(this->fctGenerators.find(name) != this->fctGenerators.end(),
               "a function named '" + name + "' has already been declared");
      throw_if(
          this->bFctGenerators.find(name) != this->bFctGenerators.end(),
          "a binary function named '" + name + "' has already been declared");
      throw_if(this->extOpGenerators.find(name) != this->extOpGenerators.end(),
               "an external operator named '" + name +
                   "' has already been declared.");
    }  // end of FunctionGeneratorManager::checkName

    void Evaluator::FunctionGeneratorManager::insert(
        const std::string& name, const Evaluator::FunctionGenerator f) {
      this->checkName(name);
      this->fctGenerators.insert({name, f});
    }  // end of FunctionGeneratorManager::insert

    void Evaluator::FunctionGeneratorManager::insert(
        const std::string& name, const Evaluator::BinaryFunctionGenerator f) {
      this->checkName(name);
      this->bFctGenerators.insert({name, f});
    }  // end of FunctionGeneratorManager::insert

    void Evaluator::FunctionGeneratorManager::insert(
        const std::string& name, const Evaluator::ExternalFunctionGenerator f) {
      this->checkName(name);
      this->extOpGenerators.insert({name, f});
    }  // end of FunctionGeneratorManager::insert

    template <double (*f)(const double)>
    static std::pair<
        std::string,
        std::function<Evaluator::ExprPtr(const Evaluator::ExprPtr)>>
    makeStandardFunctionGenerator(const char* const n) {
      return {n, [n](const Evaluator::ExprPtr e) {
                return std::shared_ptr<parser::Expr>(
                    new parser::StandardFunction<f>(n, e));
              }};
    }  // end of makeStandardFunctionGenerator

    template <double (*f)(const double, const double)>
    static std::pair<std::string,
                     std::function<Evaluator::ExprPtr(
                         const Evaluator::ExprPtr, const Evaluator::ExprPtr)>>
    makeBinaryFunctionGenerator(const char* const n) {
      return {n, [n](const Evaluator::ExprPtr e1, const Evaluator::ExprPtr e2) {
                return std::shared_ptr<parser::Expr>(
                    new parser::StandardBinaryFunction<f>(n, e1, e2));
              }};
    }  // end of makeStandardFunctionGenerator

    Evaluator::FunctionGeneratorManager::FunctionGeneratorManager() {
      this->constants.insert(
          {"Cste::AtomicMassConstant",
           tfel::PhysicalConstants<double>::AtomicMassConstant});
      this->constants.insert({"Cste::mu", tfel::PhysicalConstants<double>::mu});
      this->constants.insert(
          {"Cste::AvogadroConstant",
           tfel::PhysicalConstants<double>::AvogadroConstant});
      this->constants.insert({"Cste::Na", tfel::PhysicalConstants<double>::Na});
      this->constants.insert(
          {"Cste::BoltzmannConstant",
           tfel::PhysicalConstants<double>::BoltzmannConstant});
      this->constants.insert({"Cste::kb", tfel::PhysicalConstants<double>::kb});
      this->constants.insert(
          {"Cste::ConductanceQuantum",
           tfel::PhysicalConstants<double>::ConductanceQuantum});
      this->constants.insert({"Cste::G0", tfel::PhysicalConstants<double>::G0});
      this->constants.insert(
          {"Cste::ElectricConstant",
           tfel::PhysicalConstants<double>::ElectricConstant});
      this->constants.insert({"Cste::e0", tfel::PhysicalConstants<double>::e0});
      this->constants.insert({"Cste::ElectronMass",
                              tfel::PhysicalConstants<double>::ElectronMass});
      this->constants.insert({"Cste::me", tfel::PhysicalConstants<double>::me});
      this->constants.insert({"Cste::ElectronVolt",
                              tfel::PhysicalConstants<double>::ElectronVolt});
      this->constants.insert({"Cste::eV", tfel::PhysicalConstants<double>::eV});
      this->constants.insert(
          {"Cste::ElementaryCharge",
           tfel::PhysicalConstants<double>::ElementaryCharge});
      this->constants.insert({"Cste::e", tfel::PhysicalConstants<double>::e});
      this->constants.insert(
          {"Cste::FaradayConstant",
           tfel::PhysicalConstants<double>::FaradayConstant});
      this->constants.insert({"Cste::F", tfel::PhysicalConstants<double>::F});
      this->constants.insert(
          {"Cste::FineStructureConstant",
           tfel::PhysicalConstants<double>::FineStructureConstant});
      this->constants.insert({"Cste::a", tfel::PhysicalConstants<double>::a});
      this->constants.insert(
          {"Cste::MolarGasConstant",
           tfel::PhysicalConstants<double>::MolarGasConstant});
      this->constants.insert({"Cste::R", tfel::PhysicalConstants<double>::R});
      this->constants.insert(
          {"Cste::StefanBoltzmannConstant",
           tfel::PhysicalConstants<double>::StefanBoltzmannConstant});
      this->constants.insert({"Cste::s", tfel::PhysicalConstants<double>::s});
      this->fctGenerators.insert(
          {makeStandardFunctionGenerator<exp>("exp"),
           makeStandardFunctionGenerator<exp2>("exp2"),
           makeStandardFunctionGenerator<expm1>("expm1"),
           makeStandardFunctionGenerator<cbrt>("cbrt"),
           makeStandardFunctionGenerator<fabs>("abs"),
           makeStandardFunctionGenerator<sqrt>("sqrt"),
           makeStandardFunctionGenerator<log>("ln"),
           makeStandardFunctionGenerator<log>("log"),
           makeStandardFunctionGenerator<log10>("log10"),
           makeStandardFunctionGenerator<log2>("log2"),
           makeStandardFunctionGenerator<log1p>("log1p"),
           makeStandardFunctionGenerator<cosh>("cosh"),
           makeStandardFunctionGenerator<sinh>("sinh"),
           makeStandardFunctionGenerator<tanh>("tanh"),
           makeStandardFunctionGenerator<acosh>("acosh"),
           makeStandardFunctionGenerator<asinh>("asinh"),
           makeStandardFunctionGenerator<atanh>("atanh"),
           makeStandardFunctionGenerator<sin>("sin"),
           makeStandardFunctionGenerator<cos>("cos"),
           makeStandardFunctionGenerator<tan>("tan"),
           makeStandardFunctionGenerator<acos>("acos"),
           makeStandardFunctionGenerator<asin>("asin"),
           makeStandardFunctionGenerator<atan>("atan"),
           makeStandardFunctionGenerator<erf>("erf"),
           makeStandardFunctionGenerator<erfc>("erfc"),
           makeStandardFunctionGenerator<tgamma>("tgamma"),
           makeStandardFunctionGenerator<lgamma>("lgamma"),
           makeStandardFunctionGenerator<Evaluator::Heavyside>("H")});
      this->bFctGenerators.insert({
          makeBinaryFunctionGenerator<Evaluator::max>("max"),
          makeBinaryFunctionGenerator<Evaluator::min>("min"),
          makeBinaryFunctionGenerator<hypot>("hypot"),
          makeBinaryFunctionGenerator<atan2>("atan2"),
      });
      this->extOpGenerators.insert({"power", EvaluatorTreatPower});
    }  // end of Evaluator::FunctionGeneratorManager::FunctionGeneratorManager

    Evaluator::FunctionGeneratorManager&
    Evaluator::getFunctionGeneratorManager() {
      static FunctionGeneratorManager m;
      return m;
    }  // end of Evaluator::getFunctionGeneratorManager()

    bool Evaluator::isValidIdentifier(const std::string& s) {
      const auto& f = Evaluator::getFunctionGeneratorManager();
      if (s == "diff") {
        return false;
      }
      if (f.extOpGenerators.count(s) != 0) {
        return false;
      }
      if (f.constants.count(s) != 0) {
        return false;
      }
      if (f.fctGenerators.count(s) != 0) {
        return false;
      }
      if (f.bFctGenerators.count(s) != 0) {
        return false;
      }
      return true;
    }  // end of Evaluator::isValidIdentifier

    double Evaluator::Heavyside(const double x) {
      return x < 0 ? 0 : 1;
    }  // end of Evaluator::Heavyside

    double Evaluator::max(const double a, const double b) {
      return std::max(a, b);
    }  // end of Evaluator::max

    double Evaluator::min(const double a, const double b) {
      return std::min(a, b);
    }  // end of Evaluator::min

    void Evaluator::checkCyclicDependency(
        std::vector<std::string>& names) const {
      raise_if(this->expr == nullptr,
               "Evaluator::checkCyclicDependency: "
               "uninitialized evaluator");
      this->expr->checkCyclicDependency(names);
    }  // end of Evaluator::checkCyclicDependency

    void Evaluator::checkCyclicDependency(const std::string& name) const {
      std::vector<std::string> names(1, name);
      raise_if(this->expr == nullptr,
               "Evaluator::checkCyclicDependency: "
               "uninitialized evaluator");
      this->expr->checkCyclicDependency(names);
    }  // end of Evaluator::checkCyclicDependency

    void Evaluator::checkCyclicDependency() const {
      std::vector<std::string> names;
      raise_if(this->expr == nullptr,
               "Evaluator::checkCyclicDependency: "
               "uninitialized evaluator");
      this->expr->checkCyclicDependency(names);
    }  // end of Evaluator::checkCyclicDependency

    template <typename T>
    bool Evaluator::convert(const std::string& value) {
      std::istringstream is(value);
      T res;
      is >> res;
      return !(!is || (!is.eof()));
    }  // end of convert

    bool Evaluator::isNumber(const std::string& value) {
      return convert<double>(value);
    }  // end of Evaluator::isNumber

    std::vector<std::string> Evaluator::analyseParameters(
        std::vector<std::string>::const_iterator& p,
        const std::vector<std::string>::const_iterator pe) {
      std::vector<std::string> params;
      checkNotEndOfExpression("Evaluator::analyseParameters", p, pe);
      while (*p != ">") {
        // check parameter validity
        for (const auto c : *p) {
          raise_if(!(isalpha(c) || isdigit(c)) || (c == '_') || (c == '-'),
                   "Evaluator::analyseParameters: "
                   "invalid parameter '" +
                       *p + "'");
        }
        params.push_back(*p);
        ++p;
        Evaluator::checkNotEndOfExpression("Evaluator::analyseParameters", p,
                                           pe);
        if (*p != ">") {
          raise_if(*p != ",",
                   "Evaluator::analyseParameters: "
                   "unexpected token '" +
                       *p +
                       "' "
                       "(expected ',' or '>')");

          ++p;
          Evaluator::checkNotEndOfExpression(
              "Evaluator::"
              "analyseParameters",
              p, pe);
        }
      }
      ++p;
      return params;
    }  // end of Evaluator::analyseParameters

    std::vector<std::shared_ptr<Evaluator::TExpr>> Evaluator::analyseArguments(
        const unsigned short nbr,
        std::vector<std::string>::const_iterator& p,
        const std::vector<std::string>::const_iterator pe,
        const bool b) {
      auto res = std::vector<std::shared_ptr<Evaluator::TExpr>>{};
      if (nbr > 0) {
        unsigned short i;
        for (i = 0; i != nbr - 1u; ++i) {
          res.push_back(this->treatGroup(p, pe, b, ","));
          ++p;
        }
        res.push_back(this->treatGroup(p, pe, b, ")"));
      }
      return res;
    }  // end of Evaluator::analyseArguments

    std::vector<std::shared_ptr<Evaluator::TExpr>> Evaluator::analyseArguments(
        std::vector<std::string>::const_iterator& p,
        const std::vector<std::string>::const_iterator pe,
        const bool b) {
      auto res = std::vector<std::shared_ptr<Evaluator::TExpr>>{};
      const auto nbr = this->countNumberOfArguments(p, pe);
      if (nbr > 0) {
        for (unsigned short i = 0; i != nbr - 1u; ++i) {
          res.push_back(this->treatGroup(p, pe, b, ","));
          ++p;
        }
        res.push_back(this->treatGroup(p, pe, b, ")"));
      }
      return res;
    }  // end of Evaluator::analyseArguments

    unsigned short Evaluator::countNumberOfArguments(
        std::vector<std::string>::const_iterator p,
        const std::vector<std::string>::const_iterator pe) {
      unsigned short opened = 1;
      unsigned short nbr = 1;
      Evaluator::checkNotEndOfExpression("Evaluator::countNumberOfArguments", p,
                                         pe);
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
        msg << "Evaluator::countNumberOfArguments : group ended while "
            << "a parenthesis was still opened";
      } else {
        msg << "Evaluator::countNumberOfArguments : group ended while "
            << opened << " parenthesis were still opened";
      }
      raise(msg.str());
    }  // end of Evaluator::analyseArguments

    std::pair<bool, std::vector<std::string>::const_iterator> Evaluator::search(
        std::vector<std::string>::const_iterator p,
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
          return make_pair(true, p);
        }
        ++p;
        if (s.empty()) {
          test = (p != pe);
        } else {
          test = (p != pe) && (*p != s);
        }
      }
      return {false, p};
    }  // end of Evaluator::search

    std::shared_ptr<Evaluator::TLogicalExpr> Evaluator::treatLogicalExpression(
        const std::vector<std::string>::const_iterator p,
        const std::vector<std::string>::const_iterator pe,
        const bool b) {
      using namespace tfel::math::parser;
      auto throw_if = [](const bool c, const std::string& m) {
        raise_if(c, "Evaluator::treatLogicalExpression: " + m);
      };
      auto pb = p;
      auto pbe = pe;
      checkNotEndOfExpression("Evaluator::treatLogicalExpression", pb, pbe);
      const auto pa = this->search(pb, pbe, "&&", "");
      const auto po = this->search(pb, pbe, "||", "");
      if ((pa.second != pe) || (po.second != pe)) {
        auto pt = pa.first ? pa.second : po.second;
        throw_if(pt == pb, "no left logical expression");
        throw_if(pt + 1 == pbe, "no right logical expression");
        auto lo = this->treatLogicalExpression(pb, pt, b);
        auto ro = this->treatLogicalExpression(pt + 1, pbe, b);
        if (*pt == "&&") {
          return std::make_shared<TLogicalBinaryOperation<OpAnd>>(lo, ro);
        } else if (*pt == "||") {
          return std::make_shared<TLogicalBinaryOperation<OpOr>>(lo, ro);
        } else {
          throw_if(true, "unkwown operator '" + *pt + "'");
        }
      }
      if (*pb == "(") {
        --pbe;
        Evaluator::checkNotEndOfExpression("Evaluator::treatLogicalExpression",
                                           pb, pbe);
        throw_if(*pbe != ")", "unmatched parenthesis");
        ++pb;
        return this->treatLogicalExpression(pb, pbe, b);
      }
      if (*pb == "!") {
        ++pb;
        return std::make_shared<TNegLogicalExpr>(
            this->treatLogicalExpression(pb, pbe, b));
      }
      return this->treatLogicalExpression2(pb, pbe, b);
    }  // end of Evaluator::treatLogicalExpression

    std::vector<std::string>::const_iterator
    Evaluator::searchComparisonOperator(
        const std::vector<std::string>::const_iterator pb,
        const std::vector<std::string>::const_iterator pe) {
      unsigned short openedParenthesis = 0;
      bool found = false;
      bool test;
      auto p = pb;
      std::vector<std::string>::const_iterator po;
      while (p != pe) {
        if (*p == "(") {
          ++openedParenthesis;
        }
        if (*p == ")") {
          raise_if(openedParenthesis == 0,
                   "Analyser::readNextGroup: "
                   "unbalanced parenthesis");
          --openedParenthesis;
        }
        if (openedParenthesis == 0) {
          if ((*p == "==") || (*p == "<=") || (*p == ">") || (*p == ">=")) {
            if (found) {
              std::ostringstream msg;
              msg << "Evaluator::treatLogicalExpression : "
                  << "more than one logical operator found (error while "
                     "parsing '";
              std::copy(pb, pe, std::ostream_iterator<std::string>(msg, " "));
              msg << "')";
              raise(msg.str());
            }
            po = p;
            found = true;
          }
          if (*p == "<") {
            if (p == pb) {
              std::ostringstream msg;
              msg << "Evaluator::treatLogicalExpression : "
                  << "no left part to logical operator (error while parsing '";
              std::copy(p, pe, std::ostream_iterator<std::string>(msg, " "));
              msg << "')";
              raise(msg.str());
            }
            auto prev = p - 1;
            auto peo =
                Evaluator::getFunctionGeneratorManager().extOpGenerators.find(
                    *prev);
            if (peo != Evaluator::getFunctionGeneratorManager()
                           .extOpGenerators.end()) {
              ++p;
              Evaluator::checkNotEndOfExpression(
                  "Evaluator::searchComparisonOperator", p, pe);
              test = true;
              while (test) {
                if (*p == ">") {
                  test = false;
                }
                ++p;
                Evaluator::checkNotEndOfExpression(
                    "Evaluator::searchComparisonOperator", p, pe);
              }
              raise_if(*p != "(",
                       "Evaluator::searchComparisonOperator: "
                       "expected '(', read '" +
                           *p + "'");
              ++openedParenthesis;
            } else {
              if (found) {
                std::ostringstream msg;
                msg << "Evaluator::treatLogicalExpression : "
                    << "more than one logical operator found "
                    << "(error while parsing '";
                std::copy(pb, pe, std::ostream_iterator<std::string>(msg, " "));
                msg << "')";
                raise(msg.str());
              }
              po = p;
              found = true;
            }
          }
        }
        ++p;
      }
      if (!found) {
        std::ostringstream msg;
        msg << "Evaluator::treatLogicalExpression : "
            << "no logical operator found (error while parsing '";
        std::copy(pb, pe, std::ostream_iterator<std::string>(msg, " "));
        msg << "')";
        raise(msg.str());
      }
      if (po == pb) {
        std::ostringstream msg;
        msg << "Evaluator::treatLogicalExpression : "
            << "no left part to logical operator (error while parsing '";
        std::copy(p, pe, std::ostream_iterator<std::string>(msg, " "));
        msg << "')";
        raise(msg.str());
      }
      if (po + 1 == pe) {
        std::ostringstream msg;
        msg << "Evaluator::treatLogicalExpression : "
            << "no right part to logical operator (error while parsing '";
        std::copy(pb, pe, std::ostream_iterator<std::string>(msg, " "));
        msg << "')";
        raise(msg.str());
      }
      return po;
    }  // end of Evaluator::searchComparisonOperator

    std::shared_ptr<Evaluator::TLogicalExpr> Evaluator::treatLogicalExpression2(
        const std::vector<std::string>::const_iterator p,
        const std::vector<std::string>::const_iterator pe,
        const bool b) {
      using namespace tfel::math::parser;
      auto plo = this->searchComparisonOperator(p, pe);
      auto tmp2 = p;
      auto loexpr = this->treatGroup(tmp2, plo, b, "");
      auto tmp3 = plo + 1;
      auto roexpr = this->treatGroup(tmp3, pe, b, "");
      if (*plo == "==") {
        return std::make_shared<TLogicalOperation<OpEqual>>(loexpr, roexpr);
      } else if (*plo == ">") {
        return std::make_shared<TLogicalOperation<OpGreater>>(loexpr, roexpr);
      } else if (*plo == ">=") {
        return std::make_shared<TLogicalOperation<OpGreaterOrEqual>>(loexpr,
                                                                     roexpr);
      } else if (*plo == "<") {
        return std::make_shared<TLogicalOperation<OpLesser>>(loexpr, roexpr);
      } else if (*plo == "<=") {
        return std::make_shared<TLogicalOperation<OpLesserOrEqual>>(loexpr,
                                                                    roexpr);
      }
      raise(
          "Evaluator::treatLogicalExpression2 : "
          "unsupported logical operator '" +
          *plo + "'");
    }  // end of Evaluator::treatLogicalExpression2

    std::shared_ptr<Evaluator::TExpr> Evaluator::treatGroup(
        std::vector<std::string>::const_iterator& p,
        const std::vector<std::string>::const_iterator pe,
        const bool b,
        const std::string& s) {
      using namespace tfel::math::parser;
      auto throw_if = [](const bool c, const std::string& m) {
        raise_if(c, "Evaluator::treatGroup: " + m);
      };
      Evaluator::checkNotEndOfExpression("Evaluator::treatGroup", p, pe);
      throw_if(*p == s, "empty group");
      auto res = this->search(p, pe, "?", s);
      if (res.first) {
        throw_if(p == res.second,
                 "no conditional expression preceeding '?' character");
        throw_if(res.second + 1 == pe,
                 "nothing expression following '?' character");
        throw_if(this->search(res.second + 1, pe, "?", s).first,
                 "imbricated conditional expression are not supported "
                 "(use parenthesis as as workaround)");
        const auto res2 = this->search(res.second + 1, pe, ":", s);
        throw_if(!res2.first, "no matching ':' character to '?' character");
        throw_if(res.second + 1 == res2.second,
                 "empty left conditional expression");
        throw_if(res2.second + 1 == pe, "empty right conditional expression");
        auto l = this->treatLogicalExpression(p, res.second, b);
        auto tmp2 = res.second + 1;
        auto lexpr = this->treatGroup(tmp2, res2.second, b, "");
        p = res2.second + 1;
        auto rexpr = this->treatGroup(p, pe, b, s);
        return std::make_shared<TConditionalExpr>(l, lexpr, rexpr);
      }
      return this->treatGroup2(p, pe, b, s);
    }  // end of Evaluator::treatGroup

    std::shared_ptr<Evaluator::TExpr> Evaluator::treatGroup2(
        std::vector<std::string>::const_iterator& p,
        std::vector<std::string>::const_iterator pe,
        const bool b,
        const std::string& s) {
      auto checkIdentifier = [](const std::string& v) {
        auto throw_if = [&v](const bool c) {
          if (c) {
            tfel::raise(
                "Evaluator::treatGroup2: "
                "invalid variable name '" +
                v + "'");
          }
        };
        throw_if(v.empty());
        auto ps = v.begin();
        const auto pse = v.end();
        throw_if(isdigit(*ps));
        throw_if((!isalpha(*ps)) && (!(isdigit(*ps))) && (*ps != '_') &&
                 (*ps != '$'));
        ++ps;
        while (ps != pse) {
          throw_if((!isalpha(*ps)) && (!(isdigit(*ps))) && (*ps != '_') &&
                   (*ps != '['));
          throw_if(isspace(*ps));
          if (*ps == '[') {
            ++ps;
            throw_if(ps == pse);
            throw_if(!isdigit(*ps));
            while (*ps != ']') {
              throw_if(!isdigit(*ps));
              ++ps;
              throw_if(ps == pse);
            }
          }
          ++ps;
        }
      };  // end of checkIdentifier
      // this must be captured du to a gcc 4.7 bug
      auto readVariableOrFunctionName =
          [this, &p, pe,
           checkIdentifier]() -> std::pair<unsigned int, std::string> {
        Evaluator::checkNotEndOfExpression("Evaluator::treatGroup2", p, pe);
        checkIdentifier(*p);
        auto vn = *p;
        auto vs = 1u;
        ++p;
        while ((p != pe) && (*p == ":")) {
          ++p;
          Evaluator::checkNotEndOfExpression("Evaluator::treatGroup2", p, pe);
          Evaluator::readSpecifiedToken("Evaluator::treatGroup2", ":", p, pe);
          Evaluator::checkNotEndOfExpression("Evaluator::treatGroup2", p, pe);
          checkIdentifier(*p);
          vn += "::" + *p;
          ++vs;
          ++p;
        }
        return {vs, vn};
      };  // end of readVariableOfFunctionName
      assert(p != pe);
      auto g = std::make_shared<TGroup>();
      bool test;
      if (s.empty()) {
        test = (p != pe);
      } else {
        test = (p != pe) && (*p != s);
      }
      while (test) {
        if (*p == "diff") {
          ++p;
          this->treatDiff(p, pe, g.get(), b);
          ++p;
        } else if (isNumber(*p)) {
          // number
          std::istringstream converter(*p);
          converter.precision(16);
          double value;
          converter >> value;
          g->add(std::make_shared<TNumber>(*p, value));
          ++p;
        } else if (*p == "(") {
          ++p;
          // begin group
          g->add(this->treatGroup(p, pe, b));
          ++p;
        } else if (*p == "+") {
          g->add(std::make_shared<TOperator>("+"));
          ++p;
        } else if (*p == "-") {
          g->add(std::make_shared<TOperator>("-"));
          ++p;
        } else if (*p == "*") {
          g->add(std::make_shared<TOperator>("*"));
          ++p;
        } else if (*p == "/") {
          g->add(std::make_shared<TOperator>("/"));
          ++p;
        } else if (*p == "**") {
          g->add(std::make_shared<TOperator>("**"));
          ++p;
        } else {
          const auto vn = readVariableOrFunctionName();
          const auto& fgm = Evaluator::getFunctionGeneratorManager();
          const auto pc = fgm.constants.find(vn.second);
          const auto p2 = fgm.fctGenerators.find(vn.second);
          const auto p3 = fgm.bFctGenerators.find(vn.second);
          const auto p4 = fgm.extOpGenerators.find(vn.second);
          if (p4 != fgm.extOpGenerators.end()) {
            Evaluator::checkNotEndOfExpression("Evaluator::treatGroup2", p, pe);
            if (*p == "<") {
              ++p;
              const auto& params = this->analyseParameters(p, pe);
              Evaluator::readSpecifiedToken("Evaluator::treatGroup2", "(", p,
                                            pe);
              auto args = this->analyseArguments(p, pe, b);
              g->add(std::make_shared<TExternalOperator>(p4->second, params,
                                                         args));
            } else {
              Evaluator::readSpecifiedToken("Evaluator::treatGroup2", "(", p,
                                            pe);
              auto params = std::vector<std::string>{};
              auto args = this->analyseArguments(p, pe, b);
              g->add(std::make_shared<TExternalOperator>(p4->second, params,
                                                         args));
            }
            ++p;
          } else if (p3 != fgm.bFctGenerators.end()) {
            // call to binary function
            Evaluator::readSpecifiedToken("Evaluator::treatGroup2", "(", p, pe);
            const auto args = this->analyseArguments(2, p, pe, b);
            g->add(std::make_shared<TBinaryFunction>(p3->second, args[0],
                                                     args[1]));
            ++p;
          } else if (p2 != fgm.fctGenerators.end()) {
            // call to function
            Evaluator::readSpecifiedToken("Evaluator::treatGroup2", "(", p, pe);
            g->add(std::make_shared<TFunction>(p2->second,
                                               this->treatGroup(p, pe, b)));
            ++p;
          } else if (pc != fgm.constants.end()) {
            std::ostringstream converter;
            converter.precision(15);
            converter << pc->second;
            g->add(std::make_shared<TNumber>(converter.str(), pc->second));
          } else {
            if ((p != pe) && (*p == "(")) {
              if (this->manager != nullptr) {
                this->addExternalFunctionToGroup(g.get(), p, pe, vn.second, b);
                ++p;
              } else {
                raise("Evaluator::treatGroup2 : unknown function '" +
                      vn.second + "'");
              }
            } else {
              auto args = std::vector<std::shared_ptr<Evaluator::TExpr>>{};
              if (b) {
                // variable name is fixed
                if (this->positions.find(vn.second) == this->positions.end()) {
                  if (vn.first != 1) {
                    raise_if(this->manager == nullptr,
                             "Evaluator::treatGroup2: "
                             "unknown function '" +
                                 vn.second + "'");
                  } else {
                    raise_if(this->manager == nullptr,
                             "Evaluator::treatGroup2: "
                             "unknown variable '" +
                                 vn.second + "'");
                  }
                  g->add(std::make_shared<TExternalFunctionExpr>(
                      vn.second, args, this->manager));
                } else {
                  raise_if(vn.first != 1u,
                           "Evaluator::treatGroup2: "
                           "invalid variable name '" +
                               vn.second + "'");
                  g->add(std::make_shared<TVariable>(vn.second, *this));
                }
              } else {
                if (vn.first == 1u) {
                  g->add(std::make_shared<TVariable>(vn.second, *this));
                } else {
                  raise_if(this->manager == nullptr,
                           "Evaluator::treatGroup2: "
                           "unknown function '" +
                               vn.second + "'");
                  g->add(std::make_shared<TExternalFunctionExpr>(
                      vn.second, args, this->manager));
                }
              }
            }
          }
        }
        if (s.empty()) {
          test = (p != pe);
        } else {
          test = (p != pe) && (*p != s);
        }
      }
      raise_if((!s.empty()) && (p == pe),
               "Evaluator::treatGroup2: "
               "unterminated group (group began with '" +
                   s + "').");
      return std::move(g);
    }  // end of Evaluator::treatGroup2

    std::vector<double>::size_type Evaluator::registerVariable(
        const std::string& vname) {
      const auto p = this->positions.find(vname);
      const auto pe = this->positions.end();
      if (p != pe) {
        return p->second;
      }
      const auto pos = this->variables.size();
      this->positions.insert({vname, pos});
      this->variables.resize(pos + 1u);
      return pos;
    }  // end of Evaluator::registerVariable

    void Evaluator::setVariableValue(const std::string& vname,
                                     const double value) {
      auto p = this->positions.find(vname);
      raise_if(p == this->positions.end(),
               "Evaluator::setVariableValue: "
               "variable '" +
                   vname + "' does not exist");
      this->variables[p->second] = value;
    }  // end of Evaluator::setVariableValue

    void Evaluator::setVariableValue(const char* const vname,
                                     const double value) {
      auto p = this->positions.find(vname);
      raise_if(p == this->positions.end(),
               "Evaluator::setVariableValue: "
               "variable '" +
                   std::string(vname) + "' does not exist");
      this->variables[p->second] = value;
    }  // end of Evaluator::setVariableValue

    void Evaluator::setVariableValue(const std::vector<double>::size_type pos,
                                     const double value) {
      if (pos >= this->variables.size()) {
        std::ostringstream msg;
        msg << "Evaluator::setVariableValue : position '" << pos
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
    }  // end of Evaluator::setVariableValue

    std::string Evaluator::getCxxFormula(
        const std::map<std::string, std::string>& m) const {
      raise_if(this->expr == nullptr,
               "Evaluator::getCxxFormula: "
               "uninitialized expression.");
      for (const auto& mv : m) {
        raise_if(this->positions.count(mv.first) == 0,
                 "Evaluator::getCxxFormula: "
                 "invalid substitution '" +
                     mv.first + "'");
      }
      auto vn = std::vector<std::string>{};
      vn.resize(this->positions.size());
      for (const auto& mv : this->positions) {
        const auto p = m.find(mv.first);
        if (p != m.end()) {
          vn[mv.second] = p->second;
        } else {
          vn[mv.second] = mv.first;
        }
      }
      return this->expr->getCxxFormula(vn);
    }  // end of Evaluator::getCxxFormula

    double Evaluator::operator()(const std::map<std::string, double>& vs) {
      return this->getValue(vs);
    }  // end of Evaluator::operator()

    double Evaluator::getValue(const std::map<std::string, double>& vs) {
      for (const auto& v : vs) {
        this->setVariableValue(v.first, v.second);
      }
      return this->getValue();
    }  // end of Evaluator::getValue

    double Evaluator::getValue() const {
      raise_if(this->expr == nullptr,
               "Evaluator::getValue: "
               "uninitialized expression.");
      return this->expr->getValue();
    }  // end of Evaluator::getValue

    double Evaluator::operator()() const {
      return this->getValue();
    }  // end of Evaluator::operator()

    std::vector<std::string> Evaluator::getVariablesNames() const {
      auto res = std::vector<std::string>{};
      res.resize(this->positions.size());
      auto i = std::vector<std::string>::size_type{};
      for (const auto& p : this->positions) {
        res[i] = p.first;
        ++i;
      }
      return res;
    }  // end of Evaluator::getVariablesNames

    std::vector<double>::size_type Evaluator::getNumberOfVariables() const {
      return this->variables.size();
    }  // end of Evaluator::getNumberOfVariables() const;

    void Evaluator::analyse(const std::string& f, const bool b) {
      try {
        tfel::math::parser::EvaluatorBase::analyse(f);
        auto p = this->tokens.cbegin();
        const auto pe = this->tokens.cend();
        auto g = this->treatGroup(p, pe, b, "");
        g->reduce();
        this->expr = g->analyse();
      } catch (std::exception& e) {
        tfel::raise(
            "Evaluator::analyse: "
            "analysis of formula '" +
            f +
            "' failed "
            "(" +
            std::string(e.what()) + ")");
      }
    }

    void Evaluator::addExternalFunctionToGroup(
        TGroup* const g,
        std::vector<std::string>::const_iterator& p,
        const std::vector<std::string>::const_iterator pe,
        const std::string& fn,
        const bool b) {
      Evaluator::readSpecifiedToken("Evaluator::addExternalFunctionToGroup",
                                    "(", p, pe);
      auto args = this->analyseArguments(p, pe, b);
      g->add(std::make_shared<TExternalFunctionExpr>(fn, args, this->manager));
    }  // end of Evaluator::addExternalFunctionToGroup

    Evaluator::Evaluator() = default;

    Evaluator::Evaluator(const Evaluator& src)
        : EvaluatorBase(src),
          variables(src.variables),
          positions(src.positions) {
      this->manager = src.manager;
      if (src.expr != nullptr) {
        this->expr = src.expr->clone(this->variables);
      }
    }  // end of Evaluator::Evaluator

    Evaluator& Evaluator::operator=(const Evaluator& src) {
      if (this != &src) {
        this->variables = src.variables;
        this->positions = src.positions;
        this->manager = src.manager;
        if (src.expr != nullptr) {
          this->expr = src.expr->clone(this->variables);
        } else {
          this->expr.reset();
        }
      }
      return *this;
    }  // end of Evaluator::Evaluator

    Evaluator::Evaluator(const std::string& f) {
      this->setFunction(f);
    }  // end of Evaluator::Evaluator

    Evaluator::Evaluator(const std::vector<std::string>& vars,
                         const std::string& f) {
      this->setFunction(vars, f);
    }  // end of Evaluator::Evaluator

    Evaluator::Evaluator(
        const std::string& f,
        std::shared_ptr<tfel::math::parser::ExternalFunctionManager>& m) {
      this->setFunction(f, m);
    }  // end of Evaluator::Evaluator

    Evaluator::Evaluator(
        const std::vector<std::string>& vars,
        const std::string& f,
        std::shared_ptr<tfel::math::parser::ExternalFunctionManager>& m) {
      this->setFunction(vars, f, m);
    }  // end of Evaluator::Evaluator

    Evaluator::Evaluator(const double v) {
      std::ostringstream str;
      str.precision(15);
      str << v;
      this->expr = std::make_shared<parser::Number>(str.str(), v);
    }  // end of Evaluator::Evaluator

    void Evaluator::clear() {
      this->variables.clear();
      this->positions.clear();
      this->expr.reset();
      this->manager.reset();
    }

    void Evaluator::setFunction(const std::string& f) {
      this->clear();
      this->analyse(f, false);
    }  // end of Evaluator::setFunction

    void Evaluator::setFunction(const std::vector<std::string>& vars,
                                const std::string& f) {
      this->clear();
      this->variables.resize(vars.size());
      auto pos = std::vector<double>::size_type{0u};
      for (const auto& v : vars) {
        raise_if(this->positions.find(v) != this->positions.end(),
                 "Evaluator::setFunction: "
                 "variable '" +
                     v + "' multiply defined.");
        raise_if(!Evaluator::isValidIdentifier(v),
                 "Evaluator::setFunction: "
                 "variable '" +
                     v + "' is not valid.");
        this->positions[v] = pos;
        ++pos;
      }
      this->analyse(f, true);
    }  // end of Evaluator::setFunction

    void Evaluator::setFunction(
        const std::string& f,
        std::shared_ptr<tfel::math::parser::ExternalFunctionManager>& m) {
      this->clear();
      this->manager = m;
      this->analyse(f, false);
    }  // end of Evaluator::setFunction

    void Evaluator::setFunction(
        const std::vector<std::string>& vars,
        const std::string& f,
        std::shared_ptr<tfel::math::parser::ExternalFunctionManager>& m) {
      auto throw_if = [](const bool c, const std::string& msg) {
        raise_if(c, "Evaluator::treatGroup: " + msg);
      };
      this->clear();
      this->manager = m;
      this->variables.resize(vars.size());
      auto pos = std::vector<double>::size_type{};
      for (const auto& v : vars) {
        throw_if(this->positions.find(v) != this->positions.end(),
                 "variable '" + v + "' multiply defined");
        throw_if(!Evaluator::isValidIdentifier(v),
                 "variable '" + v + "' is not valid.");
        this->positions[v] = pos;
        ++pos;
      }
      this->analyse(f, true);
    }  // end of Evaluator::setFunction

    std::shared_ptr<tfel::math::parser::ExternalFunction>
    Evaluator::differentiate(const std::vector<double>::size_type pos) const {
      auto pev = std::make_shared<Evaluator>();
      raise_if(this->expr == nullptr,
               "Evaluator::differentiate: "
               "uninitialized expression.");
      pev->variables.resize(this->variables.size());
      pev->positions = this->positions;
      if (this->variables.empty()) {
        // no variable
        pev->expr = std::make_shared<parser::Number>("0", 0.);
      } else {
        if (pos >= this->variables.size()) {
          std::ostringstream msg;
          msg << "Evaluator::differentiate : position '" << pos
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
        pev->expr = this->expr->differentiate(pos, pev->variables);
      }
      return std::move(pev);
    }  // end of Evaluator::differentiate

    std::shared_ptr<tfel::math::parser::ExternalFunction>
    Evaluator::differentiate(const std::string& v) const {
      return this->differentiate(this->getVariablePosition(v));
    }  // end of std::shared_ptr<ExternalFunction>

    std::shared_ptr<tfel::math::parser::ExternalFunction>
    Evaluator::resolveDependencies() const {
      this->checkCyclicDependency();
      auto f = std::make_shared<Evaluator>(*this);
      f->expr = f->expr->resolveDependencies(f->variables);
      return std::move(f);
    }  // end of Evaluator::resolveDependencies() const

    void Evaluator::removeDependencies() {
      this->checkCyclicDependency();
      this->expr = this->expr->resolveDependencies(this->variables);
    }  // end of Evaluator::removeDependencies() const

    std::shared_ptr<tfel::math::parser::ExternalFunctionManager>
    Evaluator::getExternalFunctionManager() {
      return this->manager;
    }  // end of Evaluator::getExternalFunctionManager

    std::vector<double>::size_type Evaluator::getVariablePosition(
        const std::string& name) const {
      const auto p = this->positions.find(name);
      raise_if(p == this->positions.end(),
               "Evaluator::getVariablePosition: "
               "unknown variable '" +
                   name + "'");
      return p->second;
    }  // end of Evaluator::getVariablePosition(const std::string&)

    std::shared_ptr<tfel::math::parser::ExternalFunction>
    Evaluator::createFunctionByChangingParametersIntoVariables(
        std::vector<std::string>& nparams,
        const std::vector<double>&,
        const std::vector<std::string>& params,
        const std::map<std::string, std::vector<double>::size_type>&) const {
      std::set<std::string> ev_params;
      nparams.clear();
      this->getParametersNames(ev_params);
      for (const auto& p : params) {
        if (ev_params.find(p) != ev_params.end()) {
          if (std::find(nparams.begin(), nparams.end(), p) == nparams.end()) {
            nparams.push_back(p);
          }
        }
      }
      return this->createFunctionByChangingParametersIntoVariables(nparams);
    }  // end of Evaluator::createFunctionByChangingParametersIntoVariables

    std::shared_ptr<tfel::math::parser::ExternalFunction>
    Evaluator::createFunctionByChangingParametersIntoVariables(
        const std::vector<std::string>& params) const {
      using namespace tfel::math::parser;
      auto throw_if = [](const bool b, const std::string& m) {
        raise_if(b,
                 "Evaluator::createFunctionByChanging"
                 "ParametersIntoVariables: " +
                     m);
      };
      std::set<std::string> ev_params;
      throw_if(
          this->variables.size() != this->positions.size(),
          "internal error: variables size is not equal to the positions size");
      this->getParametersNames(ev_params);
      for (const auto& p : params) {
        throw_if(ev_params.find(p) == ev_params.end(),
                 "no parameter '" + p + "'");
        throw_if(this->positions.find(p) != this->positions.end(),
                 "'" + p + "' is alredy a variable of this function");
      }
      auto pev = std::make_shared<Evaluator>();
      pev->variables.resize(this->variables.size() + params.size());
      pev->positions = this->positions;
      auto i = this->variables.size();
      for (const auto& p : params) {
        throw_if(!(pev->positions.insert({p, i}).second),
                 "internal error (variable '" + p + "' alredy declared)");
        ++i;
      }
      pev->manager = this->manager;
      pev->expr = this->expr->createFunctionByChangingParametersIntoVariables(
          pev->variables, params, pev->positions);
      return std::move(pev);
    }  // end of Evaluator::createFunctionByChangingParametersIntoVariables

    void Evaluator::getParametersNames(std::set<std::string>& n) const {
      raise_if(this->expr == nullptr,
               "Evaluator::getParametersNames: "
               "uninitialized evaluator");
      return this->expr->getParametersNames(n);
    }  // end of Evaluator::getParametersNames

    Evaluator::~Evaluator() = default;

  }  // end of namespace math

}  // end of namespace tfel
