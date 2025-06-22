/*!
 * \file   TinyBroydenSolver.hxx
 * \brief
 * \author Thomas Helfer
 * \date   05/07/2021
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_TINYBROYDENSOLVER_HXX
#define LIB_TFEL_MATH_TINYBROYDENSOLVER_HXX

#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/tmatrix.hxx"
#include "TFEL/Math/NonLinearSolvers/TinyNonLinearSolverBase.hxx"

namespace tfel::math {

  /*!
   * \brief A class based on the curiously recurring template pattern (CRTP)
   * to solve system of non linear equations using the Broyden algorithm.
   * \tparam N: size of the system of non linear equations.
   * \tparam NumericType: numeric type.
   * \tparam Child: base class.
   *
   * By default, the `Child` class must:
   *
   * 1. Set the values of `epsilon` and `iterMax` data members and the initial
   * guess of the unknowns, i.e. by setting the value of the `zeros` data
   * member.
   * 2. Initialize the jacobian matrix, i.e. the jacobian data member
   * 3. Provide a method called `computeResidual` which must computes
   * the residual, i.e. the data member `fzeros` using the current estimate of
   * the solution, i.e. the data member `zeros`.
   */
  template <unsigned short N, typename NumericType, typename Child>
  struct TinyBroydenSolver : TinyNonLinearSolverBase<N, NumericType, Child> {
    //
    static_assert(N != 0, "invalid size");
    static_assert(std::is_floating_point_v<NumericType>,
                  "invalid numeric type");
    //! \brief default constructor
    TinyBroydenSolver() = default;
    //! \brief default constructor
    TinyBroydenSolver(TinyBroydenSolver&) noexcept = default;
    //! \brief default constructor
    TinyBroydenSolver(TinyBroydenSolver&&) noexcept = default;
    //! \brief default constructor
    TinyBroydenSolver& operator=(TinyBroydenSolver&&) noexcept = default;
    //! \brief default constructor
    TinyBroydenSolver& operator=(const TinyBroydenSolver&) noexcept = default;
    //! \brief destructor
    ~TinyBroydenSolver() noexcept = default;
    //! \brief update the jacobian matrix
    TFEL_HOST_DEVICE void updateOrCheckJacobian();
    //! \brief compute a new correction to the unknowns
    TFEL_HOST_DEVICE bool computeNewCorrection();

   protected:
    //! \brief residual of the previous iterations
    tvector<N, NumericType> fzeros_1;
    //! \brief jacobian matrix
    tmatrix<N, N, NumericType> jacobian;
  };

}  // end of namespace tfel::math

#include "TFEL/Math/NonLinearSolvers/TinyBroydenSolver.ixx"

#endif /* LIB_TFEL_MATH_TINYBROYDENSOLVER_HXX */
