/*!
 * \file   include/TFEL/Math/Tensor/tensor.ixx
 * \brief  This file implements the methods of the class tensor.
 * \author Thomas Helfer
 * \date   02 jun 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_TENSORIXX
#define LIB_TFEL_TENSORIXX

#include <cmath>
#include <iterator>

#include "TFEL/FSAlgorithm/FSAlgorithm.hxx"

#include "TFEL/TypeTraits/IsSafelyReinterpretCastableTo.hxx"
#include "TFEL/Math/General/Abs.hxx"
#include "TFEL/Math/General/MathConstants.hxx"
#include "TFEL/Math/Vector/VectorUtilities.hxx"
#include "TFEL/Math/Tensor/TensorChangeBasis.hxx"

namespace tfel {

  namespace math {

    namespace internals {

      /*!
       * An helper struct used to defined a tensor from the fortran
       * matrix
       */
      template <unsigned short N>
      struct BuildTensorFromFortranMatrix;

      /*!
       * Partial specialisation in 1D
       */
      template <>
      struct BuildTensorFromFortranMatrix<1u> {
        /*!
         * \param[in] t : tensor
         * \param[in] v : fortran values
         */
        template <typename T>
        TFEL_MATH_INLINE2 static void exe(
            T* const t, const tfel::typetraits::base_type<T>* const v) {
          t[0] = T(v[0]);
          t[1] = T(v[4]);
          t[2] = T(v[8]);
        }  // end of exe
      };   // end of struct BuildTensorFromFortranMatrix<1u>

      /*!
       * Partial specialisation in 2D
       */
      template <>
      struct BuildTensorFromFortranMatrix<2u> {
        /*!
         * \param[in] t : tensor
         * \param[in] v : fortran values
         */
        template <typename T>
        TFEL_MATH_INLINE2 static void exe(
            T* const t, const tfel::typetraits::base_type<T>* const v) {
          BuildTensorFromFortranMatrix<1u>::exe(t, v);
          t[3] = T(v[3]);
          t[4] = T(v[1]);
        }  // end of exe
      };   // end of struct BuildTensorFromFortranMatrix<1u>

      /*!
       * Partial specialisation in 3D
       */
      template <>
      struct BuildTensorFromFortranMatrix<3u> {
        /*!
         * \param[in] t : tensor
         * \param[in] v : fortran values
         */
        template <typename T>
        TFEL_MATH_INLINE2 static void exe(
            T* const t, const tfel::typetraits::base_type<T>* const v) {
          BuildTensorFromFortranMatrix<2u>::exe(t, v);
          t[5] = T(v[6]);
          t[6] = T(v[2]);
          t[7] = T(v[7]);
          t[8] = T(v[5]);
        }  // end of exe
      };   // end of struct BuildTensorFromFortranMatrix<1u>

      template <unsigned short N>
      struct TensorMatrixAccessOperator;

      template <>
      struct TensorMatrixAccessOperator<1u> {
        template <typename T>
        TFEL_MATH_INLINE static T exe(const tfel::math::tensor<1u, T>& t,
                                      const unsigned short i,
                                      const unsigned short j) {
          if ((i > 2) || (j > 2)) {
            throw(TensorInvalidIndexesException());
          }
          return (i == j) ? t(i) : T(0);
        }
      };

      template <>
      struct TensorMatrixAccessOperator<2u> {
        template <typename T>
        TFEL_MATH_INLINE static T exe(const tfel::math::tensor<2u, T>& t,
                                      const unsigned short i,
                                      const unsigned short j) {
          if ((i > 2) || (j > 2)) {
            throw(TensorInvalidIndexesException());
          }
          if (i == j) {
            return t(i);
          } else if ((i == 0) && (j == 1)) {
            return t(3);
          } else if ((i == 1) && (j == 0)) {
            return t(4);
          }
          return T(0);
        }
      };

      template <>
      struct TensorMatrixAccessOperator<3u> {
        template <typename T>
        TFEL_MATH_INLINE static T exe(const tfel::math::tensor<3u, T>& t,
                                      const unsigned short i,
                                      const unsigned short j) {
          if ((i == j) && (i < 3)) {
            return t(i);
          } else if ((i == 0) && (j == 1)) {
            return t(3);
          } else if ((i == 1) && (j == 0)) {
            return t(4);
          } else if ((i == 0) && (j == 2)) {
            return t(5);
          } else if ((i == 2) && (j == 0)) {
            return t(6);
          } else if ((i == 1) && (j == 2)) {
            return t(7);
          } else if ((i == 2) && (j == 1)) {
            return t(8);
          }
          throw(TensorInvalidIndexesException());
        }
      };

    }  // end of namespace internals

    template <typename Child>
    template <typename TensorType>
    typename std::enable_if<
        tfel::meta::Implements<TensorType, TensorConcept>::cond &&
            TensorTraits<Child>::dime == TensorTraits<TensorType>::dime &&
            tfel::typetraits::IsAssignableTo<TensorNumType<TensorType>,
                                             TensorNumType<Child>>::cond,
        Child&>::type
    tensor_base<Child>::operator=(const TensorType& src) {
      auto& child = static_cast<Child&>(*this);
      vectorToTab<TensorDimeToSize<TensorTraits<Child>::dime>::value>::exe(
          src, child);
      return child;
    }

    template <typename Child>
    template <typename T>
    typename std::enable_if<
        tfel::typetraits::IsAssignableTo<T, TensorNumType<Child>>::cond,
        Child&>::type
    tensor_base<Child>::operator=(const std::initializer_list<T>& src) {
      using Copy = tfel::fsalgo::copy<
          TensorDimeToSize<TensorTraits<Child>::dime>::value>;
      if (src.size() != TensorDimeToSize<TensorTraits<Child>::dime>::value) {
        throw(TensorInvalidInitializerListSizeException());
      }
      auto& child = static_cast<Child&>(*this);
      Copy::exe(src.begin(), child.begin());
      return child;
    }

    template <typename Child>
    template <typename TensorType>
    typename std::enable_if<
        tfel::meta::Implements<TensorType, TensorConcept>::cond &&
            TensorTraits<Child>::dime == TensorTraits<TensorType>::dime &&
            tfel::typetraits::IsAssignableTo<TensorNumType<TensorType>,
                                             TensorNumType<Child>>::cond,
        Child&>::type
    tensor_base<Child>::operator+=(const TensorType& src) {
      auto& child = static_cast<Child&>(*this);
      VectorUtilities<
          TensorDimeToSize<TensorTraits<Child>::dime>::value>::PlusEqual(child,
                                                                         src);
      return child;
    }

    template <typename Child>
    template <typename TensorType>
    typename std::enable_if<
        tfel::meta::Implements<TensorType, TensorConcept>::cond &&
            TensorTraits<Child>::dime == TensorTraits<TensorType>::dime &&
            tfel::typetraits::IsAssignableTo<TensorNumType<TensorType>,
                                             TensorNumType<Child>>::cond,
        Child&>::type
    tensor_base<Child>::operator-=(const TensorType& src) {
      auto& child = static_cast<Child&>(*this);
      VectorUtilities<
          TensorDimeToSize<TensorTraits<Child>::dime>::value>::MinusEqual(child,
                                                                          src);
      return child;
    }

    // *= operator
    template <typename Child>
    template <typename T2>
    typename std::enable_if<
        tfel::typetraits::IsScalar<T2>::cond &&
            std::is_same<
                typename ResultType<TensorNumType<Child>, T2, OpMult>::type,
                TensorNumType<Child>>::value,
        Child&>::type
    tensor_base<Child>::operator*=(const T2 s) {
      auto& child = static_cast<Child&>(*this);
      VectorUtilities<
          TensorDimeToSize<TensorTraits<Child>::dime>::value>::scale(child, s);
      return child;
    }

    // /= operator
    template <typename Child>
    template <typename T2>
    typename std::enable_if<
        tfel::typetraits::IsScalar<T2>::cond &&
            std::is_same<
                typename ResultType<TensorNumType<Child>, T2, OpDiv>::type,
                TensorNumType<Child>>::value,
        Child&>::type
    tensor_base<Child>::operator/=(const T2 s) {
      auto& child = static_cast<Child&>(*this);
      VectorUtilities<TensorDimeToSize<TensorTraits<Child>::dime>::value>::
          scale(child, (static_cast<tfel::typetraits::base_type<T2>>(1u)) / s);
      return child;
    }

    template <unsigned short N, typename T>
    void tensor<N, T>::buildFromFortranMatrix(
        tensor<N, T>& t, const tfel::typetraits::base_type<T>* const v) {
      using tfel::math::internals::BuildTensorFromFortranMatrix;
      BuildTensorFromFortranMatrix<N>::template exe<T>(t.begin(), v);
    }  // end of void tensor<N,T>::buildFromFortranMatrix

    template <unsigned short N, typename T>
    tensor<N, T> tensor<N, T>::buildFromFortranMatrix(
        const tfel::typetraits::base_type<T>* const v) {
      using tfel::math::internals::BuildTensorFromFortranMatrix;
      tensor<N, T> t;
      BuildTensorFromFortranMatrix<N>::template exe<T>(t.begin(), v);
      return t;
    }  // end of void tensor<N,T>::buildFromFortranMatrix

    template <unsigned short N, typename T>
    template <
        typename T2,
        typename std::enable_if<tfel::typetraits::IsAssignableTo<T2, T>::cond,
                                bool>::type>
    constexpr tensor<N, T>::tensor(const T2& init)
        : fsarray<TensorDimeToSize<N>::value, T>(init) {}

    template <unsigned short N, typename T>
    template <
        typename T2,
        typename std::enable_if<tfel::typetraits::IsAssignableTo<T2, T>::cond,
                                bool>::type>
    constexpr tensor<N, T>::tensor(const std::initializer_list<T2>& init)
        : fsarray<TensorDimeToSize<N>::value, T>(init) {}

    template <unsigned short N, typename T>
    T& tensor<N, T>::operator()(const unsigned short i) {
      return this->v[i];
    }

    template <unsigned short N, typename T>
    constexpr const T& tensor<N, T>::operator()(const unsigned short i) const {
      return this->v[i];
    }

    template <unsigned short N, typename T>
    T tensor<N, T>::operator()(const unsigned short i,
                               const unsigned short j) const {
      using tfel::math::internals::TensorMatrixAccessOperator;
      return TensorMatrixAccessOperator<N>::exe(*this, i, j);
    }

    template <unsigned short N, typename T>
    constexpr typename tensor<N, T>::RunTimeProperties
    tensor<N, T>::getRunTimeProperties() const {
      return RunTimeProperties();
    }

    // Import from values
    template <unsigned short N, typename T>
    template <typename T2>
    typename std::enable_if<tfel::typetraits::IsSafelyReinterpretCastableTo<
                                T2,
                                tfel::typetraits::base_type<T>>::cond,
                            void>::type
    tensor<N, T>::import(const T2* const src) {
      typedef tfel::typetraits::base_type<T> base;
      typedef tfel::fsalgo::copy<TensorDimeToSize<N>::value> Copy;
      TFEL_STATIC_ASSERT(
          (tfel::typetraits::IsSafelyReinterpretCastableTo<T, base>::cond));
      Copy::exe(src, reinterpret_cast<base*>(this->v));
    }

    // Write to Tab
    template <unsigned short N, typename T>
    void tensor<N, T>::write(tfel::typetraits::base_type<T>* const t) const {
      typedef tfel::typetraits::base_type<T> base;
      typedef tfel::fsalgo::copy<TensorDimeToSize<N>::value> Copy;
      TFEL_STATIC_ASSERT(
          (tfel::typetraits::IsSafelyReinterpretCastableTo<T, base>::cond));
      Copy::exe(reinterpret_cast<const base*>(this->v), t);
    }

    // ChangeBasis
    template <unsigned short N, typename T>
    void tensor<N, T>::changeBasis(const rotation_matrix<T>& m) {
      TensorChangeBasis<N, T>::exe(this->v, m);
    }

    // Return Id
    template <unsigned short N, typename T>
    const tensor<N, T>& tensor<N, T>::Id() {
      typedef tfel::typetraits::base_type<T> base;
      constexpr base zero{0};
      constexpr base one{1};
      constexpr base IdCoef[] = {one,  one,  one,  zero, zero,
                                 zero, zero, zero, zero};
      static const tensor<N, T> id(IdCoef);
      return id;
    }  // end of tensor<N,T>::Id

    template <unsigned short N, typename T>
    template <typename InputIterator>
    void tensor<N, T>::copy(const InputIterator src) {
      tfel::fsalgo::copy<TensorDimeToSize<N>::value>::exe(src, *this);
    }

    template <unsigned short N, typename T, typename OutputIterator>
    TFEL_MATH_INLINE2
        typename std::enable_if<tfel::typetraits::IsScalar<T>::cond, void>::type
        exportToBaseTypeArray(const tensor<N, T>& t, OutputIterator p) {
      typedef tfel::typetraits::base_type<T> base;
      typedef tfel::fsalgo::copy<TensorDimeToSize<N>::value> Copy;
      TFEL_STATIC_ASSERT(
          (tfel::typetraits::IsSafelyReinterpretCastableTo<T, base>::cond));
      Copy::exe(reinterpret_cast<const base*>(&t[0]), p);
    }

    template <typename TensorType>
    typename std::enable_if<
        tfel::meta::Implements<TensorType, TensorConcept>::cond &&
            TensorTraits<TensorType>::dime == 1u,
        tensor<1u,
               typename ComputeBinaryResult<
                   tfel::typetraits::base_type<TensorNumType<TensorType>>,
                   TensorNumType<TensorType>,
                   OpDiv>::Result>>::type
    invert(const TensorType& t) {
      typedef TensorNumType<TensorType> T;
      typedef tfel::typetraits::base_type<T> base;
      typedef typename ComputeBinaryResult<base, T, OpDiv>::Result T2;
      tensor<1u, T2> t2;
      t2(0) = base(1) / t(0);
      t2(1) = base(1) / t(1);
      t2(2) = base(1) / t(2);
      return t2;
    }

    template <typename TensorType>
    typename std::enable_if<
        tfel::meta::Implements<TensorType, TensorConcept>::cond &&
            TensorTraits<TensorType>::dime == 2u,
        tensor<2u,
               typename ComputeBinaryResult<
                   tfel::typetraits::base_type<TensorNumType<TensorType>>,
                   TensorNumType<TensorType>,
                   OpDiv>::Result>>::type
    invert(const TensorType& t) {
      typedef TensorNumType<TensorType> T;
      typedef tfel::typetraits::base_type<T> base;
      typedef typename ComputeBinaryResult<base, T, OpDiv>::Result T3;
      tensor<2u, T3> t2;
      const auto id = base(1) / (t(0) * t(1) - t(3) * t(4));
      t2(0) = t(1) * id;
      t2(1) = t(0) * id;
      t2(3) = -t(3) * id;
      t2(4) = -t(4) * id;
      t2(2) = base(1) / t(2);
      return t2;
    }

    template <typename TensorType>
    typename std::enable_if<
        tfel::meta::Implements<TensorType, TensorConcept>::cond &&
            TensorTraits<TensorType>::dime == 3u,
        tensor<3u,
               typename ComputeBinaryResult<
                   tfel::typetraits::base_type<TensorNumType<TensorType>>,
                   TensorNumType<TensorType>,
                   OpDiv>::Result>>::type
    invert(const TensorType& t) {
      typedef TensorNumType<TensorType> T;
      typedef tfel::typetraits::base_type<T> base;
      typedef typename ComputeBinaryResult<base, T, OpDiv>::Result T4;
      tensor<3u, T4> t2;
      const auto id = base(1) / det(t);
      t2(0) = (t(1) * t(2) - t(7) * t(8)) * id;
      t2(1) = (t(0) * t(2) - t(5) * t(6)) * id;
      t2(2) = (t(0) * t(1) - t(3) * t(4)) * id;
      t2(3) = (t(5) * t(8) - t(2) * t(3)) * id;
      t2(4) = (t(6) * t(7) - t(2) * t(4)) * id;
      t2(5) = (t(3) * t(7) - t(1) * t(5)) * id;
      t2(6) = (t(4) * t(8) - t(1) * t(6)) * id;
      t2(7) = (t(4) * t(5) - t(0) * t(7)) * id;
      t2(8) = (t(3) * t(6) - t(0) * t(8)) * id;
      return t2;
    }

    template <typename TensorType>
    typename std::enable_if<
        tfel::meta::Implements<TensorType, TensorConcept>::cond,
        tensor<TensorTraits<TensorType>::dime,
               typename ComputeUnaryResult<TensorNumType<TensorType>,
                                           Power<2>>::Result>>::type
    computeDeterminantDerivative(const TensorType& F) {
      tensor<TensorTraits<TensorType>::dime,
             typename ComputeUnaryResult<TensorNumType<TensorType>,
                                         Power<2>>::Result>
          dJ;
      computeDeterminantDerivative(dJ, F);
      return dJ;
    }

    template <typename TensorType>
    typename std::enable_if<
        tfel::meta::Implements<TensorType, TensorConcept>::cond,
        tensor<TensorTraits<TensorType>::dime, TensorNumType<TensorType>>>::type
    change_basis(
        const TensorType& t,
        const tmatrix<3u,
                      3u,
                      tfel::typetraits::base_type<TensorNumType<TensorType>>>&
            r) {
      tensor<TensorTraits<TensorType>::dime, TensorNumType<TensorType>> nt{t};
      nt.changeBasis(r);
      return nt;
    }  // end of change_basis

    template <class T>
    typename std::enable_if<
        ((tfel::meta::Implements<T, StensorConcept>::cond) &&
         (StensorTraits<T>::dime == 1u)),
        tensor<1u, StensorNumType<T>>>::type
    unsyme(const T& s) {
      return {s[0], s[1], s[2]};
    }

    template <class T>
    typename std::enable_if<
        ((tfel::meta::Implements<T, StensorConcept>::cond) &&
         (StensorTraits<T>::dime == 2u)),
        tensor<2u, StensorNumType<T>>>::type
    unsyme(const T& s) {
      using real = tfel::typetraits::base_type<StensorNumType<T>>;
      constexpr const auto cste = Cste<real>::isqrt2;
      const auto s01 = s[3] * cste;
      return {s[0], s[1], s[2], s01, s01};
    }

    template <class T>
    typename std::enable_if<
        ((tfel::meta::Implements<T, StensorConcept>::cond) &&
         (StensorTraits<T>::dime == 3u)),
        tensor<3u, StensorNumType<T>>>::type
    unsyme(const T& s) {
      using real = tfel::typetraits::base_type<StensorNumType<T>>;
      constexpr const auto cste = Cste<real>::isqrt2;
      const auto s01 = s[3] * cste;
      const auto s02 = s[4] * cste;
      const auto s12 = s[5] * cste;
      return {s[0], s[1], s[2], s01, s01, s02, s02, s12, s12};
    }

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_TENSORIXX */
