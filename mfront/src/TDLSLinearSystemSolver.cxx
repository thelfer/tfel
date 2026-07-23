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
            .addDataTypeValidator<bool>("unroll_inner_loop");
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
        this->out_of_tile_search_strategy ==
            OutOfTileSearchStrategy::FIRST_ACCEPTABLE;
      } else if ((s == "full_scan") ||  //
                 (s == "full-scan") ||  //
                 (s == "FullScan")) {
        this->out_of_tile_search_strategy == OutOfTileSearchStrategy::FULL_SCAN;
      } else {
        tfel::raise(
            "TDLSLinearSystemSolver: "  //
            "invalid out of tile search strategy (expected 'first_acceptable' "
            "or 'full_scan', got '" +
            s + "')");
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
    os << "struct MFrontTDLSLinearSystemConfiguration\n"
       << ": tdls::TiledLUppDefaultConfig<NumericType>{\n";
    if (this->tile_size.has_value()) {
      os << "static constexpr int tile_size = ";
      if (std::holds_alternative<int>(*(tile_size))) {
        os << std::get<int>(*(tile_size)) << ";\n";
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
        os << f << ";\n";
      }
    }
    if (this->schedule.has_value()) {
      os << "static constexpr tdls::TiledLUppSchedule schedule = ";
      if (*(this->schedule) == SchedulePolicy::LEFT_LOOKING) {
        os << "tdls::TiledLUppSchedule::LeftLooking;\n";
      } else {
        os << "tdls::TiledLUppSchedule::RightLooking;\n";
      }
    }
    if (this->out_of_tile_search_strategy.has_value()) {
      os << "static constexpr bool oot_first_acceptable = ";
      if (*(this->out_of_tile_search_strategy) ==
          OutOfTileSearchStrategy::FIRST_ACCEPTABLE) {
        os << "true;\n";
      } else {
        os << "false;\n";
      }
    }
    if (this->unroll_inner.has_value()) {
      os << "static constexpr bool unroll_inner = ";
      if (*(this->unroll_inner)) {
        os << "true;\n";
      } else {
        os << "false;\n";
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
       << "tfel::math::tvector<" << n << ", int>{};\n";
    if (s.returned_value.has_value()) {
      os << *(s.returned_value) << " = ";
    }
    os << "tdls::solve_inplace<MFrontTDLSLinearSystemConfiguration>("
       << s.matrix << ", mfront_tdls_pivot, " << s.rhs << ");\n";
  }  // end of writeLinearSystemResolution

  TDLSLinearSystemSolver::~TDLSLinearSystemSolver() = default;

}  // end of namespace mfront
