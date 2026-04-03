/*!
 * \file   src/NUMODIS/Utilities.cxx
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

#include <cerrno>
#include <numeric>
#include <ostream>
#include <algorithm>

#include "NUMODIS/Math/Utilities.hxx"

namespace numodis {

  namespace math {

    //===============================================================
    // dUnitVector
    //---------------------------------------------------------------
    //! Returns the unit vector V corresponding to a vector U (3D)
    //---------------------------------------------------------------
    // Note that we do not check whether U is of non-zero length.
    // \param U input vector (3D)
    // \param V input vector (3D)
    //===============================================================
    double dUnitVector(const Vect3& U, Vect3& V) {
      double norm = U.Length();

      V = U / norm;

      return norm;
    }

    //===============================================================
    // dUnitVector
    //---------------------------------------------------------------
    //! Normalize vector U (3D)
    //---------------------------------------------------------------
    // Note that we do not check whether U is of non-zero length.
    // \param U input/output vector (3D)
    //===============================================================
    double dUnitVector(Vect3& U) {
      double norm = U.Length();

      U /= norm;

      return norm;
    }

    //===============================================================
    // iCollinear
    //---------------------------------------------------------------
    //! Determine whether two vectors of int are collinear or not
    //---------------------------------------------------------------
    /*!

      \bug QUID DE VECTEURS NULS? QUI SE SERT DE CA ET POURQUOI ?

      \param U first vector
      \param V second vector
      \return true if U and V are collinear, false otherwise
    */
    //===============================================================
    bool iCollinear(const std::vector<int>& U, const std::vector<int>& V) {
      //----------------------------------------------------
      // dot products (compatible with 4 indices notations)
      //----------------------------------------------------
      int UV = iScalProduct(U, V);
      int UU = iScalProduct(U, U);
      int VV = iScalProduct(V, V);

      //-------------------------
      // compare scalar products
      //-------------------------
      return (UU != 0 && VV != 0 ? (UU * VV == UV * UV) : (UU == VV));
    }

    //===============================================================
    // iCollinear
    //---------------------------------------------------------------
    //! Determine whether two std::vector<int> are collinear or not
    //---------------------------------------------------------------
    /*!
      \param U first vector
      \param V second vector
      \param pdirection +1/0/-1
      \return true if U and V are collinear, false otherwise
    */
    //===============================================================
    bool iCollinear(const std::vector<int>& U,
                    const std::vector<int>& V,
                    int* pdirection) {
      //----------------
      // initialization
      //----------------
      *pdirection = 0;

      //--------------
      // compare size
      //--------------
      if (U.size() != V.size()) return false;

      //----------------------------------------------------
      // dot products (compatible with 4 indices notations)
      //----------------------------------------------------
      int UV = iScalProduct(U, V);
      int UU = iScalProduct(U, U);
      int VV = iScalProduct(V, V);

      //-------------------------
      // compare scalar products
      //-------------------------
      if (UU * VV == UV * UV) {
        *pdirection = (UV < 0 ? -1 : +1);
        return true;
      }

      //---------------
      // default value
      //---------------
      return false;
    }

    //===============================================================
    // iSortVector
    //---------------------------------------------------------------
    //! Sort a vector of int
    //---------------------------------------------------------------
    /*! \param U vector to be sorted then sorted                 */
    //===============================================================
    void iSortVector(std::vector<int>& U) {
      std::vector<int> V(U.size());

      // copy as a vector
      for (unsigned i = 0; i < U.size(); i++) V[i] = U[i];

      // sort as a vector
      sort(V.begin(), V.end());

      // copy back
      for (unsigned i = 0; i < U.size(); i++) U[i] = V[i];
    }

    //===============================================================
    // iSortVector3FirstValue
    //---------------------------------------------------------------
    //! Sort 3 first values of a vector of int
    //---------------------------------------------------------------
    /*! \param U vector to be sorted then sorted                 */
    //===============================================================
    void iSortVector3FirstValue(std::vector<int>& U) {
      if (U[0] < U[1]) {
        std::swap(U[0], U[1]);
      }
      if (U[1] < U[2]) {
        std::swap(U[1], U[2]);
      } else
        return;
      if (U[0] < U[1]) {
        std::swap(U[0], U[1]);
      }
    }

    //===============================================================
    // GCD
    //---------------------------------------------------------------
    //! Compute the greatest common divisor of an ensemble of integers
    //---------------------------------------------------------------
    /*!
      \param u input vector of int
      \return greatest common divisor
    */
    //===============================================================
    int GCD(const std::vector<int>& u) {
      // trivial situation
      if (u.size() == 1) return std::abs(u[0]);
      // default value
      int gcd = std::abs(u[0]);
      // find the overall gcd
      for (unsigned i = 1; i < u.size(); i++)
        gcd = numodis::math::GCD(gcd, std::abs(u[i]));
      // output value
      return gcd;
    }

    //===============================================================
    // Epsilon
    //---------------------------------------------------------------
    //! Permutation / Levi Civita symbol
    //---------------------------------------------------------------
    /*!

      This function should work for {0,1,2} and {1,2,3}.

      \param i i
      \param j j
      \param k k
      \return permutation(i,j,k) (-1, 0 or +1)

    */
    //===============================================================
    int Epsilon(const unsigned i, const unsigned j, const unsigned k) {
      // look for identical terms
      if (i == j || j == k || i == k) return 0;

      // look for odd permutations
      if (i + 2 == j || j + 2 == k || k + 2 == i) return -1;

      // default value
      return 1;
    }

  }  // end namespace math

}  // end of namespace numodis
