/*!
 * \file   include/FSES/Utilities.hxx
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

#ifndef LIB_FSES_UTILITIES_HXX
#define LIB_FSES_UTILITIES_HXX

#include <cstdlib>
#include <type_traits>

namespace fses {

  /*!
   * An enumeration of the various eigen values available
   */
  enum struct EigenValuesOrdering {
    //! sort eigenvalues from the lowest to the greatest
    ASCENDING,
    //! sort eigenvalues from the greatest to the lowest
    DESCENDING,
    //! no ordering
    UNSORTED
  };  // end of EigenValuesOrdering

  /*!
   * \return the square of the input
   * \param[in] x: value
   */
  template <typename T>
  inline constexpr T square(const T);

  /*!
   * \brief a metafunction returning the numerical type used by a
   * matrix
   */
  template <typename VectorType>
  struct GetVectorNumericType;
  //! a simple alias
  template <typename VectorType>
  using GetVectorNumericType_t =
      typename GetVectorNumericType<VectorType>::type;
  /*!
   * \brief get values of a vector
   */
  template <typename VectorType, typename IndexType>
  inline GetVectorNumericType_t<VectorType>& at(VectorType&, const IndexType&);
  /*!
   * \brief get values of a vector
   */
  template <typename VectorType, typename IndexType>
  inline const GetVectorNumericType_t<VectorType>& at(const VectorType&,
                                                      const IndexType&);
  /*!
   * \brief a metafunction returning the numerical type used by a
   * matrix
   */
  template <typename MatrixType>
  struct GetMatrixNumericType;
  //! a simple alias
  template <typename MatrixType>
  using GetMatrixNumericType_t =
      typename GetMatrixNumericType<MatrixType>::type;
  /*!
   * \brief get values of a matrix
   */
  template <typename MatrixType, typename IndexType>
  inline GetMatrixNumericType_t<MatrixType>& at(MatrixType&,
                                                const IndexType&,
                                                const IndexType&);
  /*!
   * \brief get values of a matrix
   */
  template <typename MatrixType, typename IndexType>
  inline const GetMatrixNumericType_t<MatrixType>& at(const MatrixType&,
                                                      const IndexType&,
                                                      const IndexType&);
  /*!
   * fill the values of a 3x3 matrix
   * \param[out] m: matrix
   * \param[in]  v: value
   */
  template <typename MatrixType, typename NumericType>
  void fill(MatrixType&, const NumericType&);

  template <typename real>
  inline bool is_negligible(const real&, const real&);
  /*!
   * \brief sort the eigenvalues and the eigenvectors
   * \param[in,out] m: eigenvectors, stored as column of a 3x3 matrix
   * \param[in,out] v: eigenvalues
   * \param[in]     o: ordering
   */
  template <typename MatrixType, typename VectorType>
  inline void sort(MatrixType&, VectorType&, const EigenValuesOrdering);

}  // end of namespace fses

#include "FSES/Utilities.ixx"

#endif /* LIB_FSES_UTILITIES_HXX */
