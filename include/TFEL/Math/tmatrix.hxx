/*!
 * \file   include/TFEL/Math/tmatrix.hxx
 * \brief
 * \author Thomas Helfer
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef TFEL_MATH_TINY_MATRIX_HXX
#define TFEL_MATH_TINY_MATRIX_HXX 1

#include <cstddef>
#include <type_traits>
#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/TypeTraits/IsAssignableTo.hxx"
#include "TFEL/Math/fsarray.hxx"
#include "TFEL/Math/General/MathObjectTraits.hxx"
#include "TFEL/Math/General/BasicOperations.hxx"
#include "TFEL/Math/General/EmptyRunTimeProperties.hxx"
#include "TFEL/Math/Matrix/MatrixConcept.hxx"
#include "TFEL/Math/Matrix/MatrixConceptOperations.hxx"
#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/power.hxx"
#include "TFEL/Math/Forward/tmatrix.hxx"

namespace tfel::math {

  // forward declaration
  template <unsigned short N,
            unsigned short M,
            unsigned short I,
            unsigned short J,
            unsigned short K,
            typename T>
  struct tmatrix_row_view;

  // forward declaration
  template <unsigned short N,
            unsigned short M,
            unsigned short I,
            unsigned short J,
            unsigned short K,
            typename T>
  struct tmatrix_const_row_view;

  // forward declaration
  template <unsigned short N,
            unsigned short M,
            unsigned short I,
            unsigned short J,
            unsigned short K,
            typename T>
  struct tmatrix_column_view;

  // forward declaration
  template <unsigned short N,
            unsigned short M,
            unsigned short I,
            unsigned short J,
            unsigned short K,
            typename T>
  struct tmatrix_const_column_view;

  // forward declaration
  template <unsigned short N,
            unsigned short M,
            unsigned short I,
            unsigned short J,
            unsigned short R,
            unsigned short C,
            typename T>
  struct tmatrix_submatrix_view;

  // forward declaration
  template <unsigned short N,
            unsigned short M,
            unsigned short I,
            unsigned short J,
            unsigned short R,
            unsigned short C,
            typename T>
  struct tmatrix_const_submatrix_view;

  /*!
   * \brief a base for tmatrix or classes acting like tmatrix.
   */
  template <typename Child, unsigned short N, unsigned short M, typename T>
  struct tmatrix_base {
    // Assignement operator
    /*
     * \param expr : a matrix expression.
     * \return Child& a reference to this.
     * \pre T2 must be assignable to a T.
     */
    template <typename T2, typename Operation>
    TFEL_MATH_INLINE std::enable_if_t<isAssignableTo<T2, T>(), Child&>
    operator=(const Expr<tmatrix<N, M, T2>, Operation>&);
    // Assignement operator
    template <typename T2>
    TFEL_MATH_INLINE std::enable_if_t<isAssignableTo<T2, T>(), Child&>
    operator=(const tmatrix<N, M, T2>&);
    // Assignement operator
    /*
     * \param expr : a matrix expression.
     * \return Child& a reference to this.
     * \pre T2 must be assignable to a T.
     */
    template <typename T2, typename Operation>
    TFEL_MATH_INLINE std::enable_if_t<isAssignableTo<T2, T>(), Child&>
    operator+=(const Expr<tmatrix<N, M, T2>, Operation>&);
    // Assignement operator
    template <typename T2>
    TFEL_MATH_INLINE std::enable_if_t<isAssignableTo<T2, T>(), Child&>
    operator+=(const tmatrix<N, M, T2>&);
    // Assignement operator
    /*
     * \param expr : a matrix expression.
     * \return Child& a reference to this.
     * \pre T2 must be assignable to a T.
     */
    template <typename T2, typename Operation>
    TFEL_MATH_INLINE std::enable_if_t<isAssignableTo<T2, T>(), Child&>
    operator-=(const Expr<tmatrix<N, M, T2>, Operation>&);
    // Assignement operator
    template <typename T2>
    TFEL_MATH_INLINE std::enable_if_t<isAssignableTo<T2, T>(), Child&>
    operator-=(const tmatrix<N, M, T2>&);
    /*!
     * operator*=
     */
    template <typename T2>
    TFEL_MATH_INLINE
        std::enable_if_t<isScalar<T2>() &&
                             std::is_same<result_type<T, T2, OpMult>, T>::value,
                         Child&>
        operator*=(const T2);
    /*!
     * operator/=
     */
    template <typename T2>
    TFEL_MATH_INLINE
        std::enable_if_t<isScalar<T2>() &&
                             std::is_same<result_type<T, T2, OpMult>, T>::value,
                         Child&>
        operator/=(const T2);
  };

  template <unsigned short N, unsigned short M, typename ValueType = double>
  struct tmatrix
      : MatrixConcept<tmatrix<N, M, ValueType>>,
        GenericFixedSizeArray<tmatrix<N, M, ValueType>,
                              FixedSizeRowMajorMatrixPolicy<N, M, ValueType>> {
    static_assert((N != 0) && (M != 0));
    //! \brief a simple alias
    using GenericFixedSizeArrayBase =
        GenericFixedSizeArray<tmatrix<N, M, ValueType>,
                              FixedSizeRowMajorMatrixPolicy<N, M, ValueType>>;
    //
    TFEL_MATH_FIXED_SIZE_ARRAY_DEFAULT_METHODS(tmatrix,
                                               GenericFixedSizeArrayBase);
    //! \return the identity matrix
    static tmatrix<N, N, ValueType> Id();
    // inheriting GenericFixedSizeArray' access operators
    using GenericFixedSizeArrayBase::operator[];
    using GenericFixedSizeArrayBase::operator();
    //! return the number of columns of the matrix
    constexpr unsigned short getNbCols() const;
    //! return the number of rows of the matrix
    constexpr unsigned short getNbRows() const;
    /*!
     * \return a view of a row of this matrix
     * \param[in] I : row index
     */
    template <unsigned short I>
    tmatrix_row_view<N, M, I, 0, M, ValueType> row_view();
    /*!
     * \return a view of a slice of row of this matrix
     * \param[in] I : row index
     * \param[in] J : starting index in the row
     * \param[in] K : size of the slice
     */
    template <unsigned short I, unsigned short J, unsigned short K>
    tmatrix_row_view<N, M, I, J, K, ValueType> row_view();
    /*!
     * \return a view of a row of this matrix
     * \param[in] I : row index
     */
    template <unsigned short I>
    tmatrix_const_row_view<N, M, I, 0, M, ValueType> row_view() const;
    /*!
     * \return a view of a slice of row of this matrix
     * \param[in] I : row index
     * \param[in] J : starting index in the row
     * \param[in] K : size of the slice
     */
    template <unsigned short I, unsigned short J, unsigned short K>
    tmatrix_const_row_view<N, M, I, J, K, ValueType> row_view() const;
    /*!
     * \return a view of a column of this matrix
     * \param[in] I : column index
     */
    template <unsigned short I>
    tmatrix_column_view<N, M, I, 0, N, ValueType> column_view();
    /*!
     * \return a view of a slice of column of this matrix
     * \param[in] I : column index
     * \param[in] J : starting index in the column
     * \param[in] K : size of the slice
     */
    template <unsigned short I, unsigned short J, unsigned short K>
    tmatrix_column_view<N, M, I, J, K, ValueType> column_view();
    /*!
     * \return a view of a slice of column of this matrix
     * \param[in] I : column index
     * \param[in] J : starting index in the column
     * \param[in] K : size of the slice
     */
    template <unsigned short I, unsigned short J, unsigned short K>
    tmatrix_const_column_view<N, M, I, J, K, ValueType> column_view() const;
    /*!
     * \return a view of a column of this matrix
     * \param[in] I : column index
     */
    template <unsigned short I>
    tmatrix_const_column_view<N, M, I, 0, N, ValueType> column_view() const;
    /*!
     * \return a view of a sub matrix of this matrix
     * \param[in] I : submatrix row    starting index
     * \param[in] J : submatrix column starting index
     * \param[in] R : number of rows
     * \param[in] C : number of columns
     */
    template <unsigned short I,
              unsigned short J,
              unsigned short R,
              unsigned short C>
    tmatrix_submatrix_view<N, M, I, J, R, C, ValueType> submatrix_view();
    /*!
     * \return a view of a sub matrix of this matrix
     * \param[in] I : submatrix row    starting index
     * \param[in] J : submatrix column starting index
     * \param[in] R : number of rows
     * \param[in] C : number of columns
     */
    template <unsigned short I,
              unsigned short J,
              unsigned short R,
              unsigned short C>
    tmatrix_const_submatrix_view<N, M, I, J, R, C, ValueType> submatrix_view() const;

    TFEL_MATH_INLINE2
    ValueType max() const;

    TFEL_MATH_INLINE2
    ValueType abs_max() const;

    TFEL_MATH_INLINE2
    void swap_rows(const unsigned short i, const unsigned short j);

    /*!
     * copy the N*Mth elements following this argument.
     * \param const InputIterator, an iterator to the first element
     * to be copied.
     */
    template <typename InputIterator>
    TFEL_MATH_INLINE2 void copy(const InputIterator);
  };

  /*!
   * \return the row of the matrix with the given number;
   * \param[in] m: matrix
   * \param[in] c: row number
   */
  template <unsigned short N, unsigned short M, typename T>
  tvector<M, T> getRow(const tmatrix<N, M, T>& m, const unsigned short);
  /*!
   * \return the column of the matrix with the given number;
   * \param[in] m: matrix
   * \param[in] c: column number
   */
  template <unsigned short N, unsigned short M, typename T>
  tvector<N, T> getColumn(const tmatrix<N, M, T>& m, const unsigned short);

  // Transpose
  template <unsigned short N, unsigned short M, typename T>
  TFEL_MATH_INLINE2 tmatrix<M, N, T> transpose(const tmatrix<N, M, T>&);

  template <typename T, typename Operation>
  TFEL_MATH_INLINE typename ComputeUnaryResult<T, Power<3>>::Result det(
      const Expr<tmatrix<2, 2, T>, Operation>&);

  template <typename T>
  TFEL_MATH_INLINE typename ComputeUnaryResult<T, Power<3>>::Result det(
      const tmatrix<2, 2, T>&);

  template <typename T, typename Operation>
  TFEL_MATH_INLINE typename ComputeUnaryResult<T, Power<3>>::Result det(
      const Expr<tmatrix<3, 3, T>, Operation>&);

  template <typename T>
  TFEL_MATH_INLINE typename ComputeUnaryResult<T, Power<3>>::Result det(
      const tmatrix<3, 3, T>&);

  }  // end of namespace tfel::math

  namespace tfel::typetraits {
    template <unsigned short N, unsigned short M, typename T2, typename T>
    struct IsAssignableTo<tfel::math::tmatrix<N, M, T2>,
                          tfel::math::tmatrix<N, M, T>> {
      //! \brief result
      static constexpr bool cond = isAssignableTo<T2, T>();
    };

  }  // end of namespace tfel::typetraits

#include "TFEL/Math/Matrix/tmatrix_row_view.hxx"
#include "TFEL/Math/Matrix/tmatrix_const_row_view.hxx"
#include "TFEL/Math/Matrix/tmatrix_column_view.hxx"
#include "TFEL/Math/Matrix/tmatrix_const_column_view.hxx"
#include "TFEL/Math/Matrix/tmatrix_submatrix_view.hxx"
#include "TFEL/Math/Matrix/tmatrix_const_submatrix_view.hxx"
#include "TFEL/Math/Matrix/tmatrix.ixx"
#include "TFEL/Math/Matrix/tmatrixResultType.hxx"

#endif /* TFEL_MATH_TINY_MATRIX_HXX */
