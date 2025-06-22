/*!
 * \file  mfront/include/MFront/MFrontDebugMode.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 11 avril 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONTDEBUGMODE_HXX
#define LIB_MFRONTDEBUGMODE_HXX

#include "MFront/MFrontConfig.hxx"

namespace mfront {

  /*!
   * \return true if the debug mode is enabled
   */
  MFRONT_VISIBILITY_EXPORT bool getDebugMode();

  /*!
   * \brief enable or diable the debug mode
   * \param[in] b : if true, set the debug mode
   */
  MFRONT_VISIBILITY_EXPORT void setDebugMode(const bool);

}  // namespace mfront

#endif /* LIB_MFRONTDEBUGMODE_HXX */
