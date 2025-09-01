/*!
 * \file   SolverWorkSpace.hxx
 * \brief
 * \author Thomas Helfer
 * \date   04 nov. 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_SOLVERWORKSPACE_HXX
#define LIB_MTEST_SOLVERWORKSPACE_HXX

#include "TFEL/Math/vector.hxx"
#include "TFEL/Math/matrix.hxx"
#include "TFEL/Math/LUSolve.hxx"

#include "MTest/Config.hxx"
#include "MTest/Types.hxx"

namespace mtest {

  /*!
   * work space used by the solver
   */
  struct SolverWorkSpace {
    //! a simple alias
    using size_type = tfel::math::matrix<real>::size_type;
    //! stiffness matrix
    tfel::math::matrix<real> K;
    // residual
    tfel::math::vector<real> r;
    // unknowns correction
    tfel::math::vector<real> du;
    // permuation matrix
    tfel::math::Permutation<size_type> p_lu;
    // temporary vector used by the LUSolve::exe function
    tfel::math::vector<real> x;
  };  // end of struct SolverWorkSpace

  /*!
   * \param[out] wk : workspace
   * \param[in]  s  : problem size
   */
  MTEST_VISIBILITY_EXPORT void initialize(SolverWorkSpace&,
                                          const SolverWorkSpace::size_type);

}  // namespace mtest

#endif /* LIB_MTEST_SOLVERWORKSPACE_HXX */
