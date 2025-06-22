/*!
 * \file  src/Material/DDIF2Base.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 25 nov 2010
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "TFEL/Material/DDIF2Base.hxx"

namespace tfel {

  namespace material {

#ifdef _MSC_VER
    const double DDIF2Base::emin1 = 1.e-7;
    const double DDIF2Base::emin_rk1 = 1.e-5;
    const double DDIF2Base::eps = 1.e-10;
#endif
  }  // end of namespace material

}  // end of namespace tfel
