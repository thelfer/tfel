/*!
 * \file   include/TFEL/Math/IntegerEvaluator.hxx
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

#ifndef LIB_TFEL_MATH_INTEGEREVALUATOR_HXX
#define LIB_TFEL_MATH_INTEGEREVALUATOR_HXX

#include <map>
#include <vector>
#include <string>
#include <memory>

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Math/Parser/EvaluatorBase.hxx"

namespace tfel::math::parser {

  struct TFEL_VISIBILITY_LOCAL IntegerExpr {
    virtual int getValue() const = 0;
    virtual std::shared_ptr<IntegerExpr> clone(
        const std::vector<int>&) const = 0;
    virtual ~IntegerExpr();
  };  // end of struct IntegerExpr
  //! a simple alias
  using IntegerExprPtr = std::shared_ptr<IntegerExpr>;

}  // end of namespace tfel::math::parser

namespace tfel::math {

  /*!
   * \brief class in charge of handling a function defined by a string.
   */
  struct TFELMATHPARSER_VISIBILITY_EXPORT IntegerEvaluator
      : public tfel::math::parser::EvaluatorBase {
    struct TExpr;
    struct TOperator;
    struct TGroup;
    struct TNumber;
    struct TVariable;
    struct TBinaryOperation;
    struct TNegation;
    static void checkNotEndOfExpression(
        const std::string&,
        const std::string&,
        const std::vector<std::string>::const_iterator,
        const std::vector<std::string>::const_iterator);
    static void checkNotEndOfExpression(
        const std::string&,
        const std::vector<std::string>::const_iterator,
        const std::vector<std::string>::const_iterator);
    /*!
     * \param const std::string&, function definition
     */
    explicit IntegerEvaluator(const std::string&);
    /*!
     * \param const vector<std::string>&, variable names
     * \param const std::string&, function definition
     */
    explicit IntegerEvaluator(const std::vector<std::string>&,
                              const std::string&);
    //! Default constructor
    IntegerEvaluator();
    //! Copy constructor
    IntegerEvaluator(const IntegerEvaluator&);
    //! Assignement operator constructor
    IntegerEvaluator& operator=(const IntegerEvaluator&);
    /*!
     * \param const std::string&, function definition
     */
    void setFunction(const std::string&);
    /*!
     * \param const vector<std::string>&, variable names
     * \param const std::string&, function definition
     */
    void setFunction(const std::vector<std::string>&, const std::string&);
    int getValue() const;
    std::vector<std::string> getVariablesNames() const;
    std::vector<int>::size_type getNumberOfVariables() const;
    void setVariableValue(const std::vector<int>::size_type, const int);
    void setVariableValue(const std::string&, const int);
    ~IntegerEvaluator();

   private:
    std::vector<int> variables;
    std::map<std::string, std::vector<int>::size_type> positions;
    std::shared_ptr<tfel::math::parser::IntegerExpr> expr;
    template <typename T>
    TFEL_VISIBILITY_LOCAL static bool convert(const std::string&);
    TFEL_VISIBILITY_LOCAL static bool isNumber(const std::string&);
    TFEL_VISIBILITY_LOCAL std::vector<int>::size_type registerVariable(
        const std::string&);
    TFEL_VISIBILITY_LOCAL std::vector<int>::size_type getVariablePosition(
        const std::string&) const;
    TFEL_VISIBILITY_LOCAL std::vector<std::string> analyseParameters(
        std::vector<std::string>::const_iterator&,
        const std::vector<std::string>::const_iterator);
    TFEL_VISIBILITY_LOCAL std::vector<std::shared_ptr<IntegerEvaluator::TExpr>>
    analyseArguments(std::vector<std::string>::const_iterator&,
                     const std::vector<std::string>::const_iterator,
                     const bool);
    TFEL_VISIBILITY_LOCAL std::vector<std::shared_ptr<IntegerEvaluator::TExpr>>
    analyseArguments(const unsigned short,
                     std::vector<std::string>::const_iterator&,
                     const std::vector<std::string>::const_iterator,
                     const bool);
    TFEL_VISIBILITY_LOCAL std::shared_ptr<IntegerEvaluator::TExpr> treatGroup(
        std::vector<std::string>::const_iterator&,
        std::vector<std::string>::const_iterator,
        const bool = false,
        const std::string& = ")");
    TFEL_VISIBILITY_LOCAL unsigned short countNumberOfArguments(
        std::vector<std::string>::const_iterator,
        const std::vector<std::string>::const_iterator);
    TFEL_VISIBILITY_LOCAL void analyse(const std::string&, const bool = false);
    TFEL_VISIBILITY_LOCAL
    std::pair<bool, std::vector<std::string>::const_iterator> search(
        std::vector<std::string>::const_iterator,
        std::vector<std::string>::const_iterator,
        const std::string&,
        const std::string&);
  };  // end of struct IntegerEvaluator

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_INTEGEREVALUATOR_HXX */
