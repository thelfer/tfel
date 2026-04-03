/*!
 * \file   FSESSymmetricEigenSolver.hxx
 * \brief
 * \author Joachim Kopp/Thomas Helfer
 * \date   30/12/2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 *
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 *
 * This file was adapted from Numerical diagonalization of 3x3 matrices:
 * https://arxiv.org/pdf/physics/0610206v3.pdf
 *
 * ----------------------------------------------------------------------------
 * Numerical diagonalization of 3x3 matrices
 * Copyright (C) 2006
 * ----------------------------------------------------------------------------
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 * ----------------------------------------------------------------------------
 */

#ifndef LIB_TFEL_MATH_FSESSYMMETRICEIGENSOLVER_HXX
#define LIB_TFEL_MATH_FSESSYMMETRICEIGENSOLVER_HXX

#include <cmath>
#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/tmatrix.hxx"

namespace tfel::math::internals {

  template <typename real>
  struct FSESAnalyticalSymmetricEigensolver2x2 {
    /*!
     * \brief calculates the eigenvalues of a real symmetric 2x2 matrix
     *    [ A  B ]
     *    [ B  C ]
     * \param[out] vp: eigen values
     * \param[in]  A:  component of the matrix
     * \param[in]  B:  component of the matrix
     * \param[in]  C:  component of the matrix
     */
    static void computeEigenValues(tvector<3u, real>&,
                                   const real,
                                   const real,
                                   const real);
    /*!
     * \brief calculates the eigensystem of a real symmetric 2x2 matrix
     *    [ A  B ]
     *    [ B  C ]
     * in the form
     *    [ A  B ]  =  [ cs  -sn ] [ rt1   0  ] [  cs  sn ]
     *    [ B  C ]     [ sn   cs ] [  0   rt2 ] [ -sn  cs ]
     * where rt1 >= rt2.
     * \param[out] vp: eigen values
     * \param[out] m:  eigen vectors
     * \param[in]  A:  component of the matrix
     * \param[in]  B:  component of the matrix
     * \param[in]  C:  component of the matrix
     */
    static void computeEigenVectors(tvector<3u, real>&,
                                    tmatrix<3u, 3u, real>&,
                                    const real,
                                    const real,
                                    const real);
  };

  template <typename real>
  struct FSESAnalyticalSymmetricEigensolver3x3 {
    static void computeEigenValues(tvector<3u, real>&,
                                   const tmatrix<3u, 3u, real>&);
    static void computeEigenVectors(tvector<3u, real>&,
                                    tmatrix<3u, 3u, real>&,
                                    tmatrix<3u, 3u, real>&);
  };

}  // end of namespace tfel::math::internals

#include "TFEL/Math/Stensor/Internals/FSESSymmetricEigenSolver.ixx"

#endif /* LIB_TFEL_MATH_FSESSYMMETRICEIGENSOLVER_HXX */
