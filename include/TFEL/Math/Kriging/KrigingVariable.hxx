/*!
 * \file  include/TFEL/Math/Kriging/KrigingVariable.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 11 avr 2009
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_KRIGINGVARIABLES_HXX
#define LIB_TFEL_MATH_KRIGINGVARIABLES_HXX

#include "TFEL/Math/tvector.hxx"

namespace tfel::math {

  template <unsigned short N, typename T>
  struct KrigingVariable {
    typedef tvector<N, T> type;
  };  // end of KrigingVariable<N,T>

  template <typename T>
  struct KrigingVariable<1u, T> {
    typedef T type;
  };  // end of KrigingVariable<1u,T>

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_KRIGINGVARIABLES_HXX */
