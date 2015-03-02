/*!
 * \file   mfront/include/MFront/UMAT/UMATOrthotropicBehaviour.hxx
 * \brief  This file declares the UMATOrthotropicBehaviour class
 * \author Helfer Thomas
 * \date   18 mar 2008
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MFRONT_UMATORTHOTROPICBEHAVIOUR_H_
#define LIB_MFRONT_UMATORTHOTROPICBEHAVIOUR_H_ 

#include"MFront/UMAT/UMATConfig.hxx"
#include"MFront/UMAT/UMATTraits.hxx"

namespace umat
{

  /*!
   * This behaviour tells which behaviour is used
   * \param[in] Type : behaviour type
   * \param[in] N    : spatial dimension
   */
  template<UMATBehaviourType Type,
	   unsigned short N>
  struct UMATOrthotropicOffset;

  template<>
  struct MFRONT_UMAT_VISIBILITY_EXPORT
  UMATOrthotropicOffset<SMALLSTRAINSTANDARDBEHAVIOUR,1u>
  {
    // 6 elastic coefficients
    //   mass density
    // 3 thermal expansion coefficients
    static constexpr unsigned short value = 10u;
  }; // end of struct UMATOrthotropicOffset<SMALLSTRAINSTANDARDBEHAVIOUR,1u>

  template<>
  struct MFRONT_UMAT_VISIBILITY_EXPORT
  UMATOrthotropicOffset<SMALLSTRAINSTANDARDBEHAVIOUR,2u>
  {
    // 7 elastic coefficients
    // 2 components of direction
    //   mass density
    // 3 thermal expansion coefficients
    static constexpr unsigned short value = 13u;
  }; // end of struct UMATOrthotropicOffset<SMALLSTRAINSTANDARDBEHAVIOUR,2u>

  template<>
  struct MFRONT_UMAT_VISIBILITY_EXPORT
  UMATOrthotropicOffset<SMALLSTRAINSTANDARDBEHAVIOUR,3u>
  {
    // 9 elastic coefficients
    // 6 components of direction
    //   mass density
    // 3 thermal expansion coefficients
    static constexpr unsigned short value = 19u;
  }; // end of struct UMATOrthotropicOffset<SMALLSTRAINSTANDARDBEHAVIOUR,3u>

} // end of namespace umat

#endif /* LIB_MFRONT_UMATORTHOTROPICBEHAVIOUR_H_ */

