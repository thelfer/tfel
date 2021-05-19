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
#include "TFEL/Math/General/MathObjectTraits.hxx"
#include "TFEL/Math/General/BasicOperations.hxx"
#include "TFEL/Math/General/EmptyRunTimeProperties.hxx"
#include "TFEL/Math/Array/GenericFixedSizeArray.hxx"
#include "TFEL/Math/Array/View.hxx"
#include "TFEL/Math/Matrix/MatrixConcept.hxx"
#include "TFEL/Math/Matrix/MatrixConceptOperations.hxx"
#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/power.hxx"
#include "TFEL/Math/Forward/tmatrix.hxx"

namespace tfel::math {

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
    TFEL_MATH_INLINE std::enable_if_t<
        isScalar<T2>() && std::is_same<result_type<T, T2, OpMult>, T>::value,
        Child&>
    operator*=(const T2);
    /*!
     * operator/=
     */
    template <typename T2>
    TFEL_MATH_INLINE std::enable_if_t<
        isScalar<T2>() && std::is_same<result_type<T, T2, OpMult>, T>::value,
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
    constexpr auto row_view();
    /*!
     * \return a view of a slice of row of this matrix
     * \param[in] I : row index
     * \param[in] J : starting index in the row
     * \param[in] K : size of the slice
     */
    template <unsigned short I, unsigned short J, unsigned short K>
    constexpr auto row_view();
    /*!
     * \return a view of a row of this matrix
     * \param[in] I : row index
     */
    template <unsigned short I>
    constexpr auto row_view() const;
    /*!
     * \return a view of a slice of row of this matrix
     * \param[in] I : row index
     * \param[in] J : starting index in the row
     * \param[in] K : size of the slice
     */
    template <unsigned short I, unsigned short J, unsigned short K>
    constexpr auto row_view() const;
    /*!
     * \return a view of a column of this matrix
     * \param[in] I : column index
     */
    template <unsigned short I>
    constexpr auto column_view();
    /*!
     * \return a view of a slice of column of this matrix
     * \param[in] I : column index
     * \param[in] J : starting index in the column
     * \param[in] K : size of the slice
     */
    template <unsigned short I, unsigned short J, unsigned short K>
    constexpr auto column_view();
    /*!
     * \return a view of a slice of column of this matrix
     * \param[in] I : column index
     * \param[in] J : starting index in the column
     * \param[in] K : size of the slice
     */
    template <unsigned short I, unsigned short J, unsigned short K>
    constexpr auto column_view() const;
    /*!
     * \return a view of a column of this matrix
     * \param[in] I : column index
     */
    template <unsigned short I>
    constexpr auto column_view() const;
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
    auto submatrix_view();
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
    auto submatrix_view() const;

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
   * \brief a simple alias for backward compatibility
   * \tparam N: number of rows
   * \tparam M: number of columns
   * \tparam T: value type
   */
  template <unsigned short N, unsigned short M, typename T>
  using TMatrixView = View<tmatrix<N, M, T>>;
  /*!
   * \brief a simple alias for backward compatibility
   * \tparam N: number of rows
   * \tparam M: number of columns
   * \tparam T: value type
   */
  template <unsigned short N, unsigned short M, typename T>
  using ConstTMatrixView = ConstView<tmatrix<N, M, T>>;

  // Transpose
  template <unsigned short N, unsigned short M, typename T>
  TFEL_MATH_INLINE2 tmatrix<M, N, T> transpose(const tmatrix<N, M, T>&);

  template <typename T, typename Operation>
  TFEL_MATH_INLINE auto det(
      const Expr<tmatrix<2, 2, T>, Operation>&);

  template <typename T>
  TFEL_MATH_INLINE auto det(
      const tmatrix<2, 2, T>&);

  template <typename T, typename Operation>
  TFEL_MATH_INLINE auto det(
      const Expr<tmatrix<3, 3, T>, Operation>&);

  template <typename T>
  TFEL_MATH_INLINE auto det(const tmatrix<3, 3, T>&);

}  // end of namespace tfel::math

namespace tfel::typetraits {

  //! \brief specialisation of `IsAssignableTo` for tiny matrices
  template <unsigned short N, unsigned short M, typename T2, typename T>
  struct IsAssignableTo<tfel::math::tmatrix<N, M, T2>,
                        tfel::math::tmatrix<N, M, T>> {
    //! \brief result
    static constexpr bool cond = isAssignableTo<T2, T>();
  };

}  // end of namespace tfel::typetraits

#include "TFEL/Math/Matrix/tmatrix.ixx"
#include "TFEL/Math/Matrix/tmatrixResultType.hxx"

#endif /* TFEL_MATH_TINY_MATRIX_HXX */
