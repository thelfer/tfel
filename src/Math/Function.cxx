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
      tfel::utilities::shared_ptr<Expr>                                           \
      differentiateFunction<std::X>(const tfel::utilities::shared_ptr<Expr> expr, \
				    const std::vector<double>::size_type  pos,  \
				    const std::vector<double>& v)
#else  /* __SUNPRO_CC */
#define TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DEFINITION(X)  \
      template<>                                                             \
      tfel::utilities::shared_ptr<Expr>                                        \
      differentiateFunction<X>(const tfel::utilities::shared_ptr<Expr> expr,   \
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
	shared_ptr<Expr> ce(expr->clone(v));
	shared_ptr<Expr> de(expr->differentiate(pos,v));
	shared_ptr<Expr> expe(new StandardFunction<exp>(ce));
	return shared_ptr<Expr>(new BinaryOperation<OpMult>(expe,de));
      } // end of differentiateFunction

      TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DEFINITION(sin)
      {
	using namespace std;
	using namespace tfel::utilities;
	shared_ptr<Expr> ce(expr->clone(v));
	shared_ptr<Expr> de(expr->differentiate(pos,v));
	shared_ptr<Expr> cose(new StandardFunction<cos>(ce));
	return shared_ptr<Expr>(new BinaryOperation<OpMult>(cose,de));
      } // end of differentiateFunction

      TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DEFINITION(cos)
      {
	using namespace std;
	using namespace tfel::utilities;
	shared_ptr<Expr> ce(expr->clone(v));
	shared_ptr<Expr> de(expr->differentiate(pos,v));
	shared_ptr<Expr> sine(new StandardFunction<sin>(ce));
	return shared_ptr<Expr>(new BinaryOperation<OpMult>(shared_ptr<Expr>(new Negation(sine)),de));
      } // end of differentiateFunction

      TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DEFINITION(tan)
      {
	using namespace std;
	using namespace tfel::utilities;
	shared_ptr<Expr> ce(expr->clone(v));
	shared_ptr<Expr> de(expr->differentiate(pos,v));
	shared_ptr<Expr> tane(new StandardFunction<tan>(ce));
	shared_ptr<Expr> tan2e(new BinaryOperation<OpMult>(tane,tane));
	shared_ptr<Expr> tan2e_(new BinaryOperation<OpPlus>(shared_ptr<Expr>(new Number(1.)),tan2e));
	return shared_ptr<Expr>(new BinaryOperation<OpMult>(tan2e_,de));
      } // end of differentiateFunction

      TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DEFINITION(sqrt)
      {
	using namespace std;
	using namespace tfel::utilities;
	shared_ptr<Expr> ce(expr->clone(v));
	shared_ptr<Expr> de(expr->differentiate(pos,v));
	shared_ptr<Expr> sqrte(new StandardFunction<sqrt>(ce));
	shared_ptr<Expr> sqrte_(new BinaryOperation<OpDiv>(shared_ptr<Expr>(new Number(0.5)),
							 sqrte));
	return shared_ptr<Expr>(new BinaryOperation<OpMult>(sqrte_,de));
      } // end of differentiateFunction

      TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DEFINITION(log)
      {
	using namespace std;
	using namespace tfel::utilities;
	shared_ptr<Expr> ce(expr->clone(v));
	shared_ptr<Expr> de(expr->differentiate(pos,v));
	return shared_ptr<Expr>(new BinaryOperation<OpDiv>(de,ce));
      } // end of differentiateFunction

      TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DEFINITION(log10)
      {
	using namespace std;
	using namespace std;
	using namespace tfel::utilities;
	static double ln10 = log(10.);
	shared_ptr<Expr> ce(expr->clone(v));
	shared_ptr<Expr> de(expr->differentiate(pos,v));
	shared_ptr<Expr> de_(new BinaryOperation<OpMult>(shared_ptr<Expr>(new Number(ln10)),de));
	return shared_ptr<Expr>(new BinaryOperation<OpDiv>(de_,ce));
      } // end of differentiateFunction

      TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DEFINITION(asin)
      {
	using namespace std;
	using namespace tfel::utilities;
	shared_ptr<Expr> ce(expr->clone(v));
	shared_ptr<Expr> de(expr->differentiate(pos,v));
	shared_ptr<Expr> den1(new BinaryOperation<OpMult>(ce,ce));
	shared_ptr<Expr> den2(new BinaryOperation<OpMinus>(shared_ptr<Expr>(new Number(1.)),den1));
	shared_ptr<Expr> den3(new StandardFunction<sqrt>(den2));
	return shared_ptr<Expr>(new BinaryOperation<OpDiv>(de,den3));
      } // end of differentiateFunction

      TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DEFINITION(acos)
      {
	using namespace std;
	using namespace tfel::utilities;
	shared_ptr<Expr> ce(expr->clone(v));
	shared_ptr<Expr> de(expr->differentiate(pos,v));
	shared_ptr<Expr> num1(new Number(-1.));
	shared_ptr<Expr> num2(new BinaryOperation<OpMult>(num1,de));
	shared_ptr<Expr> den1(new BinaryOperation<OpMult>(ce,ce));
	shared_ptr<Expr> den2(new BinaryOperation<OpMinus>(shared_ptr<Expr>(new Number(1.)),den1));
	shared_ptr<Expr> den3(new StandardFunction<sqrt>(den2));
	return shared_ptr<Expr>(new BinaryOperation<OpDiv>(num2,den3));
      } // end of differentiateFunction

      TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DEFINITION(atan)
      {
	using namespace std;
	using namespace tfel::utilities;
	shared_ptr<Expr> ce(expr->clone(v));
	shared_ptr<Expr> de(expr->differentiate(pos,v));
	shared_ptr<Expr> den1(new BinaryOperation<OpMult>(ce,ce));
	shared_ptr<Expr> den2(new BinaryOperation<OpPlus>(shared_ptr<Expr>(new Number(1.)),den1));
	return shared_ptr<Expr>(new BinaryOperation<OpDiv>(de,den2));
      } // end of differentiateFunction

      TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DEFINITION(sinh)
      {
	using namespace std;
	using namespace tfel::utilities;
	shared_ptr<Expr> ce(expr->clone(v));
	shared_ptr<Expr> de(expr->differentiate(pos,v));
	shared_ptr<Expr> coshe(new StandardFunction<cosh>(ce));
	return shared_ptr<Expr>(new BinaryOperation<OpMult>(coshe,de));
      } // end of differentiateFunction

      TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DEFINITION(cosh)
      {
	using namespace std;
	using namespace tfel::utilities;
	shared_ptr<Expr> ce(expr->clone(v));
	shared_ptr<Expr> de(expr->differentiate(pos,v));
	shared_ptr<Expr> sinhe(new StandardFunction<sinh>(ce));
	return shared_ptr<Expr>(new BinaryOperation<OpMult>(sinhe,de));
      } // end of differentiateFunction

      TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DEFINITION(tanh)
      {
	using namespace std;
	using namespace tfel::utilities;
	shared_ptr<Expr> ce(expr->clone(v));
	shared_ptr<Expr> de(expr->differentiate(pos,v));
	shared_ptr<Expr> coshe(new StandardFunction<cosh>(ce));
	shared_ptr<Expr> coshe2(new BinaryOperation<OpMult>(coshe,coshe));
	return shared_ptr<Expr>(new BinaryOperation<OpDiv>(de,coshe2));
      } // end of differentiateFunction

    } // end of namespace parser

  } // end of namespace math

} // end of namespace tfel
