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

#include <variant>
#include <optional>
#include "TFEL/Utilities/Data.hxx"
#include "MFront/LinearSystemSolverBase.hxx"

namespace mfront {

  struct TDLSLinearSystemSolver : LinearSystemSolverBase {
    /*!
     * \brief constructor
     * \param[in] opts: options passed to the linear solver
     */
    TDLSLinearSystemSolver(const tfel::utilities::DataMap&);
    //
    std::vector<std::string> getSpecificHeaders() const override;
    std::vector<std::string> getReservedNames() const override;
    void writeSpecificMembers(std::ostream&,
                              const BehaviourDescription&,
                              const Hypothesis) const override;
    void writeLinearSystemResolution(
        std::ostream&,
        const BehaviourDescription&,
        const AbstractNonLinearSystemSolver&,
        const Hypothesis,
        const LinearSystemVariables&) const override;
    //! \brief destructor
    ~TDLSLinearSystemSolver() override;

   protected:
    //
    enum struct SchedulePolicy { LEFT_LOOKING, RIGHT_LOOKING };
    enum struct OutOfTileSearchStrategy { FIRST_ACCEPTABLE, FULL_SCAN };
    //
    std::optional<std::variant<int, std::string>> tile_size;
    std::optional<bool> unroll_inner;
    std::optional<SchedulePolicy> schedule;
    std::optional<OutOfTileSearchStrategy> out_of_tile_search_strategy;
  };  // end of struct TDLSLinearSystemSolver

}  // end of namespace mfront

#endif /* LIB_MFRONT_TDLSLINEARSYSTEMSOLVER_HXX */
