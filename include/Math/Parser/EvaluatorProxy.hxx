/*!
* \file   EvaluatorProxy.hxx
* \brief  
* 
* \author Helfer Thomas
* \date   13 jan 2009
*/

#ifndef _LIB_TFEL_MATH_EVALUATORPROXY_HXX_
#define _LIB_TFEL_MATH_EVALUATORPROXY_HXX_ 

#include"Math/Parser/EvaluatorFunction.hxx"
#include"Math/Evaluator.hxx"

#define TFEL_MATH_PARSER_EVALUATORPROXY(X)	      \
  EvaluatorProxy<X> evaluatorproxy##X(#X)
#define TFEL_MATH_PARSER_EVALUATORPROXY1P1V(X)	      \
  EvaluatorProxy1P1V<X> evaluatorproxy1p1v##X(#X)
#define TFEL_MATH_PARSER_EVALUATORPROXY2P1V(X)	      \
  EvaluatorProxy2P1V<X> evaluatorproxy2p1v##X(#X)
#define TFEL_MATH_PARSER_EVALUATORPROXY2V(X)	      \
  EvaluatorProxy2V<X> evaluatorproxy2V##X(#X)
#define TFEL_MATH_PARSER_EVALUATORPROXY1P2V(X)	      \
  EvaluatorProxy1P2V<X> evaluatorproxy1p2v##X(#X)
#define TFEL_MATH_PARSER_EVALUATORPROXY2P2V(X)	      \
  EvaluatorProxy2P2V<X> evaluatorproxy2p2v##X(#X)
#define TFEL_MATH_PARSER_EVALUATORPROXYNV(X,Y)	      \
  EvaluatorProxyNV<X,Y> evaluatorproxynv##Y(#Y)
#define TFEL_MATH_PARSER_EVALUATORPROXY1PNV(X,Y)      \
  EvaluatorProxy1PNV<X,Y> evaluatorproxy1pnv##Y(#Y)
#define TFEL_MATH_PARSER_EVALUATORPROXY2PNV(X,Y)      \
  EvaluatorProxy2PNV<X,Y> evaluatorproxy2pnv##Y(#Y)
#define TFEL_MATH_PARSER_EVALUATORPROXY1UPNV(X,Y)     \
  EvaluatorProxy1UPNV<X,Y> evaluatorproxy1upnv##Y(#Y)
#define TFEL_MATH_PARSER_EVALUATORPROXY2UPNV(X,Y)     \
  EvaluatorProxy2UPNV<X,Y> evaluatorproxy2upnv##Y(#Y)

namespace tfel
{

  namespace math
  {

    namespace parser
    {
    

      tfel::utilities::SmartPtr<tfel::math::parser::Expr>
      EvaluatorFunction1P1VGenerator(double (*)(int,double),
				     const std::vector<std::string>&,
				     std::vector<tfel::utilities::SmartPtr<tfel::math::parser::Expr> >&);

      template<double (*f)(double)>
      struct EvaluatorProxy
      {
	EvaluatorProxy(const std::string&);
      }; // end of struct EvaluatorProxy

      template<double (*f)(double,double)>
      struct EvaluatorProxy2V
      {
	EvaluatorProxy2V(const std::string&);
      }; // end of struct EvaluatorProxy2V

      template<double (*)(int,double)>			   
      struct EvaluatorProxy1P1V
      {
	EvaluatorProxy1P1V(const std::string&);
      }; // end of struct EvaluatorProxy1P1V

      template<double (*)(int,int,double)>			   
      struct EvaluatorProxy2P1V
      {
	EvaluatorProxy2P1V(const std::string&);
      }; // end of struct EvaluatorProxy2P1V

      template<double (*f)(int,double,double)>			   
      struct EvaluatorProxy1P2V
      {
	EvaluatorProxy1P2V(const std::string&);
      }; // end of struct EvaluatorProxy1P2V

      template<double (*f)(int,int,double,double)>
      struct EvaluatorProxy2P2V
      {
	EvaluatorProxy2P2V(const std::string&);
      }; // end of struct EvaluatorProxy2P2V

      template<unsigned short N,
	       typename EvaluatorFunctionWrapper<N>::type f>
      struct EvaluatorProxyNV
      {
	EvaluatorProxyNV(const std::string&);
      }; // end of struct EvaluatorProxyNV

      template<unsigned short N,
	       typename EvaluatorFunctionWrapper<N>::type1P f>
      struct EvaluatorProxy1PNV
      {
	EvaluatorProxy1PNV(const std::string&);
      }; // end of struct EvaluatorProxy1PNV

      template<unsigned short N,
	       typename EvaluatorFunctionWrapper<N>::type2P f>
      struct EvaluatorProxy2PNV
      {
	EvaluatorProxy2PNV(const std::string&);
      }; // end of struct EvaluatorProxy2PNV

      template<unsigned short N,
	       typename EvaluatorFunctionWrapper<N>::type1UP f>
      struct EvaluatorProxy1UPNV
      {
	EvaluatorProxy1UPNV(const std::string&);
      }; // end of struct EvaluatorProxy1UPNV

      template<unsigned short N,
	       typename EvaluatorFunctionWrapper<N>::type2UP f>
      struct EvaluatorProxy2UPNV
      {
	EvaluatorProxy2UPNV(const std::string&);
      }; // end of struct EvaluatorProxy2UPNV

    } // end of namespace parser

  } // end of namespace math

} // end of namespace tfel

#include"Math/Parser/EvaluatorProxy.ixx"

#endif /* _LIB_TFEL_MATH_EVALUATORPROXY_HXX_ */
