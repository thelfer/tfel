/*!
 * \file   MFront/DefaultLinearSystemSolver.hxx
 * \brief    
 * \author Thomas Helfer
 * \date   05/07/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_DEFAULTLINEARSYSTEMSOLVER_HXX
#define LIB_MFRONT_DEFAULTLINEARSYSTEMSOLVER_HXX

#include "MFront/LinearSystemSolverBase.hxx"

namespace mfront{

  struct DefaultLinearSystemSolver : LinearSystemSolverBase {
    //! \brief destructor
    ~DefaultLinearSystemSolver() override;
  };

} // end of namespace mfront

#endif /* LIB_MFRONT_DEFAULTLINEARSYSTEMSOLVER_HXX */
