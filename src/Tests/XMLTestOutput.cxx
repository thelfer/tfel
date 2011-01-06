/*!
 * \file   XMLTestOutput.cxx
 * \author Helfer Thomas <helferthomas@free.fr>
 * \date   10 Apr 10
 * \brief  
 */

#include<fstream>
#include<stdexcept>

#include"TFEL/Tests/XMLTestOutput.hxx"

namespace tfel
{

  namespace tests
  {

    XMLTestOutput::XMLTestOutput(const std::string& o)
      : pos(new std::ofstream(o.c_str())),
	os(*(pos.get()))
    {
      using namespace std;
      if(!(*pos)){
	string msg("XMLTestOutput::XMLTestOutput : ");
	msg += "can't open file '"+o+"'";
	throw(runtime_error(msg));
      }
      this->os << "<?xml version=\"1.0\" ?>" << endl;
    } // end of XMLTestOutput::XMLTestOutput
    
    void
    XMLTestOutput::beginTestSuite(const std::string& n)
    {
      using namespace std;
      this->os << "<testsuite name=\""+n+"\">" << endl;
    } // end of XMLTestOutput::beginTestSuite

    void
    XMLTestOutput::addTest(const std::string& g,
			   const std::string& n,
			   const TestResult& r)
    {
      using namespace std;
      TestResult::const_iterator p;
      this->os << "<testcase classname=\"" << g << "\" name=\""+n+"\">" << endl;
      if(r.success()){
	this->os << "<success>" << endl;
      } else {
	this->os << "<failure>" << endl;
      }
      this->os << "<![CDATA[" << endl;
      if(!r.details().empty()){
	this->os << r.details() << endl;
      }
      for(p=r.begin();p!=r.end();++p){
	this->treatTest(*p,"* ");
      }
      this->os << "]]>" << endl;
      if(r.success()){
	this->os << "</success>" << endl;
      } else {
	this->os << "</failure>" << endl;
      }
      this->os << "</testcase>" << endl;
    } // end of XMLTestOutput::addTest

    void
    XMLTestOutput::treatTest(const TestResult& r,
			     const std::string& s)
    {
      using namespace std;
      TestResult::const_iterator p;
      string out(s);
      out += r.details();
      if(out.size()<65){
	out.resize(65,' ');
      } else {
	out = out.substr(0,65);
      }
      if(r.success()){
	this->os << out << ": SUCCESS" << endl;
      } else {
	this->os << out << ": ";
	this->os << "FAILED" << endl;
      }
      for(p=r.begin();p!=r.end();++p){
	this->treatTest(*p,s+'*');
      }
    } // end of XMLTestOutput::treatTest

    void
    XMLTestOutput::endTestSuite(const TestResult&)
    {
      using namespace std;
      this->os << "</testsuite>" << endl;
    } // end of XMLTestOutput::endTestSuite
  
    XMLTestOutput::~XMLTestOutput()
    {} // end of XMLTestOutput::~XMLTestOutput

  } // end of namespace tests

} // end of namespace tfel
