/*!
 * \file   SolverWorkSpace.hxx
 * \brief    
 * \author THOMAS HELFER
 * \date   04 nov. 2015
 */

#ifndef _LIB_MTEST_SOLVERWORKSPACE_HXX_
#define _LIB_MTEST_SOLVERWORKSPACE_HXX_

#include"TFEL/Math/vector.hxx"
#include"TFEL/Math/matrix.hxx"
#include"TFEL/Math/LUSolve.hxx"

#include"MTest/Config.hxx"
#include"MTest/Types.hxx"

namespace mtest{

  /*!
   * work space used by the solver
   */
  struct SolverWorkSpace
  {
    //! a simple alias
    using size_type = tfel::math::matrix<real>::size_type;
    //! stiffness matrix
    tfel::math::matrix<real> K;
    // residual
    tfel::math::vector<real> r;
    // unknowns correction
    tfel::math::vector<real> du;
    // unknowns estimation at the previous iteration
    tfel::math::vector<real> u10;
    // permuation matrix
    tfel::math::Permutation<size_type> p_lu;
    // temporary vector used by the LUSolve::exe function
    tfel::math::vector<real> x;
  }; // end of struct SolverWorkSpace

  /*!
   * \param[out] wk : workspace
   * \param[in]  s  : problem size
   */
  MTEST_VISIBILITY_EXPORT void
  initialize(SolverWorkSpace&,
	     const SolverWorkSpace::size_type);
  
} // end of namespace namespace mtest

#endif /* _LIB_MTEST_SOLVERWORKSPACE_HXX_ */
