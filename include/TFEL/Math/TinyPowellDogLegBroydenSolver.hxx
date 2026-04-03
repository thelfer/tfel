/*!
 * \file include/TFEL/Math/TinyPowellDogLegBroydenSolver.hxx
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

#ifndef LIB_TFEL_MATH_TINYPOWELLDOGLEGBROYDENSOLVER_HXX
#define LIB_TFEL_MATH_TINYPOWELLDOGLEGBROYDENSOLVER_HXX

#include <type_traits>
#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/tmatrix.hxx"
#include "TFEL/Math/NonLinearSolvers/TinyNonLinearSolverBase.hxx"

namespace tfel::math {

  /*!
   * \brief class allocating on the stack a workspace usable by the
   * `TinyPowellDogLegBroydenSolver` class.
   * \tparam N: size of the system of non linear equations.
   * \tparam NumericType: numeric type.
   */
  template <unsigned short N, typename NumericType>
  struct StackAllocatedTinyPowellDogLegBroydenSolverWorkspace
      : public StackAllocatedTinyNonLinearSolverWorkspace<N, NumericType> {
    //! \brief approximation of the invert of the jacobian matrix
    tmatrix<N, N, NumericType> jacobian;
    //! \brief residual of the previous iterations
    tvector<N, NumericType> fzeros_1;
  };

  /*!
   * \brief A class based on the curiously recurring template pattern (CRTP)
   * to solve system of non linear equations using the
   * Newton-Raphson algorithm coupled with the Powell' dog leg algorithm.
   * \tparam N: size of the system of non linear equations.
   * \tparam NumericType: numeric type.
   * \tparam Child: base class.
   * \tparam ExternalWorkSpace: class containing data members used by the
   * solver.
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
  template <unsigned short N,
            typename NumericType,
            typename Child,
            template <unsigned short, typename> typename ExternalWorkSpace =
                StackAllocatedTinyPowellDogLegBroydenSolverWorkspace>
  struct TinyPowellDogLegBroydenSolver
      : TinyNonLinearSolverBase<N, NumericType, Child, ExternalWorkSpace> {
    //
    static_assert(N != 0, "invalid size");
    static_assert(std::is_floating_point_v<NumericType>,
                  "invalid numeric type");
    //! \brief default constructor
    TinyPowellDogLegBroydenSolver() = default;
    //! \brief default constructor
    TinyPowellDogLegBroydenSolver(TinyPowellDogLegBroydenSolver&) noexcept =
        default;
    //! \brief default constructor
    TinyPowellDogLegBroydenSolver(TinyPowellDogLegBroydenSolver&&) noexcept =
        default;
    //! \brief default constructor
    TinyPowellDogLegBroydenSolver& operator=(
        TinyPowellDogLegBroydenSolver&&) noexcept = default;
    //! \brief default constructor
    TinyPowellDogLegBroydenSolver& operator=(
        const TinyPowellDogLegBroydenSolver&) noexcept = default;
    //! \brief destructor
    ~TinyPowellDogLegBroydenSolver() noexcept = default;
    //! \brief update the jacobian matrix
    TFEL_HOST_DEVICE void updateOrCheckJacobian();
    //! \brief compute a new correction to the unknowns
    TFEL_HOST_DEVICE bool computeNewCorrection();

   protected:
    //! \brief size of the trust region
    NumericType powell_dogleg_trust_region_size;
  };

}  // end of namespace tfel::math

#include "TFEL/Math/NonLinearSolvers/TinyPowellDogLegBroydenSolver.ixx"

#endif /* LIB_TFEL_MATH_TINYPOWELLDOGLEGBROYDENSOLVER_HXX */
