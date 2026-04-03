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

#include "TFEL/Math/Stensor/Internals/FSESSymmetricEigenSolver.hxx"
#include "TFEL/Math/Stensor/Internals/GteSymmetricEigenSolver.hxx"
#include "TFEL/Math/Stensor/Internals/StensorComputeEigenValues.hxx"
#include "TFEL/Math/Stensor/Internals/StensorComputeEigenVectors.hxx"
#include "TFEL/Math/Stensor/Internals/HarariEigenSolver.hxx"
#include "FSES/syevj3.hxx"
#include "FSES/syevq3.hxx"
#include "FSES/syevd3.hxx"
#include "FSES/syevh3.hxx"

namespace tfel::math::internals {

  /*!
   * A base class for interfacing various eigen solvers.
   * \tparam N:       space dimension
   * \tparam NumType: numeric type
   */
  template <unsigned short N, typename NumType>
  struct StensorEigenSolverBase;

  template <typename NumType>
  struct StensorEigenSolverBase<3u, NumType> {
    static_assert(
        tfel::typetraits::IsFundamentalNumericType<base_type<NumType>>::cond);
    static_assert(tfel::typetraits::IsReal<base_type<NumType>>::cond);
    //! build a matrix of the base type
    static constexpr auto as_base_matrix(const NumType* const v) noexcept {
      constexpr auto icste = Cste<NumType>::isqrt2;
      return tmatrix<3u, 3u, base_type<NumType>>{
          base_type_cast(v[0]),          //
          base_type_cast(v[3]) * icste,  //
          base_type_cast(v[4]) * icste,  //
          base_type_cast(v[3]) * icste,  //
          base_type_cast(v[1]),          //
          base_type_cast(v[5]) * icste,  //
          base_type_cast(v[4]) * icste,  //
          base_type_cast(v[5]) * icste,  //
          base_type_cast(v[2])};
    }  // end of as_base_matrix
  };   // end of StensorEigenSolverBase<3u, NumType>

  /*!
   * A class for interfacing various eigen solvers. By default,
   * the TFELEIGENSOLVER solver is used
   * \tparam es:      eigen solver
   * \tparam N:       space dimension
   * \tparam NumType: numeric type
   */
  template <stensor_common::EigenSolver es, unsigned short N, typename NumType>
  struct StensorEigenSolver;

  template <stensor_common::EigenSolver, unsigned short N, typename NumType>
  struct StensorEigenSolver {
    //! base type
    using real = base_type<NumType>;
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
      real values[StensorDimeToSize<N>::value];
      real eigen_values[3];
      tfel::fsalgo::transform<StensorDimeToSize<N>::value>::exe(
          v, values, [](const auto& value) { return base_type_cast(value); });
      StensorComputeEigenValues<N>::exe(values, eigen_values[0],
                                        eigen_values[1], eigen_values[2], b);
      vp0 = NumType(eigen_values[0]);
      vp1 = NumType(eigen_values[1]);
      vp2 = NumType(eigen_values[2]);
    }  // end of computeEigenValues
    /*!
     * \param[out] vp: eigen values
     * \param[out] m:  eigen vectors
     * \param[in]  v:  stensor values
     * \param[in]  b:  refine eigenvalues
     */
    TFEL_MATH_INLINE static void computeEigenVectors(tvector<3u, NumType>& vp,
                                                     tmatrix<3u, 3u, real>& vec,
                                                     const NumType* const v,
                                                     const bool b) {
      real values[StensorDimeToSize<N>::value];
      tvector<3u, real> eigen_values;
      tfel::fsalgo::transform<StensorDimeToSize<N>::value>::exe(
          v, values, [](const auto& value) { return base_type_cast(value); });
      StensorComputeEigenVectors<N>::exe(values, eigen_values, vec, b);
      tfel::fsalgo::transform<3u>::exe(
          eigen_values.cbegin(), vp.begin(),
          [](const auto& value) { return NumType(value); });
    }  // end of computeEigenVectors
  };

  /*!
   * \brief Partial specialisation of the `StensorEigenSolver`
   * class for the HARARIEIGENSOLVER solver in 3D
   * \tparam NumType: numeric type
   */
  template <typename NumType>
  struct StensorEigenSolver<stensor_common::HARARIEIGENSOLVER, 3u, NumType>
      : StensorEigenSolverBase<3u, NumType> {
    //! base type
    using real = base_type<NumType>;
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
      auto sm = StensorEigenSolverBase<3u, NumType>::as_base_matrix(v);
      auto vp = tvector<3u, real>{};
      HarariEigensolver3x3<real>::computeEigenValues(
          vp, sm(0, 0), sm(1, 1), sm(2, 2), sm(0, 1), sm(0, 2), sm(1, 2));
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
    TFEL_MATH_INLINE static void computeEigenVectors(tvector<3u, NumType>& vp,
                                                     tmatrix<3u, 3u, real>& m,
                                                     const NumType* const v,
                                                     const bool) {
      auto sm = StensorEigenSolverBase<3u, NumType>::as_base_matrix(v);
      auto vp2 = tvector<3u, real>{};
      HarariEigensolver3x3<real>::computeEigenVectors(
          vp2, m, sm(0, 0), sm(1, 1), sm(2, 2), sm(0, 1), sm(0, 2), sm(1, 2));
      vp = {NumType(vp2(0)), NumType(vp2(1)), NumType(vp2(2))};
    }  // end of computeEigenVectors
  };   // end of struct StensorEigenSolver

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
    using real = base_type<NumType>;
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
      constexpr auto icste = Cste<real>::isqrt2;
      auto vp = tvector<3u, real>{};
      auto m = tmatrix<3u, 3u, real>{};
      GteSymmetricEigensolver3x3<real>::exe(
          vp, m, base_type_cast(v[0]), base_type_cast(v[3]) * icste,
          base_type_cast(v[4]) * icste, base_type_cast(v[1]),
          base_type_cast(v[5]) * icste, base_type_cast(v[2]), b);
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
    TFEL_MATH_INLINE static void computeEigenVectors(tvector<3u, NumType>& vp,
                                                     tmatrix<3u, 3u, real>& m,
                                                     const NumType* const v,
                                                     const bool b) {
      constexpr auto icste = Cste<real>::isqrt2;
      auto vp2 = tvector<3u, real>{};
      GteSymmetricEigensolver3x3<real>::exe(
          vp2, m, base_type_cast(v[0]), base_type_cast(v[3]) * icste,
          base_type_cast(v[4]) * icste, base_type_cast(v[1]),
          base_type_cast(v[5]) * icste, base_type_cast(v[2]), b);
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
    using real = base_type<NumType>;
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
      constexpr auto icste = Cste<real>::isqrt2;
      auto vp = tvector<3u, real>{};
      FSESAnalyticalSymmetricEigensolver2x2<real>::computeEigenValues(
          vp, base_type_cast(v[0]), base_type_cast(v[3]) * icste,
          base_type_cast(v[1]));
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
    TFEL_MATH_INLINE static void computeEigenVectors(tvector<3u, NumType>& vp,
                                                     tmatrix<3u, 3u, real>& m,
                                                     const NumType* const v,
                                                     const bool) {
      constexpr auto icste = Cste<real>::isqrt2;
      auto vp2 = tvector<3u, real>{};
      FSESAnalyticalSymmetricEigensolver2x2<real>::computeEigenVectors(
          vp2, m, base_type_cast(v[0]), base_type_cast(v[3]) * icste,
          base_type_cast(v[1]));
      vp = {NumType(vp2(0)), NumType(vp2(1)), v[2]};
      m(2, 0) = real(0);
      m(2, 1) = real(0);
      m(0, 2) = real(0);
      m(1, 2) = real(0);
      m(2, 2) = real(1);
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
                            NumType> : StensorEigenSolverBase<3u, NumType> {
    //! base type
    using real = base_type<NumType>;
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
      auto sm = StensorEigenSolverBase<3u, NumType>::as_base_matrix(v);
      auto vp = tvector<3u, real>{};
      FSESAnalyticalSymmetricEigensolver3x3<real>::computeEigenValues(vp, sm);
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
    TFEL_MATH_INLINE static void computeEigenVectors(tvector<3u, NumType>& vp,
                                                     tmatrix<3u, 3u, real>& m,
                                                     const NumType* const v,
                                                     const bool) {
      auto sm = StensorEigenSolverBase<3u, NumType>::as_base_matrix(v);
      auto vp2 = tvector<3u, real>{};
      FSESAnalyticalSymmetricEigensolver3x3<real>::computeEigenVectors(vp2, m,
                                                                       sm);
      vp = {NumType(vp2(0)), NumType(vp2(1)), NumType(vp2(2))};
    }  // end of computeEigenVectors
  };   // end of struct StensorEigenSolver

  /*!
   * \brief Partial specialisation of the `StensorEigenSolver`
   * class for the FSESJACOBIEIGENSOLVER solver in 2D
   * \tparam NumType: numeric type
   */
  template <typename NumType>
  struct StensorEigenSolver<stensor_common::FSESJACOBIEIGENSOLVER, 2u, NumType>
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
  struct StensorEigenSolver<stensor_common::FSESJACOBIEIGENSOLVER, 3u, NumType>
      : StensorEigenSolverBase<3u, NumType> {
    //! base type
    using real = base_type<NumType>;
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
      auto sm = StensorEigenSolverBase<3u, NumType>::as_base_matrix(v);
      auto vp = tvector<3u, real>{};
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
    TFEL_MATH_INLINE static void computeEigenVectors(tvector<3u, NumType>& vp,
                                                     tmatrix<3u, 3u, real>& m,
                                                     const NumType* const v,
                                                     const bool) {
      auto sm = StensorEigenSolverBase<3u, NumType>::as_base_matrix(v);
      auto vp2 = tvector<3u, real>{};
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
  };  // end of StensorEigenSolver<stensor_common::FSESQLEIGENSOLVER,2u,NumType>

  /*!
   * \brief Partial specialisation of the `StensorEigenSolver`
   * class for the FSESQLEIGENSOLVER solver in 3D
   * \tparam NumType: numeric type
   */
  template <typename NumType>
  struct StensorEigenSolver<stensor_common::FSESQLEIGENSOLVER, 3u, NumType>
      : StensorEigenSolverBase<3u, NumType> {
    //! base type
    using real = base_type<NumType>;
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
      auto sm = StensorEigenSolverBase<3u, NumType>::as_base_matrix(v);
      auto vp = tvector<3u, real>{};
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
    TFEL_MATH_INLINE static void computeEigenVectors(tvector<3u, NumType>& vp,
                                                     tmatrix<3u, 3u, real>& m,
                                                     const NumType* const v,
                                                     const bool) {
      auto sm = StensorEigenSolverBase<3u, NumType>::as_base_matrix(v);
      auto vp2 = tvector<3u, real>{};
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
  struct StensorEigenSolver<stensor_common::FSESCUPPENEIGENSOLVER, 2u, NumType>
      : public StensorEigenSolver<stensor_common::FSESANALYTICALEIGENSOLVER,
                                  2u,
                                  NumType> {
  };  // end of StensorEigenSolver<FSESCUPPENEIGENSOLVER,2u,NumType>

  /*!
   * \brief Partial specialisation of the `StensorEigenSolver`
   * class for the FSESCUPPENEIGENSOLVER solver in 3D
   * \tparam NumType: numeric type
   */
  template <typename NumType>
  struct StensorEigenSolver<stensor_common::FSESCUPPENEIGENSOLVER, 3u, NumType>
      : StensorEigenSolverBase<3u, NumType> {
    //! base type
    using real = base_type<NumType>;
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
      auto vp = tvector<3u, real>{};
      auto sm = StensorEigenSolverBase<3u, NumType>::as_base_matrix(v);
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
    TFEL_MATH_INLINE static void computeEigenVectors(tvector<3u, NumType>& vp,
                                                     tmatrix<3u, 3u, real>& m,
                                                     const NumType* const v,
                                                     const bool) {
      auto vp2 = tvector<3u, real>{};
      auto sm = StensorEigenSolverBase<3u, NumType>::as_base_matrix(v);
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
  struct StensorEigenSolver<stensor_common::FSESHYBRIDEIGENSOLVER, 2u, NumType>
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
  struct StensorEigenSolver<stensor_common::FSESHYBRIDEIGENSOLVER, 3u, NumType>
      : StensorEigenSolverBase<3u, NumType> {
    //! base type
    using real = base_type<NumType>;
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
      auto sm = StensorEigenSolverBase<3u, NumType>::as_base_matrix(v);
      auto vp = tvector<3u, real>{};
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
    TFEL_MATH_INLINE static void computeEigenVectors(tvector<3u, NumType>& vp,
                                                     tmatrix<3u, 3u, real>& m,
                                                     const NumType* const v,
                                                     const bool) {
      auto sm = StensorEigenSolverBase<3u, NumType>::as_base_matrix(v);
      auto vp2 = tvector<3u, real>{};
      fses::syevh3(m, vp2, sm);
      vp = {NumType(vp2(0)), NumType(vp2(1)), NumType(vp2(2))};
    }  // end of computeEigenVectors
  };   // end of struct StensorEigenSolver

}  // namespace tfel::math::internals

#endif /* LIB_TFEL_MATH_INTERNALS_STENSOREIGENSOLVER_HXX */
