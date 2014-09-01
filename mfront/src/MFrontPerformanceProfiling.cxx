/*! 
 * \file  mfront/src/MFrontPerformanceProfiling.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 22 août 2014
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
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
