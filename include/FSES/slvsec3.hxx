/*!
 * \file   include/FSES/slvsec3.hxx
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

#ifndef LIB_FSES_SLVSEC3_HXX
#define LIB_FSES_SLVSEC3_HXX

namespace fses {

  // ----------------------------------------------------------------------------
  // Finds the three roots w_j of the secular equation
  //   f(w_j) = 1 + Sum[ z_i / (d_i - w_j) ]  ==  0.
  // It is assumed that d_0 <= d_1 <= d_2, and that all z_i have the same sign.
  // The arrays P_i will contain the information required for the calculation
  // of the eigenvectors:
  //   P_ij = d_i - w_j.
  // These differences can be obtained with better accuracy from intermediate
  // results.
  // ----------------------------------------------------------------------------
  template <typename MatrixType, typename VectorType, typename real>
  void slvsec3(MatrixType&,
               VectorType&,
               const real d[3],
               const real z[3],
               int i0,
               int i1,
               int i2);

}  // end of namespace fses

#include "FSES/slvsec3.ixx"

#endif /* LIB_FSES_SLVSEC3_HXX */
