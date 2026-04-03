/*!
 * \file   include/TFEL/Math/NonLinearSolvers/TinyNonLinearSolverBase.hxx
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

#ifndef LIB_TFEL_MATH_NONLINEARSOLVERS_TINYNONLINEARSOLVERBASE_HXX
#define LIB_TFEL_MATH_NONLINEARSOLVERS_TINYNONLINEARSOLVERBASE_HXX

#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/tmatrix.hxx"

namespace tfel::math {

  /*!
   * \brief base class for allocating on the stack a workspace
   * usable by the solvers derived from
   * `TinyNonLinearSolverBase` class.
   * \tparam N: size of the system of non linear equations.
   * \tparam NumericType: numeric type.
   */
  template <unsigned short N, typename NumericType>
  struct StackAllocatedTinyNonLinearSolverWorkspace {
    //! \brief residual vector
    tvector<N, NumericType> fzeros;
    //! \brief current estimate of the unknowns
    tvector<N, NumericType> zeros;
    //! \brief current correction
    tvector<N, NumericType> delta_zeros;
  };

  /*!
   * \brief A base class for algorithms dedicated to solve finite sized system
   * of non linear equations.
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
   * the residual, i.e. the data member `fzeros`  using the current estimate of
   * the solution, i.e. the data member `zeros`. When meaningful, this method
   * may also compute the jacobian matrix, i.e. the data member `jacobian`.
   *
   * This class has numerous customisation points:
   *
   * - `checkConvergence`: checks if convergence is achieved.
   * - `processNewEstimate`: this method is called at the beginning of the
   * `resolution and each time the estimate of the solution is updated. The
   * default implementation does nothing.
   * - `rejectCurrentCorrection`: this method is meant to
   * be overloaded by the `Child` class to handle the rejection of the current
   * correction. The default implementation does nothing.
   * - `reportBeginningOfResolution`: this method is meant to be overloaded by
   * the `Child` class to trace the beginning of the resolution. The default
   * implementation does nothing.
   * - `reportSuccess`: this method is meant to be overloaded by the `Child`
   * class to report a successfull resolution. The default implementation does
   * nothing.
   * - `reportFailure`: this method is meant to be overloaded by the `Child`
   * class to report the resolution fails because the maximal number of
   * iterations has been reached. The default implementation does nothing.
   * - `reportInvalidResidualEvaluation`:  this method is meant to
   * be overloaded by the `Child` class to report that the residual evaluation
   * failed. The default implementation does nothing.
   * - `reportNewCorrectionComputationFailure`: this method is meant to
   * be overloaded by the `Child` class to report the failure of the computation
   * of a new correction.
   */
  template <unsigned short N,
            typename NumericType,
            typename Child,
            template <unsigned short, typename> typename ExternalWorkSpace =
                StackAllocatedTinyNonLinearSolverWorkspace>
  struct TinyNonLinearSolverBase : public ExternalWorkSpace<N, NumericType> {
    //
    static_assert(N != 0, "invalid size");
    static_assert(std::is_floating_point_v<NumericType>,
                  "invalid numeric type");
    //! \brief a simple alias
    using iteration_number_type = unsigned short;
    //! \brief default constructor
    TinyNonLinearSolverBase() = default;
    /*!
     * \brief constructor used to initialize the external workspace
     * \param[in] args: arguments forwarded to the external workspace
     */
    template <typename... ExternalWorkSpaceArguments>
    TFEL_HOST_DEVICE TinyNonLinearSolverBase(
        ExternalWorkSpaceArguments&&...) noexcept;
    //! \brief default constructor
    TinyNonLinearSolverBase(TinyNonLinearSolverBase&) noexcept = default;
    //! \brief default constructor
    TinyNonLinearSolverBase(TinyNonLinearSolverBase&&) noexcept = default;
    //! \brief default constructor
    TinyNonLinearSolverBase& operator=(TinyNonLinearSolverBase&&) noexcept =
        default;
    //! \brief default constructor
    TinyNonLinearSolverBase& operator=(
        const TinyNonLinearSolverBase&) noexcept = default;
    //! \brief destructor
    ~TinyNonLinearSolverBase() noexcept = default;

   protected:
    /*!
     * \brief solve the non linear problem. This methods wraps the resolution
     * algorithm and tries to handles failures of the latter.
     *
     * Failure is handled by dividing the last correction by two and restarting
     * the resolution algorithm implemented in `solveNonLinearSystem2`. This can
     * be done several times, up to the moment when the correction allows the
     * resolution algorithm to restart correctly.
     *
     * If the resolution algorithm fails before being able to compute a
     * correction, it probably means that the initial estimate given by the user
     * is pretty bad. Since the main usage of the non linear solver is to find
     * the increment of the internal state variables of some physical
     * behaviours, the null vector is a pretty natural choice for an intial
     * estimate, so we simply choose here to divide the initial estimate by two
     * and restart the resolution algorithm implemented in
     * `solveNonLinearSystem2`. Of course, this strategy is questionable in the
     * general case.
     *
     * \return true on success
     */
    TFEL_HOST_DEVICE bool solveNonLinearSystem();
    /*!
     * \brief solve the non linear problem. This method is called by the
     * `solveNonLinearSystem` and must contain the core of the resolution
     * algorithm.
     * \return true on success
     */
    TFEL_HOST_DEVICE bool solveNonLinearSystem2();
    /*!
     * \brief this method is called at the beginning of the
     * `solveNonLinearSystem` method.
     *
     * This method can be used to initialize some internal state, including
     * numerical parameters.
     */
    TFEL_HOST_DEVICE constexpr void
    executeInitialisationTaskBeforeResolution() noexcept {}
    /*!
     * \brief this method is called at the beginning of the
     * `solveNonLinearSystem2` method.
     *
     *
     * This method can be used to initialize some internal state, including
     * numerical parameters.
     */
    TFEL_HOST_DEVICE constexpr void
    executeInitialisationTaskBeforeBeginningOfCoreAlgorithm() noexcept {}
    //! \return the norm of the residual
    TFEL_HOST_DEVICE NumericType computeResidualNorm() const noexcept {
      return norm(this->fzeros);
    }
    /*!
     * \brief check the convergence of the method
     * \param[in] e: current error
     */
    TFEL_HOST_DEVICE constexpr bool checkConvergence(
        const NumericType e) const noexcept {
      return e < this->epsilon;
    }
    /*!
     * \brief solve linear system
     * \return true on success
     * \param[in] m: matrix
     * \param[in,out] v: right hand side on input, solution on output
     */
    template <MatrixConcept FixedSizeMatrixType,
              VectorConcept FixedSizeVectorType>
    TFEL_HOST_DEVICE bool solveLinearSystem(
        FixedSizeMatrixType&, FixedSizeVectorType&) const noexcept;
    /*!
     * \brief update the jacobian matrix if required.
     *
     * This method can be used to compute the jacobian or part of the
     * jacobian numerically. If the jacobian was computed in
     * `computeResidual`, this method can be used to compare it to a
     * numerical approximation.
     *
     * This method is not used directly used by the
     * `TinyNonLinearSolverBase`. It is only defined to name an appropriate
     * method which shall be used in derived class if required.
     */
    TFEL_HOST_DEVICE constexpr void updateOrCheckJacobian() noexcept {}
    /*!
     * \brief method meant to set bounds on some components of the current
     * correction or to implement a line search or to implement static
     * decondensation.
     */
    TFEL_HOST_DEVICE constexpr void processNewCorrection() noexcept {}
    /*!
     * \brief method called when the current correction is rejected.
     *
     * This can be used to cancel static decondensation.
     */
    TFEL_HOST_DEVICE constexpr void rejectCurrentCorrection() noexcept {}
    /*!
     * \brief method meant to process the new estimate.
     *
     * This method may be called to apply bounds on the estimate.
     */
    TFEL_HOST_DEVICE constexpr void processNewEstimate() noexcept {}
    //! \brief method called when the resolution begins
    TFEL_HOST_DEVICE constexpr void reportBeginningOfResolution()
        const noexcept {}
    //! \brief method called when the resolution succeeds
    TFEL_HOST_DEVICE constexpr void reportSuccess() const noexcept {}
    //! \brief method called when the resolution fails
    TFEL_HOST_DEVICE constexpr void reportFailure() const noexcept {}
    //! \brief method called when the evaluation of the residual failed.
    TFEL_HOST_DEVICE constexpr void reportInvalidResidualEvaluation()
        const noexcept {}
    //! \brief method called when the computation of a new correction failed.
    TFEL_HOST_DEVICE constexpr void reportNewCorrectionComputationFailure()
        const noexcept {}
    /*!
     * \brief method called after a standard Newton step
     * \param[in] e: error
     */
    TFEL_HOST_DEVICE constexpr void reportStandardIteration(
        const NumericType) const noexcept {}
    //! \brief criterion value
    NumericType epsilon;
    /*!
     * \brief current iteration number
     *
     * This variable is set to zero at the beginning of the
     * `solveNonLinearSystem` method
     */
    iteration_number_type iter;
    //! \brief maximum number of iterations
    iteration_number_type iterMax;
    /*!
     * \brief boolean stating if an increment of the unknowns has been computed.
     */
    bool is_delta_zeros_defined;
  };

}  // end of namespace tfel::math

#include "TFEL/Math/NonLinearSolvers/TinyNonLinearSolverBase.ixx"

#endif /* LIB_TFEL_MATH_NONLINEARSOLVERS_TINYNONLINEARSOLVERBASE_HXX */
