/*!
 * \file   BinaryFunction.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   02 oct 2007
 */

#ifndef _LIB_TFEL_BINARYFUNCTION_HXX_
#define _LIB_TFEL_BINARYFUNCTION_HXX_ 

#include"TFEL/Utilities/SmartPtr.hxx"
#include"TFEL/Math/Parser/Expr.hxx"

namespace tfel
{
  namespace math
  {

    namespace parser
    {

      struct BinaryFunction
	: public Expr
      {
	virtual ~BinaryFunction();
      };

      struct StandardBinaryFunctionBase
      {
	static void
	throwUnimplementedDifferentiateFunctionException(void);
	static void
	throwInvalidCallException(const int);
      }; // end of struct StandardBinaryFunctionBase

      template<double (*f)(const double,const double)>
      struct StandardBinaryFunction
	: public BinaryFunction,
	  protected StandardBinaryFunctionBase
      {
	StandardBinaryFunction(const tfel::utilities::SmartPtr<Expr>,
			       const tfel::utilities::SmartPtr<Expr>);
	void
	checkCyclicDependency(std::vector<std::string>&) const;
	double getValue(void) const;
	tfel::utilities::SmartPtr<Expr>
	resolveDependencies(void) const;
	tfel::utilities::SmartPtr<Expr>
	differentiate(const std::vector<double>::size_type,
		      const std::vector<double>&) const;
	tfel::utilities::SmartPtr<Expr>
	clone(const std::vector<double>&) const;
	void
	getParametersNames(std::set<std::string>&) const;
	~StandardBinaryFunction();
      private:
	const tfel::utilities::SmartPtr<Expr> expr1;
	const tfel::utilities::SmartPtr<Expr> expr2;
      }; // end of struct StandardBinaryFunction

    } // end of namespace parser

  } // end of namespace math

} // end of namespace tfel

#include"TFEL/Math/Parser/BinaryFunction.ixx"

#endif /* _LIB_TFEL_BINARYFUNCTION_HXX */

