/*! 
 * \file  MFrontDebugMode.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 11 avril 2014
 */

#ifndef _LIB_MFRONTDEBUGMODE_H_
#define _LIB_MFRONTDEBUGMODE_H_ 

#include"TFEL/Config/TFELConfig.hxx"

namespace mfront{

  /*!
   * \return true if the debug mode is enabled
   */
  TFEL_VISIBILITY_EXPORT bool getDebugMode();

  /*!
   * \brief enable or diable the debug mode
   * \param[in] b : if true, set the debug mode
   */
  TFEL_VISIBILITY_EXPORT void setDebugMode(const bool);

}

#endif /* _LIB_MFRONTDEBUGMODE_H */

