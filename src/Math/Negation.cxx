/*!
 * \file   Negation.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   04 oct 2007
 */

#include"Math/Parser/Negation.hxx"

namespace tfel
{
  namespace math
  {

    namespace parser
    {

      Negation::Negation(const tfel::utilities::SmartPtr<Expr> e)
	: expr(e)
      {} // end of Negation::Negation
      
      double
      Negation::getValue(void) const
      {
	return -(this->expr->getValue());
      } // end of Negation::getValue(void)

      void
      Negation::checkCyclicDependency(const std::vector<std::string>& names) const
	throw(std::runtime_error)
      {
	this->expr->checkCyclicDependency(names);
      } // end of Negation::checkCyclicDependency

      tfel::utilities::SmartPtr<Expr>
      Negation::differentiate(const std::vector<double>::size_type pos,
			      const std::vector<double>& variable) const
      {
	using namespace tfel::utilities;
	SmartPtr<Expr> e = this->expr->differentiate(pos,variable);
	return SmartPtr<Expr>(new Negation(e));
      } // end of Negation::differentiate
      
      tfel::utilities::SmartPtr<Expr>
      Negation::clone(const std::vector<double>& v) const
      {
	using namespace tfel::utilities;
	return SmartPtr<Expr>(new Negation(this->expr->clone(v)));
      }

      tfel::utilities::SmartPtr<Expr>
      Negation::resolveDependencies(void) const
      {
	using namespace tfel::utilities;
	return SmartPtr<Expr>(new Negation(this->expr->resolveDependencies()));
      } // end of Negation::resolveDependencies

      Negation::~Negation()
      {} // end of Negation::~Negation()

    } // end of namespace parser

  } // end of namespace math

} // end of namespace tfel

