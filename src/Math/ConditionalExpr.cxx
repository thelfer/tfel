/*!
 * \file   ConditionalExpr.cxx
 * \brief  
 * \author Helfer Thomas
 * \date   09 jan 2009
 */

#include<cmath>
#include<limits>

#include"TFEL/Math/Parser/ConditionalExpr.hxx"

namespace tfel
{

  namespace math
  {

    namespace parser
    {

      ConditionalExpr::ConditionalExpr(const tfel::utilities::SmartPtr<LogicalExpr> c_,
				       const tfel::utilities::SmartPtr<Expr> a_,
				       const tfel::utilities::SmartPtr<Expr> b_)
	: c(c_),a(a_),b(b_)
      {} // end of ConditionalExpr::ConditionalExpr
      
      double
      ConditionalExpr::getValue(void) const
      {
	if(this->c->getValue()){
	  return this->a->getValue();
	}
	return this->b->getValue();
      } // end of ConditionalExpr::getValue(void) const

      void
      ConditionalExpr::checkCyclicDependency(std::vector<std::string>& vars) const
      {
	using namespace std;
	vector<string> a_vars;
	vector<string> b_vars;
	vector<string> c_vars;
	this->c->checkCyclicDependency(c_vars);
	this->a->checkCyclicDependency(a_vars);
	this->b->checkCyclicDependency(b_vars);
	mergeVariablesNames(vars,a_vars);
	mergeVariablesNames(vars,b_vars);
	mergeVariablesNames(vars,c_vars);
      } // end of ConditionalExpr::checkCyclicDependency(const std::vector<std::string>& vars) const

      tfel::utilities::SmartPtr<Expr>
      ConditionalExpr::resolveDependencies(const std::vector<double>&v) const
      {
	using namespace tfel::utilities;
	return SmartPtr<Expr>(new ConditionalExpr(this->c->resolveDependencies(v),
						  this->a->resolveDependencies(v),
						  this->b->resolveDependencies(v)));
      }// end of ConditionalExpr::resolveDependencies

      void
      ConditionalExpr::getParametersNames(std::set<std::string>& p) const
      {
	this->c->getParametersNames(p);
	this->a->getParametersNames(p);
	this->b->getParametersNames(p);
      } // end of ConditionalExpr::getParametersNames(std::set<std::string>&)

      tfel::utilities::SmartPtr<Expr>
      ConditionalExpr::differentiate(const std::vector<double>::size_type pos,
				     const std::vector<double>& v) const
      {
	using namespace tfel::utilities;
	return SmartPtr<Expr>(new ConditionalExpr(this->c->clone(v),
						  this->a->differentiate(pos,v),
						  this->b->differentiate(pos,v)));
      } // end of ConditionalExpr::differentiate

      tfel::utilities::SmartPtr<Expr>
      ConditionalExpr::clone(const std::vector<double>& v) const
      {
	using namespace tfel::utilities;
	return SmartPtr<Expr>(new ConditionalExpr(this->c->clone(v),
						  this->a->clone(v),
						  this->b->clone(v)));
      } // end of ConditionalExpr::clone

      
      tfel::utilities::SmartPtr<Expr>
      ConditionalExpr::createFunctionByChangingParametersIntoVariables(const std::vector<double>& v,
								       const std::vector<std::string>& p,
								       const std::map<std::string,
								       std::vector<double>::size_type>& pos) const
      {
	using namespace tfel::utilities;
	SmartPtr<LogicalExpr> nc = this->c->createFunctionByChangingParametersIntoVariables(v,p,pos);
	SmartPtr<Expr> na = this->a->createFunctionByChangingParametersIntoVariables(v,p,pos);
	SmartPtr<Expr> nb = this->b->createFunctionByChangingParametersIntoVariables(v,p,pos);
	return SmartPtr<Expr>(new ConditionalExpr(nc,na,nb));
      } // end of ConditionalExpr::createFunctionByChangingParametersIntoVariables

      ConditionalExpr::~ConditionalExpr()
      {} // end of ConditionalExpr::~ConditionalExpr()

    } // end of namespace parser

  } // end of namespace math

} // end of namespace tfel
