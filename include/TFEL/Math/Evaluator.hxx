/*!
 * \file   include/TFEL/Math/Evaluator.hxx
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

#ifndef LIB_TFEL_MATH_EVALUATOR_HXX
#define LIB_TFEL_MATH_EVALUATOR_HXX

#include <map>
#include <memory>
#include <vector>
#include <string>
#include <functional>

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Math/Parser/Expr.hxx"
#include "TFEL/Math/Parser/EvaluatorBase.hxx"
#include "TFEL/Math/Parser/ExternalFunction.hxx"
#include "TFEL/Math/Parser/ExternalFunctionManager.hxx"

namespace tfel::math {

  /*!
   * \brief class in charge of handling a function defined by a
   * string.
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
    using ExprPtr = std::shared_ptr<tfel::math::parser::Expr>;
    using FunctionGenerator = std::function<ExprPtr(const ExprPtr)>;
    using BinaryFunctionGenerator =
        std::function<ExprPtr(const ExprPtr, const ExprPtr)>;
    typedef ExprPtr (*ExternalFunctionGenerator)(
        const std::vector<std::string>&, std::vector<ExprPtr>&);
    struct FunctionGeneratorManager;
    static FunctionGeneratorManager& getFunctionGeneratorManager();

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
    static bool isInteger(const std::string&);
    static int convertToInt(const std::string&, const std::string&);
    /*!
     * \brief return an expression representing the power of the given argument
     * with an integer exponent.
     * \param[in] e: argument
     * \param[in] n: exponent
     */
    static std::shared_ptr<tfel::math::parser::Expr>
    makePowerFunctionExpression(std::shared_ptr<tfel::math::parser::Expr>,
                                const int);
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
     * \param external function definition
     */
    explicit Evaluator(
        const std::string&,
        std::shared_ptr<tfel::math::parser::ExternalFunctionManager>);
    /*!
     * \param[in] const vector<std::string>&, variable name
     * \param[in] const std::string&, function definition
     * \param[in] external function definition
     */
    explicit Evaluator(
        const std::vector<std::string>&,
        const std::string&,
        std::shared_ptr<tfel::math::parser::ExternalFunctionManager>);
    /*!
     * \brief build an evaluator from a value.
     * \param[in] v: value
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
    /*!
     * \brief evaluate the formula
     * \return the result of the evaluation
     * \note variables values shall have been set with the
     * `setVariableValue` method.
     */
    double getValue() const override;
    /*!
     * \brief evaluate the formula
     * \param[in] vs: a map giving the values of some of the
     * variables recquired to evaluate the formula.
     * \return the result of the evaluation
     * \note the values of the variables not given in argument shall
     * have been set with the `setVariableValue` method.
     */
    double getValue(const std::map<std::string, double>&);
    /*!
     * \brief evaluate the formula
     * \return the result of the evaluation
     * \note variables values shall have been set with the
     * `setVariableValue` method.
     */
    double operator()() const;
    /*!
     * \brief evaluate the formula
     * \param[in] vs: a map giving the values of some of the
     * variables recquired to evaluate the formula.
     * \return the result of the evaluation
     * \note the values of the variables not given in argument shall
     * have been set with the `setVariableValue` method.
     */
    double operator()(const std::map<std::string, double>&);
    /*!
     * \return a string representation of the evaluator suitable to
     * be integrated in a C++ code.
     * \param[in] m: a map used to change the names of the variables
     */
    virtual std::string getCxxFormula(
        const std::map<std::string, std::string>& = {}) const;
    virtual std::vector<std::string> getVariablesNames() const;
    std::vector<double>::size_type getNumberOfVariables() const override;
    virtual void checkCyclicDependency() const;
    void checkCyclicDependency(const std::string&) const override;
    void checkCyclicDependency(std::vector<std::string>&) const override;
    std::shared_ptr<tfel::math::parser::ExternalFunction> differentiate(
        const std::vector<double>::size_type) const override;
    std::shared_ptr<tfel::math::parser::ExternalFunction> differentiate(
        const std::string&) const override;
    std::shared_ptr<tfel::math::parser::ExternalFunction> resolveDependencies()
        const override;
    virtual void removeDependencies();
    void setVariableValue(const std::vector<double>::size_type,
                          const double) override;
    virtual void setVariableValue(const std::string&, const double);
    virtual void setVariableValue(const char* const, const double);
    std::shared_ptr<ExternalFunction>
    createFunctionByChangingParametersIntoVariables(
        const std::vector<std::string>&) const override;
    std::shared_ptr<ExternalFunction>
    createFunctionByChangingParametersIntoVariables(
        std::vector<std::string>&,
        const std::vector<double>&,
        const std::vector<std::string>&,
        const std::map<std::string, std::vector<double>::size_type>&)
        const override;
    void getParametersNames(std::set<std::string>&) const override;
    ~Evaluator() override;

   private:
    template <typename T>
    TFEL_VISIBILITY_LOCAL static bool convert(const std::string&);
    TFEL_VISIBILITY_LOCAL static bool isNumber(const std::string&);
    //! \brief clear the object
    TFEL_VISIBILITY_LOCAL void clear();
    TFEL_VISIBILITY_LOCAL void treatDiff(
        std::vector<std::string>::const_iterator&,
        const std::vector<std::string>::const_iterator,
        Evaluator::TGroup* const,
        const bool);
    TFEL_VISIBILITY_LOCAL std::vector<double>::size_type registerVariable(
        const std::string&);
    TFEL_VISIBILITY_LOCAL
    std::shared_ptr<tfel::math::parser::ExternalFunctionManager>
    getExternalFunctionManager();
    TFEL_VISIBILITY_LOCAL std::vector<double>::size_type getVariablePosition(
        const std::string&) const;
    TFEL_VISIBILITY_LOCAL std::vector<std::string> analyseParameters(
        std::vector<std::string>::const_iterator&,
        const std::vector<std::string>::const_iterator);
    TFEL_VISIBILITY_LOCAL std::vector<std::shared_ptr<Evaluator::TExpr>>
    analyseArguments(std::vector<std::string>::const_iterator&,
                     const std::vector<std::string>::const_iterator,
                     const bool);
    TFEL_VISIBILITY_LOCAL std::vector<std::shared_ptr<Evaluator::TExpr>>
    analyseArguments(const unsigned short,
                     std::vector<std::string>::const_iterator&,
                     const std::vector<std::string>::const_iterator,
                     const bool);
    /*!
     * \brief analyse a formula by splitting it in tokens that are
     * then parsed by the `treatGroup` method.
     * \param[in] f: formula
     * \param[in] b: if true, the variable names are known and no
     * variable can be added dynamically.
     */
    TFEL_VISIBILITY_LOCAL void analyse(const std::string&, const bool = false);
    /*!
     * \brief analyse an group of tokens. This functions checks if a
     * conditional operator is found and treats it recursively. If
     * no conditional operator is found, the `treatGroup2` method is
     * called.
     * \param[in] p:  iterator to the current position
     * \param[in] pe: iterator past-the-end of the sequence
     * \param[in] b:  if true, the variable names are known and no
     * variable can be added dynamically.
     * \param[in] s:  string delimiting the end of the expression.
     */
    TFEL_VISIBILITY_LOCAL std::shared_ptr<Evaluator::TExpr> treatGroup(
        std::vector<std::string>::const_iterator&,
        std::vector<std::string>::const_iterator,
        const bool = false,
        const std::string& = ")");
    /*!
     *
     */
    TFEL_VISIBILITY_LOCAL unsigned short countNumberOfArguments(
        std::vector<std::string>::const_iterator,
        const std::vector<std::string>::const_iterator);
    /*!
     *
     */
    TFEL_VISIBILITY_LOCAL void addExternalFunctionToGroup(
        TGroup* const,
        std::vector<std::string>::const_iterator&,
        const std::vector<std::string>::const_iterator,
        const std::string&,
        const bool);
    TFEL_VISIBILITY_LOCAL
    std::pair<bool, std::vector<std::string>::const_iterator> search(
        std::vector<std::string>::const_iterator,
        std::vector<std::string>::const_iterator,
        const std::string&,
        const std::string&);
    TFEL_VISIBILITY_LOCAL std::vector<std::string>::const_iterator
    searchComparisonOperator(const std::vector<std::string>::const_iterator,
                             const std::vector<std::string>::const_iterator);
    TFEL_VISIBILITY_LOCAL std::shared_ptr<Evaluator::TLogicalExpr>
    treatLogicalExpression(const std::vector<std::string>::const_iterator,
                           const std::vector<std::string>::const_iterator,
                           const bool);
    TFEL_VISIBILITY_LOCAL std::shared_ptr<Evaluator::TLogicalExpr>
    treatLogicalExpression2(const std::vector<std::string>::const_iterator,
                            const std::vector<std::string>::const_iterator,
                            const bool);
    /*!
     * \brief analyse a subexpression
     * \param[in] p:  iterator to the current position
     * \param[in] pe: iterator past the last valid iterator
     * \param[in] b: if true, the variable names are known and no
     * variable can be added dynamically.
     * \param[in] s: string delimiting the end of the subexpression
     */
    TFEL_VISIBILITY_LOCAL std::shared_ptr<Evaluator::TExpr> treatGroup2(
        std::vector<std::string>::const_iterator&,
        std::vector<std::string>::const_iterator,
        const bool = false,
        const std::string& = ")");

    static ExternalFunctionRegister externalFunctionRegister
        TFEL_VISIBILITY_LOCAL;
    //! \brief variables values
    std::vector<double> variables;
    /*!
     * \brief a simple mapping between variables' names and their
     * position in the array of variables' values.
     */
    std::map<std::string, std::vector<double>::size_type> positions;
    /*!
     * \brief the expression resulting from the analysis of the
     * formula to be evaluated
     */
    ExprPtr expr;
    //! \brief a pointer to externally defined functions
    std::shared_ptr<tfel::math::parser::ExternalFunctionManager> manager;
  };  // end of struct Evaluator

}  // end of namespace tfel::math

#include "TFEL/Math/Parser/EvaluatorTExpr.hxx"

#endif /* LIB_TFEL_MATH_EVALUATOR_HXX */
