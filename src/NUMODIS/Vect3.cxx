/*!
 * \file   src/NUMODIS/Vect3.cxx
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

#include <cmath>
#include <limits>
#include <ostream>
#include <stdexcept>

#include "TFEL/Raise.hxx"
#include "NUMODIS/Vect3.hxx"

namespace numodis {

  //===============================================================
  // Vect3::Vect3
  //---------------------------------------------------------------
  //! Constructor
  //---------------------------------------------------------------
  /*! Note: the vector is set to 0                               */
  //===============================================================
  Vect3::Vect3() {
    this->_vector[0] = 0.0;
    this->_vector[1] = 0.0;
    this->_vector[2] = 0.0;
  }

  //===============================================================
  // Vect3::Vect3
  //---------------------------------------------------------------
  //! Constructor
  //---------------------------------------------------------------
  /*!
    \param x x
    \param y y
    \param z z
  */
  //===============================================================
  Vect3::Vect3(double x, double y, double z) {
    this->_vector[0] = x;
    this->_vector[1] = y;
    this->_vector[2] = z;
  }

  //===============================================================
  // Vect3::Vect3
  //---------------------------------------------------------------
  //! Copy constructor
  //---------------------------------------------------------------
  /*! \param vec vector to be copied                             */
  //===============================================================
  Vect3::Vect3(const Vect3& vec) {
    this->_vector[0] = vec._vector[0];
    this->_vector[1] = vec._vector[1];
    this->_vector[2] = vec._vector[2];
  }

  //===============================================================
  // Vect3::Vect3
  //---------------------------------------------------------------
  //! Copy operator
  //---------------------------------------------------------------
  /*!
    \param vec vector to be copied
    \return reference to the current vector
  */
  //===============================================================
  Vect3& Vect3::operator=(const Vect3& vec) {
    this->_vector[0] = vec._vector[0];
    this->_vector[1] = vec._vector[1];
    this->_vector[2] = vec._vector[2];
    return *this;
  }

  //===============================================================
  // Vect3::Vect3
  //---------------------------------------------------------------
  //! Comparison operator
  //---------------------------------------------------------------
  /*!
    \param vec vector to be compared to
    \return true if equal, false otherwise
  */
  //===============================================================
  bool Vect3::operator==(const Vect3& vec) {
    return (this->_vector[0] == vec._vector[0] &&
            this->_vector[1] == vec._vector[1] &&
            this->_vector[2] == vec._vector[2]);
  }

  //===============================================================
  // Vect3::Vect3
  //---------------------------------------------------------------
  //! Assignment operator
  //---------------------------------------------------------------
  /*!
    \param scalar initial value for all the terms of the vector
    \return reference to the current vector
  */
  //===============================================================
  Vect3& Vect3::operator=(const double scalar) {
    this->_vector[0] = scalar;
    this->_vector[1] = scalar;
    this->_vector[2] = scalar;
    return *this;
  }

  //===============================================================
  // Vect3::Cross
  //---------------------------------------------------------------
  //! Return the cross product of two vectors W = U x V
  //---------------------------------------------------------------
  /*!
    Note : This object is the first vector U
    \param V second vector
    \return Cross-product W = U x V
  */
  //===============================================================
  Vect3 Vect3::Cross(const Vect3& vec) const {
    return Vect3(
        this->_vector[1] * vec._vector[2] - this->_vector[2] * vec._vector[1],
        this->_vector[2] * vec._vector[0] - this->_vector[0] * vec._vector[2],
        this->_vector[0] * vec._vector[1] - this->_vector[1] * vec._vector[0]);
  }

  //===============================================================
  // Vect3::UnitCross
  //---------------------------------------------------------------
  //! Cross product of two vectors W = U x V and normalization
  //---------------------------------------------------------------
  /*!
    Note : This object is the first vector U
    \param V second vector
    \return Cross-product W = U x V as a unit vector
  */
  //===============================================================
  Vect3 Vect3::UnitCross(const Vect3& vec) const {
    Vect3 cross = this->Cross(vec);
    cross.Normalize();
    return cross;
  }

  //===============================================================
  // Vect3::operator*
  //---------------------------------------------------------------
  //! Multiplies a vector by a scalar
  //---------------------------------------------------------------
  /*!
    \param scalar
    \return new output vector
  */
  //===============================================================
  Vect3 Vect3::operator*(const double scalar) const {
    return Vect3(this->_vector[0] * scalar, this->_vector[1] * scalar,
                 this->_vector[2] * scalar);
  }

  //===============================================================
  // Vect3::operator --
  Vect3 Vect3::operator-(const double scalar) const {
    return Vect3(this->_vector[0] - scalar, this->_vector[1] - scalar,
                 this->_vector[2] - scalar);
  }

  Vect3 operator*(const double scalar, const Vect3& vec) {
    return Vect3(vec[0] * scalar, vec[1] * scalar, vec[2] * scalar);
  }

  Vect3 operator-(const double scalar, const Vect3& vec) {
    return Vect3(vec[0] - scalar, vec[1] - scalar, vec[2] - scalar);
  }

  Vect3 Vect3::operator+(const Vect3& vec) const {
    return Vect3(this->_vector[0] + vec[0], this->_vector[1] + vec[1],
                 this->_vector[2] + vec[2]);
  }

  Vect3 Vect3::operator-(const Vect3& vec) const {
    return Vect3(this->_vector[0] - vec[0], this->_vector[1] - vec[1],
                 this->_vector[2] - vec[2]);
  }

  Vect3 Vect3::operator/(double scalar) const {
    return Vect3(this->_vector[0] / scalar, this->_vector[1] / scalar,
                 this->_vector[2] / scalar);
  }

  Vect3& Vect3::operator*=(const double scalar) {
    this->_vector[0] *= scalar;
    this->_vector[1] *= scalar;
    this->_vector[2] *= scalar;
    return *this;
  }

  Vect3& Vect3::operator/=(const double scalar) {
    this->_vector[0] /= scalar;
    this->_vector[1] /= scalar;
    this->_vector[2] /= scalar;
    return *this;
  }

  Vect3& Vect3::operator+=(const Vect3& vec) {
    this->_vector[0] += vec._vector[0];
    this->_vector[1] += vec._vector[1];
    this->_vector[2] += vec._vector[2];
    return *this;
  }

  Vect3& Vect3::operator+=(const double scalar) {
    this->_vector[0] += scalar;
    this->_vector[1] += scalar;
    this->_vector[2] += scalar;
    return *this;
  }

  Vect3& Vect3::operator-=(const Vect3& vec) {
    this->_vector[0] -= vec._vector[0];
    this->_vector[1] -= vec._vector[1];
    this->_vector[2] -= vec._vector[2];
    return *this;
  }

  Vect3 Vect3::operator-() const {
    return Vect3(-this->_vector[0], -this->_vector[1], -this->_vector[2]);
  }

  double Vect3::operator[](int i) const { return this->_vector[i]; }

  double& Vect3::operator[](int i) { return this->_vector[i]; }

  double Vect3::Dot(const Vect3& vec) const {
    return (this->_vector[0] * vec._vector[0] +
            this->_vector[1] * vec._vector[1] +
            this->_vector[2] * vec._vector[2]);
  }

  void Vect3::Swap(Vect3& vec) {
    std::swap(this->_vector[0], vec._vector[0]);
    std::swap(this->_vector[1], vec._vector[1]);
    std::swap(this->_vector[2], vec._vector[2]);
  }

  double Vect3::Length() const {
    return sqrt(this->_vector[0] * this->_vector[0] +
                this->_vector[1] * this->_vector[1] +
                this->_vector[2] * this->_vector[2]);
  }

  double Vect3::SquareLength() const {
    return (this->_vector[0] * this->_vector[0] +
            this->_vector[1] * this->_vector[1] +
            this->_vector[2] * this->_vector[2]);
  }

  bool Vect3::Normalize() {
    const auto length = Length();
    tfel::raise_if(length <= std::numeric_limits<double>::min(),
                   "Vect3::Normalize: "
                   "Trying to normalize null vector");
    *this /= length;
    return true;
  }

  Vect3 Vect3::UnitVector() const {
    double length = Length();

    return Vect3(this->_vector[0] / length, this->_vector[1] / length,
                 this->_vector[2] / length);
  }

  //===============================================================
  // Vect3 : Operator <<
  //---------------------------------------------------------------
  //! Print the data of the vector in a stream
  //---------------------------------------------------------------
  /*!
    \param os a stream
    \param vect3 vector to be displayed
  */
  //===============================================================
  std::ostream& operator<<(std::ostream& stream, const Vect3& vect3) {
    stream << "[" << vect3._vector[0] << ", " << vect3._vector[1] << ", "
           << vect3._vector[2] << "]";
    return stream;
  }

}  // end of namespace numodis
