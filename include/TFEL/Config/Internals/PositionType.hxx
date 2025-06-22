/*!
 * \file   include/TFEL/Config/Internals/PositionType.hxx
 * \brief  This file declares the  class
 * \author Helfer Thomas
 * \date   25 Sep 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_POSITIONTYPE_H_
#define LIB_TFEL_POSITIONTYPE_H_

#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/Forward/qt.hxx"
#include "TFEL/Math/Quantity/Unit.hxx"

namespace tfel {

  namespace config {

    namespace internals {

      template <unsigned short N, typename T, bool use_qt>
      struct PositionType {
        typedef tfel::math::tvector<N, tfel::math::qt<tfel::math::Length, T>>
            type;
      };

      template <unsigned short N, typename T>
      struct PositionType<N, T, false> {
        typedef tfel::math::tvector<N, T> type;
      };

      template <typename T, bool use_qt>
      struct PositionType<1u, T, use_qt> {
        typedef tfel::math::qt<tfel::math::Length,
                               tfel::math::qt<tfel::math::NoUnit, T>>
            type;
      };

      template <typename T>
      struct PositionType<1u, T, false> {
        typedef T type;
      };

    }  // end of namespace internals

  }  // end of namespace config

}  // end of namespace tfel

#endif /* LIB_TFEL_POSITIONTYPE_H_ */
