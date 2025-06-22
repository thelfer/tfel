/*!
 * \file   include/TFEL/Math/TinyNewtonRaphsonSolver.hxx
 *
 * \brief
 * \author Thomas Helfer
 * \date   02 Aug 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_TINYNEWTONRAPHSONSOLVER_HXX
#define LIB_TFEL_MATH_TINYNEWTONRAPHSONSOLVER_HXX

#include <type_traits>
#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/tmatrix.hxx"
#include "TFEL/Math/NonLinearSolvers/TinyNonLinearSolverBase.hxx"

namespace tfel::math {

  /*!
   * \brief class allocating on the stack a workspace usable by the
   * `TinyNewtonRaphsonSolver` class.
   * \tparam N: size of the system of non linear equations.
   * \tparam NumericType: numeric type.
   */
  template <unsigned short N, typename NumericType>
  struct StackAllocatedTinyNewtonRaphsonSolverWorkspace
      : public StackAllocatedTinyNonLinearSolverWorkspace<N, NumericType> {
    //! \brief jacobian matrix
    tmatrix<N, N, NumericType> jacobian;
  };

  /*!
   * \brief A class based on the curiously recurring template pattern (CRTP)
   * to solve system of non linear equations using the Newton-Raphson algorithm.
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
                StackAllocatedTinyNewtonRaphsonSolverWorkspace>
  struct TinyNewtonRaphsonSolver
      : TinyNonLinearSolverBase<N, NumericType, Child, ExternalWorkSpace> {
    //
    static_assert(N != 0, "invalid size");
    static_assert(std::is_floating_point_v<NumericType>,
                  "invalid numeric type");
    //! \brief default constructor
    TinyNewtonRaphsonSolver() = default;
    //! \brief constructor
    template <typename... ExternalWorkSpaceArguments>
    TFEL_HOST_DEVICE TinyNewtonRaphsonSolver(ExternalWorkSpaceArguments&&...);
    //! \brief default constructor
    TinyNewtonRaphsonSolver(TinyNewtonRaphsonSolver&) noexcept = default;
    //! \brief default constructor
    TinyNewtonRaphsonSolver(TinyNewtonRaphsonSolver&&) noexcept = default;
    //! \brief default constructor
    TinyNewtonRaphsonSolver& operator=(TinyNewtonRaphsonSolver&&) noexcept =
        default;
    //! \brief default constructor
    TinyNewtonRaphsonSolver& operator=(
        const TinyNewtonRaphsonSolver&) noexcept = default;
    //! \brief compute a new correction to the unknowns
    TFEL_HOST_DEVICE bool computeNewCorrection();
    //! \brief destructor
    ~TinyNewtonRaphsonSolver() noexcept = default;
  };

}  // end of namespace tfel::math

#include "TFEL/Math/NonLinearSolvers/TinyNewtonRaphsonSolver.ixx"

#endif /* LIB_TFEL_MATH_TINYNEWTONRAPHSONSOLVER_HXX */
