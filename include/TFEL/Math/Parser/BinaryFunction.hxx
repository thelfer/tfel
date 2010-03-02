/*!
 * \file   BinaryFunction.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   02 oct 2007
 */

#ifndef _LIB_TFEL_BINARYFUNCTION_HXX_
#define _LIB_TFEL_BINARYFUNCTION_HXX_ 

#include"TFEL/Config/TFELConfig.hxx"
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
      struct TFEL_VISIBILITY_LOCAL StandardBinaryFunction
	: public BinaryFunction,
	  protected StandardBinaryFunctionBase
      {
	StandardBinaryFunction(const tfel::utilities::shared_ptr<Expr>,
			       const tfel::utilities::shared_ptr<Expr>);
	void
	checkCyclicDependency(std::vector<std::string>&) const;
	double getValue(void) const;
	tfel::utilities::shared_ptr<Expr>
	resolveDependencies(const std::vector<double>&) const;
	tfel::utilities::shared_ptr<Expr>
	differentiate(const std::vector<double>::size_type,
		      const std::vector<double>&) const;
	tfel::utilities::shared_ptr<Expr>
	clone(const std::vector<double>&) const;
	tfel::utilities::shared_ptr<Expr>
	createFunctionByChangingParametersIntoVariables(const std::vector<double>&,
							const std::vector<std::string>&,
							const std::map<std::string,
							std::vector<double>::size_type>&) const;	
	void
	getParametersNames(std::set<std::string>&) const;
	~StandardBinaryFunction();
      private:
	const tfel::utilities::shared_ptr<Expr> expr1;
	const tfel::utilities::shared_ptr<Expr> expr2;
      }; // end of struct StandardBinaryFunction

    } // end of namespace parser

  } // end of namespace math

} // end of namespace tfel

#include"TFEL/Math/Parser/BinaryFunction.ixx"

#endif /* _LIB_TFEL_BINARYFUNCTION_HXX */

