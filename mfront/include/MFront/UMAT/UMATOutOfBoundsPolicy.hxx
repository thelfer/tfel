/*! 
 * \file  mfront/include/MFront/UMAT/UMATOutOfBoundsPolicy.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 11 jan 2010
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_MFRONT_UMATOUTOFBOUNDSPOLICY_H_
#define _LIB_TFEL_MFRONT_UMATOUTOFBOUNDSPOLICY_H_ 

#include"MFront/UMAT/UMATConfig.hxx"
#include"TFEL/Material/OutOfBoundsPolicy.hxx"

namespace umat
{

  struct MFRONT_UMAT_VISIBILITY_EXPORT UMATOutOfBoundsPolicy
  {
    static const UMATOutOfBoundsPolicy&
    getUMATOutOfBoundsPolicy();
    tfel::material::OutOfBoundsPolicy
    getOutOfBoundsPolicy() const;
  private:
    UMATOutOfBoundsPolicy();
    UMATOutOfBoundsPolicy(const UMATOutOfBoundsPolicy&);
    UMATOutOfBoundsPolicy&
    operator=(const UMATOutOfBoundsPolicy&);
    tfel::material::OutOfBoundsPolicy policy;
  }; // end of struct UMATOutOfBounds

} // end of namespace umat

#endif /* _LIB_TFEL_MFRONT_UMATOUTOFBOUNDS_H */

