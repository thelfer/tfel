/*!
 * \file   MFront/TDLSLinearSystemSolver.hxx
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

#ifndef LIB_MFRONT_TDLSLINEARSYSTEMSOLVER_HXX
#define LIB_MFRONT_TDLSLINEARSYSTEMSOLVER_HXX

#include "MFront/LinearSystemSolverBase.hxx"

namespace mfront{

  struct TDLSLinearSystemSolver : LinearSystemSolverBase {
    //
    std::vector<std::string> getSpecificHeaders() const override;
    void writeLinearSystemResolution(
        std::ostream&,
        const BehaviourDescription&,
        const AbstractNonLinearSystemSolver&,
        const Hypothesis,
        const LinearSystemVariables&) const override;
    //! \brief destructor
    ~TDLSLinearSystemSolver() override;
  };

} // end of namespace mfront

#endif /* LIB_MFRONT_TDLSLINEARSYSTEMSOLVER_HXX */
