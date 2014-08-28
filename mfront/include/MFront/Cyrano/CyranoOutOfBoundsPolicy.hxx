/*! 
 * \file  mfront/include/MFront/Cyrano/CyranoOutOfBoundsPolicy.hxx
 * \brief
 * \author Helfer Thomas
 * \date   21 fév 2014
 */

#ifndef _LIB_TFEL_MFRONT_CYRANOOUTOFBOUNDSPOLICY_H_
#define _LIB_TFEL_MFRONT_CYRANOOUTOFBOUNDSPOLICY_H_ 

#include"MFront/Cyrano/CyranoConfig.hxx"
#include"TFEL/Material/OutOfBoundsPolicy.hxx"

namespace cyrano
{

  struct MFRONT_CYRANO_VISIBILITY_EXPORT CyranoOutOfBoundsPolicy
  {
    static const CyranoOutOfBoundsPolicy&
    getCyranoOutOfBoundsPolicy();
    tfel::material::OutOfBoundsPolicy
    getOutOfBoundsPolicy() const;
  private:
    CyranoOutOfBoundsPolicy();
    CyranoOutOfBoundsPolicy(const CyranoOutOfBoundsPolicy&);
    CyranoOutOfBoundsPolicy&
    operator=(const CyranoOutOfBoundsPolicy&);
    tfel::material::OutOfBoundsPolicy policy;
  }; // end of struct CyranoOutOfBounds

} // end of namespace cyrano

#endif /* _LIB_TFEL_MFRONT_CYRANOOUTOFBOUNDS_H */

