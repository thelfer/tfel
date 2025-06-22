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
  template <typename StensorType, typename ScalarType>
  struct DerivativeTypeDispatcher<StensorTag,
                                  ScalarTag,
                                  StensorType,
                                  ScalarType> {
    static_assert(implementsStensorConcept<StensorType>(),
                  "StensorType is not a symmetric tensor");
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
  template <typename ScalarType, typename StensorType>
  struct DerivativeTypeDispatcher<ScalarTag,
                                  StensorTag,
                                  ScalarType,
                                  StensorType> {
    static_assert(implementsStensorConcept<StensorType>(),
                  "StensorType is not a symmetric tensor");
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
      GTESYMMETRICQREIGENSOLVER
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
      : StensorConcept<stensor<N, ValueType>>,
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
    TFEL_HOST_DEVICE TFEL_MATH_INLINE2 std::enable_if_t<
        std::is_same<typename std::iterator_traits<InputIterator>::value_type,
                     base_type<ValueType>>::value,
        void>
    importVoigt(const InputIterator);
    //! \brief import from external memory location which uses Voigt notations
    //! for stresses
    template <typename InputIterator>
    TFEL_HOST_DEVICE TFEL_MATH_INLINE2 std::enable_if_t<
        std::is_same<typename std::iterator_traits<InputIterator>::value_type,
                     base_type<ValueType>>::value,
        void>
    importTab(const InputIterator);
    //! \brief import values from external memory location
    template <typename InputIterator>
    TFEL_HOST_DEVICE TFEL_MATH_INLINE2 std::enable_if_t<
        std::is_same<typename std::iterator_traits<InputIterator>::value_type,
                     base_type<ValueType>>::value,
        void>
    import(const InputIterator);
    //! \brief export to external memory location using Voigt notations for
    //! stresses
    TFEL_HOST_DEVICE void exportTab(base_type<ValueType>* const) const;
    //! \brief write to external memory location
    TFEL_HOST_DEVICE void write(base_type<ValueType>* const) const;
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
     */
    TFEL_HOST_DEVICE TFEL_MATH_INLINE2 void changeBasis(
        const rotation_matrix<ValueType>&);
    //! \return the identity
    TFEL_HOST_DEVICE
    TFEL_MATH_INLINE static constexpr stensor<N, base_type<ValueType>> Id();
    /*!
     * copy the value from a container
     */
    template <typename InputIterator>
    TFEL_HOST_DEVICE TFEL_MATH_INLINE2 void copy(const InputIterator src);
    /*!
     * build a symmetric tensor from a matrix
     * \param[in] m : matrix
     */
    template <typename MatrixType>
    TFEL_HOST_DEVICE static TFEL_MATH_INLINE2
        std::enable_if_t<isAssignableTo<numeric_type<MatrixType>, ValueType>(),
                         stensor<N, ValueType>>
        buildFromMatrix(const MatrixType&);

    /*!
     * build a symmetric tensor from the diadic product of a vector with
     * itself
     * \param[in] v : vector
     */
    template <typename VectorType>
    TFEL_HOST_DEVICE static TFEL_MATH_INLINE2 std::enable_if_t<
        isAssignableTo<typename ComputeUnaryResult<numeric_type<VectorType>,
                                                   Power<2>>::Result,
                       ValueType>(),
        stensor<N, ValueType>>
    buildFromVectorDiadicProduct(const VectorType&);

    /*!
     * build a symmetric tensor from the symmetric diadic product of two
     * vectors
     * \param[in] v1 : first  vector
     * \param[in] v1 : second vector
     */
    template <typename VectorType, typename VectorType2>
    TFEL_HOST_DEVICE static TFEL_MATH_INLINE2 std::enable_if_t<
        isAssignableTo<BinaryOperationResult<numeric_type<VectorType>,
                                             numeric_type<VectorType2>,
                                             OpMult>,
                       ValueType>(),
        stensor<N, ValueType>>
    buildFromVectorsSymmetricDiadicProduct(const VectorType&,
                                           const VectorType2&);

    /*!
     * build a symmetric tensor from its eigen values and vectors
     * \param[in] v1 : first  eigen value
     * \param[in] v2 : second eigen value
     * \param[in] v2 : third  eigen value
     * \param[in] m  : matrix containing the eigen vectors
     */
    TFEL_HOST_DEVICE static stensor<N, ValueType>
    buildFromEigenValuesAndVectors(const ValueType&,
                                   const ValueType&,
                                   const ValueType&,
                                   const rotation_matrix<ValueType>&);

    /*!
     * build a symmetric tensor from its eigen values and vectors
     * \param[in] vp : eigen values
     * \param[in] m  : matrix containing the eigen vectors
     */
    TFEL_HOST_DEVICE static stensor<N, ValueType>
    buildFromEigenValuesAndVectors(const tvector<3u, ValueType>&,
                                   const rotation_matrix<ValueType>&);

    /*!
     * build the logarithm of a symmetric tensor given through its eigen
     * values and vectors
     * \param[in] v1 : first  eigen value
     * \param[in] v2 : second eigen value
     * \param[in] v2 : third  eigen value
     * \param[in] m  : matrix containing the eigen vectors
     */
    TFEL_HOST_DEVICE static stensor<N, ValueType>
    buildLogarithmFromEigenValuesAndVectors(const ValueType&,
                                            const ValueType&,
                                            const ValueType&,
                                            const rotation_matrix<ValueType>&);

    /*!
     * build the logarithm of a symmetric tensor given through its eigen
     * values and vectors
     * \param[in] vp : eigen values
     * \param[in] m  : matrix containing the eigen vectors
     */
    TFEL_HOST_DEVICE static stensor<N, ValueType>
    buildLogarithmFromEigenValuesAndVectors(const tvector<3u, ValueType>&,
                                            const rotation_matrix<ValueType>&);

    /*!
     * build the absolute value of a symmetric tensor given through its
     * eigen values and vectors \param[in] v1 : first  eigen value
     * \param[in] v2 : second eigen value
     * \param[in] v2 : third  eigen value
     * \param[in] m  : matrix containing the eigen vectors
     */
    TFEL_HOST_DEVICE static stensor<N, ValueType>
    buildAbsoluteValueFromEigenValuesAndVectors(
        const ValueType&,
        const ValueType&,
        const ValueType&,
        const rotation_matrix<ValueType>&);

    /*!
     * build the absolute value of a symmetric tensor given through its
     * eigen values and vectors \param[in] vp : eigen values \param[in] m  :
     * matrix containing the eigen vectors
     */
    TFEL_HOST_DEVICE static stensor<N, ValueType>
    buildAbsoluteValueFromEigenValuesAndVectors(
        const tvector<3u, ValueType>&, const rotation_matrix<ValueType>&);

    /*!
     * build the positive part of a symmetric tensor given through its eigen
     * values and vectors
     * \param[in] v1 : first  eigen value
     * \param[in] v2 : second eigen value
     * \param[in] v2 : third  eigen value
     * \param[in] m  : matrix containing the eigen vectors
     */
    TFEL_HOST_DEVICE static stensor<N, ValueType>
    buildPositivePartFromEigenValuesAndVectors(
        const ValueType&,
        const ValueType&,
        const ValueType&,
        const rotation_matrix<ValueType>&);
    /*!
     * build the positive part of a symmetric tensor given through its eigen
     * values and vectors
     * \param[in] vp : eigen values
     * \param[in] m  : matrix containing the eigen vectors
     */
    TFEL_HOST_DEVICE static stensor<N, ValueType>
    buildPositivePartFromEigenValuesAndVectors(
        const tvector<3u, ValueType>&, const rotation_matrix<ValueType>&);
    /*!
     * build the positive part of a symmetric tensor given through its eigen
     * values and vectors
     * \param[in] v1 : first  eigen value
     * \param[in] v2 : second eigen value
     * \param[in] v2 : third  eigen value
     * \param[in] m  : matrix containing the eigen vectors
     */
    TFEL_HOST_DEVICE static stensor<N, ValueType>
    buildNegativePartFromEigenValuesAndVectors(
        const ValueType&,
        const ValueType&,
        const ValueType&,
        const rotation_matrix<ValueType>&);

    /*!
     * build the negative part of a symmetric tensor given through its eigen
     * values and vectors
     * \param[in] vp : eigen values
     * \param[in] m  : matrix containing the eigen vectors
     */
    TFEL_HOST_DEVICE static stensor<N, ValueType>
    buildNegativePartFromEigenValuesAndVectors(
        const tvector<3u, ValueType>&, const rotation_matrix<ValueType>&);
    /*!
     * compute the eigenvalues derivatives with respect with this tensor
     * \param[out] n : derivatives of the  eigenvalues
     * \param[in]  m  : eigenvectors
     * This eigenvalues of the derivatives is given by~:
     * \f[
     * \frac{\displaystyle \partial}{\lambda_{i}}{\displaystyle
     * \underbrace{s}}=\underbrace{n}_{i}=\vec{e}_{i}\otimes\vec{e}_{i}
     * \f]
     * \(\underbrace{n}_{i}\) are the eigen tensors associated to the given
     * tensor.
     */
    TFEL_HOST_DEVICE std::tuple<stensor<N, ValueType>,
                                stensor<N, ValueType>,
                                stensor<N, ValueType>>
    computeEigenValuesDerivatives(const rotation_matrix<ValueType>&);
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
    template <typename StensorType>
    TFEL_HOST_DEVICE static std::enable_if_t<
        (implementsStensorConcept<StensorType>()) &&
            (getSpaceDimension<StensorType>() == N) &&
            (isAssignableTo<base_type<ValueType>, numeric_type<StensorType>>()),
        void>
    computeEigenValuesDerivatives(StensorType&,
                                  StensorType&,
                                  StensorType&,
                                  const rotation_matrix<ValueType>&);
    /*!
     * \brief compute the "eigentensors"
     * \param[out] n: derivativse of the eigenvalues
     * \param[in]  m: eigenvectors
     */
    TFEL_HOST_DEVICE static std::tuple<stensor<N, ValueType>,
                                       stensor<N, ValueType>,
                                       stensor<N, ValueType>>
    computeEigenTensors(const rotation_matrix<ValueType>&);
    /*!
     * compute the "eigentensors"
     * \param[out] n0: derivative of the first  eigenvalue
     * \param[out] n1: derivative of the second eigenvalue
     * \param[out] n2: derivative of the third  eigenvalue
     * \param[in]  m:  eigenvectors
     */
    template <typename StensorType>
    TFEL_HOST_DEVICE static std::enable_if_t<
        (implementsStensorConcept<StensorType>()) &&
            (getSpaceDimension<StensorType>() == N) &&
            (isAssignableTo<base_type<ValueType>, numeric_type<StensorType>>()),
        void>
    computeEigenTensors(StensorType&,
                        StensorType&,
                        StensorType&,
                        const rotation_matrix<ValueType>&);
    /*!
     * \brief compute the "eigentensors" derivatives
     * \param[out] dn0_ds: derivative of the first  eigentensor
     * \param[out] dn1_ds: derivative of the second eigentensor
     * \param[out] dn2_ds: derivative of the third  eigentensor
     * \param[in]  vp:     eigen values
     * \param[in]  m:      eigen vectors
     * \param[in]  eps:    numerical parameter for regularisation
     */
    template <typename ST2toST2Type>
    TFEL_HOST_DEVICE static std::enable_if_t<
        (implementsST2toST2Concept<ST2toST2Type>()) &&
            (getSpaceDimension<ST2toST2Type>() == N) &&
            (isAssignableTo<
                BinaryOperationResult<base_type<ValueType>, ValueType, OpDiv>,
                numeric_type<ST2toST2Type>>()),
        void>
    computeEigenTensorsDerivatives(ST2toST2Type&,
                                   ST2toST2Type&,
                                   ST2toST2Type&,
                                   const tvector<3u, ValueType>&,
                                   const rotation_matrix<ValueType>&,
                                   const ValueType);
    /*!
     * compute an isotropic function
     * \param[in]  f:   function
     * \param[in]  vp:  eigen values
     * \param[in]  m:   eigenvectors
     */
    template <typename Function>
    TFEL_HOST_DEVICE static stensor<N,
                                    std::invoke_result_t<Function, ValueType>>
    computeIsotropicFunction(const Function&,
                             const tvector<3u, ValueType>&,
                             const rotation_matrix<ValueType>&);
    /*!
     * compute an isotropic function
     * \param[in]  f: function values for each eigen values
     * \param[in]  m: eigenvectors
     */
    template <typename T2>
    TFEL_HOST_DEVICE static stensor<N, T2> computeIsotropicFunction(
        const tvector<3u, T2>&, const rotation_matrix<ValueType>&);
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
    TFEL_HOST_DEVICE static st2tost2<N, T2> computeIsotropicFunctionDerivative(
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
    template <typename ST2toST2Type, typename T1, typename T2>
    TFEL_HOST_DEVICE static std::enable_if_t<
        (implementsST2toST2Concept<ST2toST2Type>()) &&
            (getSpaceDimension<ST2toST2Type>() == N) &&
            (isAssignableTo<
                BinaryOperationResult<base_type<ValueType>, ValueType, OpDiv>,
                numeric_type<ST2toST2Type>>()),
        void>
    computeIsotropicFunctionDerivative(ST2toST2Type&,
                                       const tvector<3u, T1>&,
                                       const tvector<3u, T2>&,
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
    template <typename Function, typename FunctionDerivative>
    TFEL_HOST_DEVICE static st2tost2<
        N,
        std::invoke_result_t<FunctionDerivative, ValueType>>
    computeIsotropicFunctionDerivative(const Function&,
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
    template <typename ST2toST2Type,
              typename Function,
              typename FunctionDerivative>
    TFEL_HOST_DEVICE static std::enable_if_t<
        (implementsST2toST2Concept<ST2toST2Type>()) &&
            (getSpaceDimension<ST2toST2Type>() == N) &&
            (isAssignableTo<
                BinaryOperationResult<base_type<ValueType>, ValueType, OpDiv>,
                numeric_type<ST2toST2Type>>()),
        void>
    computeIsotropicFunctionDerivative(ST2toST2Type&,
                                       const Function&,
                                       const FunctionDerivative&,
                                       const tvector<3u, ValueType>&,
                                       const rotation_matrix<ValueType>&,
                                       const ValueType);
    /*!
     * \return the value of an isotropic function
     * \param[in] f: function
     * \param[in] b:   if true, refinement of eigen values is performed
     */
    template <EigenSolver = TFELEIGENSOLVER, typename Function>
    TFEL_HOST_DEVICE stensor<N, std::invoke_result_t<Function, ValueType>>
    computeIsotropicFunction(const Function&, const bool = false) const;
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
    TFEL_HOST_DEVICE
        st2tost2<N, std::invoke_result_t<FunctionDerivative, ValueType>>
        computeIsotropicFunctionDerivative(const Function&,
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
    TFEL_HOST_DEVICE std::pair<
        stensor<N, std::invoke_result_t<Function, ValueType>>,
        st2tost2<N, std::invoke_result_t<FunctionDerivative, ValueType>>>
    computeIsotropicFunctionAndDerivative(const Function&,
                                          const FunctionDerivative&,
                                          const ValueType,
                                          const bool = false) const;
  };  // end of struct stensor

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
   * export the given vector to an array of the
   */
  template <unsigned short N, typename T, typename OutputIterator>
  TFEL_HOST_DEVICE TFEL_MATH_INLINE2 std::enable_if_t<isScalar<T>(), void>
  exportToBaseTypeArray(const stensor<N, T>&, OutputIterator);

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
  TFEL_HOST_DEVICE T tresca(const stensor<1u, T>&, const bool = false);

  template <typename StensorType>
  TFEL_HOST_DEVICE std::enable_if_t<
      implementsStensorConcept<StensorType>(),
      stensor<getSpaceDimension<StensorType>(), numeric_type<StensorType>>>
  square_root(const StensorType&);

  template <typename StensorType>
  TFEL_HOST_DEVICE std::enable_if_t<
      implementsStensorConcept<StensorType>(),
      typename ComputeUnaryResult<numeric_type<StensorType>, Power<3>>::Result>
  det(const StensorType&);
  /*!
   * \return the derivative of the determinant
   * \param[in] s: tensor where the the determinant is evaluated
   */
  template <typename StensorType>
  TFEL_HOST_DEVICE std::enable_if_t<
      implementsStensorConcept<StensorType>(),
      stensor<getSpaceDimension<StensorType>(),
              typename ComputeUnaryResult<numeric_type<StensorType>,
                                          Power<2>>::Result>>
  computeDeterminantDerivative(const StensorType&);
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
  template <typename StensorType>
  TFEL_HOST_DEVICE std::enable_if_t<
      implementsStensorConcept<StensorType>(),
      stensor<getSpaceDimension<StensorType>(),
              typename ComputeUnaryResult<numeric_type<StensorType>,
                                          Power<2>>::Result>>
  computeDeviatorDeterminantDerivative(const StensorType&);
  /*!
   * \brief rotate a symmetric tensor using a rotation matrix
   * \param[in] s: symmetric tensor to be rotated
   * \param[in] r: rotation matrix
   * \return the rotated symmetric tensor
   */
  template <typename StensorType>
  TFEL_HOST_DEVICE TFEL_MATH_INLINE2 std::enable_if_t<
      implementsStensorConcept<StensorType>(),
      stensor<getSpaceDimension<StensorType>(), numeric_type<StensorType>>>
  change_basis(const StensorType&,
               const rotation_matrix<numeric_type<StensorType>>&);

  template <typename StensorType>
  TFEL_HOST_DEVICE TFEL_MATH_INLINE2 std::enable_if_t<
      implementsStensorConcept<StensorType>(),
      stensor<getSpaceDimension<StensorType>(),
              BinaryOperationResult<base_type<numeric_type<StensorType>>,
                                    numeric_type<StensorType>,
                                    OpDiv>>>
  invert(const StensorType&);
  /*!
   * \brief compute the logarithm of a symmetric tensor
   * \param s : tensor
   * \param b : if true, refinement of eigen values is performed
   */
  template <typename StensorType>
  TFEL_HOST_DEVICE TFEL_MATH_INLINE2
      std::enable_if_t<((implementsStensorConcept<StensorType>()) &&
                        (getSpaceDimension<StensorType>() == 1u) &&
                        (tfel::typetraits::IsFundamentalNumericType<
                            numeric_type<StensorType>>::cond)),
                       stensor<1u, numeric_type<StensorType>>>
      logarithm(const StensorType&, const bool = false);
  /*!
   * \brief compute the logarithm of a symmetric tensor
   * \param s : tensor
   * \param b : if true, refinement of eigen values is performed
   */
  template <typename StensorType>
  TFEL_HOST_DEVICE TFEL_MATH_INLINE2 std::enable_if_t<
      ((implementsStensorConcept<StensorType>()) &&
       ((getSpaceDimension<StensorType>() == 2u) ||
        (getSpaceDimension<StensorType>() == 3u)) &&
       (tfel::typetraits::IsFundamentalNumericType<
           numeric_type<StensorType>>::cond)),
      stensor<getSpaceDimension<StensorType>(), numeric_type<StensorType>>>
  logarithm(const StensorType&, const bool = false);
  /*!
   * \brief compute the absolute value of a symmetric tensor
   * \param s : tensor
   * \param b : if true, refinement of eigen values is performed
   */
  template <typename StensorType>
  TFEL_HOST_DEVICE
      std::enable_if_t<((implementsStensorConcept<StensorType>()) &&
                        (getSpaceDimension<StensorType>() == 1u) &&
                        (tfel::typetraits::IsFundamentalNumericType<
                            numeric_type<StensorType>>::cond)),
                       stensor<1u, numeric_type<StensorType>>>
      absolute_value(const StensorType&, const bool = false);
  /*!
   * \brief compute the absolute value of a symmetric tensor
   * \param s : tensor
   * \param b : if true, refinement of eigen values is performed
   */
  template <typename StensorType>
  TFEL_HOST_DEVICE std::enable_if_t<
      ((implementsStensorConcept<StensorType>()) &&
       ((getSpaceDimension<StensorType>() == 2u) ||
        (getSpaceDimension<StensorType>() == 3u)) &&
       (tfel::typetraits::IsFundamentalNumericType<
           numeric_type<StensorType>>::cond)),
      stensor<getSpaceDimension<StensorType>(), numeric_type<StensorType>>>
  absolute_value(const StensorType&, const bool = false);
  /*!
   * \brief compute the positive part of a symmetric tensor
   * \param s : tensor
   * \param b : if true, refinement of eigen values is performed
   */
  template <typename StensorType>
  TFEL_HOST_DEVICE
      std::enable_if_t<((implementsStensorConcept<StensorType>()) &&
                        (getSpaceDimension<StensorType>() == 1u) &&
                        (tfel::typetraits::IsFundamentalNumericType<
                            numeric_type<StensorType>>::cond)),
                       stensor<1u, numeric_type<StensorType>>>
      positive_part(const StensorType&, const bool = false);
  /*!
   * \brief compute the positive part of a symmetric tensor
   * \param s : tensor
   * \param b : if true, refinement of eigen values is performed
   */
  template <typename StensorType>
  TFEL_HOST_DEVICE std::enable_if_t<
      ((implementsStensorConcept<StensorType>()) &&
       ((getSpaceDimension<StensorType>() == 2u) ||
        (getSpaceDimension<StensorType>() == 3u)) &&
       (tfel::typetraits::IsFundamentalNumericType<
           numeric_type<StensorType>>::cond)),
      stensor<getSpaceDimension<StensorType>(), numeric_type<StensorType>>>
  positive_part(const StensorType&, const bool = false);
  /*!
   * \brief compute the negative part of a symmetric tensor
   * \param s : tensor
   * \param b : if true, refinement of eigen values is performed
   */
  template <typename StensorType>
  TFEL_HOST_DEVICE
      std::enable_if_t<((implementsStensorConcept<StensorType>()) &&
                        (getSpaceDimension<StensorType>() == 1u) &&
                        (tfel::typetraits::IsFundamentalNumericType<
                            numeric_type<StensorType>>::cond)),
                       stensor<1u, numeric_type<StensorType>>>
      negative_part(const StensorType&, const bool = false);
  /*!
   * \brief compute the negative part of a symmetric tensor
   * \param s : tensor
   * \param b : if true, refinement of eigen values is performed
   */
  template <typename StensorType>
  TFEL_HOST_DEVICE std::enable_if_t<
      ((implementsStensorConcept<StensorType>()) &&
       ((getSpaceDimension<StensorType>() == 2u) ||
        (getSpaceDimension<StensorType>() == 3u)) &&
       (tfel::typetraits::IsFundamentalNumericType<
           numeric_type<StensorType>>::cond)),
      stensor<getSpaceDimension<StensorType>(), numeric_type<StensorType>>>
  negative_part(const StensorType&, const bool = false);
  /*!
   * \return the square of a symmetric stensor
   * \param[in] s : squared tensor
   */
  template <typename StensorType>
  TFEL_HOST_DEVICE
      std::enable_if_t<implementsStensorConcept<StensorType>() &&
                           getSpaceDimension<StensorType>() == 1u,
                       stensor<1u,
                               BinaryOperationResult<numeric_type<StensorType>,
                                                     numeric_type<StensorType>,
                                                     OpMult>>>
      square(const StensorType&);
  /*!
   * \return the square of a symmetric stensor
   * \param[in] s : squared tensor
   */
  template <typename StensorType>
  TFEL_HOST_DEVICE
      std::enable_if_t<implementsStensorConcept<StensorType>() &&
                           getSpaceDimension<StensorType>() == 2u,
                       stensor<2u,
                               BinaryOperationResult<numeric_type<StensorType>,
                                                     numeric_type<StensorType>,
                                                     OpMult>>>
      square(const StensorType&);
  /*!
   * \return the square of a symmetric stensor
   * \param[in] s : squared tensor
   */
  template <typename StensorType>
  TFEL_HOST_DEVICE
      std::enable_if_t<implementsStensorConcept<StensorType>() &&
                           getSpaceDimension<StensorType>() == 3u,
                       stensor<3u,
                               BinaryOperationResult<numeric_type<StensorType>,
                                                     numeric_type<StensorType>,
                                                     OpMult>>>
      square(const StensorType&);
  /*!
   * \return the value of an isotropic function
   * \param[in] f: function
   * \param[in] s: symmetric tensor
   * \param[in] b: if true, refinement of eigen values is performed
   */
  template <
      typename stensor_common::EigenSolver = stensor_common::TFELEIGENSOLVER,
      typename Function,
      typename StensorType>
  TFEL_HOST_DEVICE std::enable_if_t<
      implementsStensorConcept<StensorType>(),
      stensor<getSpaceDimension<StensorType>(), numeric_type<StensorType>>>
  computeIsotropicFunction(const Function&,
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
      typename StensorType>
  TFEL_HOST_DEVICE std::enable_if_t<
      implementsStensorConcept<StensorType>(),
      st2tost2<getSpaceDimension<StensorType>(), numeric_type<StensorType>>>
  computeIsotropicFunctionDerivative(const Function&,
                                     const FunctionDerivative&,
                                     const StensorType&,
                                     const numeric_type<StensorType>,
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
      typename StensorType>
  TFEL_HOST_DEVICE
      std::enable_if_t<implementsStensorConcept<StensorType>(),
                       std::pair<stensor<getSpaceDimension<StensorType>(),
                                         numeric_type<StensorType>>,
                                 st2tost2<getSpaceDimension<StensorType>(),
                                          numeric_type<StensorType>>>>
      computeIsotropicFunctionDerivative(const Function&,
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
  template <typename T, typename T2>
  TFEL_HOST_DEVICE TFEL_MATH_INLINE2 std::enable_if_t<
      ((implementsStensorConcept<T>()) && (getSpaceDimension<T>() == 1u) &&
       (implementsStensorConcept<T2>()) && (getSpaceDimension<T2>() == 1u) &&
       (tfel::typetraits::IsFundamentalNumericType<numeric_type<T2>>::cond)),
      stensor<1u, numeric_type<T>>>
  convertCorotationnalCauchyStressToSecondPiolaKirchhoffStress(const T&,
                                                               const T2&);
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
  template <typename T, typename T2>
  TFEL_HOST_DEVICE TFEL_MATH_INLINE2 std::enable_if_t<
      ((implementsStensorConcept<T>()) && (getSpaceDimension<T>() == 2u) &&
       (implementsStensorConcept<T2>()) && (getSpaceDimension<T2>() == 2u) &&
       (tfel::typetraits::IsFundamentalNumericType<numeric_type<T2>>::cond)),
      stensor<2u, numeric_type<T>>>
  convertCorotationnalCauchyStressToSecondPiolaKirchhoffStress(const T&,
                                                               const T2&);
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
  template <typename T, typename T2>
  TFEL_HOST_DEVICE TFEL_MATH_INLINE2 std::enable_if_t<
      ((implementsStensorConcept<T>()) && (getSpaceDimension<T>() == 3u) &&
       (implementsStensorConcept<T2>()) && (getSpaceDimension<T2>() == 3u) &&
       (tfel::typetraits::IsFundamentalNumericType<numeric_type<T2>>::cond)),
      stensor<3u, numeric_type<T>>>
  convertCorotationnalCauchyStressToSecondPiolaKirchhoffStress(const T&,
                                                               const T2&);
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
  template <typename T, typename T2>
  TFEL_HOST_DEVICE TFEL_MATH_INLINE2 std::enable_if_t<
      ((implementsStensorConcept<T>()) && (getSpaceDimension<T>() == 1u) &&
       (implementsStensorConcept<T2>()) && (getSpaceDimension<T2>() == 1u) &&
       (tfel::typetraits::IsFundamentalNumericType<numeric_type<T2>>::cond)),
      stensor<1u, numeric_type<T>>>
  convertSecondPiolaKirchhoffStressToCorotationnalCauchyStress(const T&,
                                                               const T2&);
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
  template <typename T, typename T2>
  TFEL_HOST_DEVICE TFEL_MATH_INLINE2 std::enable_if_t<
      ((implementsStensorConcept<T>()) && (getSpaceDimension<T>() == 2u) &&
       (implementsStensorConcept<T2>()) && (getSpaceDimension<T2>() == 2u) &&
       (tfel::typetraits::IsFundamentalNumericType<numeric_type<T2>>::cond)),
      stensor<2u, numeric_type<T>>>
  convertSecondPiolaKirchhoffStressToCorotationnalCauchyStress(const T&,
                                                               const T2&);
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
  template <typename T, typename T2>
  TFEL_HOST_DEVICE TFEL_MATH_INLINE2 std::enable_if_t<
      ((implementsStensorConcept<T>()) && (getSpaceDimension<T>() == 3u) &&
       (implementsStensorConcept<T2>()) && (getSpaceDimension<T2>() == 3u) &&
       (tfel::typetraits::IsFundamentalNumericType<numeric_type<T2>>::cond)),
      stensor<3u, numeric_type<T>>>
  convertSecondPiolaKirchhoffStressToCorotationnalCauchyStress(const T&,
                                                               const T2&);
  /*!
   * \brief return the symmetric product of two stensors as a symmetric tensor
   * \f[
   * s1*s2+s2*s1
   * \f]
   * \param[in] s1: first  tensor
   * \param[in] s2: second tensor
   */
  template <typename StensorType1, typename StensorType2>
  TFEL_HOST_DEVICE
      std::enable_if_t<((implementsStensorConcept<StensorType1>()) &&
                        (implementsStensorConcept<StensorType2>()) &&
                        (getSpaceDimension<StensorType1>() ==
                         getSpaceDimension<StensorType2>())),
                       stensor<getSpaceDimension<StensorType1>(),
                               result_type<numeric_type<StensorType1>,
                                           numeric_type<StensorType2>,
                                           OpMult>>>
      symmetric_product(const StensorType1&, const StensorType2&);

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

#include "TFEL/Math/Stensor/StensorSizeToDime.hxx"
#include "TFEL/Math/Stensor/stensor.ixx"
#include "TFEL/Math/Stensor/stensorResultType.hxx"
#include "TFEL/Math/Stensor/DecompositionInPositiveAndNegativeParts.hxx"

#endif /* LIB_TFEL_MATH_STENSOR_HXX */
