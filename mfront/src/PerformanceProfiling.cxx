/*!
 * \file  mfront/src/PerformanceProfiling.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 22 août 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <ostream>

#include "MFront/DSLUtilities.hxx"
#include "MFront/PerformanceProfiling.hxx"

namespace mfront {

  void writeStandardPerformanceProfiling(std::ostream& os,
                                         const std::string& c,
                                         const std::string& v,
                                         const std::string& s) {
    using namespace std;
    const string tn =
        s.empty() ? "mfront_local_timer" : "mfront_local_timer_" + s;
    os << "mfront::BehaviourProfiler::Timer " << tn << "(" << c
       << "Profiler::getProfiler(),"
       << "mfront::BehaviourProfiler::" << makeUpperCase(v) << ");" << endl;
  }  // end of BehaviourDSLCommon::writeStandardPerformanceProfiling

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
       << "mfront::BehaviourProfiler::" << makeUpperCase(v) << ");" << endl;
  }  // end of BehaviourDSLCommon::writeStandardPerformanceProfilingBegin

  void writeStandardPerformanceProfilingEnd(std::ostream& os) {
    using namespace std;
    os << "}" << endl;
  }  // end of BehaviourDSLCommon::writeStandardPerformanceProfilingEnd

}  // end of namespace mfront
