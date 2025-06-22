/*!
 * \file   include/TFEL/Math/Parser/EvaluatorProxy.ixx
 * \brief
 *
 * \author Helfer Thomas
 * \date   15 jan 2009
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_EVALUATORPROXY_IXX_
#define LIB_TFEL_MATH_EVALUATORPROXY_IXX_

namespace tfel {

  namespace math {

    namespace parser {

      template <EvaluatorProxyFunctionPtr1V f>
      EvaluatorProxy<f>::EvaluatorProxy(const std::string& name) {
        using namespace tfel::math;
        Evaluator::getFunctionGeneratorManager().insert(
            name, &StandardFctGenerator<f>);
      }

      template <EvaluatorProxyFunctionPtr2V f>
      EvaluatorProxy2V<f>::EvaluatorProxy2V(const std::string& name) {
        using namespace tfel::math;
        Evaluator::getFunctionGeneratorManager().insert(
            name, &StandardBinaryFctGenerator<f>);
      }  // end of struct EvaluatorProxy::EvaluatorProxy2V

      template <EvaluatorProxyFunctionPtr1P1V f>
      EvaluatorProxy1P1V<f>::EvaluatorProxy1P1V(const std::string& name) {
        using namespace tfel::math;
        Evaluator::getFunctionGeneratorManager().insert(
            name, &EvaluatorFunction1P1VGenerator<f>);
      }  // end of EvaluatorProxy1P1V::EvaluatorProxy1P1V

      template <EvaluatorProxyFunctionPtr2P1V f>
      EvaluatorProxy2P1V<f>::EvaluatorProxy2P1V(const std::string& name) {
        using namespace tfel::math;
        Evaluator::getFunctionGeneratorManager().insert(
            name, &EvaluatorFunction2P1VGenerator<f>);
      }  // end of EvaluatorProxy2P1V::EvaluatorProxy2P1V

      template <EvaluatorProxyFunctionPtr1P2V f>
      EvaluatorProxy1P2V<f>::EvaluatorProxy1P2V(const std::string& name) {
        using namespace tfel::math;
        Evaluator::getFunctionGeneratorManager().insert(
            name, &EvaluatorFunction1P2VGenerator<f>);
      }  // end of EvaluatorProxy1P2V::EvaluatorProxy1P2V

      template <EvaluatorProxyFunctionPtr2P2V f>
      EvaluatorProxy2P2V<f>::EvaluatorProxy2P2V(const std::string& name) {
        using namespace tfel::math;
        Evaluator::getFunctionGeneratorManager().insert(
            name, &EvaluatorFunction2P2VGenerator<f>);
      }  // end of EvaluatorProxy2P2V::EvaluatorProxy2P2V

      template <unsigned short N, typename EvaluatorFunctionWrapper<N>::type f>
      EvaluatorProxyNV<N, f>::EvaluatorProxyNV(const std::string& name) {
        using namespace tfel::math;
        Evaluator::getFunctionGeneratorManager().insert(
            name, &EvaluatorFunctionNVGenerator<N, f>);
      }  // end of EvaluatorProxyNV::EvaluatorProxyNV

      template <unsigned short N,
                typename EvaluatorFunctionWrapper<N>::type1P f>
      EvaluatorProxy1PNV<N, f>::EvaluatorProxy1PNV(const std::string& name) {
        using namespace tfel::math;
        Evaluator::getFunctionGeneratorManager().insert(
            name, &EvaluatorFunction1PNVGenerator<N, f>);
      }  // end of EvaluatorProxy1PNV::EvaluatorProxy1PNV

      template <unsigned short N,
                typename EvaluatorFunctionWrapper<N>::type2P f>
      EvaluatorProxy2PNV<N, f>::EvaluatorProxy2PNV(const std::string& name) {
        using namespace tfel::math;
        Evaluator::getFunctionGeneratorManager().insert(
            name, &EvaluatorFunction2PNVGenerator<N, f>);
      }  // end of EvaluatorProxy2PNV::EvaluatorProxy2PNV

      template <unsigned short N,
                typename EvaluatorFunctionWrapper<N>::type1UP f>
      EvaluatorProxy1UPNV<N, f>::EvaluatorProxy1UPNV(const std::string& name) {
        using namespace tfel::math;
        Evaluator::getFunctionGeneratorManager().insert(
            name, &EvaluatorFunction1UPNVGenerator<N, f>);
      }  // end of EvaluatorProxy1UPNV::EvaluatorProxy1UPNV

      template <unsigned short N,
                typename EvaluatorFunctionWrapper<N>::type2UP f>
      EvaluatorProxy2UPNV<N, f>::EvaluatorProxy2UPNV(const std::string& name) {
        using namespace tfel::math;
        Evaluator::getFunctionGeneratorManager().insert(
            name, &EvaluatorFunction2UPNVGenerator<N, f>);
      }  // end of EvaluatorProxy2UPNV::EvaluatorProxy2UPNV

    }  // end of namespace parser

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_EVALUATORPROXY_IXX_ */
