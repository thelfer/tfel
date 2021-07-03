/*!
 * \file   include/TFEL/Math/NonLinearSolvers/TinyNonLinearSolverBase.hxx
 * \brief
 * \author Thomas Helfer
 * \date   02/07/2021
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_NONLINEARSOLVERS_TINYNONLINEARSOLVERBASE_HXX
#define LIB_TFEL_MATH_NONLINEARSOLVERS_TINYNONLINEARSOLVERBASE_HXX

#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/tmatrix.hxx"

namespace tfel::math {

  /*!
   * \brief A base class for algorithms dedicated to solve finite sized system
   * of non linear equations.
   * \tparam N: size of the system of non linear equations.
   * \tparam NumericType: numeric type.
   * \tparam Child: base class.
   *
   * By default, the `Child` class must:
   *
   * 1. Set the values of `epsilon` and `iterMax` data members and the initial
   * guess of the unknowns, i.e. by setting the value of the `zeros` data
   * member.
   * 2. Provide a method called `computeResidualAndJacobian` which must computes
   * the residual, i.e. the data member `fzeros` and the jacobian matrix, i.e.
   * the data member `jacobian` using the current estimate of the solution, i.e.
   * the data member `zeros`.
   *
   * This class has numerous customisation points:
   * - `checkConvergence`: checks if convergence is achieved.
   * - `processNewEstimate`: this method is called at the beginning of the
   * `resolution and each time the estimate of the solution is updated. The
   * default implementation does nothing.
   * - `reportBeginningOfResolution`: this method is meant to be overloaded by
   * the `Child` class to trace the beginning of the resolution. The default
   * implementation does nothing.
   * - `reportSuccess`: this method is meant to be overloaded by the `Child`
   * class to report a successfull resolution. The default implementation does
   * nothing.
   * - `reportFailure`: this method is meant to be overloaded by the `Child`
   * class to report the resolution fails because the maximal number of
   * iterations has been reached. The default implementation does nothing.
   * - `reportInvalidResidualEvaluation`:  this method  this method is meant to
   * be overloaded by the `Child` class to report that the residual evaluation
   * failed. The default implementation does nothing.
   */
  template <unsigned short N, typename NumericType, typename Child>
  struct TinyNonLinearSolverBase {
    //
    static_assert(N != 0, "invalid size");
    static_assert(std::is_floating_point_v<NumericType>,
                  "invalid numeric type");
    //! \brief a simple alias
    using size_type = unsigned short;
    //! \brief default constructor
    TinyNonLinearSolverBase() = default;
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
     * \brief solve the non linear problem
     * \return true on success
     */
    bool solveNonLinearSystem();
    //! \return the norm of the residual
    NumericType computeResidualNorm() { return norm(this->fzeros); }
    /*!
     * \brief check the convergence of the method
     * \param[in] e: current error
     */
    bool checkConvergence(const NumericType e) { return e < this->epsilon; }
    /*!
     * \brief update the jacobian matrix if required.
     *
     * This method can be used to compute the jacobian or part of the jacobian
     * numerically. If the jacobian was computed analytically in
     * `computeResidualAndJacobian`, this method can be used to compare it to a
     * numerical approximation.
     */
    void updateOrCheckJacobian() {}
    /*!
     * \brief method meant to set bounds on some components of the current
     * correction or to implement a line search.
     * \param[in] c: current Newton correction
     */
    void processNewCorrection() {}
    /*!
     * \brief method meant to process the new estimate.
     *
     * This method may be called to apply bounds on the estimate.
     */
    void processNewEstimate() {}
    //! \brief method called when the resolution begins
    void reportBeginningOfResolution() const {}
    //! \brief method called when the resolution succeeds
    void reportSuccess() const {}
    //! \brief method called when the resolution fails
    void reportFailure() const {}
    //! \brief method called with the evaluation of the residual failed.
    void reportInvalidResidualEvaluation() const {}
    /*!
     * \brief method called after a standard Newton step
     * \param[in] e: error
     */
    void reportStandardNewtonIteration(const NumericType) const {}
    //! \brief jacobian matrix
    tmatrix<N, N, NumericType> jacobian;
    //! \brief residual vector
    tvector<N, NumericType> fzeros;
    //! \brief current estimate of the unknowns
    tvector<N, NumericType> zeros;
    //! \brief current correction
    tvector<N, NumericType> delta_zeros;
    //! \brief criterion value
    NumericType epsilon;
    //! \brief current iteration number
    size_type iter;
    //! \brief maximum number of iterations
    size_type iterMax;
  };

}  // end of namespace tfel::math

#include "TFEL/Math/NonLinearSolvers/TinyNonLinearSolverBase.ixx"

#endif /* LIB_TFEL_MATH_NONLINEARSOLVERS_TINYNONLINEARSOLVERBASE_HXX */
