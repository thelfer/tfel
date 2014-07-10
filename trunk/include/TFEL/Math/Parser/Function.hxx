/*!
 * \file   Function.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   02 oct 2007
 */

#ifndef _LIB_TFEL_FUNCTION_HXX_
#define _LIB_TFEL_FUNCTION_HXX_ 

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Utilities/SmartPtr.hxx"
#include"TFEL/System/ExternalFunctionsPrototypes.hxx"
#include"TFEL/Math/Parser/Expr.hxx"

namespace tfel
{
  namespace math
  {

    namespace parser
    {

      struct Function
	: public Expr
      {
	virtual ~Function();
      };
      
      typedef double(* StandardFunctionPtr)(double); 

      struct StandardFunctionBase
      {
	static void
	throwUnimplementedDifferentiateFunctionException(void);
	static void
	throwInvalidCallException(const double,
				  const int);
      }; // end of struct StandardFunctionBase

      template<StandardFunctionPtr f>
      struct TFEL_VISIBILITY_LOCAL StandardFunction
	: public Function
      {
	StandardFunction(const tfel::utilities::shared_ptr<Expr>);
	double getValue(void) const;
	void
	checkCyclicDependency(std::vector<std::string>&) const;
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
	~StandardFunction();
      private:
	const tfel::utilities::shared_ptr<Expr> expr;
      }; // end of struct StandardFunction

    } // end of namespace parser

  } // end of namespace math

} // end of namespace tfel

#include"TFEL/Math/Parser/Function.ixx"

#endif /* _LIB_TFEL_FUNCTION_HXX */

