/*!
 * \file   TestCaseParameters.cxx
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

#include "TFEL/Raise.hxx"
#include "MFMTestGenerator/TestCaseParameters.hxx"

namespace mfmtg{

  const TestCaseParameter& getParameter(const TestCaseParameters& parameters,
                                        const std::string& n) {
    const auto p = parameters.find(n);
    if (p == parameters.end()) {
      tfel::raise("getParameter: no parameter named '" + n + "'");
    }
    return p->second;
  }  // end of getParameter

  const TestCaseParameters& getTestCaseParameters(
      const TestCaseParameters& parameters, const std::string& n) {
    const auto& p = getParameter(parameters, n);
    if (!p.is<TestCaseParameters>()) {
      tfel::raise("getTestCaseParameters: parameter '" + n +
                  "' has not the expected type");
    }
    return p.get<TestCaseParameters>();
  }  // end of getTestCaseParameters

  void throwInvalidParameterTypeException(const std::string& n) {
    tfel::raise("get_parameter: unexpected type for parameter '" + n + "'");
  }  // end of throwInvalidParameterTypeException

} // end of namespace mfmtg