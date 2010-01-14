/*!
 * \file   Function.ixx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   02 oct 2007
 */

#ifndef _LIB_TFEL_FUNCTION_IXX_
#define _LIB_TFEL_FUNCTION_IXX_ 

#include<iostream>
#include<string>
#include<stdexcept>
#include<cerrno>
#include<cstring>
#include<cmath>
#include<iostream>

namespace tfel
{
  namespace math
  {

    namespace parser
    {

      template<double (*f)(double)>
      StandardFunction<f>::StandardFunction(const tfel::utilities::SmartPtr<Expr> e)
	: expr(e)
      {} // end of StandardFunction::StandardFunction

      template<double (*f)(double)>
      StandardFunction<f>::~StandardFunction()
      {} // end of StandardFunction::StandardFunction

      template<double (*f)(double)>
      double
      StandardFunction<f>::getValue(void) const
      {
	using namespace std;
	double res;
	const double arg = this->expr->getValue();
	int old = errno;
	errno = 0;
	res = f(arg);
	if(errno!=0){
	  string msg("StandardFunction<f>::getValue : call to function failed (");
	  msg += strerror(errno);
	  msg += ")";
	  throw(runtime_error(msg));
	}
	errno = old;
	return res;
      } // end of StandardFunction::StandardFunction

      template<double (*f)(double)>
      void
      StandardFunction<f>::checkCyclicDependency(const std::vector<std::string>& names) const
	throw(std::runtime_error)
      {
	this->expr->checkCyclicDependency(names);
      } // end of StandardFunction<f>::checkCyclicDependency

      template<double (*f)(double)>
      tfel::utilities::SmartPtr<Expr>
      StandardFunction<f>::resolveDependencies(void) const
      {
	using namespace tfel::utilities;
	return SmartPtr<Expr>(new StandardFunction<f>(this->expr->resolveDependencies()));
      } // end of StandardFunction<f>::resolveDependencies(void)
      
      template<double (*f)(double)>
      tfel::utilities::SmartPtr<Expr>
      StandardFunction<f>::clone(const std::vector<double>& v) const
      {
	using namespace tfel::utilities;
	return SmartPtr<Expr>(new StandardFunction<f>(this->expr->clone(v)));
      } // end of StandardFunction<f>::clone

      template<double (*f)(double)>
      tfel::utilities::SmartPtr<Expr>
      differentiateFunction(const tfel::utilities::SmartPtr<Expr>,
			    const std::vector<double>::size_type,
			    const std::vector<double>&)
      {
	using namespace std;
	using namespace tfel::utilities;
	string msg("StandardBinaryFunction<f>::differentiate : ");
	msg += "unimplemented feature";
	throw(runtime_error(msg));
	return SmartPtr<Expr>(0);
      }
      
      template<double (*f)(double)>
      tfel::utilities::SmartPtr<Expr>
      StandardFunction<f>::differentiate(const std::vector<double>::size_type pos,
					 const std::vector<double>& v) const
      {
	return differentiateFunction<f>(this->expr,pos,v);
      } // end of StandardFunction<f>::differentiate

      template<>
      tfel::utilities::SmartPtr<Expr>
      differentiateFunction<std::exp>(const tfel::utilities::SmartPtr<Expr>,
				      const std::vector<double>::size_type,
				      const std::vector<double>&);

      template<>
      tfel::utilities::SmartPtr<Expr>
      differentiateFunction<std::sin>(const tfel::utilities::SmartPtr<Expr>,
				      const std::vector<double>::size_type,
				      const std::vector<double>&);

      template<>
      tfel::utilities::SmartPtr<Expr>
      differentiateFunction<std::cos>(const tfel::utilities::SmartPtr<Expr>,
				      const std::vector<double>::size_type,
				      const std::vector<double>&);

      template<>
      tfel::utilities::SmartPtr<Expr>
      differentiateFunction<std::tan>(const tfel::utilities::SmartPtr<Expr>,
				      const std::vector<double>::size_type,
				      const std::vector<double>&);

      template<>
      tfel::utilities::SmartPtr<Expr>
      differentiateFunction<std::sqrt>(const tfel::utilities::SmartPtr<Expr>,
				       const std::vector<double>::size_type,
				       const std::vector<double>&);

      template<>
      tfel::utilities::SmartPtr<Expr>
      differentiateFunction<std::log>(const tfel::utilities::SmartPtr<Expr>,
				      const std::vector<double>::size_type,
				      const std::vector<double>&);

      template<>
      tfel::utilities::SmartPtr<Expr>
      differentiateFunction<std::log10>(const tfel::utilities::SmartPtr<Expr>,
					const std::vector<double>::size_type,
					const std::vector<double>&);

      template<>
      tfel::utilities::SmartPtr<Expr>
      differentiateFunction<std::asin>(const tfel::utilities::SmartPtr<Expr>,
				       const std::vector<double>::size_type,
				       const std::vector<double>&);
      template<>
      tfel::utilities::SmartPtr<Expr>
      differentiateFunction<std::acos>(const tfel::utilities::SmartPtr<Expr>,
				       const std::vector<double>::size_type,
				       const std::vector<double>&);

      template<>
      tfel::utilities::SmartPtr<Expr>
      differentiateFunction<std::atan>(const tfel::utilities::SmartPtr<Expr>,
				       const std::vector<double>::size_type,
				       const std::vector<double>&);

      template<>
      tfel::utilities::SmartPtr<Expr>
      differentiateFunction<std::sinh>(const tfel::utilities::SmartPtr<Expr>,
				       const std::vector<double>::size_type,
				       const std::vector<double>&);

      template<>
      tfel::utilities::SmartPtr<Expr>
      differentiateFunction<std::cosh>(const tfel::utilities::SmartPtr<Expr>,
				       const std::vector<double>::size_type,
				       const std::vector<double>&);

      template<>
      tfel::utilities::SmartPtr<Expr>
      differentiateFunction<std::tanh>(const tfel::utilities::SmartPtr<Expr>,
				       const std::vector<double>::size_type,
				       const std::vector<double>&);
      
    } // end of namespace parser

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_FUNCTION_IXX */

