/*!
 * \file   include/TFEL/Math/Parser/BinaryOperator.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   02 oct 2007
 */

#ifndef _LIB_TFEL_BINARYOPERATOR_HXX_
#define _LIB_TFEL_BINARYOPERATOR_HXX_ 

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Utilities/SmartPtr.hxx"
#include"TFEL/Math/Parser/Expr.hxx"

namespace tfel
{
  namespace math
  {

    namespace parser
    {

      struct OpPlus
      {
	static double TFEL_VISIBILITY_LOCAL
	apply(const double,const double);
      }; // end of struct OpPlus

      struct OpMinus
      {
	static double TFEL_VISIBILITY_LOCAL
	apply(const double,const double);
      }; // end of struct OpMinus

      struct OpMult
      {
	static double TFEL_VISIBILITY_LOCAL
	apply(const double,const double);
      }; // end of struct OpMult

      struct OpDiv
      {
	static double TFEL_VISIBILITY_LOCAL
	apply(const double,const double);
      }; // end of struct OpDiv

      struct OpPower
      {
	static double TFEL_VISIBILITY_LOCAL
	apply(const double,const double);
      }; // end of struct OpPower

      struct BinaryOperationBase
      {
	static void TFEL_VISIBILITY_LOCAL
	throwUnimplementedDifferentiateFunctionException();
      }; // end of struct BinaryOperationBase

      template<typename Op>
      struct TFEL_VISIBILITY_LOCAL BinaryOperation
	: public Expr,
	  protected BinaryOperationBase
      {
	BinaryOperation(const tfel::utilities::shared_ptr<Expr>,
			const tfel::utilities::shared_ptr<Expr>);
	double
	getValue(void) const;
	void
	checkCyclicDependency(std::vector<std::string>&) const
	  throw(std::runtime_error);
	tfel::utilities::shared_ptr<Expr>
	resolveDependencies(const std::vector<double>&) const;
 	tfel::utilities::shared_ptr<Expr>
	differentiate(const std::vector<double>::size_type,
		      const std::vector<double>&) const;
	tfel::utilities::shared_ptr<Expr>
	clone(const std::vector<double>&) const;
	void
	getParametersNames(std::set<std::string>&) const;
	tfel::utilities::shared_ptr<Expr>
	createFunctionByChangingParametersIntoVariables(const std::vector<double>&,
							const std::vector<std::string>&,
							const std::map<std::string,
							std::vector<double>::size_type>&) const;
	~BinaryOperation();
      private:
	const tfel::utilities::shared_ptr<Expr> a;
	const tfel::utilities::shared_ptr<Expr> b;
      }; // end of struct BinaryOperation

    } // end of namespace parser

  } // end of namespace math

} // end of namespace tfel

#include"TFEL/Math/Parser/BinaryOperator.ixx"

#endif /* _LIB_TFEL_BINARYOPERATOR_HXX */

