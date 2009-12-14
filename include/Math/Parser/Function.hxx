/*!
 * \file   Function.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   02 oct 2007
 */

#ifndef _LIB_TFEL_FUNCTION_HXX_
#define _LIB_TFEL_FUNCTION_HXX_ 

#include"Utilities/SmartPtr.hxx"
#include"Math/Parser/Expr.hxx"

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

      template<double (*f)(const double)>
      struct StandardFunction
	: public Function
      {
	StandardFunction(const tfel::utilities::SmartPtr<Expr>);
	double getValue(void) const;
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
	~StandardFunction();
      private:
	const tfel::utilities::SmartPtr<Expr> expr;
      }; // end of struct StandardFunction

    } // end of namespace parser

  } // end of namespace math

} // end of namespace tfel

#include"Math/Parser/Function.ixx"

#endif /* _LIB_TFEL_FUNCTION_HXX */

