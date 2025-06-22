/*!
 * \file   include/TFEL/Math/Matrix/matrix.ixx
 * \brief
 * \author Thomas Helfer
 * \date   05 oct 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_MATRIX_IXX
#define LIB_TFEL_MATH_MATRIX_IXX

namespace tfel::math {

  template <typename ValueType>
  matrix<ValueType>::matrix(const typename matrix::size_type r,
                            const typename matrix::size_type c,
                            const ValueType& v) {
    this->resize(r, c, v);
  }  // end of matrix

  template <typename ValueType>
  void matrix<ValueType>::resize(const typename matrix::size_type r,
                                 const typename matrix::size_type c,
                                 const ValueType& v) {
    using IndexingPolicy = typename matrix<ValueType>::indexing_policy;
    this->data_values.resize(r * c, v);
    static_cast<IndexingPolicy&>(*this) = IndexingPolicy{r, c};
  }  // end of resize

  template <typename ValueType>
  typename matrix<ValueType>::size_type matrix<ValueType>::getNbRows() const {
    return this->getNumberOfRows();
  }  // end of getNbRows

  template <typename ValueType>
  typename matrix<ValueType>::size_type matrix<ValueType>::getNbCols() const {
    return this->getNumberOfColumns();
  }  // end of getNbCols

  template <typename ValueType>
  void matrix<ValueType>::swap(matrix& m) {
    using IndexingPolicy = typename matrix::indexing_policy;
    this->data_values.swap(m.data_values);
    const auto old = static_cast<IndexingPolicy&>(m);
    static_cast<IndexingPolicy&>(m) = static_cast<IndexingPolicy&>(*this);
    static_cast<IndexingPolicy&>(*this) = old;
  }  // end of swap

  template <typename ValueType>
  matrix<ValueType>::~matrix() noexcept = default;

}  // namespace tfel::math

#endif /* LIB_TFEL_MATH_MATRIX_IXX */
