/*! 
 * \file  MFrontPerformanceProfiling.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 22 août 2014
 */

#ifndef _LIB_MFRONTPERFORMANCEPROFILING_H_
#define _LIB_MFRONTPERFORMANCEPROFILING_H_ 

#include<iosfwd>
#include<string>

namespace mfront{

  /*!
   * \brief : an helper function to declare a timer
   * \param[out] os : output stream
   * \param[in]  c  : class name
   * \param[in]  v  : code block name
   * \param[in]  s  : time temporary variable suffix
   */
  void
  writeStandardPerformanceProfiling(std::ostream&,
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
  void
  writeStandardPerformanceProfilingBegin(std::ostream&,
					 const std::string&,
					 const std::string&,
					 const std::string& = "");
  /*!
   * \brief : an helper function writing the end of a
   * performance measurement
   * \param[out] os : output stream
   */
  void
  writeStandardPerformanceProfilingEnd(std::ostream&);

} // end of namespace mfront

#endif /* _LIB_MFRONTPERFORMANCEPROFILING_H */
