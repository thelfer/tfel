/*!
 * \file include/NUMODIS/Math/Utilities.hxx
 * \brief Contains basic mathematical functions and routines
 *
 * Note that:
 * - Some functions are declared as INLINE functions for maximum
 * efficiency. As a consequence, they are declared AND defined
 * in the .hxx file.
 * - Some functions have their BLAS or LAPACK equivalent, which
 * are specifically designed to hanlde very large objects and are
 * not efficient for small calculations. As a consequence, you
 * should prefer the following functions for 3D calculations.
 * \author Laurent Dupuy
 * \date   9/06/2017
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef NUMEODIS_UTILITIES_HXX
#define NUMEODIS_UTILITIES_HXX

#include <cmath>
#include <vector>
#include <cstdlib>
#include "NUMODIS/Config.hxx"
#include "NUMODIS/Vect3.hxx"

namespace numodis {

  namespace math {

    // Greatest common divisor for rings (including unsigned integers)
    template <typename RingType>
    RingType gcd_euclidean(RingType a, RingType b) {
      constexpr auto zero = static_cast<RingType>(0);
      // Reduce by GCD-remainder property [GCD(a,b) == GCD(b,a MOD b)]
      while (true) {
        if (a == zero) return b;
        b %= a;

        if (b == zero) return a;
        a %= b;
      }
    }  // end of gcd_euclidean

    //===============================================================
    // GCD
    //---------------------------------------------------------------
    //! Compute the greatest common divisor of two positive integers
    //---------------------------------------------------------------
    /*!
     * \param a first integer
     * \param b second integer
     * \return greatest common divisor
     */
    //===============================================================
    template <typename IntegerType>
    inline IntegerType GCD(const IntegerType& a, const IntegerType& b) {
      // Avoid repeated construction
      constexpr auto zero = static_cast<IntegerType>(0);
      const auto result = gcd_euclidean(a, b);
      return (result < zero) ? static_cast<IntegerType>(-result) : result;
    }

    TFELNUMODIS_VISIBILITY_EXPORT int GCD(const std::vector<int>& u);

    //===============================================================
    // iCrossProduct
    //---------------------------------------------------------------
    //! Cross product of two vectors (3D): W = U x V
    //---------------------------------------------------------------
    /*!
      \param U first vector (3D)
      \param V second vector (3D)
      \param W cross-product
    */
    //===============================================================
    inline void iCrossProduct(const std::vector<int>& U,
                              const std::vector<int>& V,
                              std::vector<int>& W) {
      W[0] = U[1] * V[2] - U[2] * V[1];
      W[1] = U[2] * V[0] - U[0] * V[2];
      W[2] = U[0] * V[1] - U[1] * V[0];
    }

    //===============================================================
    // iCrossProduct
    //---------------------------------------------------------------
    //! Cross product of two vectors (3D): W = U x V
    //---------------------------------------------------------------
    /*!
      \param U first vector (3D)
      \param V second vector (3D)
      \param W cross-product
    */
    //===============================================================
    inline void iCrossProduct(const int U[], const int V[], int W[]) {
      W[0] = U[1] * V[2] - U[2] * V[1];
      W[1] = U[2] * V[0] - U[0] * V[2];
      W[2] = U[0] * V[1] - U[1] * V[0];
    }

    //===============================================================
    // iScalProduct
    //---------------------------------------------------------------
    //! Scalar product of two std::vector<int>: U.V
    //---------------------------------------------------------------
    /*
      \param U first vector
      \param V second vector
    */
    //===============================================================
    inline int iScalProduct(const std::vector<int>& U,
                            const std::vector<int>& V) {
      int sum = 0;
      for (unsigned i = 0; i != U.size(); i++) sum += U[i] * V[i];
      return sum;
    }

    //===============================================================
    // abs
    //---------------------------------------------------------------
    //! return the absolute value of a vector
    //---------------------------------------------------------------
    /*
      \param U first vector
      \param V second vector
    */
    //===============================================================
    inline std::vector<int> abs(const std::vector<int>& U) {
      std::vector<int> V(U);
      for (unsigned i = 0; i != V.size(); i++) V[i] = std::abs(V[i]);

      return V;
    }

    //===============================================================
    // dTripleProduct
    //---------------------------------------------------------------
    //! Triple product of three vectors: (UxV).W
    //---------------------------------------------------------------
    /*!
      \param U first vector (3D)
      \param V second vector (3D)
      \param W third vector (3D)
      \return triple product
    */
    //===============================================================
    inline double dTripleProduct(const Vect3& U,
                                 const Vect3& V,
                                 const Vect3& W) {
      return (U[1] * V[2] - U[2] * V[1]) * W[0] +
             (U[2] * V[0] - U[0] * V[2]) * W[1] +
             (U[0] * V[1] - U[1] * V[0]) * W[2];
    }

    //===============================================================
    // dNorm
    //---------------------------------------------------------------
    //! Norm of a vector (3D)
    //---------------------------------------------------------------
    /*! \param X a vector                                          */
    //===============================================================
    inline double dNorm(const Vect3& X) { return X.Norm(); }

    //===============================================================
    // dNorm2
    //---------------------------------------------------------------
    //! Norm^2 of a vector (3D)
    //---------------------------------------------------------------
    /*! \param X a vector                                          */
    //===============================================================
    inline double dNorm2(const Vect3& X) { return X.SquareLength(); }

    //===============================================================
    // dDistance
    //---------------------------------------------------------------
    //! Distance between two points (3D)
    //---------------------------------------------------------------
    /*!
      \param X1 vector
      \param X2 vector
      \return distance between X1 and X2
    */
    //===============================================================
    inline double dDistance(const Vect3& X1, const Vect3& X2) {
      Vect3 X(X1 - X2);
      return X.Norm();
    }

    //===============================================================
    // dCubeRoot
    //---------------------------------------------------------------
    //! Cube root of x.
    //---------------------------------------------------------------
    /*!
      This function works even if x is negative.
      \param x
      \return Cube root of x
    */
    //===============================================================
    inline double dCubeRoot(const double x) {
      return (x >= 0.0 ? pow(x, 1. / 3.) : -pow(-x, 1. / 3.));
    }

    //===============================================================
    // dRound
    //---------------------------------------------------------------
    //! Returns the integer value closest to x
    //---------------------------------------------------------------
    /*!
      There is no round function in the standard library. Some
      compilers may include round though.
      Examples:
      x = -0.51 -> dRound = -1
      x = -0.49 -> dRound =  0
      x = 0.49 -> dRound = 0
      x = 0.51 -> dRound = 1
      \param x real number
      \return the integer value closest to x
    */
    //===============================================================
    inline int dRound(const double x) {
      return static_cast<int>(x > 0.0 ? x + 0.5 : x - 0.5);
    }

    //===============================================================
    // dInt
    //---------------------------------------------------------------
    //! Returns the smallest integer value less than x.
    //---------------------------------------------------------------
    /*!
      Note that dInt is different from ceil!
      Examples:
      x = -0.51 -> dInt = -1
      x = -0.49 -> dInt = -1
      x =  0.49 -> dInt = 0
      x =  0.51 -> dInt = 0
      x = 1.01  -> dInt = 1
      \param x real number
      \return the smallest integer value less than x
    */
    //===============================================================
    inline int dInt(const double x) {
      const auto ix = static_cast<int>(x);
      return x >= 0.0 ? ix : -1 + ix;
    }

    //===============================================================
    // Kronecker
    //---------------------------------------------------------------
    //! Returns the value of Kronecker (i,j)
    //---------------------------------------------------------------
    /*!
      \param i an integer
      \param j another integer
      \return 1 if (i=j), 0 otherwise
    */
    //===============================================================
    inline int Kronecker(const unsigned i, const unsigned j) {
      return (i == j);
    }

    //===============================================================
    // Abs<...>
    //---------------------------------------------------------------
    //! Calculate the absolute value
    //===============================================================
    template <class TYPE>
    struct Abs {
      //! \brief a simple alias
      using argument_type = TYPE;
      //! \brief a simple alias
      using result_type = void;
      //! defines the absolute value function of x
      /*! \param x input value */
      void operator()(TYPE& x) { x = (x >= 0 ? x : -x); }
    };

    //===============================================================
    // Sgn
    //---------------------------------------------------------------
    //! Return the sign of a number
    //---------------------------------------------------------------
    /*!
      \param val input value
      \return -1, 0 or 1
    */
    //===============================================================
    template <typename T>
    int Sgn(const T val) {
      return (T(0) < val) - (val < T(0));
    }

    //===============================================================
    //                 NON-INLINE FUNCTIONS
    //===============================================================

    double dUnitVector(const Vect3& U, Vect3& V);

    double dUnitVector(Vect3& U);

    bool dCollinear(const Vect3& U, const Vect3& V);

    bool dCollinear(const Vect3& U, const Vect3& V, Vect3& W);

    bool dOrthogonal(const Vect3& U, const Vect3& V, double* res);

    bool iCollinear(const std::vector<int>& U, const std::vector<int>& V);

    bool iCollinear(const std::vector<int>& U,
                    const std::vector<int>& V,
                    int* pdirection);

    void iSortVector(std::vector<int>& U);

    void iSortVector3FirstValue(std::vector<int>& U);

    int Epsilon(const unsigned i, const unsigned j, const unsigned k);

  }  // end namespace math

}  // end of namespace numodis

#endif  // NUMEODIS_UTILITIES_HXX
