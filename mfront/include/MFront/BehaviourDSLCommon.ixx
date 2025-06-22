/*!
 * \file   mfront/include/MFront/BehaviourDSLCommon.ixx
 * \brief
 * \author Helfer Thomas
 * \brief  14 févr. 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_MFRONTBEHAVIOURPARSERCOMMON_IXX_
#define LIB_MFRONT_MFRONTBEHAVIOURPARSERCOMMON_IXX_

namespace mfront {

  template <typename T, typename T2>
  BehaviourDSLCommon::StandardVariableModifier<T, T2>::StandardVariableModifier(
      T& i, const Hypothesis h, const MPtr p)
      : instance(i),
        hypothesis(h),
        mptr(p) {
  }  // end of StandardVariableModifier<T,T2>::StandardVariableModifier

  template <typename T, typename T2>
  std::string BehaviourDSLCommon::StandardVariableModifier<T, T2>::exe(
      const std::string& v, const bool b) {
    return (this->instance.*mptr)(this->hypothesis, v, b);
  }  // end of StandardVariableModifier<T,T2>::exe

  template <typename T, typename T2>
  BehaviourDSLCommon::StandardVariableModifier<T, T2>::
      ~StandardVariableModifier() {
  }  // end of StandardVariableModifier<T,T2>::~StandardVariableModifier

  template <typename T, typename T2>
  std::shared_ptr<BehaviourDSLCommon::VariableModifier>
  BehaviourDSLCommon::makeVariableModifier(
      T& i,
      const Hypothesis h,
      std::string (T2::*p)(const Hypothesis, const std::string&, const bool)) {
    return std::shared_ptr<VariableModifier>(
        new StandardVariableModifier<T, T2>(i, h, p));
  }  // end of makeVariableModifier

  template <typename T, typename T2>
  BehaviourDSLCommon::StandardWordAnalyser<T, T2>::StandardWordAnalyser(
      T& i, const Hypothesis h, const MPtr p)
      : instance(i),
        mptr(p),
        hypothesis(h) {
  }  // end of StandardWordAnalyser<T,T2>::StandardWordAnalyser

  template <typename T, typename T2>
  void BehaviourDSLCommon::StandardWordAnalyser<T, T2>::exe(
      const std::string& v) {
    (this->instance.*mptr)(this->hypothesis, v);
  }  // end of StandardWordAnalyser<T,T2>::exe

  template <typename T, typename T2>
  BehaviourDSLCommon::StandardWordAnalyser<T, T2>::~StandardWordAnalyser() {
  }  // end of StandardWordAnalyser<T,T2>::~StandardWordAnalyser

  template <typename T, typename T2>
  std::shared_ptr<BehaviourDSLCommon::WordAnalyser>
  BehaviourDSLCommon::makeWordAnalyser(T& i,
                                       const Hypothesis h,
                                       void (T2::*p)(const Hypothesis,
                                                     const std::string&)) {
    return std::shared_ptr<WordAnalyser>(
        new StandardWordAnalyser<T, T2>(i, h, p));
  }  // end of makeWordAnalyser

  template <typename T, typename T2>
  BehaviourDSLCommon::CodeBlockOptions BehaviourDSLCommon::readCodeBlock(
      T& child,
      const std::string& n,
      std::string (T2::*m)(const Hypothesis, const std::string&, const bool),
      const bool b,
      const bool s)

  {
    CodeBlockOptions o;
    this->readCodeBlockOptions(o, s);
    this->treatUnsupportedCodeBlockOptions(o);
    this->readCodeBlock(child, o, n, m, b);
    return o;
  }

  template <typename T, typename T2, typename T3>
  BehaviourDSLCommon::CodeBlockOptions BehaviourDSLCommon::readCodeBlock(
      T& child,
      const std::string& n,
      std::string (T2::*m)(const Hypothesis, const std::string&, const bool),
      void (T3::*a)(const Hypothesis, const std::string&),
      const bool b,
      const bool s)

  {
    CodeBlockOptions o;
    this->readCodeBlockOptions(o, s);
    this->treatUnsupportedCodeBlockOptions(o);
    this->readCodeBlock(child, o, n, m, a, b);
    return o;
  }  // end of BehaviourDSLCommon::readCodeBlock

  template <typename T, typename T2, typename T3>
  void BehaviourDSLCommon::readCodeBlock(
      T& child,
      const BehaviourDSLCommon::CodeBlockOptions& o,
      const std::string& n,
      std::string (T2::*m)(const Hypothesis, const std::string&, const bool),
      void (T3::*a)(const Hypothesis, const std::string&),
      const bool b)

  {
    const auto& h = o.hypotheses;
    const auto beg = this->current;
    for (auto ph = h.cbegin(); ph != h.cend(); ++ph) {
      this->current = beg;
      const auto& md = this->mb.getBehaviourData(*ph);
      auto vm = makeVariableModifier(child, *ph, m);
      auto wa = makeWordAnalyser(child, *ph, a);
      CodeBlockParserOptions option;
      option.qualifyStaticVariables = b;
      option.qualifyMemberVariables = b;
      option.modifier = vm;
      option.analyser = wa;
      option.mn = md.getRegistredMembersNames();
      option.smn = md.getRegistredStaticMembersNames();
      const auto& c = this->readNextBlock(option);
      this->disableVariableDeclaration(*ph);
      this->mb.setCode(*ph, n, c, o.m, o.p);
    }
  }  // end of BehaviourDSLCommon::readCodeBlock

  template <typename T, typename T2>
  void BehaviourDSLCommon::readCodeBlock(
      T& child,
      const BehaviourDSLCommon::CodeBlockOptions& o,
      const std::string& n,
      std::string (T2::*m)(const Hypothesis, const std::string&, const bool),
      const bool b) {
    const auto& h = o.hypotheses;
    const auto beg = this->current;
    for (auto ph = h.cbegin(); ph != h.cend(); ++ph) {
      const auto& md = this->mb.getBehaviourData(*ph);
      this->current = beg;
      auto vm = makeVariableModifier(child, *ph, m);
      CodeBlockParserOptions option;
      option.qualifyStaticVariables = b;
      option.qualifyMemberVariables = b;
      option.modifier = vm;
      option.mn = md.getRegistredMembersNames();
      option.smn = md.getRegistredStaticMembersNames();
      const auto& c = this->readNextBlock(option);
      this->disableVariableDeclaration(*ph);
      this->mb.setCode(*ph, n, c, o.m, o.p);
    }
  }  // end of BehaviourDSLCommon::readCodeBlock

  template <typename T, typename T2>
  BehaviourDSLCommon::CodeBlockOptions BehaviourDSLCommon::readCodeBlock(
      T& child,
      const std::string& n1,
      const std::string& n2,
      std::string (T2::*m1)(const Hypothesis, const std::string&, const bool),
      std::string (T2::*m2)(const Hypothesis, const std::string&, const bool),
      const bool b,
      const bool s)

  {
    using namespace std;
    using std::shared_ptr;
    CodeBlockOptions o;
    this->readCodeBlockOptions(o, s);
    this->treatUnsupportedCodeBlockOptions(o);
    this->readCodeBlock(child, o, n1, n2, m1, m2, b);
    return o;
  }  // end of BehaviourDSLCommon::readCodeBlock

  template <typename T, typename T2>
  void BehaviourDSLCommon::readCodeBlock(
      T& child,
      const BehaviourDSLCommon::CodeBlockOptions& o,
      const std::string& n1,
      const std::string& n2,
      std::string (T2::*m1)(const Hypothesis, const std::string&, const bool),
      std::string (T2::*m2)(const Hypothesis, const std::string&, const bool),
      const bool b)

  {
    const auto& hs = o.hypotheses;
    const auto beg = this->current;
    for (const auto& h : hs) {
      const auto& md = this->mb.getBehaviourData(h);
      this->current = beg;
      CodeBlock c1;
      CodeBlock c2;
      CodeBlockParserOptions o1;
      o1.qualifyStaticVariables = b;
      o1.qualifyMemberVariables = b;
      o1.modifier = makeVariableModifier(child, h, m1);
      o1.mn = md.getRegistredMembersNames();
      o1.smn = md.getRegistredStaticMembersNames();
      CodeBlockParserOptions o2;
      o2.qualifyStaticVariables = b;
      o2.qualifyMemberVariables = b;
      o2.modifier = makeVariableModifier(child, h, m2);
      o2.mn = md.getRegistredMembersNames();
      o2.smn = md.getRegistredStaticMembersNames();
      this->readNextBlock(c1, c2, o1, o2);
      this->disableVariableDeclaration(h);
      this->mb.setCode(h, n1, c1, o.m, o.p);
      this->mb.setCode(h, n2, c2, o.m, o.p);
    }
  }  // end of BehaviourDSLCommon::readCodeBlock

}  // end of namespace mfront

#endif /* LIB_MFRONT_MFRONTBEHAVIOURPARSERCOMMON_IXX_ */
