/*! 
 * \file   mfront/include/MFront/PedanticMode.hxx
 * \brief
 * \author Helfer Thomas
 * \brief  20/05/2014
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MFRONT_MFRONTPEDANTICMODE_H_
#define LIB_MFRONT_MFRONTPEDANTICMODE_H_ 

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

#endif /* LIB_MFRONT_MFRONTPEDANTICMODE_H_ */

