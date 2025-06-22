/*!
 * \file  mfront/include/MFront/PerformanceProfiling.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 22 août 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONTPERFORMANCEPROFILING_H_
#define LIB_MFRONTPERFORMANCEPROFILING_H_

#include <iosfwd>
#include <string>

namespace mfront {

  /*!
   * \brief : an helper function to declare a timer
   * \param[out] os : output stream
   * \param[in]  c  : class name
   * \param[in]  v  : code block name
   * \param[in]  s  : time temporary variable suffix
   */
  void writeStandardPerformanceProfiling(std::ostream&,
                                         const std::string&,
                                         const std::string&,
                                         const std::string& = "");
  /*!
   * \brief : an helper function writing the beginning of a
   * performance measurement
   * \param[out] os : output stream
   * \param[in]  c  : class name
   * \param[in]  v  : code block name
   * \param[in]  s  : time temporary variable suffix
   */
  void writeStandardPerformanceProfilingBegin(std::ostream&,
                                              const std::string&,
                                              const std::string&,
                                              const std::string& = "");
  /*!
   * \brief : an helper function writing the end of a
   * performance measurement
   * \param[out] os : output stream
   */
  void writeStandardPerformanceProfilingEnd(std::ostream&);

}  // end of namespace mfront

#endif /* LIB_MFRONTPERFORMANCEPROFILING_H_ */
