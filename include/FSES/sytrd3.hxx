/*!
 * \file   include/FSES/sytrd3.hxx
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

#ifndef LIB_FSES_SYTRD3_HXX
#define LIB_FSES_SYTRD3_HXX

namespace fses {

  // ----------------------------------------------------------------------------
  // Reduces a symmetric 3x3 matrix to tridiagonal form by applying
  // (unitary) Householder transformations:
  //            [ d[0]  e[0]       ]
  //    A = Q . [ e[0]  d[1]  e[1] ] . Q^T
  //            [       e[1]  d[2] ]
  // The function accesses only the diagonal and upper triangular parts of
  // A. The access is read-only.
  // ---------------------------------------------------------------------------
  template <typename MatrixType,
            typename VectorType,
            typename VectorType2,
            typename MatrixType2>
  inline void sytrd3(MatrixType&,
                     VectorType&,
                     VectorType2&,
                     const MatrixType2&);

}  // end of namespace fses

#include "FSES/sytrd3.ixx"

#endif /* LIB_FSES_SYTRD3_HXX */
