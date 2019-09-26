/*!
 * \file   TestCaseParameters.hxx
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

#ifndef LIB_MFM_TEST_GENERATOR_TESTCASEPARAMETERS_HXX
#define LIB_MFM_TEST_GENERATOR_TESTCASEPARAMETERS_HXX

#include <string>
#include "TFEL/Utilities/Data.hxx"
#include "MFMTestGenerator/Config.hxx"

namespace mfmtg {

  //! \brief a simple alias
  using TestCaseParameter = tfel::utilities::Data;

  //! \brief a structure holding all the parameters of a test case.
  using TestCaseParameters = std::map<std::string, TestCaseParameter>;

  /*!
   * \return the parameter with the given name
   * \param[in] p: parameters
   * \param[in] n: name of the parameter to be retrieved.
   */
  template <typename T>
  const T& get(const TestCaseParameters&, const std::string&);

  /*!
   * \return the parameter with the given name
   * \param[in] p: parameters
   * \param[in] n: name of the parameter to be retrieved.
   */
  MFMTG_VISIBILITY_EXPORT const TestCaseParameter& getParameter(
      const TestCaseParameters&, const std::string&);

  /*!
   * \throw an exception reporting that the type of the parameter with the given
   * name is not the one that what was requested.
   * \param[in] n: name of the missing parameter
   */
  TFEL_NORETURN MFMTG_VISIBILITY_EXPORT void
  throwInvalidParameterTypeException(const std::string&);

}  // end of namespace mfmtg

#include "MFMTestGenerator/TestCaseParameters.ixx"

#endif /* LIB_MFM_TEST_GENERATOR_TESTCASEPARAMETERS_HXX */
