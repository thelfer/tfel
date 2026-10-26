/*!
 * \file  mfront/src/PerformanceProfiling.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 22 août 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <array>
#include <ostream>
#include <algorithm>
#include "TFEL/Raise.hxx"
#include "MFront/MFrontUtilities.hxx"
#include "MFront/PerformanceProfiling.hxx"

namespace mfront {

  /*!
   * \return the index of the given code block in the `BehaviourProfiler`
   * class
   * \param[in] v: code block name
   */
  static std::string getBehaviourProfilerIndex(std::string_view v) {
    constexpr auto indices =
        std::array<std::string_view, 28>{"FLOWRULE",
                                         "BEFOREINITIALIZELOCALVARIABLES",
                                         "INITIALIZELOCALVARIABLES",
                                         "AFTERINITIALIZELOCALVARIABLES",
                                         "COMPUTEPREDICTOR",
                                         "COMPUTEPREDICTIONOPERATOR",
                                         "INTEGRATOR",
                                         "COMPUTETHERMODYNAMICFORCES",
                                         "COMPUTEDERIVATIVE",
                                         "INITIALIZEJACOBIAN",
                                         "COMPUTEFDF",
                                         "SOLVELINEARSYSTEM",
                                         "COMPUTEFINALTHERMODYNAMICFORCES",
                                         "COMPUTETANGENTOPERATOR",
                                         "UPDATEAUXILIARYSTATEVARIABLES",
                                         "FINITESTRAINPREPROCESSING",
                                         "FINITESTRAINPOSTPROCESSING",
                                         "USERDEFINEDCODE1",
                                         "USERDEFINEDCODE2",
                                         "ADDITIONALCONVERGENCECHECKS",
                                         "APRIORITIMESTEPSCALINGFACTOR",
                                         "APOSTERIORITIMESTEPSCALINGFACTOR",
                                         "COMPUTESTRESSFREEEXPANSION",
                                         "COMPUTEINTERNALENERGY",
                                         "COMPUTEDISSIPATEDENERGY",
                                         "COMPUTESPEEDOFSOUND",
                                         "INITIALIZEJACOBIANINVERT",
                                         "TOTALTIME"};
    // the suffix of the tangent and prediction operators of finite strain
    // behaviours (`ComputeTangentOperator-DPK1_DF` for instance) and the
    // index of the flow of the MultipleIsotropicMisesFlows DSL (`FlowRule0`
    // for instance) are dropped
    auto n = v.substr(0, v.find('-'));
    n = n.substr(0, n.find_last_not_of("0123456789\n") + 1);
    if (n == "RungeKuttaUpdateAuxiliaryStateVariables") {
      n = "UpdateAuxiliaryStateVariables";
    }
    const auto i = makeUpperCase(n);
    if (std::find(indices.begin(), indices.end(), i) == indices.end()) {
      tfel::raise(
          "getBehaviourProfilerIndex: "
          "no profiling index associated with code block '" +
          std::string{v} + "'");
    }
    return i;
  }  // end of getBehaviourProfilerIndex

  void writeStandardPerformanceProfiling(std::ostream& os,
                                         std::string_view c,
                                         std::string_view v,
                                         std::string_view s) {
    os << "mfront::BehaviourProfiler::Timer ";
    if (!s.empty()) {
      os << "mfront_local_timer_" << s;
    } else {
      os << "mfront_local_timer";
    }
    os << "(" << c << "Profiler::getProfiler(),"
       << "mfront::BehaviourProfiler::" << getBehaviourProfilerIndex(v)
       << ");\n";
  }  // end of writeStandardPerformanceProfiling

  void writeStandardPerformanceProfilingBegin(std::ostream& os,
                                              std::string_view c,
                                              std::string_view v,
                                              std::string_view s) {
    os << "{\n"
       << "mfront::BehaviourProfiler::Timer ";
    if (!s.empty()) {
      os << "mfront_local_timer_" << s;
    } else {
      os << "mfront_local_timer";
    }
    os << "(" << c << "Profiler::getProfiler(),"
       << "mfront::BehaviourProfiler::" << getBehaviourProfilerIndex(v)
       << ");\n";
  }  // end of writeStandardPerformanceProfilingBegin

  void writeStandardPerformanceProfilingEnd(std::ostream& os) {
    os << "}\n";
  }  // end of writeStandardPerformanceProfilingEnd

}  // end of namespace mfront
