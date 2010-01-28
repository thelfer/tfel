/*!
 * \file   Evaluator.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   02 oct 2007
 */

#ifndef _LIB_TFEL_MATH_EVALUATOR_HXX_
#define _LIB_TFEL_MATH_EVALUATOR_HXX_ 

#include<vector>
#include<string>
#include<map>

#include"TFEL/Utilities/SmartPtr.hxx"
#include"TFEL/Math/Parser/Expr.hxx"  
#include"TFEL/Math/Parser/EvaluatorBase.hxx"  
#include"TFEL/Math/Parser/ExternalFunction.hxx"
#include"TFEL/Math/Parser/ExternalFunctionManager.hxx"

namespace tfel
{

  namespace math
  {

    struct Evaluator
      : public tfel::math::parser::EvaluatorBase,
	public tfel::math::parser::ExternalFunction
    {
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
      template<typename LogicalOperator>
      struct TLogicalOperation;
      template<typename LogicalOperator>
      struct TLogicalBinaryOperation;
      struct TConditionalExpr;
      typedef tfel::utilities::SmartPtr<tfel::math::parser::Expr>
      (*FunctionGenerator)(const tfel::utilities::SmartPtr<tfel::math::parser::Expr>);
      typedef tfel::utilities::SmartPtr<tfel::math::parser::Expr>
      (*BinaryFunctionGenerator)(const tfel::utilities::SmartPtr<tfel::math::parser::Expr>,
				 const tfel::utilities::SmartPtr<tfel::math::parser::Expr>);
      typedef tfel::utilities::SmartPtr<tfel::math::parser::Expr>
      (*ExternalFunctionGenerator)(const std::vector<std::string>&,
				   std::vector<tfel::utilities::SmartPtr<tfel::math::parser::Expr> >&);
      struct FunctionGeneratorManager;
      static FunctionGeneratorManager&
      getFunctionGeneratorManager(void);
      static bool
      isValidIdentifier(const std::string&);
      static void
      checkParameterNumber(const std::vector<double>::size_type,
			   const std::vector<double>::size_type);
      static void
      checkVariableNumber(const std::vector<double>::size_type,
			  const std::vector<double>::size_type);
      static void
      checkNotEndOfExpression(const std::string&,
			      const std::string&,
			      const std::vector<std::string>::const_iterator,
			      const std::vector<std::string>::const_iterator);
      static void
      checkNotEndOfExpression(const std::string&,
			      const std::vector<std::string>::const_iterator,
			      const std::vector<std::string>::const_iterator);
      static void
      readSpecifiedToken(const std::string&,
			 const std::string&,
			 std::vector<std::string>::const_iterator&, 
			 const std::vector<std::string>::const_iterator);
      static unsigned short
      readUnsignedShortValue(const std::string&,
			     std::vector<std::string>::const_iterator&,
			     const std::vector<std::string>::const_iterator);
      static unsigned short
      convertToUnsignedShort(const std::string&,
			     const std::string&);
      static unsigned int
      convertToUnsignedInt(const std::string&,
			   const std::string&);
      static int
      convertToInt(const std::string&,
		   const std::string&);
      /*
       * \param const std::string&, function definition
       */
      Evaluator(const std::string&);
      /*
       * \param const vector<std::string>&, variable names
       * \param const std::string&, function definition
       */
      Evaluator(const std::vector<std::string>&,
		const std::string&);
      /*
       * \param const std::string&, function definition
       * \param tfel::utilities::SmartPtr<ExternalFunctionManager>&, external
       * function definition
       */
      Evaluator(const std::string&,
		tfel::utilities::SmartPtr<tfel::math::parser::ExternalFunctionManager>&);
      /*
       * \param const vector<std::string>&, variable name
       * \param const std::string&, function definition
       * \param tfel::utilities::SmartPtr<ExternalFunctionManager>&, external
       * function definition
       */
      Evaluator(const std::vector<std::string>&,
		const std::string&,
		tfel::utilities::SmartPtr<tfel::math::parser::ExternalFunctionManager>&);
      //! Default constructor
      Evaluator();
      //! Copy constructor
      Evaluator(const Evaluator&);
      //! Assignement operator constructor
      Evaluator&
      operator=(const Evaluator&);
      /*
       * \param const std::string&, function definition
       */
      void
      setFunction(const std::string&);
      /*
       * \param const vector<std::string>&, variable names
       * \param const std::string&, function definition
       */
      void
      setFunction(const std::vector<std::string>&,
		  const std::string&);
      /*
       * \param const std::string&, function definition
       * \param tfel::utilities::SmartPtr<ExternalFunctionManager>&, external
       * function definition
       */
      void
      setFunction(const std::string&,
		  tfel::utilities::SmartPtr<tfel::math::parser::ExternalFunctionManager>&);
      /*
       * \param const vector<std::string>&, variable name
       * \param const std::string&, function definition
       * \param tfel::utilities::SmartPtr<ExternalFunctionManager>&, external
       * function definition
       */
      void
      setFunction(const std::vector<std::string>&,
		  const std::string&,
		  tfel::utilities::SmartPtr<tfel::math::parser::ExternalFunctionManager>&);
      double
      getValue(void) const;
      std::vector<std::string>
      getVariablesNames(void) const;
      std::vector<double>::size_type
      getNumberOfVariables(void) const;
      void
      checkCyclicDependency(void) const
	throw(std::runtime_error);
      void
      checkCyclicDependency(const std::string&) const
	throw(std::runtime_error);
      void
      checkCyclicDependency(const std::vector<std::string>&) const
	throw(std::runtime_error);
      tfel::utilities::SmartPtr<tfel::math::parser::ExternalFunction>
      differentiate(const std::vector<double>::size_type) const;
      tfel::utilities::SmartPtr<tfel::math::parser::ExternalFunction>
      differentiate(const std::string&) const;
      tfel::utilities::SmartPtr<ExternalFunction>
      resolveDependencies(void) const;
      void
      removeDependencies(void);
      void
      setVariableValue(const std::vector<double>::size_type,
		       const double);
      void
      setVariableValue(const std::string&,const double);
      ~Evaluator();
    private:
      static ExternalFunctionRegister externalFunctionRegister;
      static double max(const double,const double);
      static double min(const double,const double);
      static double Heavyside(const double);
      std::vector<double> variables;
      std::map<std::string,std::vector<double>::size_type> positions;
      tfel::utilities::SmartPtr<tfel::math::parser::Expr> expr;
      tfel::utilities::SmartPtr<tfel::math::parser::ExternalFunctionManager> manager;
      template<typename T>
      static bool
      convert(const std::string&);
      static bool
      isNumber(const std::string&);
      void
      treatDiff(std::vector<std::string>::const_iterator&,
		const std::vector<std::string>::const_iterator,
		Evaluator::TGroup * const,
		const bool);
      std::vector<double>::size_type
      registerVariable(const std::string&);
      tfel::utilities::SmartPtr<tfel::math::parser::ExternalFunctionManager>
      getExternalFunctionManager();
      std::vector<double>::size_type
      getVariablePosition(const std::string&) const;
      std::vector<std::string>
      analyseParameters(std::vector<std::string>::const_iterator&,
			const std::vector<std::string>::const_iterator);
      std::vector<tfel::utilities::SmartPtr<Evaluator::TExpr> >
      analyseArguments(std::vector<std::string>::const_iterator&,
		       const std::vector<std::string>::const_iterator,
		       const bool);
      std::vector<tfel::utilities::SmartPtr<Evaluator::TExpr> >
      analyseArguments(const unsigned short,
		       std::vector<std::string>::const_iterator&,
		       const std::vector<std::string>::const_iterator,
		       const bool);
      tfel::utilities::SmartPtr<Evaluator::TExpr>
      treatGroup(std::vector<std::string>::const_iterator&,
		 std::vector<std::string>::const_iterator,
		 const bool = false,
		 const std::string& = ")");
      unsigned short
      countNumberOfArguments(std::vector<std::string>::const_iterator,
			     const std::vector<std::string>::const_iterator);
      void
      addExternalFunctionToGroup(TGroup * const,
				 std::vector<std::string>::const_iterator&,
				 const std::vector<std::string>::const_iterator,
				 const bool);
      void
      analyse(const std::string&,
	      const bool = false);
      std::pair<bool,std::vector<std::string>::const_iterator>
      search(std::vector<std::string>::const_iterator,
	     std::vector<std::string>::const_iterator,
	     const std::string&,
	     const std::string&);
      std::vector<std::string>::const_iterator
      searchComparisonOperator(const std::vector<std::string>::const_iterator,
			       const std::vector<std::string>::const_iterator);
      tfel::utilities::SmartPtr<Evaluator::TLogicalExpr>
      treatLogicalExpression(const std::vector<std::string>::const_iterator,
			     const std::vector<std::string>::const_iterator,
			     const bool);
      tfel::utilities::SmartPtr<Evaluator::TLogicalExpr>
      treatLogicalExpression2(const std::vector<std::string>::const_iterator,
			      const std::vector<std::string>::const_iterator,
			      const bool);
      tfel::utilities::SmartPtr<Evaluator::TExpr>
      treatGroup2(std::vector<std::string>::const_iterator&,
		  std::vector<std::string>::const_iterator,
		  const bool = false,
		  const std::string& = ")");
    }; // end of struct Evaluator

    struct Evaluator::FunctionGeneratorManager
    {
      void
      insert(const std::string&,
	     const Evaluator::FunctionGenerator);
      void
      insert(const std::string&,
	     const Evaluator::BinaryFunctionGenerator);
      void
      insert(const std::string&,
	     const Evaluator::ExternalFunctionGenerator);
    private:
      friend class Evaluator;
      FunctionGeneratorManager();
      void
      checkName(const std::string&) const;
      std::map<std::string,Evaluator::FunctionGenerator> fctGenerators;
      std::map<std::string,Evaluator::BinaryFunctionGenerator> bFctGenerators;
      std::map<std::string,Evaluator::ExternalFunctionGenerator> extOpGenerators;
    }; // end of struct FunctionManager

    template<double (*f)(const double,const double)>
    static tfel::utilities::SmartPtr<tfel::math::parser::Expr>
    StandardBinaryFctGenerator(tfel::utilities::SmartPtr<tfel::math::parser::Expr>,
			       tfel::utilities::SmartPtr<tfel::math::parser::Expr>);
    
    template<double (*f)(const double)>
    static tfel::utilities::SmartPtr<tfel::math::parser::Expr>
    StandardFctGenerator(const tfel::utilities::SmartPtr<tfel::math::parser::Expr>);
    
  } // end of namespace math

} // end of namespace tfel

#include"TFEL/Math/Evaluator.ixx"
#include"TFEL/Math/Parser/EvaluatorTExpr.hxx"

#endif /* _LIB_TFEL_MATH_EVALUATOR_HXX */
