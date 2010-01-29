/*!
 * \file   Variable.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   02 oct 2007
 */

#include<iostream>

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
      
      tfel::utilities::SmartPtr<Expr>
      Variable::differentiate(const std::vector<double>::size_type dpos,
			      const std::vector<double>&) const
      {
	using namespace tfel::utilities;
	if(dpos!=this->pos){
	  return SmartPtr<Expr>(new Number(0.));
	}
	return SmartPtr<Expr>(new Number(1.));
      } // end of Variable::differentiate

      tfel::utilities::SmartPtr<Expr>
      Variable::clone(const std::vector<double>& v_) const
      {
	using namespace tfel::utilities;
	return SmartPtr<Expr>(new Variable(v_,this->pos));
      } // end of Variable::clone

      tfel::utilities::SmartPtr<Expr>
      Variable::resolveDependencies(void) const
      {
	using namespace tfel::utilities;
	return SmartPtr<Expr>(new Variable(this->v,this->pos));
      } // end of Variable::resolveDependencies

    } // end of namespace parser

  } // end of namespace math

} // end of namespace tfel
