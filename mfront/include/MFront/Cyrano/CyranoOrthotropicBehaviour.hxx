/*!
 * \file   CyranoOrthotropicBehaviour.hxx
 * \brief  This file declares the CyranoOrthotropicBehaviour class
 * \author Helfer Thomas
 * \date   21 fév 2014
 */

#ifndef _LIB_MFRONT_CYRANOORTHOTROPICBEHAVIOUR_H_
#define _LIB_MFRONT_CYRANOORTHOTROPICBEHAVIOUR_H_ 

#include"MFront/Cyrano/CyranoConfig.hxx"
#include"MFront/Cyrano/CyranoTraits.hxx"

namespace cyrano
{

  /*!
   * This behaviour tells which behaviour is used
   * \param[in] Type : behaviour type
   * \param[in] N    : spatial dimension
   */
  struct CyranoOrthotropicOffset
  {
    // 6 elastic coefficients
    //   mass density
    // 3 thermal expansion coefficients
    static const unsigned short value = 10u;
  }; // end of struct CyranoOrthotropicOffset

} // end of namespace cyrano

#endif /* _LIB_MFRONT_CYRANOORTHOTROPICBEHAVIOUR_H */

