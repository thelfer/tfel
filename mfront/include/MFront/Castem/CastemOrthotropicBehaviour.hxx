/*!
 * \file   mfront/include/MFront/Castem/CastemOrthotropicBehaviour.hxx
 * \brief  This file declares the CastemOrthotropicBehaviour class
 * \author Helfer Thomas
 * \date   18 mar 2008
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MFRONT_CASTEMORTHOTROPICBEHAVIOUR_H_
#define LIB_MFRONT_CASTEMORTHOTROPICBEHAVIOUR_H_ 

#include"MFront/Castem/CastemConfig.hxx"
#include"MFront/Castem/CastemTraits.hxx"

#ifndef _MSC_VER
#define CastemORTHOTROPICBEHAVIOUR_CONSTEXPR constexpr
#else
#define CastemORTHOTROPICBEHAVIOUR_CONSTEXPR const
#endif

namespace castem
{

  /*!
   * This behaviour tells which behaviour is used
   * \param[in] Type : behaviour type
   * \param[in] N    : spatial dimension
   */
  template<CastemBehaviourType Type,
	   unsigned short N>
  struct CastemOrthotropicOffset;

  template<>
  struct MFRONT_CASTEM_VISIBILITY_EXPORT
  CastemOrthotropicOffset<SMALLSTRAINSTANDARDBEHAVIOUR,1u>
  {
    // 6 elastic coefficients
    //   mass density
    // 3 thermal expansion coefficients
    static CastemORTHOTROPICBEHAVIOUR_CONSTEXPR unsigned short value = 10u;
  }; // end of struct CastemOrthotropicOffset<SMALLSTRAINSTANDARDBEHAVIOUR,1u>

  template<>
  struct MFRONT_CASTEM_VISIBILITY_EXPORT
  CastemOrthotropicOffset<SMALLSTRAINSTANDARDBEHAVIOUR,2u>
  {
    // 7 elastic coefficients
    // 2 components of direction
    //   mass density
    // 3 thermal expansion coefficients
    static CastemORTHOTROPICBEHAVIOUR_CONSTEXPR unsigned short value = 13u;
  }; // end of struct CastemOrthotropicOffset<SMALLSTRAINSTANDARDBEHAVIOUR,2u>

  template<>
  struct MFRONT_CASTEM_VISIBILITY_EXPORT
  CastemOrthotropicOffset<SMALLSTRAINSTANDARDBEHAVIOUR,3u>
  {
    // 9 elastic coefficients
    // 6 components of direction
    //   mass density
    // 3 thermal expansion coefficients
    static CastemORTHOTROPICBEHAVIOUR_CONSTEXPR unsigned short value = 19u;
  }; // end of struct CastemOrthotropicOffset<SMALLSTRAINSTANDARDBEHAVIOUR,3u>

} // end of namespace castem

#endif /* LIB_MFRONT_CASTEMORTHOTROPICBEHAVIOUR_H_ */

