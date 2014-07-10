/*! 
 * \file  UMATOutOfBoundsPolicy.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 11 jan 2010
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

