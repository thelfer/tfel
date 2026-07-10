/*!
 * \file   mfront/src/TDLSLinearSystemSolver.cxx
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

#include <ostream>
#include "MFront/BehaviourData.hxx"
#include "MFront/BehaviourDescription.hxx"
#include "MFront/AbstractNonLinearSystemSolver.hxx"
#include "MFront/TDLSLinearSystemSolver.hxx"

namespace mfront {

  TDLSLinearSystemSolver::TDLSLinearSystemSolver(
      const tfel::utilities::DataMap& opts){
    auto validator =
        tfel::utilities::DataMapValidator{}  //
            .addDataTypeValidator<int>("tile_size")
            .addDataTypeValidator<std::string>("schedule")
            .addDataTypeValidator<std::string>("out_of_tile_search_strategy")
            .addDataTypeValidator<bool>("unroll_inner");
    validator.validate(opts);
  }

  std::vector<std::string> TDLSLinearSystemSolver::getSpecificHeaders() const {
    auto headers = LinearSystemSolverBase::getSpecificHeaders();
    headers.push_back("tdls/tdls.hpp");
    return headers;
  }

  void TDLSLinearSystemSolver::writeLinearSystemResolution(
      std::ostream& os,
      const BehaviourDescription& bd,
      const AbstractNonLinearSystemSolver&,
      const Hypothesis h,
      const LinearSystemVariables& s) const {
    const auto& d = bd.getBehaviourData(h);
    const auto& ivs = d.getIntegrationVariables();
    const auto n = ivs.getTypeSize();
    os << "auto mfront_tdls_pivot = "
       << "tfel::math::tvector<" << n << ", int>{};\n";
    if (s.returned_value.has_value()) {
      os << *(s.returned_value) << " = ";
    }
    os << "tdls::solve_inplace("
       << s.matrix << ", mfront_tdls_pivot, " << s.rhs << ");\n";
  }  // end of writeLinearSystemResolution

  TDLSLinearSystemSolver::~TDLSLinearSystemSolver() = default;


}  // end of namespace mfront
