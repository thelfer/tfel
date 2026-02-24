/*!
 * \file   mfront/include/MFront/PedanticMode.hxx
 * \brief
 * \author Thomas Helfer
 * \brief  20/05/2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_MFRONTPEDANTICMODE_HXX
#define LIB_MFRONT_MFRONTPEDANTICMODE_HXX

#include "MFront/MFrontConfig.hxx"

namespace mfront {

  /*!
   * \return true if the pedantic mode is enabled
   */
  MFRONT_VISIBILITY_EXPORT bool getPedanticMode();

  /*!
   * \brief enable or diable the pedantic mode
   * \param[in] b : if true, set the pedantic mode
   */
  MFRONT_VISIBILITY_EXPORT void setPedanticMode(const bool);

}  // namespace mfront

#endif /* LIB_MFRONT_MFRONTPEDANTICMODE_HXX */
