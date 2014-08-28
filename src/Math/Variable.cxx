/*!
 * \file   src/Math/Variable.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   02 oct 2007
 */

#include<stdexcept>

#include"TFEL/Math/Parser/Number.hxx"
#include"TFEL/Math/Parser/Variable.hxx"

namespace tfel
{
  namespace math
  {

    namespace parser
    {

      Variable::Variable(const std::vector<double>& v_,
			 const std::vector<double>::size_type p_)
	: v(v_), pos(p_)
      {} // end of Variable::Variable

      double
      Variable::getValue(void) const
      {
	return this->v[this->pos];
      } // end of Variable::getValue

      void
      Variable::checkCyclicDependency(std::vector<std::string>&) const
      {} // end of Variable::checkCyclicDependency
      
      void
      Variable::getParametersNames(std::set<std::string>&) const
      {} // end of Variable::getParametersNames

      tfel::utilities::shared_ptr<Expr>
      Variable::differentiate(const std::vector<double>::size_type dpos,
			      const std::vector<double>&) const
      {
	using namespace tfel::utilities;
	if(dpos!=this->pos){
	  return shared_ptr<Expr>(new Number(0.));
	}
	return shared_ptr<Expr>(new Number(1.));
      } // end of Variable::differentiate

      tfel::utilities::shared_ptr<Expr>
      Variable::clone(const std::vector<double>& v_) const
      {
	using namespace tfel::utilities;
	return shared_ptr<Expr>(new Variable(v_,this->pos));
      } // end of Variable::clone

      tfel::utilities::shared_ptr<Expr>
      Variable::createFunctionByChangingParametersIntoVariables(const std::vector<double>& v_,
								const std::vector<std::string>&,
								const std::map<std::string,
								std::vector<double>::size_type>&) const
      {
	using namespace std;
	using namespace tfel::utilities;
	if(v_.size()<this->pos){
	  string msg("Variable::createFunctionByChangingParametersIntoVariables : ");
	  msg += "invalid vector size";
	  throw(runtime_error(msg));
	}
	return shared_ptr<Expr>(new Variable(v_,this->pos));
      } // end of Variable::createFunctionByChangingParametersIntoVariables

      tfel::utilities::shared_ptr<Expr>
      Variable::resolveDependencies(const std::vector<double>& nv) const
      {
	using namespace tfel::utilities;
	return shared_ptr<Expr>(new Variable(nv,this->pos));
      } // end of Variable::resolveDependencies

    } // end of namespace parser

  } // end of namespace math

} // end of namespace tfel
