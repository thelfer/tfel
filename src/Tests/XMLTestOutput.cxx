/*!
 * \file   XMLTestOutput.cxx
 * \author Helfer Thomas <helferthomas@free.fr>
 * \date   10 Apr 10
 * \brief  
 */

#include<fstream>
#include<iomanip>
#include<stdexcept>

#include"TFEL/Tests/XMLTestOutput.hxx"

namespace tfel
{

  namespace tests
  {

    static
    void replace_all(std::string& s,
		     const char c,
		     const std::string& n)
    {
      using namespace std;
      string::size_type p  = 0u;
      string::size_type ns = n.size();
      if((s.size()==0)||(ns==0)){
	return;
      }
      while((p=s.find(c,p))!=string::npos){
	s.replace(p,1u,n);
	p+=ns;
      }
    } // end of replace_all

    static void
    convert_to_xml(std::string& s)
    {
      replace_all(s,'&',"&amp;");
      replace_all(s,'<',"&lt;");
      replace_all(s,'>',"&gt;");
      replace_all(s,'"',"&quot;");
      replace_all(s,'\'',"&apos;");
    }

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
      string out(n);
      convert_to_xml(out);
      this->os << "<testsuite name=\""+out+"\">" << endl;
    } // end of XMLTestOutput::beginTestSuite

    void
    XMLTestOutput::addTest(const std::string& g,
			   const std::string& n,
			   const TestResult& r)
    {
      using namespace std;
      TestResult::const_iterator p;
      string gout(g);
      string nout(n);
      convert_to_xml(nout);
      convert_to_xml(gout);
      this->os << "<testcase classname=\"" << gout 
	       << "\" name=\"" << nout << "\" time=\""
	       << r.duration() << "\">" << endl;
      if(r.success()){
	this->os << "<success>" << endl;
      } else {
	this->os << "<failure>" << endl;
      }
      if(!r.details().empty()){
	string out(r.details());
	convert_to_xml(out);
	this->os << out << endl;
      }
      for(p=r.begin();p!=r.end();++p){
	this->treatTest(*p);
      }
      if(r.success()){
	this->os << "</success>" << endl;
      } else {
	this->os << "</failure>" << endl;
      }
      this->os << "</testcase>" << endl;
    } // end of XMLTestOutput::addTest

    void
    XMLTestOutput::treatTest(const TestResult& r)
    {
      using namespace std;
      TestResult::const_iterator p;
      string out;
      if(r.success()){
	out = "SUCCESS : ";
      } else {
	out = "FAILURE : ";
      }
      out += r.details();
      convert_to_xml(out);
      this->os << out << endl;
      for(p=r.begin();p!=r.end();++p){
	this->treatTest(*p);
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
