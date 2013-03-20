/*! 
 * \file  AsterOrthotropicBehaviour.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 20 mars 2013
 */

#ifndef _LIB_MFRONT_ASTERORTHOTROPICBEHAVIOUR_H_
#define _LIB_MFRONT_ASTERORTHOTROPICBEHAVIOUR_H_ 

#include"MFront/Aster/AsterConfig.hxx"

namespace aster
{

  template<unsigned short>
  struct AsterOrthotropicOffset;

  template<>
  struct MFRONT_ASTER_VISIBILITY_EXPORT AsterOrthotropicOffset<1u>
  {
    // 6 elastic coefficients
    //   mass density
    // 3 thermal expansion coefficients
    static const unsigned short value = 10u;
  }; // end of struct AsterOrthotropicOffset<1u>

  template<>
  struct MFRONT_ASTER_VISIBILITY_EXPORT AsterOrthotropicOffset<2u>
  {
    // 7 elastic coefficients
    //   mass density
    // 3 thermal expansion coefficients
    static const unsigned short value = 11u;
  }; // end of struct AsterOrthotropicOffset<2u>

  template<>
  struct MFRONT_ASTER_VISIBILITY_EXPORT AsterOrthotropicOffset<3u>
  {
    // 9 elastic coefficients
    //   mass density
    // 3 thermal expansion coefficients
    static const unsigned short value = 13u;
  }; // end of struct AsterOrthotropicOffset<3u>

} // end of namespace aster

#endif /* _LIB_ASTERORTHOTROPICBEHAVIOUR_H */
