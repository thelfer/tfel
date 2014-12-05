/*!
 * \file   include/TFEL/Math/Parser/KrigedFunction.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   02 oct 2007
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_MATH_PARSER_KRIGEDFUNCTION_HXX_
#define _LIB_TFEL_MATH_PARSER_KRIGEDFUNCTION_HXX_ 

#include<vector>
#include<string>
#include<vector>
#include<utility>
#include<stdexcept>

#include"TFEL/Config/TFELConfig.hxx"
#include<memory>
#include"TFEL/Math/Kriging.hxx"
#include"TFEL/Math/Parser/ExternalFunction.hxx"

namespace tfel
{

  namespace math
  {

    namespace parser
    {

      struct TFELMATHPARSER_VISIBILITY_EXPORT KrigedFunctionBase
      {
	static void
	throwInvalidCreateFunctionByChangingParametersIntoVariables(void);

	static void
	throwInvalidIndexException(const std::vector<double>::size_type i,
				   const unsigned short N);
	static void
	throwUnimplementedDifferentiateFunctionException();
      }; // end of KrigedFunctionBase

      template<unsigned short N>
      struct TFELMATHPARSER_VISIBILITY_EXPORT KrigedFunction
	: public ExternalFunction
      {
	typedef typename KrigingVariable<N,double>::type Variable;
	typedef std::pair<Variable,double> Point;

	KrigedFunction(const std::vector<Point>&, // data
		       const double = 0.);        // nugget effect

	KrigedFunction(const KrigedFunction&);

	virtual double
	getValue(void) const;

	virtual void
	setVariableValue(const std::vector<double>::size_type,
			 const double);

	virtual std::vector<double>::size_type
	getNumberOfVariables(void) const;

	virtual void
	checkCyclicDependency(const std::string&) const;

	virtual void
	checkCyclicDependency(std::vector<std::string>&) const;

	virtual std::shared_ptr<ExternalFunction>
	differentiate(const std::vector<double>::size_type) const;

	virtual	std::shared_ptr<ExternalFunction>
	differentiate(const std::string&) const;

	virtual	std::shared_ptr<ExternalFunction>
	resolveDependencies(void) const;

	virtual std::shared_ptr<ExternalFunction>
	createFunctionByChangingParametersIntoVariables(const std::vector<std::string>&) const;

	virtual std::shared_ptr<ExternalFunction>
	createFunctionByChangingParametersIntoVariables(std::vector<std::string>&,
							const std::vector<double>&,
							const std::vector<std::string>&,
							const std::map<std::string,
							std::vector<double>::size_type>&) const;

	virtual void
	getParametersNames(std::set<std::string>&) const;

	virtual ~KrigedFunction();

      private:

	TFEL_VISIBILITY_LOCAL KrigedFunction();
	
	TFEL_VISIBILITY_LOCAL KrigedFunction&
	operator = (const KrigedFunction&);

	std::shared_ptr<tfel::math::Kriging<N> > k;

	Variable v;

      }; // end of struct KrigedFunction
      
    } // end of namespace parser

  } // end of namespace math

} // end of namespace tfel

#include"TFEL/Math/Parser/KrigedFunction.ixx"

#endif /* _LIB_TFEL_MATH_PARSER_KRIGEDFUNCTION_HXX */
