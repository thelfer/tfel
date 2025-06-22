/*!
 * \file   Solver.hxx
 * \brief
 * \author Thomas Helfer
 * \date   08 nov. 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_SOLVER_HXX
#define LIB_MTEST_SOLVER_HXX

#include "MTest/Config.hxx"
#include "MTest/Types.hxx"

namespace mtest {

  // forward declaration
  struct StudyCurrentState;
  // forward declaration
  struct SolverWorkSpace;
  // forward declaration
  struct Study;
  // forward declaration
  struct SolverOptions;

  /*!
   *
   */
  struct Solver {
    /*!
     * \brief find the current state of the structure at the end of the
     * time step
     * \param[out] scs: structure current state
     * \param[out]  wk: workspace
     * \param[out]   s: physical system
     * \param[in]    o: solver options
     * \param[in]   ti: time at the beginning of the time step
     * \param[in]   ti: time at the end of the time step
     */
    virtual void execute(StudyCurrentState& scs,
                         SolverWorkSpace& wk,
                         const Study& s,
                         const SolverOptions& o,
                         const real ti,
                         const real te) = 0;
    //! destructor
    virtual ~Solver();
  };

}  // end of namespace mtest

#endif /* LIB_MTEST_SOLVER_HXX */
