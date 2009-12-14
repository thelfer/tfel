/*!
 * \file   Function.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   02 oct 2007
 */

#include<iostream>

#include"Math/Parser/Negation.hxx"
#include"Math/Parser/Number.hxx"
#include"Math/Parser/BinaryOperator.hxx"
#include"Math/Parser/Function.hxx"

namespace tfel
{
  namespace math
  {

    namespace parser
    {

      Function::~Function()
      {} // end of Function::~Function

      template<>
      tfel::utilities::SmartPtr<Expr>
      differentiateFunction<std::exp>(const tfel::utilities::SmartPtr<Expr> expr,
				      const std::vector<double>::size_type pos,
				      const std::vector<double>& v)
      {
	using namespace tfel::utilities;
	SmartPtr<Expr> ce(expr->clone(v));
	SmartPtr<Expr> de(expr->differentiate(pos,v));
	SmartPtr<Expr> expe(new StandardFunction<std::exp>(ce));
	return SmartPtr<Expr>(new BinaryOperation<OpMult>(expe,de));
      } // end of differentiateFunction

      template<>
      tfel::utilities::SmartPtr<Expr>
      differentiateFunction<std::sin>(const tfel::utilities::SmartPtr<Expr> expr,
				      const std::vector<double>::size_type pos,
				      const std::vector<double>& v)
      {
	using namespace tfel::utilities;
	SmartPtr<Expr> ce(expr->clone(v));
	SmartPtr<Expr> de(expr->differentiate(pos,v));
	SmartPtr<Expr> cose(new StandardFunction<std::cos>(ce));
	return SmartPtr<Expr>(new BinaryOperation<OpMult>(cose,de));
      } // end of differentiateFunction

      template<>
      tfel::utilities::SmartPtr<Expr>
      differentiateFunction<std::cos>(const tfel::utilities::SmartPtr<Expr> expr,
				      const std::vector<double>::size_type pos,
				      const std::vector<double>& v)
      {
	using namespace tfel::utilities;
	SmartPtr<Expr> ce(expr->clone(v));
	SmartPtr<Expr> de(expr->differentiate(pos,v));
	SmartPtr<Expr> sine(new StandardFunction<std::sin>(ce));
	return SmartPtr<Expr>(new BinaryOperation<OpMult>(SmartPtr<Expr>(new Negation(sine)),de));
      } // end of differentiateFunction

      template<>
      tfel::utilities::SmartPtr<Expr>
      differentiateFunction<std::tan>(const tfel::utilities::SmartPtr<Expr> expr,
				      const std::vector<double>::size_type pos,
				      const std::vector<double>& v)
      {
	using namespace tfel::utilities;
	SmartPtr<Expr> ce(expr->clone(v));
	SmartPtr<Expr> de(expr->differentiate(pos,v));
	SmartPtr<Expr> tane(new StandardFunction<std::tan>(ce));
	SmartPtr<Expr> tan2e(new BinaryOperation<OpMult>(tane,tane));
	SmartPtr<Expr> tan2e_(new BinaryOperation<OpPlus>(SmartPtr<Expr>(new Number(1.)),tan2e));
	return SmartPtr<Expr>(new BinaryOperation<OpMult>(tan2e_,de));
      } // end of differentiateFunction

      template<>
      tfel::utilities::SmartPtr<Expr>
      differentiateFunction<std::sqrt>(const tfel::utilities::SmartPtr<Expr> expr,
				       const std::vector<double>::size_type pos,
				       const std::vector<double>& v)
      {
	using namespace tfel::utilities;
	SmartPtr<Expr> ce(expr->clone(v));
	SmartPtr<Expr> de(expr->differentiate(pos,v));
	SmartPtr<Expr> sqrte(new StandardFunction<std::sqrt>(ce));
	SmartPtr<Expr> sqrte_(new BinaryOperation<OpDiv>(SmartPtr<Expr>(new Number(0.5)),
							 sqrte));
	return SmartPtr<Expr>(new BinaryOperation<OpMult>(sqrte_,de));
      } // end of differentiateFunction

      template<>
      tfel::utilities::SmartPtr<Expr>
      differentiateFunction<std::log>(const tfel::utilities::SmartPtr<Expr> expr,
				      const std::vector<double>::size_type pos,
				      const std::vector<double>& v)
      {
	using namespace tfel::utilities;
	SmartPtr<Expr> ce(expr->clone(v));
	SmartPtr<Expr> de(expr->differentiate(pos,v));
	return SmartPtr<Expr>(new BinaryOperation<OpDiv>(de,ce));
      } // end of differentiateFunction

      template<>
      tfel::utilities::SmartPtr<Expr>
      differentiateFunction<std::log10>(const tfel::utilities::SmartPtr<Expr> expr,
					const std::vector<double>::size_type pos,
					const std::vector<double>& v)
      {
	using namespace std;
	using namespace tfel::utilities;
	static double ln10 = log(10.);
	SmartPtr<Expr> ce(expr->clone(v));
	SmartPtr<Expr> de(expr->differentiate(pos,v));
	SmartPtr<Expr> de_(new BinaryOperation<OpMult>(SmartPtr<Expr>(new Number(ln10)),de));
	return SmartPtr<Expr>(new BinaryOperation<OpDiv>(de_,ce));
      } // end of differentiateFunction

      template<>
      tfel::utilities::SmartPtr<Expr>
      differentiateFunction<std::asin>(const tfel::utilities::SmartPtr<Expr> expr,
				       const std::vector<double>::size_type pos,
				       const std::vector<double>& v)
      {
	using namespace tfel::utilities;
	SmartPtr<Expr> ce(expr->clone(v));
	SmartPtr<Expr> de(expr->differentiate(pos,v));
	SmartPtr<Expr> den1(new BinaryOperation<OpMult>(ce,ce));
	SmartPtr<Expr> den2(new BinaryOperation<OpMinus>(SmartPtr<Expr>(new Number(1.)),den1));
	SmartPtr<Expr> den3(new StandardFunction<std::sqrt>(den2));
	return SmartPtr<Expr>(new BinaryOperation<OpDiv>(de,den3));
      } // end of differentiateFunction

      template<>
      tfel::utilities::SmartPtr<Expr>
      differentiateFunction<std::acos>(const tfel::utilities::SmartPtr<Expr> expr,
				       const std::vector<double>::size_type pos,
				       const std::vector<double>& v)
      {
	using namespace tfel::utilities;
	SmartPtr<Expr> ce(expr->clone(v));
	SmartPtr<Expr> de(expr->differentiate(pos,v));
	SmartPtr<Expr> num1(new Number(-1.));
	SmartPtr<Expr> num2(new BinaryOperation<OpMult>(num1,de));
	SmartPtr<Expr> den1(new BinaryOperation<OpMult>(ce,ce));
	SmartPtr<Expr> den2(new BinaryOperation<OpMinus>(SmartPtr<Expr>(new Number(1.)),den1));
	SmartPtr<Expr> den3(new StandardFunction<std::sqrt>(den2));
	return SmartPtr<Expr>(new BinaryOperation<OpDiv>(num2,den3));
      } // end of differentiateFunction

      template<>
      tfel::utilities::SmartPtr<Expr>
      differentiateFunction<std::atan>(const tfel::utilities::SmartPtr<Expr> expr,
				       const std::vector<double>::size_type pos,
				       const std::vector<double>& v)
      {
	using namespace tfel::utilities;
	SmartPtr<Expr> ce(expr->clone(v));
	SmartPtr<Expr> de(expr->differentiate(pos,v));
	SmartPtr<Expr> den1(new BinaryOperation<OpMult>(ce,ce));
	SmartPtr<Expr> den2(new BinaryOperation<OpPlus>(SmartPtr<Expr>(new Number(1.)),den1));
	return SmartPtr<Expr>(new BinaryOperation<OpDiv>(de,den2));
      } // end of differentiateFunction

      template<>
      tfel::utilities::SmartPtr<Expr>
      differentiateFunction<std::sinh>(const tfel::utilities::SmartPtr<Expr> expr,
				       const std::vector<double>::size_type pos,
				       const std::vector<double>& v)
      {
	using namespace tfel::utilities;
	SmartPtr<Expr> ce(expr->clone(v));
	SmartPtr<Expr> de(expr->differentiate(pos,v));
	SmartPtr<Expr> coshe(new StandardFunction<std::cosh>(ce));
	return SmartPtr<Expr>(new BinaryOperation<OpMult>(coshe,de));
      } // end of differentiateFunction

      template<>
      tfel::utilities::SmartPtr<Expr>
      differentiateFunction<std::cosh>(const tfel::utilities::SmartPtr<Expr> expr,
				       const std::vector<double>::size_type pos,
				       const std::vector<double>& v)
      {
	using namespace tfel::utilities;
	SmartPtr<Expr> ce(expr->clone(v));
	SmartPtr<Expr> de(expr->differentiate(pos,v));
	SmartPtr<Expr> sinhe(new StandardFunction<std::sinh>(ce));
	return SmartPtr<Expr>(new BinaryOperation<OpMult>(sinhe,de));
      } // end of differentiateFunction

      template<>
      tfel::utilities::SmartPtr<Expr>
      differentiateFunction<std::tanh>(const tfel::utilities::SmartPtr<Expr> expr,
				       const std::vector<double>::size_type pos,
				       const std::vector<double>& v)
      {
	using namespace tfel::utilities;
	SmartPtr<Expr> ce(expr->clone(v));
	SmartPtr<Expr> de(expr->differentiate(pos,v));
	SmartPtr<Expr> coshe(new StandardFunction<std::cosh>(ce));
	SmartPtr<Expr> coshe2(new BinaryOperation<OpMult>(coshe,coshe));
	return SmartPtr<Expr>(new BinaryOperation<OpDiv>(de,coshe2));
      } // end of differentiateFunction

    } // end of namespace parser

  } // end of namespace math

} // end of namespace tfel
