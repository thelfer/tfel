/*!
 * \file   mfront/include/MFront/MFrontLogStream.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 10 jun 2011
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_LOGSTREAM_H_
#define LIB_MFRONT_LOGSTREAM_H_

#include <string>
#include <iosfwd>

#include "MFront/MFrontConfig.hxx"

namespace mfront {

  /*!
   * \brief list the possible values for the logging facilities
   *        provided by the getVerboseMode/getLogStream functions
   */
  enum VerboseLevel {
    VERBOSE_QUIET = -1,  //<! no output
    VERBOSE_LEVEL0 = 0,  //<! minimal output
    VERBOSE_LEVEL1 = 1,  //<! a simpler output
    VERBOSE_LEVEL2 = 2,  //<! a much detailled output
    VERBOSE_LEVEL3 = 3,  //<! the finer level for standard user
    VERBOSE_DEBUG = 4,   //<! an output adapted for debugging
    VERBOSE_FULL = 5     //<! a very detailled output
  };                     // end of enum VerboseLevel

  /*!
   * \return the current verbose level
   */
  MFRONTLOGSTREAM_VISIBILITY_EXPORT VerboseLevel& getVerboseMode();

  /*!
   * change the verbose level
   * \param the new verbose level
   */
  MFRONTLOGSTREAM_VISIBILITY_EXPORT void setVerboseMode(
      const VerboseLevel = VERBOSE_LEVEL0);

  /*!
   * \return the current logging stream
   */
  MFRONTLOGSTREAM_VISIBILITY_EXPORT std::ostream& getLogStream();

  /*!
   * set the current logging stream
   * \param f : file name
   */
  MFRONTLOGSTREAM_VISIBILITY_EXPORT void setLogStream(const std::string& f);

  /*!
   * set the current logging stream
   * \param os : new logging stream
   * \warning the stream is not handled by this function.
   * The user has to take care of it
   */
  MFRONTLOGSTREAM_VISIBILITY_EXPORT void setLogStream(std::ostream&);

}  // end of namespace mfront

#endif /* LIB_MFRONT_LOGSTREAM_H_ */
