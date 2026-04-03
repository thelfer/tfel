/*!
 * \file   RoundingMode.hxx
 * \brief
 * \author Thomas Helfer
 * \date   09 mai 2017
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_ROUNDINGMODE_HXX
#define LIB_MTEST_ROUNDINGMODE_HXX

#include <string>
#include "MTest/Config.hxx"

namespace mtest {

  /*!
   * \brief set the rounding mode to the specified mode
   * \param[in] m: rounding mode
   */
  MTEST_VISIBILITY_EXPORT void setRoundingMode(const std::string&);
  /*!
   * \brief set the rounding mode according to currently selected
   * rounding mode.
   *
   * This can be used the reset the rounding mode after calling an
   * external function that may change the rounding, or to select a
   * random rounding mode.
   *
   * \param[in] m: rounding mode
   */
  MTEST_VISIBILITY_EXPORT void setRoundingMode();

}  // end of namespace mtest

#endif /* LIB_MTEST_ROUNDINGMODE_HXX */
