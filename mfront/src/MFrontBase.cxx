/*! 
 * \file  MFrontBase.cxx
 * \brief
 * \author Helfer Thomas
 * \date   04 mars 2015
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<sstream>
#include<stdexcept>

#include"TFEL/Utilities/CxxTokenizer.hxx"
#include"TFEL/Utilities/StringAlgorithms.hxx"
#include"TFEL/System/ExternalLibraryManager.hxx"

#include"MFront/SearchFile.hxx"
#include"MFront/PedanticMode.hxx"
#include"MFront/MFrontLogStream.hxx"
#include"MFront/MFrontDebugMode.hxx"
#include"MFront/DSLFactory.hxx"
#include"MFront/MFrontBase.hxx"

namespace mfront{

  std::shared_ptr<AbstractDSL>
  MFrontBase::getDSL(const std::string& f)
  {
    using namespace std;
    using namespace tfel::system;
    using namespace tfel::utilities;
    auto& dslFactory = DSLFactory::getDSLFactory();
    shared_ptr<AbstractDSL> dsl;
    string library,dslName;
    CxxTokenizer file(f);
    file.stripComments();
    auto pt = file.begin();
    const auto pte = file.end();
    bool found=false;
    while((pt!=pte)&&(!found)){
      if((pt->value=="@Parser")||(pt->value=="@DSL")){
	if(pt!=file.begin()){
	  auto ptp = pt;
	  --ptp;
	  if(ptp->value!=";"){
	    string msg("MFrontBase::getDSL : ");
	    msg += "the keyword @Parser or @DSL does not begin a new instruction.";
	    throw(runtime_error(msg));
	  }
	}
	++pt;
	if(pt==pte){
	  ostringstream msg;
	  msg << "MFrontBase::getDSL : ";
	  msg << "unexepected end of file (exepected dsl name).\n";
	  msg << "Error at line " << pt->line << ".";
	  throw(runtime_error(msg.str()));
	}
	if(pt->value==";"){
	  ostringstream msg;
	  msg << "MFrontBase::getDSL : ";
	  msg << "unexepected end of file (exepected dsl name).\n";
	  msg << "Error at line " << pt->line << ".";
	  throw(runtime_error(msg.str()));
	}
	dslName = pt->value;
	++pt;
	if(pt==pte){
	  ostringstream msg;
	  msg << "MFrontBase::getDSL : ";
	  msg << "unexepected end of file (exepected library name or ';').\n";
	  msg << "Error at line " << pt->line << ".";
	  throw(runtime_error(msg.str()));
	}
	if(pt->value!=";"){
	  library = pt->value;
	  ++pt;
	  if(pt==pte){
	    ostringstream msg;
	    msg << "MFrontBase::getDSL : ";
	    msg << "unexepected end of file (exepected ';').\n";
	    msg << "Error at line " << pt->line << ".";
	    throw(runtime_error(msg.str()));
	  }
	  if(pt->value!=";"){
	    ostringstream msg;
	    msg << "MFrontBase::getDSL : ";
	    msg << "unexepected token '" << pt->value << "' (exepected ';').\n";
	    msg << "Error at line " << pt->line << ".";
	    throw(runtime_error(msg.str()));
	  }
	}
	found = true;
      }
      ++pt;
    }
    if(found){
      try{
	if(!library.empty()){
	  auto& lm = ExternalLibraryManager::getExternalLibraryManager();
	  lm.loadLibrary(library);
	}
	dsl = dslFactory.createNewParser(dslName);
      } 
      catch(runtime_error& r){
	ostringstream msg;
	msg << "MFrontBase::getDSL : error while loading dsl "
	    << dslName << " (" << r.what() << ")\n";
	msg << "Available dsls : " << endl;
	const auto& dsls = dslFactory.getRegistredParsers();
	copy(dsls.begin(),dsls.end(),ostream_iterator<string>(msg," "));
	throw(runtime_error(msg.str()));
      }
    } else {
      if(getVerboseMode()>=VERBOSE_LEVEL2){
	auto& log = getLogStream();
	log << "MFrontBase::getDSL : no dsl specified, using default" << endl;
      }
      dsl = dslFactory.createNewParser();
    }
    return dsl;
  } // end of MFrontBase::getAbstractDSL

  MFrontBase::MFrontBase()
  {
    using namespace tfel::system;
    using namespace tfel::utilities;
    // calling mfront plugins
    const char * libs = ::getenv("MFRONT_ADDITIONAL_LIBRARIES");
    if(libs!=nullptr){
      auto& lm = ExternalLibraryManager::getExternalLibraryManager();
      for(const auto& l : tokenize(libs,':')){
	lm.loadLibrary(l);
      }
    }
  } // end of MFrontBase

  void
  MFrontBase::treatSearchPath(void)
  {
    using namespace std;
    const auto& o = this->getCurrentCommandLineArgument().getOption();
    if(o.empty()){
      throw(runtime_error("MFrontBase::treatSearchPath : "
			  "no path given"));
    }
    SearchFile::addSearchPaths(o);
  }

  bool
  MFrontBase::treatUnknownArgumentBase(void){
    const auto& a = this->getCurrentCommandLineArgument();
    if(a[0]=='-'){
      bool ok = false;
      if(a.size()>=4){
	if((a[1]=='-')&&(a[2]=='@')){
	  const auto& o = a.getOption();
	  auto cmd = a.substr(2);
	  if(!o.empty()){
	    cmd += ' '+o;
	  }
	  cmd += ';';
	  this->ecmds.push_back(cmd);
	  ok = true;
	}
      }
      return ok;
    }
    this->inputs.insert(a);
    return true;
  } // end of MFrontBase::treatUnknownArgument

  void
  MFrontBase::treatVerbose(void)
  {
    using namespace std;
    if(getCurrentCommandLineArgument().getOption().empty()){
      setVerboseMode(VERBOSE_LEVEL1);
    } else {
      const auto& o = this->getCurrentCommandLineArgument().getOption();
      if(o=="quiet"){
	setVerboseMode(VERBOSE_QUIET);
      } else if(o=="level0"){
	setVerboseMode(VERBOSE_LEVEL0);
      } else if(o=="level1"){
	setVerboseMode(VERBOSE_LEVEL1);
      } else if (o=="level2"){
	setVerboseMode(VERBOSE_LEVEL2);
      } else if (o=="level3"){
	setVerboseMode(VERBOSE_LEVEL3);
      } else if (o=="debug"){
	setVerboseMode(VERBOSE_DEBUG);
      } else if (o=="full"){
	setVerboseMode(VERBOSE_FULL);
      } else {
	string msg("MTestMain::treatVerbose : ");
	msg += "unknown option '"+o+"'";
	throw(runtime_error(msg));
      }
    }
  }

  void
  MFrontBase::treatDebug(void)
  {
    setDebugMode(true);
  }

  void
  MFrontBase::treatPedantic(void)
  {
    setPedanticMode(true);
  }

  void
  MFrontBase::treatWarning(void)
  {}

  void
  MFrontBase::setInterface(const std::string& i){
    using std::runtime_error;
    if(!this->interfaces.insert(i).second){
      throw(runtime_error("MFrontBase::treatInterface : "
			  "the interface '"+i+"' has "
			  "already been specified"));
    }
  } // end of MFrontBase::setInterface
  
  void
  MFrontBase::treatInterface(void)
  {
    using std::runtime_error;
    const auto& o = this->getCurrentCommandLineArgument().getOption();
    if(o.empty()){
      throw(runtime_error("MFrontBase::treatInterface : "
			  "no option given to the "
			  "'--interface' argument"));
    }
    for(const auto& i : tfel::utilities::tokenize(o,',')){
      if(i.empty()){
	throw(runtime_error("MFrontBase::treatInterface : "
			    "empty interface specified."));
      }
      this->setInterface(i);
    }
  } // end of MFrontBase::treatInterface

  MFrontBase::~MFrontBase()
  {} // end of MFrontBase::~MFrontBase()

} // end of namespace mfront
