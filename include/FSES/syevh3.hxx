/*!
 * \file   include/FSES/syevh3.hxx
 * \brief
 * \author Joachim Kopp/Thomas Helfer
 * \date   02 janv. 2017
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 *
 * ----------------------------------------------------------------------------
 * This file has been introduced in TFEL with the courtesy of Joachim Kopp.
 *
 * Original licence
 *
 * Numerical diagonalization of 3x3 matrcies
 * Copyright (C) 2006  Joachim Kopp
 *
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
 */

#ifndef LIB_FSES_SYEVH3_HXX
#define LIB_FSES_SYEVH3_HXX

namespace fses {

  // ----------------------------------------------------------------------------
  // Calculates the eigenvalues and normalized eigenvectors of a symmetric 3x3
  // matrix A using Cardano's method for the eigenvalues and an analytical
  // method based on vector cross products for the eigenvectors. However,
  // if conditions are such that a large error in the results is to be
  // expected, the routine falls back to using the slower, but more
  // accurate QL algorithm. Only the diagonal and upper triangular parts of A
  // need to contain meaningful values. Access to A is read-only.
  // ----------------------------------------------------------------------------
  // Parameters:
  //   Q: Storage buffer for eigenvectors
  //   w: Storage buffer for eigenvalues
  //   A: The symmetric input matrix
  // ----------------------------------------------------------------------------
  // Return value:
  //   0: Success
  //  -1: Error
  // ----------------------------------------------------------------------------
  // Dependencies:
  //   syevc3(), sytrd3(), syevq3()
  // ----------------------------------------------------------------------------
  // Version history:
  //   v1.1: Simplified fallback condition --> speed-up
  //   v1.0: First released version
  // ----------------------------------------------------------------------------
  template <typename MatrixType, typename VectorType, typename MatrixType2>
  int syevh3(MatrixType&, VectorType&, const MatrixType2&);

}  // end of namespace fses

#include "FSES/syevh3.ixx"

#endif /* LIB_FSES_SYEVH3_HXX */
