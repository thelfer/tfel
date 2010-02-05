/*!
 * \file   Number.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   02 oct 2007
 */

#include<iostream>

#include"TFEL/Math/Parser/Number.hxx"

namespace tfel
{
  namespace math
  {

    namespace parser
    {

      Number::Number(const double v)
	: value(v)
      {} // end of Number::Number
      
      double
      Number::getValue() const
      {
	return this->value;
      } // end of Number::getValue

      void
      Number::getParametersNames(std::set<std::string>&) const
      {} // end of Number::getParametersNames

      void
      Number::checkCyclicDependency(std::vector<std::string>&) const
      {} // end of Number::checkCyclicDependency

      tfel::utilities::SmartPtr<Expr>
      Number::differentiate(const std::vector<double>::size_type,
			    const std::vector<double>&) const
      {
	using namespace tfel::utilities;
	return SmartPtr<Expr>(new Number(0.));
      } // end of Number::differentiate

      tfel::utilities::SmartPtr<Expr>
      Number::clone(const std::vector<double>&) const
      {
	using namespace tfel::utilities;
	return SmartPtr<Expr>(new Number(this->value));
      } // end of Number::clone

      tfel::utilities::SmartPtr<Expr>
      Number::createFunctionByChangingParametersIntoVariables(const std::vector<double>& v,
							      const std::vector<std::string>&,
							      const std::map<std::string,
							      std::vector<double>::size_type>&) const
      {
	return this->clone(v);
      } // end of Number::createFunctionByChangingParametersIntoVariables

      tfel::utilities::SmartPtr<Expr>
      Number::resolveDependencies() const
      {
	using namespace tfel::utilities;
	return SmartPtr<Expr>(new Number(this->value));
      } // end of Number::resolveDependencies

    } // end of namespace parser

  } // end of namespace math

} // end of namespace tfel
