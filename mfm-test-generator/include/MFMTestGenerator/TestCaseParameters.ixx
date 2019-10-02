/*!
 * \file   TestCaseParameters.ixx
 * \brief
 * \author Thomas Helfer
 * \date   17/09/2019
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFM_TEST_GENERATOR_TESTCASEPARAMETERS_IXX
#define LIB_MFM_TEST_GENERATOR_TESTCASEPARAMETERS_IXX

namespace mfmtg {

  template <typename... String>
  bool contains(const TestCaseParameters& parameters,
                const std::string& n,
                const String&... args) {
    const auto& p = getTestCaseParameters(parameters, n);
    return contains(p, args...);
  }  // end of get

  template <typename T, typename... String>
  const T& get(const TestCaseParameters& parameters,
               const std::string& n,
               const String&... args) {
    const auto& p = getTestCaseParameters(parameters, n);
    return get<T>(p, args...);
  }  // end of get

  template <typename T>
  const T& get(const TestCaseParameters& parameters, const std::string& n) {
    const auto& p = getParameter(parameters, n);
    if (!p.is<T>()) {
      throwInvalidParameterTypeException(n);
    }
    return p.get<T>();
  }  // end of get

  template <typename T, typename T2, typename... String>
  T get_if(const TestCaseParameters& parameters,
           const std::string& n,
           const String&... args,
           const T2& v) {
    const auto& p = getTestCaseParameters(parameters, n);
    return get_if<T>(p, args..., v);
  }  // end of get_if

  template <typename T, typename T2>
  T get_if(const TestCaseParameters& parameters,
           const std::string& n,
           const T2& v) {
    if (!contains(parameters, n)) {
      return T(v);
    }
    return get<T>(parameters, n);
  }  // end of get_if

  template <typename... String>
  const TestCaseParameter& getParameter(const TestCaseParameters& parameters,
                                        const std::string& n,
                                        const String&... args) {
    const auto& p = getTestCaseParameters(parameters, n);
    return getParameter(p, args...);
  }  // end of getParameter

  template <typename... String>
  Evolution getEvolution(const TestCaseParameters& parameters,
                         const std::string& n,
                         const String&... args) {
    const auto& p = getTestCaseParameters(parameters, n);
    return getEvolution(p, args...);
  }  // end of getEvolution

  template <typename... String>
  std::map<std::string, Evolution> getEvolutions(
      const TestCaseParameters& parameters,
      const std::string& n,
      const String&... args) {
    const auto& p = getTestCaseParameters(parameters, n);
    return getEvolutions(p, args...);
  }  // end of getEvolution

}  // end of namespace mfmtg

#endif /* LIB_MFM_TEST_GENERATOR_TESTCASEPARAMETERS_IXX */
