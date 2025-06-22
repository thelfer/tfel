/*!
 * \file   include/TFEL/Math/Stensor/Internals/StensorComputeEigenVectors.hxx
 * \brief
 * \author Thomas Helfer
 * \date   13 Jul 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_STENSORCOMPUTEEIGENVECTORS_HXX
#define LIB_TFEL_MATH_STENSORCOMPUTEEIGENVECTORS_HXX

#include <cmath>
#include <cassert>
#include <algorithm>
#include <type_traits>

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/TypeTraits/IsReal.hxx"
#include "TFEL/TypeTraits/IsFundamentalNumericType.hxx"
#include "TFEL/Math/General/Abs.hxx"
#include "TFEL/Math/General/CubicRoots.hxx"
#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/tmatrix.hxx"
#include "TFEL/Math/stensor.hxx"

namespace tfel::math::internals {

  template <unsigned short N>
  struct StensorComputeEigenVectors;

  template <>
  struct StensorComputeEigenVectors<1u> {
    template <typename T>
    static bool test(const T* const,
                     const tfel::math::tvector<3u, T>&,
                     const tfel::math::rotation_matrix<T>&) {
      return true;
    }

    template <typename VectorType, typename T>
    static bool computeEigenVector(VectorType& v,
                                   const T* const s,
                                   const T vp) {
      static_assert(tfel::typetraits::IsFundamentalNumericType<
                    numeric_type<VectorType>>::cond);
      static_assert(std::is_same_v<numeric_type<VectorType>, T>);
      static_assert(tfel::typetraits::IsFundamentalNumericType<T>::cond);
      static_assert(tfel::typetraits::IsReal<T>::cond);
      constexpr auto zero = T{0};
      constexpr auto one = T{1};
      constexpr const auto e = 10 * std::numeric_limits<T>::min();
      if (tfel::math::abs(s[0] - vp) < e) {
        v = {one, zero, zero};
        return true;
      }
      if (tfel::math::abs(s[1] - vp) < e) {
        v = {zero, one, zero};
        return true;
      }
      if (tfel::math::abs(s[2] - vp) < e) {
        v = {zero, zero, one};
        return true;
      }
      return false;
    }

    template <typename T>
    static bool exe(const T* const s,
                    tfel::math::tvector<3u, T>& vp,
                    tfel::math::rotation_matrix<T>& m,
                    const bool) {
      static_assert(tfel::typetraits::IsFundamentalNumericType<T>::cond);
      static_assert(tfel::typetraits::IsReal<T>::cond);
      vp = {s[0], s[1], s[2]};
      m = tfel::math::rotation_matrix<T>::Id();
      return true;
    }
  };

  template <>
  struct StensorComputeEigenVectors<2u> {
    template <typename T>
    static bool test(const T* const s,
                     const tfel::math::tvector<3u, T>& vp,
                     const tfel::math::rotation_matrix<T>& m) {
      static_assert(tfel::typetraits::IsFundamentalNumericType<T>::cond);
      static_assert(tfel::typetraits::IsReal<T>::cond);
      using tfel::math::constexpr_fct::sqrt;
      constexpr auto icste = Cste<T>::isqrt2;
      // first eigenvalue
      auto y0 = s[0] * m(0, 0) + s[3] * icste * m(1, 0) - vp(0) * m(0, 0);
      auto y1 = s[1] * m(1, 0) + s[3] * icste * m(0, 0) - vp(0) * m(1, 0);
      auto tmp = std::sqrt(y0 * y0 + y1 * y1);
      if (tmp > 100 * std::numeric_limits<T>::epsilon()) {
        return false;
      }
      // second eigenvalue
      y0 = s[0] * m(0, 1) + s[3] * icste * m(1, 1) - vp(1) * m(0, 1);
      y1 = s[1] * m(1, 1) + s[3] * icste * m(0, 1) - vp(1) * m(1, 1);
      tmp = std::sqrt(y0 * y0 + y1 * y1);
      if (tmp > 100 * std::numeric_limits<T>::epsilon()) {
        return false;
      }
      return true;
    }

    template <typename VectorType, typename T>
    static bool computeEigenVector(VectorType& v,
                                   const T* const s,
                                   const T vp) {
      using namespace tfel::typetraits;
      static_assert(IsFundamentalNumericType<numeric_type<VectorType>>::cond);
      static_assert(std::is_same_v<numeric_type<VectorType>, T>);
      static_assert(IsFundamentalNumericType<T>::cond);
      static_assert(IsReal<T>::cond);
      constexpr auto zero = T{0};
      constexpr auto one = T{1};
      if (tfel::math::abs(s[2] - vp) < 10 * std::numeric_limits<T>::min()) {
        v(0) = zero;
        v(1) = zero;
        v(2) = one;
        return true;
      }
      v(2) = zero;
      return computeEigenVector(s[0], s[1], s[3], vp, v(0), v(1));
    }

    template <typename T>
    static bool exe(const T* const s,
                    tfel::math::tvector<3u, T>& vp,
                    tfel::math::rotation_matrix<T>& vec,
                    const bool) {
      static_assert(tfel::typetraits::IsFundamentalNumericType<T>::cond);
      static_assert(tfel::typetraits::IsReal<T>::cond);
      vp(2) = s[2];
      vec = tmatrix<3u, 3u, T>::Id();
      StensorComputeEigenVectors<2u>::computeEigenValues(s[0], s[1], s[3],
                                                         vp(0), vp(1));
      const auto prec =
          10 * std::max(vp(0), vp(1)) * std::numeric_limits<T>::epsilon();
      if (tfel::math::abs(vp(0) - vp(1)) <= prec) {
        return true;
      }
      if (StensorComputeEigenVectors<2u>::computeEigenVector(
              s[0], s[1], s[3], vp(0), vec(0, 0), vec(1, 0)) == false) {
        return false;
      }
      vec(0, 1) = -vec(1, 0);
      vec(1, 1) = vec(0, 0);
#ifdef TFEL_PARANOIC_CHECK
      return tfel::math::internals::StensorComputeEigenVectors<2u>::test(s, vp,
                                                                         vec);
#else
      return true;
#endif
    }

   private:
    template <typename T>
    static void computeEigenValues(
        const T s0, const T s1, const T s3, T& vp1, T& vp2) {
      static_assert(tfel::typetraits::IsFundamentalNumericType<T>::cond);
      static_assert(tfel::typetraits::IsReal<T>::cond);
      constexpr const auto one_half = T{1} / T{2};
      const T tr = one_half * (s0 + s1);
      const T tmp = s0 - s1;
      const T tmp2 = std::sqrt(one_half * (tmp * tmp * one_half + s3 * s3));
      vp1 = tr + tmp2;
      vp2 = tr - tmp2;
    }

    template <typename T>
    static bool computeEigenVector(
        const T s_0, const T s_1, const T s3, const T& vp, T& x, T& y) {
      static_assert(tfel::typetraits::IsFundamentalNumericType<T>::cond);
      static_assert(tfel::typetraits::IsReal<T>::cond);
      constexpr auto zero = T{0};
      constexpr auto one = T{1};
      constexpr auto cste = Cste<T>::sqrt2;
      auto s0 = s_0 - vp;
      auto s1 = s_1 - vp;
      if (tfel::math::abs(s3) <
          std::max(std::min(s_0, s_1) * std::numeric_limits<T>::epsilon(),
                   10 * std::numeric_limits<T>::min())) {
        if (tfel::math::abs(s0) > tfel::math::abs(s1)) {
          x = zero;
          y = one;
          return true;
        } else {
          x = one;
          y = zero;
          return true;
        }
      }
      if (tfel::math::abs(s0) > tfel::math::abs(s1)) {
        if (tfel::math::abs(s0) < 100 * std::numeric_limits<T>::min()) {
          return false;
        }
        y = one;
        x = -s3 / (cste * s0);
      } else {
        if (tfel::math::abs(s1) < 100 * std::numeric_limits<T>::min()) {
          return false;
        }
        x = one;
        y = -s3 / (cste * s1);
      }
      s0 = std::sqrt(x * x + y * y);
      if (s0 < 100 * std::numeric_limits<T>::min()) {
        return false;
      }
      x /= s0;
      y /= s0;
      return true;
    }
  };

  template <>
  struct StensorComputeEigenVectors<3u> {
    template <typename T>
    static bool test(const T* const s,
                     const tfel::math::tvector<3u, T>& vp,
                     const tfel::math::rotation_matrix<T>& m) {
      static_assert(tfel::typetraits::IsFundamentalNumericType<T>::cond);
      static_assert(tfel::typetraits::IsReal<T>::cond);
      constexpr auto icste = Cste<T>::isqrt2;
      // first eigenvalue
      auto y0 = s[0] * m(0, 0) + s[3] * icste * m(1, 0) +
                s[4] * icste * m(2, 0) - vp(0) * m(0, 0);
      auto y1 = s[1] * m(1, 0) + s[3] * icste * m(0, 0) +
                s[5] * icste * m(2, 0) - vp(0) * m(1, 0);
      auto y2 = s[2] * m(2, 0) + s[4] * icste * m(0, 0) +
                s[5] * icste * m(1, 0) - vp(0) * m(2, 0);
      auto tmp = norm(y0, y1, y2);
      if (tmp > 100 * std::numeric_limits<T>::epsilon()) {
        return false;
      }
      // second eigenvalue
      y0 = s[0] * m(0, 1) + s[3] * icste * m(1, 1) + s[4] * icste * m(2, 1) -
           vp(1) * m(0, 1);
      y1 = s[1] * m(1, 1) + s[3] * icste * m(0, 1) + s[5] * icste * m(2, 1) -
           vp(1) * m(1, 1);
      y2 = s[2] * m(2, 1) + s[4] * icste * m(0, 1) + s[5] * icste * m(1, 1) -
           vp(1) * m(2, 1);
      tmp = norm(y0, y1, y2);
      if (tmp > 100 * std::numeric_limits<T>::epsilon()) {
        return false;
      }
      // third eigenvalue
      y0 = s[0] * m(0, 2) + s[3] * icste * m(1, 2) + s[4] * icste * m(2, 2) -
           vp(2) * m(0, 2);
      y1 = s[1] * m(1, 2) + s[3] * icste * m(0, 2) + s[5] * icste * m(2, 2) -
           vp(2) * m(1, 2);
      y2 = s[2] * m(2, 2) + s[4] * icste * m(0, 2) + s[5] * icste * m(1, 2) -
           vp(2) * m(2, 2);
      tmp = norm(y0, y1, y2);
      if (tmp > 1000 * std::numeric_limits<T>::epsilon()) {
        return false;
      }
      return true;
    }

    template <typename VectorType, typename T>
    static bool computeEigenVector(VectorType& v,
                                   const T* const src,
                                   const T vp) {
      using namespace tfel::typetraits;
      static_assert(IsFundamentalNumericType<numeric_type<VectorType>>::cond);
      static_assert(std::is_same_v<numeric_type<VectorType>, T>);
      static_assert(IsFundamentalNumericType<T>::cond);
      static_assert(IsReal<T>::cond);
      return computeEigenVector(src, vp, v(0), v(1), v(2));
    }

    template <typename T>
    static bool exe(const T* const s,
                    tfel::math::tvector<3u, T>& vp,
                    tfel::math::rotation_matrix<T>& vec,
                    const bool b) {
      using namespace std;
      using tfel::math::tvector;
      static_assert(tfel::typetraits::IsFundamentalNumericType<T>::cond);
      static_assert(tfel::typetraits::IsReal<T>::cond);
      constexpr const auto rel_prec = 100 * std::numeric_limits<T>::epsilon();
      StensorComputeEigenValues<3u>::exe(s, vp(0), vp(1), vp(2), b);
      const auto tr = (s[0] + s[1] + s[2]) / 3;
      auto ms = T(0);
      for (unsigned short i = 0; i != 6; ++i) {
        ms = std::max(ms, tfel::math::abs(s[i]));
      }
      const bool bsmall = ((ms < 100 * std::numeric_limits<T>::min()) ||
                           (ms * std::numeric_limits<T>::epsilon() <
                            100 * std::numeric_limits<T>::min()));
      if (bsmall) {
        // all eigenvalues are equal
        vec = tmatrix<3u, 3u, T>::Id();
#ifdef TFEL_PARANOIC_CHECK
        return tfel::math::internals::StensorComputeEigenVectors<3>::test(s, vp,
                                                                          vec);
#else
        return true;
#endif /* LIB_TFEL_STENSORCOMPUTEEIGENVECTORS_HXX */
      }
      const auto ims = T(1) / ms;
      tvector<6u, T> s2(s);
      tvector<3u, T> vp2(vp);
      vp2(0) = (vp(0) - tr) * ims;
      vp2(1) = (vp(1) - tr) * ims;
      vp2(2) = (vp(2) - tr) * ims;
      s2 *= ims;
      s2(0) -= tr * ims;
      s2(1) -= tr * ims;
      s2(2) -= tr * ims;
      const auto prec =
          10 * std::max(rel_prec, 100 * std::numeric_limits<T>::min());
      if (((tfel::math::abs(vp2(0) - vp2(1)) <= prec) &&
           (tfel::math::abs(vp2(0) - vp2(2)) <= prec)) ||
          ((tfel::math::abs(vp2(1) - vp2(0)) <= prec) &&
           (tfel::math::abs(vp2(1) - vp2(2)) <= prec)) ||
          ((tfel::math::abs(vp2(2) - vp2(0)) <= prec) &&
           (tfel::math::abs(vp2(2) - vp2(1)) <= prec))) {
        // all eigenvalues are equal
        vec = tmatrix<3u, 3u, T>::Id();
#ifdef TFEL_PARANOIC_CHECK
        return tfel::math::internals::StensorComputeEigenVectors<3>::test(s, vp,
                                                                          vec);
#else
        return true;
#endif /* LIB_TFEL_STENSORCOMPUTEEIGENVECTORS_HXX */
      }
      if ((tfel::math::abs(vp2(0) - vp2(1)) > prec) &&
          (tfel::math::abs(vp2(0) - vp2(2)) > prec)) {
        // vp0 is single
        if (computeEigenVector(s2.begin(), vp2(0), vec(0, 0), vec(1, 0),
                               vec(2, 0)) == false) {
          return false;
        }
        if (tfel::math::abs(vp2(1) - vp2(2)) > prec) {
          // vp1 is single
          if (computeEigenVector(s2.begin(), vp2(1), vec(0, 1), vec(1, 1),
                                 vec(2, 1)) == false) {
            return false;
          }
          cross_product(vec(0, 2), vec(1, 2), vec(2, 2), vec(0, 0), vec(1, 0),
                        vec(2, 0), vec(0, 1), vec(1, 1), vec(2, 1));
        } else {
          // vp1 and vp2 are equal
          find_perpendicular_vector(vec(0, 1), vec(1, 1), vec(2, 1), vec(0, 0),
                                    vec(1, 0), vec(2, 0));
          cross_product(vec(0, 2), vec(1, 2), vec(2, 2), vec(0, 0), vec(1, 0),
                        vec(2, 0), vec(0, 1), vec(1, 1), vec(2, 1));
        }
#ifdef TFEL_PARANOIC_CHECK
        return tfel::math::internals::StensorComputeEigenVectors<3>::test(s, vp,
                                                                          vec);
#else
        return true;
#endif /* LIB_TFEL_STENSORCOMPUTEEIGENVECTORS_HXX */
      } else if ((tfel::math::abs(vp2(1) - vp2(0)) > prec) &&
                 (tfel::math::abs(vp2(1) - vp2(2)) > prec)) {
        // vp1 is single, vp0 and vp2 are equal
        assert(tfel::math::abs(vp2(0) - vp2(2)) < prec);
        if (computeEigenVector(s2.begin(), vp2(1), vec(0, 1), vec(1, 1),
                               vec(2, 1)) == false) {
          return false;
        }
        find_perpendicular_vector(vec(0, 0), vec(1, 0), vec(2, 0), vec(0, 1),
                                  vec(1, 1), vec(2, 1));
        cross_product(vec(0, 2), vec(1, 2), vec(2, 2), vec(0, 0), vec(1, 0),
                      vec(2, 0), vec(0, 1), vec(1, 1), vec(2, 1));
#ifdef TFEL_PARANOIC_CHECK
        return tfel::math::internals::StensorComputeEigenVectors<3>::test(s, vp,
                                                                          vec);
#else
        return true;
#endif /* LIB_TFEL_STENSORCOMPUTEEIGENVECTORS_HXX */
      }
      assert((tfel::math::abs(vp2(2) - vp2(0)) > prec) &&
             (tfel::math::abs(vp2(2) - vp2(1)) > prec));
      assert(tfel::math::abs(vp2(0) - vp2(1)) < prec);
      if (computeEigenVector(s2.begin(), vp2(2), vec(0, 2), vec(1, 2),
                             vec(2, 2)) == false) {
        return false;
      }
      find_perpendicular_vector(vec(0, 0), vec(1, 0), vec(2, 0), vec(0, 2),
                                vec(1, 2), vec(2, 2));
      cross_product(vec(0, 1), vec(1, 1), vec(2, 1), vec(0, 2), vec(1, 2),
                    vec(2, 2), vec(0, 0), vec(1, 0), vec(2, 0));
#ifdef TFEL_PARANOIC_CHECK
      return tfel::math::internals::StensorComputeEigenVectors<3>::test(s, vp,
                                                                        vec);
#else
      return true;
#endif /* LIB_TFEL_STENSORCOMPUTEEIGENVECTORS_HXX */
    }

   private:
    template <typename T>
    static T conditionning_number(const T a, const T b, const T c) {
      static_assert(tfel::typetraits::IsFundamentalNumericType<T>::cond);
      static_assert(tfel::typetraits::IsReal<T>::cond);
      auto tmp = a + b;
      auto tmp2 = a - b;
      const auto X = tmp / 2;
      const auto Y = std::sqrt(tmp2 * tmp2 + 2 * c * c) / 2;
      tmp = tfel::math::abs(X + Y);
      tmp2 = tfel::math::abs(X - Y);
      if (tmp > tmp2) {
        if (tmp2 < 100 * std::numeric_limits<T>::min()) {
          return std::numeric_limits<T>::max();
        }
        return tmp / tmp2;
      } else {
        if (tmp < 100 * std::numeric_limits<T>::min()) {
          return std::numeric_limits<T>::max();
        }
        return tmp2 / tmp;
      }
    }

    template <typename T>
    static T norm(const T& x0, const T& x1, const T& x2) {
      static_assert(tfel::typetraits::IsFundamentalNumericType<T>::cond);
      static_assert(tfel::typetraits::IsReal<T>::cond);
      return std::sqrt(x0 * x0 + x1 * x1 + x2 * x2);
    }

    template <typename T>
    static T norm2(const T x0, const T x1, const T x2) {
      static_assert(tfel::typetraits::IsFundamentalNumericType<T>::cond);
      static_assert(tfel::typetraits::IsReal<T>::cond);
      return x0 * x0 + x1 * x1 + x2 * x2;
    }

    template <typename T>
    static void cross_product(T& z0,
                              T& z1,
                              T& z2,
                              const T x0,
                              const T x1,
                              const T x2,
                              const T y0,
                              const T y1,
                              const T y2) {
      static_assert(tfel::typetraits::IsFundamentalNumericType<T>::cond);
      static_assert(tfel::typetraits::IsReal<T>::cond);
      z0 = x1 * y2 - x2 * y1;
      z1 = x2 * y0 - x0 * y2;
      z2 = x0 * y1 - x1 * y0;
    }

    template <typename T>
    static void find_perpendicular_vector(
        T& y0, T& y1, T& y2, const T x0, const T x1, const T x2) {
      static_assert(tfel::typetraits::IsFundamentalNumericType<T>::cond);
      static_assert(tfel::typetraits::IsReal<T>::cond);
      constexpr auto zero = T{0};
      constexpr auto one = T{1};
      const auto norm2_x = norm2(x0, x1, x2);
      if (norm2_x < 100 * std::numeric_limits<T>::min()) {
        // x is null
        y0 = one;
        y1 = zero;
        y2 = zero;
        return;
      }
      if (tfel::math::abs(x0) < tfel::math::abs(x1)) {
        if (tfel::math::abs(x0) < tfel::math::abs(x2)) {
          //|x0| is min, (1 0 0) is a good choice
          y0 = one - x0 * x0 / norm2_x;
          y1 = -x0 * x1 / norm2_x;
          y2 = -x0 * x2 / norm2_x;
        } else {
          //|x2| is min, (0 0 1) is a good choice
          y0 = -x2 * x0 / norm2_x;
          y1 = -x2 * x1 / norm2_x;
          y2 = one - x2 * x2 / norm2_x;
        }
      } else if (tfel::math::abs(x1) < tfel::math::abs(x2)) {
        // |x1| is min, (0 0 1) is a good choice
        y0 = -x1 * x0 / norm2_x;
        y1 = one - x1 * x1 / norm2_x;
        y2 = -x1 * x2 / norm2_x;
      } else {
        // |x2| is min, (0 0 1) is a good choice
        y0 = -x2 * x0 / norm2_x;
        y1 = -x2 * x1 / norm2_x;
        y2 = one - x2 * x2 / norm2_x;
      }
      const auto tmp = norm(y0, y1, y2);
      y0 /= tmp;
      y1 /= tmp;
      y2 /= tmp;
    }

    template <typename T>
    static bool computeEigenVector(
        const T* const src, const T vp, T& v0, T& v1, T& v2) {
      using namespace tfel::math;
      static_assert(tfel::typetraits::IsFundamentalNumericType<T>::cond);
      static_assert(tfel::typetraits::IsReal<T>::cond);
      // Assume that vp is a single eigenvalue
      constexpr auto icste = Cste<T>::isqrt2;
      const auto a = src[0] - vp;
      const auto b = src[3] * icste;
      const auto c = src[4] * icste;
      const auto d = src[1] - vp;
      const auto e = src[5] * icste;
      const auto f = src[2] - vp;
      const auto det3 = a * d - b * b;
      const auto det2 = a * f - c * c;
      const auto det1 = d * f - e * e;
      const auto prec =
          std::max(tfel::math::abs(vp) * std::numeric_limits<T>::epsilon(),
                   10 * std::numeric_limits<T>::min());
      if ((tfel::math::abs(det1) < prec) && (tfel::math::abs(det2) < prec) &&
          (tfel::math::abs(det3) < prec)) {
        tvector<3u, T> v;
        tmatrix<3u, 3u, T> m;
        if (!StensorComputeEigenVectors<3u>::exe(src, v, m, true)) {
          v0 = v1 = v2 = T(0);
          return false;
        }
        const auto d0 = tfel::math::abs(v[0] - vp);
        const auto d1 = tfel::math::abs(v[1] - vp);
        const auto d2 = tfel::math::abs(v[2] - vp);
        if ((d0 < d1) && (d0 < d2)) {
          v0 = m(0, 0);
          v1 = m(1, 0);
          v2 = m(2, 0);
          return true;
        } else if ((d1 < d2) && (d1 < d0)) {
          v0 = m(0, 1);
          v1 = m(1, 1);
          v2 = m(2, 1);
          return true;
        }
        v0 = m(0, 2);
        v1 = m(1, 2);
        v2 = m(2, 2);
        return true;
      }
      if ((tfel::math::abs(det3) >= tfel::math::abs(det1)) &&
          (tfel::math::abs(det3) >= tfel::math::abs(det2))) {
        v0 = (b * e - c * d) / det3;
        v1 = (b * c - a * e) / det3;
        v2 = T(1);
      } else if ((tfel::math::abs(det1) >= tfel::math::abs(det2)) &&
                 (tfel::math::abs(det1) >= tfel::math::abs(det3))) {
        v0 = T(1);
        v1 = (c * e - b * f) / det1;
        v2 = (b * e - c * d) / det1;
      } else {
        v0 = (c * e - b * f) / det2;
        v1 = T(1);
        v2 = (b * c - a * e) / det2;
      }
      const auto nr = norm(v0, v1, v2);
      v0 /= nr;
      v1 /= nr;
      v2 /= nr;
      return true;
    }
  };

}  // end of namespace tfel::math::internals

#endif /* LIB_TFEL_MATH_STENSORCOMPUTEEIGENVECTORS_HXX */
