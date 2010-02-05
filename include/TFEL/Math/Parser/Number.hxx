/*!
 * \file   Number.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   02 oct 2007
 */

#ifndef _LIB_TFEL_NUMBER_HXX_
#define _LIB_TFEL_NUMBER_HXX_ 

#include"TFEL/Math/Parser/Expr.hxx"

namespace tfel
{
  namespace math
  {

    namespace parser
    {

      struct Number
	: public Expr
      {
	Number(const double);
	void
	checkCyclicDependency(std::vector<std::string>&) const;
	tfel::utilities::SmartPtr<Expr>
	differentiate(const std::vector<double>::size_type,
		      const std::vector<double>&) const;
	tfel::utilities::SmartPtr<Expr>
	clone(const std::vector<double>&) const;
	tfel::utilities::SmartPtr<Expr>
	resolveDependencies(void) const;
	void
	getParametersNames(std::set<std::string>&) const;
	tfel::utilities::SmartPtr<Expr>
	createFunctionByChangingParametersIntoVariables(const std::vector<double>&,
							const std::vector<std::string>&,
							const std::map<std::string,
							std::vector<double>::size_type>&) const;
	double
	getValue(void) const;
      private:
	const double value;
      }; // end of struct Number

    } // end of namespace parser

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_NUMBER_HXX */

