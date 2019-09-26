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

  template <typename T>
  const T& get(const TestCaseParameters& parameters, const std::string& n) {
    const auto& p = getParameter(parameters, n);
    if (!p.is<T>()) {
      throwInvalidParameterTypeException(n);
    }
    return p.get<T>();
  }  // end of get

}  // end of namespace mfmtg

#endif /* LIB_MFM_TEST_GENERATOR_TESTCASEPARAMETERS_IXX */
