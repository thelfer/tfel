/*!
 * \file  include/TFEL/Material/OutOfBoundsPolicy.hxx
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

#ifndef LIB_TFEL_MATERIAL_OUTOFBOUNDSPOLICY_HXX
#define LIB_TFEL_MATERIAL_OUTOFBOUNDSPOLICY_HXX

namespace tfel {

  namespace material {

    enum OutOfBoundsPolicy {
      Warning,
      Strict,
      None
    };  // end of enum OutOfBoundsPolicy

  }  // end of namespace material

}  // end of namespace tfel

#endif /* LIB_TFEL_MATERIAL_OUTOFBOUNDSPOLICY_HXX */
