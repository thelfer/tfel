/*!
 * \file   include/TFEL/Math/Parser/IntegerEvaluatorTExpr.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   13 jan 2009
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include"TFEL/Config/TFELConfig.hxx"

#ifndef LIB_TFEL_MATH_INTEGEREVALUATORTEXPR_HXX_
#define LIB_TFEL_MATH_INTEGEREVALUATORTEXPR_HXX_ 

namespace tfel
{

  namespace math
  {

    struct IntegerEvaluator::TExpr
    {
      TExpr() = default;
      TExpr(const TExpr&) = default;
      TExpr(TExpr&&) = default;
      TExpr& operator=(TExpr&&) = default;
      TExpr& operator=(const TExpr&) = default;
      virtual parser::IntegerExprPtr
      analyse(void) = 0;
      virtual void reduce(void) = 0;
      virtual bool isOperator(void) const =0;
      virtual ~TExpr();
    }; // end of IntegerEvaluator::TExpr
  
    struct IntegerEvaluator::TNegation
      : public IntegerEvaluator::TExpr
    {
      //! expression returned
      struct Negation
	: public tfel::math::parser::IntegerExpr
      {
	Negation(const parser::IntegerExprPtr);
	int getValue(void) const;
	parser::IntegerExprPtr
	clone(const std::vector<int>&) const;
	~Negation();
      private:
	const parser::IntegerExprPtr expr;
      };
      TNegation(std::shared_ptr<IntegerEvaluator::TExpr>);
      bool
      isOperator(void) const;
      parser::IntegerExprPtr
      analyse(void);
      void
      reduce(void);
      ~TNegation();
    private:
      std::shared_ptr<IntegerEvaluator::TExpr> expr;
    }; // end of struct IntegerEvaluator::TNegation

    struct IntegerEvaluator::TBinaryOperation
      : public IntegerEvaluator::TExpr
    {
      struct IntegerOpPlus
      {
	static int TFEL_VISIBILITY_LOCAL
	apply(const int,const int);
      }; // end of struct IntegerOpPlus
      struct IntegerOpMinus
      {
	static int TFEL_VISIBILITY_LOCAL
	apply(const int,const int);
      }; // end of struct IntegerOpMinus
      struct IntegerOpMult
      {
	static int TFEL_VISIBILITY_LOCAL
	apply(const int,const int);
      }; // end of struct IntegerOpMult
      struct IntegerOpDiv
      {
	static int TFEL_VISIBILITY_LOCAL
	apply(const int,const int);
      }; // end of struct IntegerOpDiv
     /*!
      * Expression generated
      */
      template<typename Op>
      struct TFEL_VISIBILITY_LOCAL BinaryOperation
	: public tfel::math::parser::IntegerExpr
      {
	BinaryOperation(const parser::IntegerExprPtr,
			const parser::IntegerExprPtr);
	int
	getValue(void) const;
	parser::IntegerExprPtr
	clone(const std::vector<int>&) const;
	~BinaryOperation();
      private:
	const parser::IntegerExprPtr a;
	const parser::IntegerExprPtr b;
      }; // end of struct BinaryOperation
      /*!
       * Constructor
       */
      TBinaryOperation(std::shared_ptr<IntegerEvaluator::TExpr>,
		       const std::shared_ptr<TOperator>,
		       std::shared_ptr<IntegerEvaluator::TExpr>);
      bool
      isOperator(void) const;
      void
      reduce(void);
      parser::IntegerExprPtr
      analyse(void);
      ~TBinaryOperation();
    private:
      std::shared_ptr<IntegerEvaluator::TExpr> a;
      const std::shared_ptr<TOperator>op;
      std::shared_ptr<IntegerEvaluator::TExpr> b;
    }; // end of struct IntegerEvaluator::TBinaryOperation

    struct IntegerEvaluator::TVariable
      : public IntegerEvaluator::TExpr
    {
      struct Variable
	: public tfel::math::parser::IntegerExpr
      {
	Variable(const std::vector<int>&,
		 const std::vector<int>::size_type);
	int
	getValue(void) const;
	parser::IntegerExprPtr
	clone(const std::vector<int>&) const;
      private:
	const std::vector<int>& v;
	const std::vector<int>::size_type pos;
      }; // end of struct Variable
      TVariable(const std::string&,
		IntegerEvaluator &);
      TVariable(const std::vector<int>::size_type,
		std::vector<int>&);
      bool
      isOperator(void) const;
      void reduce(void);
      parser::IntegerExprPtr
      analyse(void);
    private:
      std::vector<int>& vars;
      std::vector<int>::size_type pos;
    };

    struct IntegerEvaluator::TOperator
      : public IntegerEvaluator::TExpr
    {
      TOperator(const std::string&);
      std::string
      getOperatorType(void) const;
      bool
      isOperator(void) const;
      void
      reduce(void);
      parser::IntegerExprPtr
      analyse(void);
    private:
      const std::string type;
    }; // end of struct IntegerEvaluator::TOperator

    struct IntegerEvaluator::TGroup
      : public IntegerEvaluator::TExpr
    {
      bool
      isOperator(void) const;
      void
      add(std::shared_ptr<IntegerEvaluator::TExpr>const);
      void
      reduce(void);
      parser::IntegerExprPtr
      analyse(void);
      ~TGroup();
    private:
      void
      reduce(const std::string&);
      std::vector<std::shared_ptr<IntegerEvaluator::TExpr> > subExpr;
    }; // end of struct IntegerEvaluator::TGroup

    struct IntegerEvaluator::TNumber
      : public IntegerEvaluator::TExpr
    {
      struct Number
	: public tfel::math::parser::IntegerExpr
      {
	Number(const int);
	parser::IntegerExprPtr
	clone(const std::vector<int>&) const;
	int
	getValue(void) const;
      private:
	const int value;
      }; // end of struct Number
      TNumber(const int v);
      bool
      isOperator(void) const;
      parser::IntegerExprPtr
      analyse(void);
      void
      reduce(void);
    private:
      const int value;
    }; // end of struct IntegerEvaluator::TNumber

  } // end of namespace math

} // end of namespace tfel

#endif /* LIB_TFEL_MATH_INTEGEREVALUATORTEXPR_HXX_ */
