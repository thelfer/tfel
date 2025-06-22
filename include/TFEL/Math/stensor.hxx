/*!
 * \file   include/TFEL/Math/stensor.hxx
 * \brief
 * \author Thomas Helfer
 * \date   04 May 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_STENSOR_HXX
#define LIB_TFEL_MATH_STENSOR_HXX

#include <tuple>
#include <cstddef>
#include <iterator>
#include <type_traits>
#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/TypeTraits/IsAssignableTo.hxx"
#include "TFEL/Math/power.hxx"
#include "TFEL/Math/General/MathObjectTraits.hxx"
#include "TFEL/Math/General/BasicOperations.hxx"
#include "TFEL/Math/General/EmptyRunTimeProperties.hxx"
#include "TFEL/Math/General/DerivativeType.hxx"
#include "TFEL/Math/Array/GenericFixedSizeArray.hxx"
#include "TFEL/Math/Array/View.hxx"
#include "TFEL/Math/Vector/VectorConcept.hxx"
#include "TFEL/Math/Matrix/MatrixConcept.hxx"
#include "TFEL/Math/ST2toST2/ST2toST2Concept.hxx"
#include "TFEL/Math/Stensor/StensorConcept.hxx"
#include "TFEL/Math/Stensor/StensorConceptOperations.hxx"
#include "TFEL/Math/Forward/tvector.hxx"
#include "TFEL/Math/Forward/tmatrix.hxx"
#include "TFEL/Math/Forward/stensor.hxx"
#include "TFEL/Math/Forward/st2tost2.hxx"

namespace tfel::math {

  /*!
   * \brief partial specialisation of the `DerivativeTypeDispatcher`
   * metafunction.
   */
  template <StensorConcept StensorType, typename ScalarType>
  struct DerivativeTypeDispatcher<StensorTag,
                                  ScalarTag,
                                  StensorType,
                                  ScalarType> {
    static_assert(isScalar<ScalarType>(), "ScalarType is not a scalar");
    static_assert(isScalar<numeric_type<StensorType>>(),
                  "the symmetric tensor type does not hold a scalar");
    //! \brief result
    using type =
        stensor<getSpaceDimension<StensorType>(),
                derivative_type<numeric_type<StensorType>, ScalarType>>;
  };  // end of struct DerivativeTypeDispatcher
  /*!
   * \brief partial specialisation of the `DerivativeTypeDispatcher`
   * metafunction.
   */
  template <typename ScalarType, StensorConcept StensorType>
  struct DerivativeTypeDispatcher<ScalarTag,
                                  StensorTag,
                                  ScalarType,
                                  StensorType> {
    static_assert(isScalar<ScalarType>(), "ScalarType is not a scalar");
    //! \brief result
    using type =
        stensor<getSpaceDimension<StensorType>(),
                derivative_type<ScalarType, numeric_type<StensorType>>>;
  };  // end of struct DerivativeTypeDispatcher

  //! \brief an helper structure inherited by the `stensor` class
  struct stensor_common {
    //! \brief available eigen solver
    enum EigenSolver {
      //! historical algorithm
      TFELEIGENSOLVER,
      //! non iterative solver from Joachim Kopp,
      FSESANALYTICALEIGENSOLVER,
      //! Jacobi iterative solver from Joachim Kopp
      FSESJACOBIEIGENSOLVER,
      //! QL with implicit shifts iterative solver from Joachim Kopp
      FSESQLEIGENSOLVER,
      //!  Cuppen's Divide & Conquer solver from Joachim Kopp
      FSESCUPPENEIGENSOLVER,
      //!  hybride solver from Joachim Kopp
      FSESHYBRIDEIGENSOLVER,
      //! iterative solver from David Eberly, Geometric Tools
      GTESYMMETRICQREIGENSOLVER,
      //! non iterative solver from Isaac Harari and Uri Albocher,
      HARARIEIGENSOLVER
    };  // end of EigenSolver
    //! \brief available eigen solver
    enum EigenValuesOrdering {
      //! sort eigenvalues from the lowest to the greatest
      ASCENDING,
      //! sort eigenvalues from the greatest to the lowest
      DESCENDING,
      //! no ordering
      UNSORTED
    };  // end of EigenValuesOrdering
  };    // end of struct stensor_common

  template <unsigned short N, typename ValueType>
  struct stensor
      : StensorConceptBase<stensor<N, ValueType>>,
        stensor_common,
        GenericFixedSizeArray<
            stensor<N, ValueType>,
            FixedSizeVectorPolicy<StensorDimeToSize<N>::value, ValueType>> {
    static_assert((N == 1u) || (N == 2u) || (N == 3u));
    //! \brief a simple alias
    using GenericFixedSizeArrayBase = GenericFixedSizeArray<
        stensor<N, ValueType>,
        FixedSizeVectorPolicy<StensorDimeToSize<N>::value, ValueType>>;
    //
    TFEL_MATH_FIXED_SIZE_ARRAY_DEFAULT_METHODS(stensor,
                                               GenericFixedSizeArrayBase);
    // inheriting GenericFixedSizeArray' access operators
    using GenericFixedSizeArrayBase::operator[];
    using GenericFixedSizeArrayBase::operator();
    //! \brief import from external memory location which uses Voigt notations
    //! for strains
    template <typename InputIterator>
    TFEL_HOST_DEVICE constexpr void
    importVoigt(const InputIterator) noexcept requires(
        std::is_same_v<typename std::iterator_traits<InputIterator>::value_type,
                       base_type<ValueType>>);
    //! \brief import from external memory location which uses Voigt notations
    //! for stresses
    template <typename InputIterator>
    TFEL_HOST_DEVICE constexpr void
    importTab(const InputIterator) noexcept requires(
        std::is_same_v<typename std::iterator_traits<InputIterator>::value_type,
                       base_type<ValueType>>);
    //! \brief import values from external memory location
    template <typename InputIterator>
    TFEL_HOST_DEVICE constexpr void
    import(const InputIterator) noexcept requires(
        std::is_same_v<typename std::iterator_traits<InputIterator>::value_type,
                       base_type<ValueType>>);
    /*!
     * \brief export to external memory location using Voigt notations for0
     * stresses
     */
    TFEL_HOST_DEVICE constexpr void  //
    exportTab(base_type<ValueType>* const) const noexcept;
    //! \brief write to external memory location
    TFEL_HOST_DEVICE constexpr void  //
    write(base_type<ValueType>* const) const noexcept;
    /*!
     * compute eigenvalues
     * \tparam     es:  eigen solver
     * \param[out] vp0: first eigen value
     * \param[out] vp1: first eigen value
     * \param[out] vp2: first eigen value
     * \param[in]  b:   refine eigenvalues
     */
    template <EigenSolver = TFELEIGENSOLVER>
    TFEL_HOST_DEVICE TFEL_MATH_INLINE2 void computeEigenValues(
        ValueType&, ValueType&, ValueType&, const bool = false) const;
    /*!
     * compute eigenvalues
     * \tparam     es: eigen solver
     * \param[out] vp: eigen values
     * \param[in]  b:  refine eigenvalues
     */
    template <EigenSolver = TFELEIGENSOLVER>
    TFEL_HOST_DEVICE TFEL_MATH_INLINE2 void computeEigenValues(
        tvector<3u, ValueType>&, const bool = false) const;
    /*!
     * compute eigenvalues
     * \tparam    es: eigen solver
     * \param[in] b:  refine eigenvalues
     * \return eigen values
     */
    template <EigenSolver = TFELEIGENSOLVER>
    TFEL_HOST_DEVICE TFEL_MATH_INLINE2 tvector<3u, ValueType>
    computeEigenValues(const bool = false) const;
    /*!
     * compute eigenvalues
     * \tparam     es:  eigen solver
     * \param[out] vp0: first eigen value
     * \param[out] vp1: first eigen value
     * \param[out] vp2: first eigen value
     * \param[in]  o:   eigenvalues ordering
     * \param[in]  b:   refine eigenvalues
     */
    template <EigenSolver = TFELEIGENSOLVER>
    TFEL_HOST_DEVICE TFEL_MATH_INLINE2 void computeEigenValues(
        ValueType&,
        ValueType&,
        ValueType&,
        const EigenValuesOrdering,
        const bool = false) const;
    /*!
     * compute eigenvalues
     * \tparam     es: eigen solver
     * \param[out] vp: eigen values
     * \param[in]  o:  eigenvalues ordering
     * \param[in]  b:  refine eigenvalues
     */
    template <EigenSolver = TFELEIGENSOLVER>
    TFEL_HOST_DEVICE TFEL_MATH_INLINE2 void computeEigenValues(
        tvector<3u, ValueType>&,
        const EigenValuesOrdering,
        const bool = false) const;
    /*!
     * compute eigenvalues
     * \tparam    es: eigen solver
     * \param[in] o:  eigenvalues ordering
     * \param[in] b:  refine eigenvalues
     * \return eigen values
     */
    template <EigenSolver = TFELEIGENSOLVER>
    TFEL_HOST_DEVICE TFEL_MATH_INLINE2 tvector<3u, ValueType>
    computeEigenValues(const EigenValuesOrdering, const bool = false) const;
    /*!
     * compute eigenvectors and eigenvalues
     * \tparam    es: eigen solver
     * \param[in] b:  refine eigenvalues
     */
    template <EigenSolver = TFELEIGENSOLVER>
    TFEL_HOST_DEVICE TFEL_MATH_INLINE2
        std::tuple<tvector<3u, ValueType>, rotation_matrix<ValueType>>
        computeEigenVectors(const bool = false) const;
    /*!
     * compute eigenvectors and eigenvalues
     * \tparam     es: eigen solver
     * \param[out] vp : eigenvalues
     * \param[out] m  : rotation matrix
     * \param[in]  b  : refine eigenvalues
     */
    template <EigenSolver = TFELEIGENSOLVER>
    TFEL_HOST_DEVICE TFEL_MATH_INLINE2 void computeEigenVectors(
        tvector<3u, ValueType>&,
        rotation_matrix<ValueType>&,
        const bool = false) const;
    /*!
     * compute eigenvectors and eigenvalues
     * \tparam    es: eigen solver
     * \param[in] o:  eigenvalues ordering
     * \param[in] b: refine eigenvalues
     */
    template <EigenSolver = TFELEIGENSOLVER>
    TFEL_HOST_DEVICE TFEL_MATH_INLINE2
        std::tuple<tvector<3u, ValueType>, rotation_matrix<ValueType>>
        computeEigenVectors(const EigenValuesOrdering,
                            const bool = false) const;
    /*!
     * compute eigenvectors and eigenvalues
     * \tparam     es: eigen solver
     * \param[out] vp: eigenvalues
     * \param[out] m:  rotation matrix
     * \param[in]  o:  eigenvalues ordering
     * \param[in]  b:  refine eigenvalues
     */
    template <EigenSolver = TFELEIGENSOLVER>
    TFEL_HOST_DEVICE TFEL_MATH_INLINE2 void computeEigenVectors(
        tvector<3u, ValueType>&,
        rotation_matrix<ValueType>&,
        const EigenValuesOrdering,
        const bool = false) const;
    /*!
     * compute eigenvector associated to a given eigenvalue
     * \note the given vector must be properly allocated and the
     * eigenvector is stored in the three first locations
     * \note no check made that the given value is an eigenvalue of
     * the tensor
     * \note the eigen value geometric multiplicity must be egal to
     * one.
     * \param[out] ev : eigen vector
     * \param[in]  vp : eigenvalue
     */
    template <typename VectorType>
    TFEL_HOST_DEVICE TFEL_MATH_INLINE2 bool computeEigenVector(
        VectorType&, const ValueType) const;
    /*!
     * \brief change basis
     * \param[in] m: rotation matrix
     */
    TFEL_HOST_DEVICE constexpr void changeBasis(
        const rotation_matrix<base_type<ValueType>>&) noexcept;
    //! \return the identity
    TFEL_HOST_DEVICE static constexpr stensor<N, base_type<ValueType>>
    Id() noexcept;

    /*!
     * \brief This function set the component (i,j) of a `stensor` to
     * a value `Aij`, using the function `VoigtIndex`.
     * \return void
     * \tparam NumType: type of the values of the `stensor`
     * \tparam T : type of the value `Aij` to set must be same as
     * the type of the values of the `stensor`.
     * \param[in] A: `stensor`
     * \param[in] i,j: `unsigned short`
     * \param[in] Aij: value that we want for \f[A_{ij}\f]
     */
    template <typename NumType, typename T>
    TFEL_HOST_DEVICE constexpr void setComponent(StensorConcept auto&,
                                                 unsigned short,
                                                 unsigned short,
                                                 const T&) noexcept
        requires(isAssignableTo<NumType, T>());

    /*!
     * \brief This function returns the component (i,j) of a `stensor`,
     * using the function `VoigtIndex`.
     * \return the value \f[A_{ij}\f]
     * \param[in] A: `stensor`
     * \param[in] i,j: `unsigned short`
     */
    TFEL_HOST_DEVICE constexpr auto getComponent(const StensorConcept auto&,
                                                 unsigned short,
                                                 unsigned short);

    /*!
     * copy the value from a container
     */
    template <typename InputIterator>
    TFEL_HOST_DEVICE void constexpr copy(const InputIterator src) noexcept;
    /*!
     * build a symmetric tensor from a matrix
     * \param[in] m : matrix
     */
    template <MatrixConcept MatrixType>
    TFEL_HOST_DEVICE static constexpr auto buildFromMatrix(
        const MatrixType&) noexcept
        requires(isAssignableTo<numeric_type<MatrixType>, ValueType>());

    /*!
     * build a symmetric tensor from the diadic product of a vector with
     * itself
     * \param[in] v : vector
     */
    template <VectorConcept VectorType>
    TFEL_HOST_DEVICE static constexpr auto buildFromVectorDiadicProduct(
        const VectorType&) noexcept
        requires(
            isAssignableTo<typename ComputeUnaryResult<numeric_type<VectorType>,
                                                       Power<2>>::Result,
                           ValueType>());

    /*!
     * build a symmetric tensor from the symmetric diadic product of two
     * vectors
     * \param[in] v1 : first  vector
     * \param[in] v1 : second vector
     */
    template <VectorConcept VectorType, VectorConcept VectorType2>
    TFEL_HOST_DEVICE static constexpr auto
    buildFromVectorsSymmetricDiadicProduct(const VectorType&,
                                           const VectorType2&) noexcept
        requires(isAssignableTo<BinaryOperationResult<numeric_type<VectorType>,
                                                      numeric_type<VectorType2>,
                                                      OpMult>,
                                ValueType>());

    /*!
     * build a symmetric tensor from its eigen values and vectors
     * \param[in] v1 : first  eigen value
     * \param[in] v2 : second eigen value
     * \param[in] v2 : third  eigen value
     * \param[in] m  : matrix containing the eigen vectors
     */
    TFEL_HOST_DEVICE static constexpr auto buildFromEigenValuesAndVectors(
        const ValueType&,
        const ValueType&,
        const ValueType&,
        const rotation_matrix<ValueType>&) noexcept;

    /*!
     * build a symmetric tensor from its eigen values and vectors
     * \param[in] vp : eigen values
     * \param[in] m  : matrix containing the eigen vectors
     */
    TFEL_HOST_DEVICE static constexpr auto buildFromEigenValuesAndVectors(
        const tvector<3u, ValueType>&,
        const rotation_matrix<ValueType>&) noexcept;

    /*!
     * build the logarithm of a symmetric tensor given through its eigen
     * values and vectors
     * \param[in] v1 : first  eigen value
     * \param[in] v2 : second eigen value
     * \param[in] v2 : third  eigen value
     * \param[in] m  : matrix containing the eigen vectors
     */
    TFEL_HOST_DEVICE static auto buildLogarithmFromEigenValuesAndVectors(
        const ValueType&,
        const ValueType&,
        const ValueType&,
        const rotation_matrix<ValueType>&) noexcept;

    /*!
     * build the logarithm of a symmetric tensor given through its eigen
     * values and vectors
     * \param[in] vp : eigen values
     * \param[in] m  : matrix containing the eigen vectors
     */
    TFEL_HOST_DEVICE static auto buildLogarithmFromEigenValuesAndVectors(
        const tvector<3u, ValueType>&,
        const rotation_matrix<ValueType>&) noexcept;

    /*!
     * build the absolute value of a symmetric tensor given through its
     * eigen values and vectors
     * \param[in] v1 : first  eigen value
     * \param[in] v2 : second eigen value
     * \param[in] v2 : third  eigen value
     * \param[in] m  : matrix containing the eigen vectors
     */
    TFEL_HOST_DEVICE static constexpr auto
    buildAbsoluteValueFromEigenValuesAndVectors(
        const ValueType&,
        const ValueType&,
        const ValueType&,
        const rotation_matrix<ValueType>&) noexcept;

    /*!
     * build the absolute value of a symmetric tensor given through its
     * eigen values and vectors \param[in] vp : eigen values \param[in] m  :
     * matrix containing the eigen vectors
     */
    TFEL_HOST_DEVICE static constexpr auto
    buildAbsoluteValueFromEigenValuesAndVectors(
        const tvector<3u, ValueType>&,
        const rotation_matrix<ValueType>&) noexcept;

    /*!
     * build the positive part of a symmetric tensor given through its eigen
     * values and vectors
     * \param[in] v1 : first  eigen value
     * \param[in] v2 : second eigen value
     * \param[in] v2 : third  eigen value
     * \param[in] m  : matrix containing the eigen vectors
     */
    TFEL_HOST_DEVICE static constexpr auto
    buildPositivePartFromEigenValuesAndVectors(
        const ValueType&,
        const ValueType&,
        const ValueType&,
        const rotation_matrix<ValueType>&) noexcept;
    /*!
     * build the positive part of a symmetric tensor given through its eigen
     * values and vectors
     * \param[in] vp : eigen values
     * \param[in] m  : matrix containing the eigen vectors
     */
    TFEL_HOST_DEVICE static constexpr auto
    buildPositivePartFromEigenValuesAndVectors(
        const tvector<3u, ValueType>&,
        const rotation_matrix<ValueType>&) noexcept;
    /*!
     * build the positive part of a symmetric tensor given through its eigen
     * values and vectors
     * \param[in] v1 : first  eigen value
     * \param[in] v2 : second eigen value
     * \param[in] v2 : third  eigen value
     * \param[in] m  : matrix containing the eigen vectors
     */
    TFEL_HOST_DEVICE static constexpr auto
    buildNegativePartFromEigenValuesAndVectors(
        const ValueType&,
        const ValueType&,
        const ValueType&,
        const rotation_matrix<ValueType>&) noexcept;
    /*!
     * build the negative part of a symmetric tensor given through its eigen
     * values and vectors
     * \param[in] vp : eigen values
     * \param[in] m  : matrix containing the eigen vectors
     */
    TFEL_HOST_DEVICE static constexpr auto
    buildNegativePartFromEigenValuesAndVectors(
        const tvector<3u, ValueType>&,
        const rotation_matrix<ValueType>&) noexcept;
    /*!
     * compute the eigenvalues derivatives with respect with this tensor
     * \param[out] n: derivatives of the  eigenvalues
     * \param[in]  m: eigenvectors
     *
     * This eigenvalues of the derivatives is given by~:
     * \f[
     * \frac{\displaystyle \partial}{\lambda_{i}}{\displaystyle
     * \underbrace{s}}=\underbrace{n}_{i}=\vec{e}_{i}\otimes\vec{e}_{i}
     * \f]
     * \(\underbrace{n}_{i}\) are the eigen tensors associated to the given
     * tensor.
     */
    TFEL_HOST_DEVICE static constexpr auto computeEigenValuesDerivatives(
        const rotation_matrix<ValueType>&) noexcept;
    /*!
     * compute the eigenvalues derivatives with respect with this tensor
     * \param[out] n0 : derivative of the first eigenvalue
     * \param[out] n1 : derivative of the second eigenvalue
     * \param[out] n2 : derivative of the third  eigenvalue
     * \param[in]  m  : eigenvectors
     * This eigenvalues of the derivatives is given by~:
     * \f[
     * \frac{\displaystyle \partial}{\lambda_{i}}{\displaystyle
     * \underbrace{s}}=\underbrace{n}_{i}=\vec{e}_{i}\otimes\vec{e}_{i}
     * \f]
     * \f$\underbrace{n}_{i}\f$ are the eigen tensors associated to the
     * given tensor.
     */
    template <StensorConcept StensorType>
    TFEL_HOST_DEVICE static constexpr void computeEigenValuesDerivatives(
        StensorType&,
        StensorType&,
        StensorType&,
        const rotation_matrix<ValueType>&) noexcept  //
        requires((getSpaceDimension<StensorType>() == N) &&
                 (isAssignableTo<base_type<ValueType>,
                                 numeric_type<StensorType>>()));
    /*!
     * \return the eigentensors
     * \param[in]  m: eigenvectors
     */
    TFEL_HOST_DEVICE static constexpr auto computeEigenTensors(
        const rotation_matrix<ValueType>&) noexcept;
    /*!
     * \brief compute the eigentensors
     * \param[out] n0: derivative of the first  eigenvalue
     * \param[out] n1: derivative of the second eigenvalue
     * \param[out] n2: derivative of the third  eigenvalue
     * \param[in]  m:  eigenvectors
     */
    template <StensorConcept StensorType>
    TFEL_HOST_DEVICE static constexpr void computeEigenTensors(
        StensorType&,
        StensorType&,
        StensorType&,
        const rotation_matrix<ValueType>&) noexcept  //
        requires((getSpaceDimension<StensorType>() == N) &&
                 (isAssignableTo<base_type<ValueType>,
                                 numeric_type<StensorType>>()));
    /*!
     * \brief compute the "eigentensors" derivatives
     * \param[out] dn0_ds: derivative of the first  eigentensor
     * \param[out] dn1_ds: derivative of the second eigentensor
     * \param[out] dn2_ds: derivative of the third  eigentensor
     * \param[in]  vp:     eigen values
     * \param[in]  m:      eigen vectors
     * \param[in]  eps:    numerical parameter for regularisation
     */
    template <ST2toST2Concept ST2toST2Type>
    TFEL_HOST_DEVICE static void computeEigenTensorsDerivatives(
        ST2toST2Type&,
        ST2toST2Type&,
        ST2toST2Type&,
        const tvector<3u, ValueType>&,
        const rotation_matrix<ValueType>&,
        const ValueType)  //
        requires(
            (getSpaceDimension<ST2toST2Type>() == N) &&
            (isAssignableTo<
                BinaryOperationResult<base_type<ValueType>, ValueType, OpDiv>,
                numeric_type<ST2toST2Type>>()));
    /*!
     * \brief compute an isotropic function
     * \param[in]  f:   function
     * \param[in]  vp:  eigen values
     * \param[in]  m:   eigenvectors
     */
    template <typename Function>
    TFEL_HOST_DEVICE static auto computeIsotropicFunction(
        const Function&,
        const tvector<3u, ValueType>&,
        const rotation_matrix<ValueType>&);
    /*!
     * compute an isotropic function
     * \param[in]  f: function values for each eigen values
     * \param[in]  m: eigenvectors
     */
    template <typename T2>
    TFEL_HOST_DEVICE static constexpr auto computeIsotropicFunction(
        const tvector<3u, T2>&, const rotation_matrix<ValueType>&) noexcept;
    /*!
     * \return the derivative of an isotropic function
     * \param[in]  f:   function values
     * \param[in]  df:  values of the derivative of the function
     * \param[in]  vp:  eigen values
     * \param[in]  m:   eigenvectors
     * \param[in]  eps: criterion value used to judge if two eigenvalues are
     * equals
     */
    template <typename T1, typename T2>
    TFEL_HOST_DEVICE static auto computeIsotropicFunctionDerivative(
        const tvector<3u, T1>&,
        const tvector<3u, T2>&,
        const tvector<3u, ValueType>&,
        const rotation_matrix<ValueType>&,
        const ValueType);
    /*!
     * compute the derivative of an isotropic function
     * \param[out] d:   result
     * \param[in]  f:   function values
     * \param[in]  df:  values of the derivative of the function
     * \param[in]  vp:  eigen values
     * \param[in]  m:   eigenvectors
     * \param[in]  eps: criterion value used to judge if two eigenvalues are
     * equals
     */
    template <ST2toST2Concept ST2toST2Type, typename T1, typename T2>
    TFEL_HOST_DEVICE static void computeIsotropicFunctionDerivative(
        ST2toST2Type&,
        const tvector<3u, T1>&,
        const tvector<3u, T2>&,
        const tvector<3u, ValueType>&,
        const rotation_matrix<ValueType>&,
        const ValueType) requires((getSpaceDimension<ST2toST2Type>() == N) &&
                                  (isAssignableTo<
                                      BinaryOperationResult<
                                          base_type<ValueType>,
                                          ValueType,
                                          OpDiv>,
                                      numeric_type<ST2toST2Type>>()));
    /*!
     * compute the derivative of an isotropic function
     * \param[out] d:   result
     * \param[in]  f:   function
     * \param[in]  df:  derivative of the function
     * \param[in]  vp:  eigen values
     * \param[in]  m:   eigenvectors
     * \param[in]  eps: criterion value used to judge if two eigenvalues are
     * equals
     */
    template <typename Function, typename FunctionDerivative>
    TFEL_HOST_DEVICE static auto computeIsotropicFunctionDerivative(
        const Function&,
        const FunctionDerivative&,
        const tvector<3u, ValueType>&,
        const rotation_matrix<ValueType>&,
        const ValueType);
    /*!
     * compute the derivative of an isotropic function
     * \param[out] d:   result
     * \param[in]  f:   function
     * \param[in]  df:  derivative of the function
     * \param[in]  vp:  eigen values
     * \param[in]  m:   eigenvectors
     * \param[in]  eps: criterion value used to judge if two eigenvalues are
     * equals
     */
    template <ST2toST2Concept ST2toST2Type,
              typename Function,
              typename FunctionDerivative>
    TFEL_HOST_DEVICE static void computeIsotropicFunctionDerivative(
        ST2toST2Type&,
        const Function&,
        const FunctionDerivative&,
        const tvector<3u, ValueType>&,
        const rotation_matrix<ValueType>&,
        const ValueType) requires((getSpaceDimension<ST2toST2Type>() == N) &&
                                  (isAssignableTo<
                                      BinaryOperationResult<
                                          base_type<ValueType>,
                                          ValueType,
                                          OpDiv>,
                                      numeric_type<ST2toST2Type>>()));
    /*!
     * \return the value of an isotropic function
     * \param[in] f: function
     * \param[in] b:   if true, refinement of eigen values is performed
     */
    template <EigenSolver = TFELEIGENSOLVER, typename Function>
    TFEL_HOST_DEVICE auto computeIsotropicFunction(const Function&,
                                                   const bool = false) const;
    /*!
     * \return the derivative of an isotropic function
     * \param[in] f:   function
     * \param[in] df:  derivative of the function
     * \param[in] eps: criterion value used to judge if two eigenvalues are
     * equals
     * \param[in] b:   if true, refinement of eigen values is performed
     */
    template <EigenSolver = TFELEIGENSOLVER,
              typename Function,
              typename FunctionDerivative>
    TFEL_HOST_DEVICE auto computeIsotropicFunctionDerivative(
        const Function&,
        const FunctionDerivative&,
        const ValueType,
        const bool = false) const;
    /*!
     * \return the derivative of an isotropic function
     * \param[in] f:   function
     * \param[in] df:  derivative of the function
     * \param[in] eps: criterion value used to judge if two eigenvalues are
     * equals
     * \param[in] b:   if true, refinement of eigen values is performed
     */
    template <EigenSolver = TFELEIGENSOLVER,
              typename Function,
              typename FunctionDerivative>
    TFEL_HOST_DEVICE auto computeIsotropicFunctionAndDerivative(
        const Function&,
        const FunctionDerivative&,
        const ValueType,
        const bool = false) const;
  };  // end of struct stensor

  // class template argument deduction
  template <typename... T>
  stensor(T&&... t) -> stensor<StensorSizeToDime<sizeof...(T)>::value,
                               std::common_type_t<T...>>;

  /*!
   * \brief a simple alias for backward compatibility
   * \tparam N: space dimension
   * \tparam T: value type
   */
  template <unsigned short N, typename T>
  using StensorView = View<stensor<N, T>>;
  /*!
   * \brief a simple alias for backward compatibility
   * \tparam N: space dimension
   * \tparam T: value type
   */
  template <unsigned short N, typename T>
  using ConstStensorView = ConstView<stensor<N, T>>;
  /*!
   * \brief export the given vector to an array
   */
  template <unsigned short N, typename T, typename OutputIterator>
  TFEL_HOST_DEVICE constexpr void exportToBaseTypeArray(const stensor<N, T>&,
                                                        OutputIterator) noexcept
      requires(isScalar<T>());

  /*!
   * compute the tresca stress
   * for a symmetric tensor
   *
   * \param s : symmetric tensor
   */
  template <unsigned short N, typename T>
  TFEL_HOST_DEVICE T tresca(const stensor<N, T>&, const bool = false);

  /*!
   * compute the tresca stress
   * for a symmetric tensor.
   *
   * Partial specialisation in 1D
   *
   * \param s : symmetric tensor
   */
  template <typename T>
  TFEL_HOST_DEVICE constexpr T tresca(const stensor<1u, T>&,
                                      const bool = false);
  //! \return the square root of a symmetric tensor
  TFEL_HOST_DEVICE auto square_root(const StensorConcept auto&);
  //! \return the determinant of a symmetric tensor
  TFEL_HOST_DEVICE constexpr auto det(const StensorConcept auto&) noexcept;
  /*!
   * \return the derivative of the determinant
   * \param[in] s: tensor where the the determinant is evaluated
   */
  TFEL_HOST_DEVICE constexpr auto computeDeterminantDerivative(
      const StensorConcept auto&) noexcept;
  /*!
   * \return the derivative of the determinant of the deviator of
   * symmetric tensor.
   *
   * Let \f$\underline{s}\f$ be a symmetric tensor and \f$J_{3}\f$
   * be the determinant of \f$\underline{s}'\f$ the deviator of
   * \f$\underline{s}\f$:
   * \f[
   * J_{3} = \mathrm{det}\left(\underline{s}'\right)
   *       =
   * \mathrm{det}\left(\underline{s}-\mathrm{tr}\left(\underline{s}'\right)\,\underline{I}\right)
   * \f]
   *
   * This function computes \f$\displaystyle\frac{\partial J_{3}}{\partial
   * \underline{\sigma}}\f$.
   *
   * \param[in] s: tensor where the the determinant is evaluated
   */
  TFEL_HOST_DEVICE constexpr auto computeDeviatorDeterminantDerivative(
      const StensorConcept auto&) noexcept;

  /*!
   * \brief rotate a symmetric tensor using a rotation matrix
   * \param[in] s: symmetric tensor to be rotated
   * \param[in] r: rotation matrix
   * \return the rotated symmetric tensor
   */
  template <StensorConcept StensorType>
  TFEL_HOST_DEVICE constexpr auto change_basis(
      const StensorType&,
      const rotation_matrix<numeric_type<StensorType>>&) noexcept;
  /*!
   * \return the inverse of a symmetric tensor
   * \param[in] s: symmetric tensor
   */
  TFEL_HOST_DEVICE constexpr auto invert(const StensorConcept auto&) noexcept;
  /*!
   * \brief compute the logarithm of a symmetric tensor
   * \param s : tensor
   * \param b : if true, refinement of eigen values is performed
   */
  template <StensorConcept StensorType>
  TFEL_HOST_DEVICE auto
  logarithm(const StensorType&, const bool = false) requires(
      tfel::typetraits::IsFundamentalNumericType<
          numeric_type<StensorType>>::cond);
  /*!
   * \brief compute the absolute value of a symmetric tensor
   * \param s : tensor
   * \param b : if true, refinement of eigen values is performed
   */
  template <StensorConcept StensorType>
  TFEL_HOST_DEVICE auto
  absolute_value(const StensorType&, const bool = false) requires(
      tfel::typetraits::IsFundamentalNumericType<
          numeric_type<StensorType>>::cond);
  /*!
   * \brief compute the positive part of a symmetric tensor
   * \param s : tensor
   * \param b : if true, refinement of eigen values is performed
   */
  template <StensorConcept StensorType>
  TFEL_HOST_DEVICE auto
  positive_part(const StensorType&, const bool = false) requires(
      tfel::typetraits::IsFundamentalNumericType<
          numeric_type<StensorType>>::cond);
  /*!
   * \brief compute the negative part of a symmetric tensor
   * \param s : tensor
   * \param b : if true, refinement of eigen values is performed
   */
  template <StensorConcept StensorType>
  TFEL_HOST_DEVICE auto
  negative_part(const StensorType&, const bool = false) requires(
      tfel::typetraits::IsFundamentalNumericType<
          numeric_type<StensorType>>::cond);
  /*!
   * \return the square of a symmetric stensor
   * \param[in] s : squared tensor
   */
  TFEL_HOST_DEVICE auto square(const StensorConcept auto&);
  /*!
   * \return the value of an isotropic function
   * \param[in] f: function
   * \param[in] s: symmetric tensor
   * \param[in] b: if true, refinement of eigen values is performed
   */
  template <
      typename stensor_common::EigenSolver = stensor_common::TFELEIGENSOLVER,
      typename Function,
      StensorConcept StensorType>
  TFEL_HOST_DEVICE auto computeIsotropicFunction(const Function&,
                                                 const StensorType&,
                                                 const bool = false);
  /*!
   * \return the derivative of an isotropic function
   * \param[in] f:   function
   * \param[in] df:  derivative of the function
   * \param[in] s:   symmetric tensor
   * \param[in] eps: criterion value used to judge if two eigenvalues are
   * equals
   * \param[in] b:   if true, refinement of eigen values is performed
   */
  template <
      typename stensor_common::EigenSolver = stensor_common::TFELEIGENSOLVER,
      typename Function,
      typename FunctionDerivative,
      StensorConcept StensorType>
  TFEL_HOST_DEVICE auto computeIsotropicFunctionDerivative(
      const Function&,
      const FunctionDerivative&,
      const StensorType&,
      const numeric_type<StensorType>,
      const bool = false);
  /*!
   * \return the value and the derivative of an isotropic function
   * \param[in] f:   function
   * \param[in] df:  derivative of the function
   * \param[in] s:   symmetric tensor
   * \param[in] eps: criterion value used to judge if two eigenvalues are
   * equals
   * \param[in] b:   if true, refinement of eigen values is performed
   */
  template <
      typename stensor_common::EigenSolver = stensor_common::TFELEIGENSOLVER,
      typename Function,
      typename FunctionDerivative,
      StensorConcept StensorType>
  TFEL_HOST_DEVICE auto computeIsotropicFunctionAndDerivative(
      const Function&,
      const FunctionDerivative&,
      const StensorType&,
      const numeric_type<StensorType>,
      const bool = false);
  /*!
   * \brief convert the corotationnal cauchy stress to the second
   * Piola-Kirchhoff stress
   * \f[
   * \underline{S} =
   * J\,\underline{U}^{-1}\,.\,\underline{\sigma}\,.\,\underline{U}^{-1}
   * \f]
   * \param[in] s: corotationnal cauchy stress
   * \param[in] U: stretch tensor
   * \return the second Piola-Kirchhoff stress
   */
  template <StensorConcept T, StensorConcept T2>
  TFEL_HOST_DEVICE constexpr auto
  convertCorotationnalCauchyStressToSecondPiolaKirchhoffStress(
      const T&, const T2&) noexcept
      requires(
          (getSpaceDimension<T>() == getSpaceDimension<T2>()) &&
          (tfel::typetraits::IsFundamentalNumericType<numeric_type<T2>>::cond));
  /*!
   * \brief convert the second Piola-Kirchhoff stress to the
   * corotationnal cauchy stress:
   * \f[
   * \underline{S} =
   * J\,\underline{U}^{-1}\,.\,\underline{\sigma}\,.\,\underline{U}^{-1}
   * \f]
   * \param[in] S: the second Piola-Kirchhoff stress
   * \param[in] U: stretch tensor
   * \return the corotationnal cauchy stress
   */
  template <StensorConcept T, StensorConcept T2>
  TFEL_HOST_DEVICE constexpr auto
  convertSecondPiolaKirchhoffStressToCorotationnalCauchyStress(
      const T&, const T2&) noexcept
      requires(
          (getSpaceDimension<T>() == getSpaceDimension<T2>()) &&
          (tfel::typetraits::IsFundamentalNumericType<numeric_type<T2>>::cond));
  /*!
   * \brief return the symmetric product of two stensors as a symmetric tensor:
   * \f[
   * s1 * s2 + s2 * s1
   * \f]
   * \param[in] s1: first  tensor
   * \param[in] s2: second tensor
   */
  template <StensorConcept StensorType1, StensorConcept StensorType2>
  TFEL_HOST_DEVICE constexpr auto symmetric_product(
      const StensorType1&, const StensorType2&) noexcept
      requires(getSpaceDimension<StensorType1>() ==
               getSpaceDimension<StensorType2>());

}  // end of namespace tfel::math

namespace tfel::typetraits {

  template <unsigned short N, typename T2, typename T>
  struct IsAssignableTo<tfel::math::stensor<N, T2>, tfel::math::stensor<N, T>> {
    /*!
     *  Result
     */
    static constexpr bool cond = isAssignableTo<T2, T>();
  };

}  // end of namespace tfel::typetraits

#include "TFEL/Math/Stensor/stensor.ixx"
#include "TFEL/Math/Stensor/stensorResultType.hxx"
#include "TFEL/Math/Stensor/DecompositionInPositiveAndNegativeParts.hxx"

#endif /* LIB_TFEL_MATH_STENSOR_HXX */
