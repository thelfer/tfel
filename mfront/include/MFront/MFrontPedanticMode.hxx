/*! 
 * \file   MFrontPedanticMode.hxx
 * \brief
 * \author Helfer Thomas
 * \brief  20/05/2014
 */

#ifndef _LIB_MFRONT_MFRONTPEDANTICMODE_H_
#define _LIB_MFRONT_MFRONTPEDANTICMODE_H_ 

#include"TFEL/Config/TFELConfig.hxx"

namespace mfront{

  /*!
   * \return true if the pedantic mode is enabled
   */
  TFEL_VISIBILITY_EXPORT bool getPedanticMode();

  /*!
   * \brief enable or diable the pedantic mode
   * \param[in] b : if true, set the pedantic mode
   */
  TFEL_VISIBILITY_EXPORT void setPedanticMode(const bool);

}

#endif /* _LIB_MFRONT_MFRONTPEDANTICMODE_H */

