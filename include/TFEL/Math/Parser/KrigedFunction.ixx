/*!
 * \file   include/TFEL/Math/Parser/KrigedFunction.ixx
 * \brief
 *
 * \author Thomas Helfer
 * \date   02/11/2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_PARSER_KRIGEDFUNCTION_IXX
#define LIB_TFEL_MATH_PARSER_KRIGEDFUNCTION_IXX

namespace tfel::math::parser {

  template <unsigned short N>
  KrigedFunction<N>::KrigedFunction(
      const std::vector<typename KrigedFunction<N>::Point>& points,
      const double n)
      : k(new Kriging<N>()) {
    for (const auto& p : points) {
      this->k->addValue(p.first, p.second);
    }
    this->k->setNuggetEffect(n);
    this->k->buildInterpolation();
  }  // end of KrigedFunction<N>::KrigedFunction

  template <unsigned short N>
  KrigedFunction<N>::KrigedFunction(const KrigedFunction<N>& src)
      : ExternalFunction(src),
        k(src.k),
        v(src.v) {}  // end of KrigedFunction<N>::KrigedFunction

  template <unsigned short N>
  double KrigedFunction<N>::getValue() const {
    return this->k->operator()(this->v);
  }  // end of KrigedFunction<N>::getValue()

  template <unsigned short N>
  void KrigedFunction<N>::setVariableValue(
      const std::vector<double>::size_type i_, const double v_) {
    typename Variable::size_type i(
        static_cast<typename Variable::size_type>(i_));
    if (i >= N) {
      KrigedFunctionBase::throwInvalidIndexException(i, N);
    }
    this->v(i) = v_;
  }  // end of KrigedFunction<N>::setVariableValue

  template <>
  TFELMATHPARSER_VISIBILITY_EXPORT void KrigedFunction<1u>::setVariableValue(
      const std::vector<double>::size_type, const double);

  template <unsigned short N>
  std::vector<double>::size_type KrigedFunction<N>::getNumberOfVariables()
      const {
    return N;
  }  // end of KrigedFunction<N>::getNumberOfVariables

  template <unsigned short N>
  void KrigedFunction<N>::getParametersNames(std::set<std::string>&) const {
  }  // end of KrigedFunction<N>::getParametersNames

  template <unsigned short N>
  void KrigedFunction<N>::checkCyclicDependency(const std::string&) const {
  }  // end of KrigedFunction<N>::checkCyclicDependency

  template <unsigned short N>
  void KrigedFunction<N>::checkCyclicDependency(
      std::vector<std::string>&) const {
  }  // end of KrigedFunction<N>::checkCyclicDependency

  template <unsigned short N>
  std::shared_ptr<ExternalFunction> KrigedFunction<N>::differentiate(
      const std::vector<double>::size_type) const {
    KrigedFunctionBase::throwUnimplementedDifferentiateFunctionException();
#ifndef _MSC_VER
    return {};
#endif
  }  // end of KrigedFunction<N>::differentiate

  template <unsigned short N>
  std::shared_ptr<ExternalFunction> KrigedFunction<N>::differentiate(
      const std::string&) const {
    KrigedFunctionBase::throwUnimplementedDifferentiateFunctionException();
#ifndef _MSC_VER
    return {};
#endif
  }  // end of KrigedFunction<N>::differentiate

  template <unsigned short N>
  std::shared_ptr<ExternalFunction> KrigedFunction<N>::resolveDependencies()
      const {
    return std::shared_ptr<ExternalFunction>(new KrigedFunction<N>(*this));
  }  // end of KrigedFunction<N>::resolveDependencies

  template <unsigned short N>
  std::shared_ptr<ExternalFunction>
  KrigedFunction<N>::createFunctionByChangingParametersIntoVariables(
      const std::vector<std::string>&) const {
    KrigedFunctionBase::
        throwInvalidCreateFunctionByChangingParametersIntoVariables();
#ifndef _MSC_VER
    return {};
#endif
  }  // end of
     // KrigedFunction<N>::createFunctionByChangingParametersIntoVariables

  template <unsigned short N>
  std::shared_ptr<ExternalFunction>
  KrigedFunction<N>::createFunctionByChangingParametersIntoVariables(
      std::vector<std::string>& vnames,
      const std::vector<double>&,
      const std::vector<std::string>&,
      const std::map<std::string, std::vector<double>::size_type>&) const {
    vnames.clear();
    return this->resolveDependencies();
  }  // end of
     // KrigedFunction<N>::createFunctionByChangingParametersIntoVariables

  template <unsigned short N>
  KrigedFunction<N>::~KrigedFunction() = default;

}  // end of namespace tfel::math::parser

#endif /* LIB_TFEL_MATH_PARSER_KRIGEDFUNCTION_IXX */
