/*!
 * \file   Expr.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   02 oct 2007
 */

#ifndef _LIB_TFEL_EXPR_HXX_
#define _LIB_TFEL_EXPR_HXX_ 

#include<map>
#include<set>
#include<vector>
#include<string>
#include<stdexcept>

#include"TFEL/Utilities/SmartPtr.hxx"

namespace tfel
{
  namespace math
  {

    namespace parser
    {

      struct Expr
      {
	virtual double getValue(void) const = 0;
	virtual void
	checkCyclicDependency(std::vector<std::string>&) const = 0;
	virtual tfel::utilities::shared_ptr<Expr>
	resolveDependencies(const std::vector<double>&) const = 0;
	virtual tfel::utilities::shared_ptr<Expr>
	clone(const std::vector<double>&) const = 0;
	virtual tfel::utilities::shared_ptr<Expr>
	differentiate(const std::vector<double>::size_type,
		      const std::vector<double>&) const = 0;
	virtual void
	getParametersNames(std::set<std::string>&) const = 0;
	virtual tfel::utilities::shared_ptr<Expr>
	createFunctionByChangingParametersIntoVariables(const std::vector<double>&,
							const std::vector<std::string>&,
							const std::map<std::string,
							std::vector<double>::size_type>&) const = 0;
	virtual ~Expr();
      }; // end of struct Expr

      /*!
       * a simple helper function for checkCyclicDependency
       */
      void
      mergeVariablesNames(std::vector<std::string>&,
			  const std::vector<std::string>&);

    } // end of namespace parser

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_EXPR_HXX */
