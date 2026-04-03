/*!
 * \file   include/FSES/std_array.hxx
 * \brief
 * \author Thomas Helfer
 * \date   03 janv. 2017
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

#ifndef LIB_FSES_STD_ARRAY_HXX
#define LIB_FSES_STD_ARRAY_HXX

#include <array>
#include "FSES/Utilities.hxx"

namespace fses {

  /*!
   * \brief partial specialisation for std::array
   * \tparam real: numeric type
   * \tparam N: size
   */
  template <typename real, std::size_t N>
  struct GetVectorNumericType<std::array<real, N>> {
    //! the result
    using type = real;
  };  // end of GetVectorNumericType<std::array<real,N>>
  /*!
   * \brief partial specialisation for std::array
   * \tparam real: numeric type
   * \tparam N: size
   */
  template <typename real, std::size_t N>
  struct GetVectorNumericType<const std::array<real, N>> {
    //! the result
    using type = real;
  };  // end of GetVectorNumericType<std::array<real,N>>

  template <typename T, size_t N>
  struct VectorAccess<std::array<T, N>> {
    template <typename IndexType>
    static T& exe(std::array<T, N>& v, const IndexType& i) {
      return v[i];
    }
    template <typename IndexType>
    static const T& exe(const std::array<T, N>& v, const IndexType& i) {
      return v[i];
    }
  };

  /*!
   * \brief partial specialisation for std::array
   * \tparam real: numeric type
   * \tparam N: number of rows
   * \tparam M: of columns
   */
  template <typename real, std::size_t N, std::size_t M>
  struct GetMatrixNumericType<std::array<std::array<real, M>, N>> {
    //! the result
    using type = real;
  };  // end of GetMatrixNumericType<std::array<std::array<real,M>,N>>
  /*!
   * \brief partial specialisation for std::array
   * \tparam real: numeric type
   * \tparam N: number of rows
   * \tparam M: of columns
   */
  template <typename real, std::size_t N, std::size_t M>
  struct GetMatrixNumericType<const std::array<std::array<real, M>, N>> {
    //! the result
    using type = real;
  };  // end of GetMatrixNumericType<std::array<std::array<real,M>,N>>

  template <typename T, std::size_t N, std::size_t M>
  struct MatrixAccess<std::array<std::array<T, M>, N>> {
    template <typename IndexType>
    static T& exe(std::array<std::array<T, M>, N>& v,
                  const IndexType& i,
                  const IndexType& j) {
      return v[i][j];
    }
    template <typename IndexType>
    static const T& exe(const std::array<std::array<T, M>, N>& v,
                        const IndexType& i,
                        const IndexType& j) {
      return v[i][j];
    }
  };

}  // end of namespace fses

#endif /* LIB_FSES_STD_ARRAY_HXX */
