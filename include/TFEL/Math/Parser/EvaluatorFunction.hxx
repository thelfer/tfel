/*!
* \file   EvaluatorFunction.hxx
* \brief  
* 
* \author Helfer Thomas
* \date   15 jan 2009
*/

#ifndef _LIB_TFEL_MATH_EVALUATORFUNCTION_HXX_
#define _LIB_TFEL_MATH_EVALUATORFUNCTION_HXX_ 

#include"TFEL/Math/Evaluator.hxx"

namespace tfel
{

  namespace math
  {

    namespace parser
    {

#ifdef __SUNPRO_CC
      extern "C" {
#endif /* __SUNPRO_CC */
	typedef double (*EvaluatorProxyFunctionPtr1V)(double); 
	typedef double (*EvaluatorProxyFunctionPtr2V)(double,double); 
	typedef double (*EvaluatorProxyFunctionPtr1P1V)(int,double); 
	typedef double (*EvaluatorProxyFunctionPtr1P2V)(int,double,double);
	typedef double (*EvaluatorProxyFunctionPtr2P1V)(int,int,double);
	typedef double (*EvaluatorProxyFunctionPtr2P2V)(int,int,double,double);
#ifdef __SUNPRO_CC
     }
#endif /* __SUNPRO_CC */

      struct EvaluatorFunctionBase
	: public Function
      {
	EvaluatorFunctionBase();
	tfel::utilities::SmartPtr<Expr>
	differentiate(const std::vector<double>::size_type,
		      const std::vector<double>&) const;
	virtual ~EvaluatorFunctionBase();
      }; // end of struct EvaluatorFunctionBase

      struct EvaluatorFunction1VBase
	: public EvaluatorFunctionBase
      {
	EvaluatorFunction1VBase(const tfel::utilities::SmartPtr<Expr>);
	void
	checkCyclicDependency(std::vector<std::string>&) const;
	virtual ~EvaluatorFunction1VBase();
      protected:
	const tfel::utilities::SmartPtr<Expr> expr;
      }; // end of struct EvaluatorFunction1V

      struct EvaluatorFunction2VBase
	: public EvaluatorFunctionBase
      {
	EvaluatorFunction2VBase(const tfel::utilities::SmartPtr<Expr>,
				const tfel::utilities::SmartPtr<Expr>);
	void
	checkCyclicDependency(std::vector<std::string>&) const;
	virtual ~EvaluatorFunction2VBase();
      protected:
	const tfel::utilities::SmartPtr<Expr> e1;
	const tfel::utilities::SmartPtr<Expr> e2;
      }; // end of struct EvaluatorFunction2V

      struct EvaluatorFunction1P1V
	: public EvaluatorFunction1VBase
      {
	EvaluatorFunction1P1V(const EvaluatorProxyFunctionPtr1P1V,
			      const int,
			      const tfel::utilities::SmartPtr<Expr>);
	double getValue(void) const;
	tfel::utilities::SmartPtr<Expr>
	resolveDependencies(void) const;
	tfel::utilities::SmartPtr<Expr>
	clone(const std::vector<double>&) const;
	~EvaluatorFunction1P1V();
      private:
	EvaluatorProxyFunctionPtr1P1V f;
	const int n;
      }; // end of struct EvaluatorFunction1P1V

      struct EvaluatorFunction2P1V
	: public EvaluatorFunction1VBase
      {
	EvaluatorFunction2P1V(const EvaluatorProxyFunctionPtr2P1V,
			      const int,
			      const int,
			      const tfel::utilities::SmartPtr<Expr>);
	double getValue(void) const;
	tfel::utilities::SmartPtr<Expr>
	resolveDependencies(void) const;
	tfel::utilities::SmartPtr<Expr>
	clone(const std::vector<double>&) const;
	~EvaluatorFunction2P1V();
      private:
	EvaluatorProxyFunctionPtr2P1V f;
	const int n;
	const int m;
      }; // end of struct EvaluatorFunction2P1V

      struct EvaluatorFunction1P2V
	: public EvaluatorFunction2VBase
      {
	EvaluatorFunction1P2V(const EvaluatorProxyFunctionPtr1P2V,
			      const int,
			      const tfel::utilities::SmartPtr<Expr>,
			      const tfel::utilities::SmartPtr<Expr>);
	double getValue(void) const;
	tfel::utilities::SmartPtr<Expr>
	resolveDependencies(void) const;
	tfel::utilities::SmartPtr<Expr>
	clone(const std::vector<double>&) const;
	~EvaluatorFunction1P2V();
      private:
	EvaluatorProxyFunctionPtr1P2V f;
	const int n;
      }; // end of struct EvaluatorFunction1P2V

      struct EvaluatorFunction2P2V
	: public EvaluatorFunction2VBase
      {
	EvaluatorFunction2P2V(const EvaluatorProxyFunctionPtr2P2V,
			      const int,
			      const int,
			      const tfel::utilities::SmartPtr<Expr>,
			      const tfel::utilities::SmartPtr<Expr>);
	double getValue(void) const;
	tfel::utilities::SmartPtr<Expr>
	resolveDependencies(void) const;
	tfel::utilities::SmartPtr<Expr>
	clone(const std::vector<double>&) const;
	~EvaluatorFunction2P2V();
      private:
	EvaluatorProxyFunctionPtr2P2V f;
	const int n;
	const int m;
      }; // end of struct EvaluatorFunction2P2V

      template<unsigned short N>
      struct EvaluatorFunctionWrapper;

      template<unsigned short N>
      struct EvaluatorFunctionNV
	: public EvaluatorFunctionBase
      {
	EvaluatorFunctionNV(typename EvaluatorFunctionWrapper<N>::type,
			    const std::vector<tfel::utilities::SmartPtr<Expr> >&);
	double getValue(void) const;
	tfel::utilities::SmartPtr<Expr>
	resolveDependencies(void) const;
	tfel::utilities::SmartPtr<Expr>
	clone(const std::vector<double>&) const;
	void
	checkCyclicDependency(std::vector<std::string>&) const;
	virtual ~EvaluatorFunctionNV();
      protected:
	typename EvaluatorFunctionWrapper<N>::type f;
	const std::vector<tfel::utilities::SmartPtr<Expr> > args;
      }; // end of struct EvaluatorFunctionNV

      template<unsigned short N>
      struct EvaluatorFunction1PNV
	: public EvaluatorFunctionBase
      {
	EvaluatorFunction1PNV(typename EvaluatorFunctionWrapper<N>::type1P,
			      const int,
			      const std::vector<tfel::utilities::SmartPtr<Expr> >&);
	double getValue(void) const;
	tfel::utilities::SmartPtr<Expr>
	resolveDependencies(void) const;
	tfel::utilities::SmartPtr<Expr>
	clone(const std::vector<double>&) const;
	void
	checkCyclicDependency(std::vector<std::string>&) const;
	virtual ~EvaluatorFunction1PNV();
      protected:
	typename EvaluatorFunctionWrapper<N>::type1P f;
	const int n;
	const std::vector<tfel::utilities::SmartPtr<Expr> > args;
      }; // end of struct EvaluatorFunction1PNV

      template<unsigned short N>
      struct EvaluatorFunction2PNV
	: public EvaluatorFunctionBase
      {
	EvaluatorFunction2PNV(typename EvaluatorFunctionWrapper<N>::type2P,
			      const int,const int,
			      const std::vector<tfel::utilities::SmartPtr<Expr> >&);
	double getValue(void) const;
	tfel::utilities::SmartPtr<Expr>
	resolveDependencies(void) const;
	tfel::utilities::SmartPtr<Expr>
	clone(const std::vector<double>&) const;
	void
	checkCyclicDependency(std::vector<std::string>&) const;
	virtual ~EvaluatorFunction2PNV();
      protected:
	typename EvaluatorFunctionWrapper<N>::type2P f;
	const int n;
	const int m;
	const std::vector<tfel::utilities::SmartPtr<Expr> > args;
      }; // end of struct EvaluatorFunction2PNV

      template<unsigned short N>
      struct EvaluatorFunction1UPNV
	: public EvaluatorFunctionBase
      {
	EvaluatorFunction1UPNV(typename EvaluatorFunctionWrapper<N>::type1UP,
			       const unsigned int,
			       const std::vector<tfel::utilities::SmartPtr<Expr> >&);
	double getValue(void) const;
	tfel::utilities::SmartPtr<Expr>
	resolveDependencies(void) const;
	tfel::utilities::SmartPtr<Expr>
	clone(const std::vector<double>&) const;
	void
	checkCyclicDependency(std::vector<std::string>&) const;
	virtual ~EvaluatorFunction1UPNV();
      protected:
	typename EvaluatorFunctionWrapper<N>::type1UP f;
	const unsigned int n;
	const std::vector<tfel::utilities::SmartPtr<Expr> > args;
      }; // end of struct EvaluatorFunction1UPNV

      template<unsigned short N>
      struct EvaluatorFunction2UPNV
	: public EvaluatorFunctionBase
      {
	EvaluatorFunction2UPNV(typename EvaluatorFunctionWrapper<N>::type2UP,
			       const unsigned int n,
			       const unsigned int m,
			       const std::vector<tfel::utilities::SmartPtr<Expr> >&);
	double getValue(void) const;
	tfel::utilities::SmartPtr<Expr>
	resolveDependencies(void) const;
	tfel::utilities::SmartPtr<Expr>
	clone(const std::vector<double>&) const;
	void
	checkCyclicDependency(std::vector<std::string>&) const;
	virtual ~EvaluatorFunction2UPNV();
      protected:
	typename EvaluatorFunctionWrapper<N>::type2UP f;
	const unsigned int n;
	const unsigned int m;
	const std::vector<tfel::utilities::SmartPtr<Expr> > args;
      }; // end of struct EvaluatorFunction2UPNV

    } // end of namespace parser

  } // end of namespace math

} // end of namespace tfel

#include"TFEL/Math/Parser/EvaluatorFunction.ixx"

#endif /* _LIB_TFEL_MATH_EVALUATORFUNCTION_HXX_  */
