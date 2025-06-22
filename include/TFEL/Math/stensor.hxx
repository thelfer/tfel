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
#include "TFEL/Metaprogramming/StaticAssert.hxx"
#include "TFEL/TypeTraits/IsScalar.hxx"
#include "TFEL/TypeTraits/BaseType.hxx"
#include "TFEL/TypeTraits/IsAssignableTo.hxx"
#include "TFEL/TypeTraits/IsSafelyReinterpretCastableTo.hxx"
#include "TFEL/Math/power.hxx"
#include "TFEL/Math/General/BasicOperations.hxx"
#include "TFEL/Math/General/EmptyRunTimeProperties.hxx"
#include "TFEL/Math/Vector/VectorUtilities.hxx"
#include "TFEL/Math/Vector/VectorConcept.hxx"
#include "TFEL/Math/Matrix/MatrixConcept.hxx"
#include "TFEL/Math/ST2toST2/ST2toST2Concept.hxx"
#include "TFEL/Math/Stensor/StensorConcept.hxx"
#include "TFEL/Math/Stensor/StensorConceptOperations.hxx"
#include "TFEL/Math/Forward/tvector.hxx"
#include "TFEL/Math/Forward/tmatrix.hxx"
#include "TFEL/Math/Forward/stensor.hxx"
#include "TFEL/Math/Forward/st2tost2.hxx"
#include "TFEL/Math/fsarray.hxx"

namespace tfel {

  namespace math {

    template <unsigned short N, typename T>
    struct TFEL_VISIBILITY_LOCAL StensorTraits<stensor<N, T>> {
      typedef T NumType;
      typedef unsigned short IndexType;
      static constexpr unsigned short dime = N;
    };

    /*!
     * \brief a base for stensor or classes acting like stensor.
     * \param Child : child class
     * \param N     : spatial dimension
     * \param T     : numerical type
     */
    template <typename Child>
    struct stensor_base {
      /*!
       * Assignement operator
       */
      template <typename StensorType>
      TFEL_MATH_INLINE typename std::enable_if<
          tfel::meta::Implements<StensorType,
                                 tfel::math::StensorConcept>::cond &&
              StensorTraits<Child>::dime == StensorTraits<StensorType>::dime &&
              tfel::typetraits::IsAssignableTo<StensorNumType<StensorType>,
                                               StensorNumType<Child>>::cond,
          Child&>::type
      operator=(const StensorType&);
      //! Assignement operator
      template <typename StensorType>
      TFEL_MATH_INLINE typename std::enable_if<
          tfel::meta::Implements<StensorType,
                                 tfel::math::StensorConcept>::cond &&
              StensorTraits<Child>::dime == StensorTraits<StensorType>::dime &&
              tfel::typetraits::IsAssignableTo<StensorNumType<StensorType>,
                                               StensorNumType<Child>>::cond,
          Child&>::type
      operator+=(const StensorType&);
      //! Assignement operator
      template <typename StensorType>
      TFEL_MATH_INLINE typename std::enable_if<
          tfel::meta::Implements<StensorType,
                                 tfel::math::StensorConcept>::cond &&
              StensorTraits<Child>::dime == StensorTraits<StensorType>::dime &&
              tfel::typetraits::IsAssignableTo<StensorNumType<StensorType>,
                                               StensorNumType<Child>>::cond,
          Child&>::type
      operator-=(const StensorType&);
      /*!
       * operator*=
       */
      template <typename T2>
      TFEL_MATH_INLINE typename std::enable_if<
          tfel::typetraits::IsScalar<T2>::cond &&
              std::is_same<
                  typename ResultType<StensorNumType<Child>, T2, OpMult>::type,
                  StensorNumType<Child>>::value,
          Child&>::type
      operator*=(const T2);
      /*!
       * operator/=
       */
      template <typename T2>
      TFEL_MATH_INLINE typename std::enable_if<
          tfel::typetraits::IsScalar<T2>::cond &&
              std::is_same<
                  typename ResultType<StensorNumType<Child>, T2, OpDiv>::type,
                  StensorNumType<Child>>::value,
          Child&>::type
      operator/=(const T2);
    };  // end of struct stensor_base

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

    template <unsigned short N, typename T>
    struct TFEL_VISIBILITY_LOCAL stensor
        : public StensorConcept<stensor<N, T>>,
          public stensor_common,
          public stensor_base<stensor<N, T>>,
          public fsarray<StensorDimeToSize<N>::value, T> {
      //! This is a StensorConcept requirement.
      typedef EmptyRunTimeProperties RunTimeProperties;
      //! \brief default constructor
      TFEL_MATH_INLINE explicit constexpr stensor() = default;
      /*!
       * \brief Default Constructor
       * \param[in] init: value used to initialise the components of the stensor
       */
      template <
          typename T2,
          typename std::enable_if<tfel::typetraits::IsAssignableTo<T2, T>::cond,
                                  bool>::type = true>
      TFEL_MATH_INLINE explicit constexpr stensor(const T2&);
      /*!
       * \brief Default Constructor
       * \param[in] init: values used to initialise the components of the vector
       */
      template <
          typename T2,
          typename std::enable_if<tfel::typetraits::IsAssignableTo<T2, T>::cond,
                                  bool>::type = true>
      TFEL_MATH_INLINE constexpr stensor(const std::initializer_list<T2>&);
      /*!
       * \brief Default Constructor.
       * \param const tfel::typetraits::base_type<T>*
       * const, pointer to a tabular used to initialise the components
       * of the stensor. This tabular is left unchanged.
       */
      template <typename InputIterator,
                typename std::enable_if<
                    std::is_same<typename std::iterator_traits<
                                     InputIterator>::value_type,
                                 tfel::typetraits::base_type<T>>::value,
                    bool>::type = true>
      TFEL_MATH_INLINE explicit stensor(const InputIterator);
      //! \brief copy constructor
      TFEL_MATH_INLINE constexpr stensor(const stensor<N, T>&) = default;
      /*!
       * \brief constructor
       */
      template <
          typename T2,
          typename std::enable_if<tfel::typetraits::IsAssignableTo<T2, T>::cond,
                                  bool>::type = true>
      TFEL_MATH_INLINE constexpr stensor(const stensor<N, T2>&);
      /*!
       * copy from stensor expression template object
       */
      template <
          typename T2,
          typename Operation,
          typename std::enable_if<tfel::typetraits::IsAssignableTo<T2, T>::cond,
                                  bool>::type = true>
      TFEL_MATH_INLINE stensor(const Expr<stensor<N, T2>, Operation>& src);
      //! copy assignement operator
      stensor& operator=(const stensor&) = default;
      /*!
       * \brief Import from Voigt
       */
      template <typename InputIterator>
      TFEL_MATH_INLINE2 typename std::enable_if<
          std::is_same<typename std::iterator_traits<InputIterator>::value_type,
                       tfel::typetraits::base_type<T>>::value,
          void>::type
      importVoigt(const InputIterator);
      /*!
       * Import from Tab (Voigt notations for stresses)
       */
      template <typename InputIterator>
      TFEL_MATH_INLINE2 typename std::enable_if<
          std::is_same<typename std::iterator_traits<InputIterator>::value_type,
                       tfel::typetraits::base_type<T>>::value,
          void>::type
      importTab(const InputIterator);
      /*!
       * Import values
       */
      template <typename InputIterator>
      TFEL_MATH_INLINE2 typename std::enable_if<
          std::is_same<typename std::iterator_traits<InputIterator>::value_type,
                       tfel::typetraits::base_type<T>>::value,
          void>::type
      import(const InputIterator);
      /*!
       * Export to Tab (Voigt notations for stresses)
       */
      template <typename T2>
      TFEL_MATH_INLINE2 typename std::enable_if<
          tfel::typetraits::IsSafelyReinterpretCastableTo<
              T2,
              tfel::typetraits::base_type<T>>::cond,
          void>::type
      exportTab(T2* const) const;
      /*!
       * Write to Tab
       */
      template <typename T2>
      TFEL_MATH_INLINE2 typename std::enable_if<
          tfel::typetraits::IsSafelyReinterpretCastableTo<
              T2,
              tfel::typetraits::base_type<T>>::cond,
          void>::type
      write(T2* const) const;
      //! using stensor_base::operator=
      using stensor_base<stensor>::operator=;
      /*!
       * compute eigenvalues
       * \tparam     es:  eigen solver
       * \param[out] vp0: first eigen value
       * \param[out] vp1: first eigen value
       * \param[out] vp2: first eigen value
       * \param[in]  b:   refine eigenvalues
       */
      template <EigenSolver = TFELEIGENSOLVER>
      TFEL_MATH_INLINE2 void computeEigenValues(T&,
                                                T&,
                                                T&,
                                                const bool = false) const;
      /*!
       * compute eigenvalues
       * \tparam     es: eigen solver
       * \param[out] vp: eigen values
       * \param[in]  b:  refine eigenvalues
       */
      template <EigenSolver = TFELEIGENSOLVER>
      TFEL_MATH_INLINE2 void computeEigenValues(tvector<3u, T>&,
                                                const bool = false) const;
      /*!
       * compute eigenvalues
       * \tparam    es: eigen solver
       * \param[in] b:  refine eigenvalues
       * \return eigen values
       */
      template <EigenSolver = TFELEIGENSOLVER>
      TFEL_MATH_INLINE2 tvector<3u, T> computeEigenValues(
          const bool = false) const;
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
      TFEL_MATH_INLINE2 void computeEigenValues(
          T&, T&, T&, const EigenValuesOrdering, const bool = false) const;
      /*!
       * compute eigenvalues
       * \tparam     es: eigen solver
       * \param[out] vp: eigen values
       * \param[in]  o:  eigenvalues ordering
       * \param[in]  b:  refine eigenvalues
       */
      template <EigenSolver = TFELEIGENSOLVER>
      TFEL_MATH_INLINE2 void computeEigenValues(tvector<3u, T>&,
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
      TFEL_MATH_INLINE2 tvector<3u, T> computeEigenValues(
          const EigenValuesOrdering, const bool = false) const;
      /*!
       * compute eigenvectors and eigenvalues
       * \tparam    es: eigen solver
       * \param[in] b:  refine eigenvalues
       */
      template <EigenSolver = TFELEIGENSOLVER>
      TFEL_MATH_INLINE2 std::tuple<tvector<3u, T>, rotation_matrix<T>>
      computeEigenVectors(const bool = false) const;
      /*!
       * compute eigenvectors and eigenvalues
       * \tparam     es: eigen solver
       * \param[out] vp : eigenvalues
       * \param[out] m  : rotation matrix
       * \param[in]  b  : refine eigenvalues
       */
      template <EigenSolver = TFELEIGENSOLVER>
      TFEL_MATH_INLINE2 void computeEigenVectors(tvector<3u, T>&,
                                                 rotation_matrix<T>&,
                                                 const bool = false) const;
      /*!
       * compute eigenvectors and eigenvalues
       * \tparam    es: eigen solver
       * \param[in] o:  eigenvalues ordering
       * \param[in] b: refine eigenvalues
       */
      template <EigenSolver = TFELEIGENSOLVER>
      TFEL_MATH_INLINE2 std::tuple<tvector<3u, T>, rotation_matrix<T>>
      computeEigenVectors(const EigenValuesOrdering, const bool = false) const;
      /*!
       * compute eigenvectors and eigenvalues
       * \tparam     es: eigen solver
       * \param[out] vp: eigenvalues
       * \param[out] m:  rotation matrix
       * \param[in]  o:  eigenvalues ordering
       * \param[in]  b:  refine eigenvalues
       */
      template <EigenSolver = TFELEIGENSOLVER>
      TFEL_MATH_INLINE2 void computeEigenVectors(tvector<3u, T>&,
                                                 rotation_matrix<T>&,
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
      TFEL_MATH_INLINE2 bool computeEigenVector(VectorType&, const T) const;
      /*!
       * change basis
       */
      TFEL_MATH_INLINE2 void changeBasis(const rotation_matrix<T>&);
      /*!
       * Return Identity
       */
      TFEL_MATH_INLINE static constexpr stensor<N, T> Id();

      TFEL_MATH_INLINE constexpr const T& operator()(
          const unsigned short) const noexcept;

      TFEL_MATH_INLINE T& operator()(const unsigned short) noexcept;
      // fsarray access operator
      using fsarray<StensorDimeToSize<N>::value, T>::operator[];
      /*!
       * \return the RunTimeProperties of the stensor
       */
      constexpr TFEL_MATH_INLINE RunTimeProperties
      getRunTimeProperties() const noexcept;
      /*!
       * copy the value from a container
       */
      template <typename InputIterator>
      TFEL_MATH_INLINE2 void copy(const InputIterator src);

      /*!
       * build a symmetric tensor from a matrix
       * \param[in] m : matrix
       */
      template <typename MatrixType>
      static TFEL_MATH_INLINE2 typename std::enable_if<
          tfel::typetraits::IsAssignableTo<MatrixNumType<MatrixType>, T>::cond,
          stensor<N, T>>::type
      buildFromMatrix(const MatrixType&);

      /*!
       * build a symmetric tensor from the diadic product of a vector with
       * itself \param[in] v : vector
       */
      template <typename VectorType>
      static TFEL_MATH_INLINE2 typename std::enable_if<
          tfel::typetraits::IsAssignableTo<
              typename ComputeUnaryResult<VectorNumType<VectorType>,
                                          Power<2>>::Result,
              T>::cond,
          stensor<N, T>>::type
      buildFromVectorDiadicProduct(const VectorType&);

      /*!
       * build a symmetric tensor from the symmetric diadic product of two
       * vectors \param[in] v1 : first  vector \param[in] v1 : second vector
       */
      template <typename VectorType, typename VectorType2>
      static TFEL_MATH_INLINE2 typename std::enable_if<
          tfel::typetraits::IsAssignableTo<
              typename ComputeBinaryResult<VectorNumType<VectorType>,
                                           VectorNumType<VectorType2>,
                                           OpMult>::Result,
              T>::cond,
          stensor<N, T>>::type
      buildFromVectorsSymmetricDiadicProduct(const VectorType&,
                                             const VectorType2&);

      /*!
       * build a symmetric tensor from its eigen values and vectors
       * \param[in] v1 : first  eigen value
       * \param[in] v2 : second eigen value
       * \param[in] v2 : third  eigen value
       * \param[in] m  : matrix containing the eigen vectors
       */
      static stensor<N, T> buildFromEigenValuesAndVectors(
          const T&, const T&, const T&, const rotation_matrix<T>&);

      /*!
       * build a symmetric tensor from its eigen values and vectors
       * \param[in] vp : eigen values
       * \param[in] m  : matrix containing the eigen vectors
       */
      static stensor<N, T> buildFromEigenValuesAndVectors(
          const tvector<3u, T>&, const rotation_matrix<T>&);

      /*!
       * build the logarithm of a symmetric tensor given through its eigen
       * values and vectors \param[in] v1 : first  eigen value \param[in] v2 :
       * second eigen value \param[in] v2 : third  eigen value \param[in] m  :
       * matrix containing the eigen vectors
       */
      static stensor<N, T> buildLogarithmFromEigenValuesAndVectors(
          const T&, const T&, const T&, const rotation_matrix<T>&);

      /*!
       * build the logarithm of a symmetric tensor given through its eigen
       * values and vectors \param[in] vp : eigen values \param[in] m  : matrix
       * containing the eigen vectors
       */
      static stensor<N, T> buildLogarithmFromEigenValuesAndVectors(
          const tvector<3u, T>&, const rotation_matrix<T>&);

      /*!
       * build the absolute value of a symmetric tensor given through its eigen
       * values and vectors \param[in] v1 : first  eigen value \param[in] v2 :
       * second eigen value \param[in] v2 : third  eigen value \param[in] m  :
       * matrix containing the eigen vectors
       */
      static stensor<N, T> buildAbsoluteValueFromEigenValuesAndVectors(
          const T&, const T&, const T&, const rotation_matrix<T>&);

      /*!
       * build the absolute value of a symmetric tensor given through its eigen
       * values and vectors \param[in] vp : eigen values \param[in] m  : matrix
       * containing the eigen vectors
       */
      static stensor<N, T> buildAbsoluteValueFromEigenValuesAndVectors(
          const tvector<3u, T>&, const rotation_matrix<T>&);

      /*!
       * build the positive part of a symmetric tensor given through its eigen
       * values and vectors \param[in] v1 : first  eigen value \param[in] v2 :
       * second eigen value \param[in] v2 : third  eigen value \param[in] m  :
       * matrix containing the eigen vectors
       */
      static stensor<N, T> buildPositivePartFromEigenValuesAndVectors(
          const T&, const T&, const T&, const rotation_matrix<T>&);
      /*!
       * build the positive part of a symmetric tensor given through its eigen
       * values and vectors \param[in] vp : eigen values \param[in] m  : matrix
       * containing the eigen vectors
       */
      static stensor<N, T> buildPositivePartFromEigenValuesAndVectors(
          const tvector<3u, T>&, const rotation_matrix<T>&);
      /*!
       * build the positive part of a symmetric tensor given through its eigen
       * values and vectors \param[in] v1 : first  eigen value \param[in] v2 :
       * second eigen value \param[in] v2 : third  eigen value \param[in] m  :
       * matrix containing the eigen vectors
       */
      static stensor<N, T> buildNegativePartFromEigenValuesAndVectors(
          const T&, const T&, const T&, const rotation_matrix<T>&);

      /*!
       * build the negative part of a symmetric tensor given through its eigen
       * values and vectors \param[in] vp : eigen values \param[in] m  : matrix
       * containing the eigen vectors
       */
      static stensor<N, T> buildNegativePartFromEigenValuesAndVectors(
          const tvector<3u, T>&, const rotation_matrix<T>&);
      /*!
       * compute the eigenvalues derivatives with respect with this tensor
       * \param[out] n : derivatives of the  eigenvalues
       * \param[in]  m  : eigenvectors
       * This eigenvalues of the derivatives is given by~:
       * \f[
       * \frac{\displaystyle \partial}{\lambda_{i}}{\displaystyle
       * \underbrace{s}}=\underbrace{n}_{i}=\vec{e}_{i}\otimes\vec{e}_{i} \f]
       * \(\underbrace{n}_{i}\) are the eigen tensors associated to the given
       * tensor.
       */
      std::tuple<stensor<N, T>, stensor<N, T>, stensor<N, T>>
      computeEigenValuesDerivatives(const rotation_matrix<T>&);
      /*!
       * compute the eigenvalues derivatives with respect with this tensor
       * \param[out] n0 : derivative of the first eigenvalue
       * \param[out] n1 : derivative of the second eigenvalue
       * \param[out] n2 : derivative of the third  eigenvalue
       * \param[in]  m  : eigenvectors
       * This eigenvalues of the derivatives is given by~:
       * \f[
       * \frac{\displaystyle \partial}{\lambda_{i}}{\displaystyle
       * \underbrace{s}}=\underbrace{n}_{i}=\vec{e}_{i}\otimes\vec{e}_{i} \f]
       * \f$\underbrace{n}_{i}\f$ are the eigen tensors associated to the given
       * tensor.
       */
      template <typename StensorType>
      static typename std::enable_if<
          (tfel::meta::Implements<StensorType,
                                  tfel::math::StensorConcept>::cond) &&
              (StensorTraits<StensorType>::dime == N) &&
              (tfel::typetraits::IsAssignableTo<
                  tfel::typetraits::base_type<T>,
                  StensorNumType<StensorType>>::cond),
          void>::type
      computeEigenValuesDerivatives(StensorType&,
                                    StensorType&,
                                    StensorType&,
                                    const rotation_matrix<T>&);
      /*!
       * \brief compute the "eigentensors"
       * \param[out] n: derivativse of the eigenvalues
       * \param[in]  m: eigenvectors
       */
      static std::tuple<stensor<N, T>, stensor<N, T>, stensor<N, T>>
      computeEigenTensors(const rotation_matrix<T>&);
      /*!
       * compute the "eigentensors"
       * \param[out] n0: derivative of the first  eigenvalue
       * \param[out] n1: derivative of the second eigenvalue
       * \param[out] n2: derivative of the third  eigenvalue
       * \param[in]  m:  eigenvectors
       */
      template <typename StensorType>
      static typename std::enable_if<
          (tfel::meta::Implements<StensorType,
                                  tfel::math::StensorConcept>::cond) &&
              (StensorTraits<StensorType>::dime == N) &&
              (tfel::typetraits::IsAssignableTo<
                  tfel::typetraits::base_type<T>,
                  StensorNumType<StensorType>>::cond),
          void>::type
      computeEigenTensors(StensorType&,
                          StensorType&,
                          StensorType&,
                          const rotation_matrix<T>&);
      /*!
       * \brief compute the "eigentensors" derivatives
       * \param[out] dn0_ds: derivative of the first  eigenvalue
       * \param[out] dn1_ds: derivative of the second eigenvalue
       * \param[out] dn2_ds: derivative of the third  eigenvalue
       * \param[in]  vp:     eigen values
       * \param[in]  m:      eigen vectors
       * \param[in]  eps:    numerical parameter for regularisation
       */
      template <typename ST2toST2Type>
      static typename std::enable_if<
          (tfel::meta::Implements<ST2toST2Type, ST2toST2Concept>::cond) &&
              (ST2toST2Traits<ST2toST2Type>::dime == N) &&
              (tfel::typetraits::IsAssignableTo<
                  typename ComputeBinaryResult<tfel::typetraits::base_type<T>,
                                               T,
                                               OpDiv>::Result,
                  ST2toST2NumType<ST2toST2Type>>::cond),
          void>::type
      computeEigenTensorsDerivatives(ST2toST2Type&,
                                     ST2toST2Type&,
                                     ST2toST2Type&,
                                     const tvector<3u, T>&,
                                     const rotation_matrix<T>&,
                                     const T);
      /*!
       * compute an isotropic function
       * \param[in]  f:   function
       * \param[in]  vp:  eigen values
       * \param[in]  m:   eigenvectors
       */
      template <typename Function>
      static stensor<N, typename std::result_of<Function(T)>::type>
      computeIsotropicFunction(const Function&,
                               const tvector<3u, T>&,
                               const rotation_matrix<T>&);
      /*!
       * compute an isotropic function
       * \param[in]  f: function values for each eigen values
       * \param[in]  m: eigenvectors
       */
      template <typename T2>
      static stensor<N, T2> computeIsotropicFunction(const tvector<3u, T2>&,
                                                     const rotation_matrix<T>&);
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
      static st2tost2<N, T2> computeIsotropicFunctionDerivative(
          const tvector<3u, T1>&,
          const tvector<3u, T2>&,
          const tvector<3u, T>&,
          const rotation_matrix<T>&,
          const T);
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
      static typename std::enable_if<
          (tfel::meta::Implements<ST2toST2Type, ST2toST2Concept>::cond) &&
              (ST2toST2Traits<ST2toST2Type>::dime == N) &&
              (tfel::typetraits::IsAssignableTo<
                  typename ComputeBinaryResult<tfel::typetraits::base_type<T>,
                                               T,
                                               OpDiv>::Result,
                  ST2toST2NumType<ST2toST2Type>>::cond),
          void>::type
      computeIsotropicFunctionDerivative(ST2toST2Type&,
                                         const tvector<3u, T1>&,
                                         const tvector<3u, T2>&,
                                         const tvector<3u, T>&,
                                         const rotation_matrix<T>&,
                                         const T);
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
      static st2tost2<N, typename std::result_of<FunctionDerivative(T)>::type>
      computeIsotropicFunctionDerivative(const Function&,
                                         const FunctionDerivative&,
                                         const tvector<3u, T>&,
                                         const rotation_matrix<T>&,
                                         const T);
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
      static typename std::enable_if<
          (tfel::meta::Implements<ST2toST2Type, ST2toST2Concept>::cond) &&
              (ST2toST2Traits<ST2toST2Type>::dime == N) &&
              (tfel::typetraits::IsAssignableTo<
                  typename ComputeBinaryResult<tfel::typetraits::base_type<T>,
                                               T,
                                               OpDiv>::Result,
                  ST2toST2NumType<ST2toST2Type>>::cond),
          void>::type
      computeIsotropicFunctionDerivative(ST2toST2Type&,
                                         const Function&,
                                         const FunctionDerivative&,
                                         const tvector<3u, T>&,
                                         const rotation_matrix<T>&,
                                         const T);
      /*!
       * \return the value of an isotropic function
       * \param[in] f: function
       * \param[in] b:   if true, refinement of eigen values is performed
       */
      template <EigenSolver = TFELEIGENSOLVER, typename Function>
      stensor<N, typename std::result_of<Function(T)>::type>
      computeIsotropicFunction(const Function&, const bool = false) const;
      /*!
       * \return the derivative of an isotropic function
       * \param[in] f:   function
       * \param[in] df:  derivative of the function
       * \param[in] eps: criterion value used to judge if two eigenvalues are
       * equals \param[in] b:   if true, refinement of eigen values is performed
       */
      template <EigenSolver = TFELEIGENSOLVER,
                typename Function,
                typename FunctionDerivative>
      st2tost2<N, typename std::result_of<FunctionDerivative(T)>::type>
      computeIsotropicFunctionDerivative(const Function&,
                                         const FunctionDerivative&,
                                         const T,
                                         const bool = false) const;
      /*!
       * \return the derivative of an isotropic function
       * \param[in] f:   function
       * \param[in] df:  derivative of the function
       * \param[in] eps: criterion value used to judge if two eigenvalues are
       * equals \param[in] b:   if true, refinement of eigen values is performed
       */
      template <EigenSolver = TFELEIGENSOLVER,
                typename Function,
                typename FunctionDerivative>
      std::pair<
          stensor<N, typename std::result_of<Function(T)>::type>,
          st2tost2<N, typename std::result_of<FunctionDerivative(T)>::type>>
      computeIsotropicFunctionAndDerivative(const Function&,
                                            const FunctionDerivative&,
                                            const T,
                                            const bool = false) const;

     private:
      //! a simple check
      TFEL_STATIC_ASSERT((N == 1u) || (N == 2u) || (N == 3u));
    };  // end of class stensor

    /*!
     * export the given vector to an array of the
     */
    template <unsigned short N, typename T, typename OutputIterator>
    TFEL_MATH_INLINE2
        typename std::enable_if<tfel::typetraits::IsScalar<T>::cond, void>::type
        exportToBaseTypeArray(const stensor<N, T>&, OutputIterator);

    /*!
     * compute the tresca stress
     * for a symmetric tensor
     *
     * \param s : symmetric tensor
     */
    template <unsigned short N, typename T>
    T tresca(const stensor<N, T>&, const bool = false);

    /*!
     * compute the tresca stress
     * for a symmetric tensor.
     *
     * Partial specialisation in 1D
     *
     * \param s : symmetric tensor
     */
    template <typename T>
    T tresca(const stensor<1u, T>&, const bool = false);

    template <typename StensorType>
    typename std::enable_if<
        tfel::meta::Implements<StensorType, StensorConcept>::cond,
        stensor<StensorTraits<StensorType>::dime,
                StensorNumType<StensorType>>>::type
    square_root(const StensorType&);

    template <typename StensorType>
    typename std::enable_if<
        tfel::meta::Implements<StensorType, StensorConcept>::cond,
        typename ComputeUnaryResult<StensorNumType<StensorType>,
                                    Power<3>>::Result>::type
    det(const StensorType&);
    /*!
     * \return the derivative of the determinant
     * \param[in] s: tensor where the the determinant is evaluated
     */
    template <typename StensorType>
    typename std::enable_if<
        tfel::meta::Implements<StensorType, StensorConcept>::cond,
        stensor<StensorTraits<StensorType>::dime,
                typename ComputeUnaryResult<StensorNumType<StensorType>,
                                            Power<2>>::Result>>::type
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
    typename std::enable_if<
        tfel::meta::Implements<StensorType, StensorConcept>::cond,
        stensor<StensorTraits<StensorType>::dime,
                typename ComputeUnaryResult<StensorNumType<StensorType>,
                                            Power<2>>::Result>>::type
    computeDeviatorDeterminantDerivative(const StensorType&);
    /*!
     * \brief rotate a symmetric tensor using a rotation matrix
     * \param[in] s: symmetric tensor to be rotated
     * \param[in] r: rotation matrix
     * \return the rotated symmetric tensor
     */
    template <typename StensorType>
    TFEL_MATH_INLINE2 typename std::enable_if<
        tfel::meta::Implements<StensorType, StensorConcept>::cond,
        stensor<StensorTraits<StensorType>::dime,
                StensorNumType<StensorType>>>::type
    change_basis(const StensorType&,
                 const rotation_matrix<StensorNumType<StensorType>>&);

    template <typename StensorType>
    TFEL_MATH_INLINE2 typename std::enable_if<
        tfel::meta::Implements<StensorType, StensorConcept>::cond,
        stensor<StensorTraits<StensorType>::dime,
                typename ComputeBinaryResult<
                    tfel::typetraits::base_type<StensorNumType<StensorType>>,
                    StensorNumType<StensorType>,
                    OpDiv>::Result>>::type
    invert(const StensorType&);
    /*!
     * \brief compute the logarithm of a symmetric tensor
     * \param s : tensor
     * \param b : if true, refinement of eigen values is performed
     */
    template <typename StensorType>
    TFEL_MATH_INLINE2 typename std::enable_if<
        ((tfel::meta::Implements<StensorType, StensorConcept>::cond) &&
         (StensorTraits<StensorType>::dime == 1u) &&
         (tfel::typetraits::IsFundamentalNumericType<
             StensorNumType<StensorType>>::cond)),
        stensor<1u, StensorNumType<StensorType>>>::type
    logarithm(const StensorType&, const bool = false);
    /*!
     * \brief compute the logarithm of a symmetric tensor
     * \param s : tensor
     * \param b : if true, refinement of eigen values is performed
     */
    template <typename StensorType>
    TFEL_MATH_INLINE2 typename std::enable_if<
        ((tfel::meta::Implements<StensorType, StensorConcept>::cond) &&
         ((StensorTraits<StensorType>::dime == 2u) ||
          (StensorTraits<StensorType>::dime == 3u)) &&
         (tfel::typetraits::IsFundamentalNumericType<
             StensorNumType<StensorType>>::cond)),
        stensor<StensorTraits<StensorType>::dime,
                StensorNumType<StensorType>>>::type
    logarithm(const StensorType&, const bool = false);
    /*!
     * \brief compute the absolute value of a symmetric tensor
     * \param s : tensor
     * \param b : if true, refinement of eigen values is performed
     */
    template <typename StensorType>
    typename std::enable_if<
        ((tfel::meta::Implements<StensorType, StensorConcept>::cond) &&
         (StensorTraits<StensorType>::dime == 1u) &&
         (tfel::typetraits::IsFundamentalNumericType<
             StensorNumType<StensorType>>::cond)),
        stensor<1u, StensorNumType<StensorType>>>::type
    absolute_value(const StensorType&, const bool = false);
    /*!
     * \brief compute the absolute value of a symmetric tensor
     * \param s : tensor
     * \param b : if true, refinement of eigen values is performed
     */
    template <typename StensorType>
    typename std::enable_if<
        ((tfel::meta::Implements<StensorType, StensorConcept>::cond) &&
         ((StensorTraits<StensorType>::dime == 2u) ||
          (StensorTraits<StensorType>::dime == 3u)) &&
         (tfel::typetraits::IsFundamentalNumericType<
             StensorNumType<StensorType>>::cond)),
        stensor<StensorTraits<StensorType>::dime,
                StensorNumType<StensorType>>>::type
    absolute_value(const StensorType&, const bool = false);
    /*!
     * \brief compute the positive part of a symmetric tensor
     * \param s : tensor
     * \param b : if true, refinement of eigen values is performed
     */
    template <typename StensorType>
    typename std::enable_if<
        ((tfel::meta::Implements<StensorType, StensorConcept>::cond) &&
         (StensorTraits<StensorType>::dime == 1u) &&
         (tfel::typetraits::IsFundamentalNumericType<
             StensorNumType<StensorType>>::cond)),
        stensor<1u, StensorNumType<StensorType>>>::type
    positive_part(const StensorType&, const bool = false);
    /*!
     * \brief compute the positive part of a symmetric tensor
     * \param s : tensor
     * \param b : if true, refinement of eigen values is performed
     */
    template <typename StensorType>
    typename std::enable_if<
        ((tfel::meta::Implements<StensorType, StensorConcept>::cond) &&
         ((StensorTraits<StensorType>::dime == 2u) ||
          (StensorTraits<StensorType>::dime == 3u)) &&
         (tfel::typetraits::IsFundamentalNumericType<
             StensorNumType<StensorType>>::cond)),
        stensor<StensorTraits<StensorType>::dime,
                StensorNumType<StensorType>>>::type
    positive_part(const StensorType&, const bool = false);
    /*!
     * \brief compute the negative part of a symmetric tensor
     * \param s : tensor
     * \param b : if true, refinement of eigen values is performed
     */
    template <typename StensorType>
    typename std::enable_if<
        ((tfel::meta::Implements<StensorType, StensorConcept>::cond) &&
         (StensorTraits<StensorType>::dime == 1u) &&
         (tfel::typetraits::IsFundamentalNumericType<
             StensorNumType<StensorType>>::cond)),
        stensor<1u, StensorNumType<StensorType>>>::type
    negative_part(const StensorType&, const bool = false);
    /*!
     * \brief compute the negative part of a symmetric tensor
     * \param s : tensor
     * \param b : if true, refinement of eigen values is performed
     */
    template <typename StensorType>
    typename std::enable_if<
        ((tfel::meta::Implements<StensorType, StensorConcept>::cond) &&
         ((StensorTraits<StensorType>::dime == 2u) ||
          (StensorTraits<StensorType>::dime == 3u)) &&
         (tfel::typetraits::IsFundamentalNumericType<
             StensorNumType<StensorType>>::cond)),
        stensor<StensorTraits<StensorType>::dime,
                StensorNumType<StensorType>>>::type
    negative_part(const StensorType&, const bool = false);
    /*!
     * \return the square of a symmetric stensor
     * \param[in] s : squared tensor
     */
    template <typename StensorType>
    typename std::enable_if<
        tfel::meta::Implements<StensorType, StensorConcept>::cond &&
            StensorTraits<StensorType>::dime == 1u,
        stensor<1u,
                typename ComputeBinaryResult<StensorNumType<StensorType>,
                                             StensorNumType<StensorType>,
                                             OpMult>::Result>>::type
    square(const StensorType&);
    /*!
     * \return the square of a symmetric stensor
     * \param[in] s : squared tensor
     */
    template <typename StensorType>
    typename std::enable_if<
        tfel::meta::Implements<StensorType, StensorConcept>::cond &&
            StensorTraits<StensorType>::dime == 2u,
        stensor<2u,
                typename ComputeBinaryResult<StensorNumType<StensorType>,
                                             StensorNumType<StensorType>,
                                             OpMult>::Result>>::type
    square(const StensorType&);
    /*!
     * \return the square of a symmetric stensor
     * \param[in] s : squared tensor
     */
    template <typename StensorType>
    typename std::enable_if<
        tfel::meta::Implements<StensorType, StensorConcept>::cond &&
            StensorTraits<StensorType>::dime == 3u,
        stensor<3u,
                typename ComputeBinaryResult<StensorNumType<StensorType>,
                                             StensorNumType<StensorType>,
                                             OpMult>::Result>>::type
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
    typename std::enable_if<
        tfel::meta::Implements<StensorType, StensorConcept>::cond,
        stensor<StensorTraits<StensorType>::dime,
                StensorNumType<StensorType>>>::type
    computeIsotropicFunction(const Function&,
                             const StensorType&,
                             const bool = false);
    /*!
     * \return the derivative of an isotropic function
     * \param[in] f:   function
     * \param[in] df:  derivative of the function
     * \param[in] s:   symmetric tensor
     * \param[in] eps: criterion value used to judge if two eigenvalues are
     * equals \param[in] b:   if true, refinement of eigen values is performed
     */
    template <
        typename stensor_common::EigenSolver = stensor_common::TFELEIGENSOLVER,
        typename Function,
        typename FunctionDerivative,
        typename StensorType>
    typename std::enable_if<
        tfel::meta::Implements<StensorType, StensorConcept>::cond,
        st2tost2<StensorTraits<StensorType>::dime,
                 StensorNumType<StensorType>>>::type
    computeIsotropicFunctionDerivative(const Function&,
                                       const FunctionDerivative&,
                                       const StensorType&,
                                       const StensorNumType<StensorType>,
                                       const bool = false);
    /*!
     * \return the derivative of an isotropic function
     * \param[in] f:   function
     * \param[in] df:  derivative of the function
     * \param[in] s:   symmetric tensor
     * \param[in] eps: criterion value used to judge if two eigenvalues are
     * equals \param[in] b:   if true, refinement of eigen values is performed
     */
    template <
        typename stensor_common::EigenSolver = stensor_common::TFELEIGENSOLVER,
        typename Function,
        typename FunctionDerivative,
        typename StensorType>
    typename std::enable_if<
        tfel::meta::Implements<StensorType, StensorConcept>::cond,
        std::pair<stensor<StensorTraits<StensorType>::dime,
                          StensorNumType<StensorType>>,
                  st2tost2<StensorTraits<StensorType>::dime,
                           StensorNumType<StensorType>>>>::type
    computeIsotropicFunctionDerivative(const Function&,
                                       const FunctionDerivative&,
                                       const StensorType&,
                                       const StensorNumType<StensorType>,
                                       const bool = false);
    /*!
     * \brief convert the corotationnal cauchy stress to the second
     * Piola-Kirchhoff stress \f[ \underline{S} =
     * J\,\underline{U}^{-1}\,.\,\underline{\sigma}\,.\,\underline{U}^{-1} \f]
     * \param[in] s: corotationnal cauchy stress
     * \param[in] U: stretch tensor
     * \return the second Piola-Kirchhoff stress
     */
    template <typename T, typename T2>
    TFEL_MATH_INLINE2 typename std::enable_if<
        ((tfel::meta::Implements<T, StensorConcept>::cond) &&
         (StensorTraits<T>::dime == 1u) &&
         (tfel::meta::Implements<T2, StensorConcept>::cond) &&
         (StensorTraits<T2>::dime == 1u) &&
         (tfel::typetraits::IsFundamentalNumericType<
             StensorNumType<T2>>::cond)),
        stensor<1u, StensorNumType<T>>>::type
    convertCorotationnalCauchyStressToSecondPiolaKirchhoffStress(const T&,
                                                                 const T2&);
    /*!
     * \brief convert the corotationnal cauchy stress to the second
     * Piola-Kirchhoff stress \f[ \underline{S} =
     * J\,\underline{U}^{-1}\,.\,\underline{\sigma}\,.\,\underline{U}^{-1} \f]
     * \param[in] s: corotationnal cauchy stress
     * \param[in] U: stretch tensor
     * \return the second Piola-Kirchhoff stress
     */
    template <typename T, typename T2>
    TFEL_MATH_INLINE2 typename std::enable_if<
        ((tfel::meta::Implements<T, StensorConcept>::cond) &&
         (StensorTraits<T>::dime == 2u) &&
         (tfel::meta::Implements<T2, StensorConcept>::cond) &&
         (StensorTraits<T2>::dime == 2u) &&
         (tfel::typetraits::IsFundamentalNumericType<
             StensorNumType<T2>>::cond)),
        stensor<2u, StensorNumType<T>>>::type
    convertCorotationnalCauchyStressToSecondPiolaKirchhoffStress(const T&,
                                                                 const T2&);
    /*!
     * \brief convert the corotationnal cauchy stress to the second
     * Piola-Kirchhoff stress \f[ \underline{S} =
     * J\,\underline{U}^{-1}\,.\,\underline{\sigma}\,.\,\underline{U}^{-1} \f]
     * \param[in] s: corotationnal cauchy stress
     * \param[in] U: stretch tensor
     * \return the second Piola-Kirchhoff stress
     */
    template <typename T, typename T2>
    TFEL_MATH_INLINE2 typename std::enable_if<
        ((tfel::meta::Implements<T, StensorConcept>::cond) &&
         (StensorTraits<T>::dime == 3u) &&
         (tfel::meta::Implements<T2, StensorConcept>::cond) &&
         (StensorTraits<T2>::dime == 3u) &&
         (tfel::typetraits::IsFundamentalNumericType<
             StensorNumType<T2>>::cond)),
        stensor<3u, StensorNumType<T>>>::type
    convertCorotationnalCauchyStressToSecondPiolaKirchhoffStress(const T&,
                                                                 const T2&);
    /*!
     * \brief convert the second Piola-Kirchhoff stress to the
     * corotationnal cauchy stress:
     * \f[
     * \underline{S} =
     * J\,\underline{U}^{-1}\,.\,\underline{\sigma}\,.\,\underline{U}^{-1} \f]
     * \param[in] S: the second Piola-Kirchhoff stress
     * \param[in] U: stretch tensor
     * \return the corotationnal cauchy stress
     */
    template <typename T, typename T2>
    TFEL_MATH_INLINE2 typename std::enable_if<
        ((tfel::meta::Implements<T, StensorConcept>::cond) &&
         (StensorTraits<T>::dime == 1u) &&
         (tfel::meta::Implements<T2, StensorConcept>::cond) &&
         (StensorTraits<T2>::dime == 1u) &&
         (tfel::typetraits::IsFundamentalNumericType<
             StensorNumType<T2>>::cond)),
        stensor<1u, StensorNumType<T>>>::type
    convertSecondPiolaKirchhoffStressToCorotationnalCauchyStress(const T&,
                                                                 const T2&);
    /*!
     * \brief convert the second Piola-Kirchhoff stress to the
     * corotationnal cauchy stress:
     * \f[
     * \underline{S} =
     * J\,\underline{U}^{-1}\,.\,\underline{\sigma}\,.\,\underline{U}^{-1} \f]
     * \param[in] S: the second Piola-Kirchhoff stress
     * \param[in] U: stretch tensor
     * \return the corotationnal cauchy stress
     */
    template <typename T, typename T2>
    TFEL_MATH_INLINE2 typename std::enable_if<
        ((tfel::meta::Implements<T, StensorConcept>::cond) &&
         (StensorTraits<T>::dime == 2u) &&
         (tfel::meta::Implements<T2, StensorConcept>::cond) &&
         (StensorTraits<T2>::dime == 2u) &&
         (tfel::typetraits::IsFundamentalNumericType<
             StensorNumType<T2>>::cond)),
        stensor<2u, StensorNumType<T>>>::type
    convertSecondPiolaKirchhoffStressToCorotationnalCauchyStress(const T&,
                                                                 const T2&);
    /*!
     * \brief convert the second Piola-Kirchhoff stress to the
     * corotationnal cauchy stress:
     * \f[
     * \underline{S} =
     * J\,\underline{U}^{-1}\,.\,\underline{\sigma}\,.\,\underline{U}^{-1} \f]
     * \param[in] S: the second Piola-Kirchhoff stress
     * \param[in] U: stretch tensor
     * \return the corotationnal cauchy stress
     */
    template <typename T, typename T2>
    TFEL_MATH_INLINE2 typename std::enable_if<
        ((tfel::meta::Implements<T, StensorConcept>::cond) &&
         (StensorTraits<T>::dime == 3u) &&
         (tfel::meta::Implements<T2, StensorConcept>::cond) &&
         (StensorTraits<T2>::dime == 3u) &&
         (tfel::typetraits::IsFundamentalNumericType<
             StensorNumType<T2>>::cond)),
        stensor<3u, StensorNumType<T>>>::type
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
    typename std::enable_if<
        ((tfel::meta::Implements<StensorType1, StensorConcept>::cond) &&
         (tfel::meta::Implements<StensorType2, StensorConcept>::cond) &&
         (StensorTraits<StensorType1>::dime == 1u) &&
         (StensorTraits<StensorType2>::dime == 1u)),
        stensor<3u,
                typename ResultType<StensorNumType<StensorType1>,
                                    StensorNumType<StensorType2>,
                                    OpMult>::type>>::type
    symmetric_product(const StensorType1&, const StensorType2&);
    /*!
     * \brief return the symmetric product of two stensors as a symmetric
     * tensor: \f[ s1*s2+s2*s1 \f] \param[in] s1: first  tensor \param[in] s2:
     * second tensor
     */
    template <typename StensorType1, typename StensorType2>
    typename std::enable_if<
        ((tfel::meta::Implements<StensorType1, StensorConcept>::cond) &&
         (tfel::meta::Implements<StensorType2, StensorConcept>::cond) &&
         (StensorTraits<StensorType1>::dime == 2u) &&
         (StensorTraits<StensorType2>::dime == 2u)),
        stensor<2u,
                typename ResultType<StensorNumType<StensorType1>,
                                    StensorNumType<StensorType2>,
                                    OpMult>::type>>::type
    symmetric_product(const StensorType1&, const StensorType2&);
    /*!
     * \brief return the symmetric product of two stensors as a symmetric tensor
     * \f[
     * s1*s2+s2*s1
     * \f]
     * \param[in] s1: first  tensor
     * \param[in] s2: second tensor
     */
    template <typename StensorType1, typename StensorType2>
    typename std::enable_if<
        ((tfel::meta::Implements<StensorType1, StensorConcept>::cond) &&
         (tfel::meta::Implements<StensorType2, StensorConcept>::cond) &&
         (StensorTraits<StensorType1>::dime == 3u) &&
         (StensorTraits<StensorType2>::dime == 3u)),
        stensor<3u,
                typename ResultType<StensorNumType<StensorType1>,
                                    StensorNumType<StensorType2>,
                                    OpMult>::type>>::type
    symmetric_product(const StensorType1&, const StensorType2&);

  }  // end of namespace math

  namespace typetraits {

    template <unsigned short N, typename T2, typename T>
    struct IsAssignableTo<tfel::math::stensor<N, T2>,
                          tfel::math::stensor<N, T>> {
      /*!
       *  Result
       */
      static constexpr bool cond = IsAssignableTo<T2, T>::cond;
    };

  }  // end of namespace typetraits

}  // end of namespace tfel

#include "TFEL/Math/Stensor/StensorSizeToDime.hxx"
#include "TFEL/Math/Stensor/stensor.ixx"
#include "TFEL/Math/Stensor/stensorResultType.hxx"
#include "TFEL/Math/Stensor/DecompositionInPositiveAndNegativeParts.hxx"

#endif /* LIB_TFEL_MATH_STENSOR_HXX */
