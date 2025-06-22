/*!
 * \file   include/TFEL/Math/Stensor/stensor.ixx
 * \brief  This file implements the methods of the class stensor.
 * \author Helfer Thomas
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

#include "TFEL/TypeTraits/IsSafelyReinterpretCastableTo.hxx"
#include "TFEL/Math/General/Abs.hxx"
#include "TFEL/Math/General/MathConstants.hxx"
#include "TFEL/Math/General/ConstExprMathFunctions.hxx"
#include "TFEL/Math/Vector/VectorUtilities.hxx"

#include "TFEL/Math/Stensor/Internals/StensorComputeEigenValues.hxx"
#include "TFEL/Math/Stensor/Internals/StensorComputeEigenVectors.hxx"
#include "TFEL/Math/Stensor/Internals/StensorImport.hxx"
#include "TFEL/Math/Stensor/Internals/StensorExport.hxx"
#include "TFEL/Math/Stensor/Internals/StensorChangeBasis.hxx"

namespace tfel {

  namespace math {

    namespace internals {
      template <unsigned short N>
      struct StensorComputeEigenVectors_;

      template <unsigned short N>
      struct StensorDeterminant;

      template <>
      struct StensorDeterminant<1u> {
        template <typename T>
        static typename ComputeUnaryResult<T, Power<3>>::Result exe(
            const tfel::math::stensor<1u, T>& s) {
          return s(0) * s(1) * s(2);
        }
      };  // end of struct StensorDeterminant

      template <>
      struct StensorDeterminant<2u> {
        template <typename T>
        static TFEL_MATH_INLINE typename ComputeUnaryResult<T, Power<3>>::Result
        exe(const tfel::math::stensor<2u, T>& s) {
          typedef typename tfel::typetraits::BaseType<T>::type base;
          return s(2) * (s(0) * s(1) - s(3) * s(3) / base(2));
        }
      };  // end of struct StensorDeterminant

      template <>
      struct StensorDeterminant<3u> {
        template <typename T>
        static TFEL_MATH_INLINE typename ComputeUnaryResult<T, Power<3>>::Result
        exe(const tfel::math::stensor<3u, T>& s) {
          constexpr const auto cste = Cste<T>::sqrt2;
          return (T(2) * s(0) * s(1) * s(2) + cste * s(3) * s(4) * s(5) -
                  s(2) * s(3) * s(3) - s(1) * s(4) * s(4) -
                  s(0) * s(5) * s(5)) /
                 real(2);
        }
      };  // end of struct StensorDeterminant

      template <unsigned short N>
      struct StensorInvert;

      template <>
      struct StensorInvert<1u> {
        template <typename T>
        static TFEL_MATH_INLINE
            stensor<1u,
                    typename ComputeBinaryResult<
                        typename tfel::typetraits::BaseType<T>::type,
                        T,
                        OpDiv>::Result>
            exe(const stensor<1u, T>& s) {
          typedef typename tfel::typetraits::BaseType<T>::type base;
          typedef typename ComputeBinaryResult<base, T, OpDiv>::Result T2;
          stensor<1u, T2> inv;
          inv(0) = base(1) / s(0);
          inv(1) = base(1) / s(1);
          inv(2) = base(1) / s(2);
          return inv;
        }
      };

      template <>
      struct StensorInvert<2u> {
        template <typename T>
        static TFEL_MATH_INLINE
            stensor<2u,
                    typename ComputeBinaryResult<
                        typename tfel::typetraits::BaseType<T>::type,
                        T,
                        OpDiv>::Result>
            exe(const stensor<2u, T>& s) {
          typedef typename tfel::typetraits::BaseType<T>::type base;
          typedef typename ComputeBinaryResult<base, T, OpDiv>::Result T2;
          typedef typename ComputeUnaryResult<T, Power<3>>::Result T3;
          stensor<2u, T2> inv;
          const T3 inv_det = base(1) / det(s);
          inv(0) = s(1) * s(2) * inv_det;
          inv(1) = s(0) * s(2) * inv_det;
          inv(2) = base(1) / s(2);
          inv(3) = -s(2) * s(3) * inv_det;
          return inv;
        }
      };

      template <>
      struct StensorInvert<3u> {
        template <typename T>
        static TFEL_MATH_INLINE
            stensor<3u,
                    typename ComputeBinaryResult<
                        typename tfel::typetraits::BaseType<T>::type,
                        T,
                        OpDiv>::Result>
            exe(const stensor<3u, T>& s) {
          typedef typename tfel::typetraits::BaseType<T>::type base;
          typedef typename ComputeBinaryResult<base, T, OpDiv>::Result T2;
          typedef typename ComputeUnaryResult<T, Power<3>>::Result T3;
          TFEL_CONSTEXPR const base two = base(2);
          TFEL_CONSTEXPR const base one_half = base(1) / two;
          constexpr const base icste = Cste<base>::isqrt2;
          stensor<3u, T2> inv;
          const T3 iJ = base(1) / det(s);
          inv(0) = (s(1) * s(2) - s(5) * s(5) * one_half) * iJ;
          inv(1) = (s(0) * s(2) - s(4) * s(4) * one_half) * iJ;
          inv(2) = (s(0) * s(1) - s(3) * s(3) * one_half) * iJ;
          inv(3) = (icste * s(4) * s(5) - s(2) * s(3)) * iJ;
          inv(4) = (icste * s(3) * s(5) - s(1) * s(4)) * iJ;
          inv(5) = (icste * s(3) * s(4) - s(0) * s(5)) * iJ;
          return inv;
        }
      };

      template <unsigned short N>
      struct BuildStensorFromMatrix;

      template <>
      struct BuildStensorFromMatrix<1u> {
        template <typename T, typename MatrixType>
        static TFEL_MATH_INLINE2 TFEL_VISIBILITY_LOCAL typename std::enable_if<
            tfel::typetraits::IsAssignableTo<
                typename MatrixTraits<MatrixType>::NumType,
                T>::cond,
            void>::type
        exe(stensor<1u, T>& s, const MatrixType& m) {
          s[0] = m(0, 0);
          s[1] = m(1, 1);
          s[2] = m(2, 2);
        }
      };

      template <>
      struct BuildStensorFromMatrix<2u> {
        template <typename T, typename MatrixType>
        static TFEL_MATH_INLINE2 TFEL_VISIBILITY_LOCAL typename std::enable_if<
            tfel::typetraits::IsAssignableTo<
                typename MatrixTraits<MatrixType>::NumType,
                T>::cond,
            void>::type
        exe(stensor<2u, T>& s, const MatrixType& m) {
          constexpr const auto cste = Cste<T>::isqrt2 / 2;
          s[0] = m(0, 0);
          s[1] = m(1, 1);
          s[2] = m(2, 2);
          s[3] = (m(0, 1) + m(1, 0)) * cste;
        }
      };

      template <>
      struct BuildStensorFromMatrix<3u> {
        template <typename T, typename MatrixType>
        static TFEL_MATH_INLINE2 TFEL_VISIBILITY_LOCAL typename std::enable_if<
            tfel::typetraits::IsAssignableTo<
                typename MatrixTraits<MatrixType>::NumType,
                T>::cond,
            void>::type
        exe(stensor<3u, T>& s, const MatrixType& m) {
          constexpr const auto cste = Cste<T>::isqrt2 / 2;
          s[0] = m(0, 0);
          s[1] = m(1, 1);
          s[2] = m(2, 2);
          s[3] = (m(0, 1) + m(1, 0)) * cste;
          s[4] = (m(0, 2) + m(2, 0)) * cste;
          s[5] = (m(2, 1) + m(1, 2)) * cste;
        }
      };

      template <unsigned short N>
      struct BuildStensorFromVectorDiadicProduct;

      template <>
      struct BuildStensorFromVectorDiadicProduct<1u> {
        template <typename T, typename VectorType>
        static TFEL_MATH_INLINE2 TFEL_VISIBILITY_LOCAL typename std::enable_if<
            tfel::typetraits::IsAssignableTo<
                typename ComputeUnaryResult<
                    typename VectorTraits<VectorType>::NumType,
                    Power<2>>::Result,
                T>::cond,
            void>::type
        exe(stensor<1u, T>& s, const VectorType& v) {
          s[0] = v(0) * v(0);
          s[1] = v(1) * v(1);
          s[2] = v(2) * v(2);
        }
      };

      template <>
      struct BuildStensorFromVectorDiadicProduct<2u> {
        template <typename T, typename VectorType>
        static TFEL_MATH_INLINE2 TFEL_VISIBILITY_LOCAL typename std::enable_if<
            tfel::typetraits::IsAssignableTo<
                typename ComputeUnaryResult<
                    typename VectorTraits<VectorType>::NumType,
                    Power<2>>::Result,
                T>::cond,
            void>::type
        exe(stensor<2u, T>& s, const VectorType& v) {
          constexpr const auto cste = Cste<T>::sqrt2;
          s[0] = v(0) * v(0);
          s[1] = v(1) * v(1);
          s[2] = v(2) * v(2);
          s[3] = v(0) * v(1) * cste;
        }
      };

      template <>
      struct BuildStensorFromVectorDiadicProduct<3u> {
        template <typename T, typename VectorType>
        static TFEL_MATH_INLINE2 TFEL_VISIBILITY_LOCAL typename std::enable_if<
            tfel::typetraits::IsAssignableTo<
                typename ComputeUnaryResult<
                    typename VectorTraits<VectorType>::NumType,
                    Power<2>>::Result,
                T>::cond,
            void>::type
        exe(stensor<3u, T>& s, const VectorType& v) {
          constexpr const auto cste = Cste<T>::sqrt2;
          s[0] = v(0) * v(0);
          s[1] = v(1) * v(1);
          s[2] = v(2) * v(2);
          s[3] = v(0) * v(1) * cste;
          s[4] = v(0) * v(2) * cste;
          s[5] = v(1) * v(2) * cste;
        }
      };

      template <unsigned short N>
      struct BuildStensorFromVectorsSymmetricDiadicProduct;

      template <>
      struct BuildStensorFromVectorsSymmetricDiadicProduct<1u> {
        template <typename T, typename VectorType, typename VectorType2>
        static TFEL_MATH_INLINE2 TFEL_VISIBILITY_LOCAL typename std::enable_if<
            tfel::typetraits::IsAssignableTo<
                typename ComputeBinaryResult<
                    typename VectorTraits<VectorType>::NumType,
                    typename VectorTraits<VectorType2>::NumType,
                    OpMult>::Result,
                T>::cond,
            void>::type
        exe(stensor<1u, T>& s, const VectorType& v1, const VectorType2& v2) {
          s[0] = 2 * v1(0) * v2(0);
          s[1] = 2 * v1(1) * v2(1);
          s[2] = 2 * v1(2) * v2(2);
        }
      };

      template <>
      struct BuildStensorFromVectorsSymmetricDiadicProduct<2u> {
        template <typename T, typename VectorType, typename VectorType2>
        static TFEL_MATH_INLINE2 TFEL_VISIBILITY_LOCAL typename std::enable_if<
            tfel::typetraits::IsAssignableTo<
                typename ComputeBinaryResult<
                    typename VectorTraits<VectorType>::NumType,
                    typename VectorTraits<VectorType2>::NumType,
                    OpMult>::Result,
                T>::cond,
            void>::type
        exe(stensor<2u, T>& s, const VectorType& v1, const VectorType2& v2) {
          constexpr const auto cste = Cste<T>::sqrt2;
          s[0] = 2 * v1(0) * v2(0);
          s[1] = 2 * v1(1) * v2(1);
          s[2] = 2 * v1(2) * v2(2);
          s[3] = cste * (v1(0) * v2(1) + v2(0) * v1(1));
        }
      };

      template <>
      struct BuildStensorFromVectorsSymmetricDiadicProduct<3u> {
        template <typename T, typename VectorType, typename VectorType2>
        static TFEL_MATH_INLINE2 TFEL_VISIBILITY_LOCAL typename std::enable_if<
            tfel::typetraits::IsAssignableTo<
                typename ComputeBinaryResult<
                    typename VectorTraits<VectorType>::NumType,
                    typename VectorTraits<VectorType2>::NumType,
                    OpMult>::Result,
                T>::cond,
            void>::type
        exe(stensor<3u, T>& s, const VectorType& v1, const VectorType2& v2) {
          constexpr const auto cste = Cste<T>::sqrt2;
          s[0] = 2 * v1(0) * v2(0);
          s[1] = 2 * v1(1) * v2(1);
          s[2] = 2 * v1(2) * v2(2);
          s[3] = (v1(0) * v2(1) + v2(0) * v1(1)) * cste;
          s[4] = (v1(0) * v2(2) + v2(0) * v1(2)) * cste;
          s[5] = (v1(1) * v2(2) + v2(1) * v1(2)) * cste;
        }
      };

      template <unsigned short N>
      struct BuildStensorFromEigenValuesAndVectors;

      template <>
      struct BuildStensorFromEigenValuesAndVectors<1u> {
        template <typename T>
        static TFEL_MATH_INLINE2 TFEL_VISIBILITY_LOCAL void exe(
            stensor<1u, T>& s,
            const T& v1,
            const T& v2,
            const T& v3,
            const tmatrix<3u,
                          3u,
                          typename tfel::typetraits::BaseType<T>::type>&) {
          s[0] = v1;
          s[1] = v2;
          s[2] = v3;
        }
      };

      template <>
      struct BuildStensorFromEigenValuesAndVectors<2u> {
        template <typename T>
        static TFEL_MATH_INLINE2 TFEL_VISIBILITY_LOCAL void exe(
            stensor<2u, T>& s,
            const T& v1,
            const T& v2,
            const T& v3,
            const tmatrix<3u, 3u, typename tfel::typetraits::BaseType<T>::type>&
                m) {
          constexpr const auto cste = Cste<T>::sqrt2;
          s[0] = v1 * m(0, 0) * m(0, 0) + v2 * m(0, 1) * m(0, 1);
          s[1] = v1 * m(1, 0) * m(1, 0) + v2 * m(1, 1) * m(1, 1);
          s[2] = v3;
          s[3] = (v1 * m(0, 0) * m(1, 0) + v2 * m(0, 1) * m(1, 1)) * cste;
        }
      };

      template <>
      struct BuildStensorFromEigenValuesAndVectors<3u> {
        template <typename T>
        static TFEL_MATH_INLINE2 TFEL_VISIBILITY_LOCAL void exe(
            stensor<3u, T>& s,
            const T& v1,
            const T& v2,
            const T& v3,
            const tmatrix<3u, 3u, typename tfel::typetraits::BaseType<T>::type>&
                m) {
          constexpr const auto cste = Cste<T>::sqrt2;
          s[0] = v1 * m(0, 0) * m(0, 0) + v2 * m(0, 1) * m(0, 1) +
                 v3 * m(0, 2) * m(0, 2);
          s[1] = v1 * m(1, 0) * m(1, 0) + v2 * m(1, 1) * m(1, 1) +
                 v3 * m(1, 2) * m(1, 2);
          s[2] = v1 * m(2, 0) * m(2, 0) + v2 * m(2, 1) * m(2, 1) +
                 v3 * m(2, 2) * m(2, 2);
          s[3] = (v1 * m(0, 0) * m(1, 0) + v2 * m(0, 1) * m(1, 1) +
                  v3 * m(0, 2) * m(1, 2)) *
                 cste;
          s[4] = (v1 * m(0, 0) * m(2, 0) + v2 * m(0, 1) * m(2, 1) +
                  v3 * m(0, 2) * m(2, 2)) *
                 cste;
          s[5] = (v1 * m(1, 0) * m(2, 0) + v2 * m(1, 1) * m(2, 1) +
                  v3 * m(1, 2) * m(2, 2)) *
                 cste;
        }
      };

      template <unsigned short N>
      struct StensorComputeEigenValuesDerivatives;

      template <>
      struct StensorComputeEigenValuesDerivatives<1u> {
        template <typename StensorType, typename BaseType>
        static TFEL_MATH_INLINE2 typename std::enable_if<
            (tfel::meta::Implements<StensorType, StensorConcept>::cond) &&
                (StensorTraits<StensorType>::dime == 1u) &&
                (tfel::typetraits::IsAssignableTo<
                    BaseType,
                    typename StensorTraits<StensorType>::NumType>::cond),
            void>::type
        exe(StensorType& n0,
            StensorType& n1,
            StensorType& n2,
            const tfel::math::tmatrix<3u, 3u, BaseType>&) {
          n0(0) = BaseType(1);
          n0(1) = BaseType(0);
          n0(2) = BaseType(0);
          n1(0) = BaseType(0);
          n1(1) = BaseType(1);
          n1(2) = BaseType(0);
          n2(0) = BaseType(0);
          n2(1) = BaseType(0);
          n2(2) = BaseType(1);
        }
      };

      template <>
      struct StensorComputeEigenValuesDerivatives<2u> {
        template <typename StensorType, typename BaseType>
        static TFEL_MATH_INLINE2 typename std::enable_if<
            (tfel::meta::Implements<StensorType, StensorConcept>::cond) &&
                (StensorTraits<StensorType>::dime == 2u) &&
                (tfel::typetraits::IsAssignableTo<
                    BaseType,
                    typename StensorTraits<StensorType>::NumType>::cond),
            void>::type
        exe(StensorType& n0,
            StensorType& n1,
            StensorType& n2,
            const tfel::math::tmatrix<3u, 3u, BaseType>& m) {
          using namespace tfel::math;
          const tvector<3u, BaseType> v0 = m.template column_view<0u>();
          const tvector<3u, BaseType> v1 = m.template column_view<1u>();
          n0 = StensorType::buildFromVectorDiadicProduct(v0);
          n1 = StensorType::buildFromVectorDiadicProduct(v1);
          n2(0) = BaseType(0);
          n2(1) = BaseType(0);
          n2(2) = BaseType(1);
          n2(3) = BaseType(0);
        }
      };

      template <>
      struct StensorComputeEigenValuesDerivatives<3u> {
        template <typename StensorType, typename BaseType>
        static TFEL_MATH_INLINE2 typename std::enable_if<
            (tfel::meta::Implements<StensorType, StensorConcept>::cond) &&
                (StensorTraits<StensorType>::dime == 3u) &&
                (tfel::typetraits::IsAssignableTo<
                    BaseType,
                    typename StensorTraits<StensorType>::NumType>::cond),
            void>::type
        exe(StensorType& n0,
            StensorType& n1,
            StensorType& n2,
            const tfel::math::tmatrix<3u, 3u, BaseType>& m) {
          using namespace tfel::math;
          const tvector<3u, BaseType> v0 = m.template column_view<0u>();
          const tvector<3u, BaseType> v1 = m.template column_view<1u>();
          const tvector<3u, BaseType> v2 = m.template column_view<2u>();
          n0 = StensorType::buildFromVectorDiadicProduct(v0);
          n1 = StensorType::buildFromVectorDiadicProduct(v1);
          n2 = StensorType::buildFromVectorDiadicProduct(v2);
        }
      };

      template <unsigned short N>
      struct StensorComputeEigenTensorsDerivatives;

      struct StensorComputeEigenTensorsDerivativesBase {
        template <typename NumType>
        TFEL_MATH_INLINE static typename ComputeBinaryResult<
            typename tfel::typetraits::BaseType<NumType>::type,
            NumType,
            OpDiv>::Result
        regularized_inverse(const NumType x, const NumType eps) {
          using namespace std;
          if (std::abs(x) < 100 * numeric_limits<NumType>::min()) {
            return NumType(0);
          }
          return regularization_function(x / eps) / x;
        }  // end of regularization
       protected:
        template <typename NumType>
        TFEL_MATH_INLINE static NumType regularization_function(
            const NumType x) {
          using namespace std;
          if (std::abs(x) > 1) {
            return NumType(1);
          }
          return x * x * (4 - x * x) / 3;
        }
      };

      template <>
      struct StensorComputeEigenTensorsDerivatives<1u> {
        template <typename ST2toST2Type, typename NumType>
        static TFEL_MATH_INLINE2 typename std::enable_if<
            (tfel::meta::Implements<ST2toST2Type, ST2toST2Concept>::cond) &&
                (ST2toST2Traits<ST2toST2Type>::dime == 1u) &&
                (tfel::typetraits::IsAssignableTo<
                    typename ComputeBinaryResult<
                        typename tfel::typetraits::BaseType<NumType>::type,
                        NumType,
                        OpDiv>::Result,
                    typename ST2toST2Traits<ST2toST2Type>::NumType>::cond),
            void>::type
        exe(ST2toST2Type& dn0_ds,
            ST2toST2Type& dn1_ds,
            ST2toST2Type& dn2_ds,
            const tfel::math::tvector<3u, NumType>&,
            const tfel::math::tmatrix<
                3u,
                3u,
                typename tfel::typetraits::BaseType<NumType>::type>&,
            const NumType) {
          using namespace tfel::math;
          using namespace tfel::typetraits;
          typedef
              typename ComputeBinaryResult<typename BaseType<NumType>::type,
                                           NumType, OpDiv>::Result InvNumType;
          dn0_ds = st2tost2<1u, InvNumType>(InvNumType(0));
          dn1_ds = dn0_ds;
          dn2_ds = dn0_ds;
        }  // end of exe
      };

      template <>
      struct StensorComputeEigenTensorsDerivatives<2u>
          : public StensorComputeEigenTensorsDerivativesBase {
        template <typename ST2toST2Type, typename NumType>
        static TFEL_MATH_INLINE2 typename std::enable_if<
            (tfel::meta::Implements<ST2toST2Type, ST2toST2Concept>::cond) &&
                (ST2toST2Traits<ST2toST2Type>::dime == 2u) &&
                (tfel::typetraits::IsAssignableTo<
                    typename ComputeBinaryResult<
                        typename tfel::typetraits::BaseType<NumType>::type,
                        NumType,
                        OpDiv>::Result,
                    typename ST2toST2Traits<ST2toST2Type>::NumType>::cond),
            void>::type
        exe(ST2toST2Type& dn0_ds,
            ST2toST2Type& dn1_ds,
            ST2toST2Type& dn2_ds,
            const tfel::math::tvector<3u, NumType>& vp,
            const tfel::math::tmatrix<
                3u,
                3u,
                typename tfel::typetraits::BaseType<NumType>::type>& m,
            const NumType eps) {
          typedef typename tfel::typetraits::BaseType<NumType>::type base;
          typedef typename ComputeBinaryResult<base, NumType, OpDiv>::Result
              InvNumType;
          constexpr const auto icste = Cste<base>::isqrt2;
          const tvector<3u, base> v0 = m.template column_view<0u>();
          const tvector<3u, base> v1 = m.template column_view<1u>();
          const stensor<2u, base> n01 =
              stensor<2u, base>::buildFromVectorsSymmetricDiadicProduct(v0,
                                                                        v1) *
              icste;
          dn0_ds = regularized_inverse(vp(0) - vp(1), eps) * (n01 ^ n01);
          dn1_ds = -dn0_ds;
          dn2_ds = st2tost2<2u, InvNumType>(InvNumType(0));
        }  // end of exe
      };

      template <>
      struct StensorComputeEigenTensorsDerivatives<3u>
          : public StensorComputeEigenTensorsDerivativesBase {
        template <typename ST2toST2Type, typename NumType>
        static TFEL_MATH_INLINE2 typename std::enable_if<
            (tfel::meta::Implements<ST2toST2Type, ST2toST2Concept>::cond) &&
                (ST2toST2Traits<ST2toST2Type>::dime == 3u) &&
                (tfel::typetraits::IsAssignableTo<
                    typename ComputeBinaryResult<
                        typename tfel::typetraits::BaseType<NumType>::type,
                        NumType,
                        OpDiv>::Result,
                    typename ST2toST2Traits<ST2toST2Type>::NumType>::cond),
            void>::type
        exe(ST2toST2Type& dn0_ds,
            ST2toST2Type& dn1_ds,
            ST2toST2Type& dn2_ds,
            const tfel::math::tvector<3u, NumType>& vp,
            const tfel::math::tmatrix<
                3u,
                3u,
                typename tfel::typetraits::BaseType<NumType>::type>& m,
            const NumType eps) {
          using namespace tfel::math;
          typedef typename tfel::typetraits::BaseType<NumType>::type base;
          constexpr const auto cste = Cste<base>::isqrt2;
          const tvector<3u, base> v0 = m.template column_view<0u>();
          const tvector<3u, base> v1 = m.template column_view<1u>();
          const tvector<3u, base> v2 = m.template column_view<2u>();
          const stensor<3u, base> n01 =
              stensor<3u, base>::buildFromVectorsSymmetricDiadicProduct(v0,
                                                                        v1) *
              cste;
          const stensor<3u, base> n02 =
              stensor<3u, base>::buildFromVectorsSymmetricDiadicProduct(v0,
                                                                        v2) *
              cste;
          const stensor<3u, base> n12 =
              stensor<3u, base>::buildFromVectorsSymmetricDiadicProduct(v1,
                                                                        v2) *
              cste;
          dn0_ds = regularized_inverse(vp(0) - vp(1), eps) * (n01 ^ n01) +
                   regularized_inverse(vp(0) - vp(2), eps) * (n02 ^ n02);
          dn1_ds = regularized_inverse(vp(1) - vp(0), eps) * (n01 ^ n01) +
                   regularized_inverse(vp(1) - vp(2), eps) * (n12 ^ n12);
          dn2_ds = regularized_inverse(vp(2) - vp(0), eps) * (n02 ^ n02) +
                   regularized_inverse(vp(2) - vp(1), eps) * (n12 ^ n12);
        }  // end of exe
      };

      template <unsigned short N>
      struct StensorComputeIsotropicFunctionDerivative;

      template <>
      struct StensorComputeIsotropicFunctionDerivative<1u> {
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
                  typename FunctionDerivative,
                  typename T>
        static typename std::enable_if<
            (tfel::meta::Implements<ST2toST2Type, ST2toST2Concept>::cond) &&
                (ST2toST2Traits<ST2toST2Type>::dime == 1u),
            void>::type
        exe(ST2toST2Type& d,
            const Function&,
            const FunctionDerivative& df,
            const tvector<3u, T>& vp,
            const tmatrix<3u,
                          3u,
                          typename tfel::typetraits::BaseType<T>::type>&,
            const T) {
          using real =
              typename tfel::math::ST2toST2Traits<ST2toST2Type>::NumType;
          d(0, 0) = df(vp[0]);
          d(0, 1) = d(0, 2) = real(0);
          d(1, 1) = df(vp[1]);
          d(1, 0) = d(1, 2) = real(0);
          d(2, 2) = df(vp[2]);
          d(2, 0) = d(2, 1) = real(0);
        }  // end of exe
      };

      template <>
      struct StensorComputeIsotropicFunctionDerivative<2u> {
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
                  typename T,
                  typename Function,
                  typename FunctionDerivative>
        static typename std::enable_if<
            (tfel::meta::Implements<ST2toST2Type, ST2toST2Concept>::cond) &&
                (ST2toST2Traits<ST2toST2Type>::dime == 2u),
            void>::type
        exe(ST2toST2Type& d,
            const Function& f,
            const FunctionDerivative& df,
            const tvector<3u, T>& vp,
            const tmatrix<3u, 3u, typename tfel::typetraits::BaseType<T>::type>&
                m,
            const T eps) {
          using real =
              typename tfel::math::ST2toST2Traits<ST2toST2Type>::NumType;
          using base = typename tfel::typetraits::BaseType<real>::type;
          using tvector = tfel::math::tvector<3u, real>;
          using stensor = tfel::math::stensor<2u, real>;
          constexpr const base cste = Cste<base>::sqrt2;
          stensor n0, n1, n2;
          stensor::computeEigenTensors(n0, n1, n2, m);
          const tvector v0 = m.template column_view<0u>();
          const tvector v1 = m.template column_view<1u>();
          const auto n01 =
              stensor::buildFromVectorsSymmetricDiadicProduct(v0, v1) / cste;
          if (std::abs(vp(0) - vp(1)) > eps) {
            d = (n0 ^ n0) * df(vp(0)) + (n1 ^ n1) * df(vp(1)) +
                (n2 ^ n2) * df(vp(2)) +
                (f(vp(0)) - f(vp(1))) / (vp(0) - vp(1)) * (n01 ^ n01);
          } else {
            const auto vpm = (vp(0) + vp(1)) / 2;
            d = ((n0 ^ n0) + (n1 ^ n1) + (n01 ^ n01)) * df(vpm) +
                (n2 ^ n2) * df(vp(2));
          }
        }  // end of exe
      };

      template <>
      struct StensorComputeIsotropicFunctionDerivative<3u> {
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
                  typename T,
                  typename Function,
                  typename FunctionDerivative>
        static typename std::enable_if<
            (tfel::meta::Implements<ST2toST2Type, ST2toST2Concept>::cond) &&
                (ST2toST2Traits<ST2toST2Type>::dime == 3u),
            void>::type
        exe(ST2toST2Type& d,
            const Function& f,
            const FunctionDerivative& df,
            const tvector<3u, T>& vp,
            const tmatrix<3u, 3u, typename tfel::typetraits::BaseType<T>::type>&
                m,
            const T eps) {
          using real =
              typename tfel::math::ST2toST2Traits<ST2toST2Type>::NumType;
          using base = typename tfel::typetraits::BaseType<real>::type;
          using tvector = tfel::math::tvector<3u, real>;
          using stensor = tfel::math::stensor<3u, real>;
          using st2tost2 = tfel::math::st2tost2<3u, real>;
          constexpr const base cste = Cste<base>::sqrt2;
          if ((std::abs(vp(0) - vp(1)) < eps) &&
              (std::abs(vp(0) - vp(2)) < eps)) {
            const auto vpm = (vp(0) + vp(1) + vp(2)) / 3;
            d = st2tost2::Id() * df(vpm);
          } else if (std::abs(vp(0) - vp(1)) < eps) {
            const tvector v0 = m.template column_view<0u>();
            const tvector v1 = m.template column_view<1u>();
            const tvector v2 = m.template column_view<2u>();
            stensor n0, n1, n2;
            stensor::computeEigenTensors(n0, n1, n2, m);
            const auto n01 =
                stensor::buildFromVectorsSymmetricDiadicProduct(v0, v1) / cste;
            const auto n02 =
                stensor::buildFromVectorsSymmetricDiadicProduct(v0, v2) / cste;
            const auto n12 =
                stensor::buildFromVectorsSymmetricDiadicProduct(v1, v2) / cste;
            const auto vpm = (vp(0) + vp(1)) / 2;
            d = (((n0 ^ n0) + (n1 ^ n1) + (n01 ^ n01)) * df(vpm) +
                 (n2 ^ n2) * df(vp(2)) +
                 (f(vp(0)) - f(vp(2))) / (vpm - vp(2)) *
                     ((n02 ^ n02) + (n12 ^ n12)));
          } else if (std::abs(vp(0) - vp(2)) < eps) {
            const tvector v0 = m.template column_view<0u>();
            const tvector v1 = m.template column_view<1u>();
            const tvector v2 = m.template column_view<2u>();
            stensor n0, n1, n2;
            stensor::computeEigenTensors(n0, n1, n2, m);
            const auto n01 =
                stensor::buildFromVectorsSymmetricDiadicProduct(v0, v1) / cste;
            const auto n02 =
                stensor::buildFromVectorsSymmetricDiadicProduct(v0, v2) / cste;
            const auto n12 =
                stensor::buildFromVectorsSymmetricDiadicProduct(v1, v2) / cste;
            const auto vpm = (vp(0) + vp(2)) / 2;
            d = (((n0 ^ n0) + (n2 ^ n2) + (n02 ^ n02)) * df(vpm) +
                 (n1 ^ n1) * df(vp(1)) +
                 (f(vp(0)) - f(vp(1))) / (vpm - vp(1)) *
                     ((n01 ^ n01) + (n12 ^ n12)));
          } else if (std::abs(vp(1) - vp(2)) < eps) {
            const tvector v0 = m.template column_view<0u>();
            const tvector v1 = m.template column_view<1u>();
            const tvector v2 = m.template column_view<2u>();
            stensor n0, n1, n2;
            stensor::computeEigenTensors(n0, n1, n2, m);
            const auto n01 =
                stensor::buildFromVectorsSymmetricDiadicProduct(v0, v1) / cste;
            const auto n02 =
                stensor::buildFromVectorsSymmetricDiadicProduct(v0, v2) / cste;
            const auto n12 =
                stensor::buildFromVectorsSymmetricDiadicProduct(v1, v2) / cste;
            const auto vpm = (vp(1) + vp(2)) / 2;
            d = ((n0 ^ n0) * df(vp(0)) +
                 ((n1 ^ n1) + (n2 ^ n2) + (n12 ^ n12)) * df(vpm) +
                 ((f(vp(0)) - f(vp(1))) / (vp(0) - vpm)) *
                     ((n01 ^ n01) + (n02 ^ n02)));
          } else {
            st2tost2 dn0, dn1, dn2;
            stensor n0, n1, n2;
            stensor::computeEigenTensors(n0, n1, n2, m);
            stensor::computeEigenTensorsDerivatives(dn0, dn1, dn2, vp, m,
                                                    eps / 4);
            d = (n0 ^ n0) * df(vp(0)) + (n1 ^ n1) * df(vp(1)) +
                (n2 ^ n2) * df(vp(2)) + f(vp(0)) * dn0 + f(vp(1)) * dn1 +
                f(vp(2)) * dn2;
          }
        }  // end of exe
      };

    }  // end of namespace internals

    template <typename Child>
    template <typename StensorType>
    typename std::enable_if<
        tfel::meta::Implements<StensorType, StensorConcept>::cond &&
            StensorTraits<Child>::dime == StensorTraits<StensorType>::dime &&
            tfel::typetraits::IsAssignableTo<
                typename StensorTraits<StensorType>::NumType,
                typename StensorTraits<Child>::NumType>::cond,
        Child&>::type
    stensor_base<Child>::operator=(const StensorType& src) {
      auto& child = static_cast<Child&>(*this);
      vectorToTab<StensorDimeToSize<StensorTraits<Child>::dime>::value>::exe(
          src, child);
      return child;
    }

    template <typename Child>
    template <typename StensorType>
    typename std::enable_if<
        tfel::meta::Implements<StensorType, StensorConcept>::cond &&
            StensorTraits<Child>::dime == StensorTraits<StensorType>::dime &&
            tfel::typetraits::IsAssignableTo<
                typename StensorTraits<StensorType>::NumType,
                typename StensorTraits<Child>::NumType>::cond,
        Child&>::type
    stensor_base<Child>::operator+=(const StensorType& src) {
      auto& child = static_cast<Child&>(*this);
      VectorUtilities<StensorDimeToSize<StensorTraits<Child>::dime>::value>::
          PlusEqual(child, src);
      return child;
    }

    template <typename Child>
    template <typename StensorType>
    typename std::enable_if<
        tfel::meta::Implements<StensorType, StensorConcept>::cond &&
            StensorTraits<Child>::dime == StensorTraits<StensorType>::dime &&
            tfel::typetraits::IsAssignableTo<
                typename StensorTraits<StensorType>::NumType,
                typename StensorTraits<Child>::NumType>::cond,
        Child&>::type
    stensor_base<Child>::operator-=(const StensorType& src) {
      auto& child = static_cast<Child&>(*this);
      VectorUtilities<StensorDimeToSize<StensorTraits<Child>::dime>::value>::
          MinusEqual(child, src);
      return child;
    }

    // *= operator
    template <typename Child>
    template <typename T2>
    typename std::enable_if<
        tfel::typetraits::IsScalar<T2>::cond &&
            std::is_same<
                typename ResultType<typename StensorTraits<Child>::NumType,
                                    T2,
                                    OpMult>::type,
                typename StensorTraits<Child>::NumType>::value,
        Child&>::type
    stensor_base<Child>::operator*=(const T2 s) {
      auto& child = static_cast<Child&>(*this);
      VectorUtilities<
          StensorDimeToSize<StensorTraits<Child>::dime>::value>::scale(child,
                                                                       s);
      return child;
    }

    // /= operator
    template <typename Child>
    template <typename T2>
    typename std::enable_if<
        tfel::typetraits::IsScalar<T2>::cond &&
            std::is_same<
                typename ResultType<typename StensorTraits<Child>::NumType,
                                    T2,
                                    OpDiv>::type,
                typename StensorTraits<Child>::NumType>::value,
        Child&>::type
    stensor_base<Child>::operator/=(const T2 s) {
      auto& child = static_cast<Child&>(*this);
      VectorUtilities<StensorDimeToSize<StensorTraits<Child>::dime>::value>::
          scale(
              child,
              (static_cast<typename tfel::typetraits::BaseType<T2>::type>(1u)) /
                  s);
      return child;
    }

    template <unsigned short N, typename T>
    template <
        typename T2,
        typename std::enable_if<tfel::typetraits::IsAssignableTo<T2, T>::cond,
                                bool>::type>
    constexpr stensor<N, T>::stensor(const T2& init)
        : fsarray<StensorDimeToSize<N>::value, T>(init) {}

    template <unsigned short N, typename T>
    template <
        typename T2,
        typename std::enable_if<tfel::typetraits::IsAssignableTo<T2, T>::cond,
                                bool>::type>
    constexpr stensor<N, T>::stensor(const std::initializer_list<T2>& init)
        : fsarray<StensorDimeToSize<N>::value, T>(init) {}

    template <unsigned short N, typename T>
    template <typename InputIterator,
              typename std::enable_if<
                  std::is_same<
                      typename std::iterator_traits<InputIterator>::value_type,
                      typename tfel::typetraits::BaseType<T>::type>::value,
                  bool>::type>
    stensor<N, T>::stensor(const InputIterator p) {
      typedef typename tfel::typetraits::BaseType<T>::type base;
      TFEL_STATIC_ASSERT(
          (tfel::typetraits::IsSafelyReinterpretCastableTo<T, base>::cond));
      tfel::fsalgo::copy<StensorDimeToSize<N>::value>::exe(
          p, reinterpret_cast<base*>(this->v));
    }

    template <unsigned short N, typename T>
    template <
        typename T2,
        typename std::enable_if<tfel::typetraits::IsAssignableTo<T2, T>::cond,
                                bool>::type>
    constexpr stensor<N, T>::stensor(const stensor<N, T2>& src)
        : fsarray<StensorDimeToSize<N>::value, T>(src) {}

    template <unsigned short N, typename T>
    template <
        typename T2,
        typename Operation,
        typename std::enable_if<tfel::typetraits::IsAssignableTo<T2, T>::cond,
                                bool>::type>
    stensor<N, T>::stensor(const Expr<stensor<N, T2>, Operation>& src) {
      vectorToTab<StensorDimeToSize<N>::value>::exe(src, this->v);
    }

    template <unsigned short N, typename T>
    T& stensor<N, T>::operator()(const unsigned short i) noexcept {
      return this->v[i];
    }

    template <unsigned short N, typename T>
    constexpr const T& stensor<N, T>::operator()(
        const unsigned short i) const noexcept {
      return this->v[i];
    }

    template <unsigned short N, typename T>
    constexpr typename stensor<N, T>::RunTimeProperties
    stensor<N, T>::getRunTimeProperties(void) const noexcept {
      return RunTimeProperties();
    }

    // Import from Voigt
    template <unsigned short N, typename T>
    template <typename InputIterator>
    typename std::enable_if<
        std::is_same<typename std::iterator_traits<InputIterator>::value_type,
                     typename tfel::typetraits::BaseType<T>::type>::value,
        void>::type
    stensor<N, T>::importVoigt(const InputIterator p) {
      typedef typename tfel::typetraits::BaseType<T>::type base;
      tfel::math::internals::ImportFromVoigt<N>::exe(
          reinterpret_cast<base*>(this->v), p);
    }

    // Import from Tab
    template <unsigned short N, typename T>
    template <typename InputIterator>
    typename std::enable_if<
        std::is_same<typename std::iterator_traits<InputIterator>::value_type,
                     typename tfel::typetraits::BaseType<T>::type>::value,
        void>::type
    stensor<N, T>::importTab(const InputIterator p) {
      typedef typename tfel::typetraits::BaseType<T>::type base;
      typedef tfel::math::internals::ImportFromTab<N> Import;
      Import::exe(reinterpret_cast<base*>(this->v), p);
    }

    // Import from values
    template <unsigned short N, typename T>
    template <typename InputIterator>
    typename std::enable_if<
        std::is_same<typename std::iterator_traits<InputIterator>::value_type,
                     typename tfel::typetraits::BaseType<T>::type>::value,
        void>::type
    stensor<N, T>::import(const InputIterator p) {
      typedef typename tfel::typetraits::BaseType<T>::type base;
      tfel::fsalgo::copy<StensorDimeToSize<N>::value>::exe(
          p, reinterpret_cast<base*>(this->v));
    }

    // Export to Tab
    template <unsigned short N, typename T>
    template <typename T2>
    typename std::enable_if<
        tfel::typetraits::IsSafelyReinterpretCastableTo<
            T2,
            typename tfel::typetraits::BaseType<T>::type>::cond,
        void>::type
    stensor<N, T>::exportTab(T2* const src) const {
      typedef typename tfel::typetraits::BaseType<T>::type base;
      typedef tfel::math::internals::ExportToTab<N> Export;
      TFEL_STATIC_ASSERT(
          (tfel::typetraits::IsSafelyReinterpretCastableTo<T, base>::cond));
      Export::exe(reinterpret_cast<const T2*>(this->v), src);
    }

    // Write to Tab
    template <unsigned short N, typename T>
    template <typename T2>
    typename std::enable_if<
        tfel::typetraits::IsSafelyReinterpretCastableTo<
            T2,
            typename tfel::typetraits::BaseType<T>::type>::cond,
        void>::type
    stensor<N, T>::write(T2* const t) const {
      typedef typename tfel::typetraits::BaseType<T>::type base;
      typedef tfel::fsalgo::copy<StensorDimeToSize<N>::value> Copy;
      TFEL_STATIC_ASSERT(
          (tfel::typetraits::IsSafelyReinterpretCastableTo<T, base>::cond));
      Copy::exe(reinterpret_cast<const base*>(this->v), t);
    }

    // computeEigenValues
    template <unsigned short N, typename T>
    void stensor<N, T>::computeEigenValues(T& vp1,
                                           T& vp2,
                                           T& vp3,
                                           const bool b) const {
      TFEL_STATIC_ASSERT(tfel::typetraits::IsFundamentalNumericType<
                         typename tfel::typetraits::BaseType<T>::type>::cond);
      TFEL_STATIC_ASSERT(tfel::typetraits::IsReal<
                         typename tfel::typetraits::BaseType<T>::type>::cond);
      TFEL_STATIC_ASSERT(
          (tfel::typetraits::IsSafelyReinterpretCastableTo<
              T, typename tfel::typetraits::BaseType<T>::type>::cond));
      tfel::math::internals::StensorComputeEigenValues_<N>::exe(
          reinterpret_cast<const typename tfel::typetraits::BaseType<T>::type*>(
              this->v),
          reinterpret_cast<typename tfel::typetraits::BaseType<T>::type&>(vp1),
          reinterpret_cast<typename tfel::typetraits::BaseType<T>::type&>(vp2),
          reinterpret_cast<typename tfel::typetraits::BaseType<T>::type&>(vp3),
          b);
    }

    // computeEigenValues
    template <unsigned short N, typename T>
    void stensor<N, T>::computeEigenValues(tvector<3u, T>& vp,
                                           const bool b) const {
      TFEL_STATIC_ASSERT(tfel::typetraits::IsFundamentalNumericType<
                         typename tfel::typetraits::BaseType<T>::type>::cond);
      TFEL_STATIC_ASSERT(tfel::typetraits::IsReal<
                         typename tfel::typetraits::BaseType<T>::type>::cond);
      TFEL_STATIC_ASSERT(
          (tfel::typetraits::IsSafelyReinterpretCastableTo<
              T, typename tfel::typetraits::BaseType<T>::type>::cond));
      tfel::math::internals::StensorComputeEigenValues_<N>::exe(
          reinterpret_cast<
              const typename tfel::typetraits::BaseType<T>::type* const>(
              this->v),
          reinterpret_cast<typename tfel::typetraits::BaseType<T>::type&>(
              vp(0)),
          reinterpret_cast<typename tfel::typetraits::BaseType<T>::type&>(
              vp(1)),
          reinterpret_cast<typename tfel::typetraits::BaseType<T>::type&>(
              vp(2)),
          b);
    }

    // computeEigenVectors
    template <unsigned short N, typename T>
    bool stensor<N, T>::computeEigenVectors(
        tvector<3u, T>& vp,
        tmatrix<3u, 3u, typename tfel::typetraits::BaseType<T>::type>& vec,
        const bool b) const {
      TFEL_STATIC_ASSERT(tfel::typetraits::IsFundamentalNumericType<
                         typename tfel::typetraits::BaseType<T>::type>::cond);
      TFEL_STATIC_ASSERT(tfel::typetraits::IsReal<
                         typename tfel::typetraits::BaseType<T>::type>::cond);
      TFEL_STATIC_ASSERT(
          (tfel::typetraits::IsSafelyReinterpretCastableTo<
              T, typename tfel::typetraits::BaseType<T>::type>::cond));
      return tfel::math::internals::StensorComputeEigenVectors_<N>::exe(
          reinterpret_cast<const typename tfel::typetraits::BaseType<T>::type*>(
              this->v),
          reinterpret_cast<
              tvector<3u, typename tfel::typetraits::BaseType<T>::type>&>(vp),
          vec, b);
    }

    // computeEigenVectors
    template <unsigned short N, typename T>
    template <typename VectorType>
    bool stensor<N, T>::computeEigenVector(VectorType& ev, const T vp) const {
      typedef typename VectorTraits<VectorType>::NumType real;
      typedef tfel::math::internals::StensorComputeEigenVectors_<N> SCEV;
      TFEL_STATIC_ASSERT(
          tfel::typetraits::IsFundamentalNumericType<real>::cond);
      TFEL_STATIC_ASSERT(tfel::typetraits::IsReal<real>::cond);
      TFEL_STATIC_ASSERT(
          (tfel::typetraits::IsSafelyReinterpretCastableTo<T, real>::cond));
      return SCEV::computeEigenVector(ev,
                                      reinterpret_cast<const real*>(this->v),
                                      *(reinterpret_cast<const real*>(&vp)));
    }

    template <unsigned short N, typename T>
    template <typename StensorType>
    typename std::enable_if<
        (tfel::meta::Implements<StensorType, StensorConcept>::cond) &&
            (StensorTraits<StensorType>::dime == N) &&
            (tfel::typetraits::IsAssignableTo<
                typename tfel::typetraits::BaseType<T>::type,
                typename StensorTraits<StensorType>::NumType>::cond),
        void>::type
    stensor<N, T>::computeEigenValuesDerivatives(
        StensorType& n0,
        StensorType& n1,
        StensorType& n2,
        const tmatrix<3u, 3u, typename tfel::typetraits::BaseType<T>::type>&
            m) {
      return stensor<N, T>::computeEigenTensors(n0, n1, n2, m);
    }  // end of stensor<N,T>::computeEigenValuesDerivatives

    template <unsigned short N, typename T>
    template <typename StensorType>
    typename std::enable_if<
        (tfel::meta::Implements<StensorType, StensorConcept>::cond) &&
            (StensorTraits<StensorType>::dime == N) &&
            (tfel::typetraits::IsAssignableTo<
                typename tfel::typetraits::BaseType<T>::type,
                typename StensorTraits<StensorType>::NumType>::cond),
        void>::type
    stensor<N, T>::computeEigenTensors(
        StensorType& n0,
        StensorType& n1,
        StensorType& n2,
        const tmatrix<3u, 3u, typename tfel::typetraits::BaseType<T>::type>&
            m) {
      return tfel::math::internals::StensorComputeEigenValuesDerivatives<
          N>::exe(n0, n1, n2, m);
    }  // end of stensor<N,T>::computeEigenTensors

    template <unsigned short N, typename T>
    template <typename ST2toST2Type>
    typename std::enable_if<
        (tfel::meta::Implements<ST2toST2Type, ST2toST2Concept>::cond) &&
            (ST2toST2Traits<ST2toST2Type>::dime == N) &&
            (tfel::typetraits::IsAssignableTo<
                typename ComputeBinaryResult<
                    typename tfel::typetraits::BaseType<T>::type,
                    T,
                    OpDiv>::Result,
                typename ST2toST2Traits<ST2toST2Type>::NumType>::cond),
        void>::type
    stensor<N, T>::computeEigenTensorsDerivatives(
        ST2toST2Type& dn0_ds,
        ST2toST2Type& dn1_ds,
        ST2toST2Type& dn2_ds,
        const tvector<3u, T>& vp,
        const tmatrix<3u, 3u, typename tfel::typetraits::BaseType<T>::type>& m,
        const T eps) {
      return tfel::math::internals::StensorComputeEigenTensorsDerivatives<
          N>::exe(dn0_ds, dn1_ds, dn2_ds, vp, m, eps);
    }

    template <unsigned short N, typename T>
    template <typename ST2toST2Type,
              typename Function,
              typename FunctionDerivative>
    typename std::enable_if<
        (tfel::meta::Implements<ST2toST2Type, ST2toST2Concept>::cond) &&
            (ST2toST2Traits<ST2toST2Type>::dime == N) &&
            (tfel::typetraits::IsAssignableTo<
                typename ComputeBinaryResult<
                    typename tfel::typetraits::BaseType<T>::type,
                    T,
                    OpDiv>::Result,
                typename ST2toST2Traits<ST2toST2Type>::NumType>::cond),
        void>::type
    stensor<N, T>::computeIsotropicFunctionDerivative(
        ST2toST2Type& d,
        const Function& f,
        const FunctionDerivative& df,
        const tvector<3u, T>& vp,
        const tmatrix<3u, 3u, typename tfel::typetraits::BaseType<T>::type>& m,
        const T eps) {
      tfel::math::internals::StensorComputeIsotropicFunctionDerivative<N>::exe(
          d, f, df, vp, m, eps);
    }

    template <unsigned short N, typename T>
    template <typename Function>
    stensor<N, T> stensor<N, T>::computeIsotropicFunction(const Function& f,
                                                          const bool b) const {
      using base = typename tfel::typetraits::BaseType<T>::type;
      tvector<3u, T> vp;
      tmatrix<3u, 3u, base> m;
      this->computeEigenVectors(vp, m, b);
      return stensor<N, T>::buildFromEigenValuesAndVectors(f(vp(0)), f(vp(1)),
                                                           f(vp(2)), m);
    }

    template <unsigned short N, typename T>
    template <typename Function, typename FunctionDerivative>
    st2tost2<N, T> stensor<N, T>::computeIsotropicFunctionDerivative(
        const Function& f,
        const FunctionDerivative& df,
        const T eps,
        const bool b) const {
      using base = typename tfel::typetraits::BaseType<T>::type;
      st2tost2<N, T> r;
      tvector<3u, T> vp;
      tmatrix<3u, 3u, base> m;
      this->computeEigenVectors(vp, m, b);
      stensor<N, T>::computeIsotropicFunctionDerivative(r, f, df, vp, m, eps);
      return r;
    }

    template <unsigned short N, typename T>
    template <typename Function, typename FunctionDerivative>
    std::pair<stensor<N, T>, st2tost2<N, T>>
    stensor<N, T>::computeIsotropicFunctionAndDerivative(
        const Function& f,
        const FunctionDerivative& df,
        const T eps,
        const bool b) const {
      using base = typename tfel::typetraits::BaseType<T>::type;
      std::pair<stensor<N, T>, st2tost2<N, T>> r;
      tvector<3u, T> vp;
      tmatrix<3u, 3u, base> m;
      this->computeEigenVectors(vp, m, b);
      r.first = stensor<N, T>::buildFromEigenValuesAndVectors(
          f(vp(0)), f(vp(1)), f(vp(2)), m);
      stensor<N, T>::computeIsotropicFunctionDerivative(r.second, f, df, vp, m,
                                                        eps);
      return r;
    }  // end of stensor<N,T>::computeIsotropicFunctionAndDerivative

    // ChangeBasis
    template <unsigned short N, typename T>
    void stensor<N, T>::changeBasis(
        const tmatrix<3u, 3u, typename tfel::typetraits::BaseType<T>::type>&
            m) {
      return tfel::math::internals::StensorChangeBasis<N>::exe(this->v, m);
    }

    // Return Id
    template <unsigned short N, typename T>
    constexpr stensor<N, T> stensor<N, T>::Id(void) {
      return stensor<N, T>{{T{1}, T{1}, T{1}, T{0}, T{0}, T{0}}};
    }  // end of stensor<N,T>::Id

    template <unsigned short N, typename T>
    template <typename InputIterator>
    void stensor<N, T>::copy(const InputIterator src) {
      tfel::fsalgo::copy<StensorDimeToSize<N>::value>::exe(src, *this);
    }

    template <unsigned short N, typename T, typename OutputIterator>
    TFEL_MATH_INLINE2
        typename std::enable_if<tfel::typetraits::IsScalar<T>::cond, void>::type
        exportToBaseTypeArray(const stensor<N, T>& s, OutputIterator p) {
      typedef typename tfel::typetraits::BaseType<T>::type base;
      typedef tfel::fsalgo::copy<StensorDimeToSize<N>::value> Copy;
      TFEL_STATIC_ASSERT(
          (tfel::typetraits::IsSafelyReinterpretCastableTo<T, base>::cond));
      Copy::exe(reinterpret_cast<const base*>(&s[0]), p);
    }

    template <typename T>
    T tresca(const stensor<1u, T>& s, const bool) {
      const T sd1 = std::abs(s[0] - s[1]);
      const T sd2 = std::abs(s[0] - s[2]);
      const T sd3 = std::abs(s[2] - s[1]);
      const T tmp = sd1 > sd2 ? sd1 : sd2;
      const T tmp2 = sd3 > tmp ? sd3 : tmp;
      return tmp2;
    }  // end of tresca

    template <unsigned short N, typename T>
    T tresca(const stensor<N, T>& s, const bool b) {
      T s1, s2, s3;
      s.computeEigenValues(s1, s2, s3, b);
      const T sd1 = std::abs(s1 - s2);
      const T sd2 = std::abs(s1 - s3);
      const T sd3 = std::abs(s3 - s2);
      const T tmp = sd1 > sd2 ? sd1 : sd2;
      const T tmp2 = sd3 > tmp ? sd3 : tmp;
      return tmp2;
    }  // end of tresca

    template <typename StensorType>
    typename std::enable_if<
        tfel::meta::Implements<StensorType, StensorConcept>::cond,
        stensor<StensorTraits<StensorType>::dime,
                typename StensorTraits<StensorType>::NumType>>::type
    square_root(const StensorType& s) {
      using tfel::fsalgo::copy;
      typedef typename StensorTraits<StensorType>::NumType T;
      typedef typename tfel::typetraits::BaseType<T>::type real;
      stensor<StensorTraits<StensorType>::dime, T> tmp(s);
      stensor<StensorTraits<StensorType>::dime, T> r(T(0));
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
    typename std::enable_if<
        tfel::meta::Implements<StensorType, StensorConcept>::cond,
        typename ComputeUnaryResult<
            typename StensorTraits<StensorType>::NumType,
            Power<3>>::Result>::type
    det(const StensorType& s) {
      const stensor<StensorTraits<StensorType>::dime,
                    typename StensorTraits<StensorType>::NumType>
          tmp(s);
      return tfel::math::internals::StensorDeterminant<
          StensorTraits<StensorType>::dime>::exe(tmp);
    }

    template <typename StensorType>
    typename std::enable_if<
        tfel::meta::Implements<StensorType, StensorConcept>::cond,
        stensor<StensorTraits<StensorType>::dime,
                typename ComputeBinaryResult<
                    typename tfel::typetraits::BaseType<
                        typename StensorTraits<StensorType>::NumType>::type,
                    typename StensorTraits<StensorType>::NumType,
                    OpDiv>::Result>>::type
    invert(const StensorType& s) {
      const stensor<StensorTraits<StensorType>::dime,
                    typename StensorTraits<StensorType>::NumType>
          tmp(s);
      return tfel::math::internals::StensorInvert<
          StensorTraits<StensorType>::dime>::exe(tmp);
    }

    template <unsigned short N, typename T>
    template <typename MatrixType>
    typename std::enable_if<tfel::typetraits::IsAssignableTo<
                                typename MatrixTraits<MatrixType>::NumType,
                                T>::cond,
                            stensor<N, T>>::type
    stensor<N, T>::buildFromMatrix(const MatrixType& m) {
      stensor<N, T> s;
      tfel::math::internals::BuildStensorFromMatrix<N>::exe(s, m);
      return s;
    }

    template <unsigned short N, typename T>
    template <typename VectorType>
    typename std::enable_if<tfel::typetraits::IsAssignableTo<
                                typename ComputeUnaryResult<
                                    typename VectorTraits<VectorType>::NumType,
                                    Power<2>>::Result,
                                T>::cond,
                            stensor<N, T>>::type
    stensor<N, T>::buildFromVectorDiadicProduct(const VectorType& vec) {
      stensor<N, T> s;
      tfel::math::internals::BuildStensorFromVectorDiadicProduct<N>::exe(s,
                                                                         vec);
      return s;
    }

    template <unsigned short N, typename T>
    template <typename VectorType, typename VectorType2>
    typename std::enable_if<tfel::typetraits::IsAssignableTo<
                                typename ComputeBinaryResult<
                                    typename VectorTraits<VectorType>::NumType,
                                    typename VectorTraits<VectorType2>::NumType,
                                    OpMult>::Result,
                                T>::cond,
                            stensor<N, T>>::type
    stensor<N, T>::buildFromVectorsSymmetricDiadicProduct(
        const VectorType& v1, const VectorType2& v2) {
      stensor<N, T> s;
      tfel::math::internals::BuildStensorFromVectorsSymmetricDiadicProduct<
          N>::exe(s, v1, v2);
      return s;
    }

    template <unsigned short N, typename T>
    stensor<N, T> stensor<N, T>::buildFromEigenValuesAndVectors(
        const T& v1,
        const T& v2,
        const T& v3,
        const tmatrix<3u, 3u, typename tfel::typetraits::BaseType<T>::type>&
            m) {
      stensor<N, T> s;
      tfel::math::internals::BuildStensorFromEigenValuesAndVectors<N>::exe(
          s, v1, v2, v3, m);
      return s;
    }

    template <unsigned short N, typename T>
    stensor<N, T> stensor<N, T>::buildFromEigenValuesAndVectors(
        const tvector<3u, T>& vp,
        const tmatrix<3u, 3u, typename tfel::typetraits::BaseType<T>::type>&
            m) {
      stensor<N, T> s;
      tfel::math::internals::BuildStensorFromEigenValuesAndVectors<N>::exe(
          s, vp(0), vp(1), vp(2), m);
      return s;
    }

    template <unsigned short N, typename T>
    stensor<N, T> stensor<N, T>::buildLogarithmFromEigenValuesAndVectors(
        const T& v1,
        const T& v2,
        const T& v3,
        const tmatrix<3u, 3u, typename tfel::typetraits::BaseType<T>::type>&
            m) {
      stensor<N, T> s;
      tfel::math::internals::BuildStensorFromEigenValuesAndVectors<N>::exe(
          s, std::log(v1), std::log(v2), std::log(v3), m);
      return s;
    }

    template <unsigned short N, typename T>
    stensor<N, T> stensor<N, T>::buildLogarithmFromEigenValuesAndVectors(
        const tvector<3u, T>& vp,
        const tmatrix<3u, 3u, typename tfel::typetraits::BaseType<T>::type>&
            m) {
      stensor<N, T> s;
      tfel::math::internals::BuildStensorFromEigenValuesAndVectors<N>::exe(
          s, std::log(vp(0)), std::log(vp(1)), std::log(vp(2)), m);
      return s;
    }

    template <unsigned short N, typename T>
    stensor<N, T> stensor<N, T>::buildPositivePartFromEigenValuesAndVectors(
        const T& v1,
        const T& v2,
        const T& v3,
        const tmatrix<3u, 3u, typename tfel::typetraits::BaseType<T>::type>&
            m) {
      stensor<N, T> s;
      tfel::math::internals::BuildStensorFromEigenValuesAndVectors<N>::exe(
          s, std::max(T(0), v1), std::max(T(0), v2), std::max(T(0), v3), m);
      return s;
    }

    template <unsigned short N, typename T>
    stensor<N, T> stensor<N, T>::buildPositivePartFromEigenValuesAndVectors(
        const tvector<3u, T>& vp,
        const tmatrix<3u, 3u, typename tfel::typetraits::BaseType<T>::type>&
            m) {
      stensor<N, T> s;
      tfel::math::internals::BuildStensorFromEigenValuesAndVectors<N>::exe(
          s, std::max(T(0), vp(0)), std::max(T(0), vp(1)),
          std::max(T(0), vp(2)), m);
      return s;
    }

    template <unsigned short N, typename T>
    stensor<N, T> stensor<N, T>::buildNegativePartFromEigenValuesAndVectors(
        const T& v1,
        const T& v2,
        const T& v3,
        const tmatrix<3u, 3u, typename tfel::typetraits::BaseType<T>::type>&
            m) {
      stensor<N, T> s;
      tfel::math::internals::BuildStensorFromEigenValuesAndVectors<N>::exe(
          s, std::min(T(0), v1), std::min(T(0), v2), std::min(T(0), v3), m);
      return s;
    }

    template <unsigned short N, typename T>
    stensor<N, T> stensor<N, T>::buildNegativePartFromEigenValuesAndVectors(
        const tvector<3u, T>& vp,
        const tmatrix<3u, 3u, typename tfel::typetraits::BaseType<T>::type>&
            m) {
      stensor<N, T> s;
      tfel::math::internals::BuildStensorFromEigenValuesAndVectors<N>::exe(
          s, std::min(T(0), vp(0)), std::min(T(0), vp(1)),
          std::min(T(0), vp(2)), m);
      return s;
    }

    template <typename StensorType>
    typename std::enable_if<
        ((tfel::meta::Implements<StensorType, StensorConcept>::cond) &&
         (StensorTraits<StensorType>::dime == 1u) &&
         (tfel::typetraits::IsFundamentalNumericType<
             typename StensorTraits<StensorType>::NumType>::cond)),
        stensor<1u, typename StensorTraits<StensorType>::NumType>>::type
    logarithm(const StensorType& s, const bool) {
      typedef typename StensorTraits<StensorType>::NumType NumType;
      stensor<1u, NumType> l;
      l(0) = std::log(s(0));
      l(1) = std::log(s(1));
      l(2) = std::log(s(2));
      return l;
    }

    template <typename StensorType>
    typename std::enable_if<
        ((tfel::meta::Implements<StensorType, StensorConcept>::cond) &&
         ((StensorTraits<StensorType>::dime == 2u) ||
          (StensorTraits<StensorType>::dime == 3u)) &&
         (tfel::typetraits::IsFundamentalNumericType<
             typename StensorTraits<StensorType>::NumType>::cond)),
        stensor<StensorTraits<StensorType>::dime,
                typename StensorTraits<StensorType>::NumType>>::type
    logarithm(const StensorType& s_, const bool b) {
      typedef typename StensorTraits<StensorType>::NumType NumType;
      typedef typename tfel::typetraits::BaseType<NumType>::type base;
      typedef stensor<StensorTraits<StensorType>::dime, NumType> Stensor;
      tvector<3u, NumType> vp;
      tmatrix<3u, 3u, base> m;
      stensor<StensorTraits<StensorType>::dime, NumType> s(s_);
      s.computeEigenVectors(vp, m, b);
      return Stensor::buildFromEigenValuesAndVectors(
          std::log(vp(0)), std::log(vp(1)), std::log(vp(2)), m);
    }

    template <typename StensorType>
    typename std::enable_if<
        ((tfel::meta::Implements<StensorType, StensorConcept>::cond) &&
         (StensorTraits<StensorType>::dime == 1u) &&
         (tfel::typetraits::IsFundamentalNumericType<
             typename StensorTraits<StensorType>::NumType>::cond)),
        stensor<1u, typename StensorTraits<StensorType>::NumType>>::type
    absolute_value(const StensorType& s, const bool) {
      return {std::abs(s(0)), std::abs(s(1)), std::abs(s(2))};
    }

    template <typename StensorType>
    typename std::enable_if<
        ((tfel::meta::Implements<StensorType, StensorConcept>::cond) &&
         ((StensorTraits<StensorType>::dime == 2u) ||
          (StensorTraits<StensorType>::dime == 3u)) &&
         (tfel::typetraits::IsFundamentalNumericType<
             typename StensorTraits<StensorType>::NumType>::cond)),
        stensor<StensorTraits<StensorType>::dime,
                typename StensorTraits<StensorType>::NumType>>::type
    absolute_value(const StensorType& s_, const bool b) {
      typedef typename StensorTraits<StensorType>::NumType NumType;
      typedef typename tfel::typetraits::BaseType<NumType>::type base;
      typedef stensor<StensorTraits<StensorType>::dime, NumType> Stensor;
      tvector<3u, NumType> vp;
      tmatrix<3u, 3u, base> m;
      stensor<StensorTraits<StensorType>::dime, NumType> s(s_);
      s.computeEigenVectors(vp, m, b);
      return Stensor::buildFromEigenValuesAndVectors(
          std::abs(vp(0)), std::abs(vp(1)), std::abs(vp(2)), m);
    }

    template <typename StensorType>
    typename std::enable_if<
        ((tfel::meta::Implements<StensorType, StensorConcept>::cond) &&
         (StensorTraits<StensorType>::dime == 1u) &&
         (tfel::typetraits::IsFundamentalNumericType<
             typename StensorTraits<StensorType>::NumType>::cond)),
        stensor<1u, typename StensorTraits<StensorType>::NumType>>::type
    positive_part(const StensorType& s, const bool) {
      typedef typename StensorTraits<StensorType>::NumType NumType;
      return {std::max(s(0), NumType(0)), std::max(s(1), NumType(0)),
              std::max(s(2), NumType(0))};
    }

    template <typename StensorType>
    typename std::enable_if<
        ((tfel::meta::Implements<StensorType, StensorConcept>::cond) &&
         ((StensorTraits<StensorType>::dime == 2u) ||
          (StensorTraits<StensorType>::dime == 3u)) &&
         (tfel::typetraits::IsFundamentalNumericType<
             typename StensorTraits<StensorType>::NumType>::cond)),
        stensor<StensorTraits<StensorType>::dime,
                typename StensorTraits<StensorType>::NumType>>::type
    positive_part(const StensorType& s_, const bool b) {
      typedef typename StensorTraits<StensorType>::NumType NumType;
      typedef typename tfel::typetraits::BaseType<NumType>::type base;
      typedef stensor<StensorTraits<StensorType>::dime, NumType> Stensor;
      tvector<3u, NumType> vp;
      tmatrix<3u, 3u, base> m;
      stensor<StensorTraits<StensorType>::dime, NumType> s(s_);
      s.computeEigenVectors(vp, m, b);
      return Stensor::buildFromEigenValuesAndVectors(
          std::max(vp(0), NumType(0)), std::max(vp(1), NumType(0)),
          std::max(vp(2), NumType(0)), m);
    }

    template <typename StensorType>
    typename std::enable_if<
        ((tfel::meta::Implements<StensorType, StensorConcept>::cond) &&
         (StensorTraits<StensorType>::dime == 1u) &&
         (tfel::typetraits::IsFundamentalNumericType<
             typename StensorTraits<StensorType>::NumType>::cond)),
        stensor<1u, typename StensorTraits<StensorType>::NumType>>::type
    negative_part(const StensorType& s, const bool) {
      typedef typename StensorTraits<StensorType>::NumType NumType;
      return {std::min(s(0), NumType(0)), std::min(s(1), NumType(0)),
              std::min(s(2), NumType(0))};
    }

    template <typename StensorType>
    typename std::enable_if<
        ((tfel::meta::Implements<StensorType, StensorConcept>::cond) &&
         ((StensorTraits<StensorType>::dime == 2u) ||
          (StensorTraits<StensorType>::dime == 3u)) &&
         (tfel::typetraits::IsFundamentalNumericType<
             typename StensorTraits<StensorType>::NumType>::cond)),
        stensor<StensorTraits<StensorType>::dime,
                typename StensorTraits<StensorType>::NumType>>::type
    negative_part(const StensorType& s_, const bool b) {
      typedef typename StensorTraits<StensorType>::NumType NumType;
      typedef typename tfel::typetraits::BaseType<NumType>::type base;
      typedef stensor<StensorTraits<StensorType>::dime, NumType> Stensor;
      tvector<3u, NumType> vp;
      tmatrix<3u, 3u, base> m;
      stensor<StensorTraits<StensorType>::dime, NumType> s(s_);
      s.computeEigenVectors(vp, m, b);
      return Stensor::buildFromEigenValuesAndVectors(
          std::min(vp(0), NumType(0)), std::min(vp(1), NumType(0)),
          std::min(vp(2), NumType(0)), m);
    }

    template <typename StensorType>
    typename std::enable_if<
        tfel::meta::Implements<StensorType, StensorConcept>::cond &&
            StensorTraits<StensorType>::dime == 1u,
        stensor<1u,
                typename ComputeBinaryResult<
                    typename StensorTraits<StensorType>::NumType,
                    typename StensorTraits<StensorType>::NumType,
                    OpMult>::Result>>::type
    square(const StensorType& s) {
      using T = typename StensorTraits<StensorType>::NumType;
      using T2 = typename ComputeBinaryResult<T, T, OpMult>::Result;
      return stensor<1u, T2>{s(0) * s(0), s(1) * s(1), s(2) * s(2)};
    }

    template <typename StensorType>
    typename std::enable_if<
        tfel::meta::Implements<StensorType, StensorConcept>::cond &&
            StensorTraits<StensorType>::dime == 2u,
        stensor<2u,
                typename ComputeBinaryResult<
                    typename StensorTraits<StensorType>::NumType,
                    typename StensorTraits<StensorType>::NumType,
                    OpMult>::Result>>::type
    square(const StensorType& s) {
      using T = typename StensorTraits<StensorType>::NumType;
      using T2 = typename ComputeBinaryResult<T, T, OpMult>::Result;
      using base = typename tfel::typetraits::BaseType<T>::type;
      TFEL_CONSTEXPR const base one_half = 1 / base(2);
      return stensor<2u, T2>{(s(3) * s(3) + 2 * s(0) * s(0)) * one_half,
                             (s(3) * s(3) + 2 * s(1) * s(1)) * one_half,
                             s(2) * s(2), (s(1) + s(0)) * s(3)};
    }

    template <typename StensorType>
    typename std::enable_if<
        tfel::meta::Implements<StensorType, StensorConcept>::cond &&
            StensorTraits<StensorType>::dime == 3u,
        stensor<3u,
                typename ComputeBinaryResult<
                    typename StensorTraits<StensorType>::NumType,
                    typename StensorTraits<StensorType>::NumType,
                    OpMult>::Result>>::type
    square(const StensorType& s) {
      using T = typename StensorTraits<StensorType>::NumType;
      using T2 = typename ComputeBinaryResult<T, T, OpMult>::Result;
      using base = typename tfel::typetraits::BaseType<T>::type;
      typedef typename StensorTraits<StensorType>::NumType T;
      constexpr const base cste = Cste<base>::sqrt2;
      TFEL_CONSTEXPR const base one_half = 1 / (base(2));
      return stensor<3u, T2>{
          (s(4) * s(4) + s(3) * s(3) + 2 * s(0) * s(0)) * one_half,
          (s(5) * s(5) + s(3) * s(3) + 2 * s(1) * s(1)) * one_half,
          (s(5) * s(5) + s(4) * s(4) + 2 * s(2) * s(2)) * one_half,
          (cste * s(4) * s(5) + 2 * (s(1) + s(0)) * s(3)) * one_half,
          (cste * s(3) * s(5) + 2 * (s(2) + s(0)) * s(4)) * one_half,
          (2 * (s(2) + s(1)) * s(5) + cste * s(3) * s(4)) * one_half};
    }

    template <typename StensorType>
    typename std::enable_if<
        tfel::meta::Implements<StensorType, StensorConcept>::cond,
        stensor<StensorTraits<StensorType>::dime,
                typename StensorTraits<StensorType>::NumType>>::type
    change_basis(
        const StensorType& s,
        const tmatrix<3u,
                      3u,
                      typename tfel::typetraits::BaseType<
                          typename StensorTraits<StensorType>::NumType>::type>&
            r) {
      stensor<StensorTraits<StensorType>::dime,
              typename StensorTraits<StensorType>::NumType>
          ns{s};
      ns.changeBasis(r);
      return ns;
    }

    template <typename Function, typename StensorType>
    typename std::enable_if<
        tfel::meta::Implements<StensorType, StensorConcept>::cond,
        stensor<StensorTraits<StensorType>::dime,
                typename StensorTraits<StensorType>::NumType>>::type
    computeIsotropicFunction(const Function& f,
                             const StensorType& s,
                             const bool b) {
      return s.computeIsotropicFunction(f, b);
    }

    template <typename Function,
              typename FunctionDerivative,
              typename StensorType>
    typename std::enable_if<
        tfel::meta::Implements<StensorType, StensorConcept>::cond,
        st2tost2<StensorTraits<StensorType>::dime,
                 typename StensorTraits<StensorType>::NumType>>::type
    computeIsotropicFunctionDerivative(
        const Function& f,
        const FunctionDerivative& df,
        const StensorType& s,
        const typename StensorTraits<StensorType>::NumType eps,
        const bool b) {
      return s.computeIsotropicFunctionDerivative(f, df, eps, b);
    }

    template <typename Function,
              typename FunctionDerivative,
              typename StensorType>
    typename std::enable_if<
        tfel::meta::Implements<StensorType, StensorConcept>::cond,
        std::pair<stensor<StensorTraits<StensorType>::dime,
                          typename StensorTraits<StensorType>::NumType>,
                  st2tost2<StensorTraits<StensorType>::dime,
                           typename StensorTraits<StensorType>::NumType>>>::type
    computeIsotropicFunctionDerivative(
        const Function& f,
        const FunctionDerivative& df,
        const StensorType& s,
        const typename StensorTraits<StensorType>::NumType eps,
        const bool b) {
      return s.computeIsotropicFunctionDerivative(f, df, eps, b);
    }

    template <typename T, typename T2>
    typename std::enable_if<
        ((tfel::meta::Implements<T, StensorConcept>::cond) &&
         (StensorTraits<T>::dime == 1u) &&
         (tfel::meta::Implements<T2, StensorConcept>::cond) &&
         (StensorTraits<T2>::dime == 1u) &&
         (tfel::typetraits::IsFundamentalNumericType<
             typename StensorTraits<T2>::NumType>::cond)),
        stensor<1u, typename StensorTraits<T>::NumType>>::type
    convertCorotationnalCauchyStressToSecondPiolaKirchhoffStress(const T& s,
                                                                 const T2& U) {
      const auto J = det(U);
      return {J * s[0] / (U[0] * U[0]), J * s[1] / (U[1] * U[1]),
              J * s[2] / (U[2] * U[2])};
    }

    template <typename T, typename T2>
    typename std::enable_if<
        ((tfel::meta::Implements<T, StensorConcept>::cond) &&
         (StensorTraits<T>::dime == 2u) &&
         (tfel::meta::Implements<T2, StensorConcept>::cond) &&
         (StensorTraits<T2>::dime == 2u) &&
         (tfel::typetraits::IsFundamentalNumericType<
             typename StensorTraits<T2>::NumType>::cond)),
        stensor<2u, typename StensorTraits<T>::NumType>>::type
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
    typename std::enable_if<
        ((tfel::meta::Implements<T, StensorConcept>::cond) &&
         (StensorTraits<T>::dime == 3u) &&
         (tfel::meta::Implements<T2, StensorConcept>::cond) &&
         (StensorTraits<T2>::dime == 3u) &&
         (tfel::typetraits::IsFundamentalNumericType<
             typename StensorTraits<T2>::NumType>::cond)),
        stensor<3u, typename StensorTraits<T>::NumType>>::type
    convertCorotationnalCauchyStressToSecondPiolaKirchhoffStress(const T& s,
                                                                 const T2& U) {
      typedef typename tfel::typetraits::BaseType<
          typename StensorTraits<T>::NumType>::type real;
      constexpr real cste = Cste<real>::sqrt2;
      const auto J = det(U);
      const auto iU = invert(U);
      return {
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

    template <typename T, typename T2>
    typename std::enable_if<
        ((tfel::meta::Implements<T, StensorConcept>::cond) &&
         (StensorTraits<T>::dime == 1u) &&
         (tfel::meta::Implements<T2, StensorConcept>::cond) &&
         (StensorTraits<T2>::dime == 1u) &&
         (tfel::typetraits::IsFundamentalNumericType<
             typename StensorTraits<T2>::NumType>::cond)),
        stensor<1u, typename StensorTraits<T>::NumType>>::type
    convertSecondPiolaKirchhoffStressToCorotationnalCauchyStress(const T& S,
                                                                 const T2& U) {
      const auto iJ = 1 / det(U);
      return {iJ * U[0] * S[0] * U[0], iJ * U[1] * S[1] * U[1],
              iJ * U[2] * S[2] * U[2]};
    }

    template <typename T, typename T2>
    typename std::enable_if<
        ((tfel::meta::Implements<T, StensorConcept>::cond) &&
         (StensorTraits<T>::dime == 2u) &&
         (tfel::meta::Implements<T2, StensorConcept>::cond) &&
         (StensorTraits<T2>::dime == 2u) &&
         (tfel::typetraits::IsFundamentalNumericType<
             typename StensorTraits<T2>::NumType>::cond)),
        stensor<2u, typename StensorTraits<T>::NumType>>::type
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
    typename std::enable_if<
        ((tfel::meta::Implements<T, StensorConcept>::cond) &&
         (StensorTraits<T>::dime == 3u) &&
         (tfel::meta::Implements<T2, StensorConcept>::cond) &&
         (StensorTraits<T2>::dime == 3u) &&
         (tfel::typetraits::IsFundamentalNumericType<
             typename StensorTraits<T2>::NumType>::cond)),
        stensor<3u, typename StensorTraits<T>::NumType>>::type
    convertSecondPiolaKirchhoffStressToCorotationnalCauchyStress(const T& S,
                                                                 const T2& U) {
      typedef typename tfel::typetraits::BaseType<
          typename StensorTraits<T>::NumType>::type real;
      constexpr real cste = Cste<real>::sqrt2;
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
    typename std::enable_if<
        ((tfel::meta::Implements<StensorType1, StensorConcept>::cond) &&
         (tfel::meta::Implements<StensorType2, StensorConcept>::cond) &&
         (StensorTraits<StensorType1>::dime == 1u) &&
         (StensorTraits<StensorType2>::dime == 1u)),
        stensor<
            3u,
            typename ResultType<typename StensorTraits<StensorType1>::NumType,
                                typename StensorTraits<StensorType2>::NumType,
                                OpMult>::type>>::type
    symmetric_product(const StensorType1& s1, const StensorType2& s2) {
      return {2 * s1[0] * s2[0], 2 * s1[1] * s2[1], 2 * s1[2] * s2[2]};
    }
    template <typename StensorType1, typename StensorType2>
    typename std::enable_if<
        ((tfel::meta::Implements<StensorType1, StensorConcept>::cond) &&
         (tfel::meta::Implements<StensorType2, StensorConcept>::cond) &&
         (StensorTraits<StensorType1>::dime == 2u) &&
         (StensorTraits<StensorType2>::dime == 2u)),
        stensor<
            2u,
            typename ResultType<typename StensorTraits<StensorType1>::NumType,
                                typename StensorTraits<StensorType2>::NumType,
                                OpMult>::type>>::type
    symmetric_product(const StensorType1& s1, const StensorType2& s2) {
      return {2 * s1[0] * s2[0] + s1[3] * s2[3],
              2 * s1[1] * s2[1] + s1[3] * s2[3], 2 * s1[2] * s2[2],
              (s1[1] + s1[0]) * s2[3] + s1[3] * s2[1] + s1[3] * s2[0]};
    }
    template <typename StensorType1, typename StensorType2>
    typename std::enable_if<
        ((tfel::meta::Implements<StensorType1, StensorConcept>::cond) &&
         (tfel::meta::Implements<StensorType2, StensorConcept>::cond) &&
         (StensorTraits<StensorType1>::dime == 3u) &&
         (StensorTraits<StensorType2>::dime == 3u)),
        stensor<
            3u,
            typename ResultType<typename StensorTraits<StensorType1>::NumType,
                                typename StensorTraits<StensorType2>::NumType,
                                OpMult>::type>>::type
    symmetric_product(const StensorType1& s1, const StensorType2& s2) {
      using res =
          typename ResultType<typename StensorTraits<StensorType1>::NumType,
                              typename StensorTraits<StensorType2>::NumType,
                              OpMult>::type;
      constexpr const auto icste = Cste<res>::isqrt2;
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

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_STENSOR_IXX */
