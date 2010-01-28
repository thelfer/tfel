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
      Number::checkCyclicDependency(const std::vector<std::string>&) const
	throw(std::runtime_error)
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
      Number::resolveDependencies() const
      {
	using namespace tfel::utilities;
	return SmartPtr<Expr>(new Number(this->value));
      } // end of Number::resolveDependencies

    } // end of namespace parser

  } // end of namespace math

} // end of namespace tfel
