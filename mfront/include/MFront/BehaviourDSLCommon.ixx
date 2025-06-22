/*!
 * \file   mfront/include/MFront/BehaviourDSLCommon.ixx
 * \brief
 * \author Thomas Helfer
 * \brief  14 févr. 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_MFRONTBEHAVIOURPARSERCOMMON_IXX
#define LIB_MFRONT_MFRONTBEHAVIOURPARSERCOMMON_IXX

namespace mfront {

  template <typename T, typename T2>
  BehaviourDSLCommon::CodeBlockOptions BehaviourDSLCommon::treatCodeBlock(
      T& child,
      const std::string& n,
      std::string (T2::*m)(const Hypothesis, const std::string&, const bool),
      const bool b,
      const bool s) {
    std::function<std::string(const Hypothesis, const std::string&, const bool)>
        f = [&child, m](const Hypothesis h, const std::string& sv,
                        const bool bv) { return (child.*m)(h, sv, bv); };
    return this->treatCodeBlock(n, f, b, s);
  }

  template <typename T, typename T2, typename T3>
  BehaviourDSLCommon::CodeBlockOptions BehaviourDSLCommon::treatCodeBlock(
      T& child,
      const std::string& n,
      std::string (T2::*m)(const Hypothesis, const std::string&, const bool),
      void (T3::*a)(const Hypothesis, const std::string&),
      const bool b,
      const bool s) {
    std::function<std::string(const Hypothesis, const std::string&, const bool)>
        fm = [&child, m](const Hypothesis h, const std::string& sv,
                         const bool bv) { (child.*m)(h, sv, bv); };
    std::function<void(CodeBlock&, const Hypothesis, const std::string&)> fa =
        [&child, a](CodeBlock&, const Hypothesis h, const std::string& sv) {
          (child.*a)(h, sv);
        };
    return this->treatCodeBlock(n, fm, fa, b, s);
  }  // end of BehaviourDSLCommon::treatCodeBlock

  template <typename T, typename T2, typename T3>
  void BehaviourDSLCommon::treatCodeBlock(
      T& child,
      const BehaviourDSLCommon::CodeBlockOptions& o,
      const std::string& n,
      std::string (T2::*m)(const Hypothesis, const std::string&, const bool),
      void (T3::*a)(const Hypothesis, const std::string&),
      const bool b) {
    std::function<std::string(const Hypothesis, const std::string&, const bool)>
        fm = [&child, m](const Hypothesis h, const std::string& sv,
                         const bool bv) { (child.*m)(h, sv, bv); };
    std::function<void(CodeBlock&, const Hypothesis, const std::string&)> fa =
        [&child, a](CodeBlock&, const Hypothesis h, const std::string& sv) {
          (child.*a)(h, sv);
        };
    this->treatCodeBlock(o, n, fm, fa, b);
  }  // end of BehaviourDSLCommon::treatCodeBlock

  template <typename T, typename T2>
  void BehaviourDSLCommon::treatCodeBlock(
      T& child,
      const BehaviourDSLCommon::CodeBlockOptions& o,
      const std::string& n,
      std::string (T2::*m)(const Hypothesis, const std::string&, const bool),
      const bool b) {
    std::function<std::string(const Hypothesis, const std::string&, const bool)>
        f = [&child, m](const Hypothesis h, const std::string& sv,
                        const bool bv) { return (child.*m)(h, sv, bv); };
    this->treatCodeBlock(o, n, f, b);
  }  // end of BehaviourDSLCommon::treatCodeBlock

  template <typename T, typename T2>
  BehaviourDSLCommon::CodeBlockOptions BehaviourDSLCommon::treatCodeBlock(
      T& child,
      const std::string& n1,
      const std::string& n2,
      std::string (T2::*m1)(const Hypothesis, const std::string&, const bool),
      std::string (T2::*m2)(const Hypothesis, const std::string&, const bool),
      const bool b,
      const bool s) {
    std::function<std::string(const Hypothesis, const std::string&, const bool)>
        f1 = [&child, m1](const Hypothesis h, const std::string& sv,
                          const bool bv) { return (child.*m1)(h, sv, bv); };
    std::function<std::string(const Hypothesis, const std::string&, const bool)>
        f2 = [&child, m2](const Hypothesis h, const std::string& sv,
                          const bool bv) { return (child.*m2)(h, sv, bv); };
    return this->treatCodeBlock(n1, n2, f1, f2, b, s);
  }  // end of BehaviourDSLCommon::treatCodeBlock

  template <typename T, typename T2>
  void BehaviourDSLCommon::treatCodeBlock(
      T& child,
      const BehaviourDSLCommon::CodeBlockOptions& o,
      const std::string& n1,
      const std::string& n2,
      std::string (T2::*m1)(const Hypothesis, const std::string&, const bool),
      std::string (T2::*m2)(const Hypothesis, const std::string&, const bool),
      const bool b) {
    std::function<std::string(const Hypothesis, const std::string&, const bool)>
        f1 = [&child, m1](const Hypothesis h, const std::string& sv,
                          const bool bv) { return (child.*m1)(h, sv, bv); };
    std::function<std::string(const Hypothesis, const std::string&, const bool)>
        f2 = [&child, m2](const Hypothesis h, const std::string& sv,
                          const bool bv) { (child.*m2)(h, sv, bv); };
    this->treatCodeBlock(o, n1, n2, f1, f2, b);
  }  // end of BehaviourDSLCommon::treatCodeBlock

}  // end of namespace mfront

#endif /* LIB_MFRONT_MFRONTBEHAVIOURPARSERCOMMON_IXX */
