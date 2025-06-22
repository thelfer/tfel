/*!
 * \file   include/TFEL/Math/tensor.hxx
 * \brief
 * \author Thomas Helfer
 * \date   01 Déc 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_TENSOR_HXX
#define LIB_TFEL_MATH_TENSOR_HXX

#include <cstddef>
#include <initializer_list>
#include <type_traits>

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Metaprogramming/StaticAssert.hxx"

#include "TFEL/TypeTraits/IsScalar.hxx"
#include "TFEL/TypeTraits/BaseType.hxx"
#include "TFEL/TypeTraits/IsAssignableTo.hxx"
#include "TFEL/TypeTraits/IsSafelyReinterpretCastableTo.hxx"

#include "TFEL/FSAlgorithm/FSAlgorithm.hxx"

#include "TFEL/Math/General/BasicOperations.hxx"
#include "TFEL/Math/General/EmptyRunTimeProperties.hxx"

#include "TFEL/Math/fsarray.hxx"
#include "TFEL/Math/Vector/VectorUtilities.hxx"
#include "TFEL/Math/Tensor/TensorSizeToDime.hxx"
#include "TFEL/Math/Tensor/TensorConcept.hxx"
#include "TFEL/Math/Tensor/TensorConceptOperations.hxx"

#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/tmatrix.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/Forward/tensor.hxx"

namespace tfel {

  namespace math {

    template <unsigned short N, typename T>
    struct TFEL_VISIBILITY_LOCAL TensorTraits<tensor<N, T>> {
      typedef T NumType;
      typedef unsigned short IndexType;
      static constexpr unsigned short dime = N;
    };

    /*!
     * \brief a base for tensor or classes acting like tensor.
     * \param Child : child class
     * \param N     : spatial dimension
     * \param T     : numerical type
     */
    template <typename Child>
    struct tensor_base {
      /*!
       * Assignement operator
       */
      template <typename TensorType>
      TFEL_MATH_INLINE typename std::enable_if<
          tfel::meta::Implements<TensorType, TensorConcept>::cond &&
              TensorTraits<Child>::dime == TensorTraits<TensorType>::dime &&
              tfel::typetraits::IsAssignableTo<TensorNumType<TensorType>,
                                               TensorNumType<Child>>::cond,
          Child&>::type
      operator=(const TensorType&);
      /*!
       * Assignement operator
       */
      template <typename T>
      TFEL_MATH_INLINE typename std::enable_if<
          tfel::typetraits::IsAssignableTo<T, TensorNumType<Child>>::cond,
          Child&>::type
      operator=(const std::initializer_list<T>&);
      //! Assignement operator
      template <typename TensorType>
      TFEL_MATH_INLINE typename std::enable_if<
          tfel::meta::Implements<TensorType, TensorConcept>::cond &&
              TensorTraits<Child>::dime == TensorTraits<TensorType>::dime &&
              tfel::typetraits::IsAssignableTo<TensorNumType<TensorType>,
                                               TensorNumType<Child>>::cond,
          Child&>::type
      operator+=(const TensorType&);
      //! Assignement operator
      template <typename TensorType>
      TFEL_MATH_INLINE typename std::enable_if<
          tfel::meta::Implements<TensorType, TensorConcept>::cond &&
              TensorTraits<Child>::dime == TensorTraits<TensorType>::dime &&
              tfel::typetraits::IsAssignableTo<TensorNumType<TensorType>,
                                               TensorNumType<Child>>::cond,
          Child&>::type
      operator-=(const TensorType&);
      /*!
       * operator*=
       */
      template <typename T2>
      TFEL_MATH_INLINE typename std::enable_if<
          tfel::typetraits::IsScalar<T2>::cond &&
              std::is_same<
                  typename ResultType<TensorNumType<Child>, T2, OpMult>::type,
                  TensorNumType<Child>>::value,
          Child&>::type
      operator*=(const T2);
      /*!
       * operator/=
       */
      template <typename T2>
      TFEL_MATH_INLINE typename std::enable_if<
          tfel::typetraits::IsScalar<T2>::cond &&
              std::is_same<
                  typename ResultType<TensorNumType<Child>, T2, OpDiv>::type,
                  TensorNumType<Child>>::value,
          Child&>::type
      operator/=(const T2);
    };  // end of struct tensor_base

    template <unsigned short N, typename T>
    struct tensor : public TensorConcept<tensor<N, T>>,
                    public tensor_base<tensor<N, T>>,
                    public fsarray<TensorDimeToSize<N>::value, T> {
      /*
       * This is a TensorConcept requirement.
       */
      typedef EmptyRunTimeProperties RunTimeProperties;
      /*!
       * \brief Build a tensor from a fortran matrix.
       * \param[in] t: tensor to be filled
       * \param[in] v: pointer to an array used to initialise the
       * components of the tensor. This array is left unchanged.
       */
      TFEL_MATH_INLINE2 static void buildFromFortranMatrix(
          tensor<N, T>&, const tfel::typetraits::base_type<T>* const);
      /*!
       * \brief Build a tensor from a fortran matrix.
       * \param[in] t: tensor to be filled
       * \param[in] v: pointer to an array used to initialise the
       * components of the tensor. This array is left unchanged.
       */
      TFEL_MATH_INLINE2 static tensor<N, T> buildFromFortranMatrix(
          const tfel::typetraits::base_type<T>* const);
      /*!
       * \brief Default Constructor
       * \warning enabled only if storage is static
       */
      TFEL_MATH_INLINE explicit constexpr tensor() = default;
      /*!
       * \brief Default Constructor
       * \param T, value used to initialise the components of the tensor
       * \warning enabled only if storage is static
       */
      template <
          typename T2,
          typename std::enable_if<tfel::typetraits::IsAssignableTo<T2, T>::cond,
                                  bool>::type = true>
      TFEL_MATH_INLINE explicit constexpr tensor(const T2&);
      /*!
       * \brief Default Constructor.
       * \param init, pointer to an array used to initialise the
       * components of the tensor. This array is left unchanged.
       */
      template <typename InputIterator,
                typename std::enable_if<
                    std::is_same<typename std::iterator_traits<
                                     InputIterator>::value_type,
                                 tfel::typetraits::base_type<T>>::value,
                    bool>::type = true>
      TFEL_MATH_INLINE explicit tensor(const InputIterator p) {
        using base = tfel::typetraits::base_type<T>;
        tfel::fsalgo::copy<TensorDimeToSize<N>::value>::exe(
            p, reinterpret_cast<base*>(this->v));
      }
      /*!
       * \brief Default Constructor
       * \param[in] init: values used to initialise the components of the vector
       */
      template <
          typename T2,
          typename std::enable_if<tfel::typetraits::IsAssignableTo<T2, T>::cond,
                                  bool>::type = true>
      TFEL_MATH_INLINE constexpr tensor(const std::initializer_list<T2>&);
      //! \brief copy constructor
      TFEL_MATH_INLINE constexpr tensor(const tensor<N, T>&) = default;
      // Copy Constructor
      template <typename T2, typename Op>
      TFEL_MATH_INLINE tensor(const Expr<tensor<N, T2>, Op>& src) {
        TFEL_STATIC_ASSERT(
            (tfel::typetraits::IsSafelyReinterpretCastableTo<T2, T>::cond));
        vectorToTab<TensorDimeToSize<N>::value>::exe(src, this->v);
      }
      //! assignement operator
      TFEL_MATH_INLINE tensor& operator=(const tensor<N, T>&) = default;
      /*!
       * Write to Tab
       */
      TFEL_MATH_INLINE2 void write(tfel::typetraits::base_type<T>* const) const;
      /*!
       * Import values
       */
      template <typename T2>
      TFEL_MATH_INLINE2 typename std::enable_if<
          tfel::typetraits::IsSafelyReinterpretCastableTo<
              T2,
              tfel::typetraits::base_type<T>>::cond,
          void>::type
      import(const T2* const);

      //! assignement operator
      using tensor_base<tensor<N, T>>::operator=;

      //! change basis
      TFEL_MATH_INLINE2 void changeBasis(const rotation_matrix<T>&);

      /*!
       * \return the identity tensor
       */
      TFEL_MATH_INLINE static const tensor<N, T>& Id();

      using fsarray<TensorDimeToSize<N>::value, T>::operator[];

      TFEL_MATH_INLINE constexpr const T& operator()(
          const unsigned short) const;
      TFEL_MATH_INLINE T& operator()(const unsigned short);
      TFEL_MATH_INLINE T operator()(const unsigned short,
                                    const unsigned short) const;
      /*!
       * Return the RunTimeProperties of the tvector
       * \return tvector::RunTimeProperties
       */
      constexpr TFEL_MATH_INLINE RunTimeProperties getRunTimeProperties() const;

      template <typename InputIterator>
      TFEL_MATH_INLINE2 void copy(const InputIterator src);

     private:
      //! a simple check
      TFEL_STATIC_ASSERT((N == 1u) || (N == 2u) || (N == 3u));
    };  // end of class tensor

    template <unsigned short N, typename T, typename OutputIterator>
    TFEL_MATH_INLINE2
        typename std::enable_if<tfel::typetraits::IsScalar<T>::cond, void>::type
        exportToBaseTypeArray(const tensor<N, T>&, OutputIterator);
    /*!
     * \return the invert of a 1D tensor
     * \param[in] t : tensor to be inverted
     */
    template <typename TensorType>
    TFEL_MATH_INLINE2 typename std::enable_if<
        tfel::meta::Implements<TensorType, TensorConcept>::cond &&
            TensorTraits<TensorType>::dime == 1u,
        tensor<1u,
               typename ComputeBinaryResult<
                   tfel::typetraits::base_type<TensorNumType<TensorType>>,
                   TensorNumType<TensorType>,
                   OpDiv>::Result>>::type
    invert(const TensorType&);
    /*!
     * \return the invert of a 2D tensor
     * \param[in] t : tensor to be inverted
     */
    template <typename TensorType>
    TFEL_MATH_INLINE2 typename std::enable_if<
        tfel::meta::Implements<TensorType, TensorConcept>::cond &&
            TensorTraits<TensorType>::dime == 2u,
        tensor<2u,
               typename ComputeBinaryResult<
                   tfel::typetraits::base_type<TensorNumType<TensorType>>,
                   TensorNumType<TensorType>,
                   OpDiv>::Result>>::type
    invert(const TensorType&);
    /*!
     * \return the invert of a 3D tensor
     * \param[in] t : tensor to be inverted
     */
    template <typename TensorType>
    TFEL_MATH_INLINE2 typename std::enable_if<
        tfel::meta::Implements<TensorType, TensorConcept>::cond &&
            TensorTraits<TensorType>::dime == 3u,
        tensor<3u,
               typename ComputeBinaryResult<
                   tfel::typetraits::base_type<TensorNumType<TensorType>>,
                   TensorNumType<TensorType>,
                   OpDiv>::Result>>::type
    invert(const TensorType&);
    /*!
     * \return the derivative of the determinant
     * \param[in] F: tensor where the the determinant is evaluated
     */
    template <typename TensorType>
    typename std::enable_if<
        tfel::meta::Implements<TensorType, TensorConcept>::cond,
        tensor<TensorTraits<TensorType>::dime,
               typename ComputeUnaryResult<TensorNumType<TensorType>,
                                           Power<2>>::Result>>::type
    computeDeterminantDerivative(const TensorType&);
    /*!
     * \brief rotate a tensor using a rotation matrix
     * \param[in] s: tensor to be rotated
     * \param[in] r: rotation matrix
     * \return the rotated tensor
     */
    template <typename TensorType>
    TFEL_MATH_INLINE2 typename std::enable_if<
        tfel::meta::Implements<TensorType, TensorConcept>::cond,
        tensor<TensorTraits<TensorType>::dime, TensorNumType<TensorType>>>::type
    change_basis(const TensorType&,
                 const rotation_matrix<TensorNumType<TensorType>>&);

    template <class T>
    TFEL_MATH_INLINE typename std::enable_if<
        ((tfel::meta::Implements<T, StensorConcept>::cond) &&
         (StensorTraits<T>::dime == 1u)),
        tensor<1u, StensorNumType<T>>>::type
    unsyme(const T&);

    template <class T>
    TFEL_MATH_INLINE typename std::enable_if<
        ((tfel::meta::Implements<T, StensorConcept>::cond) &&
         (StensorTraits<T>::dime == 2u)),
        tensor<2u, StensorNumType<T>>>::type
    unsyme(const T&);

    template <class T>
    TFEL_MATH_INLINE typename std::enable_if<
        ((tfel::meta::Implements<T, StensorConcept>::cond) &&
         (StensorTraits<T>::dime == 3u)),
        tensor<3u, StensorNumType<T>>>::type
    unsyme(const T&);

  }  // end of namespace math

  namespace typetraits {

    template <unsigned short N, typename T2, typename T>
    struct IsAssignableTo<tfel::math::tensor<N, T2>, tfel::math::tensor<N, T>> {
      //! result
      static constexpr bool cond = IsAssignableTo<T2, T>::cond;
    };

  }  // end of namespace typetraits

}  // end of namespace tfel

#include "TFEL/Math/Tensor/TensorSizeToDime.hxx"
#include "TFEL/Math/Tensor/tensor.ixx"
#include "TFEL/Math/Tensor/tensorResultType.hxx"

#endif /* LIB_TFEL_MATH_TENSOR_HXX */
