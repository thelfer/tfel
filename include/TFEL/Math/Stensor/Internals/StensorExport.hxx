/*!
 * \file   include/TFEL/Math/Stensor/Internals/StensorExport.hxx
 *
 * \brief
 * \author Helfer Thomas
 * \date   31 Jul 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_STENSOREXPORT_H_
#define LIB_TFEL_STENSOREXPORT_H_

namespace tfel {

  namespace math {

    namespace internals {

      template <unsigned int N>
      struct ExportToTab;

      template <unsigned int N>
      struct ExportToVoigt;

    }  // end of namespace internals

  }  // end of namespace math

}  // end of namespace tfel

#include "TFEL/Math/Stensor/Internals/StensorExport.ixx"

#endif /* LIB_TFEL_STENSOREXPORT_H_ */
