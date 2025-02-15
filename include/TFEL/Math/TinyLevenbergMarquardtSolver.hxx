/*!
 * \file   TinyLevenbergMarquardtSolver.hxx
 * \brief
 * \author Thomas Helfer
 * \date   06/07/2021
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_TINYLEVENBERGMARQUARDTSOLVER_HXX
#define LIB_TFEL_MATH_TINYLEVENBERGMARQUARDTSOLVER_HXX

#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/tmatrix.hxx"
#include "TFEL/Math/NonLinearSolvers/TinyNonLinearSolverBase.hxx"

namespace tfel::math {

  /*!
   * \brief class allocating on the stack a workspace usable by the
   * `TinyLevenbergMarquardtSolver` class.
   * \tparam N: size of the system of non linear equations.
   * \tparam NumericType: numeric type.
   */
  template <unsigned short N, typename NumericType>
  struct StackAllocatedTinyLevenbergMarquardtSolverWorkspace
      : public StackAllocatedTinyNonLinearSolverWorkspace<N, NumericType> {
    //! \brief jacobian matrix
    tmatrix<N, N, NumericType> jacobian;
    tmatrix<N, N, NumericType> levmar_jacobian_1;
    tvector<N, NumericType> levmar_fzeros_1;
  };  // end of StackAllocatedTinyLevenbergMarquardtSolverWorkspace

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
                StackAllocatedTinyLevenbergMarquardtSolverWorkspace>
  struct TinyLevenbergMarquardtSolver
      : TinyNonLinearSolverBase<N, NumericType, Child, ExternalWorkSpace> {
    //
    static_assert(N != 0, "invalid size");
    static_assert(std::is_floating_point_v<NumericType>,
                  "invalid numeric type");
    //! \brief default constructor
    TinyLevenbergMarquardtSolver() = default;
    //! \brief default constructor
    TinyLevenbergMarquardtSolver(TinyLevenbergMarquardtSolver&) noexcept =
        default;
    //! \brief default constructor
    TinyLevenbergMarquardtSolver(TinyLevenbergMarquardtSolver&&) noexcept =
        default;
    //! \brief default constructor
    TinyLevenbergMarquardtSolver& operator=(
        TinyLevenbergMarquardtSolver&&) noexcept = default;
    //! \brief default constructor
    TinyLevenbergMarquardtSolver& operator=(
        const TinyLevenbergMarquardtSolver&) noexcept = default;
    //! \brief destructor
    ~TinyLevenbergMarquardtSolver() noexcept = default;
    /*!
     * \brief this method is called at the beginning of the
     * `solveNonLinearSystem` method.
     */
    TFEL_HOST_DEVICE constexpr void
    executeInitialisationTaskBeforeResolution() noexcept {
      this->levmar_mu = this->levmar_mu0;
    }
    /*!
     * \brief this method is called at the beginning of the
     * `solveNonLinearSystem2` method.
     */
    TFEL_HOST_DEVICE constexpr void
    executeInitialisationTaskBeforeBeginningOfCoreAlgorithm() noexcept {
      this->levmar_first = true;
    }
    //! \brief compute a new correction to the unknowns
    TFEL_HOST_DEVICE bool computeNewCorrection();
    //!
    TFEL_HOST_DEVICE bool computeLevenbergMarquardtCorrection();

   protected:
    //! \brief
    NumericType levmar_mu0;
    NumericType levmar_p0;
    NumericType levmar_p1;
    NumericType levmar_p2;
    NumericType levmar_m;

   private:
    NumericType levmar_error;
    NumericType levmar_error_1;
    NumericType levmar_mu;
    bool levmar_first;
  };

}  // end of namespace tfel::math

#include "TFEL/Math/NonLinearSolvers/TinyLevenbergMarquardtSolver.ixx"

#endif /* LIB_TFEL_MATH_TINYLEVENBERGMARQUARDTSOLVER_HXX */
