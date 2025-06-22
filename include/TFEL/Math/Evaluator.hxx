/*!
 * \file   include/TFEL/Math/Evaluator.hxx
 * \brief
 *
 * \author Helfer Thomas
 * \date   02 oct 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_EVALUATOR_HXX
#define LIB_TFEL_MATH_EVALUATOR_HXX

#include <map>
#include <memory>
#include <vector>
#include <string>

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Math/Parser/Expr.hxx"
#include "TFEL/Math/Parser/EvaluatorBase.hxx"
#include "TFEL/Math/Parser/ExternalFunction.hxx"
#include "TFEL/Math/Parser/ExternalFunctionManager.hxx"

namespace tfel {

  namespace math {

    /*!
     * class in charge of handling a function defined by a string.
     *
     */
    struct TFELMATHPARSER_VISIBILITY_EXPORT Evaluator
        : public tfel::math::parser::EvaluatorBase,
          public tfel::math::parser::ExternalFunction {
      struct ExternalFunctionRegister;
      struct TExpr;
      struct TOperator;
      struct TGroup;
      struct TFunction;
      struct TBinaryFunction;
      struct TExternalOperator;
      struct TNumber;
      struct TVariable;
      struct TBinaryOperation;
      struct TNegation;
      struct TExternalFunctionExpr;
      struct TDifferentiatedFunctionExpr;
      struct TLogicalExpr;
      struct TNegLogicalExpr;
      template <typename LogicalOperator>
      struct TLogicalOperation;
      template <typename LogicalOperator>
      struct TLogicalBinaryOperation;
      struct TConditionalExpr;
      typedef std::shared_ptr<tfel::math::parser::Expr> (*FunctionGenerator)(
          const std::shared_ptr<tfel::math::parser::Expr>);
      typedef std::shared_ptr<tfel::math::parser::Expr> (
          *BinaryFunctionGenerator)(
          const std::shared_ptr<tfel::math::parser::Expr>,
          const std::shared_ptr<tfel::math::parser::Expr>);
      typedef std::shared_ptr<tfel::math::parser::Expr> (
          *ExternalFunctionGenerator)(
          const std::vector<std::string>&,
          std::vector<std::shared_ptr<tfel::math::parser::Expr>>&);
      struct FunctionGeneratorManager;
      static FunctionGeneratorManager& getFunctionGeneratorManager(void);

      static double max(const double, const double);
      static double min(const double, const double);
      static double Heavyside(const double);

      static bool isValidIdentifier(const std::string&);
      static void checkParameterNumber(const std::vector<double>::size_type,
                                       const std::vector<double>::size_type);
      static void checkVariableNumber(const std::vector<double>::size_type,
                                      const std::vector<double>::size_type);
      static void checkNotEndOfExpression(
          const std::string&,
          const std::string&,
          const std::vector<std::string>::const_iterator,
          const std::vector<std::string>::const_iterator);
      static void checkNotEndOfExpression(
          const std::string&,
          const std::vector<std::string>::const_iterator,
          const std::vector<std::string>::const_iterator);
      static void readSpecifiedToken(
          const std::string&,
          const std::string&,
          std::vector<std::string>::const_iterator&,
          const std::vector<std::string>::const_iterator);
      static unsigned short readUnsignedShortValue(
          const std::string&,
          std::vector<std::string>::const_iterator&,
          const std::vector<std::string>::const_iterator);
      static unsigned short convertToUnsignedShort(const std::string&,
                                                   const std::string&);
      static unsigned int convertToUnsignedInt(const std::string&,
                                               const std::string&);
      static int convertToInt(const std::string&, const std::string&);
      /*!
       * \param const std::string&, function definition
       */
      explicit Evaluator(const std::string&);
      /*!
       * \param const vector<std::string>&, variable names
       * \param const std::string&, function definition
       */
      explicit Evaluator(const std::vector<std::string>&, const std::string&);
      /*!
       * \param const std::string&, function definition
       * \param std::shared_ptr<ExternalFunctionManager>&, external
       * function definition
       */
      explicit Evaluator(
          const std::string&,
          std::shared_ptr<tfel::math::parser::ExternalFunctionManager>&);
      /*!
       * \param const vector<std::string>&, variable name
       * \param const std::string&, function definition
       * \param std::shared_ptr<ExternalFunctionManager>&, external
       * function definition
       */
      explicit Evaluator(
          const std::vector<std::string>&,
          const std::string&,
          std::shared_ptr<tfel::math::parser::ExternalFunctionManager>&);
      /*!
       * Build an evaluator from a value
       * v : value
       */
      explicit Evaluator(const double);
      //! Default constructor
      Evaluator();
      //! Copy constructor
      Evaluator(const Evaluator&);
      //! Assignement operator constructor
      Evaluator& operator=(const Evaluator&);
      /*!
       * \param const std::string&, function definition
       */
      void setFunction(const std::string&);
      /*!
       * \param const vector<std::string>&, variable names
       * \param const std::string&, function definition
       */
      void setFunction(const std::vector<std::string>&, const std::string&);
      /*!
       * \param const std::string&, function definition
       * \param std::shared_ptr<ExternalFunctionManager>&, external
       * function definition
       */
      void setFunction(
          const std::string&,
          std::shared_ptr<tfel::math::parser::ExternalFunctionManager>&);
      /*!
       * \param const vector<std::string>&, variable name
       * \param const std::string&, function definition
       * \param std::shared_ptr<ExternalFunctionManager>&, external
       * function definition
       */
      void setFunction(
          const std::vector<std::string>&,
          const std::string&,
          std::shared_ptr<tfel::math::parser::ExternalFunctionManager>&);
      virtual double getValue(void) const override;
      virtual std::vector<std::string> getVariablesNames(void) const;
      virtual std::vector<double>::size_type getNumberOfVariables(
          void) const override;
      virtual void checkCyclicDependency(void) const;
      virtual void checkCyclicDependency(const std::string&) const override;
      virtual void checkCyclicDependency(
          std::vector<std::string>&) const override;
      virtual std::shared_ptr<tfel::math::parser::ExternalFunction>
      differentiate(const std::vector<double>::size_type) const override;
      virtual std::shared_ptr<tfel::math::parser::ExternalFunction>
      differentiate(const std::string&) const override;
      virtual std::shared_ptr<tfel::math::parser::ExternalFunction>
      resolveDependencies(void) const override;
      virtual void removeDependencies(void);
      virtual void setVariableValue(const std::vector<double>::size_type,
                                    const double) override;
      virtual void setVariableValue(const std::string&, const double);
      virtual void setVariableValue(const char* const, const double);
      virtual std::shared_ptr<ExternalFunction>
      createFunctionByChangingParametersIntoVariables(
          const std::vector<std::string>&) const override;
      virtual std::shared_ptr<ExternalFunction>
      createFunctionByChangingParametersIntoVariables(
          std::vector<std::string>&,
          const std::vector<double>&,
          const std::vector<std::string>&,
          const std::map<std::string, std::vector<double>::size_type>&)
          const override;
      virtual void getParametersNames(std::set<std::string>&) const override;
      virtual ~Evaluator();

     private:
      static ExternalFunctionRegister externalFunctionRegister
          TFEL_VISIBILITY_LOCAL;
      std::vector<double> variables;
      std::map<std::string, std::vector<double>::size_type> positions;
      std::shared_ptr<tfel::math::parser::Expr> expr;
      std::shared_ptr<tfel::math::parser::ExternalFunctionManager> manager;
      template <typename T>
      static bool TFEL_VISIBILITY_LOCAL convert(const std::string&);
      static bool TFEL_VISIBILITY_LOCAL isNumber(const std::string&);
      //! \brief clear the object
      TFEL_VISIBILITY_LOCAL void clear(void);
      void TFEL_VISIBILITY_LOCAL
      treatDiff(std::vector<std::string>::const_iterator&,
                const std::vector<std::string>::const_iterator,
                Evaluator::TGroup* const,
                const bool);
      std::vector<double>::size_type TFEL_VISIBILITY_LOCAL
      registerVariable(const std::string&);
      std::shared_ptr<tfel::math::parser::ExternalFunctionManager>
          TFEL_VISIBILITY_LOCAL getExternalFunctionManager();
      std::vector<double>::size_type TFEL_VISIBILITY_LOCAL
      getVariablePosition(const std::string&) const;
      std::vector<std::string> TFEL_VISIBILITY_LOCAL
      analyseParameters(std::vector<std::string>::const_iterator&,
                        const std::vector<std::string>::const_iterator);
      std::vector<std::shared_ptr<Evaluator::TExpr>> TFEL_VISIBILITY_LOCAL
      analyseArguments(std::vector<std::string>::const_iterator&,
                       const std::vector<std::string>::const_iterator,
                       const bool);
      std::vector<std::shared_ptr<Evaluator::TExpr>> TFEL_VISIBILITY_LOCAL
      analyseArguments(const unsigned short,
                       std::vector<std::string>::const_iterator&,
                       const std::vector<std::string>::const_iterator,
                       const bool);
      std::shared_ptr<Evaluator::TExpr> TFEL_VISIBILITY_LOCAL
      treatGroup(std::vector<std::string>::const_iterator&,
                 std::vector<std::string>::const_iterator,
                 const bool = false,
                 const std::string& = ")");
      unsigned short TFEL_VISIBILITY_LOCAL
      countNumberOfArguments(std::vector<std::string>::const_iterator,
                             const std::vector<std::string>::const_iterator);
      void TFEL_VISIBILITY_LOCAL
      addExternalFunctionToGroup(TGroup* const,
                                 std::vector<std::string>::const_iterator&,
                                 const std::vector<std::string>::const_iterator,
                                 const bool);
      void TFEL_VISIBILITY_LOCAL analyse(const std::string&,
                                         const bool = false);
      std::pair<bool, std::vector<std::string>::const_iterator>
          TFEL_VISIBILITY_LOCAL search(std::vector<std::string>::const_iterator,
                                       std::vector<std::string>::const_iterator,
                                       const std::string&,
                                       const std::string&);
      std::vector<std::string>::const_iterator TFEL_VISIBILITY_LOCAL
      searchComparisonOperator(const std::vector<std::string>::const_iterator,
                               const std::vector<std::string>::const_iterator);
      std::shared_ptr<Evaluator::TLogicalExpr> TFEL_VISIBILITY_LOCAL
      treatLogicalExpression(const std::vector<std::string>::const_iterator,
                             const std::vector<std::string>::const_iterator,
                             const bool);
      std::shared_ptr<Evaluator::TLogicalExpr> TFEL_VISIBILITY_LOCAL
      treatLogicalExpression2(const std::vector<std::string>::const_iterator,
                              const std::vector<std::string>::const_iterator,
                              const bool);
      std::shared_ptr<Evaluator::TExpr> TFEL_VISIBILITY_LOCAL
      treatGroup2(std::vector<std::string>::const_iterator&,
                  std::vector<std::string>::const_iterator,
                  const bool = false,
                  const std::string& = ")");
    };  // end of struct Evaluator

    struct Evaluator::FunctionGeneratorManager {
      void insert(const std::string&, const Evaluator::FunctionGenerator);
      void insert(const std::string&, const Evaluator::BinaryFunctionGenerator);
      void insert(const std::string&,
                  const Evaluator::ExternalFunctionGenerator);

     private:
      friend struct Evaluator;
      FunctionGeneratorManager();
      TFEL_VISIBILITY_LOCAL void checkName(const std::string&) const;
      std::map<std::string, Evaluator::FunctionGenerator> fctGenerators;
      std::map<std::string, Evaluator::BinaryFunctionGenerator> bFctGenerators;
      std::map<std::string, Evaluator::ExternalFunctionGenerator>
          extOpGenerators;
    };  // end of struct FunctionManager

    template <double (*f)(const double, const double)>
    static std::shared_ptr<tfel::math::parser::Expr> StandardBinaryFctGenerator(
        std::shared_ptr<tfel::math::parser::Expr>,
        std::shared_ptr<tfel::math::parser::Expr>);

    template <double (*f)(const double)>
    static std::shared_ptr<tfel::math::parser::Expr> StandardFctGenerator(
        const std::shared_ptr<tfel::math::parser::Expr>);

  }  // end of namespace math

}  // end of namespace tfel

#include "TFEL/Math/Evaluator.ixx"
#include "TFEL/Math/Parser/EvaluatorTExpr.hxx"

#endif /* LIB_TFEL_MATH_EVALUATOR_HXX */
