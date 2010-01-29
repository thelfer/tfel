/*!
 * \file   BinaryFunction.ixx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   02 oct 2007
 */

#ifndef _LIB_TFEL_BINARYFUNCTION_IXX_
#define _LIB_TFEL_BINARYFUNCTION_IXX_ 

#include<string>
#include<stdexcept>
#include<cstring>
#include<cerrno>

namespace tfel
{
  namespace math
  {

    namespace parser
    {

      template<double (*f)(const double,const double)>
      StandardBinaryFunction<f>::StandardBinaryFunction(const tfel::utilities::SmartPtr<Expr> e1,
							const tfel::utilities::SmartPtr<Expr> e2)
	: expr1(e1),expr2(e2)
      {} // end of StandardBinaryFunction::StandardBinaryFunction

      template<double (*f)(const double,const double)>
      StandardBinaryFunction<f>::~StandardBinaryFunction()
      {} // end of StandardBinaryFunction::StandardBinaryFunction

      template<double (*f)(const double,const double)>
      double
      StandardBinaryFunction<f>::getValue(void) const
      {
	using namespace std;
	double res;
	int old = errno;
	errno = 0;
	res = f(this->expr1->getValue(),this->expr2->getValue());
	if(errno!=0){
	  int e = errno;
	  errno = old;
	  StandardBinaryFunctionBase::throwInvalidCallException(e);
	}
	return res;
      } // end of StandardBinaryFunction::StandardBinaryFunction

      template<double (*f)(const double,const double)>
      void
      StandardBinaryFunction<f>::checkCyclicDependency(std::vector<std::string>& names) const
      {
	using namespace std;
	vector<string> a_names;
	vector<string> b_names;
	this->expr1->checkCyclicDependency(a_names);
	this->expr2->checkCyclicDependency(b_names);
	mergeVariablesNames(names,a_names);
	mergeVariablesNames(names,b_names);
      } // end of StandardBinaryFunction<f>::checkCyclicDependency

      template<double (*f)(const double,const double)>
      tfel::utilities::SmartPtr<Expr>
      StandardBinaryFunction<f>::resolveDependencies(void) const
      {
	using namespace tfel::utilities;
	return SmartPtr<Expr>(new StandardBinaryFunction<f>(this->expr1->resolveDependencies(),
							    this->expr2->resolveDependencies()));
      } // end of StandardBinaryFunction<f>::resolveDependencies

      template<double (*f)(const double,const double)>
      tfel::utilities::SmartPtr<Expr>
      StandardBinaryFunction<f>::differentiate(const std::vector<double>::size_type,
					       const std::vector<double>&) const
      {
	using namespace std;
	using namespace tfel::utilities;
	StandardBinaryFunctionBase::throwUnimplementedDifferentiateFunctionException();
	return SmartPtr<Expr>(0);
      } // end of StandardBinaryFunction<f>::differentiate

      template<double (*f)(const double,const double)>
      tfel::utilities::SmartPtr<Expr>
      StandardBinaryFunction<f>::clone(const std::vector<double>& v) const
      {
	using namespace tfel::utilities;
	return SmartPtr<Expr>(new StandardBinaryFunction<f>(this->expr1->clone(v),
							    this->expr2->clone(v)));
      } // end of StandardBinaryFunction<f>::clone

    } // end of namespace parser

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_BINARYFUNCTION_IXX */

