/*!
 * \file   include/TFEL/Math/Stensor/Internals/StensorEigenSolver.hxx
 * \brief
 * \author Thomas Helfer
 * \date   29 déc. 2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_INTERNALS_STENSOREIGENSOLVER_HXX
#define LIB_TFEL_MATH_INTERNALS_STENSOREIGENSOLVER_HXX

#include "TFEL/TypeTraits/BaseType.hxx"
#include "TFEL/Math/Stensor/Internals/FSESSymmetricEigenSolver.hxx"
#include "TFEL/Math/Stensor/Internals/GteSymmetricEigenSolver.hxx"
#include "TFEL/Math/Stensor/Internals/StensorComputeEigenValues.hxx"
#include "TFEL/Math/Stensor/Internals/StensorComputeEigenVectors.hxx"
#include "FSES/syevj3.hxx"
#include "FSES/syevq3.hxx"
#include "FSES/syevd3.hxx"
#include "FSES/syevh3.hxx"

namespace tfel {

  namespace math {

    namespace internals {

      /*!
       * A class for interfacing various eigen solvers. By default,
       * the TFELEIGENSOLVER solver is used
       * \tparam es:      eigen solver
       * \tparam N:       space dimension
       * \tparam NumType: numeric type
       */
      template <stensor_common::EigenSolver es,
                unsigned short N,
                typename NumType>
      struct StensorEigenSolver;

      template <stensor_common::EigenSolver, unsigned short N, typename NumType>
      struct StensorEigenSolver {
        //! base type
        using base = tfel::typetraits::base_type<NumType>;
        /*!
         * \param[out] vp0: first  eigen value
         * \param[out] vp1: second eigen value
         * \param[out] vp2: third  eigen value
         * \param[in]  v:   stensor values
         * \param[in]  b:   refine eigenvalues
         */
        TFEL_MATH_INLINE static void computeEigenValues(NumType& vp0,
                                                        NumType& vp1,
                                                        NumType& vp2,
                                                        const NumType* const v,
                                                        const bool b) {
          using namespace tfel::typetraits;
          TFEL_STATIC_ASSERT(IsFundamentalNumericType<base>::cond);
          TFEL_STATIC_ASSERT(IsReal<base>::cond);
          TFEL_STATIC_ASSERT(
              (IsSafelyReinterpretCastableTo<NumType, base>::cond));
          StensorComputeEigenValues<N>::exe(
              reinterpret_cast<const base* const>(v),
              reinterpret_cast<base&>(vp0), reinterpret_cast<base&>(vp1),
              reinterpret_cast<base&>(vp2), b);
        }  // end of computeEigenValues
        /*!
         * \param[out] vp: eigen values
         * \param[out] m:  eigen vectors
         * \param[in]  v:  stensor values
         * \param[in]  b:  refine eigenvalues
         */
        TFEL_MATH_INLINE static void computeEigenVectors(
            tfel::math::tvector<3u, NumType>& vp,
            tfel::math::tmatrix<3u, 3u, base>& vec,
            const NumType* const v,
            const bool b) {
          using namespace tfel::typetraits;
          TFEL_STATIC_ASSERT(IsFundamentalNumericType<base>::cond);
          TFEL_STATIC_ASSERT(IsReal<base>::cond);
          TFEL_STATIC_ASSERT(
              (IsSafelyReinterpretCastableTo<NumType, base>::cond));
          StensorComputeEigenVectors<N>::exe(
              reinterpret_cast<const base*>(v),
              reinterpret_cast<tvector<3u, base>&>(vp), vec, b);
        }  // end of computeEigenVectors
      };

      /*!
       * \brief Partial specialisation of the `StensorEigenSolver`
       * class for the GTESYMMETRICQREIGENSOLVER solver in 3D
       * \tparam N:       space dimension
       * \tparam NumType: numeric type
       */
      template <typename NumType>
      struct StensorEigenSolver<stensor_common::GTESYMMETRICQREIGENSOLVER,
                                3u,
                                NumType> {
        //! base type
        using base = tfel::typetraits::base_type<NumType>;
        /*!
         * \param[out] vp0: first  eigen value
         * \param[out] vp1: second eigen value
         * \param[out] vp2: third  eigen value
         * \param[in]  v:   stensor values
         * \param[in]  b:   refine eigenvalues
         */
        TFEL_MATH_INLINE static void computeEigenValues(NumType& vp0,
                                                        NumType& vp1,
                                                        NumType& vp2,
                                                        const NumType* const v,
                                                        const bool b) {
          using namespace tfel::typetraits;
          constexpr const auto icste = tfel::math::Cste<base>::isqrt2;
          TFEL_STATIC_ASSERT(IsFundamentalNumericType<base>::cond);
          TFEL_STATIC_ASSERT(IsReal<base>::cond);
          TFEL_STATIC_ASSERT(
              (IsSafelyReinterpretCastableTo<NumType, base>::cond));
          auto vp = tfel::math::tvector<3u, base>{};
          auto m = tfel::math::tmatrix<3u, 3u, base>{};
          const auto s = reinterpret_cast<const base*>(v);
          GteSymmetricEigensolver3x3<base>::exe(vp, m, s[0], s[3] * icste,
                                                s[4] * icste, s[1],
                                                s[5] * icste, s[2], b);
          vp0 = NumType(vp[0]);
          vp1 = NumType(vp[1]);
          vp2 = NumType(vp[2]);
        }  // end of computeEigenValues
        /*!
         * \param[out] vp: eigen values
         * \param[out] m:  eigen vectors
         * \param[in]  v:  stensor values
         * \param[in]  b:  refine eigenvalues
         */
        TFEL_MATH_INLINE static void computeEigenVectors(
            tfel::math::tvector<3u, NumType>& vp,
            tfel::math::tmatrix<3u, 3u, base>& m,
            const NumType* const v,
            const bool b) {
          using namespace tfel::typetraits;
          constexpr const auto icste = tfel::math::Cste<base>::isqrt2;
          TFEL_STATIC_ASSERT(IsFundamentalNumericType<base>::cond);
          TFEL_STATIC_ASSERT(IsReal<base>::cond);
          TFEL_STATIC_ASSERT(
              (IsSafelyReinterpretCastableTo<NumType, base>::cond));
          auto vp2 = tfel::math::tvector<3u, base>{};
          const auto s = reinterpret_cast<const base*>(v);
          GteSymmetricEigensolver3x3<base>::exe(vp2, m, s[0], s[3] * icste,
                                                s[4] * icste, s[1],
                                                s[5] * icste, s[2], b);
          vp = {NumType(vp2(0)), NumType(vp2(1)), NumType(vp2(2))};
        }  // end of computeEigenVectors
      };   // end of struct StensorEigenSolver

      /*!
       * \brief Partial specialisation of the `StensorEigenSolver`
       * class for the FSESANALYTICALEIGENSOLVER solver in 2D
       * \tparam NumType: numeric type
       */
      template <typename NumType>
      struct StensorEigenSolver<stensor_common::FSESANALYTICALEIGENSOLVER,
                                2u,
                                NumType> {
        //! base type
        using base = tfel::typetraits::base_type<NumType>;
        /*!
         * \param[out] vp0: first  eigen value
         * \param[out] vp1: second eigen value
         * \param[out] vp2: third  eigen value
         * \param[in]  v:   stensor values
         * \param[in]  b:   refine eigenvalues
         */
        TFEL_MATH_INLINE static void computeEigenValues(NumType& vp0,
                                                        NumType& vp1,
                                                        NumType& vp2,
                                                        const NumType* const v,
                                                        const bool) {
          using namespace tfel::typetraits;
          constexpr const auto icste = tfel::math::Cste<base>::isqrt2;
          TFEL_STATIC_ASSERT(IsFundamentalNumericType<base>::cond);
          TFEL_STATIC_ASSERT(IsReal<base>::cond);
          TFEL_STATIC_ASSERT(
              (IsSafelyReinterpretCastableTo<NumType, base>::cond));
          auto vp = tfel::math::tvector<3u, base>{};
          const auto s = reinterpret_cast<const base*>(v);
          FSESAnalyticalSymmetricEigensolver2x2<base>::computeEigenValues(
              vp, s[0], s[3] * icste, s[1]);
          vp0 = NumType(vp[0]);
          vp1 = NumType(vp[1]);
          vp2 = NumType(v[2]);
        }  // end of computeEigenValues
        /*!
         * \param[out] vp: eigen values
         * \param[out] m:  eigen vectors
         * \param[in]  v:  stensor values
         * \param[in]  b:  refine eigenvalues
         */
        TFEL_MATH_INLINE static void computeEigenVectors(
            tfel::math::tvector<3u, NumType>& vp,
            tfel::math::tmatrix<3u, 3u, base>& m,
            const NumType* const v,
            const bool) {
          using namespace tfel::typetraits;
          constexpr const auto icste = tfel::math::Cste<base>::isqrt2;
          TFEL_STATIC_ASSERT(IsFundamentalNumericType<base>::cond);
          TFEL_STATIC_ASSERT(IsReal<base>::cond);
          TFEL_STATIC_ASSERT(
              (IsSafelyReinterpretCastableTo<NumType, base>::cond));
          auto vp2 = tfel::math::tvector<3u, base>{};
          const auto s = reinterpret_cast<const base*>(v);
          FSESAnalyticalSymmetricEigensolver2x2<base>::computeEigenVectors(
              vp2, m, s[0], s[3] * icste, s[1]);
          vp = {NumType(vp2(0)), NumType(vp2(1)), v[2]};
          m(2, 0) = base(0);
          m(2, 1) = base(0);
          m(0, 2) = base(0);
          m(1, 2) = base(0);
          m(2, 2) = base(1);
        }  // end of computeEigenVectors
      };   // end of struct StensorEigenSolver

      /*!
       * \brief Partial specialisation of the `StensorEigenSolver`
       * class for the FSESANALYTICALEIGENSOLVER solver in 3D
       * \tparam NumType: numeric type
       */
      template <typename NumType>
      struct StensorEigenSolver<stensor_common::FSESANALYTICALEIGENSOLVER,
                                3u,
                                NumType> {
        //! base type
        using base = tfel::typetraits::base_type<NumType>;
        /*!
         * \param[out] vp0: first  eigen value
         * \param[out] vp1: second eigen value
         * \param[out] vp2: third  eigen value
         * \param[in]  v:   stensor values
         * \param[in]  b:   refine eigenvalues
         */
        TFEL_MATH_INLINE static void computeEigenValues(NumType& vp0,
                                                        NumType& vp1,
                                                        NumType& vp2,
                                                        const NumType* const v,
                                                        const bool) {
          using namespace tfel::typetraits;
          constexpr const auto icste = tfel::math::Cste<base>::isqrt2;
          TFEL_STATIC_ASSERT(IsFundamentalNumericType<base>::cond);
          TFEL_STATIC_ASSERT(IsReal<base>::cond);
          TFEL_STATIC_ASSERT(
              (IsSafelyReinterpretCastableTo<NumType, base>::cond));
          auto vp = tfel::math::tvector<3u, base>{};
          const auto s = reinterpret_cast<const base*>(v);
          const auto sm = tmatrix<3u, 3u, base>{
              s[0],         s[3] * icste, s[4] * icste, s[3] * icste, s[1],
              s[5] * icste, s[4] * icste, s[5] * icste, s[2]};
          FSESAnalyticalSymmetricEigensolver3x3<base>::computeEigenValues(vp,
                                                                          sm);
          vp0 = NumType(vp[0]);
          vp1 = NumType(vp[1]);
          vp2 = NumType(vp[2]);
        }  // end of computeEigenValues
        /*!
         * \param[out] vp: eigen values
         * \param[out] m:  eigen vectors
         * \param[in]  v:  stensor values
         * \param[in]  b:  refine eigenvalues
         */
        TFEL_MATH_INLINE static void computeEigenVectors(
            tfel::math::tvector<3u, NumType>& vp,
            tfel::math::tmatrix<3u, 3u, base>& m,
            const NumType* const v,
            const bool) {
          using namespace tfel::typetraits;
          constexpr const auto icste = tfel::math::Cste<base>::isqrt2;
          TFEL_STATIC_ASSERT(IsFundamentalNumericType<base>::cond);
          TFEL_STATIC_ASSERT(IsReal<base>::cond);
          TFEL_STATIC_ASSERT(
              (IsSafelyReinterpretCastableTo<NumType, base>::cond));
          auto vp2 = tfel::math::tvector<3u, base>{};
          const auto s = reinterpret_cast<const base*>(v);
          auto sm = tmatrix<3u, 3u, base>{
              s[0],         s[3] * icste, s[4] * icste, s[3] * icste, s[1],
              s[5] * icste, s[4] * icste, s[5] * icste, s[2]};
          FSESAnalyticalSymmetricEigensolver3x3<base>::computeEigenVectors(
              vp2, m, sm);
          vp = {NumType(vp2(0)), NumType(vp2(1)), NumType(vp2(2))};
        }  // end of computeEigenVectors
      };   // end of struct StensorEigenSolver

      /*!
       * \brief Partial specialisation of the `StensorEigenSolver`
       * class for the FSESJACOBIEIGENSOLVER solver in 2D
       * \tparam NumType: numeric type
       */
      template <typename NumType>
      struct StensorEigenSolver<stensor_common::FSESJACOBIEIGENSOLVER,
                                2u,
                                NumType>
          : public StensorEigenSolver<stensor_common::FSESANALYTICALEIGENSOLVER,
                                      2u,
                                      NumType> {
      };  // end of
          // StensorEigenSolver<stensor_common::FSESJACOBIEIGENSOLVER,2u,NumType>

      /*!
       * \brief Partial specialisation of the `StensorEigenSolver`
       * class for the FSESJACOBIEIGENSOLVER solver in 3D
       * \tparam NumType: numeric type
       */
      template <typename NumType>
      struct StensorEigenSolver<stensor_common::FSESJACOBIEIGENSOLVER,
                                3u,
                                NumType> {
        //! base type
        using base = tfel::typetraits::base_type<NumType>;
        /*!
         * \param[out] vp0: first  eigen value
         * \param[out] vp1: second eigen value
         * \param[out] vp2: third  eigen value
         * \param[in]  v:   stensor values
         * \param[in]  b:   refine eigenvalues
         */
        TFEL_MATH_INLINE static void computeEigenValues(NumType& vp0,
                                                        NumType& vp1,
                                                        NumType& vp2,
                                                        const NumType* const v,
                                                        const bool) {
          using namespace tfel::typetraits;
          constexpr const auto icste = tfel::math::Cste<base>::isqrt2;
          TFEL_STATIC_ASSERT(IsFundamentalNumericType<base>::cond);
          TFEL_STATIC_ASSERT(IsReal<base>::cond);
          TFEL_STATIC_ASSERT(
              (IsSafelyReinterpretCastableTo<NumType, base>::cond));
          auto vp = tfel::math::tvector<3u, base>{};
          const auto s = reinterpret_cast<const base*>(v);
          auto sm = tmatrix<3u, 3u, base>{
              s[0],         s[3] * icste, s[4] * icste, s[3] * icste, s[1],
              s[5] * icste, s[4] * icste, s[5] * icste, s[2]};
          auto m = tmatrix<3u, 3u>{};
          fses::syevj3(m, vp, sm);
          vp0 = NumType(vp[0]);
          vp1 = NumType(vp[1]);
          vp2 = NumType(vp[2]);
        }  // end of computeEigenValues
        /*!
         * \param[out] vp: eigen values
         * \param[out] m:  eigen vectors
         * \param[in]  v:  stensor values
         * \param[in]  b:  refine eigenvalues
         */
        TFEL_MATH_INLINE static void computeEigenVectors(
            tfel::math::tvector<3u, NumType>& vp,
            tfel::math::tmatrix<3u, 3u, base>& m,
            const NumType* const v,
            const bool) {
          using namespace tfel::typetraits;
          constexpr const auto icste = tfel::math::Cste<base>::isqrt2;
          TFEL_STATIC_ASSERT(IsFundamentalNumericType<base>::cond);
          TFEL_STATIC_ASSERT(IsReal<base>::cond);
          TFEL_STATIC_ASSERT(
              (IsSafelyReinterpretCastableTo<NumType, base>::cond));
          auto vp2 = tfel::math::tvector<3u, base>{};
          const auto s = reinterpret_cast<const base*>(v);
          auto sm = tmatrix<3u, 3u, base>{
              s[0],         s[3] * icste, s[4] * icste, s[3] * icste, s[1],
              s[5] * icste, s[4] * icste, s[5] * icste, s[2]};
          fses::syevj3(m, vp2, sm);
          vp = {NumType(vp2(0)), NumType(vp2(1)), NumType(vp2(2))};
        }  // end of computeEigenVectors
      };   // end of struct StensorEigenSolver

      /*!
       * \brief Partial specialisation of the `StensorEigenSolver`
       * class for the FSESQLEIGENSOLVER solver in 2D
       * \tparam NumType: numeric type
       */
      template <typename NumType>
      struct StensorEigenSolver<stensor_common::FSESQLEIGENSOLVER, 2u, NumType>
          : public StensorEigenSolver<stensor_common::FSESANALYTICALEIGENSOLVER,
                                      2u,
                                      NumType> {
      };  // end of
          // StensorEigenSolver<stensor_common::FSESQLEIGENSOLVER,2u,NumType>

      /*!
       * \brief Partial specialisation of the `StensorEigenSolver`
       * class for the FSESQLEIGENSOLVER solver in 3D
       * \tparam NumType: numeric type
       */
      template <typename NumType>
      struct StensorEigenSolver<stensor_common::FSESQLEIGENSOLVER,
                                3u,
                                NumType> {
        //! base type
        using base = tfel::typetraits::base_type<NumType>;
        /*!
         * \param[out] vp0: first  eigen value
         * \param[out] vp1: second eigen value
         * \param[out] vp2: third  eigen value
         * \param[in]  v:   stensor values
         * \param[in]  b:   refine eigenvalues
         */
        TFEL_MATH_INLINE static void computeEigenValues(NumType& vp0,
                                                        NumType& vp1,
                                                        NumType& vp2,
                                                        const NumType* const v,
                                                        const bool) {
          using namespace tfel::typetraits;
          constexpr const auto icste = tfel::math::Cste<base>::isqrt2;
          TFEL_STATIC_ASSERT(IsFundamentalNumericType<base>::cond);
          TFEL_STATIC_ASSERT(IsReal<base>::cond);
          TFEL_STATIC_ASSERT(
              (IsSafelyReinterpretCastableTo<NumType, base>::cond));
          auto vp = tfel::math::tvector<3u, base>{};
          const auto s = reinterpret_cast<const base*>(v);
          const auto sm = tmatrix<3u, 3u, base>{
              s[0],         s[3] * icste, s[4] * icste, s[3] * icste, s[1],
              s[5] * icste, s[4] * icste, s[5] * icste, s[2]};
          auto m = tmatrix<3u, 3u>{};
          fses::syevq3(m, vp, sm);
          vp0 = NumType(vp[0]);
          vp1 = NumType(vp[1]);
          vp2 = NumType(vp[2]);
        }  // end of computeEigenValues
        /*!
         * \param[out] vp: eigen values
         * \param[out] m:  eigen vectors
         * \param[in]  v:  stensor values
         * \param[in]  b:  refine eigenvalues
         */
        TFEL_MATH_INLINE static void computeEigenVectors(
            tfel::math::tvector<3u, NumType>& vp,
            tfel::math::tmatrix<3u, 3u, base>& m,
            const NumType* const v,
            const bool) {
          using namespace tfel::typetraits;
          constexpr const auto icste = tfel::math::Cste<base>::isqrt2;
          TFEL_STATIC_ASSERT(IsFundamentalNumericType<base>::cond);
          TFEL_STATIC_ASSERT(IsReal<base>::cond);
          TFEL_STATIC_ASSERT(
              (IsSafelyReinterpretCastableTo<NumType, base>::cond));
          auto vp2 = tfel::math::tvector<3u, base>{};
          const auto s = reinterpret_cast<const base*>(v);
          auto sm = tmatrix<3u, 3u, base>{
              s[0],         s[3] * icste, s[4] * icste, s[3] * icste, s[1],
              s[5] * icste, s[4] * icste, s[5] * icste, s[2]};
          fses::syevq3(m, vp2, sm);
          vp = {NumType(vp2(0)), NumType(vp2(1)), NumType(vp2(2))};
        }  // end of computeEigenVectors
      };   // end of struct StensorEigenSolver

      /*!
       * \brief Partial specialisation of the `StensorEigenSolver`
       * class for the FSESCUPPENEIGENSOLVER solver in 2D
       * \tparam NumType: numeric type
       */
      template <typename NumType>
      struct StensorEigenSolver<stensor_common::FSESCUPPENEIGENSOLVER,
                                2u,
                                NumType>
          : public StensorEigenSolver<stensor_common::FSESANALYTICALEIGENSOLVER,
                                      2u,
                                      NumType> {
      };  // end of
          // StensorEigenSolver<stensor_common::FSESCUPPENEIGENSOLVER,2u,NumType>

      /*!
       * \brief Partial specialisation of the `StensorEigenSolver`
       * class for the FSESCUPPENEIGENSOLVER solver in 3D
       * \tparam NumType: numeric type
       */
      template <typename NumType>
      struct StensorEigenSolver<stensor_common::FSESCUPPENEIGENSOLVER,
                                3u,
                                NumType> {
        //! base type
        using base = tfel::typetraits::base_type<NumType>;
        /*!
         * \param[out] vp0: first  eigen value
         * \param[out] vp1: second eigen value
         * \param[out] vp2: third  eigen value
         * \param[in]  v:   stensor values
         * \param[in]  b:   refine eigenvalues
         */
        TFEL_MATH_INLINE static void computeEigenValues(NumType& vp0,
                                                        NumType& vp1,
                                                        NumType& vp2,
                                                        const NumType* const v,
                                                        const bool) {
          using namespace tfel::typetraits;
          constexpr const auto icste = tfel::math::Cste<base>::isqrt2;
          TFEL_STATIC_ASSERT(IsFundamentalNumericType<base>::cond);
          TFEL_STATIC_ASSERT(IsReal<base>::cond);
          TFEL_STATIC_ASSERT(
              (IsSafelyReinterpretCastableTo<NumType, base>::cond));
          auto vp = tfel::math::tvector<3u, base>{};
          const auto s = reinterpret_cast<const base*>(v);
          const auto sm = tmatrix<3u, 3u, base>{
              s[0],         s[3] * icste, s[4] * icste, s[3] * icste, s[1],
              s[5] * icste, s[4] * icste, s[5] * icste, s[2]};
          auto m = tmatrix<3u, 3u>{};
          fses::syevd3(m, vp, sm);
          vp0 = NumType(vp[0]);
          vp1 = NumType(vp[1]);
          vp2 = NumType(vp[2]);
        }  // end of computeEigenValues
        /*!
         * \param[out] vp: eigen values
         * \param[out] m:  eigen vectors
         * \param[in]  v:  stensor values
         * \param[in]  b:  refine eigenvalues
         */
        TFEL_MATH_INLINE static void computeEigenVectors(
            tfel::math::tvector<3u, NumType>& vp,
            tfel::math::tmatrix<3u, 3u, base>& m,
            const NumType* const v,
            const bool) {
          using namespace tfel::typetraits;
          constexpr const auto icste = tfel::math::Cste<base>::isqrt2;
          TFEL_STATIC_ASSERT(IsFundamentalNumericType<base>::cond);
          TFEL_STATIC_ASSERT(IsReal<base>::cond);
          TFEL_STATIC_ASSERT(
              (IsSafelyReinterpretCastableTo<NumType, base>::cond));
          auto vp2 = tfel::math::tvector<3u, base>{};
          const auto s = reinterpret_cast<const base*>(v);
          auto sm = tmatrix<3u, 3u, base>{
              s[0],         s[3] * icste, s[4] * icste, s[3] * icste, s[1],
              s[5] * icste, s[4] * icste, s[5] * icste, s[2]};
          fses::syevd3(m, vp2, sm);
          vp = {NumType(vp2(0)), NumType(vp2(1)), NumType(vp2(2))};
        }  // end of computeEigenVectors
      };   // end of struct StensorEigenSolver

      /*!
       * \brief Partial specialisation of the `StensorEigenSolver`
       * class for the FSESHYBRIDEIGENSOLVER solver in 2D
       * \tparam NumType: numeric type
       */
      template <typename NumType>
      struct StensorEigenSolver<stensor_common::FSESHYBRIDEIGENSOLVER,
                                2u,
                                NumType>
          : public StensorEigenSolver<stensor_common::FSESANALYTICALEIGENSOLVER,
                                      2u,
                                      NumType> {
      };  // end of
          // StensorEigenSolver<stensor_common::FSESHYBRIDEIGENSOLVER,2u,NumType>

      /*!
       * \brief Partial specialisation of the `StensorEigenSolver`
       * class for the FSESHYBRIDEIGENSOLVER solver in 3D
       * \tparam NumType: numeric type
       */
      template <typename NumType>
      struct StensorEigenSolver<stensor_common::FSESHYBRIDEIGENSOLVER,
                                3u,
                                NumType> {
        //! base type
        using base = tfel::typetraits::base_type<NumType>;
        /*!
         * \param[out] vp0: first  eigen value
         * \param[out] vp1: second eigen value
         * \param[out] vp2: third  eigen value
         * \param[in]  v:   stensor values
         * \param[in]  b:   refine eigenvalues
         */
        TFEL_MATH_INLINE static void computeEigenValues(NumType& vp0,
                                                        NumType& vp1,
                                                        NumType& vp2,
                                                        const NumType* const v,
                                                        const bool) {
          using namespace tfel::typetraits;
          constexpr const auto icste = tfel::math::Cste<base>::isqrt2;
          TFEL_STATIC_ASSERT(IsFundamentalNumericType<base>::cond);
          TFEL_STATIC_ASSERT(IsReal<base>::cond);
          TFEL_STATIC_ASSERT(
              (IsSafelyReinterpretCastableTo<NumType, base>::cond));
          auto vp = tfel::math::tvector<3u, base>{};
          const auto s = reinterpret_cast<const base*>(v);
          const auto sm = tmatrix<3u, 3u, base>{
              s[0],         s[3] * icste, s[4] * icste, s[3] * icste, s[1],
              s[5] * icste, s[4] * icste, s[5] * icste, s[2]};
          fses::syevc3(vp, sm);
          vp0 = NumType(vp[0]);
          vp1 = NumType(vp[1]);
          vp2 = NumType(vp[2]);
        }  // end of computeEigenValues
        /*!
         * \param[out] vp: eigen values
         * \param[out] m:  eigen vectors
         * \param[in]  v:  stensor values
         * \param[in]  b:  refine eigenvalues
         */
        TFEL_MATH_INLINE static void computeEigenVectors(
            tfel::math::tvector<3u, NumType>& vp,
            tfel::math::tmatrix<3u, 3u, base>& m,
            const NumType* const v,
            const bool) {
          using namespace tfel::typetraits;
          constexpr const auto icste = tfel::math::Cste<base>::isqrt2;
          TFEL_STATIC_ASSERT(IsFundamentalNumericType<base>::cond);
          TFEL_STATIC_ASSERT(IsReal<base>::cond);
          TFEL_STATIC_ASSERT(
              (IsSafelyReinterpretCastableTo<NumType, base>::cond));
          auto vp2 = tfel::math::tvector<3u, base>{};
          const auto s = reinterpret_cast<const base*>(v);
          auto sm = tmatrix<3u, 3u, base>{
              s[0],         s[3] * icste, s[4] * icste, s[3] * icste, s[1],
              s[5] * icste, s[4] * icste, s[5] * icste, s[2]};
          fses::syevh3(m, vp2, sm);
          vp = {NumType(vp2(0)), NumType(vp2(1)), NumType(vp2(2))};
        }  // end of computeEigenVectors
      };   // end of struct StensorEigenSolver

    }  // end of namespace internals

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_INTERNALS_STENSOREIGENSOLVER_HXX */
