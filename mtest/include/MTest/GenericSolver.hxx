/*!
 * \file   GenericSolver.hxx
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

#ifndef LIB_MTEST_GENERICSOLVER_HXX
#define LIB_MTEST_GENERICSOLVER_HXX

#include "MTest/Solver.hxx"

namespace mtest {

  struct GenericSolver : public Solver {
    /*!
     * \brief find the current state of the structure at the end of the
     * time step
     * \param[out] scs: structure current state
     * \param[out]  wk: workspace
     * \param[in]    s: physical system
     * \param[in]    o: solver options
     * \param[in]   ti: time at the beginning of the time step
     * \param[in]   ti: time at the end of the time step
     */
    void execute(StudyCurrentState& scs,
                 SolverWorkSpace& wk,
                 const Study& s,
                 const SolverOptions& o,
                 const real ti,
                 const real te) override;
    //! destructor
    ~GenericSolver() override;
  };  // end of struct GenericSolver

}  // end of namespace mtest

#endif /* LIB_MTEST_GENERICSOLVER_HXX */
