/*!
 * \file   include/FSES/syev2.hxx
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

#ifndef LIB_FSES_SYEV2_HXX
#define LIB_FSES_SYEV2_HXX

namespace fses {

  // ----------------------------------------------------------------------------
  // Calculates the eigensystem of a real symmetric 2x2 matrix
  //    [ A  B ]
  //    [ B  C ]
  // in the form
  //    [ A  B ]  =  [ cs  -sn ] [ rt1   0  ] [  cs  sn ]
  //    [ B  C ]     [ sn   cs ] [  0   rt2 ] [ -sn  cs ]
  // where rt1 >= rt2. Note that this convention is different from the one used
  // in the LAPACK routine DLAEV2, where |rt1| >= |rt2|.
  // ----------------------------------------------------------------------------
  template <typename real>
  inline void syev2(real &rt1,
                    real &rt2,
                    real &cs,
                    real &sn,
                    const real A,
                    const real B,
                    const real C);

}  // end of namespace fses

#include "FSES/syev2.ixx"

#endif /* LIB_FSES_SYEV2_HXX */
