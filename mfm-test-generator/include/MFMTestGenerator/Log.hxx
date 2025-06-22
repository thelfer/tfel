/*!
 * \file   mfm-test-generator/include/MFMTestGenerator/Log.hxx
 * \brief  This file introduces some wrapper around functions defined in the
 *         `MFrontLogStream` library.
 * \author Thomas Helfer
 * \date   26/09/2019
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFMTESTGENERATOR_LOG_HXX
#define LIB_MFMTESTGENERATOR_LOG_HXX

#include <string>
#include "MFMTestGenerator/Config.hxx"

namespace mfmtg {

  /*!
   * \brief print a message if the level of verbosity is greater or equal to
   * `VERBOSE_DEBUG`.
   * \param[in] msg: message
   * \note an end of line character is automatically appended to the message
   */
  MFMTG_VISIBILITY_EXPORT void debug(const std::string&);
  /*!
   * \brief print a message if the level of verbosity is greater or equal to
   * `VERBOSE_LEVEL2`.
   * \param[in] msg: message
   * \note an end of line character is automatically appended to the message
   */
  MFMTG_VISIBILITY_EXPORT void message(const std::string&);

}  // end of namespace mfmtg

#endif /* LIB_MFMTESTGENERATOR_LOG_HXX */
