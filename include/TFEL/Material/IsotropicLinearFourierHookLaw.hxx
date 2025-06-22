/*!
 * \file   include/TFEL/Material/IsotropicLinearFourierHookLaw.hxx
 * \brief  This file declares the IsotropicLinearFourierHookLaw class
 * \author Thomas Helfer
 * \date   27 Sep 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_ISOTROPICLINEARFOURIERHOOKLAW_HXX
#define LIB_TFEL_ISOTROPICLINEARFOURIERHOOKLAW_HXX

#include "TFEL/Material/IsotropicLinearFourierLaw.hxx"
#include "TFEL/Material/IsotropicLinearHookLaw.hxx"

namespace tfel {

  namespace material {

    /*!
     * \class IsotropicLinearFourierHookLaw
     * \brief This class implements the hook law.
     * \param N, space dimension.
     * \param T, numerical type.
     * \author Thomas Helfer
     * \date   28 Jul 2006
     */
    template <unsigned short N, typename T, bool use_qt>
    class IsotropicLinearFourierHookLaw
        : public IsotropicLinearFourierLaw<N, T, use_qt>,
          public IsotropicLinearHookLaw<N, T, use_qt> {};

  }  // end of namespace material

}  // end of namespace  tfel

#endif /* LIB_TFEL_ISOTROPICLINEARFOURIERHOOKLAW_HXX */
