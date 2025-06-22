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
  std::enable_if_t<
      std::is_same<typename std::iterator_traits<InputIterator>::value_type,
                   base_type<T>>::value,
      void>
  stensor<N, T>::importVoigt(const InputIterator p) {
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
  std::enable_if_t<
      std::is_same<typename std::iterator_traits<InputIterator>::value_type,
                   base_type<T>>::value,
      void>
  stensor<N, T>::importTab(const InputIterator p) {
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
  std::enable_if_t<
      std::is_same<typename std::iterator_traits<InputIterator>::value_type,
                   base_type<T>>::value,
      void>
  stensor<N, T>::import(const InputIterator p) {
    tfel::fsalgo::transform<StensorDimeToSize<N>::value>::exe(
        p, this->begin(), [](const auto& value) { return T(value); });
  }  // end of import

  template <unsigned short N, typename T>
  void stensor<N, T>::exportTab(base_type<T>* const p) const {
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
  void stensor<N, T>::write(base_type<T>* const t) const {
    tfel::fsalgo::transform<StensorDimeToSize<N>::value>::exe(
        this->cbegin(), t,
        [](const auto& value) { return base_type_cast(value); });
  }

  template <unsigned short N, typename T>
  template <typename stensor_common::EigenSolver es>
  void stensor<N, T>::computeEigenValues(T& vp0,
                                         T& vp1,
                                         T& vp2,
                                         const bool b) const {
    using ESolver = tfel::math::internals::StensorEigenSolver<es, N, T>;
    ESolver::computeEigenValues(vp0, vp1, vp2, this->v, b);
  }  // end of stensor<N,T>::computeEigenValues

  template <unsigned short N, typename T>
  template <typename stensor_common::EigenSolver es>
  void stensor<N, T>::computeEigenValues(tvector<3u, T>& vp,
                                         const bool b) const {
    this->template computeEigenValues<es>(vp(0), vp(1), vp(2), b);
  }

  template <unsigned short N, typename T>
  template <typename stensor_common::EigenSolver es>
  tvector<3u, T> stensor<N, T>::computeEigenValues(const bool b) const {
    tvector<3u, T> vp;
    this->template computeEigenValues<es>(vp, b);
    return vp;
  }  // end of stensor<N,T>::computeEigenValues

  template <unsigned short N, typename T>
  template <typename stensor_common::EigenSolver es>
  void stensor<N, T>::computeEigenValues(
      T& vp0, T& vp1, T& vp2, const EigenValuesOrdering o, const bool b) const {
    this->template computeEigenValues<es>(vp0, vp1, vp2, b);
    tfel::math::internals::SortEigenValues<N>::exe(vp0, vp1, vp2, o);
  }  // end of stensor<N,T>::computeEigenValues

  template <unsigned short N, typename T>
  template <typename stensor_common::EigenSolver es>
  void stensor<N, T>::computeEigenValues(tvector<3u, T>& vp,
                                         const EigenValuesOrdering o,
                                         const bool b) const {
    this->template computeEigenValues<es>(vp(0), vp(1), vp(2), o, b);
  }

  template <unsigned short N, typename T>
  template <typename stensor_common::EigenSolver es>
  tvector<3u, T> stensor<N, T>::computeEigenValues(const EigenValuesOrdering o,
                                                   const bool b) const {
    tvector<3u, T> vp;
    this->template computeEigenValues<es>(vp, o, b);
    return vp;
  }  // end of stensor<N,T>::computeEigenValues

  template <unsigned short N, typename T>
  template <typename stensor_common::EigenSolver es>
  void stensor<N, T>::computeEigenVectors(tvector<3u, T>& vp,
                                          tmatrix<3u, 3u, base_type<T>>& m,
                                          const bool b) const {
    using ESolver = tfel::math::internals::StensorEigenSolver<es, N, T>;
    ESolver::computeEigenVectors(vp, m, this->v, b);
  }

  // computeEigenVectors
  template <unsigned short N, typename T>
  template <typename stensor_common::EigenSolver es>
  std::tuple<tvector<3u, T>, tmatrix<3u, 3u, base_type<T>>>
  stensor<N, T>::computeEigenVectors(const bool b) const {
    std::tuple<tvector<3u, T>, tmatrix<3u, 3u, base_type<T>>> r;
    this->template computeEigenVectors<es>(std::get<0>(r), std::get<1>(r), b);
    return r;
  }  // end of stensor<N,T>::computeEigenVectors

  template <unsigned short N, typename T>
  template <typename stensor_common::EigenSolver es>
  void stensor<N, T>::computeEigenVectors(tvector<3u, T>& vp,
                                          tmatrix<3u, 3u, base_type<T>>& m,
                                          const EigenValuesOrdering o,
                                          const bool b) const {
    this->template computeEigenVectors<es>(vp, m, b);
    tfel::math::internals::SortEigenVectors<N>::exe(vp, m, o);
  }  // end of stensor<N,T>::computeEigenVectors

  // computeEigenVectors
  template <unsigned short N, typename T>
  template <typename stensor_common::EigenSolver es>
  std::tuple<tvector<3u, T>, tmatrix<3u, 3u, base_type<T>>>
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
  bool stensor<N, T>::computeEigenVector(VectorType& ev, const T vp) const {
    using real = numeric_type<VectorType>;
    typedef tfel::math::internals::StensorComputeEigenVectors<N> SCEV;
    static_assert(tfel::typetraits::IsFundamentalNumericType<real>::cond);
    static_assert(tfel::typetraits::IsReal<real>::cond);
    T values[StensorDimeToSize<N>::value];
    this->write(values);
    return SCEV::computeEigenVector(ev, values, base_type_cast(vp));
  }

  template <unsigned short N, typename T>
  std::tuple<stensor<N, T>, stensor<N, T>, stensor<N, T>>
  stensor<N, T>::computeEigenValuesDerivatives(
      const tmatrix<3u, 3u, base_type<T>>& m) {
    return stensor<N, T>::computeEigenTensors(m);
  }  // end of stensor<N,T>::computeEigenValuesDerivatives

  template <unsigned short N, typename T>
  template <typename StensorType>
  std::enable_if_t<
      (implementsStensorConcept<StensorType>()) &&
          (getSpaceDimension<StensorType>() == N) &&
          (isAssignableTo<base_type<T>, numeric_type<StensorType>>()),
      void>
  stensor<N, T>::computeEigenValuesDerivatives(
      StensorType& n0,
      StensorType& n1,
      StensorType& n2,
      const tmatrix<3u, 3u, base_type<T>>& m) {
    return stensor<N, T>::computeEigenTensors(n0, n1, n2, m);
  }  // end of stensor<N,T>::computeEigenValuesDerivatives

  template <unsigned short N, typename T>
  std::tuple<stensor<N, T>, stensor<N, T>, stensor<N, T>>
  stensor<N, T>::computeEigenTensors(const tmatrix<3u, 3u, base_type<T>>& m) {
    std::tuple<stensor<N, T>, stensor<N, T>, stensor<N, T>> r;
    tfel::math::internals::StensorComputeEigenValuesDerivatives<N>::exe(
        std::get<0>(r), std::get<1>(r), std::get<2>(r), m);
    return r;
  }  // end of stensor<N,T>::computeEigenTensors

  template <unsigned short N, typename T>
  template <typename StensorType>
  std::enable_if_t<
      (implementsStensorConcept<StensorType>()) &&
          (getSpaceDimension<StensorType>() == N) &&
          (isAssignableTo<base_type<T>, numeric_type<StensorType>>()),
      void>
  stensor<N, T>::computeEigenTensors(StensorType& n0,
                                     StensorType& n1,
                                     StensorType& n2,
                                     const rotation_matrix<T>& m) {
    return tfel::math::internals::StensorComputeEigenValuesDerivatives<N>::exe(
        n0, n1, n2, m);
  }  // end of stensor<N,T>::computeEigenTensors

  template <unsigned short N, typename T>
  template <typename ST2toST2Type>
  std::enable_if_t<
      (implementsST2toST2Concept<ST2toST2Type>()) &&
          (getSpaceDimension<ST2toST2Type>() == N) &&
          (isAssignableTo<BinaryOperationResult<base_type<T>, T, OpDiv>,
                          numeric_type<ST2toST2Type>>()),
      void>
  stensor<N, T>::computeEigenTensorsDerivatives(ST2toST2Type& dn0_ds,
                                                ST2toST2Type& dn1_ds,
                                                ST2toST2Type& dn2_ds,
                                                const tvector<3u, T>& vp,
                                                const rotation_matrix<T>& m,
                                                const T eps) {
    return tfel::math::internals::StensorComputeEigenTensorsDerivatives<N>::exe(
        dn0_ds, dn1_ds, dn2_ds, vp, m, eps);
  }

  template <unsigned short N, typename T>
  template <typename Function>
  stensor<N, std::invoke_result_t<Function, T>>
  stensor<N, T>::computeIsotropicFunction(const Function& f,
                                          const tvector<3u, T>& vp,
                                          const rotation_matrix<T>& m) {
    return stensor<N, T>::buildFromEigenValuesAndVectors(f(vp(0)), f(vp(1)),
                                                         f(vp(2)), m);
  }

  template <unsigned short N, typename T>
  template <typename T2>
  stensor<N, T2> stensor<N, T>::computeIsotropicFunction(
      const tvector<3u, T2>& f, const rotation_matrix<T>& m) {
    return stensor<N, T>::buildFromEigenValuesAndVectors(f[0], f[1], f[2], m);
  }

  template <unsigned short N, typename T>
  template <typename Function, typename FunctionDerivative>
  st2tost2<N, std::invoke_result_t<FunctionDerivative, T>>
  stensor<N, T>::computeIsotropicFunctionDerivative(
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
  template <typename ST2toST2Type,
            typename Function,
            typename FunctionDerivative>
  std::enable_if_t<
      (implementsST2toST2Concept<ST2toST2Type>()) &&
          (getSpaceDimension<ST2toST2Type>() == N) &&
          (isAssignableTo<BinaryOperationResult<base_type<T>, T, OpDiv>,
                          numeric_type<ST2toST2Type>>()),
      void>
  stensor<N, T>::computeIsotropicFunctionDerivative(
      ST2toST2Type& d,
      const Function& f,
      const FunctionDerivative& df,
      const tvector<3u, T>& vp,
      const rotation_matrix<T>& m,
      const T eps) {
    tfel::math::internals::StensorComputeIsotropicFunctionDerivative<N>::exe2(
        d, f, df, vp, m, eps);
  }

  template <unsigned short N, typename T>
  template <typename T1, typename T2>
  st2tost2<N, T2> stensor<N, T>::computeIsotropicFunctionDerivative(
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
  template <typename ST2toST2Type, typename T1, typename T2>
  std::enable_if_t<
      (implementsST2toST2Concept<ST2toST2Type>()) &&
          (getSpaceDimension<ST2toST2Type>() == N) &&
          (isAssignableTo<BinaryOperationResult<base_type<T>, T, OpDiv>,
                          numeric_type<ST2toST2Type>>()),
      void>
  stensor<N, T>::computeIsotropicFunctionDerivative(ST2toST2Type& d,
                                                    const tvector<3u, T1>& f,
                                                    const tvector<3u, T2>& df,
                                                    const tvector<3u, T>& vp,
                                                    const rotation_matrix<T>& m,
                                                    const T eps) {
    tfel::math::internals::StensorComputeIsotropicFunctionDerivative<N>::exe(
        d, f, df, vp, m, eps);
  }

  template <unsigned short N, typename T>
  template <typename stensor_common::EigenSolver es, typename Function>
  stensor<N, std::invoke_result_t<Function, T>>
  stensor<N, T>::computeIsotropicFunction(const Function& f,
                                          const bool b) const {
    using base = base_type<T>;
    tvector<3u, T> vp;
    tmatrix<3u, 3u, base> m;
    this->template computeEigenVectors<es>(vp, m, b);
    return stensor<N, T>::buildFromEigenValuesAndVectors(f(vp(0)), f(vp(1)),
                                                         f(vp(2)), m);
  }

  template <unsigned short N, typename T>
  template <typename stensor_common::EigenSolver es,
            typename Function,
            typename FunctionDerivative>
  st2tost2<N, std::invoke_result_t<FunctionDerivative, T>>
  stensor<N, T>::computeIsotropicFunctionDerivative(
      const Function& f,
      const FunctionDerivative& df,
      const T eps,
      const bool b) const {
    using base = base_type<T>;
    st2tost2<N, T> r;
    tvector<3u, T> vp;
    tmatrix<3u, 3u, base> m;
    this->template computeEigenVectors<es>(vp, m, b);
    stensor<N, T>::computeIsotropicFunctionDerivative(r, f, df, vp, m, eps);
    return r;
  }

  template <unsigned short N, typename T>
  template <typename stensor_common::EigenSolver es,
            typename Function,
            typename FunctionDerivative>
  std::pair<stensor<N, std::invoke_result_t<Function, T>>,
            st2tost2<N, std::invoke_result_t<FunctionDerivative, T>>>
  stensor<N, T>::computeIsotropicFunctionAndDerivative(
      const Function& f,
      const FunctionDerivative& df,
      const T eps,
      const bool b) const {
    using base = base_type<T>;
    std::pair<stensor<N, T>, st2tost2<N, T>> r;
    tvector<3u, T> vp;
    tmatrix<3u, 3u, base> m;
    this->template computeEigenVectors<es>(vp, m, b);
    const auto fv = map(f, vp);
    const auto dfv = map(df, vp);
    r.first =
        stensor<N, T>::buildFromEigenValuesAndVectors(fv(0), fv(1), fv(2), m);
    stensor<N, T>::computeIsotropicFunctionDerivative(r.second, fv, dfv, vp, m,
                                                      eps);
    return r;
  }  // end of stensor<N,T>::computeIsotropicFunctionAndDerivative

  // ChangeBasis
  template <unsigned short N, typename T>
  void stensor<N, T>::changeBasis(const rotation_matrix<T>& m) {
    return tfel::math::internals::StensorChangeBasis<N>::exe(this->v, m);
  }

  // Return Id
  template <unsigned short N, typename T>
  constexpr stensor<N, base_type<T>> stensor<N, T>::Id() {
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

  template <unsigned short N, typename T>
  template <typename InputIterator>
  void stensor<N, T>::copy(const InputIterator src) {
    tfel::fsalgo::copy<StensorDimeToSize<N>::value>::exe(src, *this);
  }

  template <unsigned short N, typename T, typename OutputIterator>
  TFEL_MATH_INLINE2 std::enable_if_t<isScalar<T>(), void> exportToBaseTypeArray(
      const stensor<N, T>& s, OutputIterator p) {
    tfel::fsalgo::transform<StensorDimeToSize<N>::value>::exe(
        s.cbegin(), p, [](const auto& v) { return base_type_cast(v); });
  }

  template <typename T>
  T tresca(const stensor<1u, T>& s, const bool) {
    const auto sd1 = tfel::math::abs(s[0] - s[1]);
    const auto sd2 = tfel::math::abs(s[0] - s[2]);
    const auto sd3 = tfel::math::abs(s[2] - s[1]);
    const auto tmp = sd1 > sd2 ? sd1 : sd2;
    const auto tmp2 = sd3 > tmp ? sd3 : tmp;
    return tmp2;
  }  // end of tresca

  template <unsigned short N, typename T>
  T tresca(const stensor<N, T>& s, const bool b) {
    T s1, s2, s3;
    s.computeEigenValues(s1, s2, s3, b);
    const auto sd1 = tfel::math::abs(s1 - s2);
    const auto sd2 = tfel::math::abs(s1 - s3);
    const auto sd3 = tfel::math::abs(s3 - s2);
    const auto tmp = sd1 > sd2 ? sd1 : sd2;
    const auto tmp2 = sd3 > tmp ? sd3 : tmp;
    return tmp2;
  }  // end of tresca

  template <typename StensorType>
  std::enable_if_t<
      implementsStensorConcept<StensorType>(),
      stensor<getSpaceDimension<StensorType>(), numeric_type<StensorType>>>
  square_root(const StensorType& s) {
    using tfel::fsalgo::copy;
    typedef numeric_type<StensorType> T;
    typedef base_type<T> real;
    stensor<getSpaceDimension<StensorType>(), T> tmp(s);
    stensor<getSpaceDimension<StensorType>(), T> r(T(0));
    tmatrix<3u, 3u, real> vec;
    tvector<3u, T> vp;
    tmp.computeEigenVectors(vp, vec);
    r[0] = std::sqrt(vp[0]);
    r[1] = std::sqrt(vp[1]);
    r[2] = std::sqrt(vp[2]);
    r.changeBasis(transpose(vec));
    return r;
  }  // end of square_root

  template <typename StensorType>
  std::enable_if_t<
      implementsStensorConcept<StensorType>(),
      typename ComputeUnaryResult<numeric_type<StensorType>, Power<3>>::Result>
  det(const StensorType& s) {
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

  template <typename StensorType>
  std::enable_if_t<
      implementsStensorConcept<StensorType>(),
      stensor<getSpaceDimension<StensorType>(),
              BinaryOperationResult<base_type<numeric_type<StensorType>>,
                                    numeric_type<StensorType>,
                                    OpDiv>>>
  invert(const StensorType& s) {
    constexpr auto N = getSpaceDimension<StensorType>();
    static_assert((N == 1) || (N == 2) || (N == 3), "invalid space dimension");
    if constexpr (N == 1) {
      return {1 / s(0), 1 / s(1), 1 / s(2)};
    } else if constexpr (N == 2) {
      const auto idet = 1 / det(s);
      return {s(1) * s(2) * idet, s(0) * s(2) * idet, 1 / s(2),
              -s(2) * s(3) * idet};
    } else {
      using real = base_type<numeric_type<StensorType>>;
      constexpr auto two = real(2);
      constexpr auto one_half = real(1) / two;
      constexpr auto icste = Cste<real>::isqrt2;
      const auto iJ = 1 / det(s);
      return {(s(1) * s(2) - s(5) * s(5) * one_half) * iJ,
              (s(0) * s(2) - s(4) * s(4) * one_half) * iJ,
              (s(0) * s(1) - s(3) * s(3) * one_half) * iJ,
              (icste * s(4) * s(5) - s(2) * s(3)) * iJ,
              (icste * s(3) * s(5) - s(1) * s(4)) * iJ,
              (icste * s(3) * s(4) - s(0) * s(5)) * iJ};
    }
  }  // end of invert

  template <unsigned short N, typename T>
  template <typename MatrixType>
  std::enable_if_t<isAssignableTo<numeric_type<MatrixType>, T>(), stensor<N, T>>
  stensor<N, T>::buildFromMatrix(const MatrixType& m) {
    if constexpr (N == 1) {
      return {m(0, 0), m(1, 1), m(2, 2)};
    } else if constexpr (N == 2) {
      constexpr auto cste = Cste<T>::isqrt2;
      return {m(0, 0), m(1, 1), m(2, 2), (m(0, 1) + m(1, 0)) * cste};
    } else {
      constexpr auto cste = Cste<T>::isqrt2;
      return {m(0, 0),
              m(1, 1),
              m(2, 2),
              (m(0, 1) + m(1, 0)) * cste,
              (m(0, 2) + m(2, 0)) * cste,
              (m(2, 1) + m(1, 2)) * cste};
    }
  }  // end of buildFromMatrix

  template <unsigned short N, typename T>
  template <typename VectorType>
  std::enable_if_t<
      isAssignableTo<typename ComputeUnaryResult<numeric_type<VectorType>,
                                                 Power<2>>::Result,
                     T>(),
      stensor<N, T>>
  stensor<N, T>::buildFromVectorDiadicProduct(const VectorType& v) {
    if constexpr (N == 1) {
      return {v(0) * v(0), v(1) * v(1), v(2) * v(2)};
    } else if constexpr (N == 2) {
      constexpr auto cste = Cste<T>::sqrt2;
      return {v(0) * v(0), v(1) * v(1), v(2) * v(2), v(0) * v(1) * cste};
    } else {
      constexpr auto cste = Cste<T>::sqrt2;
      return {v(0) * v(0),        v(1) * v(1),        v(2) * v(2),
              v(0) * v(1) * cste, v(0) * v(2) * cste, v(1) * v(2) * cste};
    }
  }  // end of buildFromVectorDiadicProduct

  template <unsigned short N, typename T>
  template <typename VectorType, typename VectorType2>
  std::enable_if_t<
      isAssignableTo<BinaryOperationResult<numeric_type<VectorType>,
                                           numeric_type<VectorType2>,
                                           OpMult>,
                     T>(),
      stensor<N, T>>
  stensor<N, T>::buildFromVectorsSymmetricDiadicProduct(const VectorType& v1,
                                                        const VectorType2& v2) {
    if constexpr (N == 1) {
      return {2 * v1(0) * v2(0), 2 * v1(1) * v2(1), 2 * v1(2) * v2(2)};
    } else if constexpr (N == 2) {
      constexpr auto cste = Cste<T>::sqrt2;
      return {2 * v1(0) * v2(0), 2 * v1(1) * v2(1), 2 * v1(2) * v2(2),
              cste * (v1(0) * v2(1) + v2(0) * v1(1))};
    } else {
      constexpr auto cste = Cste<T>::sqrt2;
      return {2 * v1(0) * v2(0),
              2 * v1(1) * v2(1),
              2 * v1(2) * v2(2),
              (v1(0) * v2(1) + v2(0) * v1(1)) * cste,
              (v1(0) * v2(2) + v2(0) * v1(2)) * cste,
              (v1(1) * v2(2) + v2(1) * v1(2)) * cste};
    }
  }  // end of buildFromVectorsSymmetricDiadicProduct

  template <unsigned short N, typename T>
  stensor<N, T> stensor<N, T>::buildFromEigenValuesAndVectors(
      const T& v1,
      const T& v2,
      const T& v3,
      const tmatrix<3u, 3u, base_type<T>>& m) {
    if constexpr (N == 1) {
      return {v1, v2, v3};
    } else if constexpr (N == 2) {
      constexpr auto cste = Cste<T>::sqrt2;
      return {v1 * m(0, 0) * m(0, 0) + v2 * m(0, 1) * m(0, 1),
              v1 * m(1, 0) * m(1, 0) + v2 * m(1, 1) * m(1, 1), v3,
              (v1 * m(0, 0) * m(1, 0) + v2 * m(0, 1) * m(1, 1)) * cste};
    } else {
      constexpr auto cste = Cste<T>::sqrt2;
      return {v1 * m(0, 0) * m(0, 0) + v2 * m(0, 1) * m(0, 1) +
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
  stensor<N, T> stensor<N, T>::buildFromEigenValuesAndVectors(
      const tvector<3u, T>& vp, const tmatrix<3u, 3u, base_type<T>>& m) {
    return stensor::buildFromEigenValuesAndVectors(vp(0), vp(1), vp(2), m);
  }  // end of buildFromEigenValuesAndVectors

  template <unsigned short N, typename T>
  stensor<N, T> stensor<N, T>::buildLogarithmFromEigenValuesAndVectors(
      const T& v1,
      const T& v2,
      const T& v3,
      const tmatrix<3u, 3u, base_type<T>>& m) {
    return stensor::buildFromEigenValuesAndVectors(std::log(v1), std::log(v2),
                                                   std::log(v3), m);
  }  // end of buildLogarithmFromEigenValuesAndVectors

  template <unsigned short N, typename T>
  stensor<N, T> stensor<N, T>::buildLogarithmFromEigenValuesAndVectors(
      const tvector<3u, T>& vp, const tmatrix<3u, 3u, base_type<T>>& m) {
    return stensor::buildFromEigenValuesAndVectors(
        std::log(vp(0)), std::log(vp(1)), std::log(vp(2)), m);
  }  // end of buildLogarithmFromEigenValuesAndVectors

  template <unsigned short N, typename T>
  stensor<N, T> stensor<N, T>::buildPositivePartFromEigenValuesAndVectors(
      const T& v1,
      const T& v2,
      const T& v3,
      const tmatrix<3u, 3u, base_type<T>>& m) {
    return stensor::buildFromEigenValuesAndVectors(
        std::max(T(0), v1), std::max(T(0), v2), std::max(T(0), v3), m);
  }  // end of buildPositivePartFromEigenValuesAndVectors

  template <unsigned short N, typename T>
  stensor<N, T> stensor<N, T>::buildPositivePartFromEigenValuesAndVectors(
      const tvector<3u, T>& vp, const tmatrix<3u, 3u, base_type<T>>& m) {
    return stensor::buildFromEigenValuesAndVectors(
        std::max(T(0), vp(0)), std::max(T(0), vp(1)), std::max(T(0), vp(2)), m);
  }  // end of buildPositivePartFromEigenValuesAndVectors

  template <unsigned short N, typename T>
  stensor<N, T> stensor<N, T>::buildNegativePartFromEigenValuesAndVectors(
      const T& v1,
      const T& v2,
      const T& v3,
      const tmatrix<3u, 3u, base_type<T>>& m) {
    return stensor::buildFromEigenValuesAndVectors(
        std::min(T(0), v1), std::min(T(0), v2), std::min(T(0), v3), m);
  }  // end of buildNegativePartFromEigenValuesAndVectors

  template <unsigned short N, typename T>
  stensor<N, T> stensor<N, T>::buildNegativePartFromEigenValuesAndVectors(
      const tvector<3u, T>& vp, const tmatrix<3u, 3u, base_type<T>>& m) {
    return stensor::buildFromEigenValuesAndVectors(
        std::min(T(0), vp(0)), std::min(T(0), vp(1)), std::min(T(0), vp(2)), m);
  }  // end of buildNegativePartFromEigenValuesAndVectors

  template <typename StensorType>
  std::enable_if_t<((implementsStensorConcept<StensorType>()) &&
                    (getSpaceDimension<StensorType>() == 1u) &&
                    (tfel::typetraits::IsFundamentalNumericType<
                        numeric_type<StensorType>>::cond)),
                   stensor<1u, numeric_type<StensorType>>>
  logarithm(const StensorType& s, const bool) {
    typedef numeric_type<StensorType> NumType;
    stensor<1u, NumType> l;
    l(0) = std::log(s(0));
    l(1) = std::log(s(1));
    l(2) = std::log(s(2));
    return l;
  }

  template <typename StensorType>
  std::enable_if_t<
      ((implementsStensorConcept<StensorType>()) &&
       ((getSpaceDimension<StensorType>() == 2u) ||
        (getSpaceDimension<StensorType>() == 3u)) &&
       (tfel::typetraits::IsFundamentalNumericType<
           numeric_type<StensorType>>::cond)),
      stensor<getSpaceDimension<StensorType>(), numeric_type<StensorType>>>
  logarithm(const StensorType& s_, const bool b) {
    typedef numeric_type<StensorType> NumType;
    typedef base_type<NumType> base;
    typedef stensor<getSpaceDimension<StensorType>(), NumType> Stensor;
    tvector<3u, NumType> vp;
    tmatrix<3u, 3u, base> m;
    stensor<getSpaceDimension<StensorType>(), NumType> s(s_);
    s.computeEigenVectors(vp, m, b);
    return Stensor::buildFromEigenValuesAndVectors(
        std::log(vp(0)), std::log(vp(1)), std::log(vp(2)), m);
  }

  template <typename StensorType>
  std::enable_if_t<((implementsStensorConcept<StensorType>()) &&
                    (getSpaceDimension<StensorType>() == 1u) &&
                    (tfel::typetraits::IsFundamentalNumericType<
                        numeric_type<StensorType>>::cond)),
                   stensor<1u, numeric_type<StensorType>>>
  absolute_value(const StensorType& s, const bool) {
    return {tfel::math::abs(s(0)), tfel::math::abs(s(1)),
            tfel::math::abs(s(2))};
  }

  template <typename StensorType>
  std::enable_if_t<
      ((implementsStensorConcept<StensorType>()) &&
       ((getSpaceDimension<StensorType>() == 2u) ||
        (getSpaceDimension<StensorType>() == 3u)) &&
       (tfel::typetraits::IsFundamentalNumericType<
           numeric_type<StensorType>>::cond)),
      stensor<getSpaceDimension<StensorType>(), numeric_type<StensorType>>>
  absolute_value(const StensorType& s_, const bool b) {
    typedef numeric_type<StensorType> NumType;
    typedef base_type<NumType> base;
    typedef stensor<getSpaceDimension<StensorType>(), NumType> Stensor;
    tvector<3u, NumType> vp;
    tmatrix<3u, 3u, base> m;
    stensor<getSpaceDimension<StensorType>(), NumType> s(s_);
    s.computeEigenVectors(vp, m, b);
    return Stensor::buildFromEigenValuesAndVectors(tfel::math::abs(vp(0)),
                                                   tfel::math::abs(vp(1)),
                                                   tfel::math::abs(vp(2)), m);
  }

  template <typename StensorType>
  std::enable_if_t<((implementsStensorConcept<StensorType>()) &&
                    (getSpaceDimension<StensorType>() == 1u) &&
                    (tfel::typetraits::IsFundamentalNumericType<
                        numeric_type<StensorType>>::cond)),
                   stensor<1u, numeric_type<StensorType>>>
  positive_part(const StensorType& s, const bool) {
    typedef numeric_type<StensorType> NumType;
    return {std::max(s(0), NumType(0)), std::max(s(1), NumType(0)),
            std::max(s(2), NumType(0))};
  }

  template <typename StensorType>
  std::enable_if_t<
      ((implementsStensorConcept<StensorType>()) &&
       ((getSpaceDimension<StensorType>() == 2u) ||
        (getSpaceDimension<StensorType>() == 3u)) &&
       (tfel::typetraits::IsFundamentalNumericType<
           numeric_type<StensorType>>::cond)),
      stensor<getSpaceDimension<StensorType>(), numeric_type<StensorType>>>
  positive_part(const StensorType& s_, const bool b) {
    typedef numeric_type<StensorType> NumType;
    typedef base_type<NumType> base;
    typedef stensor<getSpaceDimension<StensorType>(), NumType> Stensor;
    tvector<3u, NumType> vp;
    tmatrix<3u, 3u, base> m;
    stensor<getSpaceDimension<StensorType>(), NumType> s(s_);
    s.computeEigenVectors(vp, m, b);
    return Stensor::buildFromEigenValuesAndVectors(
        std::max(vp(0), NumType(0)), std::max(vp(1), NumType(0)),
        std::max(vp(2), NumType(0)), m);
  }

  template <typename StensorType>
  std::enable_if_t<((implementsStensorConcept<StensorType>()) &&
                    (getSpaceDimension<StensorType>() == 1u) &&
                    (tfel::typetraits::IsFundamentalNumericType<
                        numeric_type<StensorType>>::cond)),
                   stensor<1u, numeric_type<StensorType>>>
  negative_part(const StensorType& s, const bool) {
    typedef numeric_type<StensorType> NumType;
    return {std::min(s(0), NumType(0)), std::min(s(1), NumType(0)),
            std::min(s(2), NumType(0))};
  }

  template <typename StensorType>
  std::enable_if_t<
      ((implementsStensorConcept<StensorType>()) &&
       ((getSpaceDimension<StensorType>() == 2u) ||
        (getSpaceDimension<StensorType>() == 3u)) &&
       (tfel::typetraits::IsFundamentalNumericType<
           numeric_type<StensorType>>::cond)),
      stensor<getSpaceDimension<StensorType>(), numeric_type<StensorType>>>
  negative_part(const StensorType& s_, const bool b) {
    typedef numeric_type<StensorType> NumType;
    typedef base_type<NumType> base;
    typedef stensor<getSpaceDimension<StensorType>(), NumType> Stensor;
    tvector<3u, NumType> vp;
    tmatrix<3u, 3u, base> m;
    stensor<getSpaceDimension<StensorType>(), NumType> s(s_);
    s.computeEigenVectors(vp, m, b);
    return Stensor::buildFromEigenValuesAndVectors(
        std::min(vp(0), NumType(0)), std::min(vp(1), NumType(0)),
        std::min(vp(2), NumType(0)), m);
  }

  template <typename StensorType>
  std::enable_if_t<implementsStensorConcept<StensorType>() &&
                       getSpaceDimension<StensorType>() == 1u,
                   stensor<1u,
                           BinaryOperationResult<numeric_type<StensorType>,
                                                 numeric_type<StensorType>,
                                                 OpMult>>>
  square(const StensorType& s) {
    using T = numeric_type<StensorType>;
    using T2 = BinaryOperationResult<T, T, OpMult>;
    return stensor<1u, T2>{s(0) * s(0), s(1) * s(1), s(2) * s(2)};
  }

  template <typename StensorType>
  std::enable_if_t<implementsStensorConcept<StensorType>() &&
                       getSpaceDimension<StensorType>() == 2u,
                   stensor<2u,
                           BinaryOperationResult<numeric_type<StensorType>,
                                                 numeric_type<StensorType>,
                                                 OpMult>>>
  square(const StensorType& s) {
    using T = numeric_type<StensorType>;
    using T2 = BinaryOperationResult<T, T, OpMult>;
    using base = base_type<T>;
    constexpr const base one_half = 1 / base(2);
    return stensor<2u, T2>{(s(3) * s(3) + 2 * s(0) * s(0)) * one_half,
                           (s(3) * s(3) + 2 * s(1) * s(1)) * one_half,
                           s(2) * s(2), (s(1) + s(0)) * s(3)};
  }

  template <typename StensorType>
  std::enable_if_t<implementsStensorConcept<StensorType>() &&
                       getSpaceDimension<StensorType>() == 3u,
                   stensor<3u,
                           BinaryOperationResult<numeric_type<StensorType>,
                                                 numeric_type<StensorType>,
                                                 OpMult>>>
  square(const StensorType& s) {
    using T = numeric_type<StensorType>;
    using T2 = BinaryOperationResult<T, T, OpMult>;
    using base = base_type<T>;
    typedef numeric_type<StensorType> T;
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

  template <typename StensorType>
  std::enable_if_t<
      implementsStensorConcept<StensorType>(),
      stensor<getSpaceDimension<StensorType>(), numeric_type<StensorType>>>
  change_basis(const StensorType& s,
               const tmatrix<3u, 3u, base_type<numeric_type<StensorType>>>& r) {
    stensor<getSpaceDimension<StensorType>(), numeric_type<StensorType>> ns{s};
    ns.changeBasis(r);
    return ns;
  }

  template <typename stensor_common::EigenSolver es,
            typename Function,
            typename StensorType>
  std::enable_if_t<
      implementsStensorConcept<StensorType>(),
      stensor<getSpaceDimension<StensorType>(), numeric_type<StensorType>>>
  computeIsotropicFunction(const Function& f,
                           const StensorType& s,
                           const bool b) {
    return s.template computeIsotropicFunction<es>(f, b);
  }

  template <typename stensor_common::EigenSolver es,
            typename Function,
            typename FunctionDerivative,
            typename StensorType>
  std::enable_if_t<
      implementsStensorConcept<StensorType>(),
      st2tost2<getSpaceDimension<StensorType>(), numeric_type<StensorType>>>
  computeIsotropicFunctionDerivative(const Function& f,
                                     const FunctionDerivative& df,
                                     const StensorType& s,
                                     const numeric_type<StensorType> eps,
                                     const bool b) {
    return s.template computeIsotropicFunctionDerivative<es>(f, df, eps, b);
  }

  template <typename stensor_common::EigenSolver es,
            typename Function,
            typename FunctionDerivative,
            typename StensorType>
  std::enable_if_t<implementsStensorConcept<StensorType>(),
                   std::pair<stensor<getSpaceDimension<StensorType>(),
                                     numeric_type<StensorType>>,
                             st2tost2<getSpaceDimension<StensorType>(),
                                      numeric_type<StensorType>>>>
  computeIsotropicFunctionDerivative(const Function& f,
                                     const FunctionDerivative& df,
                                     const StensorType& s,
                                     const numeric_type<StensorType> eps,
                                     const bool b) {
    return s.template computeIsotropicFunctionDerivative<es>(f, df, eps, b);
  }

  template <typename T, typename T2>
  std::enable_if_t<
      ((implementsStensorConcept<T>()) && (getSpaceDimension<T>() == 1u) &&
       (implementsStensorConcept<T2>()) && (getSpaceDimension<T2>() == 1u) &&
       (tfel::typetraits::IsFundamentalNumericType<numeric_type<T2>>::cond)),
      stensor<1u, numeric_type<T>>>
  convertCorotationnalCauchyStressToSecondPiolaKirchhoffStress(const T& s,
                                                               const T2& U) {
    const auto J = det(U);
    return {J * s[0] / (U[0] * U[0]), J * s[1] / (U[1] * U[1]),
            J * s[2] / (U[2] * U[2])};
  }

  template <typename T, typename T2>
  std::enable_if_t<
      ((implementsStensorConcept<T>()) && (getSpaceDimension<T>() == 2u) &&
       (implementsStensorConcept<T2>()) && (getSpaceDimension<T2>() == 2u) &&
       (tfel::typetraits::IsFundamentalNumericType<numeric_type<T2>>::cond)),
      stensor<2u, numeric_type<T>>>
  convertCorotationnalCauchyStressToSecondPiolaKirchhoffStress(const T& s,
                                                               const T2& U) {
    const auto J = det(U);
    const auto iU = invert(U);
    return {J *
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
  }

  template <typename T, typename T2>
  std::enable_if_t<
      ((implementsStensorConcept<T>()) && (getSpaceDimension<T>() == 3u) &&
       (implementsStensorConcept<T2>()) && (getSpaceDimension<T2>() == 3u) &&
       (tfel::typetraits::IsFundamentalNumericType<numeric_type<T2>>::cond)),
      stensor<3u, numeric_type<T>>>
  convertCorotationnalCauchyStressToSecondPiolaKirchhoffStress(const T& s,
                                                               const T2& U) {
    using real = base_type<numeric_type<T>>;
    constexpr real cste = Cste<real>::sqrt2;
    const auto J = det(U);
    const auto iU = invert(U);
    return {J *
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
  template <typename T, typename T2>
  std::enable_if_t<
      ((implementsStensorConcept<T>()) && (getSpaceDimension<T>() == 1u) &&
       (implementsStensorConcept<T2>()) && (getSpaceDimension<T2>() == 1u) &&
       (tfel::typetraits::IsFundamentalNumericType<numeric_type<T2>>::cond)),
      stensor<1u, numeric_type<T>>>
  convertSecondPiolaKirchhoffStressToCorotationnalCauchyStress(const T& S,
                                                               const T2& U) {
    const auto iJ = 1 / det(U);
    return {iJ * U[0] * S[0] * U[0], iJ * U[1] * S[1] * U[1],
            iJ * U[2] * S[2] * U[2]};
  }
  template <typename T, typename T2>
  std::enable_if_t<
      ((implementsStensorConcept<T>()) && (getSpaceDimension<T>() == 2u) &&
       (implementsStensorConcept<T2>()) && (getSpaceDimension<T2>() == 2u) &&
       (tfel::typetraits::IsFundamentalNumericType<numeric_type<T2>>::cond)),
      stensor<2u, numeric_type<T>>>
  convertSecondPiolaKirchhoffStressToCorotationnalCauchyStress(const T& S,
                                                               const T2& U) {
    const auto iJ = 1 / det(U);
    return {
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
  }
  template <typename T, typename T2>
  std::enable_if_t<
      ((implementsStensorConcept<T>()) && (getSpaceDimension<T>() == 3u) &&
       (implementsStensorConcept<T2>()) && (getSpaceDimension<T2>() == 3u) &&
       (tfel::typetraits::IsFundamentalNumericType<numeric_type<T2>>::cond)),
      stensor<3u, numeric_type<T>>>
  convertSecondPiolaKirchhoffStressToCorotationnalCauchyStress(const T& S,
                                                               const T2& U) {
    using real = base_type<numeric_type<T>>;
    constexpr auto cste = Cste<real>::sqrt2;
    const auto iJ = 1 / det(U);
    return {
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

  template <typename StensorType1, typename StensorType2>
  std::enable_if_t<((implementsStensorConcept<StensorType1>()) &&
                    (implementsStensorConcept<StensorType2>()) &&
                    (getSpaceDimension<StensorType1>() ==
                     getSpaceDimension<StensorType2>())),
                   stensor<getSpaceDimension<StensorType1>(),
                           result_type<numeric_type<StensorType1>,
                                       numeric_type<StensorType2>,
                                       OpMult>>>
  symmetric_product(const StensorType1& s1, const StensorType2& s2) {
    constexpr auto N = getSpaceDimension<StensorType1>();
    static_assert((N == 1) || (N == 2) || (N == 3), "invalid space dimension");
    if constexpr (N == 1u) {
      return {2 * s1[0] * s2[0], 2 * s1[1] * s2[1], 2 * s1[2] * s2[2]};
    } else if constexpr (N == 2u) {
      return {2 * s1[0] * s2[0] + s1[3] * s2[3],
              2 * s1[1] * s2[1] + s1[3] * s2[3], 2 * s1[2] * s2[2],
              (s1[1] + s1[0]) * s2[3] + s1[3] * s2[1] + s1[3] * s2[0]};
    } else {
      using res = result_type<numeric_type<StensorType1>,
                              numeric_type<StensorType2>, OpMult>;
      constexpr auto icste = Cste<res>::isqrt2;
      return {s1[4] * s2[4] + s1[3] * s2[3] + 2 * s1[0] * s2[0],
              s1[5] * s2[5] + s1[3] * s2[3] + 2 * s1[1] * s2[1],
              s1[5] * s2[5] + s1[4] * s2[4] + 2 * s1[2] * s2[2],
              icste * s1[4] * s2[5] + icste * s1[5] * s2[4] +
                  (s1[1] + s1[0]) * s2[3] + s1[3] * s2[1] + s1[3] * s2[0],
              icste * s1[3] * s2[5] + (s1[2] + s1[0]) * s2[4] +
                  icste * s1[5] * s2[3] + s1[4] * s2[2] + s1[4] * s2[0],
              (s1[2] + s1[1]) * s2[5] + icste * s1[3] * s2[4] +
                  icste * s1[4] * s2[3] + s1[5] * s2[2] + s1[5] * s2[1]};
    }
  }

  template <typename StensorType>
  std::enable_if_t<
      implementsStensorConcept<StensorType>(),
      stensor<getSpaceDimension<StensorType>(),
              typename ComputeUnaryResult<numeric_type<StensorType>,
                                          Power<2>>::Result>>
  computeDeterminantDerivative(const StensorType& s) {
    stensor<getSpaceDimension<StensorType>(),
            typename ComputeUnaryResult<numeric_type<StensorType>,
                                        Power<2>>::Result>
        dJ;
    computeDeterminantDerivative(dJ, s);
    return dJ;
  }

  template <typename StensorType>
  std::enable_if_t<
      implementsStensorConcept<StensorType>(),
      stensor<getSpaceDimension<StensorType>(),
              typename ComputeUnaryResult<numeric_type<StensorType>,
                                          Power<2>>::Result>>
  computeDeviatorDeterminantDerivative(const StensorType& s) {
    stensor<getSpaceDimension<StensorType>(),
            typename ComputeUnaryResult<numeric_type<StensorType>,
                                        Power<2>>::Result>
        dJ;
    computeDeviatorDeterminantDerivative(dJ, s);
    return dJ;
  }

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_STENSOR_IXX */
