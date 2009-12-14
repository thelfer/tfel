/*!
 * \file   BinaryOperator.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   02 oct 2007
 */

#ifndef _LIB_TFEL_BINARYOPERATOR_HXX_
#define _LIB_TFEL_BINARYOPERATOR_HXX_ 

#include"Utilities/SmartPtr.hxx"
#include"Math/Parser/Expr.hxx"

namespace tfel
{
  namespace math
  {

    namespace parser
    {

      struct OpPlus
      {
	static
	double
	apply(const double,const double);
      }; // end of struct OpPlus

      struct OpMinus
      {
	static
	double
	apply(const double,const double);
      }; // end of struct OpMinus

      struct OpMult
      {
	static
	double
	apply(const double,const double);
      }; // end of struct OpMult

      struct OpDiv
      {
	static
	double
	apply(const double,const double);
      }; // end of struct OpDiv

      struct OpPower
      {
	static
	double
	apply(const double,const double);
      }; // end of struct OpPower

      template<typename Op>
      struct BinaryOperation
	: public Expr
      {
	BinaryOperation(const tfel::utilities::SmartPtr<Expr>,
			const tfel::utilities::SmartPtr<Expr>);
	double
	getValue(void) const;
	void
	checkCyclicDependency(const std::vector<std::string>&) const
	  throw(std::runtime_error);
	tfel::utilities::SmartPtr<Expr>
	resolveDependencies(void) const;
 	tfel::utilities::SmartPtr<Expr>
	differentiate(const std::vector<double>::size_type,
		      const std::vector<double>&) const;
	tfel::utilities::SmartPtr<Expr>
	clone(const std::vector<double>&) const;
	~BinaryOperation();
      private:
	const tfel::utilities::SmartPtr<Expr> a;
	const tfel::utilities::SmartPtr<Expr> b;
      }; // end of struct BinaryOperation

    } // end of namespace parser

  } // end of namespace math

} // end of namespace tfel

#include"Math/Parser/BinaryOperator.ixx"

#endif /* _LIB_TFEL_BINARYOPERATOR_HXX */

