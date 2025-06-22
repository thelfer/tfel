/*!
 * \file   include/TFEL/Math/Vector/tvector.ixx
 * \brief
 *
 * \author Thomas Helfer
 * \date   28 Jun 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_TINY_VECTOR_IXX
#define LIB_TFEL_MATH_TINY_VECTOR_IXX

#include <iterator>
#include "TFEL/FSAlgorithm/FSAlgorithm.hxx"
#include "TFEL/Math/General/Abs.hxx"
#include "TFEL/Math/General/BasicOperations.hxx"
#include "TFEL/Math/General/DotProduct.hxx"

namespace tfel::math {

  template <unsigned short N, typename T>
  TFEL_HOST_DEVICE tvector<N, T>::tvector(const fsarray<N, T>& src) {
    tfel::fsalgo::copy<N>::exe(src.begin(), this->v);
  }  // end of tvector

  template <unsigned short N, typename T>
  template <typename InputIterator>
  TFEL_HOST_DEVICE constexpr void tvector<N, T>::copy(const InputIterator src) {
    tfel::fsalgo::copy<N>::exe(src, this->v);
  }

  template <unsigned short N, typename T>
  template <unsigned short I>
  TFEL_HOST_DEVICE constexpr auto tvector<N, T>::slice() {
    return tfel::math::slice<I>(*this);
  }  // end of slice

  template <unsigned short N, typename T>
  template <unsigned short I, unsigned short J>
  TFEL_HOST_DEVICE constexpr auto tvector<N, T>::slice() {
    return tfel::math::slice<I, J>(*this);
  }  // end of slice

  template <unsigned short N, typename T>
  template <unsigned short I>
  TFEL_HOST_DEVICE constexpr auto tvector<N, T>::slice() const {
    return tfel::math::slice<I>(*this);
  }  // end of slice

  template <unsigned short N, typename T>
  template <unsigned short I, unsigned short J>
  TFEL_HOST_DEVICE constexpr auto tvector<N, T>::slice() const {
    return tfel::math::slice<I, J>(*this);
  }  // end of slice

  template <unsigned short N, typename T, typename OutputIterator>
  TFEL_HOST_DEVICE constexpr std::enable_if_t<isScalar<T>(), void>
  exportToBaseTypeArray(const tvector<N, T>& v, OutputIterator p) {
    tfel::fsalgo::transform<N>::exe(
        v.begin(), p, [](const auto& value) { return base_type_cast(value); });
  }  // end of exportToBaseTypePointer

  // Norm2
  template <unsigned short N, typename T>
  TFEL_HOST_DEVICE constexpr std::enable_if_t<
      isScalar<T>(),
      typename tfel::typetraits::RealPartType<T>::type>
  norm(const tvector<N, T>& vec) noexcept {
    typedef result_type<T, T, OpMult> squareT;
    return std::sqrt(
        real(dotProduct<N>::exe(vec.begin(), vec.begin(), squareT(0u))));
  }

  template <unsigned short N, typename T>
  TFEL_HOST_DEVICE constexpr auto abs(const tvector<N, T>& v) {
    AbsSum<T> a;
    tfel::fsalgo::for_each<N>::exe(v.begin(), a);
    return a.result;
  }

  template <typename T>
  TFEL_HOST_DEVICE constexpr tvector<1u, T> makeTVector1D(const T v) {
    return {v};
  }  // end of makeTVector1D

  template <typename T>
  TFEL_HOST_DEVICE constexpr tvector<2u, T> makeTVector2D(const T v1,
                                                          const T v2) {
    return {v1, v2};
  }  // end of makeTVector2D

  template <typename T>
  TFEL_HOST_DEVICE constexpr tvector<3u, T> makeTVector3D(const T v1,
                                                          const T v2,
                                                          const T v3) {
    return {v1, v2, v3};
  }  // end of makeTVector3D

  /*!
   * \brief create a new tvector by applying a functor
   * \param[in] f: functor
   * \param[in] x: inital value
   */
  template <typename F, typename T, unsigned short N>
  TFEL_HOST_DEVICE constexpr tvector<N, std::invoke_result_t<F, T>> map(
      F f, const tvector<N, T>& x) {
    tvector<N, std::invoke_result_t<F, T>> r;
    tfel::fsalgo::transform<N>::exe(x.begin(), r.begin(), f);
    return r;
  }  // end of map

  template <typename T>
  TFEL_HOST_DEVICE constexpr tvector<3u, T> cross_product(
      const tvector<2u, T>& v1, const tvector<2u, T>& v2) {
    constexpr auto zero = T(0);
    return {zero, zero, v1[0] * v2[1] - v1[1] * v2[0]};
  }  // end of cross_product

  template <typename T>
  TFEL_HOST_DEVICE constexpr tvector<3u, T> cross_product(
      const tvector<3u, T>& v1, const tvector<3u, T>& v2) {
    return {v1[1] * v2[2] - v1[2] * v2[1], v1[2] * v2[0] - v1[0] * v2[2],
            v1[0] * v2[1] - v1[1] * v2[0]};
  }  // end of cross_product

  template <typename T>
  TFEL_HOST_DEVICE constexpr void find_perpendicular_vector(
      tvector<3u, T>& y, const tvector<3u, T>& x) {
    using real = base_type<T>;
    constexpr auto zero = T(0);
    constexpr auto one = T(1);
    const auto nx = (x | x);
    if (nx < 100 * std::numeric_limits<decltype(nx)>::min()) {
      // x is null
      y = {one, zero, zero};
      return;
    }
    const auto inx = real(1) / nx;
    if (tfel::math::abs(x(0)) < tfel::math::abs(x(1))) {
      if (tfel::math::abs(x(0)) < tfel::math::abs(x(2))) {
        //|x0| is min, (1 0 0) is a good choice
        y(0) = one - x(0) * x(0) * inx;
        y(1) = -x(0) * x(1) * inx;
        y(2) = -x(0) * x(2) * inx;
      } else {
        //|x2| is min, (0 0 1) is a good choice
        y(0) = -x(2) * x(0) * inx;
        y(1) = -x(2) * x(1) * inx;
        y(2) = one - x(2) * x(2) * inx;
      }
    } else if (tfel::math::abs(x(1)) < tfel::math::abs(x(2))) {
      // |x1| is min, (0 0 1) is a good choice
      y(0) = -x(1) * x(0) * inx;
      y(1) = one - x(1) * x(1) * inx;
      y(2) = -x(1) * x(2) * inx;
    } else {
      // |x2| is min, (0 0 1) is a good choice
      y(0) = -x(2) * x(0) * inx;
      y(1) = -x(2) * x(1) * inx;
      y(2) = one - x(2) * x(2) * inx;
    }
    y /= std::sqrt(y | y);
  }

  template <unsigned short I, unsigned short N, typename T>
  TFEL_HOST_DEVICE constexpr auto slice(tvector<N, T>& v) {
    static_assert(N > I, "invalid index");
    return map<tvector<N - I, T>, I>(v);
  }  // end of slice

  template <unsigned short I, unsigned short J, unsigned short N, typename T>
  TFEL_HOST_DEVICE constexpr auto slice(tvector<N, T>& v) {
    static_assert(N > I, "invalid index");
    static_assert(N >= J, "invalid index");
    static_assert(J > I, "invalid index");
    return map<tvector<J - I, T>, I>(v);
  }  // end of slice

  template <unsigned short I, unsigned short N, typename T>
  TFEL_HOST_DEVICE constexpr auto slice(const tvector<N, T>& v) {
    static_assert(N > I, "invalid index");
    return map<tvector<N - I, T>, I>(v);
  }  // end of slice

  template <unsigned short I, unsigned short J, unsigned short N, typename T>
  TFEL_HOST_DEVICE constexpr auto slice(const tvector<N, T>& v) {
    return map<const tvector<J - I, T>, I>(v);
  }  // end of slice

  template <typename MappedType, typename IndexingPolicyType, unsigned short N>
  TFEL_HOST_DEVICE constexpr std::enable_if_t<
      ((!isScalar<MappedType>()) && (IndexingPolicyType::hasFixedSizes) &&
       (checkIndexingPoliciesCompatiblity<
           IndexingPolicyType,
           typename std::remove_cv_t<MappedType>::indexing_policy>())),
      View<MappedType, IndexingPolicyType>>
  map(tvector<N, base_type<numeric_type<MappedType>>>& v) {
    static_assert(N >= getUnderlyingArrayMinimalSize<IndexingPolicyType>(),
                  "invalid vector size");
    return map<MappedType, IndexingPolicyType>(v.data());
  }  // end of map

  template <typename MappedType, typename IndexingPolicyType, unsigned short N>
  TFEL_HOST_DEVICE constexpr std::enable_if_t<
      ((!isScalar<MappedType>()) && (IndexingPolicyType::hasFixedSizes) &&
       (checkIndexingPoliciesCompatiblity<
           IndexingPolicyType,
           typename std::remove_cv_t<MappedType>::indexing_policy>())),
      View<const MappedType, IndexingPolicyType>>
  map(const tvector<N, base_type<numeric_type<MappedType>>>& v) {
    static_assert(N >= getUnderlyingArrayMinimalSize<IndexingPolicyType>(),
                  "invalid vector size");
    return map<const MappedType, IndexingPolicyType>(v.data());
  }  // end of map

  template <typename MappedType,
            unsigned short offset,
            typename IndexingPolicyType,
            unsigned short N,
            typename real>
  TFEL_HOST_DEVICE constexpr std::enable_if_t<
      ((!std::is_const_v<MappedType>)&&(IndexingPolicyType::hasFixedSizes) &&
       (checkIndexingPoliciesCompatiblity<
           IndexingPolicyType,
           typename MappedType::indexing_policy>())),
      View<MappedType, IndexingPolicyType>>
  map(tvector<N, real>& v) {
    static_assert(
        N >= offset + getUnderlyingArrayMinimalSize<IndexingPolicyType>(),
        "invalid vector size");
    return map<MappedType, IndexingPolicyType>(v.data() + offset);
  }  // end of map

  template <typename MappedType,
            unsigned short offset,
            typename IndexingPolicyType,
            unsigned short N,
            typename real>
  TFEL_HOST_DEVICE constexpr std::enable_if_t<
      ((IndexingPolicyType::hasFixedSizes) &&
       (checkIndexingPoliciesCompatiblity<
           IndexingPolicyType,
           typename std::remove_cv_t<MappedType>::indexing_policy>())),
      View<const MappedType, IndexingPolicyType>>
  map(const tvector<N, real>& v) {
    static_assert(
        N >= offset + getUnderlyingArrayMinimalSize<IndexingPolicyType>(),
        "invalid vector size");
    return map<const MappedType, IndexingPolicyType>(v.data() + offset);
  }  // end of map

  template <unsigned short M,
            typename MappedType,
            unsigned short offset,
            unsigned short stride,
            unsigned short N>
  TFEL_HOST_DEVICE constexpr std::enable_if_t<
      !std::is_const_v<MappedType>,
      ViewsFixedSizeVector<MappedType, unsigned short, M, stride>>
  map(tvector<N, ViewsArrayNumericType<MappedType>>& v) {
    constexpr auto mstride = getViewsArrayMinimalStride<MappedType>();
    static_assert(stride >= mstride, "invalid stride");
    static_assert(N >= offset + M * mstride, "invalid vector size");
    return ViewsFixedSizeVector<MappedType, unsigned short, M, stride>(
        v.data() + offset);
  }  // end of map

  template <unsigned short M,
            typename MappedType,
            unsigned short offset,
            unsigned short stride,
            unsigned short N>
  TFEL_HOST_DEVICE constexpr auto map(
      const tvector<N, ViewsArrayNumericType<MappedType>>& v) {
    constexpr auto mstride = getViewsArrayMinimalStride<MappedType>();
    static_assert(stride >= mstride, "invalid stride");
    static_assert(N >= offset + M * mstride, "invalid vector size");
    return ViewsFixedSizeVector<const MappedType, unsigned short, M, stride>(
        v.data() + offset);
  }  // end of map

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_TINY_VECTOR_IXX */
