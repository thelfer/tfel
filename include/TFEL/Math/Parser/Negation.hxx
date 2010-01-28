/*!
 * \file   Negation.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   02 oct 2007
 */

#ifndef _LIB_TFEL_NEGATION_HXX_
#define _LIB_TFEL_NEGATION_HXX_ 

#include"TFEL/Utilities/SmartPtr.hxx"
#include"TFEL/Math/Parser/Expr.hxx"

namespace tfel
{
  namespace math
  {

    namespace parser
    {

      struct Negation
	: public Expr
      {
	Negation(const tfel::utilities::SmartPtr<Expr>);
	double getValue(void) const;
	void
	checkCyclicDependency(const std::vector<std::string>&) const
	  throw(std::runtime_error);
	tfel::utilities::SmartPtr<Expr>
	differentiate(const std::vector<double>::size_type,
		      const std::vector<double>&) const;
	tfel::utilities::SmartPtr<Expr>
	clone(const std::vector<double>&) const;
	tfel::utilities::SmartPtr<Expr>
	resolveDependencies(void) const;
	~Negation();
      private:
	const tfel::utilities::SmartPtr<Expr> expr;
      };

    } // end of namespace parser

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_NEGATION_HXX */

