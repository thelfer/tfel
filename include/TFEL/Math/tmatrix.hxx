/*!
 * \file   include/TFEL/Math/tmatrix.hxx
 * \brief
 * \author Thomas Helfer
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef TFEL_MATH_TINY_MATRIX_HXX
#define TFEL_MATH_TINY_MATRIX_HXX 1

#include <cstddef>
#include <type_traits>
#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/TypeTraits/IsAssignableTo.hxx"
#include "TFEL/Math/Forward/qt.hxx"
#include "TFEL/Math/General/MathObjectTraits.hxx"
#include "TFEL/Math/General/BasicOperations.hxx"
#include "TFEL/Math/General/EmptyRunTimeProperties.hxx"
#include "TFEL/Math/General/DerivativeType.hxx"
#include "TFEL/Math/Array/GenericFixedSizeArray.hxx"
#include "TFEL/Math/Array/View.hxx"
#include "TFEL/Math/Matrix/MatrixConcept.hxx"
#include "TFEL/Math/Matrix/MatrixConceptOperations.hxx"
#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/power.hxx"
#include "TFEL/Math/General/DerivativeType.hxx"
#include "TFEL/Math/Forward/tmatrix.hxx"

namespace tfel::math::internals {

  /*!
   * \brief a metafunction returning a view type on a derivative in tiny
   * matrix ordered of M columns.
   * \tparam M: number of columns of the tiny matrix
   * \tparam FunctionType: function type.
   * \tparam VariableType: variable type.
   */
  template <bool is_function_type_scalar,
            bool is_variable_type_scalar,
            unsigned short M,
            typename FunctionType,
            typename VariableType>
  struct BuildDerivativeViewFromTinyMatrixImplementation {
    //! \brief a simple alias
    using FunctionIndexingPolicy = typename FunctionType::indexing_policy;
    //! \brief a simple alias
    using VariableIndexingPolicy = typename VariableType::indexing_policy;
    //! \brief a simple alias
    using DerivativeViewIndexingPolicy =
        tfel::math::FixedSizeIndexingPoliciesCartesianProduct<
            FunctionIndexingPolicy,
            VariableIndexingPolicy,
            M>;
    //! \brief result of the metafunction
    using type = tfel::math::View<
        tfel::math::derivative_type<FunctionType, VariableType>,
        DerivativeViewIndexingPolicy>;
  };  // end of BuildDerivativeViewFromTinyMatrixImplementation

  /*!
   * \brief partial specialisation when `FunctionType` is a scalar type.
   * matrix ordered in row major format of M columns.
   * \tparam M: number of columns of the tiny matrix
   * \tparam FunctionType: function type.
   * \tparam VariableType: variable type.
   */
  template <unsigned short M, typename FunctionType, typename VariableType>
  struct BuildDerivativeViewFromTinyMatrixImplementation<true,
                                                         false,
                                                         M,
                                                         FunctionType,
                                                         VariableType> {
    //! \brief a simple alias
    using FunctionIndexingPolicy = ScalarIndexingPolicy<
        index_type<tfel::math::derivative_type<FunctionType, VariableType>>>;
    //! \brief a simple alias
    using VariableIndexingPolicy = typename VariableType::indexing_policy;
    //! \brief a simple alias
    using DerivativeViewIndexingPolicy =
        tfel::math::FixedSizeIndexingPoliciesCartesianProduct<
            FunctionIndexingPolicy,
            VariableIndexingPolicy,
            M>;
    //! \brief result of the metafunction
    using type = tfel::math::View<
        tfel::math::derivative_type<FunctionType, VariableType>,
        DerivativeViewIndexingPolicy>;
  };  // end of BuildDerivativeViewFromTinyMatrixImplementation

  /*!
   * \brief partial specialisation when `VariableType` is a scalar type.
   * matrix ordered in row major format of M columns.
   * \tparam M: number of columns of the tiny matrix
   * \tparam FunctionType: function type.
   * \tparam VariableType: variable type.
   */
  template <unsigned short M, typename FunctionType, typename VariableType>
  struct BuildDerivativeViewFromTinyMatrixImplementation<false,
                                                         true,
                                                         M,
                                                         FunctionType,
                                                         VariableType> {
    //! \brief a simple alias
    using FunctionIndexingPolicy = typename FunctionType::indexing_policy;
    //! \brief a simple alias
    using VariableIndexingPolicy = ScalarIndexingPolicy<
        index_type<tfel::math::derivative_type<FunctionType, VariableType>>>;
    //! \brief a simple alias
    using DerivativeViewIndexingPolicy =
        tfel::math::FixedSizeIndexingPoliciesCartesianProduct<
            FunctionIndexingPolicy,
            VariableIndexingPolicy,
            M>;
    //! \brief result of the metafunction
    using type = tfel::math::View<
        tfel::math::derivative_type<FunctionType, VariableType>,
        DerivativeViewIndexingPolicy>;
  };  // end of BuildDerivativeViewFromTinyMatrixImplementation

  /*!
   * \partial specialisation when both the `FunctionType` and the
   * `VariableType` are scalar types.
   * \tparam M: number of columns of the tiny matrix
   * \tparam FunctionType: function type.
   * \tparam VariableType: variable type.
   */
  template <unsigned short M, typename FunctionType, typename VariableType>
  struct BuildDerivativeViewFromTinyMatrixImplementation<true,
                                                         true,
                                                         M,
                                                         FunctionType,
                                                         VariableType> {
   private:
    //! \brief a simple alias
    using DerivativeType =
        tfel::math::derivative_type<FunctionType, VariableType>;

   public:
    //! \brief result of the metafunction
    using type = std::conditional_t<
        tfel::math::isQuantity<DerivativeType>(),
        typename tfel::math::MakeQuantityReferenceType<DerivativeType>::type,
        DerivativeType&>;
  };  // end of BuildDerivativeViewFromTinyMatrixImplementation

  /*!
   * \brief a metafunction returning a view type on a derivative in tiny
   * matrix ordered in row major format of M columns.
   * \tparam M: number of columns of the tiny matrix
   * \tparam FunctionType: function type.
   * \tparam VariableType: variable type.
   */
  template <unsigned short M, typename FunctionType, typename VariableType>
  struct BuildDerivativeViewFromTinyMatrix {
    //! \brief result
    using type = typename BuildDerivativeViewFromTinyMatrixImplementation<
        tfel::math::isScalar<FunctionType>(),
        tfel::math::isScalar<VariableType>(),
        M,
        FunctionType,
        VariableType>::type;
  };  // end of struct BuildDerivativeViewFromTinyMatrix

}  // end of namespace tfel::math::internals

namespace tfel::math {

  /*!
   * \brief partial specialisation of the `DerivativeTypeDispatcher`
   * metafunction.
   */
  template <unsigned short N,
            unsigned short M,
            typename TMatrixNumericType,
            typename ScalarType>
  struct DerivativeTypeDispatcher<MatrixTag,
                                  ScalarTag,
                                  tmatrix<N, M, TMatrixNumericType>,
                                  ScalarType> {
    static_assert(isScalar<ScalarType>(),
                  "template argument ScalarType is not a scalar");
    static_assert(isScalar<TMatrixNumericType>(),
                  "template argument TMatrixNumericType is not a scalar");
    //! \brief result
    using type = tmatrix<N, M, derivative_type<TMatrixNumericType, ScalarType>>;
  };  // end of struct DerivativeTypeDispatcher
  /*!
   * \brief partial specialisation of the `DerivativeTypeDispatcher`
   * metafunction.
   */
  template <typename ScalarType,
            unsigned short N,
            unsigned short M,
            typename TMatrixNumericType>
  struct DerivativeTypeDispatcher<ScalarTag,
                                  MatrixTag,
                                  ScalarType,
                                  tmatrix<N, M, TMatrixNumericType>> {
    static_assert(isScalar<ScalarType>(),
                  "template argument ScalarType is not a scalar");
    static_assert(isScalar<TMatrixNumericType>(),
                  "template argument TMatrixNumericType is not a scalar");
    //! \brief result
    using type = tmatrix<N, M, derivative_type<ScalarType, TMatrixNumericType>>;
  };  // end of struct DerivativeTypeDispatcher

  /*!
   * \brief partial specialisation of the `DerivativeTypeDispatcher`
   * metafunction.
   */
  template <unsigned short N1,
            typename TVectorNumericTypeType1,
            unsigned short N2,
            typename TVectorNumericTypeType2>
  struct DerivativeTypeDispatcher<VectorTag,
                                  VectorTag,
                                  tvector<N1, TVectorNumericTypeType1>,
                                  tvector<N2, TVectorNumericTypeType2>> {
    static_assert(isScalar<TVectorNumericTypeType1>(),
                  "the first tensor type does not hold a scalar");
    static_assert(isScalar<TVectorNumericTypeType2>(),
                  "the second tensor type does not hold a scalar");
    //! \brief result
    using type = tmatrix<
        N1,
        N2,
        derivative_type<TVectorNumericTypeType1, TVectorNumericTypeType2>>;
  };  // end of struct DerivativeTypeDispatcher

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
    TFEL_HOST_DEVICE static constexpr auto Id();
    // inheriting GenericFixedSizeArray' access operators
    using GenericFixedSizeArrayBase::operator[];
    using GenericFixedSizeArrayBase::operator();
    //! return the number of columns of the matrix
    TFEL_HOST_DEVICE constexpr unsigned short getNbCols() const;
    //! return the number of rows of the matrix
    TFEL_HOST_DEVICE constexpr unsigned short getNbRows() const;
    /*!
     * \return a view of a row of this matrix
     * \param[in] I : row index
     */
    template <unsigned short I>
    TFEL_HOST_DEVICE constexpr auto row_view();
    /*!
     * \return a view of a slice of row of this matrix
     * \param[in] I : row index
     * \param[in] J : starting index in the row
     * \param[in] K : size of the slice
     */
    template <unsigned short I, unsigned short J, unsigned short K>
    TFEL_HOST_DEVICE constexpr auto row_view();
    /*!
     * \return a view of a row of this matrix
     * \param[in] I : row index
     */
    template <unsigned short I>
    TFEL_HOST_DEVICE constexpr auto row_view() const;
    /*!
     * \return a view of a slice of row of this matrix
     * \param[in] I : row index
     * \param[in] J : starting index in the row
     * \param[in] K : size of the slice
     */
    template <unsigned short I, unsigned short J, unsigned short K>
    TFEL_HOST_DEVICE constexpr auto row_view() const;
    /*!
     * \return a view of a column of this matrix
     * \param[in] I : column index
     */
    template <unsigned short I>
    TFEL_HOST_DEVICE constexpr auto column_view();
    /*!
     * \return a view of a slice of column of this matrix
     * \param[in] I : column index
     * \param[in] J : starting index in the column
     * \param[in] K : size of the slice
     */
    template <unsigned short I, unsigned short J, unsigned short K>
    TFEL_HOST_DEVICE constexpr auto column_view();
    /*!
     * \return a view of a slice of column of this matrix
     * \param[in] I : column index
     * \param[in] J : starting index in the column
     * \param[in] K : size of the slice
     */
    template <unsigned short I, unsigned short J, unsigned short K>
    TFEL_HOST_DEVICE constexpr auto column_view() const;
    /*!
     * \return a view of a column of this matrix
     * \param[in] I : column index
     */
    template <unsigned short I>
    TFEL_HOST_DEVICE constexpr auto column_view() const;
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
    TFEL_HOST_DEVICE constexpr auto submatrix_view();
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
    TFEL_HOST_DEVICE constexpr auto submatrix_view() const;

    TFEL_HOST_DEVICE constexpr auto max() const;

    TFEL_HOST_DEVICE constexpr auto abs_max() const;

    TFEL_HOST_DEVICE constexpr void swap_rows(const unsigned short i,
                                              const unsigned short j);

    /*!
     * copy the N*Mth elements following this argument.
     * \param const InputIterator, an iterator to the first element
     * to be copied.
     */
    template <typename InputIterator>
    TFEL_HOST_DEVICE constexpr void copy(const InputIterator);
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
  TFEL_HOST_DEVICE constexpr tmatrix<M, N, T> transpose(
      const tmatrix<N, M, T>&);

  template <typename T, typename Operation>
  TFEL_HOST_DEVICE constexpr auto det(const Expr<tmatrix<2, 2, T>, Operation>&);

  template <typename T>
  TFEL_HOST_DEVICE constexpr auto det(const tmatrix<2, 2, T>&);

  template <typename T, typename Operation>
  TFEL_HOST_DEVICE constexpr auto det(const Expr<tmatrix<3, 3, T>, Operation>&);

  template <typename T>
  TFEL_HOST_DEVICE constexpr auto det(const tmatrix<3, 3, T>&);

  /*!
   * \brief a simple alias to a view type on a derivative in tiny
   * matrix of M columns.
   * \tparam M: number of columns of the matrix.
   * \tparam FunctionType: function type.
   * \tparam VariableType: variable type.
   */
  //! \brief a simple alias
  template <unsigned short M, typename FunctionType, typename VariableType>
  using derivative_view_from_tiny_matrix = typename tfel::math::internals::
      BuildDerivativeViewFromTinyMatrix<M, FunctionType, VariableType>::type;

  /*!
   * \brief an helper function to create a derivative view from a tiny matrix
   * \tparam I: row index.
   * \tparam J: column index.
   * \tparam FunctionType: function type.
   * \tparam VariableType: variable type.
   * \tparam N: number of rows of the matrix.
   * \tparam M: number of columns of the matrix.
   */
  template <unsigned short I,
            unsigned short J,
            typename FunctionType,
            typename VariableType,
            unsigned short N,
            unsigned short M>
  TFEL_HOST_DEVICE constexpr derivative_view_from_tiny_matrix<M,
                                                              FunctionType,
                                                              VariableType>
  map_derivative(
      tmatrix<N,
              M,
              base_type<
                  numeric_type<derivative_type<FunctionType, VariableType>>>>&);

  /*!
   * \brief an helper function to create a derivative view from a tiny matrix
   * \tparam FunctionType: function type.
   * \tparam VariableType: variable type.
   * \tparam N: number of rows of the matrix.
   * \tparam M: number of columns of the matrix.
   * \param[in] I: row index.
   * \param[in] J: column index.
   */
  template <typename FunctionType,
            typename VariableType,
            unsigned short N,
            unsigned short M>
  TFEL_HOST_DEVICE constexpr derivative_view_from_tiny_matrix<M,
                                                              FunctionType,
                                                              VariableType>
  map_derivative(
      tmatrix<N,
              M,
              base_type<
                  numeric_type<derivative_type<FunctionType, VariableType>>>>&,
      const unsigned short,
      const unsigned short);

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
