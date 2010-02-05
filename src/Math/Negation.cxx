/*!
 * \file   Negation.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   04 oct 2007
 */

#include"TFEL/Math/Parser/Negation.hxx"

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
      Negation::checkCyclicDependency(std::vector<std::string>& names) const
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
      Negation::createFunctionByChangingParametersIntoVariables(const std::vector<double>& v,
								const std::vector<std::string>& params,
								const std::map<std::string,
								std::vector<double>::size_type>& pos) const
      {
	using namespace tfel::utilities;
#warning "stupid"
// 	SmartPtr<Expr> nexpr = this->expr->createFunctionByChangingParametersIntoVariables(v,params,pos);
// 	return SmartPtr<Expr>(new Negation(nexpr));
 	return SmartPtr<Expr>(0);
      } // end of Negation::createFunctionByChangingParametersIntoVariables

      tfel::utilities::SmartPtr<Expr>
      Negation::resolveDependencies(void) const
      {
	using namespace tfel::utilities;
	return SmartPtr<Expr>(new Negation(this->expr->resolveDependencies()));
      } // end of Negation::resolveDependencies

      void
      Negation::getParametersNames(std::set<std::string>& p) const
      {
	this->expr->getParametersNames(p);
      } // end of Negation::getParametersNames

      Negation::~Negation()
      {} // end of Negation::~Negation()

    } // end of namespace parser

  } // end of namespace math

} // end of namespace tfel

