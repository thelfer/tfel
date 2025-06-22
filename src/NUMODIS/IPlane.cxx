/*!
 * \file   src/NUMODIS/IPlane.cxx
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

#include <vector>
#include <numeric>
#include <ostream>

#include "NUMODIS/IPlane.hxx"

namespace numodis {

  IPlane::IPlane(IPlane&&) = default;

  IPlane::IPlane(const IPlane&) = default;

  //===============================================================
  // IPlane : Operator <<
  //---------------------------------------------------------------
  //! Print the data of a IPlane vector
  //---------------------------------------------------------------
  /*!
    \param os output stream
    \param iplane IPlane to be displayed
  */
  //===============================================================
  std::ostream& operator<<(std::ostream& os, const IPlane& iplane) {
    os << "(";
    for (unsigned i = 0; i < iplane.getNindices() - 1; i++)
      os << iplane[i] << ",";
    os << iplane[iplane.getNindices() - 1] << ")";

    return os;
  }

  //===============================================================
  // IPlane :: Operator ==
  //---------------------------------------------------------------
  //! Determines whether two glide planes are identical or not
  //---------------------------------------------------------------
  /*!
    This routine returns:
    1 if the glide planes are identical
    0 if the glide planes are different

    \return true or false
  */
  //===============================================================
  bool operator==(const IPlane& lhs, const IPlane& rhs) {
    //-------------------------------------------
    // dot products (still valid with 4 indices)
    //-------------------------------------------
    int b1b2 = numodis::math::iScalProduct(lhs.getIndex(), rhs.getIndex());
    int b1b1 = numodis::math::iScalProduct(lhs.getIndex(), lhs.getIndex());
    int b2b2 = numodis::math::iScalProduct(rhs.getIndex(), rhs.getIndex());

    //-------------------------
    // compare scalar products
    //-------------------------
    return b1b1 * b2b2 == b1b2 * b1b2;
  }

  //===============================================================
  // IPlane::operator =
  //---------------------------------------------------------------
  //! copy operator
  //---------------------------------------------------------------
  /*! \param rhs another Burgers vector                          */
  //===============================================================
  IPlane& IPlane::operator=(const IPlane& rhs) {
    //! this feature is needed only for C++98. Deprecated in C++11
    if (_index.size() != rhs._index.size()) _index.resize(rhs._index.size());
    // end feature

    _index = rhs._index;

    return *this;
  }

  //===============================================================
  // IPlane :: Operator !=
  //---------------------------------------------------------------
  //! Determines whether two glide planes are different or not
  //---------------------------------------------------------------
  /*!
    This routine returns:
    0 if the glide planes are identical
    1 if the glide planes are different

    \return true or false
  */
  //===============================================================
  bool operator!=(const IPlane& lhs, const IPlane& rhs) {
    return !(lhs == rhs);
  }

  //===============================================================
  // IPlane :: operator<
  //---------------------------------------------------------------
  //! Compare two glide planes
  //---------------------------------------------------------------
  /*! \return true or false                                      */
  //===============================================================
  bool operator<(const IPlane& lhs, const IPlane& rhs) {
    // invert (if needed) the plane orientation to have the first non-zero term
    // positive
    std::vector<int> left(lhs._index);
    std::vector<int> right(rhs._index);
    for (unsigned i = 0; i < left.size(); i++)
      if (left[i] != 0) {
        if (left[i] < 0)
          for (unsigned j = i; j != left.size(); j++) left[j] = -left[j];
        break;
      }
    for (unsigned i = 0; i < right.size(); i++)
      if (right[i] != 0) {
        if (right[i] < 0)
          for (unsigned j = i; j != left.size(); j++) right[j] = -right[j];
        break;
      }

    // compare term by term
    for (unsigned i = 0; i < left.size(); i++)
      if (left[i] != right[i]) return (left[i] < right[i]);

    return false;
  }

  //===============================================================
  // PlaneCoincide
  //---------------------------------------------------------------
  //! Determines if two planes are identical or not
  //---------------------------------------------------------------
  /*!

    This function returns:
    1 if the two planes are identical
    -1 if the two planes are opposite
    0 otherwise

    \return 1, 0 or -1

  */
  //===============================================================
  int PlaneCoincide(const IPlane& lhs, const IPlane& rhs) {
    // useful dot products
    int lhsrhs = numodis::math::iScalProduct(lhs._index, rhs._index);
    int lhslhs = numodis::math::iScalProduct(lhs._index, lhs._index);
    int rhsrhs = numodis::math::iScalProduct(rhs._index, rhs._index);

    // same direction?
    if (lhslhs * rhsrhs != lhsrhs * lhsrhs) return 0;

    // same orientation?
    return (lhsrhs > 0 ? 1 : -1);
  }

}  // end of namespace numodis
