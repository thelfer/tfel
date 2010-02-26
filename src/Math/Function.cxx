/*!
 * \file   Function.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   02 oct 2007
 */

#include"TFEL/Math/Parser/Negation.hxx"
#include"TFEL/Math/Parser/Number.hxx"
#include"TFEL/Math/Parser/BinaryOperator.hxx"
#include"TFEL/Math/Parser/Function.hxx"

#ifndef __SUNPRO_CC
#define TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DEFINITION(X)     \
      template<>                                                                \
      tfel::utilities::SmartPtr<Expr>                                           \
      differentiateFunction<std::X>(const tfel::utilities::SmartPtr<Expr> expr, \
				    const std::vector<double>::size_type  pos,  \
				    const std::vector<double>& v)
#else  /* __SUNPRO_CC */
#define TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DEFINITION(X)  \
      template<>                                                             \
      tfel::utilities::SmartPtr<Expr>                                        \
      differentiateFunction<X>(const tfel::utilities::SmartPtr<Expr> expr,   \
			       const std::vector<double>::size_type pos,     \
			       const std::vector<double>& v)
#endif /* __SUNPRO_CC */

namespace tfel
{
  namespace math
  {

    namespace parser
    {

      Function::~Function()
      {} // end of Function::~Function

      void
      StandardFunctionBase::throwUnimplementedDifferentiateFunctionException(void)
      {
	using namespace std;
	string msg("StandardFunctionBase::");
	msg += "throwUnimplementedDifferentiateFunctionException : ";
	msg += "unimplemented feature";
	throw(runtime_error(msg));
      } // end of StandardFunctionBase::throwUnimplementedDifferentiateFunctionException()

      void
      StandardFunctionBase::throwInvalidCallException(const double v,
						      const int e)
      {

	using namespace std;
	ostringstream msg;
	msg << "StandardFunctionBase::throwInvalidCallException : "
	    << "call to function failed for value " << v << "("
	    <<  strerror(e)
	    << ")";
	throw(runtime_error(msg.str()));
      } // end of struct StandardFunctionBase::throwInvalidCallException()

      TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DEFINITION(exp)
      {
	using namespace std;
	using namespace tfel::utilities;
	SmartPtr<Expr> ce(expr->clone(v));
	SmartPtr<Expr> de(expr->differentiate(pos,v));
	SmartPtr<Expr> expe(new StandardFunction<exp>(ce));
	return SmartPtr<Expr>(new BinaryOperation<OpMult>(expe,de));
      } // end of differentiateFunction

      TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DEFINITION(sin)
      {
	using namespace std;
	using namespace tfel::utilities;
	SmartPtr<Expr> ce(expr->clone(v));
	SmartPtr<Expr> de(expr->differentiate(pos,v));
	SmartPtr<Expr> cose(new StandardFunction<cos>(ce));
	return SmartPtr<Expr>(new BinaryOperation<OpMult>(cose,de));
      } // end of differentiateFunction

      TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DEFINITION(cos)
      {
	using namespace std;
	using namespace tfel::utilities;
	SmartPtr<Expr> ce(expr->clone(v));
	SmartPtr<Expr> de(expr->differentiate(pos,v));
	SmartPtr<Expr> sine(new StandardFunction<sin>(ce));
	return SmartPtr<Expr>(new BinaryOperation<OpMult>(SmartPtr<Expr>(new Negation(sine)),de));
      } // end of differentiateFunction

      TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DEFINITION(tan)
      {
	using namespace std;
	using namespace tfel::utilities;
	SmartPtr<Expr> ce(expr->clone(v));
	SmartPtr<Expr> de(expr->differentiate(pos,v));
	SmartPtr<Expr> tane(new StandardFunction<tan>(ce));
	SmartPtr<Expr> tan2e(new BinaryOperation<OpMult>(tane,tane));
	SmartPtr<Expr> tan2e_(new BinaryOperation<OpPlus>(SmartPtr<Expr>(new Number(1.)),tan2e));
	return SmartPtr<Expr>(new BinaryOperation<OpMult>(tan2e_,de));
      } // end of differentiateFunction

      TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DEFINITION(sqrt)
      {
	using namespace std;
	using namespace tfel::utilities;
	SmartPtr<Expr> ce(expr->clone(v));
	SmartPtr<Expr> de(expr->differentiate(pos,v));
	SmartPtr<Expr> sqrte(new StandardFunction<sqrt>(ce));
	SmartPtr<Expr> sqrte_(new BinaryOperation<OpDiv>(SmartPtr<Expr>(new Number(0.5)),
							 sqrte));
	return SmartPtr<Expr>(new BinaryOperation<OpMult>(sqrte_,de));
      } // end of differentiateFunction

      TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DEFINITION(log)
      {
	using namespace std;
	using namespace tfel::utilities;
	SmartPtr<Expr> ce(expr->clone(v));
	SmartPtr<Expr> de(expr->differentiate(pos,v));
	return SmartPtr<Expr>(new BinaryOperation<OpDiv>(de,ce));
      } // end of differentiateFunction

      TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DEFINITION(log10)
      {
	using namespace std;
	using namespace std;
	using namespace tfel::utilities;
	static double ln10 = log(10.);
	SmartPtr<Expr> ce(expr->clone(v));
	SmartPtr<Expr> de(expr->differentiate(pos,v));
	SmartPtr<Expr> de_(new BinaryOperation<OpMult>(SmartPtr<Expr>(new Number(ln10)),de));
	return SmartPtr<Expr>(new BinaryOperation<OpDiv>(de_,ce));
      } // end of differentiateFunction

      TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DEFINITION(asin)
      {
	using namespace std;
	using namespace tfel::utilities;
	SmartPtr<Expr> ce(expr->clone(v));
	SmartPtr<Expr> de(expr->differentiate(pos,v));
	SmartPtr<Expr> den1(new BinaryOperation<OpMult>(ce,ce));
	SmartPtr<Expr> den2(new BinaryOperation<OpMinus>(SmartPtr<Expr>(new Number(1.)),den1));
	SmartPtr<Expr> den3(new StandardFunction<sqrt>(den2));
	return SmartPtr<Expr>(new BinaryOperation<OpDiv>(de,den3));
      } // end of differentiateFunction

      TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DEFINITION(acos)
      {
	using namespace std;
	using namespace tfel::utilities;
	SmartPtr<Expr> ce(expr->clone(v));
	SmartPtr<Expr> de(expr->differentiate(pos,v));
	SmartPtr<Expr> num1(new Number(-1.));
	SmartPtr<Expr> num2(new BinaryOperation<OpMult>(num1,de));
	SmartPtr<Expr> den1(new BinaryOperation<OpMult>(ce,ce));
	SmartPtr<Expr> den2(new BinaryOperation<OpMinus>(SmartPtr<Expr>(new Number(1.)),den1));
	SmartPtr<Expr> den3(new StandardFunction<sqrt>(den2));
	return SmartPtr<Expr>(new BinaryOperation<OpDiv>(num2,den3));
      } // end of differentiateFunction

      TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DEFINITION(atan)
      {
	using namespace std;
	using namespace tfel::utilities;
	SmartPtr<Expr> ce(expr->clone(v));
	SmartPtr<Expr> de(expr->differentiate(pos,v));
	SmartPtr<Expr> den1(new BinaryOperation<OpMult>(ce,ce));
	SmartPtr<Expr> den2(new BinaryOperation<OpPlus>(SmartPtr<Expr>(new Number(1.)),den1));
	return SmartPtr<Expr>(new BinaryOperation<OpDiv>(de,den2));
      } // end of differentiateFunction

      TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DEFINITION(sinh)
      {
	using namespace std;
	using namespace tfel::utilities;
	SmartPtr<Expr> ce(expr->clone(v));
	SmartPtr<Expr> de(expr->differentiate(pos,v));
	SmartPtr<Expr> coshe(new StandardFunction<cosh>(ce));
	return SmartPtr<Expr>(new BinaryOperation<OpMult>(coshe,de));
      } // end of differentiateFunction

      TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DEFINITION(cosh)
      {
	using namespace std;
	using namespace tfel::utilities;
	SmartPtr<Expr> ce(expr->clone(v));
	SmartPtr<Expr> de(expr->differentiate(pos,v));
	SmartPtr<Expr> sinhe(new StandardFunction<sinh>(ce));
	return SmartPtr<Expr>(new BinaryOperation<OpMult>(sinhe,de));
      } // end of differentiateFunction

      TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DEFINITION(tanh)
      {
	using namespace std;
	using namespace tfel::utilities;
	SmartPtr<Expr> ce(expr->clone(v));
	SmartPtr<Expr> de(expr->differentiate(pos,v));
	SmartPtr<Expr> coshe(new StandardFunction<cosh>(ce));
	SmartPtr<Expr> coshe2(new BinaryOperation<OpMult>(coshe,coshe));
	return SmartPtr<Expr>(new BinaryOperation<OpDiv>(de,coshe2));
      } // end of differentiateFunction

    } // end of namespace parser

  } // end of namespace math

} // end of namespace tfel
