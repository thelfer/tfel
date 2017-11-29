/*! 
 * \file  mfront/include/MFront/MFrontDebugMode.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 11 avril 2014
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MFRONTDEBUGMODE_H_
#define LIB_MFRONTDEBUGMODE_H_ 

#include"MFront/MFrontConfig.hxx"

namespace mfront{

  /*!
   * \return true if the debug mode is enabled
   */
  MFRONT_VISIBILITY_EXPORT bool getDebugMode();

  /*!
   * \brief enable or diable the debug mode
   * \param[in] b : if true, set the debug mode
   */
  MFRONT_VISIBILITY_EXPORT void setDebugMode(const bool);

}

#endif /* LIB_MFRONTDEBUGMODE_H_ */

