/*! 
 * \file  MFrontPerformanceProfiling.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 22 août 2014
 */

#include<ostream>

#include"MFront/ParserUtilities.hxx"
#include"MFront/MFrontPerformanceProfiling.hxx"

namespace mfront{

  void
  writeStandardPerformanceProfiling(std::ostream& os,
				    const std::string& c,
				    const std::string& v,
				    const std::string& s)
  {
    using namespace std;
    const string tn = s.empty() ? "mfront_local_timer" : "mfront_local_timer_"+s;
    os << "mfront::MFrontBehaviourProfiler::Timer " << tn 
       << "(" << c << "Profiler::getProfiler(),"
       << "mfront::MFrontBehaviourProfiler::" << makeUpperCase(v) << ");" << endl;
  } // end of MFrontBehaviourParserCommon::writeStandardPerformanceProfiling

  void
  writeStandardPerformanceProfilingBegin(std::ostream& os,
					 const std::string& c,
					 const std::string& v,
					 const std::string& s)
  {
    using namespace std;
    const string tn = s.empty() ? "mfront_local_timer" : "mfront_local_timer_"+s;
    os << "{" << endl
       << "mfront::MFrontBehaviourProfiler::Timer " << tn 
       << "(" << c << "Profiler::getProfiler(),"
       << "mfront::MFrontBehaviourProfiler::" << makeUpperCase(v) << ");" << endl;
  } // end of MFrontBehaviourParserCommon::writeStandardPerformanceProfilingBegin

  void
  writeStandardPerformanceProfilingEnd(std::ostream& os)
  {
    using namespace std;
    os << "}" << endl;
  } // end of MFrontBehaviourParserCommon::writeStandardPerformanceProfilingEnd

} // end of namespace mfront
