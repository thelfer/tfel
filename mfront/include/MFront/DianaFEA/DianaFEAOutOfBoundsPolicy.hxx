/*!
 * \file  mfront/include/MFront/DianaFEA/DianaFEAOutOfBoundsPolicy.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 11 jan 2010
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MFRONT_DIANAFEAOUTOFBOUNDSPOLICY_HXX
#define LIB_TFEL_MFRONT_DIANAFEAOUTOFBOUNDSPOLICY_HXX

#include "MFront/DianaFEA/DianaFEAConfig.hxx"
#include "TFEL/Material/OutOfBoundsPolicy.hxx"

namespace dianafea {

  struct MFRONT_DIANAFEA_VISIBILITY_EXPORT DianaFEAOutOfBoundsPolicy {
    static const DianaFEAOutOfBoundsPolicy& getDianaFEAOutOfBoundsPolicy();
    tfel::material::OutOfBoundsPolicy getOutOfBoundsPolicy() const;

   private:
    DianaFEAOutOfBoundsPolicy();
    DianaFEAOutOfBoundsPolicy(const DianaFEAOutOfBoundsPolicy&);
    DianaFEAOutOfBoundsPolicy& operator=(const DianaFEAOutOfBoundsPolicy&);
    tfel::material::OutOfBoundsPolicy policy;
  };  // end of struct DianaFEAOutOfBounds

}  // end of namespace dianafea

#endif /* LIB_TFEL_MFRONT_DIANAFEAOUTOFBOUNDSPOLICY_HXX */
