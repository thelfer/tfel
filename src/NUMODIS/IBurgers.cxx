/*!
 * \file   src/NUMODIS/IBurger.cxx
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
#include "NUMODIS/IBurgers.hxx"

namespace numodis {

  IBurgers::IBurgers(IBurgers&&) = default;

  IBurgers::IBurgers(const IBurgers&) = default;

  //===============================================================
  // IBurgers::operator+
  //---------------------------------------------------------------
  //! operator+
  //---------------------------------------------------------------
  /*!

    Add two Burgers vectors

    \param rhs another Burgers vector
    \return sum of the Burgers vectors

  */
  //===============================================================
  IBurgers IBurgers::operator+(const IBurgers& rhs) const {
    std::vector<int> sum(this->getIndex());

    for (unsigned i = 0; i != _index.size(); i++) sum[i] += rhs._index[i];

    return IBurgers(sum);
  }

  //===============================================================
  // IBurgers::operator-
  //---------------------------------------------------------------
  //! operator-
  //---------------------------------------------------------------
  /*!

    Subtract two Burgers vectors

    \param rhs another Burgers vector
    \return sum of the Burgers vectors

  */
  //===============================================================
  IBurgers IBurgers::operator-(const IBurgers& rhs) const {
    std::vector<int> sum(this->getIndex());

    for (unsigned i = 0; i != _index.size(); i++) sum[i] -= rhs._index[i];

    return IBurgers(sum);
  }

  //===============================================================
  // IBurgers::operator =
  //---------------------------------------------------------------
  //! copy operator
  //---------------------------------------------------------------
  /*! \param rhs another Burgers vector                          */
  //===============================================================
  IBurgers& IBurgers::operator=(const IBurgers& rhs) {
    //! this feature is needed only for C++98. Deprecated in C++11
    if (_index.size() != rhs._index.size()) _index.resize(rhs._index.size());
    // end feature

    _index = rhs._index;

    return *this;
  }

  //===============================================================
  // IBurgers::operator +=
  //---------------------------------------------------------------
  //! operator +=
  //---------------------------------------------------------------
  /*!

    Add another Burgers vector to the current one

    \param rhs another Burgers vector

  */
  //===============================================================
  IBurgers& IBurgers::operator+=(const IBurgers& rhs) {
    for (unsigned i = 0; i != _index.size(); i++) _index[i] += rhs._index[i];
    return *this;
  }

  //===============================================================
  // IBurgers::operator -=
  //---------------------------------------------------------------
  //! operator -=
  //---------------------------------------------------------------
  /*!

    Subtract another Burgers vector to the current one

    \param rhs another Burgers vector

  */
  //===============================================================
  IBurgers& IBurgers::operator-=(const IBurgers& rhs) {
    for (unsigned i = 0; i != _index.size(); i++) _index[i] -= rhs._index[i];
    return *this;
  }

  //===============================================================
  // IBurgers::operator /=
  //---------------------------------------------------------------
  //! operator /=
  //---------------------------------------------------------------
  /*!

    Divide the current Burgers by an integer

    \param rhs another Burgers vector

  */
  //===============================================================
  IBurgers& IBurgers::operator/=(const int scalar) {
    for (unsigned i = 0; i != _index.size(); i++) _index[i] /= scalar;
    return *this;
  }

  IBurgers::~IBurgers() = default;

  //===============================================================
  // Coincide (friend of IBurgers)
  //---------------------------------------------------------------
  //! Determines whether two burgers vector are identical or not
  //---------------------------------------------------------------
  /*!
    This routine returns:
    1 if the burgers vectors are identical
    -1 if the burgers vector are opposite
    0 if the burgers vector are different

    \return 1, 0 or -1

  */
  //===============================================================
  int Coincide(const IBurgers& b1, const IBurgers& b2) {
    //--------------
    // compare size
    //--------------
    if (b1._index.size() != b2._index.size()) return 0;

    //----------------------------------------------------
    // Dot products (compatible with 4 indices notations)
    //----------------------------------------------------
    int b1b2 = numodis::math::iScalProduct(b1._index, b2._index);
    int b1b1 = numodis::math::iScalProduct(b1._index, b1._index);
    int b2b2 = numodis::math::iScalProduct(b2._index, b2._index);

    //-------------------------
    // compare scalar products
    //-------------------------
    if (b1b1 != 0 && b2b2 != 0) {
      if (b1b1 * b2b2 == b1b2 * b1b2)
        return (b1b2 > 0 ? 1 : -1);
      else
        return 0;
    } else
      return (b1b1 == b2b2);
  }

  //===============================================================
  // IBurgers : Operator <<
  //---------------------------------------------------------------
  //! Print the data of a Burgers vector
  //---------------------------------------------------------------
  /*!
    \param os output stream
    \param iburgers IBurgers to be printed
  */
  //===============================================================
  std::ostream& operator<<(std::ostream& os, const IBurgers& iburgers) {
    os << "[";
    for (unsigned i = 0; i != iburgers.getNindices() - 1; i++)
      os << iburgers[i] << ",";
    os << iburgers[iburgers.getNindices() - 1] << "]";

    return os;
  }

}  // end of namespace numodis
