/*!
 * \file   include/TFEL/Math/Parser/Function.ixx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   02 oct 2007
 */

#ifndef _LIB_TFEL_FUNCTION_IXX_
#define _LIB_TFEL_FUNCTION_IXX_ 

#include<string>
#include<stdexcept>
#include<cerrno>
#include<cstring>
#include<cmath>

#ifndef __SUNPRO_CC
#define TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DECLARATION(X) \
      template<>                                                             \
      tfel::utilities::shared_ptr<Expr>                                        \
      differentiateFunction<std::X>(const tfel::utilities::shared_ptr<Expr>,   \
				    const std::vector<double>::size_type,    \
				    const std::vector<double>&)
#else  /* __SUNPRO_CC */
#define TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DECLARATION(X) \
      template<>                                                             \
      tfel::utilities::shared_ptr<Expr>                                        \
      differentiateFunction<X>(const tfel::utilities::shared_ptr<Expr>,        \
			       const std::vector<double>::size_type,         \
			       const std::vector<double>&)
#endif /* __SUNPRO_CC */

namespace tfel
{

  namespace math
  {

    namespace parser
    {

      template<StandardFunctionPtr f>
      StandardFunction<f>::StandardFunction(const tfel::utilities::shared_ptr<Expr> e)
	: expr(e)
      {} // end of StandardFunction::StandardFunction

      template<StandardFunctionPtr f>
      StandardFunction<f>::~StandardFunction()
      {} // end of StandardFunction::StandardFunction

      template<StandardFunctionPtr f>
      void
      StandardFunction<f>::getParametersNames(std::set<std::string>& p) const
      {
	this->expr->getParametersNames(p);
      } // end of StandardFunction<f>::getParametersNames
      
      template<StandardFunctionPtr f>
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
	  int e = errno;
	  errno = old;
	  StandardFunctionBase::throwInvalidCallException(arg,e);
	}
	errno = old;
	return res;
      } // end of StandardFunction::StandardFunction

      template<StandardFunctionPtr f>
      void
      StandardFunction<f>::checkCyclicDependency(std::vector<std::string>& names) const
      {
	this->expr->checkCyclicDependency(names);
      } // end of StandardFunction<f>::checkCyclicDependency

      template<StandardFunctionPtr f>
      tfel::utilities::shared_ptr<Expr>
      StandardFunction<f>::resolveDependencies(const std::vector<double>& v) const
      {
	using namespace tfel::utilities;
	return shared_ptr<Expr>(new StandardFunction<f>(this->expr->resolveDependencies(v)));
      } // end of StandardFunction<f>::resolveDependencies(void)
      
      template<StandardFunctionPtr f>
      tfel::utilities::shared_ptr<Expr>
      StandardFunction<f>::clone(const std::vector<double>& v) const
      {
	using namespace tfel::utilities;
	return shared_ptr<Expr>(new StandardFunction<f>(this->expr->clone(v)));
      } // end of StandardFunction<f>::clone

      template<StandardFunctionPtr f>
      tfel::utilities::shared_ptr<Expr>
      StandardFunction<f>::createFunctionByChangingParametersIntoVariables(const std::vector<double>& v,
									   const std::vector<std::string>& p,
									   const std::map<std::string,
									   std::vector<double>::size_type>& pos) const
      {
	using namespace tfel::utilities;
	shared_ptr<Expr> nexpr = this->expr->createFunctionByChangingParametersIntoVariables(v,p,pos);
	return shared_ptr<Expr>(new StandardFunction<f>(nexpr));
      } // end of StandardFunction<f>::createFunctionByChangingParametersIntoVariables

      template<StandardFunctionPtr f>
      tfel::utilities::shared_ptr<Expr>
      differentiateFunction(const tfel::utilities::shared_ptr<Expr>,
			    const std::vector<double>::size_type,
			    const std::vector<double>&)
      {
	using namespace std;
	using namespace tfel::utilities;
	StandardFunctionBase::throwUnimplementedDifferentiateFunctionException();
	return shared_ptr<Expr>(static_cast<Expr*>(0));
      }
      
      template<StandardFunctionPtr f>
      tfel::utilities::shared_ptr<Expr>
      StandardFunction<f>::differentiate(const std::vector<double>::size_type pos,
					 const std::vector<double>& v) const
      {
	return differentiateFunction<f>(this->expr,pos,v);
      } // end of StandardFunction<f>::differentiate

      TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DECLARATION(exp);
      TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DECLARATION(sin);
      TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DECLARATION(cos);
      TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DECLARATION(tan);
      TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DECLARATION(sqrt);
      TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DECLARATION(log);
      TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DECLARATION(log10);
      TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DECLARATION(asin);
      TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DECLARATION(acos);
      TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DECLARATION(atan);
      TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DECLARATION(sinh);
      TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DECLARATION(cosh);
      TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DECLARATION(tanh);
      
    } // end of namespace parser

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_FUNCTION_IXX */

