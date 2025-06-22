/*!
 * \file   src/NUMODIS/IDirection.cxx
 * \brief
 * \author Laurent Dupuy
 * \date   9/06/2017
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <ostream>
#include <numeric>
#include <vector>

#include "NUMODIS/Math/Utilities.hxx"
#include "NUMODIS/IDirection.hxx"

namespace numodis {

  //===============================================================
  // IDirection :: Operator ==
  //---------------------------------------------------------------
  //! Determines whether two directions are identical or not
  //---------------------------------------------------------------
  /*!
    This routine returns:
    - true if the glide planes are identical
    - false if the glide planes are different

    \return true or false
  */
  //===============================================================
  bool operator==(const IDirection& lhs, const IDirection& rhs) {
    //-------------------------------------------
    // dot products (still valid with 4 indices)
    //-------------------------------------------
    int b1b2 = numodis::math::iScalProduct(lhs.getIndex(), rhs.getIndex());
    int b1b1 = numodis::math::iScalProduct(lhs.getIndex(), lhs.getIndex());
    int b2b2 = numodis::math::iScalProduct(rhs.getIndex(), rhs.getIndex());

    //-------------------------
    // compare scalar products
    //-------------------------
    return (b1b1 != 0 && b2b2 != 0 ? b1b1 * b2b2 == b1b2 * b1b2 : b1b1 == b2b2);
  }

  //===============================================================
  // IDirection :: Operator !=
  //---------------------------------------------------------------
  //! Determines whether two glide planes are different or not
  //---------------------------------------------------------------
  /*!
    This routine returns:
    false if the glide planes are identical
    true if the glide planes are different

    \return true or false
  */
  //===============================================================
  bool operator!=(const IDirection& lhs, const IDirection& rhs) {
    return !(lhs == rhs);
  }

}  // end of namespace numodis
