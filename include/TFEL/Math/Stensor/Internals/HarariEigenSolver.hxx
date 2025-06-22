/*!
 * \file   HarariEigenSolver.hxx
 * \brief
 * \author Maxence Wangermez/Thomas Helfer
 * \date   13/02/2024
 * \copyright Copyright (C) 2006-2024 CEA/DEN, EDF R&D. All rights
 * reserved.
 *
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 *
 * This file was adapted from Numerical diagonalization of 3x3
 * matrices of Isaac Harari and Uri Albocher, see
 * [1] Computation of eigenvalues of a real, symmetric 3 × 3
 * matrix with particular reference to the pernicious
 * case of two nearly equal eigenvalues
 *
 */

#ifndef LIB_TFEL_MATH_HARARIEIGENSOLVER_HXX
#define LIB_TFEL_MATH_HARARIEIGENSOLVER_HXX

#include <cmath>
#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/tmatrix.hxx"

namespace tfel::math::internals {

  template <typename real>
  struct HarariEigensolver3x3 {
    /*!
     * \brief calculates the eigensystem of a real symmetric 3x3 matrix
     *    [ A  D  E ]
     *    [ D  B  F ]
     *    [ E  F  C ]
     * \param[out] vp: eigen values
     * \param[out] m:  eigen vectors
     * \param[in]  A:  component of the matrix
     * \param[in]  B:  component of the matrix
     * \param[in]  C:  component of the matrix
     * \param[in]  D:  component of the matrix
     * \param[in]  E:  component of the matrix
     * \param[in]  F:  component of the matrix
     */
    static void computeEigenVectors(tvector<3u, real>&,
                                    tmatrix<3u, 3u, real>&,
                                    const real,
                                    const real,
                                    const real,
                                    const real,
                                    const real,
                                    const real);

    /*!
     * \brief computes the first eigen value of a real symmetric 3x3 matrix
     *    [ A  D  E ]
     *    [ D  B  F ]
     *    [ E  F  C ]
     * \param[out] vp: eigen values
     * \param[in]  A:  component of the matrix
     * \param[in]  B:  component of the matrix
     * \param[in]  C:  component of the matrix
     * \param[in]  D:  component of the matrix
     * \param[in]  E:  component of the matrix
     * \param[in]  F:  component of the matrix
     */
    static void computeEigenValues(tvector<3u, real>&,
                                   const real,
                                   const real,
                                   const real,
                                   const real,
                                   const real,
                                   const real);

   private:
    /*!
     * \brief computes the first eigen vector
     * of a real symmetric 3x3 matrix A, associated
     * with the eigen value stored in vp[0] and
     * stores in the first column of m
     *     [ A  D  E ]
     * A = [ D  B  F ]
     *     [ E  F  C ]
     * The vectors s1 and s2 of [1] are also stored
     * in columns two and three of m
     * \param[out] vp: eigen values
     * \param[out] m:  m[0]: first ev, m[1] and m[2]: orthogonal subspace
     * \param[in]  A:  component of the matrix
     * \param[in]  B:  component of the matrix
     * \param[in]  C:  component of the matrix
     * \param[in]  D:  component of the matrix
     * \param[in]  E:  component of the matrix
     * \param[in]  F:  component of the matrix
     */
    static void computeProjectionBasis(tvector<3u, real>&,
                                       tmatrix<3u, 3u, real>&,
                                       const real,
                                       const real,
                                       const real,
                                       const real,
                                       const real,
                                       const real);
  };

}  // end of namespace tfel::math::internals

#include "TFEL/Math/Stensor/Internals/HarariEigenSolver.ixx"

#endif /* LIB_TFEL_MATH_HARARIEIGENSOLVER_HXX */
