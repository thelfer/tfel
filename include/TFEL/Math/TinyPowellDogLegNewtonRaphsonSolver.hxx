/*!
 * \file include/TFEL/Math/TinyPowellDogLegNewtonRaphsonSolver.hxx
 * \brief
 * \author Thomas Helfer
 * \date   02/07/2021
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_TINYPOWELLDOGLEGNEWTONRAPHSONSOLVER_HXX
#define LIB_TFEL_MATH_TINYPOWELLDOGLEGNEWTONRAPHSONSOLVER_HXX

#include <type_traits>
#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/tmatrix.hxx"
#include "TFEL/Math/NonLinearSolvers/TinyNonLinearSolverBase.hxx"

namespace tfel::math {

  /*!
   * \brief A class based on the curiously recurring template pattern (CRTP)
   * to solve system of non linear equations using the
   * Newton-Raphson algorithm coupled with the Powell' dog leg algorithm.
   * \tparam N: size of the system of non linear equations.
   * \tparam NumericType: numeric type.
   * \tparam Child: base class.
   *
   * By default, the `Child` class must:
   *
   * 1. Set the values of `epsilon` and `iterMax` data members and the initial
   * guess of the unknowns, i.e. by setting the value of the `zeros` data
   * member.
   * 2. Provide a method called `computeResidual` which must computes
   * the residual, i.e. the data member `fzeros` and the jacobian matrix, i.e.
   * the data member `jacobian` using the current estimate of the solution, i.e.
   * the data member `zeros`.
   */
  template <unsigned short N, typename NumericType, typename Child>
  struct TinyPowellDogLegNewtonRaphsonSolver
      : TinyNonLinearSolverBase<N, NumericType, Child> {
    //
    static_assert(N != 0, "invalid size");
    static_assert(std::is_floating_point_v<NumericType>,
                  "invalid numeric type");
    //! \brief default constructor
    TinyPowellDogLegNewtonRaphsonSolver() = default;
    //! \brief default constructor
    TinyPowellDogLegNewtonRaphsonSolver(
        TinyPowellDogLegNewtonRaphsonSolver&) noexcept = default;
    //! \brief default constructor
    TinyPowellDogLegNewtonRaphsonSolver(
        TinyPowellDogLegNewtonRaphsonSolver&&) noexcept = default;
    //! \brief default constructor
    TinyPowellDogLegNewtonRaphsonSolver& operator=(
        TinyPowellDogLegNewtonRaphsonSolver&&) noexcept = default;
    //! \brief default constructor
    TinyPowellDogLegNewtonRaphsonSolver& operator=(
        const TinyPowellDogLegNewtonRaphsonSolver&) noexcept = default;
    //! \brief destructor
    ~TinyPowellDogLegNewtonRaphsonSolver() noexcept = default;
    //! \brief compute a new correction to the unknowns
    TFEL_HOST_DEVICE bool computeNewCorrection();
    //! \brief size of the trust region
    NumericType powell_dogleg_trust_region_size;
    //! \brief approximation of the invert of the jacobian matrix
    tmatrix<N, N, NumericType> jacobian;
  };

}  // end of namespace tfel::math

#include "TFEL/Math/NonLinearSolvers/TinyPowellDogLegNewtonRaphsonSolver.ixx"

#endif /* LIB_TFEL_MATH_TINYPOWELLDOGLEGNEWTONRAPHSONSOLVER_HXX */
