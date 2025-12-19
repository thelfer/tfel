/*!
 * \file   include/NUMODIS/Vect3.hxx
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

#ifndef NUMEODIS_VECT3_HXX
#define NUMEODIS_VECT3_HXX

#include <iosfwd>
#include "NUMODIS/Config.hxx"

namespace numodis {

  //===============================================================
  // Class Vect3
  //---------------------------------------------------------------
  //! class Vect3 handles a vector of double in 3 dimensions
  //---------------------------------------------------------------
  /*!

    The first author of this method is A. Etcheverry who basically
    got rid of all the valarray<double>(3) in NUMODIS to improve
    computation efficiency.

    This class offers several interesting operators to make 3D
    calculations almost as simple as in Fortran90 ;-)

  */
  //===============================================================
  struct TFELNUMODIS_VISIBILITY_EXPORT Vect3 {
    Vect3();

    Vect3(const Vect3& vec);

    Vect3(double x, double y, double z);

    bool operator==(const Vect3& vec);

    Vect3 operator-() const;

    Vect3 operator+(const Vect3& vec) const;

    Vect3 operator-(const Vect3& vec) const;

    Vect3 operator-(const double scalar) const;

    Vect3 operator*(const double scalar) const;

    Vect3 operator/(const double scalar) const;

    Vect3 operator*(const Vect3& vec) const;

    Vect3& operator=(const Vect3& vec);

    Vect3& operator=(const double scalar);

    Vect3& operator/=(const double scalar);

    Vect3& operator*=(const double scalar);

    Vect3& operator+=(const Vect3& vec);

    Vect3& operator+=(const double scalar);

    Vect3& operator-=(const Vect3& vec);

    Vect3 UnitVector() const;

    Vect3 Cross(const Vect3& vec) const;

    Vect3 UnitCross(const Vect3& vec) const;

    double& operator[](int i);

    double operator[](int i) const;

    double Length() const;

    double SquareLength() const;

    double Dot(const Vect3& vec) const;

    void Swap(Vect3& vec);

    bool Normalize();

    //==========================================================
    // Vect3::X
    //----------------------------------------------------------
    //! Return a reference to the first component of the vector
    //----------------------------------------------------------
    /*! \return reference to the component                    */
    //==========================================================
    inline double& X() { return this->_vector[0]; }

    //===========================================================
    // Vect3::Y
    //-----------------------------------------------------------
    //! Return a reference to the second component of the vector
    //-----------------------------------------------------------
    /*! \return reference to the component                     */
    //===========================================================
    inline double& Y() { return this->_vector[1]; }

    //==========================================================
    // Vect3::Z
    //----------------------------------------------------------
    //! Return a reference to the third component of the vector
    //----------------------------------------------------------
    /*! \return reference to the component                    */
    //==========================================================
    inline double& Z() { return this->_vector[2]; }

    //==========================================================
    // Vect3::X
    //----------------------------------------------------------
    //! Return the first component of the vector
    //----------------------------------------------------------
    /*! \return first component                               */
    //==========================================================
    inline double X() const { return this->_vector[0]; }

    //==========================================================
    // Vect3::Y
    //----------------------------------------------------------
    //! Return the second component of the vector
    //----------------------------------------------------------
    /*! \return second component                              */
    //==========================================================
    inline double Y() const { return this->_vector[1]; }

    //==========================================================
    // Vect3::Z
    //----------------------------------------------------------
    //! Return the third component of the vector
    //----------------------------------------------------------
    /*! \return third component                               */
    //==========================================================
    inline double Z() const { return this->_vector[2]; }

    //==========================================================
    // Vect::Norm
    //----------------------------------------------------------
    //! Return the length of the vector
    //----------------------------------------------------------
    /*! \return length of the vector                          */
    //==========================================================
    double Norm() const { return this->Length(); }

   protected:
    double _vector[3];

   private:
    TFELNUMODIS_VISIBILITY_FRIEND_EXPORT friend std::ostream& operator<<(
        std::ostream&, const Vect3&);
  };

  TFELNUMODIS_VISIBILITY_EXPORT Vect3 operator*(const double, const Vect3&);

  TFELNUMODIS_VISIBILITY_EXPORT Vect3 operator-(const double, const Vect3&);

  TFELNUMODIS_VISIBILITY_EXPORT std::ostream& operator<<(std::ostream&,
                                                         const Vect3&);

}  // namespace numodis

#endif
