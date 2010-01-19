/*!
 * \file   MFront.cxx
 * \brief  
 * \author Helfer Thomas
 * \date   09 nov 2006
 */

#include<iostream>
#include<cstdlib>
#include<fstream>
#include<sstream>
#include<string>
#include<iterator>
#include<stdexcept>
#include<cerrno>
#include<cassert>

#include<dlfcn.h> 
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include<dirent.h>
#include<unistd.h>

#include"Utilities/CxxTokenizer.hxx"
#include"System/ExternalLibraryManager.hxx"
#include"System/System.hxx"

#include"SupportedTypes.hxx"
#include"MFront.hxx"
#include"MFrontHeader.hxx"
#include"MFrontParserFactory.hxx"
#include"MFrontBehaviourInterfaceFactory.hxx"
#include"MFrontLawInterfaceFactory.hxx"
#include"MFrontLock.hxx"

namespace mfront{

  std::string MFront::callingName("mfront");

  std::vector<std::string>
  MFront::tokenize(const std::string& s,
		   const char c)
  {
    using namespace std;
    vector<string> res;
    string::size_type b = 0u;
    string::size_type e = s.find_first_of(c, b);
    while (string::npos != e || string::npos != b){
      // Found a token, add it to the vector.
      res.push_back(s.substr(b, e - b));
      b = s.find_first_not_of(c, e);
      e = s.find_first_of(c, b);
    }
    return res;
  } // end of MFront::tokenize

  const std::string&
  MFront::getCallingName(void){
    return MFront::callingName;
  } // end of MFront::getCallingName

  std::string 
  MFront::getVersionDescription(void) const
  {
    using namespace std;
    return MFrontHeader::getHeader();
 }

  std::string 
  MFront::getUsageDescription(void) const
  {
    using namespace std;
    string usage("Usage : ");
    usage += this->programName;
    usage += " [options] [files]";
    return usage;
  }

  void
  MFront::treatUnknownArgument(void)
  {
    using namespace std;
    using namespace tfel::utilities;
    if((*(this->currentArgument))[0]=='-'){
#ifndef __CYGWIN__
      ArgumentParserBase<MFront>::treatUnknownArgument();
#else
      cerr << "mfront : unsupported option '" << *(this->currentArgument) << "'\n";
      exit(EXIT_FAILURE);
#endif /* __CYGWIN__ */
    }
    this->inputs.insert(*(this->currentArgument));
    return;
  } // end of MFront::treatUnknownArgument()

  void
  MFront::treatVerbose(void)
  {
    typedef MFrontBehaviourInterfaceFactory MBIF;
    typedef MFrontLawInterfaceFactory MLIF;
    MBIF& mbif = MBIF::getMFrontBehaviourInterfaceFactory();
    MLIF& mlif = MLIF::getMFrontLawInterfaceFactory();
    this->verboseMode = true;
    mbif.setVerboseMode();
    mlif.setVerboseMode();
  }

  void
  MFront::treatDebug(void)
  {
    typedef MFrontBehaviourInterfaceFactory MBIF;
    typedef MFrontLawInterfaceFactory MLIF;
    MBIF& mbif = MBIF::getMFrontBehaviourInterfaceFactory();
    MLIF& mlif = MLIF::getMFrontLawInterfaceFactory();
    this->debugMode = true;
    mbif.setDebugMode();
    mlif.setDebugMode();
  }

  void
  MFront::treatWarning(void)
  {
    typedef MFrontBehaviourInterfaceFactory MBIF;
    typedef MFrontLawInterfaceFactory MLIF;
    MBIF& mbif = MBIF::getMFrontBehaviourInterfaceFactory();
    MLIF& mlif = MLIF::getMFrontLawInterfaceFactory();
    this->warningMode = true;
    mbif.setWarningMode();
    mlif.setWarningMode();
  }

  void
  MFront::treatMake(void)
  {
    this->genMake = true;
  } // end of MFront::treatMake

  void
  MFront::treatBuild(void)
  {
    this->genMake   = true;
    this->buildLibs = true;
  } // end of MFront::treatBuild

  void
  MFront::treatClean(void)
  {
    this->genMake   = true;
    this->cleanLibs = true;
  } // end of MFront::treatBuild

  void
  MFront::treatOMake(void)
  {
    this->oflags  = true;
    this->genMake = true;
  } // end of MFront::treatOMake

  void
  MFront::treatOBuild(void)
  {
    this->oflags    = true;
    this->genMake   = true;
    this->buildLibs = true;
  } // end of MFront::treatOBuild

  void
  MFront::treatInterface(void)
  {
    using namespace std;
    string interface;
    string tmp;
    string::size_type n;
    string::size_type n2;
    interface = this->currentArgument->getOption();
    if(interface.empty()){
      string msg("MFront::treatInterface : ");
      msg += "no option given to the --interface argument";
      throw(runtime_error(msg));
    }
    n = 0u;
    n2=interface.find(',',n);
    while(n2!=string::npos){
      tmp = interface.substr(n,n2-n);
      if(tmp.empty()){
	string msg("MFront::treatInterface : ");
	msg += "empty interface specified.";
	throw(runtime_error(msg));
      }
      if(!this->interfaces.insert(tmp).second){
	string msg("MFront::treatInterface : ");
	msg += "the interface "+tmp+" has already been specified";
	throw(runtime_error(msg));
      }
      n=n2+1;
      n2=interface.find(',',n);
    }
    tmp = interface.substr(n,n2-n);
    if(tmp.empty()){
      string msg("MFront::treatInterface : ");
      msg += "empty interface specified.";
      throw(runtime_error(msg));
    }
    if(!this->interfaces.insert(tmp).second){
      string msg("MFront::treatInterface : ");
      msg += "the interface "+tmp+" has already been specified";
      throw(runtime_error(msg));
    }
  } // end of MFront::treatOMake

  void
  MFront::treatListParsers(void)
  {
    using namespace std;
    cout << "available parsers : " << endl;
    MFrontParserFactory& parserFactory = MFrontParserFactory::getMFrontParserFactory();
    const vector<string>& parsers = parserFactory.getRegistredParsers();
    vector<string>::const_iterator p = parsers.begin();
    while(p!=parsers.end()){
      string tmp;
      tmp = "- " + *p;
      if(tmp.size()<=32){
	tmp.insert(tmp.size(),32-tmp.size(),' ');
      }
      cout << tmp << " : " << parserFactory.getParserDescription(*p) << ".\n";
      ++p;
    }
    exit(EXIT_SUCCESS);
  } // end of MFront::treatListParsers

  void
  MFront::treatTarget(void)
  {
    using namespace std;
    const vector<string>& t = MFront::tokenize(this->currentArgument->getOption(),',');
    if(t.empty()){
      string msg("MFront::treatTarget : ");
      msg += "no argument given to the --target option";
      throw(runtime_error(msg));
    }
    this->specifiedTargets.insert(t.begin(),t.end());
    this->genMake   = true;
    this->buildLibs = true;
  } // end of MFront::treatTarget

  void
  MFront::treatOTarget(void)
  {
    using namespace std;
    if(this->currentArgument==this->args.end()){
      string msg("MFront::treatTarget : ");
      msg += "no argument given to the --otarget option";
      throw(runtime_error(msg));
    }
    this->oflags    = true;
    this->treatTarget();
  } // end of MFront::treatTarget

#ifndef __CYGWIN__
  void
  MFront::treatWin32(void)
  {
    this->sys = "win32";
  } // end of MFront::treatWin32
#endif /* __CYGWIN__ */

  void 
  MFront::registerArgumentCallBacks(void)
  {
    this->registerNewCallBack("--verbose",&MFront::treatVerbose,"set verbose output");
    this->registerNewCallBack("--list-parsers",&MFront::treatListParsers,"list all available parsers");
    this->registerNewCallBack("--debug",&MFront::treatDebug,
			      "set debug mode (remove references to initial file)");
    this->registerNewCallBack("--warning","-W",&MFront::treatWarning,"print warnings");
    this->registerNewCallBack("--make",&MFront::treatMake,
			      "generate MakeFile (see also --build)");
    this->registerNewCallBack("--build",&MFront::treatBuild,
			      "generate MakeFile and build libraries");
    this->registerNewCallBack("--omake","-m",&MFront::treatOMake,
			      "generate MakeFile with optimized compilations flags (see also --obuild)");
    this->registerNewCallBack("--obuild","-b",&MFront::treatOBuild,
			      "generate MakeFile with optimized compilations flags and build libraries");
    this->registerNewCallBack("--target","-t",&MFront::treatTarget,
			      "generate MakeFile and build the specified target",true);
    this->registerNewCallBack("--otarget",&MFront::treatOTarget,
			      "generate MakeFile with optimized compilations flags and build the specified target",true);
    this->registerNewCallBack("--clean",&MFront::treatClean,
			      "generate MakeFile and clean libraries");
    this->registerNewCallBack("--interface","-i",&MFront::treatInterface,
			      "specify witch interface to use",true);
    this->registerNewCallBack("--nodeps",&MFront::treatNoDeps,
			      "don't generate compilation dependencies");
    this->registerNewCallBack("--nomelt",&MFront::treatNoMelt,
			      "don't melt librairies sources");
#ifndef __CYGWIN__
    this->registerNewCallBack("--win32",&MFront::treatWin32,
			      "specify that the target system is win32");
#endif /* __CYGWIN__ */
  } // end of MFront::registerArgumentCallBacks

  MFront::MFront(const int argc, const char *const *const argv)
    : tfel::utilities::ArgumentParserBase<MFront>(argc,argv),
#ifdef __CYGWIN__
      sys("win32"),
#else
      sys("default"),
#endif /* __CYGWIN__ */
      verboseMode(false),
      warningMode(false),
      debugMode(false),
      oflags(false),
      genMake(false),
      buildLibs(false),
      cleanLibs(false),
      nodeps(false),
      melt(true)
  {
    this->registerArgumentCallBacks();
    this->parseArguments();
    MFront::callingName = argv[0];
  } // end of MFront::MFront

  void
  MFront::treatNoDeps(void)
  {
    this->nodeps = true;
  } // end of MFront::treatNoDeps

  void
  MFront::treatNoMelt(void)
  {
    this->melt = false;
  } // end of MFront::treatNoMelt

  bool
  MFront::treatFile(void)
  {
    using namespace std;
    using namespace tfel::utilities;
    using namespace tfel::system;
    typedef MFrontBehaviourInterfaceFactory MBIF;
    typedef MFrontLawInterfaceFactory MLIF;
    typedef map<string,pair<vector<string>,vector<string> > > Target;
    MFrontParserFactory& parserFactory = MFrontParserFactory::getMFrontParserFactory();
    MBIF& mbif = MBIF::getMFrontBehaviourInterfaceFactory();
    MLIF& mlif = MLIF::getMFrontLawInterfaceFactory();
    CxxTokenizer file;
    string library;
    string parserName;
    map<string,vector<string> >::const_iterator p;
    vector<string>::const_iterator p2;
    CxxTokenizer::TokensContainer::const_iterator pt;
    CxxTokenizer::TokensContainer::const_iterator pte;
    Target::const_iterator p3;
    vector<string>::const_iterator p4;
    bool found;

    try{
      if(this->verboseMode){
	cout << "Treating file : " << this->fileName << endl;
      }
      file.openFile(this->fileName);
      file.stripComments();
      pt  = file.begin();
      pte = file.end();
      found=false;
      while((pt!=pte)&&(!found)){
	if(pt->value=="@Parser"){
	  if(pt!=file.begin()){
	    CxxTokenizer::TokensContainer::const_iterator ptp = pt;
	    --ptp;
	    if(ptp->value!=";"){
	      string msg("MFront::treatFile : ");
	      msg += "the keyword @Parser does not begin a new instruction.";
	      throw(runtime_error(msg));
	    }
	  }
	  ++pt;
	  if(pt==pte){
	    ostringstream msg;
	    msg << "MFront::treatFile : ";
	    msg << "unexepected end of file (exepected parser name).\n";
	    msg << "Error at line " << pt->line << ".";
	    throw(runtime_error(msg.str()));
	  }
	  if(pt->value==";"){
	    ostringstream msg;
	    msg << "MFront::treatFile : ";
	    msg << "unexepected end of file (exepected parser name).\n";
	    msg << "Error at line " << pt->line << ".";
	    throw(runtime_error(msg.str()));
	  }
	  parserName = pt->value;
	  ++pt;
	  if(pt==pte){
	    ostringstream msg;
	    msg << "MFront::treatFile : ";
	    msg << "unexepected end of file (exepected library name or ';').\n";
	    msg << "Error at line " << pt->line << ".";
	    throw(runtime_error(msg.str()));
	  }
	  if(pt->value!=";"){
	    library = pt->value;
	    ++pt;
	    if(pt==pte){
	      ostringstream msg;
	      msg << "MFront::treatFile : ";
	      msg << "unexepected end of file (exepected ';').\n";
	      msg << "Error at line " << pt->line << ".";
	      throw(runtime_error(msg.str()));
	    }
	    if(pt->value!=";"){
	      ostringstream msg;
	      msg << "MFront::treatFile : ";
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
	    ExternalLibraryManager& lm = ExternalLibraryManager::getExternalLibraryManager();
	    lm.loadLibrary(library);
	  }
	  this->parser = parserFactory.createNewParser(parserName);
	} 
	catch(runtime_error& r){
	  ostringstream msg;
	  msg << "MFront::treatFile : error while loading parser "
	      << parserName << " (" << r.what() << ")\n";
	  msg << "Available parsers : " << endl;
	  const vector<string>& parsers = parserFactory.getRegistredParsers();
	  copy(parsers.begin(),parsers.end(),ostream_iterator<string>(msg," "));
	  msg << endl;
	  throw(runtime_error(msg.str()));
	}
      } else {
	if(this->verboseMode){
	  cout << "No parser specified, using default" << endl;
	}
	this->parser = parserFactory.createNewParser();
      }

      if(this->verboseMode){
	this->parser->setVerboseMode();
      }
      if(this->debugMode){
	this->parser->setDebugMode();
      }
      if(this->warningMode){
	this->parser->setWarningMode();
      }
      if(!this->interfaces.empty()){
	this->parser->setInterfaces(this->interfaces);
      }
      file.clear();

      try{
	this->parser->treatFile(this->fileName);
      } 
      catch(const runtime_error& e){
	cerr << "MFront::treatFile : error while treating file " 
	     << fileName << ".\n" ;
	cerr << e.what() << endl;
	delete this->parser;
	return false;
      }

      try{
	this->parser->writeOutputFiles();
      } 
      catch(const runtime_error& e){
	cerr << "MFront::treatFile : "
	     << "error while writing ouput file.\n";
	cerr << e.what() << endl;
	delete this->parser;
	return false;
      }

      // getting generated sources
      const map<string,vector<string> >& src = this->parser->getGeneratedSources();
      for(p=src.begin();p!=src.end();++p){
	set<string>& tmp  = this->sources[p->first];
	copy(p->second.begin(),p->second.end(),insert_iterator<set<string> >(tmp,tmp.begin()));
      }
      // getting generated dependencies
      const map<string,vector<string> >& deps = this->parser->getLibrariesDependencies();
      for(p=deps.begin();p!=deps.end();++p){
	vector<string>& tmp = this->dependencies[p->first];
	for(p2=p->second.begin();p2!=p->second.end();++p2){
	  if(find(tmp.begin(),tmp.end(),*p2)==tmp.end()){
	    tmp.push_back(*p2);
	  }
	}
      }
      // getting specific targets
      const Target& t = this->parser->getSpecificTargets();
      for(p3=t.begin();p3!=t.end();++p3){
	for(p4=p3->second.first.begin();p4!=p3->second.first.end();++p4){
	  if(find(this->targets[p3->first].first.begin(),
		  this->targets[p3->first].first.end(),*p4)==this->targets[p3->first].first.end()){
	    this->targets[p3->first].first.push_back(*p4);
	  }
	}
	for(p4=p3->second.second.begin();p4!=p3->second.second.end();++p4){
	  if(find(this->targets[p3->first].second.begin(),
		  this->targets[p3->first].second.end(),*p4)==this->targets[p3->first].second.end()){
	    this->targets[p3->first].second.push_back(*p4);
	  }
	}
      }
      // getting includes
      const map<string,vector<string> >& incs = this->parser->getGlobalIncludes();
      for(p=incs.begin();p!=incs.end();++p){
	copy(p->second.begin(),
	     p->second.end(),
	     insert_iterator<set<string> >(this->globalIncludes,this->globalIncludes.begin()));
      }
      // Some clean-up
      mbif.reset();
      mlif.reset();
      delete this->parser;
    }
    catch(runtime_error& e){
      string msg("MFront::treatFile : ");
      msg += "error while treating file " + this->fileName + "\n";
      msg += e.what();
      throw(runtime_error(msg));
    }
    return true;
  } // end of MFront::treatFile(void)

  void
  MFront::analyseSources(const std::string& name)
  {
    using namespace std;
    typedef map<string,set<string> >::value_type MVType;
    ifstream file;
    ofstream f;
    set<string> files; // list of files contained in the file
    string fName;
    struct stat buffer; // for call to stat
    string object;
    set<string>::iterator p;
    set<string>::iterator p2;
    bool hasValidExtension;
    bool erased = false;
    fName = "src/" + name;
    if(stat(fName.c_str(),&buffer)!=0){
      string msg("MFront::analyseSources : can't stat "+fName);
      throw(runtime_error(msg));
    }
    if(!S_ISREG(buffer.st_mode)){
      string msg("MFront::analyseSources : "+fName+" is not a regular file");
      throw(runtime_error(msg));
    }
    file.open(fName.c_str());
    if(!file){
      string msg("MFront::analyseSources : can't open file ");
      msg += fName;
      msg += "\nError while analysing "+fName;
      throw(runtime_error(msg));
    }
    copy(istream_iterator<string>(file),
	 istream_iterator<string>(),insert_iterator<set<string> >(files,files.begin()));
    file.close();
    if(files.empty()){
      if(unlink(fName.c_str())!=0){
	string msg("MFront::analyseSources : can't unlink file"+fName);
	msg += "\nError while analysing "+fName;
	throw(runtime_error(msg));
      }
      return;
    }
    for(p=files.begin();p!=files.end();++p){
      object = p->substr(0,p->rfind("."))+".o";
      // check that the source has a valid extension
      hasValidExtension = false;
      if(p->size()>4){
	if((p->substr(p->size()-4)==".cpp")||
	   (p->substr(p->size()-4)==".cxx")){
	  hasValidExtension = true;
	}
      }
      if(p->size()>2){
	if(p->substr(p->size()-2)==".c"){
	  hasValidExtension = true;
	}
      }
      if(!hasValidExtension){
	string msg("MFront::analyseSources : ");
	msg += "source "+*p+" don't have a valid file extension, ";
	msg += "so we don't know how to treat it (valid file extension are ";
	msg += "'.cxx', '.cpp' and '.c').";
	msg += "\nError while analysing "+fName;
	throw(runtime_error(msg));
      }
//       if(!this->objects[name.substr(3,name.size()-7)].insert(object).second){
// 	string msg("MFront::analyseSources : ");
// 	msg += "two sources leads to the same object '"+object+"'.";
// 	msg += "\nError while analysing "+fName;
// 	throw(runtime_error(msg));
//       }
    }
    p=files.begin();
    while(p!=files.end()){
      if(access(("src/"+*p).c_str(),F_OK)==0){
	if(stat(("src/"+*p).c_str(),&buffer)!=0){
	  string msg("MFront::analyseSources : can't stat src/"+*p);
	  msg += "\nError while analysing "+fName;
	  throw(runtime_error(msg));
	}
	if(!S_ISREG(buffer.st_mode)){
	  string msg("MFront::analyseSources : src/"+*p+" is not a regular file");
	  msg += "\nError while analysing "+fName;
	  throw(runtime_error(msg));
	}
	if(access(("src/"+*p).c_str(),R_OK)!=0){
	  string msg("MFront::analyseSources : error while treating "+fName+".\n");
	  msg += "file src/"+*p+" is not readable.";
	  msg += "\nError while analysing "+fName;
	  throw(runtime_error(msg));
	}
	++p;
      } else {
	// removing file from the list
	p2 = p;
	++p;
	files.erase(p2);
	erased = true;
      }
    }
    if(erased){
      if(files.empty()){
	if(unlink(fName.c_str())!=0){
	  string msg("MFront::analyseSources : can't unlink file"+fName);
	  msg += "\nError while analysing "+fName;
	  throw(runtime_error(msg));
	}
	return;
      }
      f.open(fName.c_str());
      if(!f){
	string msg("MFront::analyseSources : ");
	msg += "error while opening "+fName+" for writing.\n";
	msg += "\nError while analysing "+fName;
	throw(runtime_error(msg));
      }
      copy(files.begin(),files.end(),ostream_iterator<string>(f,"\n"));
      f.close();
    }
    if(!files.empty()){
      if(this->verboseMode){
	cout << "inserting library " << name.substr(0,name.size()-3) << "so "
	     << "sources : \n";
	copy(files.begin(),files.end(),ostream_iterator<string>(cout," "));
	cout << endl;
      }
      this->sources.insert(MVType(name.substr(0,name.size()-4),
				  set<string>(files.begin(),files.end())));
    }
  } // end of MFront::analyseSources

  void
  MFront::analyseDependencies(const std::string& name)
  {
    using namespace std;
    typedef map<string,vector<string> >::value_type MVType;
    ifstream file;
    ofstream f;
    vector<string> libs; // list of libraries contained in the file
    string fName;
    string line;
    struct stat buffer; // for call to stat
    fName = "src/" + name;
    if(stat(fName.c_str(),&buffer)!=0){
      string msg("MFront::analyseDependencies : can't stat file '"+fName+"'");
      throw(runtime_error(msg));
    }
    if(!S_ISREG(buffer.st_mode)){
      string msg("MFront::analyseDependencies : '"+fName+"' is not a regular file");
      throw(runtime_error(msg));
    }
    file.open(fName.c_str());
    if(!file){
      string msg("MFront::analyseDependencies : can't open file '");
      msg += fName + "'";
      throw(runtime_error(msg));
    }
    while(!file.eof()){
      getline(file,line);
      if(find(libs.begin(),libs.end(),line)==libs.end()){
	libs.push_back(line);
      }
    }
    file.close();
    if(libs.empty()){
      if(unlink(fName.c_str())!=0){
	string msg("MFront::analyseDependencies : can't unlink file"+fName);
	throw(runtime_error(msg));
      }
      return;
    }
    if(!libs.empty()){
      if(this->verboseMode){
	cout << "inserting library " << name.substr(0,name.size()-4) << "so "
	     << "dependencies : \n";
	copy(libs.begin(),libs.end(),ostream_iterator<string>(cout," "));
	cout << endl;
      }
      this->dependencies.insert(MVType(name.substr(0,name.size()-5),libs));
    }
  } // end of MFront::analyseDependencies

  void
  MFront::analyseGlobalIncludes()
  {
    using namespace std;
    using namespace tfel::utilities;
    ifstream file;
    string name = "src/Makefile.incs";
    string line;
    struct stat buffer; // for call to stat
    if(stat(name.c_str(),&buffer)!=0){
      string msg("MFront::analyseMakefileSpecificTargets : can't stat file '"+name+"'");
      throw(runtime_error(msg));
    }
    if(!S_ISREG(buffer.st_mode)){
      string msg("MFront::analyseMakefileSpecificTargets : '"+name+"' is not a regular file");
      throw(runtime_error(msg));
    }
    file.open(name.c_str());
    if(!file){
      string msg("MFront::analyseGlobalIncludes : can't open file 'src/Makefile.incs'");
      throw(runtime_error(msg));
    }
    while(!file.eof()){
      getline(file,line);
      if(!line.empty()){
	this->globalIncludes.insert(line);
      }
    }
    file.close();
  } // end of MFront::analyseGlobalIncludes

  void
  MFront::analyseMakefileSpecificTargets()
  {
    using namespace std;
    using namespace tfel::utilities;
    ifstream file;
    string name = "src/Makefile.spec";
    string line;
    unsigned short lineNbr;
    struct stat buffer; // for call to stat
    vector<string>::const_iterator p;
    if(stat(name.c_str(),&buffer)!=0){
      string msg("MFront::analyseMakefileSpecificTargets : can't stat file '"+name+"'");
      throw(runtime_error(msg));
    }
    if(!S_ISREG(buffer.st_mode)){
      string msg("MFront::analyseMakefileSpecificTargets : '"+name+"' is not a regular file");
      throw(runtime_error(msg));
    }
    file.open(name.c_str());
    lineNbr = 0u;
    if(!file.eof()){
      getline(file,line);
      ++lineNbr;
      while(!file.eof()){
	vector<string> words;
	if(!line.empty()){
	  if(line[0]!='#'){
	    istringstream tokenizer(line);
	    copy(istream_iterator<string>(tokenizer),
		 istream_iterator<string>(),back_inserter(words));
	    if(!words.empty()){
	      if(words.size()<2){
		ostringstream msg;
		msg << "MFront::analyseMakefileSpecificTargets : "
		    << "invalid line " << lineNbr << ".\n"
		    << "Expected to read something like 'target : [dependencies]'.";
		throw(runtime_error(msg.str()));
	      }
	      if(words[1]!=":"){
		ostringstream msg;
		msg << "MFront::analyseMakefileSpecificTargets : "
		    << "invalid token '" << words[1] << "' (expected ':').\n"
		    << "Error at line " << lineNbr;
		throw(runtime_error(msg.str()));
	      }
	      for(p=words.begin()+2;p!=words.end();++p){
		if(find(this->targets[words[0]].first.begin(),
			this->targets[words[0]].first.end(),*p)==this->targets[words[0]].first.end()){
		  this->targets[words[0]].first.push_back(*p);
		}
	      }
	      if(!file.eof()){
		getline(file,line);
		++lineNbr;
		while((!file.eof())&&(!line.empty())){
		  if(find(this->targets[words[0]].second.begin(),
			  this->targets[words[0]].second.end(),line)==this->targets[words[0]].second.end()){
		    this->targets[words[0]].second.push_back(line);
		  }
		  getline(file,line);
		  ++lineNbr;
		}
	      }
	      if(!file.eof()){
		getline(file,line);
		++lineNbr;
	      }
	    } else {
	      if(!file.eof()){
		getline(file,line);
		++lineNbr;
	      }
	    }
	    words.clear();
	  } else {
	    if(!file.eof()){
	      getline(file,line);
	      ++lineNbr;
	    }
	  }
	} else {
	  if(!file.eof()){
	    getline(file,line);
	    ++lineNbr;
	  }
	}
      }
    }
    file.close();
  } // end of Mfront::analyseMakefileSpecificTargets

  void
  MFront::analyseSourceDirectory(void)
  {
    using namespace std;
    using namespace tfel::system;
    vector<string> files;
    vector<string>::const_iterator p;
    DIR *directory = 0;
    struct dirent* dir;
    struct stat buf;
    MFrontLock& l = MFrontLock::getMFrontLock();
    l.lock();
    try{
      directory = opendir("src");
      if(!directory){
	throw(runtime_error("MFront::analyseSourceDirectory : can't open directory 'src'"));
      }
      dir=readdir(directory);
      while(dir!=0){
	string file = "src/";
	file += dir->d_name;
	if(stat(file.c_str(),&buf)!=0){
	  string msg("MFront::analyseSourceDirectory : ");
	  msg += "can't stat file ";
	  msg += file;
	  cerr << msg << endl;
	  systemCall::throwSystemError(msg,errno);
	}
	if(S_ISREG(buf.st_mode)){
	  files.push_back(dir->d_name);
	}
	dir=readdir(directory);
      }
      closedir(directory);
      for(p=files.begin();p!=files.end();++p){
	if(p->substr(p->size()-4)==".src"){
	  if(this->verboseMode){
	    if(this->sys=="win32"){
	      cout << "treating library " << p->substr(0,p->size()-4) << ".dll sources.\n";
	    } else {
	      cout << "treating library " << p->substr(0,p->size()-4) << ".so sources.\n";
	    }
	  }
	  this->analyseSources(*p);
	}
	if(*p=="Makefile.spec"){
	  this->analyseMakefileSpecificTargets();
	}
	if(*p=="Makefile.incs"){
	  this->analyseGlobalIncludes();
	}
      }
      // we treat dependencies separetly because some file might had been
      // suppressed
      directory = opendir("src");
      if(!directory){
	throw(runtime_error("MFront::analyseSourceDirectory : can't open directory 'src'"));
      }
      dir=readdir(directory);
      files.clear();
      while(dir!=0){
	string file = "src/";
	file += dir->d_name;
	if(stat(file.c_str(),&buf)!=0){
	  string msg("MFront::analyseSourceDirectory : ");
	  msg += "can't stat file ";
	  msg += file;
	  systemCall::throwSystemError(msg,errno);
	}
	if(S_ISREG(buf.st_mode)){
	  files.push_back(dir->d_name);
	}
	dir=readdir(directory);
      }
      closedir(directory);
      for(p=files.begin();p!=files.end();++p){
	if(p->size()>5){
	  if(p->substr(p->size()-5)==".deps"){
	    if(this->verboseMode){
	      cout << "treating library " << p->substr(0,p->size()-5) << " dependencies.\n";
	    }
	    this->analyseDependencies(*p);
	  }
	}
      }
    }
    catch(...){
      l.unlock();
      throw;
    }
    l.unlock();
  } // end of MFront::analyseSourceDirectory
  
  void
  MFront::writeSourcesLists(void)
  {
    using namespace std;
    ofstream file;
    MFrontLock& l = MFrontLock::getMFrontLock();
    l.lock();
    try{
      map<string,set<string> >::const_iterator p;
      for(p=this->sources.begin();p!=this->sources.end();++p){
	if(this->verboseMode){
	  if(this->sys=="win32"){
	    cout << "writing sources list for library " << p->first << ".dll\n";
	  } else {
	    cout << "writing sources list for library " << p->first << ".so\n";
	  }
	}
	file.open(("src/"+p->first+".src").c_str());
	copy(p->second.begin(),p->second.end(),ostream_iterator<string>(file,"\n"));
	file.close();
      }
    }
    catch(...){
      l.unlock();
      throw;
    }
    l.unlock();
  } // end of MFront::writeSourceLists
  
  void
  MFront::writeDependenciesLists(void)
  {
    using namespace std;
    ofstream file;
    MFrontLock& l = MFrontLock::getMFrontLock();
    l.lock();
    try{
      map<string,vector<string> >::const_iterator p;
      for(p=this->dependencies.begin();p!=this->dependencies.end();++p){
	if(this->verboseMode){
	  if(this->sys=="win32"){
	    cout << "writing dependencies list for library " << p->first << ".dll\n";
	  } else {
	    cout << "writing dependencies list for library " << p->first << ".so\n";
	  }
	}
	file.open(("src/"+p->first+".deps").c_str());
	copy(p->second.begin(),p->second.end(),ostream_iterator<string>(file,"\n"));
	file.close();
      }
    }
    catch(...){
      l.unlock();
      throw;
    }
    l.unlock();
  } // end of MFront::writeDependenciesLists

  void
  MFront::writeSpecificTargets(void)
  {
    using namespace std;
    typedef map<string,pair<vector<string>,vector<string> > > Target;
    ofstream file;
    Target::const_iterator p;
    MFrontLock& l = MFrontLock::getMFrontLock();
    l.lock();
    try{
      if(!this->targets.empty()){
	file.open("src/Makefile.spec");
	if(!file){
	  string msg("MFront::writeSpecificTargets : can't open file 'src/Makefile.spec'");
	  throw(runtime_error(msg));
	}
	for(p=this->targets.begin();p!=this->targets.end();++p){
	  file << p->first << " : ";
	  copy(p->second.first.begin(),
	       p->second.first.end(),ostream_iterator<string>(file," "));
	  file << endl;
	  copy(p->second.second.begin(),
	       p->second.second.end(),ostream_iterator<string>(file,"\n"));
	  file << endl;
	}
	file.close();
      }
    }
    catch(...){
      l.unlock();
      throw;
    }
    l.unlock();
  } // end of MFront::writeSpecificTargets

  void
  MFront::writeGlobalIncludes(void)
  {
    using namespace std;
    ofstream file;
    set<string>::const_iterator p;
    MFrontLock& l = MFrontLock::getMFrontLock();
    l.lock();
    try{
      if(!this->globalIncludes.empty()){
	file.open("src/Makefile.incs");
	if(!file){
	  string msg("MFront::writeGlobalIncludes : can't open file 'src/Makefile.incs'");
	  throw(runtime_error(msg));
	}
	copy(this->globalIncludes.begin(),
	     this->globalIncludes.end(),ostream_iterator<string>(file,"\n"));
      }
    }
    catch(...){
      l.unlock();
      throw;
    }
    l.unlock();
  } // end of MFront::writeGlobalIncludes

  std::pair<bool,std::pair<std::string,std::string> >
  MFront::getLibraryDependencies(const std::string& name)
  {
    using namespace std;
    map<string,set<string> >::const_iterator p2;
    map<string,vector<string> >::const_iterator p3;
    set<string>::const_iterator p4;
    map<string,pair<vector<string>,vector<string> > >::const_iterator p5;
    vector<string>::const_iterator p6;
    pair<bool,pair<string,string> > res;
    p2 = this->sources.find(name);
    p3 = this->dependencies.find(name);
    res.first = false;
    if(p2==this->sources.end()){
      string msg("MFront::getLibraryDependencies : no library named '"+name+"'.\n");
      msg += "Internal Error.";
      throw(runtime_error(msg));
    }
    for(p4 = p2->second.begin();p4 != p2->second.end();++p4){
      if(p4->size()>4){
	if(p4->substr(p4->size()-4)==".cpp"){
	  res.first = true;
	  res.second.first += p4->substr(0,p4->size()-4)+".o ";
	}
	if(p4->substr(p4->size()-4)==".cxx"){
	  res.first = true;
	  res.second.first += p4->substr(0,p4->size()-4)+".o ";
	}
      } 
      if(p4->size()>2){
	if(p4->substr(p4->size()-2)==".c"){
	  res.second.first += p4->substr(0,p4->size()-2)+".o ";
	}
      }
    }
    if(p3!=this->dependencies.end()){
      for(p6 = p3->second.begin();p6!=p3->second.end();++p6){
	if(!p6->empty()){
	  if(p6->size()>2){
	    if(p6->substr(0,2)=="-l"){
	      string lib = p6->substr(2);
	      if(this->sources.find("lib"+lib)!=this->sources.end()){
		if(this->melt){
		  pair<bool,pair<string,string> > dep = this->getLibraryDependencies("lib"+lib);
		  res.first = res.first || dep.first;
		  res.second.first  += dep.second.first;
		  res.second.second += dep.second.second;
		} else {
		  res.second.second += "lib";
		  res.second.second += lib;
		  if(this->sys=="win32"){
		    res.second.second +=  + ".dll ";
		  } else {
		    res.second.second +=  + ".so ";
		  }
		}
	      }
	    }
	  }
	}
      }
    }
    return res;
  } // end of MFront::getLibraryDependencies

  std::string
  MFront::getLibraryLinkDependencies(const std::string& name)
  {
    using namespace std;
    map<string,vector<string> >::const_iterator p3;
    vector<string>::const_iterator p4;
    string res;
    p3 = this->dependencies.find(name);
    if(p3!=this->dependencies.end()){
      this->makeFile << "-L. ";
      for(p4 = p3->second.begin();p4!=p3->second.end();++p4){
	if(!p4->empty()){
	  if(p4->size()>2){
	    if(p4->substr(0,2)=="-l"){
	      if(this->sources.find("lib"+p4->substr(2))!=this->sources.end()){
		if(!this->melt){
		  res += this->getLibraryLinkDependencies(*p4) + " " + *p4 + " ";
		}
	      } else {
		res += *p4 + " ";
	      }
	    } else {
	      res += *p4 + " ";
	    }
	  } else {
	    res += *p4 + " ";
	  }
	}
      }
    }
    return this->sortLibraryList(res);
  } // end of MFront::getLibraryLinkDependencies(const std::string&)

  std::string
  MFront::sortLibraryList(const std::string& lib)
  {
    using namespace std;
    istringstream tokenizer(lib);
    vector<string> libs;
    vector<string> vres;
    string res;
    vector<string>::const_reverse_iterator p;
    vector<string>::const_reverse_iterator pe;
    copy(istream_iterator<string>(tokenizer),
	 istream_iterator<string>(),back_inserter(libs));
    p  = static_cast<const vector<string>&>(libs).rbegin();
    pe = static_cast<const vector<string>&>(libs).rend();
    while(p!=pe){
      if(find(vres.begin(),vres.end(),*p)==vres.end()){
	vres.push_back(*p);
	res = *p+" "+res; 
      }
      ++p;
    }
    return res;
  } // end of sortLibraryList

  void
  MFront::generateMakeFile(void)
  {
    using namespace std;
    set<string> cppSources;
    set<string> cSources;
    vector<string>::const_iterator p;
    map<string,set<string> >::const_iterator p2;
    map<string,set<string> >::const_iterator p3;
    set<string>::const_iterator p4;
    map<string,pair<vector<string>,vector<string> > >::const_iterator p5;
    vector<string>::const_iterator p6;
    set<string>::const_iterator p7;
    set<string>::const_iterator p8;
    string cc;
    string cxx;
    const char * const env_cc  = ::getenv("CC");
    const char * const env_cxx = ::getenv("CXX");
    const char * const inc = ::getenv("INCLUDES");
    const char * const cxxflags = ::getenv("CXXFLAGS");
    const char * const cflags   = ::getenv("CFLAGS");
    const char * const ldflags  = ::getenv("LDFLAGS");
    const char * ranlib  = ::getenv("RANLIB");
    const char * dlltool = ::getenv("DLLTOOL");
    const char * ar      = ::getenv("AR");
#ifdef __CYGWIN__
    if(env_cxx==0){
      cxx = "$(CXX) -mno-cygwin";
    } else {
      cxx = "$(CXX)";
    }
    if(env_cc==0){
      cc = "$(CC) -mno-cygwin";
    } else {
      cc = "$(CC)";
    }
#else
    cxx = "$(CXX)";
    cc  = "$(CC)";
#endif /* __CYGWIN__ */
    if(this->verboseMode){
      cout << "generating Makefile\n";
    }
    if(sys=="win32"){
#ifndef __CYGWIN__
      if((ranlib==0)||(dlltool==0)||(ar==0)){
	string msg("MFront::generateMakeFile : ");
	msg += "the following environnement variables ";
	msg += "shall be defined when using the --win32 flag : \n";
	msg += "- AR\n";
	msg += "- RANLIB\n";
	msg += "- DLLTOOL\n";
	throw(runtime_error(msg));
      }
#else
      if(ranlib==0){
	ranlib  = "ranlib";
      }
      if(dlltool==0){
	dlltool = "dlltool";
      }
      if(ar==0){
	ar = "ar";
      }
#endif /* __CYGWIN__ */
    }
    MFrontLock& l = MFrontLock::getMFrontLock();
    l.lock();
    try{
      this->makeFile.open("src/Makefile.mfront");
      this->makeFile.exceptions(ios::badbit|ios::failbit);
      if(!this->makeFile){
	string msg("MFront::generateMakeFile : can't open file Makefile.mfront");
	throw(runtime_error(msg));
      }
      for(p2=this->sources.begin();p2!=this->sources.end();++p2){
	for(p4 = p2->second.begin();p4 != p2->second.end();++p4){
	  if(p4->size()>4){
	    if((p4->substr(p4->size()-4)==".cpp")||
	       (p4->substr(p4->size()-4)==".cxx")){
	      cppSources.insert(*p4);
	    }
	  }
	  if(p4->size()>2){
	    if(p4->substr(p4->size()-2)==".c"){
	      cSources.insert(*p4);
	    }
	  }
	}
      }
      this->makeFile << "# Makefile generated by mfront.\n"
		     << MFrontHeader::getHeader("# ") << "\n";
      this->makeFile << "export LD_LIBRARY_PATH:=$(PWD):$(LD_LIBRARY_PATH)\n\n";
      // LINKER
      if(ar!=0){
	this->makeFile << "AR := "  << ar  << "\n";
      }
      if(ranlib!=0){
	this->makeFile << "RANLIB := "  << ranlib  << "\n";
      }
      if(dlltool!=0){
	this->makeFile << "DLLTOOL := "  << dlltool  << "\n";
      }
      // COMPILERS
      if(env_cc!=0){
	this->makeFile << "CC := "  << env_cc  << "\n";
      }
      if(env_cxx!=0){
	this->makeFile << "CXX := " << env_cxx << "\n";
      }
      if((env_cc!=0)||(env_cxx!=0)){
	this->makeFile << endl;
      }
      // INCLUDES
      this->makeFile << "INCLUDES := ";
      if(inc!=0){
	this->makeFile << inc << " ";
      }
      this->makeFile << "-I../include `tfel-config --includes`";
      if(!this->globalIncludes.empty()){
	this->makeFile << " \\\n";
	for(p7=this->globalIncludes.begin();
	    p7!=this->globalIncludes.end();){
	  this->makeFile << "\t     " << *p7;
	  if(++p7!=this->globalIncludes.end()){
	    this->makeFile << " \\\n";
	  }
	}
      }
      this->makeFile << endl << endl;
      // LDFLAGS
      if(ldflags!=0){
	this->makeFile << "LDFLAGS := " << ldflags  << endl;
      }
      // CXXFLAGS
      if(!cppSources.empty()){
	this->makeFile << "CXXFLAGS := -Wall -ansi ";
	if(cxxflags!=0){
	  this->makeFile << cxxflags << " ";
	} else if(this->oflags){
	  this->makeFile << "`tfel-config --oflags` ";
	} else {
	  this->makeFile << "-O2 ";
	}
	if(this->sys=="win32"){
	  this->makeFile << "-D'F77_FUNC(X,Y)=X\\#\\#_' -DWIN32 $(INCLUDES) \n\n";
	} else {
	  this->makeFile << "-D'F77_FUNC(X,Y)=X\\#\\#_' -fPIC $(INCLUDES) \n\n";
	}
      }
      // CFLAGS
      if(!cSources.empty()){
	this->makeFile << "CFLAGS := -W -Wall -ansi -std=c99 ";
	if(cflags!=0){
	  this->makeFile << cflags << " ";
	} else if(this->oflags){
	  this->makeFile << "`tfel-config --oflags` ";
	} else {
	  this->makeFile << "-O2 ";
	}
	if(this->sys=="win32"){
	  this->makeFile << "-D'F77_FUNC(X,Y)=X' -DWIN32 $(INCLUDES)\n\n";
	} else {
	  this->makeFile << "-D'F77_FUNC(X,Y)=X' -fPIC $(INCLUDES)\n\n";
	}
      }
      // sources list
      if(!cppSources.empty()){
	this->makeFile << "SRCCXX = ";
	p4=cppSources.begin();
	while(p4!=cppSources.end()){
	  this->makeFile << *p4;
	  ++p4;
	  if(p4!=cppSources.end()){
	    this->makeFile << " ";
	  }
	}
	this->makeFile << "\n\n";
      }
      if(!cSources.empty()){
	this->makeFile << "SRC = ";
	p4=cSources.begin();
	while(p4!=cSources.end()){
	  this->makeFile << *p4;
	  ++p4;
	  if(p4!=cSources.end()){
	    this->makeFile << " ";
	  }
	}
	this->makeFile << "\n\n";
      }
      if(!this->nodeps){
	if(!cppSources.empty()){
	  this->makeFile << "makefiles1 = $(SRCCXX:.cxx=.d)\n";
	  this->makeFile << "makefiles2 = $(makefiles1:.cpp=.d)\n";
	}
	if(!cSources.empty()){
	  this->makeFile << "makefiles3 = $(SRC:.c=.d)\n";
	}
	this->makeFile << "makefiles  =";
	if(!cppSources.empty()){
	  this->makeFile << " $(makefiles2)";
	}
	if(!cSources.empty()){
	  this->makeFile << " $(makefiles3)";
	}
      }
      this->makeFile << "\n\n";
      this->makeFile << ".PHONY = ";
      this->makeFile << "all clean ";
      for(p2=this->sources.begin();p2!=this->sources.end();++p2){
	if(this->sys=="win32"){
	  this->makeFile << p2->first << ".dll ";
	} else {
	  this->makeFile << p2->first << ".so ";
	}
      }
      for(p5=this->targets.begin();p5!=this->targets.end();++p5){
	if((p5->first!="all")&&(p5->first!="clean")){
	  this->makeFile << p5->first << " ";
	}
      }
      this->makeFile << "\n\n";
      this->makeFile << "all : ";
      for(p2=this->sources.begin();p2!=this->sources.end();++p2){
	if(this->sys=="win32"){
	  this->makeFile << p2->first << ".dll";
	} else {
	  this->makeFile << p2->first << ".so";
	}
	this->makeFile << " ";
      }    
      p5=this->targets.find("all");
      if(p5!=this->targets.end()){
	copy(p5->second.first.begin(),
	     p5->second.first.end(),ostream_iterator<string>(this->makeFile," "));
      }
      this->makeFile << "\n";
      if(p5!=this->targets.end()){
	for(p6=p5->second.second.begin();p6!=p5->second.second.end();++p6){
	  this->makeFile << "\t" << *p6 << endl;
	}
      }
      this->makeFile << "\n";
      for(p5=this->targets.begin();p5!=this->targets.end();++p5){
	if((p5->first!="all")&&(p5->first!="clean")){
	  this->makeFile << p5->first << " : ";
	  copy(p5->second.first.begin(),
	       p5->second.first.end(),ostream_iterator<string>(this->makeFile," "));
	  this->makeFile << endl;
	  for(p6=p5->second.second.begin();p6!=p5->second.second.end();++p6){
	    this->makeFile << "\t" << *p6 << endl;
	  }
	  this->makeFile << "\n";
	}
      }
      for(p2=this->sources.begin();p2!=this->sources.end();++p2){
	bool hasCxxSources = false;
	if(this->sys=="win32"){
	  this->makeFile << p2->first << ".a : ";
	} else {
	  this->makeFile << p2->first << ".so : ";
	}
	pair<bool,pair<string,string> > dep = this->getLibraryDependencies(p2->first);
	hasCxxSources = dep.first;
	if(!dep.second.first.empty()){
	  this->makeFile << dep.second.first;
	} 
	if(!dep.second.second.empty()){
	  this->makeFile << this->sortLibraryList(dep.second.second);
	}
	this->makeFile << "\n\t";
	if(this->sys=="win32"){
	  this->makeFile << "@$(AR) cr $@ $^\n\t";
	  this->makeFile << "@$(RANLIB) $@\n\n";
	  this->makeFile << p2->first << ".dll : ";
	  this->makeFile << p2->first << ".a\n\t";
	  if(hasCxxSources){
	    this->makeFile << "@" << cxx << " ";
	  } else {
	    this->makeFile << "@" << cc  << " ";
	  }
	  this->makeFile << " -mdll -o junk.tmp -Wl,--base-file,base.tmp $^\n\t";
	  this->makeFile << "@$(DLLTOOL) $^ --output-def def.tmp --no-export-all-symbols --add-stdcall-alias \n\t";
	  this->makeFile << "@$(DLLTOOL) --dllname $@ --base-file base.tmp --output-exp exp.tmp --def def.tmp\n\t";
	  if(hasCxxSources){
	    this->makeFile << "@" << cxx << " ";
	  } else {
	    this->makeFile << "@" << cc  << " ";
	  }
	  this->makeFile << " -mdll -o $@ -Wl,exp.tmp $^ "
			 << this->getLibraryLinkDependencies(p2->first)
			 << "\n\t";
	  this->makeFile << "@$(RM) junk.tmp base.tmp exp.tmp def.tmp\n\n";
	} else {
	  if(hasCxxSources){
	    this->makeFile << "@" << cxx << " ";
	  } else {
	    this->makeFile << "@" << cc  << " ";
	  }
	  if(ldflags!=0){
	    cout << "$(LDFLAGS) ";
	  }
	  this->makeFile << "-shared $^ -o $@ ";
	  this->makeFile << this->getLibraryLinkDependencies(p2->first);
	  this->makeFile << "\n\n";
	}
      }
      this->makeFile << "clean : ";
      p5=this->targets.find("clean");
      if(p5!=this->targets.end()){
	copy(p5->second.first.begin(),
	     p5->second.first.end(),ostream_iterator<string>(this->makeFile," "));
      }
      this->makeFile << "\n";
      if(this->sys=="win32"){
	this->makeFile << "\t@rm -f *.o *.dll *.d\n";
      } else {
	this->makeFile << "\t@rm -f *.o *.so *.d\n";
      }
      if(p5!=this->targets.end()){
	for(p6=p5->second.second.begin();p6!=p5->second.second.end();++p6){
	  this->makeFile << "\t" << *p6 << endl;
	}
      }
      this->makeFile << "\n";
      if(!this->nodeps){
	this->makeFile << "-include $(makefiles)\n\n";
      }
      // generic rules for objects file generation
      if(!cppSources.empty()){
	this->makeFile << "%.o:%.cxx\n";
	this->makeFile << "\t@" << cxx << " $(CXXFLAGS) $< -o $@ -c\n\n";
	this->makeFile << "%.o:%.cpp\n";
	this->makeFile << "\t@" << cxx << " $(CXXFLAGS) $< -o $@ -c\n\n";
      }
      if(!cSources.empty()){
	this->makeFile << "%.o:%.c\n";
	this->makeFile << "\t@" << cc << " $(CFLAGS) $< -o $@ -c\n\n";
      }
      if(!this->nodeps){
	if(!cppSources.empty()){
	  this->makeFile << "%.d:%.cxx\n";
	  this->makeFile << "\t@set -e; rm -f $@;					  \\\n";
	  this->makeFile << "\t$(CXX) -M $(CXXFLAGS) $< > $@.$$$$; \\\n";
	  this->makeFile << "\tsed 's,\\($*\\)\\.o[ :]*,\\1.o $@ : ,g' < $@.$$$$ > $@; \\\n";
	  this->makeFile << "\trm -f $@.$$$$\n\n";
	  this->makeFile << "%.d:%.cpp\n";
	  this->makeFile << "\t@set -e; rm -f $@;					  \\\n";
	  this->makeFile << "\t$(CXX) -M $(CXXFLAGS) $< > $@.$$$$; \\\n";
	  this->makeFile << "\tsed 's,\\($*\\)\\.o[ :]*,\\1.o $@ : ,g' < $@.$$$$ > $@; \\\n";
	  this->makeFile << "\trm -f $@.$$$$\n\n";
	}
	if(!cSources.empty()){
	  this->makeFile << "%.d:%.c\n";
	  this->makeFile << "\t@set -e; rm -f $@;					  \\\n";
	  this->makeFile << "\t$(CC) -M $(CFLAGS) $< > $@.$$$$; \\\n";
	  this->makeFile << "\tsed 's,\\($*\\)\\.o[ :]*,\\1.o $@ : ,g' < $@.$$$$ > $@; \\\n";
	  this->makeFile << "\trm -f $@.$$$$\n";
	}
      }
      this->makeFile.close();
    }
    catch(...){
      l.unlock();
      throw;
    }
    l.unlock();
  } // end of MFront::generateMakeFile
  
  void
  MFront::buildLibraries(const std::string& target)
  {
    using namespace std;
    const char *const argv[] = {"make","-f",
				"Makefile.mfront",target.c_str(),0};
    pid_t child_pid;
    int status;
    child_pid = fork();
    if(child_pid!=0){
      if(wait(&status)==-1){
	string msg("MFront::buildLibraries : ");
	msg += "something went wrong while waiting end of make process";
	throw(runtime_error(msg));
      }
      if(status!=0){
	string msg("MFront::buildLibraries : ");
	msg += "libraries building went wrong";
	throw(runtime_error(msg));
      }
    } else {
      if(chdir("./src")!=0){
	string msg("MFront::buildLibraries : ");
	msg += "can't change working directory to ./src";
	throw(runtime_error(msg));
      }
      execvp("make",const_cast<char* const*>(argv));
    }
  } // end of MFront::buildLibraries

  void
  MFront::cleanLibraries(void)
  {
    using namespace std;
    const char *const argv[] = {"make","-f",
				"Makefile.mfront",
				"clean",0};
    map<string,set<string> >::const_iterator p;
    pid_t child_pid;
    int status;
    child_pid = fork();
    if(child_pid!=0){
      if(wait(&status)==-1){
	string msg("MFront::cleanLibraries : ");
	msg += "something went wrong while waiting end of make process";
	throw(runtime_error(msg));
      }
      if(status!=0){
	string msg("MFront::cleanLibraries : ");
	msg += "can't clean libraries";
	throw(runtime_error(msg));
      }
    } else {
      if(chdir("./src")!=0){
	string msg("MFront::cleanLibraries : ");
	msg += "can't change working directory to ./src";
	throw(runtime_error(msg));
      }
      execvp("make",const_cast<char* const*>(argv));
    }
  } // end of MFront::cleanLibraries

  void
  MFront::exe(void)
  {
    using namespace std;
    using namespace tfel::system;
    typedef map<string,pair<vector<string>,vector<string> > > Target;
    vector<string> tmp;
    set<string>::const_iterator p;
    vector<string>::iterator p2;
    Target::iterator p3;
    set<string>::const_iterator p5;
    systemCall::mkdir("src");
    systemCall::mkdir("include");
    bool ok = true;
    if(this->specifiedTargets.empty()){
      this->specifiedTargets.insert("all");
    }
    if((this->inputs.empty())&&(!this->genMake)){
      string msg("MFront::exe : no file name specified ");
      msg += "and neither --make nor --build specified to the command line.\n";
      msg += this->getUsageDescription();
      throw(runtime_error(msg));
    }
    // get file generated by previous sessions
    this->analyseSourceDirectory();
    if(!this->inputs.empty()){
      for(p=this->inputs.begin();p!=this->inputs.end();++p){
	this->fileName = *p;
	try{
	  if(!this->treatFile()){
	    ok = false;
	  }
	} catch(runtime_error& e){
	  cout << "Error while treating file : " << this->fileName << endl;
	  cout << e.what() << endl;
	  ok = false;
	}
      }
      for(p3=this->targets.begin();p3!=this->targets.end();++p3){
	for(p2=p3->second.first.begin();p2!=p3->second.first.end();++p2){
	  vector<string>::iterator p4 = p2+1;
	  if(find(p4,p3->second.first.end(),*p2)==p3->second.first.end()){
	    tmp.push_back(*p2);
	  }
	}
	p3->second.first.swap(tmp);
	tmp.clear();
	for(p2=p3->second.second.begin();p2!=p3->second.second.end();++p2){
	  vector<string>::iterator p4 = p2+1;
	  if(find(p4,p3->second.second.end(),*p2)==p3->second.second.end()){
	    tmp.push_back(*p2);
	  }
	}
	p3->second.second.swap(tmp);
	tmp.clear();	
      }
      // get file generated by previous sessions
      this->analyseSourceDirectory();
      // save generated files
      this->writeSourcesLists();
      this->writeDependenciesLists();
      this->writeSpecificTargets();
      this->writeGlobalIncludes();
    }
    if(ok){
      if((this->genMake)&&(!this->sources.empty())){
	this->generateMakeFile();
      }
      if(this->cleanLibs){
	this->cleanLibraries();
      }
      if((this->buildLibs)&&(!this->sources.empty())){
	map<string,set<string> >::const_iterator pt;
	map<string,pair<vector<string>,vector<string> > >::const_iterator pt2;
	vector<string>::const_iterator pt3;
	for(p5  = this->specifiedTargets.begin();
	    p5 != this->specifiedTargets.end(); ++p5){
	  cout << "Treating target : " << *p5 << endl;
	  this->buildLibraries(*p5);
	}
	if(!this->sources.empty()){
	  if(this->sources.size()==1){
	    cout << "The following library has been build :\n";
	  } else {
	    cout << "The following libraries have been build :\n";
	  }
	  for(pt=this->sources.begin();pt!=this->sources.end();++pt){
	    if(this->sys=="win32"){
	      cout << "- " << pt->first << ".dll\n";
	    } else {
	      cout << "- " << pt->first << ".so\n";
	    }
	  }
	}
	if(!this->targets.empty()){
	  pt2=this->targets.find("all");
	  if(pt2!=this->targets.end()){	      
	    if(pt2->second.first.size()==1){
	      cout << "The following main target has been build :\n";
	    } else {
	      cout << "The following main targets have been build :\n";
	    }
	    for(pt3=pt2->second.first.begin();
		pt3!=pt2->second.first.end();++pt3){
	      cout << "- " << *pt3 << endl;
	    }
	  }
	}
      }
    }
  } // end of MFront::exe

  MFront::~MFront()
  {
    typedef MFrontBehaviourInterfaceFactory MBIF;
    typedef MFrontLawInterfaceFactory MLIF;
    MBIF& mbif = MBIF::getMFrontBehaviourInterfaceFactory();
    MLIF& mlif = MLIF::getMFrontLawInterfaceFactory();
    mbif.clear();
    mlif.clear();
  } // end of MFront::~MFront

} // end of namespace mfront
