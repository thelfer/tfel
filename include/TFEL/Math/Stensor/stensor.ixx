/*!
 * \file   include/TFEL/Math/Stensor/stensor.ixx
 * \brief  This file implements the methods of the class stensor.
 * \author Thomas Helfer
 * \date   02 jun 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_STENSOR_IXX
#define LIB_TFEL_MATH_STENSOR_IXX

#include <cmath>
#include <iterator>
#include "TFEL/FSAlgorithm/FSAlgorithm.hxx"
#include "TFEL/Math/General/Abs.hxx"
#include "TFEL/Math/General/MathConstants.hxx"
#include "TFEL/Math/General/ConstExprMathFunctions.hxx"
#include "TFEL/Math/Stensor/SymmetricStensorProduct.hxx"
#include "TFEL/Math/Stensor/Internals/StensorChangeBasis.hxx"
#include "TFEL/Math/Stensor/Internals/StensorEigenSolver.hxx"
#include "TFEL/Math/Stensor/Internals/SortEigenValues.hxx"
#include "TFEL/Math/Stensor/Internals/SortEigenVectors.hxx"
#include "TFEL/Math/Stensor/Internals/StensorComputeEigenValuesDerivatives.hxx"
#include "TFEL/Math/Stensor/Internals/StensorComputeEigenVectorsDerivatives.hxx"
#include "TFEL/Math/Stensor/Internals/StensorComputeIsotropicFunctionDerivative.hxx"

namespace tfel::math {

  template <unsigned short N, typename T>
  template <typename InputIterator>
  TFEL_HOST_DEVICE constexpr void
  stensor<N, T>::importVoigt(const InputIterator p) noexcept requires(
      std::is_same_v<typename std::iterator_traits<InputIterator>::value_type,
                     base_type<T>>) {
    tfel::fsalgo::transform<3u>::exe(
        p, this->begin(), [](const auto& value) { return T(value); });
    if constexpr (N == 2) {
      constexpr auto icste = Cste<T>::isqrt2;
      this->v[3] = T(p[3]) * icste;
    }
    if constexpr (N == 3) {
      constexpr auto icste = Cste<T>::isqrt2;
      this->v[3] = T(p[3]) * icste;
      this->v[4] = T(p[4]) * icste;
      this->v[5] = T(p[5]) * icste;
    }
  }  // end of importVoigt

  template <unsigned short N, typename T>
  template <typename InputIterator>
  TFEL_HOST_DEVICE constexpr void
  stensor<N, T>::importTab(const InputIterator p) noexcept requires(
      std::is_same_v<typename std::iterator_traits<InputIterator>::value_type,
                     base_type<T>>) {
    tfel::fsalgo::transform<3>::exe(p, this->begin(),
                                    [](const auto& value) { return T(value); });
    if constexpr (N == 2) {
      constexpr auto cste = Cste<T>::sqrt2;
      this->v[3] = T(p[3]) * cste;
    }
    if constexpr (N == 3) {
      constexpr auto cste = Cste<T>::sqrt2;
      this->v[3] = T(p[3]) * cste;
      this->v[4] = T(p[4]) * cste;
      this->v[5] = T(p[5]) * cste;
    }
  }  // end of importTab

  template <unsigned short N, typename T>
  template <typename InputIterator>
  TFEL_HOST_DEVICE constexpr void
  stensor<N, T>::import(const InputIterator p) noexcept requires(
      std::is_same_v<typename std::iterator_traits<InputIterator>::value_type,
                     base_type<T>>) {
    tfel::fsalgo::transform<StensorDimeToSize<N>::value>::exe(
        p, this->begin(), [](const auto& value) { return T(value); });
  }  // end of import

  template <unsigned short N, typename T>
  TFEL_HOST_DEVICE constexpr void stensor<N, T>::exportTab(
      base_type<T>* const p) const noexcept {
    tfel::fsalgo::transform<3u>::exe(this->cbegin(), p, [](const auto& value) {
      return base_type_cast(value);
    });
    if constexpr (N == 2) {
      constexpr auto icste = Cste<T>::isqrt2;
      p[3] = base_type_cast(this->v[3]) * icste;
    }
    if constexpr (N == 3) {
      constexpr auto icste = Cste<T>::isqrt2;
      p[3] = base_type_cast(this->v[3]) * icste;
      p[4] = base_type_cast(this->v[4]) * icste;
      p[5] = base_type_cast(this->v[5]) * icste;
    }
  }  // end of exportTab

  template <unsigned short N, typename T>
  TFEL_HOST_DEVICE constexpr void stensor<N, T>::write(
      base_type<T>* const t) const noexcept {
    tfel::fsalgo::transform<StensorDimeToSize<N>::value>::exe(
        this->cbegin(), t,
        [](const auto& value) { return base_type_cast(value); });
  }  // end of write

  template <unsigned short N, typename T>
  template <typename stensor_common::EigenSolver es>
  TFEL_HOST_DEVICE void stensor<N, T>::computeEigenValues(T& vp0,
                                                          T& vp1,
                                                          T& vp2,
                                                          const bool b) const {
    using ESolver = tfel::math::internals::StensorEigenSolver<es, N, T>;
    ESolver::computeEigenValues(vp0, vp1, vp2, this->v, b);
  }  // end of stensor<N,T>::computeEigenValues

  template <unsigned short N, typename T>
  template <typename stensor_common::EigenSolver es>
  TFEL_HOST_DEVICE void stensor<N, T>::computeEigenValues(tvector<3u, T>& vp,
                                                          const bool b) const {
    this->template computeEigenValues<es>(vp(0), vp(1), vp(2), b);
  }

  template <unsigned short N, typename T>
  template <typename stensor_common::EigenSolver es>
  TFEL_HOST_DEVICE tvector<3u, T> stensor<N, T>::computeEigenValues(
      const bool b) const {
    tvector<3u, T> vp;
    this->template computeEigenValues<es>(vp, b);
    return vp;
  }  // end of stensor<N,T>::computeEigenValues

  template <unsigned short N, typename T>
  template <typename stensor_common::EigenSolver es>
  TFEL_HOST_DEVICE void stensor<N, T>::computeEigenValues(
      T& vp0, T& vp1, T& vp2, const EigenValuesOrdering o, const bool b) const {
    this->template computeEigenValues<es>(vp0, vp1, vp2, b);
    tfel::math::internals::SortEigenValues<N>::exe(vp0, vp1, vp2, o);
  }  // end of stensor<N,T>::computeEigenValues

  template <unsigned short N, typename T>
  template <typename stensor_common::EigenSolver es>
  TFEL_HOST_DEVICE void stensor<N, T>::computeEigenValues(
      tvector<3u, T>& vp, const EigenValuesOrdering o, const bool b) const {
    this->template computeEigenValues<es>(vp(0), vp(1), vp(2), o, b);
  }

  template <unsigned short N, typename T>
  template <typename stensor_common::EigenSolver es>
  TFEL_HOST_DEVICE tvector<3u, T> stensor<N, T>::computeEigenValues(
      const EigenValuesOrdering o, const bool b) const {
    tvector<3u, T> vp;
    this->template computeEigenValues<es>(vp, o, b);
    return vp;
  }  // end of stensor<N,T>::computeEigenValues

  template <unsigned short N, typename T>
  template <typename stensor_common::EigenSolver es>
  TFEL_HOST_DEVICE void stensor<N, T>::computeEigenVectors(
      tvector<3u, T>& vp,
      tmatrix<3u, 3u, base_type<T>>& m,
      const bool b) const {
    using ESolver = tfel::math::internals::StensorEigenSolver<es, N, T>;
    ESolver::computeEigenVectors(vp, m, this->v, b);
  }

  // computeEigenVectors
  template <unsigned short N, typename T>
  template <typename stensor_common::EigenSolver es>
  TFEL_HOST_DEVICE std::tuple<tvector<3u, T>, tmatrix<3u, 3u, base_type<T>>>
  stensor<N, T>::computeEigenVectors(const bool b) const {
    std::tuple<tvector<3u, T>, tmatrix<3u, 3u, base_type<T>>> r;
    this->template computeEigenVectors<es>(std::get<0>(r), std::get<1>(r), b);
    return r;
  }  // end of stensor<N,T>::computeEigenVectors

  template <unsigned short N, typename T>
  template <typename stensor_common::EigenSolver es>
  TFEL_HOST_DEVICE void stensor<N, T>::computeEigenVectors(
      tvector<3u, T>& vp,
      tmatrix<3u, 3u, base_type<T>>& m,
      const EigenValuesOrdering o,
      const bool b) const {
    this->template computeEigenVectors<es>(vp, m, b);
    tfel::math::internals::SortEigenVectors<N>::exe(vp, m, o);
  }  // end of stensor<N,T>::computeEigenVectors

  // computeEigenVectors
  template <unsigned short N, typename T>
  template <typename stensor_common::EigenSolver es>
  TFEL_HOST_DEVICE std::tuple<tvector<3u, T>, tmatrix<3u, 3u, base_type<T>>>
  stensor<N, T>::computeEigenVectors(const EigenValuesOrdering o,
                                     const bool b) const {
    std::tuple<tvector<3u, T>, tmatrix<3u, 3u, base_type<T>>> r;
    this->template computeEigenVectors<es>(std::get<0>(r), std::get<1>(r), o,
                                           b);
    return r;
  }  // end of stensor<N,T>::computeEigenVectors

  // computeEigenVectors
  template <unsigned short N, typename T>
  template <typename VectorType>
  TFEL_HOST_DEVICE bool stensor<N, T>::computeEigenVector(VectorType& ev,
                                                          const T vp) const {
    using real = numeric_type<VectorType>;
    typedef tfel::math::internals::StensorComputeEigenVectors<N> SCEV;
    static_assert(tfel::typetraits::IsFundamentalNumericType<real>::cond);
    static_assert(tfel::typetraits::IsReal<real>::cond);
    T values[StensorDimeToSize<N>::value];
    this->write(values);
    return SCEV::computeEigenVector(ev, values, base_type_cast(vp));
  }

  template <unsigned short N, typename T>
  TFEL_HOST_DEVICE constexpr auto stensor<N, T>::computeEigenValuesDerivatives(
      const rotation_matrix<T>& m) noexcept {
    return stensor<N, T>::computeEigenTensors(m);
  }  // end of stensor<N,T>::computeEigenValuesDerivatives

  template <unsigned short N, typename T>
  template <StensorConcept StensorType>
  TFEL_HOST_DEVICE constexpr void stensor<N, T>::computeEigenValuesDerivatives(
      StensorType& n0,
      StensorType& n1,
      StensorType& n2,
      const rotation_matrix<T>& m) noexcept  //
      requires((getSpaceDimension<StensorType>() == N) &&
               (isAssignableTo<base_type<T>, numeric_type<StensorType>>())) {
    return stensor<N, T>::computeEigenTensors(n0, n1, n2, m);
  }  // end of stensor<N,T>::computeEigenValuesDerivatives

  template <unsigned short N, typename T>
  TFEL_HOST_DEVICE constexpr auto stensor<N, T>::computeEigenTensors(
      const rotation_matrix<T>& m) noexcept {
    std::tuple<stensor<N, T>, stensor<N, T>, stensor<N, T>> r;
    tfel::math::internals::StensorComputeEigenValuesDerivatives<N>::exe(
        std::get<0>(r), std::get<1>(r), std::get<2>(r), m);
    return r;
  }  // end of stensor<N,T>::computeEigenTensors

  template <unsigned short N, typename T>
  template <StensorConcept StensorType>
  TFEL_HOST_DEVICE constexpr void stensor<N, T>::computeEigenTensors(
      StensorType& n0,
      StensorType& n1,
      StensorType& n2,
      const rotation_matrix<T>& m) noexcept  //
      requires((getSpaceDimension<StensorType>() == N) &&
               (isAssignableTo<base_type<T>, numeric_type<StensorType>>())) {
    return tfel::math::internals::StensorComputeEigenValuesDerivatives<N>::exe(
        n0, n1, n2, m);
  }  // end of stensor<N,T>::computeEigenTensors

  template <unsigned short N, typename T>
  template <ST2toST2Concept ST2toST2Type>
  TFEL_HOST_DEVICE void stensor<N, T>::computeEigenTensorsDerivatives(
      ST2toST2Type& dn0_ds,
      ST2toST2Type& dn1_ds,
      ST2toST2Type& dn2_ds,
      const tvector<3u, T>& vp,
      const rotation_matrix<T>& m,
      const T eps) requires((getSpaceDimension<ST2toST2Type>() == N) &&
                            (isAssignableTo<
                                BinaryOperationResult<base_type<T>, T, OpDiv>,
                                numeric_type<ST2toST2Type>>())) {
    return tfel::math::internals::StensorComputeEigenTensorsDerivatives<N>::exe(
        dn0_ds, dn1_ds, dn2_ds, vp, m, eps);
  }

  template <unsigned short N, typename T>
  template <typename Function>
  TFEL_HOST_DEVICE auto stensor<N, T>::computeIsotropicFunction(
      const Function& f,
      const tvector<3u, T>& vp,
      const rotation_matrix<T>& m) {
    return stensor<N, T>::buildFromEigenValuesAndVectors(f(vp(0)), f(vp(1)),
                                                         f(vp(2)), m);
  }

  template <unsigned short N, typename T>
  template <typename T2>
  TFEL_HOST_DEVICE constexpr auto stensor<N, T>::computeIsotropicFunction(
      const tvector<3u, T2>& f, const rotation_matrix<T>& m) noexcept {
    return stensor<N, T>::buildFromEigenValuesAndVectors(f[0], f[1], f[2], m);
  }

  template <unsigned short N, typename T>
  template <typename Function, typename FunctionDerivative>
  TFEL_HOST_DEVICE auto stensor<N, T>::computeIsotropicFunctionDerivative(
      const Function& f,
      const FunctionDerivative& df,
      const tvector<3u, T>& vp,
      const rotation_matrix<T>& m,
      const T eps) {
    st2tost2<N, std::invoke_result_t<FunctionDerivative, T>> r;
    stensor<N, T>::computeIsotropicFunctionDerivative(r, f, df, vp, m, eps);
    return r;
  }  // end of stensor<N,T>::computeIsotropicFunctionDerivative

  template <unsigned short N, typename T>
  template <ST2toST2Concept ST2toST2Type,
            typename Function,
            typename FunctionDerivative>
  TFEL_HOST_DEVICE void stensor<N, T>::computeIsotropicFunctionDerivative(
      ST2toST2Type& d,
      const Function& f,
      const FunctionDerivative& df,
      const tvector<3u, T>& vp,
      const rotation_matrix<T>& m,
      const T eps) requires((getSpaceDimension<ST2toST2Type>() == N) &&
                            (isAssignableTo<
                                BinaryOperationResult<base_type<T>, T, OpDiv>,
                                numeric_type<ST2toST2Type>>())) {
    tfel::math::internals::StensorComputeIsotropicFunctionDerivative<N>::exe2(
        d, f, df, vp, m, eps);
  }  // end of computeIsotropicFunctionDerivative

  template <unsigned short N, typename T>
  template <typename T1, typename T2>
  TFEL_HOST_DEVICE auto stensor<N, T>::computeIsotropicFunctionDerivative(
      const tvector<3u, T1>& f,
      const tvector<3u, T2>& df,
      const tvector<3u, T>& vp,
      const rotation_matrix<T>& m,
      const T eps) {
    st2tost2<N, T2> r;
    stensor<N, T>::computeIsotropicFunctionDerivative(r, f, df, vp, m, eps);
    return r;
  }  // end of stensor<N,T>::computeIsotropicFunctionDerivative

  template <unsigned short N, typename T>
  template <ST2toST2Concept ST2toST2Type, typename T1, typename T2>
  TFEL_HOST_DEVICE void stensor<N, T>::computeIsotropicFunctionDerivative(
      ST2toST2Type& d,
      const tvector<3u, T1>& f,
      const tvector<3u, T2>& df,
      const tvector<3u, T>& vp,
      const rotation_matrix<T>& m,
      const T eps) requires((getSpaceDimension<ST2toST2Type>() == N) &&
                            (isAssignableTo<
                                BinaryOperationResult<base_type<T>, T, OpDiv>,
                                numeric_type<ST2toST2Type>>())) {
    tfel::math::internals::StensorComputeIsotropicFunctionDerivative<N>::exe(
        d, f, df, vp, m, eps);
  }

  template <unsigned short N, typename T>
  template <typename stensor_common::EigenSolver es, typename Function>
  TFEL_HOST_DEVICE auto stensor<N, T>::computeIsotropicFunction(
      const Function& f, const bool b) const {
    const auto [vp, m] = this->template computeEigenVectors<es>(b);
    return stensor<N, T>::buildFromEigenValuesAndVectors(f(vp(0)), f(vp(1)),
                                                         f(vp(2)), m);
  }  // end of computeIsotropicFunction

  template <unsigned short N, typename T>
  template <typename stensor_common::EigenSolver es,
            typename Function,
            typename FunctionDerivative>
  TFEL_HOST_DEVICE auto stensor<N, T>::computeIsotropicFunctionDerivative(
      const Function& f,
      const FunctionDerivative& df,
      const T eps,
      const bool b) const {
    const auto [vp, m] = this->template computeEigenVectors<es>(b);
    st2tost2<N, T> r;
    stensor<N, T>::computeIsotropicFunctionDerivative(r, f, df, vp, m, eps);
    return r;
  }

  template <unsigned short N, typename T>
  template <typename stensor_common::EigenSolver es,
            typename Function,
            typename FunctionDerivative>
  TFEL_HOST_DEVICE auto stensor<N, T>::computeIsotropicFunctionAndDerivative(
      const Function& f,
      const FunctionDerivative& df,
      const T eps,
      const bool b) const {
    const auto [vp, m] = this->template computeEigenVectors<es>(b);
    const auto fv = map(f, vp);
    const auto dfv = map(df, vp);
    std::pair<stensor<N, T>, st2tost2<N, T>> r;
    r.first =
        stensor<N, T>::buildFromEigenValuesAndVectors(fv(0), fv(1), fv(2), m);
    stensor<N, T>::computeIsotropicFunctionDerivative(r.second, fv, dfv, vp, m,
                                                      eps);
    return r;
  }  // end of stensor<N,T>::computeIsotropicFunctionAndDerivative

  template <unsigned short N, typename T>
  TFEL_HOST_DEVICE constexpr void stensor<N, T>::changeBasis(
      const rotation_matrix<base_type<T>>& m) noexcept {
    return tfel::math::internals::StensorChangeBasis<N>::exe(this->v, m);
  }

  template <unsigned short N, typename T>
  constexpr stensor<N, base_type<T>> stensor<N, T>::Id() noexcept {
    static_assert((N == 1) || (N == 2) || (N == 3), "invalid space dimension");
    constexpr auto zero = base_type<T>{0};
    constexpr auto one = base_type<T>{1};
    if constexpr (N == 1) {
      return {one, one, one};
    } else if constexpr (N == 2) {
      return {one, one, one, zero};
    }
    return {one, one, one, zero, zero, zero};
  }  // end of stensor<N,T>::Id

  template <typename NumType, typename T>
  TFEL_HOST_DEVICE constexpr void setComponent(StensorConcept auto& A,
                                               unsigned short i,
                                               unsigned short j,
                                               const T& Aij) noexcept
      requires(isAssignableTo<NumType, T>()) {
    using StensorType = decltype(A);
    const auto N = getSpaceDimension<StensorType>();
    const unsigned short I = VoigtIndex<N>(i, j);
    A(I) = Aij;
    constexpr auto cste = Cste<NumType>::sqrt2;
    if (I > 2) {
      A(I) = A(I) * cste;
    }
  }

  TFEL_HOST_DEVICE constexpr auto getComponent(const StensorConcept auto& A,
                                               unsigned short int i,
                                               unsigned short int j) {
    using StensorType = decltype(A);
    using T = numeric_type<StensorType>;
    const auto N = getSpaceDimension<StensorType>();
    const unsigned short I = VoigtIndex<N>(i, j);
    constexpr auto icste = Cste<base_type<T>>::isqrt2;
    auto Aij = A(I);
    if (I > 2) {
      Aij *= icste;
    }
    return Aij;
  }

  template <unsigned short N, typename T>
  template <typename InputIterator>
  TFEL_HOST_DEVICE constexpr void stensor<N, T>::copy(
      const InputIterator src) noexcept {
    tfel::fsalgo::copy<StensorDimeToSize<N>::value>::exe(src, *this);
  }

  template <unsigned short N, typename T, typename OutputIterator>
  TFEL_HOST_DEVICE constexpr void exportToBaseTypeArray(
      const stensor<N, T>& s, OutputIterator p) noexcept
      requires(isScalar<T>()) {
    tfel::fsalgo::transform<StensorDimeToSize<N>::value>::exe(
        s.cbegin(), p, [](const auto& v) { return base_type_cast(v); });
  }

  template <typename T>
  TFEL_HOST_DEVICE constexpr T tresca(const stensor<1u, T>& s, const bool) {
    const auto sd1 = tfel::math::abs(s[0] - s[1]);
    const auto sd2 = tfel::math::abs(s[0] - s[2]);
    const auto sd3 = tfel::math::abs(s[2] - s[1]);
    const auto tmp = sd1 > sd2 ? sd1 : sd2;
    const auto tmp2 = sd3 > tmp ? sd3 : tmp;
    return tmp2;
  }  // end of tresca

  template <unsigned short N, typename T>
  TFEL_HOST_DEVICE T tresca(const stensor<N, T>& s, const bool b) {
    T s1, s2, s3;
    s.computeEigenValues(s1, s2, s3, b);
    const auto sd1 = tfel::math::abs(s1 - s2);
    const auto sd2 = tfel::math::abs(s1 - s3);
    const auto sd3 = tfel::math::abs(s3 - s2);
    const auto tmp = sd1 > sd2 ? sd1 : sd2;
    const auto tmp2 = sd3 > tmp ? sd3 : tmp;
    return tmp2;
  }  // end of tresca

  TFEL_HOST_DEVICE auto square_root(const StensorConcept auto& s) {
    using StensorType = decltype(s);
    using T = numeric_type<StensorType>;
    stensor<getSpaceDimension<StensorType>(), T> tmp(s);
    stensor<getSpaceDimension<StensorType>(), T> r(T(0));
    const auto [vp, m] = tmp.computeEigenVectors();
    r[0] = std::sqrt(vp[0]);
    r[1] = std::sqrt(vp[1]);
    r[2] = std::sqrt(vp[2]);
    r.changeBasis(transpose(m));
    return r;
  }  // end of square_root

  TFEL_HOST_DEVICE constexpr auto det(const StensorConcept auto& s) noexcept {
    using StensorType = decltype(s);
    constexpr auto N = getSpaceDimension<StensorType>();
    static_assert((N == 1) || (N == 2) || (N == 3), "invalid space dimension");
    if constexpr (N == 1) {
      return s(0) * s(1) * s(2);
    } else if constexpr (N == 2) {
      return s(2) * (s(0) * s(1) - s(3) * s(3) / 2);
    } else {
      constexpr auto cste = Cste<numeric_type<StensorType>>::sqrt2;
      return (2 * s(0) * s(1) * s(2) + cste * s(3) * s(4) * s(5) -
              s(2) * s(3) * s(3) - s(1) * s(4) * s(4) - s(0) * s(5) * s(5)) /
             2;
    }
  }  // end of det

  TFEL_HOST_DEVICE constexpr auto invert(
      const StensorConcept auto& s) noexcept {
    using StensorType = decltype(s);
    using Result =
        stensor<getSpaceDimension<StensorType>(),
                BinaryOperationResult<base_type<numeric_type<StensorType>>,
                                      numeric_type<StensorType>, OpDiv>>;
    constexpr auto N = getSpaceDimension<StensorType>();
    static_assert((N == 1) || (N == 2) || (N == 3), "invalid space dimension");
    if constexpr (N == 1) {
      return Result{1 / s(0), 1 / s(1), 1 / s(2)};
    } else if constexpr (N == 2) {
      const auto idet = 1 / det(s);
      return Result{s(1) * s(2) * idet, s(0) * s(2) * idet, 1 / s(2),
                    -s(2) * s(3) * idet};
    } else {
      using real = base_type<numeric_type<StensorType>>;
      constexpr auto two = real(2);
      constexpr auto one_half = real(1) / two;
      constexpr auto icste = Cste<real>::isqrt2;
      const auto iJ = 1 / det(s);
      return Result{(s(1) * s(2) - s(5) * s(5) * one_half) * iJ,
                    (s(0) * s(2) - s(4) * s(4) * one_half) * iJ,
                    (s(0) * s(1) - s(3) * s(3) * one_half) * iJ,
                    (icste * s(4) * s(5) - s(2) * s(3)) * iJ,
                    (icste * s(3) * s(5) - s(1) * s(4)) * iJ,
                    (icste * s(3) * s(4) - s(0) * s(5)) * iJ};
    }
  }  // end of invert

  template <unsigned short N, typename T>
  template <MatrixConcept MatrixType>
  TFEL_HOST_DEVICE constexpr auto stensor<N, T>::buildFromMatrix(
      const MatrixType& m) noexcept
      requires(isAssignableTo<numeric_type<MatrixType>, T>()) {
    if constexpr (N == 1) {
      return stensor<N, T>{m(0, 0), m(1, 1), m(2, 2)};
    } else if constexpr (N == 2) {
      constexpr auto cste = Cste<T>::isqrt2;
      return stensor<N, T>{m(0, 0), m(1, 1), m(2, 2),
                           (m(0, 1) + m(1, 0)) * cste};
    } else {
      constexpr auto cste = Cste<T>::isqrt2;
      return stensor<N, T>{m(0, 0),
                           m(1, 1),
                           m(2, 2),
                           (m(0, 1) + m(1, 0)) * cste,
                           (m(0, 2) + m(2, 0)) * cste,
                           (m(2, 1) + m(1, 2)) * cste};
    }
  }  // end of buildFromMatrix

  template <unsigned short N, typename T>
  template <VectorConcept VectorType>
  TFEL_HOST_DEVICE constexpr auto stensor<N, T>::buildFromVectorDiadicProduct(
      const VectorType& v) noexcept  //
      requires(
          isAssignableTo<typename ComputeUnaryResult<numeric_type<VectorType>,
                                                     Power<2>>::Result,
                         T>()) {
    if constexpr (N == 1) {
      return stensor<N, T>{v(0) * v(0), v(1) * v(1), v(2) * v(2)};
    } else if constexpr (N == 2) {
      constexpr auto cste = Cste<T>::sqrt2;
      return stensor<N, T>{v(0) * v(0), v(1) * v(1), v(2) * v(2),
                           v(0) * v(1) * cste};
    } else {
      constexpr auto cste = Cste<T>::sqrt2;
      return stensor<N, T>{v(0) * v(0),        v(1) * v(1),
                           v(2) * v(2),        v(0) * v(1) * cste,
                           v(0) * v(2) * cste, v(1) * v(2) * cste};
    }
  }  // end of buildFromVectorDiadicProduct

  template <unsigned short N, typename T>
  template <VectorConcept VectorType, VectorConcept VectorType2>
  TFEL_HOST_DEVICE constexpr auto
  stensor<N, T>::buildFromVectorsSymmetricDiadicProduct(
      const VectorType& v1,
      const VectorType2& v2) noexcept  //
      requires(isAssignableTo<BinaryOperationResult<numeric_type<VectorType>,
                                                    numeric_type<VectorType2>,
                                                    OpMult>,
                              T>()) {
    if constexpr (N == 1) {
      return stensor<N, T>{2 * v1(0) * v2(0), 2 * v1(1) * v2(1),
                           2 * v1(2) * v2(2)};
    } else if constexpr (N == 2) {
      constexpr auto cste = Cste<T>::sqrt2;
      return stensor<N, T>{2 * v1(0) * v2(0), 2 * v1(1) * v2(1),
                           2 * v1(2) * v2(2),
                           cste * (v1(0) * v2(1) + v2(0) * v1(1))};
    } else {
      constexpr auto cste = Cste<T>::sqrt2;
      return stensor<N, T>{2 * v1(0) * v2(0),
                           2 * v1(1) * v2(1),
                           2 * v1(2) * v2(2),
                           (v1(0) * v2(1) + v2(0) * v1(1)) * cste,
                           (v1(0) * v2(2) + v2(0) * v1(2)) * cste,
                           (v1(1) * v2(2) + v2(1) * v1(2)) * cste};
    }
  }  // end of buildFromVectorsSymmetricDiadicProduct

  template <unsigned short N, typename T>
  TFEL_HOST_DEVICE constexpr auto stensor<N, T>::buildFromEigenValuesAndVectors(
      const T& v1,
      const T& v2,
      const T& v3,
      const rotation_matrix<T>& m) noexcept {
    if constexpr (N == 1) {
      return stensor<N, T>{v1, v2, v3};
    } else if constexpr (N == 2) {
      constexpr auto cste = Cste<T>::sqrt2;
      return stensor<N, T>{
          v1 * m(0, 0) * m(0, 0) + v2 * m(0, 1) * m(0, 1),
          v1 * m(1, 0) * m(1, 0) + v2 * m(1, 1) * m(1, 1), v3,
          (v1 * m(0, 0) * m(1, 0) + v2 * m(0, 1) * m(1, 1)) * cste};
    } else {
      constexpr auto cste = Cste<T>::sqrt2;
      return stensor<N, T>{v1 * m(0, 0) * m(0, 0) + v2 * m(0, 1) * m(0, 1) +
                               v3 * m(0, 2) * m(0, 2),
                           v1 * m(1, 0) * m(1, 0) + v2 * m(1, 1) * m(1, 1) +
                               v3 * m(1, 2) * m(1, 2),
                           v1 * m(2, 0) * m(2, 0) + v2 * m(2, 1) * m(2, 1) +
                               v3 * m(2, 2) * m(2, 2),
                           (v1 * m(0, 0) * m(1, 0) + v2 * m(0, 1) * m(1, 1) +
                            v3 * m(0, 2) * m(1, 2)) *
                               cste,
                           (v1 * m(0, 0) * m(2, 0) + v2 * m(0, 1) * m(2, 1) +
                            v3 * m(0, 2) * m(2, 2)) *
                               cste,
                           (v1 * m(1, 0) * m(2, 0) + v2 * m(1, 1) * m(2, 1) +
                            v3 * m(1, 2) * m(2, 2)) *
                               cste};
    }
  }  // end of buildFromEigenValuesAndVectors

  template <unsigned short N, typename T>
  TFEL_HOST_DEVICE constexpr auto stensor<N, T>::buildFromEigenValuesAndVectors(
      const tvector<3u, T>& vp, const rotation_matrix<T>& m) noexcept {
    return stensor::buildFromEigenValuesAndVectors(vp(0), vp(1), vp(2), m);
  }  // end of buildFromEigenValuesAndVectors

  template <unsigned short N, typename T>
  TFEL_HOST_DEVICE auto stensor<N, T>::buildLogarithmFromEigenValuesAndVectors(
      const T& v1,
      const T& v2,
      const T& v3,
      const rotation_matrix<T>& m) noexcept {
    return stensor::buildFromEigenValuesAndVectors(std::log(v1), std::log(v2),
                                                   std::log(v3), m);
  }  // end of buildLogarithmFromEigenValuesAndVectors

  template <unsigned short N, typename T>
  TFEL_HOST_DEVICE auto stensor<N, T>::buildLogarithmFromEigenValuesAndVectors(
      const tvector<3u, T>& vp, const rotation_matrix<T>& m) noexcept {
    return stensor::buildFromEigenValuesAndVectors(
        std::log(vp(0)), std::log(vp(1)), std::log(vp(2)), m);
  }  // end of buildLogarithmFromEigenValuesAndVectors

  template <unsigned short N, typename T>
  TFEL_HOST_DEVICE constexpr auto
  stensor<N, T>::buildPositivePartFromEigenValuesAndVectors(
      const T& v1,
      const T& v2,
      const T& v3,
      const rotation_matrix<T>& m) noexcept {
    constexpr auto zero = T{};
    return stensor::buildFromEigenValuesAndVectors(
        std::max(zero, v1), std::max(zero, v2), std::max(zero, v3), m);
  }  // end of buildPositivePartFromEigenValuesAndVectors

  template <unsigned short N, typename T>
  TFEL_HOST_DEVICE constexpr auto
  stensor<N, T>::buildPositivePartFromEigenValuesAndVectors(
      const tvector<3u, T>& vp, const rotation_matrix<T>& m) noexcept {
    constexpr auto zero = T{};
    return stensor::buildFromEigenValuesAndVectors(
        std::max(zero, vp(0)), std::max(zero, vp(1)), std::max(zero, vp(2)), m);
  }  // end of buildPositivePartFromEigenValuesAndVectors

  template <unsigned short N, typename T>
  TFEL_HOST_DEVICE constexpr auto
  stensor<N, T>::buildNegativePartFromEigenValuesAndVectors(
      const T& v1,
      const T& v2,
      const T& v3,
      const rotation_matrix<T>& m) noexcept {
    constexpr auto zero = T{};
    return stensor::buildFromEigenValuesAndVectors(
        std::min(zero, v1), std::min(zero, v2), std::min(zero, v3), m);
  }  // end of buildNegativePartFromEigenValuesAndVectors

  template <unsigned short N, typename T>
  TFEL_HOST_DEVICE constexpr auto
  stensor<N, T>::buildNegativePartFromEigenValuesAndVectors(
      const tvector<3u, T>& vp, const rotation_matrix<T>& m) noexcept {
    constexpr auto zero = T{};
    return stensor::buildFromEigenValuesAndVectors(
        std::min(zero, vp(0)), std::min(zero, vp(1)), std::min(zero, vp(2)), m);
  }  // end of buildNegativePartFromEigenValuesAndVectors

  template <StensorConcept StensorType>
  TFEL_HOST_DEVICE auto logarithm(const StensorType& s, const bool b) requires(
      tfel::typetraits::IsFundamentalNumericType<
          numeric_type<StensorType>>::cond) {
    using NumType = numeric_type<StensorType>;
    constexpr auto N = getSpaceDimension<StensorType>();
    if constexpr (N == 1) {
      return stensor<1u, NumType>{std::log(s(0)), std::log(s(1)),
                                  std::log(s(2))};
    } else {
      stensor<N, NumType> tmp(s);
      const auto [vp, m] = tmp.computeEigenVectors(b);
      return stensor<N, NumType>::buildFromEigenValuesAndVectors(
          std::log(vp(0)), std::log(vp(1)), std::log(vp(2)), m);
    }
  }  // end of logarithm

  template <StensorConcept StensorType>
  TFEL_HOST_DEVICE auto
  absolute_value(const StensorType& s, const bool b) requires(
      tfel::typetraits::IsFundamentalNumericType<
          numeric_type<StensorType>>::cond) {
    using NumType = numeric_type<StensorType>;
    constexpr auto N = getSpaceDimension<StensorType>();
    if constexpr (N == 1) {
      return stensor<1u, NumType>{tfel::math::abs(s(0)), tfel::math::abs(s(1)),
                                  tfel::math::abs(s(2))};
    } else {
      stensor<N, NumType> tmp(s);
      const auto [vp, m] = tmp.computeEigenVectors(b);
      return stensor<N, NumType>::buildFromEigenValuesAndVectors(
          tfel::math::abs(vp(0)), tfel::math::abs(vp(1)),
          tfel::math::abs(vp(2)), m);
    }
  }  // end of absolute_value

  template <StensorConcept StensorType>
  TFEL_HOST_DEVICE auto
  positive_part(const StensorType& s, const bool b) requires(
      tfel::typetraits::IsFundamentalNumericType<
          numeric_type<StensorType>>::cond) {
    using NumType = numeric_type<StensorType>;
    constexpr auto N = getSpaceDimension<StensorType>();
    constexpr auto zero = NumType{};
    if constexpr (N == 1) {
      return stensor<1u, NumType>{std::max(s(0), zero), std::max(s(1), zero),
                                  std::max(s(2), zero)};
    } else {
      stensor<N, NumType> tmp(s);
      const auto [vp, m] = tmp.computeEigenVectors(b);
      return stensor<N, NumType>::buildFromEigenValuesAndVectors(
          std::max(vp(0), zero),  //
          std::max(vp(1), zero), std::max(vp(2), zero), m);
    }
  }  // end of positive_part

  template <StensorConcept StensorType>
  TFEL_HOST_DEVICE auto
  negative_part(const StensorType& s, const bool b) requires(
      tfel::typetraits::IsFundamentalNumericType<
          numeric_type<StensorType>>::cond) {
    using NumType = numeric_type<StensorType>;
    constexpr auto N = getSpaceDimension<StensorType>();
    constexpr auto zero = NumType{};
    if constexpr (N == 1) {
      return stensor<1u, NumType>{std::min(s(0), zero), std::min(s(1), zero),
                                  std::min(s(2), zero)};
    } else {
      stensor<N, NumType> tmp(s);
      const auto [vp, m] = tmp.computeEigenVectors(b);
      return stensor<N, NumType>::buildFromEigenValuesAndVectors(
          std::min(vp(0), zero),  //
          std::min(vp(1), zero), std::min(vp(2), zero), m);
    }
  }  // end of negative_part

  TFEL_HOST_DEVICE auto square(const StensorConcept auto& s) {
    using StensorType = decltype(s);
    constexpr auto N = getSpaceDimension<StensorType>();
    using T = numeric_type<StensorType>;
    using T2 = BinaryOperationResult<T, T, OpMult>;
    if constexpr (N == 1) {
      return stensor<1u, T2>{s(0) * s(0), s(1) * s(1), s(2) * s(2)};
    } else if constexpr (N == 2) {
      using base = base_type<T>;
      constexpr const base one_half = 1 / base(2);
      return stensor<2u, T2>{(s(3) * s(3) + 2 * s(0) * s(0)) * one_half,
                             (s(3) * s(3) + 2 * s(1) * s(1)) * one_half,
                             s(2) * s(2), (s(1) + s(0)) * s(3)};
    } else {
      using base = base_type<T>;
      constexpr base cste = Cste<base>::sqrt2;
      constexpr const base one_half = 1 / (base(2));
      return stensor<3u, T2>{
          (s(4) * s(4) + s(3) * s(3) + 2 * s(0) * s(0)) * one_half,
          (s(5) * s(5) + s(3) * s(3) + 2 * s(1) * s(1)) * one_half,
          (s(5) * s(5) + s(4) * s(4) + 2 * s(2) * s(2)) * one_half,
          (cste * s(4) * s(5) + 2 * (s(1) + s(0)) * s(3)) * one_half,
          (cste * s(3) * s(5) + 2 * (s(2) + s(0)) * s(4)) * one_half,
          (2 * (s(2) + s(1)) * s(5) + cste * s(3) * s(4)) * one_half};
    }
  }  // end of square

  template <StensorConcept StensorType>
  TFEL_HOST_DEVICE constexpr auto change_basis(
      const StensorType& s,
      const tmatrix<3u, 3u, base_type<numeric_type<StensorType>>>& r) noexcept {
    stensor<getSpaceDimension<StensorType>(), numeric_type<StensorType>> ns{s};
    ns.changeBasis(r);
    return ns;
  }

  template <typename stensor_common::EigenSolver es,
            typename Function,
            StensorConcept StensorType>
  TFEL_HOST_DEVICE auto computeIsotropicFunction(const Function& f,
                                                 const StensorType& s,
                                                 const bool b) {
    return s.template computeIsotropicFunction<es>(f, b);
  }

  template <typename stensor_common::EigenSolver es,
            typename Function,
            typename FunctionDerivative,
            StensorConcept StensorType>
  TFEL_HOST_DEVICE auto computeIsotropicFunctionDerivative(
      const Function& f,
      const FunctionDerivative& df,
      const StensorType& s,
      const numeric_type<StensorType> eps,
      const bool b) {
    return s.template computeIsotropicFunctionDerivative<es>(f, df, eps, b);
  }

  template <typename stensor_common::EigenSolver es,
            typename Function,
            typename FunctionDerivative,
            StensorConcept StensorType>
  TFEL_HOST_DEVICE auto computeIsotropicFunctionAndDerivative(
      const Function& f,
      const FunctionDerivative& df,
      const StensorType& s,
      const numeric_type<StensorType> eps,
      const bool b) {
    return s.template computeIsotropicFunctionAndDerivative<es>(f, df, eps, b);
  }

  template <StensorConcept T, StensorConcept T2>
  TFEL_HOST_DEVICE constexpr auto
  convertCorotationnalCauchyStressToSecondPiolaKirchhoffStress(
      const T& s, const T2& U) noexcept
      requires((getSpaceDimension<T>() == getSpaceDimension<T2>()) &&
               (tfel::typetraits::IsFundamentalNumericType<
                   numeric_type<T2>>::cond)) {
    constexpr auto N = getSpaceDimension<T>();
    using Result = stensor<N, numeric_type<T>>;
    const auto J = det(U);
    if constexpr (N == 1) {
      return Result{J * s[0] / (U[0] * U[0]), J * s[1] / (U[1] * U[1]),
                    J * s[2] / (U[2] * U[2])};
    } else if constexpr (N == 2) {
      const auto iU = invert(U);
      return Result{J *
                        (s[1] * iU[3] * iU[3] + 2 * s[3] * iU[0] * iU[3] +
                         2 * s[0] * iU[0] * iU[0]) /
                        2,
                    J *
                        (s[0] * iU[3] * iU[3] + 2 * s[3] * iU[1] * iU[3] +
                         2 * s[1] * iU[1] * iU[1]) /
                        2,
                    J * s[2] * iU[2] * iU[2],
                    J *
                        (s[3] * iU[3] * iU[3] +
                         (2 * s[1] * iU[1] + 2 * s[0] * iU[0]) * iU[3] +
                         2 * s[3] * iU[0] * iU[1]) /
                        2};
    } else {
      constexpr auto cste = Cste<numeric_type<T>>::sqrt2;
      const auto iU = invert(U);
      return Result{
          J *
              (s[2] * iU[4] * iU[4] +
               (cste * s[5] * iU[3] + 2 * s[4] * iU[0]) * iU[4] +
               s[1] * iU[3] * iU[3] + 2 * s[3] * iU[0] * iU[3] +
               2 * s[0] * iU[0] * iU[0]) /
              2,
          J *
              (s[2] * iU[5] * iU[5] +
               (cste * s[4] * iU[3] + 2 * s[5] * iU[1]) * iU[5] +
               s[0] * iU[3] * iU[3] + 2 * s[3] * iU[1] * iU[3] +
               2 * s[1] * iU[1] * iU[1]) /
              2,
          J *
              (s[1] * iU[5] * iU[5] +
               (cste * s[3] * iU[4] + 2 * s[5] * iU[2]) * iU[5] +
               s[0] * iU[4] * iU[4] + 2 * s[4] * iU[2] * iU[4] +
               2 * s[2] * iU[2] * iU[2]) /
              2,
          J *
              ((cste * s[2] * iU[4] + s[5] * iU[3] + cste * s[4] * iU[0]) *
                   iU[5] +
               (s[4] * iU[3] + cste * s[5] * iU[1]) * iU[4] +
               s[3] * iU[3] * iU[3] +
               (2 * s[1] * iU[1] + 2 * s[0] * iU[0]) * iU[3] +
               2 * s[3] * iU[0] * iU[1]) /
              2,
          J *
              ((s[5] * iU[4] + cste * s[1] * iU[3] + cste * s[3] * iU[0]) *
                   iU[5] +
               s[4] * iU[4] * iU[4] +
               (s[3] * iU[3] + 2 * s[2] * iU[2] + 2 * s[0] * iU[0]) * iU[4] +
               cste * s[5] * iU[2] * iU[3] + 2 * s[4] * iU[0] * iU[2]) /
              2,
          J *
              (s[5] * iU[5] * iU[5] +
               (s[4] * iU[4] + s[3] * iU[3] + 2 * s[2] * iU[2] +
                2 * s[1] * iU[1]) *
                   iU[5] +
               (cste * s[0] * iU[3] + cste * s[3] * iU[1]) * iU[4] +
               cste * s[4] * iU[2] * iU[3] + 2 * s[5] * iU[1] * iU[2]) /
              2};
    }
  }

  template <StensorConcept T, StensorConcept T2>
  TFEL_HOST_DEVICE constexpr auto
  convertSecondPiolaKirchhoffStressToCorotationnalCauchyStress(
      const T& S,
      const T2& U) noexcept  //
      requires((getSpaceDimension<T>() == getSpaceDimension<T2>()) &&
               (tfel::typetraits::IsFundamentalNumericType<
                   numeric_type<T2>>::cond)) {
    constexpr auto N = getSpaceDimension<T>();
    using Result = stensor<N, numeric_type<T>>;
    const auto iJ = 1 / det(U);
    if constexpr (N == 1) {
      return Result{iJ * U[0] * S[0] * U[0], iJ * U[1] * S[1] * U[1],
                    iJ * U[2] * S[2] * U[2]};
    } else if constexpr (N == 2) {
      return Result{
          iJ *
              (S[1] * U[3] * U[3] + 2 * S[3] * U[0] * U[3] +
               2 * S[0] * U[0] * U[0]) /
              2,
          iJ *
              (S[0] * U[3] * U[3] + 2 * S[3] * U[1] * U[3] +
               2 * S[1] * U[1] * U[1]) /
              2,
          iJ * S[2] * U[2] * U[2],
          iJ *
              (S[3] * U[3] * U[3] + (2 * S[1] * U[1] + 2 * S[0] * U[0]) * U[3] +
               2 * S[3] * U[0] * U[1]) /
              2};
    } else {
      constexpr auto cste = Cste<numeric_type<T>>::sqrt2;
      return Result{
          iJ *
              (S[2] * U[4] * U[4] +
               (cste * S[5] * U[3] + 2 * S[4] * U[0]) * U[4] +
               S[1] * U[3] * U[3] + 2 * S[3] * U[0] * U[3] +
               2 * S[0] * U[0] * U[0]) /
              2,
          iJ *
              (S[2] * U[5] * U[5] +
               (cste * S[4] * U[3] + 2 * S[5] * U[1]) * U[5] +
               S[0] * U[3] * U[3] + 2 * S[3] * U[1] * U[3] +
               2 * S[1] * U[1] * U[1]) /
              2,
          iJ *
              (S[1] * U[5] * U[5] +
               (cste * S[3] * U[4] + 2 * S[5] * U[2]) * U[5] +
               S[0] * U[4] * U[4] + 2 * S[4] * U[2] * U[4] +
               2 * S[2] * U[2] * U[2]) /
              2,
          iJ *
              ((cste * S[2] * U[4] + S[5] * U[3] + cste * S[4] * U[0]) * U[5] +
               (S[4] * U[3] + cste * S[5] * U[1]) * U[4] + S[3] * U[3] * U[3] +
               (2 * S[1] * U[1] + 2 * S[0] * U[0]) * U[3] +
               2 * S[3] * U[0] * U[1]) /
              2,
          iJ *
              ((S[5] * U[4] + cste * S[1] * U[3] + cste * S[3] * U[0]) * U[5] +
               S[4] * U[4] * U[4] +
               (S[3] * U[3] + 2 * S[2] * U[2] + 2 * S[0] * U[0]) * U[4] +
               cste * S[5] * U[2] * U[3] + 2 * S[4] * U[0] * U[2]) /
              2,
          iJ *
              (S[5] * U[5] * U[5] +
               (S[4] * U[4] + S[3] * U[3] + 2 * S[2] * U[2] + 2 * S[1] * U[1]) *
                   U[5] +
               (cste * S[0] * U[3] + cste * S[3] * U[1]) * U[4] +
               cste * S[4] * U[2] * U[3] + 2 * S[5] * U[1] * U[2]) /
              2};
    }
  }  // end of convertSecondPiolaKirchhoffStressToCorotationnalCauchyStress

  template <StensorConcept StensorType1, StensorConcept StensorType2>
  TFEL_HOST_DEVICE constexpr auto symmetric_product(
      const StensorType1& s1, const StensorType2& s2) noexcept
      requires(getSpaceDimension<StensorType1>() ==
               getSpaceDimension<StensorType2>()) {
    using Result = stensor<getSpaceDimension<StensorType1>(),
                           result_type<numeric_type<StensorType1>,
                                       numeric_type<StensorType2>, OpMult>>;
    constexpr auto N = getSpaceDimension<StensorType1>();
    static_assert((N == 1) || (N == 2) || (N == 3), "invalid space dimension");
    if constexpr (N == 1u) {
      return Result{2 * s1[0] * s2[0], 2 * s1[1] * s2[1], 2 * s1[2] * s2[2]};
    } else if constexpr (N == 2u) {
      return Result{2 * s1[0] * s2[0] + s1[3] * s2[3],
                    2 * s1[1] * s2[1] + s1[3] * s2[3], 2 * s1[2] * s2[2],
                    (s1[1] + s1[0]) * s2[3] + s1[3] * s2[1] + s1[3] * s2[0]};
    } else {
      using res = result_type<numeric_type<StensorType1>,
                              numeric_type<StensorType2>, OpMult>;
      constexpr auto icste = Cste<res>::isqrt2;
      return Result{s1[4] * s2[4] + s1[3] * s2[3] + 2 * s1[0] * s2[0],
                    s1[5] * s2[5] + s1[3] * s2[3] + 2 * s1[1] * s2[1],
                    s1[5] * s2[5] + s1[4] * s2[4] + 2 * s1[2] * s2[2],
                    icste * s1[4] * s2[5] + icste * s1[5] * s2[4] +
                        (s1[1] + s1[0]) * s2[3] + s1[3] * s2[1] + s1[3] * s2[0],
                    icste * s1[3] * s2[5] + (s1[2] + s1[0]) * s2[4] +
                        icste * s1[5] * s2[3] + s1[4] * s2[2] + s1[4] * s2[0],
                    (s1[2] + s1[1]) * s2[5] + icste * s1[3] * s2[4] +
                        icste * s1[4] * s2[3] + s1[5] * s2[2] + s1[5] * s2[1]};
    }
  }  // end of symmetric_product

  TFEL_HOST_DEVICE constexpr auto computeDeterminantDerivative(
      const StensorConcept auto& s) noexcept {
    using StensorType = decltype(s);
    stensor<getSpaceDimension<StensorType>(),
            typename ComputeUnaryResult<numeric_type<StensorType>,
                                        Power<2>>::Result>
        dJ;
    computeDeterminantDerivative(dJ, s);
    return dJ;
  }

  TFEL_HOST_DEVICE constexpr auto computeDeviatorDeterminantDerivative(
      const StensorConcept auto& s) noexcept {
    using StensorType = decltype(s);
    stensor<getSpaceDimension<StensorType>(),
            typename ComputeUnaryResult<numeric_type<StensorType>,
                                        Power<2>>::Result>
        dJ;
    computeDeviatorDeterminantDerivative(dJ, s);
    return dJ;
  }

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_STENSOR_IXX */
