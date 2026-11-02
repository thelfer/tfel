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
#include "MFront/DSLUtilities.hxx"
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
                                         // the constant of the
                                         // `BehaviourProfiler` class is named
                                         // `TINYMATRIXSOLVE` in this branch
                                         "TINYMATRIXSOLVE",
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
                                         const std::string& c,
                                         const std::string& v,
                                         const std::string& s) {
    using namespace std;
    const string tn =
        s.empty() ? "mfront_local_timer" : "mfront_local_timer_" + s;
    os << "mfront::BehaviourProfiler::Timer " << tn << "(" << c
       << "Profiler::getProfiler(),"
       << "mfront::BehaviourProfiler::" << getBehaviourProfilerIndex(v) << ");"
       << endl;
  }  // end of writeStandardPerformanceProfiling

  void writeStandardPerformanceProfilingBegin(std::ostream& os,
                                              const std::string& c,
                                              const std::string& v,
                                              const std::string& s) {
    using namespace std;
    const string tn =
        s.empty() ? "mfront_local_timer" : "mfront_local_timer_" + s;
    os << "{" << endl
       << "mfront::BehaviourProfiler::Timer " << tn << "(" << c
       << "Profiler::getProfiler(),"
       << "mfront::BehaviourProfiler::" << getBehaviourProfilerIndex(v) << ");"
       << endl;
  }  // end of writeStandardPerformanceProfilingBegin

  void writeStandardPerformanceProfilingEnd(std::ostream& os) {
    using namespace std;
    os << "}" << endl;
  }  // end of BehaviourDSLCommon::writeStandardPerformanceProfilingEnd

}  // end of namespace mfront
