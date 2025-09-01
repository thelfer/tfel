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

#include <ostream>
#include "MFront/MFrontUtilities.hxx"
#include "MFront/PerformanceProfiling.hxx"

namespace mfront {

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
       << "mfront::BehaviourProfiler::" << makeUpperCase(v) << ");\n";
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
       << "mfront::BehaviourProfiler::" << makeUpperCase(v) << ");\n";
  }  // end of writeStandardPerformanceProfilingBegin

  void writeStandardPerformanceProfilingEnd(std::ostream& os) {
    os << "}\n";
  }  // end of writeStandardPerformanceProfilingEnd

}  // end of namespace mfront
