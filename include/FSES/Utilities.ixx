/*!
 * \file   include/FSES/Utilities.ixx
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

#ifndef LIB_FSES_UTILITIES_IXX
#define LIB_FSES_UTILITIES_IXX

#include <array>

namespace fses {

  template <typename T>
  constexpr T square(const T x) {
    return x * x;
  }

  /*!
   * \brief an helper metafunction returning the numerical type used
   * by a vector
   */
  template <typename VectorType>
  struct GetVectorNumericType {
    //! the result
    using type =
        typename std::decay<decltype(std::declval<VectorType>().operator()(
            0))>::type;
  };  // end of struct GetVectorNumericType

  /*!
   * \brief partial specialisation for pointers
   */
  template <typename VectorNumericType>
  struct GetVectorNumericType<VectorNumericType*> {
    //! the result
    using type = VectorNumericType;
  };  // end of struct GetVectorNumericType
  /*!
   * \brief partial specialisation for pointers
   */
  template <typename VectorNumericType, size_t N>
  struct GetVectorNumericType<VectorNumericType[N]> {
    //! the result
    using type = VectorNumericType;
  };  // end of struct GetVectorNumericType

  /*!
   * \brief an helper metafunction returning the numerical type used
   * by a matrix
   */
  template <typename MatrixType>
  struct GetMatrixNumericType {
    //! the result
    using type =
        typename std::decay<decltype(std::declval<MatrixType>().operator()(
            0, 0))>::type;
  };  // end of struct GetMatrixNumericType
  /*!
   * \brief partial specialisation for pointers
   */
  template <typename MatrixNumericType>
  struct GetMatrixNumericType<MatrixNumericType*> {
    //! the result
    using type = MatrixNumericType;
  };  // end of struct GetMatrixNumericType
  /*!
   * \brief partial specialisation for pointers
   */
  template <typename MatrixNumericType, size_t N, size_t M>
  struct GetMatrixNumericType<MatrixNumericType[N][M]> {
    //! the result
    using type = typename std::decay<MatrixNumericType>::type;
  };  // end of struct GetMatrixNumericType
  /*!
   * \brief partial specialisation for pointers
   */
  template <typename MatrixNumericType, size_t N, size_t M>
  struct GetMatrixNumericType<const MatrixNumericType (&)[N][M]> {
    //! the result
    using type = typename std::decay<MatrixNumericType>::type;
  };  // end of struct GetMatrixNumericType

  template <typename VectorType>
  struct VectorAccess {
    template <typename IndexType>
    static GetVectorNumericType_t<VectorType>& exe(VectorType& v,
                                                   const IndexType& i) {
      return v(i);
    }
    template <typename IndexType>
    static const GetVectorNumericType_t<VectorType>& exe(const VectorType& v,
                                                         const IndexType& i) {
      return v(i);
    }
  };

  template <typename T, size_t N>
  struct VectorAccess<T[N]> {
    template <typename IndexType>
    static T& exe(T v[N], const IndexType& i) {
      return v[i];
    }
    template <typename IndexType>
    static const T& exe(const T v[N], const IndexType& i) {
      return v[i];
    }
  };

  template <typename VectorType, typename IndexType>
  GetVectorNumericType_t<VectorType>& at(VectorType& v, const IndexType& i) {
    return VectorAccess<VectorType>::exe(v, i);
  }
  template <typename VectorType, typename IndexType>
  const GetVectorNumericType_t<VectorType>& at(const VectorType& v,
                                               const IndexType& i) {
    return VectorAccess<VectorType>::exe(v, i);
  }

  template <typename MatrixType>
  struct MatrixAccess {
    template <typename IndexType>
    static GetMatrixNumericType_t<MatrixType>& exe(MatrixType& v,
                                                   const IndexType& i,
                                                   const IndexType& j) {
      return v(i, j);
    }
    template <typename IndexType>
    static const GetMatrixNumericType_t<MatrixType>& exe(const MatrixType& v,
                                                         const IndexType& i,
                                                         const IndexType& j) {
      return v(i, j);
    }
  };

  template <typename T, size_t N, size_t M>
  struct MatrixAccess<T[N][M]>{
      template <typename IndexType> static T &
      exe(T v[N][M], const IndexType& i, const IndexType& j){return v[i][j];
}  // namespace fses
template <typename IndexType>
static const T& exe(const T v[N][M], const IndexType& i, const IndexType& j) {
  return v[i][j];
}
}
;

template <typename MatrixType, typename IndexType>
GetMatrixNumericType_t<MatrixType>& at(MatrixType& v,
                                       const IndexType& i,
                                       const IndexType& j) {
  return MatrixAccess<MatrixType>::exe(v, i, j);
}
template <typename MatrixType, typename IndexType>
const GetMatrixNumericType_t<MatrixType>& at(const MatrixType& v,
                                             const IndexType& i,
                                             const IndexType& j) {
  return MatrixAccess<MatrixType>::exe(v, i, j);
}

template <typename MatrixType, typename NumericType>
void fill(MatrixType& m, const NumericType& v) {
  for (int i = 0; i != 3; ++i) {
    for (int j = 0; j != 3; ++j) {
      at(m, i, j) = v;
    }
  }
}  // end of fill

template <typename real>
inline bool is_negligible(const real& x, const real& y) {
  static constexpr real e = std::numeric_limits<real>::epsilon();
  // y+x==y;
  return std::abs(x) < std::abs(y) * e;
}

template <typename MatrixType, typename VectorType>
void sort(MatrixType& m, VectorType& v, const EigenValuesOrdering o) {
  using size_type = unsigned short;
  using real = GetVectorNumericType_t<VectorType>;
  constexpr auto zero = size_type{0};
  constexpr auto one = size_type{1};
  constexpr auto two = size_type{2};
  std::array<size_type, 3u> idx = {zero, one, two};
  const std::array<real, 3u> v2 = {at(v, 0), at(v, 1), at(v, 2)};
  const std::array<std::array<real, 3u>, 3u> m2 = {
      at(m, 0, 0), at(m, 0, 1), at(m, 0, 2), at(m, 1, 0), at(m, 1, 1),
      at(m, 1, 2), at(m, 2, 0), at(m, 2, 1), at(m, 2, 2)};
  if (o == EigenValuesOrdering::ASCENDING) {
    if ((at(v, 0) < at(v, 1)) && (at(v, 1) < at(v, 2))) {
      idx = {zero, one, two};
    } else if ((at(v, 0) < at(v, 2)) && (at(v, 2) < at(v, 1))) {
      idx = {zero, two, one};
    } else if ((at(v, 1) < at(v, 0)) && (at(v, 0) < at(v, 2))) {
      idx = {one, zero, two};
    } else if ((at(v, 1) < at(v, 2)) && (at(v, 2) < at(v, 0))) {
      idx = {one, two, zero};
    } else if ((at(v, 2) < at(v, 0)) && (at(v, 0) < at(v, 1))) {
      idx = {two, zero, one};
    } else {
      // ((at(v,2)<at(v,1))&&(at(v,1)<at(v,0))){
      idx = {two, one, zero};
    }
  } else if (o == EigenValuesOrdering::DESCENDING) {
    if ((at(v, 0) > at(v, 1)) && (at(v, 1) > at(v, 2))) {
      idx = {zero, one, two};
    } else if ((at(v, 0) > at(v, 2)) && (at(v, 2) > at(v, 1))) {
      idx = {zero, two, one};
    } else if ((at(v, 1) > at(v, 0)) && (at(v, 0) > at(v, 2))) {
      idx = {one, zero, two};
    } else if ((at(v, 1) > at(v, 2)) && (at(v, 2) > at(v, 0))) {
      idx = {one, two, zero};
    } else if ((at(v, 2) > at(v, 0)) && (at(v, 0) > at(v, 1))) {
      idx = {two, zero, one};
    } else {
      // ((at(v,2)>at(v,1))&&(at(v,1)>at(v,0))){
      idx = {two, one, zero};
    }
  }
  at(v, 0) = v2[idx[0]];
  at(v, 1) = v2[idx[1]];
  at(v, 2) = v2[idx[2]];
  at(m, 0, 0) = m2[0][idx[0]];
  at(m, 1, 0) = m2[1][idx[0]];
  at(m, 2, 0) = m2[2][idx[0]];
  at(m, 0, 1) = m2[0][idx[1]];
  at(m, 1, 1) = m2[1][idx[1]];
  at(m, 2, 1) = m2[2][idx[1]];
  at(m, 0, 2) = m2[0][idx[2]];
  at(m, 1, 2) = m2[1][idx[2]];
  at(m, 2, 2) = m2[2][idx[2]];
}  // end of sort

}  // end of namespace fses

#endif /* LIB_FSES_UTILITIES_IXX */
