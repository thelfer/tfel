/*!
 * \file   TestCaseParameters.hxx
 * \brief
 * \author Thomas Helfer
 * \date   17/09/2019
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFM_TEST_GENERATOR_TESTCASEPARAMETERS_HXX
#define LIB_MFM_TEST_GENERATOR_TESTCASEPARAMETERS_HXX

#include <map>
#include <string>
#include "TFEL/Utilities/Data.hxx"
#include "MFMTestGenerator/Config.hxx"
#include "MFMTestGenerator/Forward.hxx"
#include "MFMTestGenerator/Evolution.hxx"

namespace mfmtg {

  /*!
   * \return true if a parameter with the given name exists
   * \param[in] p: parameters
   * \param[in] n: name of the parameter.
   */
  MFMTG_VISIBILITY_EXPORT bool contains(const TestCaseParameters&,
                                        const std::string&);
  /*!
   * \return true if a parameter with the given name exists
   * \param[in] p: parameters
   * \param[in] n: name of the parameter.
   */
  MFMTG_VISIBILITY_EXPORT bool contains(const TestCaseParameters&,
                                        const char* const);
  /*!
   * \return true if a parameter associated the given identifiers exists.
   * \param[in] p: parameters
   * \param[in] n: first level identitifer.
   * \param[in] args: higher level identifiers.
   */
  template <typename StringType, typename... StringTypes>
  bool contains(const TestCaseParameters&,
                const StringType&,
                const StringTypes&...);
  /*!
   * \return the parameter with the given name
   * \param[in] p: parameters
   * \param[in] n: first level identitifer.
   * \param[in] args: higher level identifiers.
   */
  template <typename StringType, typename... StringTypes>
  const TestCaseParameter& getParameter(const TestCaseParameters&,
                                        const StringType&,
                                        const StringTypes&...);
  /*!
   * \return the parameter with the given name
   * \param[in] p: parameters
   * \param[in] n: name of the parameter to be retrieved.
   */
  MFMTG_VISIBILITY_EXPORT const TestCaseParameter& getParameter(
      const TestCaseParameters&, const std::string&);
  /*!
   * \return the parameter with the given name
   * \param[in] p: parameters
   * \param[in] n: name of the parameter to be retrieved.
   */
  MFMTG_VISIBILITY_EXPORT const TestCaseParameter& getParameter(
      const TestCaseParameters&, const char* const);
  /*!
   * \return the parameter with the given name
   * \param[in] p: parameters
   * \param[in] n: name of the parameter to be retrieved.
   */
  MFMTG_VISIBILITY_EXPORT const TestCaseParameters& getTestCaseParameters(
      const TestCaseParameters&, const std::string&);
  /*!
   * \return the parameter with the given name
   * \param[in] p: parameters
   * \param[in] n: name of the parameter to be retrieved.
   */
  MFMTG_VISIBILITY_EXPORT const TestCaseParameters& getTestCaseParameters(
      const TestCaseParameters&, const char* const);
  /*!
   * \return the parameter with the given identifiers
   * \param[in] p: parameters
   * \param[in] n: first level identitifer.
   * \param[in] args: higher level identifiers.
   */
  template <typename T = std::string,
            typename StringType,
            typename... StringTypes>
  const T& get(const TestCaseParameters& parameters,
               const StringType& n,
               const StringTypes&... args);
  /*!
   * \return the parameter with the given name
   * \param[in] p: parameters
   * \param[in] n: name of the parameter to be retrieved.
   */
  template <typename T = std::string, typename StringType>
  const T& get(const TestCaseParameters&, const StringType&);
  /*!
   * \return the parameter with the given name if it exits a default value
   * otherwise.
   * \param[in] p: parameters
   * \param[in] n: name of the parameter to be retrieved.
   * \param[in] v: default value.
   */
  template <typename T = std::string, typename StringType, typename T2>
  T get_if(const TestCaseParameters&, const StringType&, const T2&);
  /*!
   * \return the evolution stored at the given address
   * \param[in] p: parameters
   * \param[in] n: first level identitifer.
   * \param[in] args: higher level identifiers.
   */
  template <typename StringType, typename... StringTypes>
  Evolution getEvolution(const TestCaseParameters&,
                         const StringType&,
                         const StringTypes&...);
  /*!
   * \return the evolution stored at the given address
   * \param[in] p: parameters
   * \param[in] n: address of the to be retrieved.
   */
  MFMTG_VISIBILITY_EXPORT Evolution getEvolution(const TestCaseParameters&,
                                                 const std::string&);
  /*!
   * \return the evolution stored at the given address
   * \param[in] p: parameters
   * \param[in] n: address of the to be retrieved.
   */
  MFMTG_VISIBILITY_EXPORT Evolution getEvolution(const TestCaseParameters&,
                                                 const char* const);
  /*!
   * \return the evolutions stored at the given address
   * \param[in] p: parameters
   * \param[in] n: first level identitifer.
   * \param[in] args: higher level identifiers.
   * \param[in] b: boolean value. If true and that if the last identifier does
   * not exist, an empty evolution is returned.
   */
  template <typename StringType, typename... StringTypes>
  std::map<std::string, Evolution> getEvolutions(const TestCaseParameters&,
                                                 const StringType&,
                                                 const StringTypes&...,
                                                 const bool = true);
  /*!
   * \return the evolutions stored at the given address
   * \param[in] p: parameters
   * \param[in] n: address of the to be retrieved.
   * \param[in] b: boolean value. If true and that if the last identifier does
   * not exist, an empty evolution is returned.
   */
  MFMTG_VISIBILITY_EXPORT std::map<std::string, Evolution> getEvolutions(
      const TestCaseParameters&, const std::string&, const bool = true);
  /*!
   * \return the evolutions stored at the given address
   * \param[in] p: parameters
   * \param[in] n: address of the to be retrieved.
   * \param[in] b: boolean value. If true and that if the last identifier does
   * not exist, an empty evolution is returned.
   */
  MFMTG_VISIBILITY_EXPORT std::map<std::string, Evolution> getEvolutions(
      const TestCaseParameters&, const char* const, const bool = true);
  /*!
   * \brief check that the parameters' names are part of the one given list.
   * \param m: parameters.
   * \param k: allowed names.
   */
  MFMTG_VISIBILITY_EXPORT void check(const TestCaseParameters&,
                                     const std::vector<std::string>&);
  /*!
   * \throw an exception reporting that the type of the parameter with the
   * given
   * name is not the one that what was requested.
   * \param[in] n: name of the missing parameter
   */
  [[noreturn]] MFMTG_VISIBILITY_EXPORT void throwInvalidParameterTypeException(
      const std::string&);

}  // end of namespace mfmtg

#include "MFMTestGenerator/TestCaseParameters.ixx"

#endif /* LIB_MFM_TEST_GENERATOR_TESTCASEPARAMETERS_HXX */
