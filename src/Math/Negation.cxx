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

      Negation::Negation(const tfel::utilities::shared_ptr<Expr> e)
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

      tfel::utilities::shared_ptr<Expr>
      Negation::differentiate(const std::vector<double>::size_type pos,
			      const std::vector<double>& variable) const
      {
	using namespace tfel::utilities;
	shared_ptr<Expr> e = this->expr->differentiate(pos,variable);
	return shared_ptr<Expr>(new Negation(e));
      } // end of Negation::differentiate
      
      tfel::utilities::shared_ptr<Expr>
      Negation::clone(const std::vector<double>& v) const
      {
	using namespace tfel::utilities;
	return shared_ptr<Expr>(new Negation(this->expr->clone(v)));
      }

      tfel::utilities::shared_ptr<Expr>
      Negation::createFunctionByChangingParametersIntoVariables(const std::vector<double>& v,
								const std::vector<std::string>& params,
								const std::map<std::string,
								std::vector<double>::size_type>& pos) const
      {
	using namespace tfel::utilities;
	shared_ptr<Expr> nexpr = this->expr->createFunctionByChangingParametersIntoVariables(v,params,pos);
	return shared_ptr<Expr>(new Negation(nexpr));
      } // end of Negation::createFunctionByChangingParametersIntoVariables

      tfel::utilities::shared_ptr<Expr>
      Negation::resolveDependencies(const std::vector<double>& v) const
      {
	using namespace tfel::utilities;
	return shared_ptr<Expr>(new Negation(this->expr->resolveDependencies(v)));
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

