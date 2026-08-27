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
#include "TFEL/Math/IntegerEvaluator.hxx"
#include "MFront/BehaviourData.hxx"
#include "MFront/SupportedTypes.hxx"
#include "MFront/BehaviourDescription.hxx"
#include "MFront/AbstractNonLinearSystemSolver.hxx"
#include "MFront/TDLSLinearSystemSolver.hxx"

namespace mfront {

  TDLSLinearSystemSolver::TDLSLinearSystemSolver(
      const tfel::utilities::DataMap& opts) {
    auto validator =
        tfel::utilities::DataMapValidator{}  //
            .addDataTypeValidator<int, std::string>("tile_size")
            .addDataTypeValidator<std::string>("schedule")
            .addDataTypeValidator<std::string>("out_of_tile_search_strategy")
            .addDataTypeValidator<bool>("unroll_inner_loop")
            .addDataTypeValidator<double>("out_of_tile_search_threshold")
            .addDataTypeValidator<double>("singular_pivot_threshold");
    validator.validate(opts);
    if (opts.contains("tile_size")) {
      if (is<int>(opts, "tile_size")) {
        const auto ts = get<int>(opts, "tile_size");
        if (ts < 1) {
          tfel::raise(
              "TDLSLinearSystemSolver: invalid tile size (must be greater or "
              "equal than one)");
        }
        this->tile_size = ts;
      } else {
        this->tile_size = get<std::string>(opts, "tile_size");
      }
    }
    if (opts.contains("unroll_inner_loop")) {
      this->unroll_inner = get<bool>(opts, "unroll_inner_loop");
    }
    if (opts.contains("schedule")) {
      const auto& s = get<std::string>(opts, "schedule");
      if ((s == "left") ||          //
          (s == "left-looking") ||  //
          (s == "left_looking") ||  //
          (s == "LeftLooking")) {
        this->schedule = SchedulePolicy::LEFT_LOOKING;
      } else if ((s == "right") ||          //
                 (s == "right-looking") ||  //
                 (s == "right_looking") ||  //
                 (s == "RightLooking")) {
        this->schedule = SchedulePolicy::RIGHT_LOOKING;
      } else {
        tfel::raise(
            "TDLSLinearSystemSolver: "  //
            "invalid schedule policy (expected 'left_looking' "
            "or 'right_looking', got '" +
            s + "')");
      }
    }
    if (opts.contains("out_of_tile_search_strategy")) {
      const auto& s = get<std::string>(opts, "out_of_tile_search_strategy");
      if ((s == "first_acceptable") ||  //
          (s == "first-acceptable") ||  //
          (s == "FirstAcceptable")) {
        this->out_of_tile_search_strategy =
            OutOfTileSearchStrategy::FIRST_ACCEPTABLE;
      } else if ((s == "full_scan") ||  //
                 (s == "full-scan") ||  //
                 (s == "FullScan")) {
        this->out_of_tile_search_strategy = OutOfTileSearchStrategy::FULL_SCAN;
      } else {
        tfel::raise(
            "TDLSLinearSystemSolver: "  //
            "invalid out of tile search strategy (expected 'first_acceptable' "
            "or 'full_scan', got '" +
            s + "')");
      }
    }
    if (opts.contains("out_of_tile_search_threshold")) {
      const auto v = get<double>(opts, "out_of_tile_search_threshold");
      if ((v < 0) || (std::fpclassify(v) == FP_ZERO)) {
        tfel::raise(
            "TDLSLinearSystemSolver: invalid value for the "
            "'out_of_tile_search_threshold' option, expecting a strictly "
            "positive value");
      }
      this->out_of_tile_search_threshold = v;
    }
    if (opts.contains("singular_pivot_threshold")) {
      const auto v = get<double>(opts, "singular_pivot_threshold");
      if ((v < 0) || (std::fpclassify(v) == FP_ZERO)) {
        tfel::raise(
            "TDLSLinearSystemSolver: invalid value for the "
            "'singular_pivot_threshold' option, expecting a strictly "
            "positive value");
      }
      this->singular_pivot_threshold = v;
    }
    if (this->singular_pivot_threshold.has_value()) {
      if (!this->out_of_tile_search_threshold.has_value()) {
        tfel::raise(
            "TDLSLinearSystemSolver: the 'singular_pivot_threshold' option "
            "requires the 'out_of_tile_search_threshold' option: the TDLS "
            "library requires the former not to exceed the latter, whose "
            "default value depends on the numeric type");
      }
      if (*(this->singular_pivot_threshold) >
          *(this->out_of_tile_search_threshold)) {
        tfel::raise(
            "TDLSLinearSystemSolver: the 'singular_pivot_threshold' option "
            "must not exceed the 'out_of_tile_search_threshold' option");
      }
    }
  }

  std::vector<std::string> TDLSLinearSystemSolver::getSpecificHeaders() const {
    auto headers = LinearSystemSolverBase::getSpecificHeaders();
    headers.push_back("tdls/tdls.hpp");
    return headers;
  }

  std::vector<std::string> TDLSLinearSystemSolver::getReservedNames() const {
    auto names = LinearSystemSolverBase::getReservedNames();
    names.push_back("MFrontTDLSLinearSystemConfiguration");
    return names;
  }  // end of getReservedNames

  void TDLSLinearSystemSolver::writeSpecificMembers(
      std::ostream& os,
      const BehaviourDescription& bd,
      const Hypothesis h) const {
    LinearSystemSolverBase::writeSpecificMembers(os, bd, h);
    // initializers follow the member order of tdls::TiledLUppConfig
    os << "static constexpr auto MFrontTDLSLinearSystemConfiguration =\n"
       << "tdls::TiledLUppConfig<NumericType>{";
    if (this->tile_size.has_value()) {
      os << "\n.tile_size = ";
      if (std::holds_alternative<int>(*(tile_size))) {
        os << std::get<int>(*(tile_size)) << ",";
      } else {
        const auto f = std::get<std::string>(*(tile_size));
        for (const auto& hs : bd.getModellingHypotheses()) {
          tfel::math::IntegerEvaluator ev(f);
          const auto& vars = ev.getVariablesNames();
          for (const auto& v : vars) {
            if (SupportedTypes::isSupportedConstexprIntegerVariables(v)) {
              if (v == "N") {
                ev.setVariableValue(v, getSpaceDimension(hs));
              } else if (v == "TVectorSize") {
                const auto s = getSpaceDimension(hs);
                ev.setVariableValue(v, s);
              } else if (v == "StensorSize") {
                const auto s = getStensorSize(hs);
                ev.setVariableValue(v, s);
              } else if (v == "TensorSize") {
                const auto s = getTensorSize(hs);
                ev.setVariableValue(v, s);
              } else {
                tfel::raise(
                    "TDLSLinearSystemSolver::writeSpecificMembers: internal "
                    "error (unsupported variable '" +
                    v + "')");
              }
            } else {
              ev.setVariableValue(v, bd.getIntegerConstant(hs, v));
            }
          }
          const auto ts = ev.getValue();
          if (ts < 1) {
            tfel::raise(
                "TDLSLinearSystemSolver::writeSpecificMembers: invalid "
                "result for the formula giving the tile size for hypothesis '" +
                tfel::material::ModellingHypothesis::toString(hs) +
                "' (value calculated is '" + std::to_string(ts) + "')");
          }
        }
        os << f << ",";
      }
    }
    if (this->schedule.has_value()) {
      os << "\n.schedule = ";
      if (*(this->schedule) == SchedulePolicy::LEFT_LOOKING) {
        os << "tdls::TiledLUppSchedule::LeftLooking,";
      } else {
        os << "tdls::TiledLUppSchedule::RightLooking,";
      }
    }
    if (this->out_of_tile_search_threshold.has_value()) {
      os << "\n.oot_threshold = NumericType{" << *out_of_tile_search_threshold
         << "},";
    }
    if (this->singular_pivot_threshold.has_value()) {
      os << "\n.singular_eps = NumericType{"
         << *(this->singular_pivot_threshold) << "},";
    }
    if (this->out_of_tile_search_strategy.has_value()) {
      os << "\n.oot_first_acceptable = ";
      if (*(this->out_of_tile_search_strategy) ==
          OutOfTileSearchStrategy::FIRST_ACCEPTABLE) {
        os << "true,";
      } else {
        os << "false,";
      }
    }
    if (this->unroll_inner.has_value()) {
      os << "\n.unroll_inner = ";
      if (*(this->unroll_inner)) {
        os << "true,";
      } else {
        os << "false,";
      }
    }
    os << "};\n\n";
  }  // end of writeSpecificMembers

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
       << "tfel::math::fsarray<" << n << ", int>{};\n";
    if (s.returned_value.has_value()) {
      os << *(s.returned_value) << " = ";
    }
    os << "::tdls::solve_inplace<MFrontTDLSLinearSystemConfiguration>("
       << s.matrix << ", mfront_tdls_pivot, " << s.rhs << ");\n";
  }  // end of writeLinearSystemResolution

  AbstractLinearSystemSolver::MatrixDecompositionResult
  TDLSLinearSystemSolver::getMatrixDecompositionResults(
      const BehaviourDescription& bd,
      const AbstractNonLinearSystemSolver&,
      const Hypothesis h,
      const std::string& n) const {
    const auto& d = bd.getBehaviourData(h);
    const auto& isvs = d.getIntegrationVariables();
    const auto nivs = mfront::getTypeSize(isvs);
    auto perturbation_type =
        "::tfel::math::fsarray<" + nivs.asString() + ", int>";
    return {.matrix = n,
            .matrix_size = nivs.asString(),
            .variables = {{.type = perturbation_type,
                           .name = "mfront_jacobian_permutation"}}};
  }  // end of getMatrixDecompositionResults

  AbstractLinearSystemSolver::MatrixDecompositionResult
  TDLSLinearSystemSolver::writeMatrixDecomposition(
      std::ostream& os,
      const BehaviourDescription& bd,
      const AbstractNonLinearSystemSolver& solver,
      const Hypothesis h,
      const MatrixDecompositionVariables& s) const {
    const auto results =
        this->getMatrixDecompositionResults(bd, solver, h, s.matrix);
    const auto tdls_solver = "::tdls::TiledLUppSolverStatic<NumericType, " +
                             results.matrix_size +
                             ", MFrontTDLSLinearSystemConfiguration>";
    if (results.variables.size() != 1) {
      tfel::raise(
          "TDLSLinearSystemSolver::writeMatrixDecomposition: "
          "invalid number of variables resulting from the decomposition");
    }
    for (const auto& v : results.variables) {
      os << "auto " << v.name << " = " << v.type << "{};\n";
    }
    if (s.returned_value.has_value()) {
      os << *(s.returned_value) << " = ";
    }
    os << "::tdls::factorize<MFrontTDLSLinearSystemConfiguration>(" << s.matrix
       << ", " << results.variables.begin()->name << ");\n";
    return results;
  }  // end of writeMatrixDecomposition

  void TDLSLinearSystemSolver::writeLinearSystemSubstitution(
      std::ostream& os,
      const MatrixDecompositionResult& r,
      const LinearSystemSubstitutionVariables& s) const {
    const auto tdls_solver = "::tdls::TiledLUppSolverStatic<NumericType, " +
                             r.matrix_size +
                             ", MFrontTDLSLinearSystemConfiguration>";
    if (r.variables.size() != 1) {
      tfel::raise(
          "invalide number of variables resulting from the matrix "
          "decomposition");
    }
    os << "::tdls::substitute_inplace<MFrontTDLSLinearSystemConfiguration>("
       << r.matrix << ", " << r.variables.begin()->name << ", " << s.rhs
       << ");\n";
  }  // end of writeLinearSystemSubstitution

  TDLSLinearSystemSolver::~TDLSLinearSystemSolver() = default;

}  // end of namespace mfront
