/*!
 * \file   TestLauncher.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   26 jan 2008
 */

#include<iterator>
#include<algorithm>
#include<fstream>
#include<sstream>
#include<cassert>
#include<cmath>
#include<limits>

#include <unistd.h> // sysconf

#include"TFEL/Utilities/TerminalColors.hxx"
#include"TFEL/System/ProcessManager.hxx"

#include"TFEL/Utilities/TestLauncher.hxx"

namespace tfel
{

  namespace utilities
  {

    static void
    TestLauncherReportStatus(std::ostream& log,
			     const std::string& command,
			     const bool success)
    {
      using namespace std;
      using namespace tfel::utilities;
      string name;
      name = command;
      if(name.size()<=80){
	name.insert(name.size(),80-name.size(),' ');
      }
      log  << name << " : ";
      cout << name << " : ";
      if(success){
	log  << "[SUCCESS]" << endl;
	cout << "[SUCCESS]" << endl;
      } else {
	log  << " [FAILED]" << endl;
	cout.write(TerminalColors::Red,sizeof(TerminalColors::Red));
	cout << " [FAILED]" << endl;
	cout.write(TerminalColors::Reset,sizeof(TerminalColors::Reset));
	cout.flush();
      }
    } // end of TestLauncherReportStatus

    TestLauncher::TestLauncher(const std::string& f,
			       std::ostream& l)
      : file(f),
	prec(1.e-8),
	testType(TestLauncher::Absolute),
	glog(l)
    {
      using namespace std;
      using namespace tfel::utilities;
      this->treatCharAsString(true);
      vector<Token> tmpLine;
      TokensContainer::const_iterator p;
      unsigned short currentLine;

      this->logfile = this->file.substr(0,this->file.rfind("."))+".checklog";

      // open log file
      log.open(logfile.c_str());
      if(!log){
	string msg("TestLauncher::TestLauncher : ");
	msg += "can't open log file "+ this->logfile;
	throw(runtime_error(msg));
      }
      // open the file
      this->openFile(this->file);
      // strip any C/C++ comments
      this->stripComments();
      // store tokens line by line
      p = this->fileTokens.begin();
      // check that we don't treat an empty file
      if(p==this->fileTokens.end()){
	string msg("TestLauncher::TestLauncher : ");
	msg += "file "+file+" is empty.";
	log << msg << endl;
	throw(runtime_error(msg));
      }
      currentLine = 0u;
      while(p!=this->fileTokens.end()){
	if(p->line!=currentLine){
	  if(!tmpLine.empty()){
	    this->tokens.push_back(tmpLine);
	  }
	  currentLine=p->line;
	  tmpLine.clear();
	}
	tmpLine.push_back(*p);
	++p;
      }
      this->tokens.push_back(tmpLine);
      // a bit of clean-up
      this->fileTokens.clear();
      // callbacks
      this->registerCallBack("MeshCommand",&TestLauncher::treatMeshCommand);
      this->registerCallBack("Command",&TestLauncher::treatCommand);
      this->registerCallBack("Test",&TestLauncher::treatTest);
      this->registerCallBack("TestType",&TestLauncher::treatTestType);
      this->registerCallBack("Precision",&TestLauncher::treatPrecision);
      // analysing the file
      this->analyseInputFile();

      this->XMLResult = "<?xml version=\"1.0\" ?>\n<testsuite name=\"tfel-check\">\n</testsuite>\n";

    } // end of TestLauncher::TestLauncher

    void TestLauncher::addTestcaseXML(const std::string& name, 
				      const std::string& co, 
				      const float time,
				      const bool success, 
				      const std::string& message="") {
      
      std::string testcase;
      std::ostringstream stime;
      stime << time;
      const std::string& test = this->file.substr(0,this->file.rfind("."));
      
      // build testcase XML string
      testcase = "<testcase classname=\"" + test + "\" name=\"" + name + "\" time=\"" + stime.str() + "\">\n";
      if(success){
	testcase += "<success>\n";
      } else {
	testcase += "<failure>\n";
      }
      testcase += "Command was : " + co + "\n";
      testcase += message ;
      if(success){
	testcase += "</success>\n";
      } else {
	testcase += "</failure>\n";
      }
      testcase += "</testcase>\n";
      // insert at the end of XMLResult
      this->XMLResult.insert(this->XMLResult.find("</testsuite>\n"), testcase);
    }

    void TestLauncher::addTestcaseXML(const std::string& name, 
				      const float time,
				      const bool success, 
				      const std::string& message="") {
      
      std::string testcase;
      std::ostringstream stime;
      stime << time;
      const std::string& test = this->file.substr(0,this->file.rfind("."));
      
      // build testcase XML string
      testcase = "<testcase classname=\"" + test + "\" name=\"" + name + "\" time=\"" + stime.str() + "\">\n";
      if(success){
	testcase += "<success>\n";
      } else {
	testcase += "<failure>\n";
      }
      testcase += message ;
      if(success){
	testcase += "</success>\n";
      } else {
	testcase += "</failure>\n";
      }
      testcase += "</testcase>\n";
      // insert at the end of XMLResult
      this->XMLResult.insert(this->XMLResult.find("</testsuite>\n"), testcase);
    }

    void TestLauncher::writeXML() {
      std::ofstream outfile;
      std::string filename;
      
      // writing to TEST-<filename w/o .check>.xml 
      filename = "TEST-" + (this->file.substr(0,this->file.rfind("."))+".xml");
      outfile.open(filename.c_str());
      outfile << this->XMLResult << std::endl;
      outfile.close();
    }

    std::string
    TestLauncher::logToString () {
      std::ifstream ilog;
      std::string buf;
      std::string l;
      
      ilog.open(this->logfile.c_str());
      if( ! ilog ) 
	return std::string("No log file...") ;
      while (getline(ilog, l)) {
	buf += l + "\n";
      }
      ilog.close();
      return buf;
    }


    void
    TestLauncher::registerCallBack(const std::string& name,
				  const CallBack& f)
    {
      using namespace std;
      typedef map<string,CallBack>::value_type MVType;
      if(!this->callBacks.insert(MVType(name,f)).second){
	string msg("TestLauncher::registerCallBack : ");
	msg += "keyword " + name + " has already been registred.";
	throw(runtime_error(msg));
      }
    } // end of TestLauncher::registerCallBack

    void
    TestLauncher::treatCommand(void)
    {
      using namespace std;
      using namespace tfel::utilities;
      if(this->current==this->line->end()){
	ostringstream msg;
	msg << "TestLauncher::treatCommand : ";
	msg << " unexpected end of line.\n";
	msg <<  "Error at line " << this->line->begin()->line;
	log << msg.str() << endl;
	throw(runtime_error(msg.str()));
      }
      if((this->current->flag!=Token::String)&&
	 (this->current->flag!=Token::Char)){
	ostringstream msg;
	msg << "TestLauncher::treatCommand : ";
	msg <<  "invalid line " << this->line->begin()->line;
	msg << " (first filename is invalid)";
	log << msg.str() << endl;
	throw(runtime_error(msg.str()));
      }
      this->commands.push_back(this->current->value.substr(1,this->current->value.size()-2));
      ++(this->current);
    } // end of TestLauncher::treatTestCommand

    void
    TestLauncher::treatMeshCommand(void)
    {
      using namespace std;
      using namespace tfel::utilities;
      if(this->current==this->line->end()){
	ostringstream msg;
	msg << "TestLauncher::treatMeshCommand : ";
	msg << " unexpected end of line.\n";
	msg <<  "Error at line " << this->line->begin()->line;
	log << msg.str() << endl;
	throw(runtime_error(msg.str()));
      }
      if((this->current->flag!=Token::String)&&
	 (this->current->flag!=Token::Char)){
	ostringstream msg;
	msg << "TestLauncher::treatMeshCommand : ";
	msg <<  "invalid line " << this->line->begin()->line;
	msg << " (first filename is invalid)";
	log << msg.str() << endl;
	throw(runtime_error(msg.str()));
      }
      this->meshCommands.push_back(this->current->value.substr(1,this->current->value.size()-2));
      ++(this->current);
    } // end of TestLauncher::treatTestMeshCommand

    void
    TestLauncher::treatTestType(void)
    {
      using namespace std;
      if(this->current==this->line->end()){
	ostringstream msg;
	msg << "TestLauncher::treatTest : ";
	msg << " unexpected end of line.\n";
	msg <<  "Error at line " << this->line->begin()->line;
	log << msg.str() << endl;
	throw(runtime_error(msg.str()));
      }
      if(this->current->value=="Absolute"){
	this->testType = TestLauncher::Absolute;
      } else if(this->current->value=="Relative"){
	this->testType = TestLauncher::Relative;
      } else {
	ostringstream msg;
	msg << "TestLauncher::treatTest : ";
	msg << " unknown test type (read '"+this->current->value+"').\n";
	msg <<  "Error at line " << this->line->begin()->line;
	log << msg.str() << endl;
	throw(runtime_error(msg.str()));
      }
      ++(this->current);
    } // end of TestLauncher::treatTestType

    void
    TestLauncher::treatTest(void)
    {
      using namespace std;
      using namespace tfel::utilities;
      if(this->current==this->line->end()){
	ostringstream msg;
	msg << "TestLauncher::treatTest : ";
	msg << " unexpected end of line.\n";
	msg <<  "Error at line " << this->line->begin()->line;
	log << msg.str() << endl;
	throw(runtime_error(msg.str()));
      }
      if((this->current->flag!=Token::String)&&
	 (this->current->flag!=Token::Char)){
	ostringstream msg;
	msg << "TestLauncher::treatTest : ";
	msg <<  "invalid line " << this->line->begin()->line;
	msg << " (first filename is invalid)";
	log << msg.str() << endl;
	throw(runtime_error(msg.str()));
      }
      string a = this->current->value.substr(1,this->current->value.size()-2);
      ++(this->current);
      if(this->current==this->line->end()){
	ostringstream msg;
	msg << "TestLauncher::treatTest : ";
	msg << " unexpected end of line.\n";
	msg <<  "Error at line " << this->line->begin()->line;
	log << msg.str() << endl;
	throw(runtime_error(msg.str()));
      }
      if((this->current->flag!=Token::String)&&
	 (this->current->flag!=Token::Char)){
	ostringstream msg;
	msg << "TestLauncher::treatTest : ";
	msg <<  "invalid line " << this->line->begin()->line;
	msg << " (first filename is invalid)";
	log << msg.str() << endl;
	throw(runtime_error(msg.str()));
      }
      string b = this->current->value.substr(1,this->current->value.size()-2);
      ++(this->current);
      if(this->current==this->line->end()){
	ostringstream msg;
	msg << "TestLauncher::treatTest : ";
	msg << " unexpected end of line.\n";
	msg <<  "Error at line " << this->line->begin()->line;
	log << msg.str() << endl;
	throw(runtime_error(msg.str()));
      }
      while(this->current!=this->line->end()){
	istringstream converter(this->current->value);
	Test test;
	test.prec = this->prec;
	test.type = this->testType;
	if((this->current->flag==Token::String)||
	   (this->current->flag==Token::Char)){
	  test.byName = true;
	  test.name   = this->current->value.substr(1,this->current->value.size()-2);
	} else {
	  unsigned short c;
	  converter >> c;
	  if(!converter&&(!converter.eof())){                                                 
	    ostringstream msg;                                                  
	    msg << "TestLauncher::treatTest : ";                          
	    msg << "could not read value from token (" << this->current->value << ").\n";      
	    msg << "Error at line : " << this->line->begin()->line;
	    log << msg.str() << endl;
	    throw(runtime_error(msg.str()));                                    
	  }
	  test.byName = false;
	  test.column = c;
	}
	this->comparisons[a][b].push_back(test);
	++(this->current);
      }
    } // end of TestLauncher::treatTest

    void
    TestLauncher::treatPrecision(void)
    {
      using namespace std;
      if(this->current==this->line->end()){
	ostringstream msg;
	msg << "TestLauncher::treatPrecision : ";
	msg << " unexpected end of line.\n";
	msg <<  "Error at line " << this->line->begin()->line;
	log << msg.str() << endl;
	throw(runtime_error(msg.str()));
      }
      istringstream converter(this->current->value);
      double c;
      converter >> c;
      if(!converter&&(!converter.eof())){                                                 
	ostringstream msg;                                                  
	msg << "TestLauncher::treatPrecision : ";                          
	msg << "could not read value from token (" << this->current->value << ").\n";      
	msg << "Error at line : " << this->line->begin()->line;
	log << msg.str() << endl;
	throw(runtime_error(msg.str()));                                    
      }
      this->prec = c;
      ++(this->current);
    } // end of TestLauncher::treatPrecision

    void
    TestLauncher::analyseInputFile(void)
    {
      using namespace std;
      map<string,CallBack>::const_iterator p;
      for(this->line=this->tokens.begin();this->line!=this->tokens.end();++(this->line)){
	this->current = this->line->begin();
	assert(this->current!=this->line->end());
	p = this->callBacks.find(this->current->value);
	if(p==this->callBacks.end()){
	  ostringstream msg;
	  msg << "TestLauncher::analyseInputFile : ";
	  msg << "unknown keyword " << this->current->value << ".\n";
	  msg << "Error at line " << this->line->begin()->line;
	  log << msg.str() << endl;
	  throw(runtime_error(msg.str()));
	}
	++(this->current);
	(this->*(p->second))();
	if(this->current!=this->line->end()){
	  ostringstream msg;
	  msg << "TestLauncher::analyseInputFile : ";
	  msg << "some tokens remains on line " <<  this->line->begin()->line 
	      << " after treatment.";
	  log << msg.str() << endl;
	  throw(runtime_error(msg.str()));
	}
      }
      // a bit of clean-up
      this->tokens.clear();
    } // end of TestLauncher::analyseInputFile

    double TestLauncher::ClockAction(ClockEventType clockevent) {
      using namespace std;
      
      clock_t r;

      switch (clockevent) {
      case START :
	r = times(&this->timeStart);
	if (r == (clock_t) -1) 
	  throw(runtime_error("Failed times system call in TestLauncher::ClockAction"));
	break;
      case STOP :
	r = times(&this->timeStop);
	if (r == (clock_t) -1) 
	  throw(runtime_error("Failed times system call in TestLauncher::ClockAction"));
	break;
      case GET :
	return static_cast<double>((this->timeStop.tms_cutime - this->timeStart.tms_cutime) / sysconf(_SC_CLK_TCK));
	break;
      default :
	throw(runtime_error("Wrong clockevent in TestLauncher::ClockAction"));	
      }
      return 0.;
    }
    
    bool
    TestLauncher::execute(void)
    {
      using namespace std;
      using namespace tfel::system;
      using namespace tfel::utilities;
      using std::exception;
      static const double eps = 100.*numeric_limits<double>::min();
      ProcessManager manager;
      map<string,map<string,vector<Test> > >::const_iterator p;
      map<string,vector<Test> >::const_iterator p2;
      vector<Test>::const_iterator p3;
      vector<string>::const_iterator p4;
      vector<double>::size_type index;
      double error;
      double max_error;
      bool gsuccess = true;
      bool success  = true;
      vector<string>::size_type i=0;

      // Mesh
      if(!this->meshCommands.empty()){
	for(i=0,p4=this->meshCommands.begin();p4!=this->meshCommands.end();++p4,++i){
	  success = true;
	  try{
	    this->ClockAction(START);
	    ostringstream out;
	    out << this->file.substr(0,this->file.rfind(".")) << "-mesh"
		<< i << ".out";
	    manager.execute(*p4,"",out.str());
	    this->ClockAction(STOP);
	    this->addTestcaseXML("Mesh",*p4, this->ClockAction(GET), true);
	  }
	  catch(exception& e){
	    log << "mesh build failed : " << e.what() << endl;
	    this->addTestcaseXML("Mesh",*p4, 0.0, false, e.what());
	    success = false;
	  }
	  TestLauncherReportStatus(glog,"** result of '"+*p4+"'",success);
	}
      }

      // Execute
      if(!this->commands.empty()){
	for(i=0,p4=this->commands.begin();p4!=this->commands.end();++p4,++i){
	  success = true;
	  try{
	    this->ClockAction(START);
	    ostringstream out;
	    out << this->file.substr(0,this->file.rfind("."))
		<< i << ".out";
	    manager.execute(*p4,"",out.str());
	    this->ClockAction(STOP);
	    this->addTestcaseXML("Exec",*p4, this->ClockAction(GET), true);
	  }
	  catch(exception& e){
	    log << "computations failed : " << e.what() << endl;
	    this->addTestcaseXML("Exec", *p4 , 0.0, false, e.what());
	    success = false;
	  }
	  TestLauncherReportStatus(glog,"** result of '"+*p4+"'",success);
	}
      }

      // Compare
      for(p=this->comparisons.begin();p!=this->comparisons.end();++p){
	success = true;
	for(p2=p->second.begin();p2!=p->second.end();++p2){
	  try {
	    TextData a(p->first);
	    TextData b(p2->first);
	    for(p3=p2->second.begin();p3!=p2->second.end();++p3){
	      log << "comparing files " << p->first << " and "
		  << p2->first << " using column ";
	      if(p3->byName){
		log << p3->name << endl; 
	      } else {
		log << p3->column << endl;
	      }
	      unsigned short colA;
	      unsigned short colB;
	      if(p3->byName){
		colA = a.findColumn(p3->name);
		colB = b.findColumn(p3->name);
	      } else {
		colA = p3->column;
		colB = p3->column;
	      }
	      const vector<double>& ca = a.getColumn(colA);
	      const vector<double>& cb = b.getColumn(colB);
	      if(ca.size()!=cb.size()){
		ostringstream msg;
		msg << "TestLauncher::execute : ";
		msg << "columns ";
		if(p3->byName){
		  msg << p3->name; 
		} else {
		  msg << p3->column;
		}		
		msg <<  " of files " << p->first << " and " << p2->first 
		    <<  " don't have the same size";
		log << msg.str() << endl;
		success = false;
	      } else {
		ostringstream msg;
		max_error = 0.;
		bool s = true;
		// Absolute comparison 
		if(p3->type==TestLauncher::Absolute){
		  for(index=0;index!=ca.size();++index){
		    error = abs(ca[index]-cb[index]);
		    if(max_error<error){
		      max_error = error;
		    }
		    if(error>p3->prec){
		      s = false;
		    }
		  }
		  if(!s){
		    msg << "comparison of files " << p->first << " and "
			<< p2->first << " using column ";
		    if(p3->byName){
		      msg << p3->name; 
		    } else {
		      msg << p3->column;
		    }
		    msg << " failed with an maximun absolute error of " 
			<< max_error << endl;
		    log << msg.str() << endl;
		    success = false;
		  } else {
		    msg << "comparison of files " << p->first << " and "
			<< p2->first << " using column ";
		    if(p3->byName){
		      msg << p3->name; 
		    } else {
		      msg << p3->column;
		    }
		    msg << " succeed with an maximun absolute error of " 
			<< max_error << endl;
		    log << msg.str() << endl;
		  }
		  
		} else { 
		  //Relative Comparison
		  for(index=0;index!=ca.size();++index){
		    error = abs(ca[index]-cb[index])/(min(abs(ca[index]),abs(cb[index]))+eps);
		    if(max_error<error){
		      max_error = error;
		    }
		    if(error>(p3->prec)){
		      s = false;
		    }
		  }
		  if(!s){
		    msg << "comparison of files " << p->first << " and "
			<< p2->first << " using column ";
		    if(p3->byName){
		      msg << p3->name; 
		    } else {
		      msg << p3->column;
		    }
		    msg << " failed with an maximun relative error of " 
			<< max_error << endl;
		    log << msg.str() << endl;
		    success = false;
		  } else {
		    msg << "comparison of files " << p->first << " and "
			<< p2->first << " using column ";
		    if(p3->byName){
		      msg << p3->name; 
		    } else {
		      msg << p3->column;
		    }
		    msg << " succeed with an maximun relative error of " 
			<< max_error << endl;
		    log << msg.str() << endl;
		  }
		}
	      }
	    }
	  }
	  catch(exception& e){
	    log << "file comparison failed : " << e.what() << endl;
	    gsuccess=false;
	    success=false;
	  }
	  TestLauncherReportStatus(glog,"** comparing files '"+p->first+"' and '"+p2->first+"'",success);
	  gsuccess = success;
	}
      }
      this->addTestcaseXML("Compare", 0.0, success, this->logToString());  
      return gsuccess;
    } // end of TestLauncher::execute

    TestLauncher::~TestLauncher() {
      this->writeXML();
    }

  } // end of namespace utilities

} // end of namespace tfel

