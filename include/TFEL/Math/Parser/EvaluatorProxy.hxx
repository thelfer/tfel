/*!
 * \file   include/TFEL/Math/Parser/EvaluatorProxy.hxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   13/01/2009
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_PARSER_EVALUATORPROXY_HXX
#define LIB_TFEL_MATH_PARSER_EVALUATORPROXY_HXX

#include "TFEL/Math/Parser/EvaluatorFunction.hxx"
#include "TFEL/Math/Evaluator.hxx"

#define TFEL_MATH_PARSER_EVALUATORPROXY(X) \
  EvaluatorProxy<X> evaluatorproxy##X(#X)
#define TFEL_MATH_PARSER_EVALUATORPROXY1P1V(X) \
  EvaluatorProxy1P1V<X> evaluatorproxy1p1v##X(#X)
#define TFEL_MATH_PARSER_EVALUATORPROXY2P1V(X) \
  EvaluatorProxy2P1V<X> evaluatorproxy2p1v##X(#X)
#define TFEL_MATH_PARSER_EVALUATORPROXY2V(X) \
  EvaluatorProxy2V<X> evaluatorproxy2V##X(#X)
#define TFEL_MATH_PARSER_EVALUATORPROXY1P2V(X) \
  EvaluatorProxy1P2V<X> evaluatorproxy1p2v##X(#X)
#define TFEL_MATH_PARSER_EVALUATORPROXY2P2V(X) \
  EvaluatorProxy2P2V<X> evaluatorproxy2p2v##X(#X)
#define TFEL_MATH_PARSER_EVALUATORPROXYNV(X, Y) \
  EvaluatorProxyNV<X, Y> evaluatorproxynv##Y(#Y)
#define TFEL_MATH_PARSER_EVALUATORPROXY1PNV(X, Y) \
  EvaluatorProxy1PNV<X, Y> evaluatorproxy1pnv##Y(#Y)
#define TFEL_MATH_PARSER_EVALUATORPROXY2PNV(X, Y) \
  EvaluatorProxy2PNV<X, Y> evaluatorproxy2pnv##Y(#Y)
#define TFEL_MATH_PARSER_EVALUATORPROXY1UPNV(X, Y) \
  EvaluatorProxy1UPNV<X, Y> evaluatorproxy1upnv##Y(#Y)
#define TFEL_MATH_PARSER_EVALUATORPROXY2UPNV(X, Y) \
  EvaluatorProxy2UPNV<X, Y> evaluatorproxy2upnv##Y(#Y)

namespace tfel::math::parser {

  std::shared_ptr<tfel::math::parser::Expr> EvaluatorFunction1P1VGenerator(
      EvaluatorProxyFunctionPtr1P1V,
      const std::vector<std::string>&,
      std::vector<std::shared_ptr<tfel::math::parser::Expr>>&);

  template <EvaluatorProxyFunctionPtr1V>
  struct TFEL_VISIBILITY_LOCAL EvaluatorProxy {
    EvaluatorProxy(const std::string&);
  };  // end of struct EvaluatorProxy

  template <EvaluatorProxyFunctionPtr2V>
  struct TFEL_VISIBILITY_LOCAL EvaluatorProxy2V {
    EvaluatorProxy2V(const std::string&);
  };  // end of struct EvaluatorProxy2V

  template <EvaluatorProxyFunctionPtr1P1V>
  struct TFEL_VISIBILITY_LOCAL EvaluatorProxy1P1V {
    EvaluatorProxy1P1V(const std::string&);
  };  // end of struct EvaluatorProxy1P1V

  template <EvaluatorProxyFunctionPtr2P1V>
  struct TFEL_VISIBILITY_LOCAL EvaluatorProxy2P1V {
    EvaluatorProxy2P1V(const std::string&);
  };  // end of struct EvaluatorProxy2P1V

  template <EvaluatorProxyFunctionPtr1P2V>
  struct TFEL_VISIBILITY_LOCAL EvaluatorProxy1P2V {
    EvaluatorProxy1P2V(const std::string&);
  };  // end of struct EvaluatorProxy1P2V

  template <EvaluatorProxyFunctionPtr2P2V>
  struct TFEL_VISIBILITY_LOCAL EvaluatorProxy2P2V {
    EvaluatorProxy2P2V(const std::string&);
  };  // end of struct EvaluatorProxy2P2V

  template <unsigned short N, typename EvaluatorFunctionWrapper<N>::type f>
  struct TFEL_VISIBILITY_LOCAL EvaluatorProxyNV {
    EvaluatorProxyNV(const std::string&);
  };  // end of struct EvaluatorProxyNV

  template <unsigned short N, typename EvaluatorFunctionWrapper<N>::type1P f>
  struct TFEL_VISIBILITY_LOCAL EvaluatorProxy1PNV {
    EvaluatorProxy1PNV(const std::string&);
  };  // end of struct EvaluatorProxy1PNV

  template <unsigned short N, typename EvaluatorFunctionWrapper<N>::type2P f>
  struct TFEL_VISIBILITY_LOCAL EvaluatorProxy2PNV {
    EvaluatorProxy2PNV(const std::string&);
  };  // end of struct EvaluatorProxy2PNV

  template <unsigned short N, typename EvaluatorFunctionWrapper<N>::type1UP f>
  struct TFEL_VISIBILITY_LOCAL EvaluatorProxy1UPNV {
    EvaluatorProxy1UPNV(const std::string&);
  };  // end of struct EvaluatorProxy1UPNV

  template <unsigned short N, typename EvaluatorFunctionWrapper<N>::type2UP f>
  struct TFEL_VISIBILITY_LOCAL EvaluatorProxy2UPNV {
    EvaluatorProxy2UPNV(const std::string&);
  };  // end of struct EvaluatorProxy2UPNV

}  // end of namespace tfel::math::parser

#include "TFEL/Math/Parser/EvaluatorProxy.ixx"

#endif /* LIB_TFEL_MATH_PARSER_EVALUATORPROXY_HXX */
