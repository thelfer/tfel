/*!
 * \file   mfront/src/MFront.cxx
 * \brief  
 * \author Helfer Thomas
 * \date   09 nov 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<iostream>
#include<cstdlib>
#include<fstream>
#include<sstream>
#include<string>
#include<iterator>
#include<stdexcept>
#include<cerrno>
#include<memory>
#include<cassert>

#include<sys/types.h>
#include<sys/stat.h>
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <io.h>
#include <conio.h>
#include <windows.h>
#include <process.h>
#else
#include<dlfcn.h> 
#include<sys/wait.h>
#include<dirent.h>
#include<unistd.h>
#endif

#include"tfel-config.hxx"
#include"TFEL/Utilities/TerminalColors.hxx"
#include"TFEL/Utilities/StringAlgorithms.hxx"
#include"TFEL/System/System.hxx"
#include"TFEL/System/ExternalLibraryManager.hxx"

#include"MFront/MFrontHeader.hxx"
#include"MFront/MFrontLogStream.hxx"
#include"MFront/TargetsDescription.hxx"
#include"MFront/SearchFile.hxx"
#include"MFront/DSLFactory.hxx"
#include"MFront/MaterialPropertyInterfaceFactory.hxx"
#include"MFront/BehaviourInterfaceFactory.hxx"
#include"MFront/ModelInterfaceFactory.hxx"
#include"MFront/MFrontLock.hxx"
#include"MFront/MFront.hxx"

template<typename Container>
static void insert_if(std::vector<std::string>& dest,
		      const Container& src){
  for(const auto& v : src){
    if(std::find(dest.begin(),dest.end(),v)==dest.end()){
      dest.push_back(v);
    }
  }
}

namespace mfront{

  static void
  checkIfFileIsRegularAndReadable(const std::string& n)
  {
    using namespace std;
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
    DWORD dwAttrib = GetFileAttributes(n.c_str());
    if((dwAttrib == INVALID_FILE_ATTRIBUTES)||
       (dwAttrib & FILE_ATTRIBUTE_DIRECTORY)){
      string msg("MFront::checkIfFileIsRegularAndReadable : can't stat file '"+n+"' ");
      msg += "or is not a regular file";
      throw(runtime_error(msg));
    }
#else
    struct stat buffer; // for call to stat
    if(stat(n.c_str(),&buffer)!=0){
      string msg("MFront::checkIfFileIsRegularAndReadable : can't stat file '"+n+"'");
      throw(runtime_error(msg));
    }
    if(!S_ISREG(buffer.st_mode)){
      string msg("MFront::checkIfFileIsRegularAndReadable : '"+n+"' is not a regular file");
      throw(runtime_error(msg));
    }
#endif
  }

#if defined _WIN32 || defined _WIN64
  static bool
  getValueInRegistry(std::string &value)
  {
    using namespace std;
    HKEY  hKey;
    char  szBuffer[512];
    DWORD dwBufferSize = sizeof(szBuffer);
    LONG  nError;
    LONG  lRes = RegOpenKeyEx(HKEY_CLASSES_ROOT,"TFELHOME-" VERSION,0,KEY_READ,&hKey);
    if(ERROR_SUCCESS != lRes){
      return false;
    }
    nError = RegQueryValueEx(hKey,"",nullptr,nullptr,
			     reinterpret_cast<LPBYTE>(szBuffer),
			     &dwBufferSize);
    RegCloseKey(hKey);
    if (ERROR_SUCCESS == nError){
      value = szBuffer;
      return true;
    }
    return false;
  }
#endif
  
  static std::string
  handleSpace(const std::string& p)
  {
    using namespace std;
    if(find(p.begin(),p.end(),' ')!=p.end()){
#if defined _WIN32 || defined _WIN64
      string msg("tfel-config handleSpace: "
		 "path to TFEL shall not contain space as "
		 "MinGW can't handle it (Found '"+p+"'). "
		 "Please change TFEL installation directory");
      throw(runtime_error(msg));
#endif
      return '"'+p+'"';
    }
    return p;
  }

  static std::string
  getTFELHOME(void)
  {
    using namespace std;
#if defined _WIN32 || defined _WIN64
    // check in the registry (installation through NSIS)
    string rpath;
    if(getValueInRegistry(rpath)){
      return handleSpace(rpath);
    }
#endif
    const char * const path = getenv("TFELHOME");
    if(path!=nullptr){
      return handleSpace(path);
    }
    
#if defined _WIN32 || defined _WIN64
    string msg("tfel-config getTFELHOME: "
	       "no TFELHOME registry key defined and no TFEHOME "
	       "environment variable defined");
    throw(runtime_error(msg));
#endif
    return "";
  }

  /*!
   * \return the path to the documentation file if available.
   * If not, an empty string is returned
   * \param[in] pn : parser name
   * \param[in] k  : keyword
   */
  static std::string
  getDocumentationFilePath(const std::string& pn,
			   const std::string& k)
  {
    using namespace std;
    string root = getTFELHOME();
    if(root.empty()){
      root = PREFIXDIR;
    }
    string fn = root+"/share/doc/mfront/"+pn+"/"+k.substr(1)+".md";
    ifstream desc(fn);
    if(desc){
      return fn;
    }
    fn = root+"/share/doc/mfront/"+k.substr(1)+".md";
    desc.open(fn);
    if(desc){
      return fn;
    }
    return "";
  }

  std::string 
  MFront::getVersionDescription(void) const
  {
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

  const tfel::utilities::Argument&
  MFront::getCurrentCommandLineArgument() const
  {
    return *(this->currentArgument);
  }

  void
  MFront::treatUnknownArgument(void)
  {
    if(!MFrontBase::treatUnknownArgumentBase()){
#if !(defined _WIN32 || defined _WIN64 ||defined __CYGWIN__)
      ArgumentParserBase<MFront>::treatUnknownArgument();
#else
		auto a = static_cast<const std::string&>(this->getCurrentCommandLineArgument());
		std::cerr << "mfront : unsupported option '" 
				<< a << '\'' << std::endl;
      exit(EXIT_FAILURE);
#endif /* __CYGWIN__ */
    }
  } // end of MFront::treatUnknownArgument()

#ifdef MFRONT_MAKE_SUPPORT  
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
    using namespace std;
    this->genMake = true;
    string level = this->currentArgument->getOption();
    if(!level.empty()){
      if(level=="level2"){
	this->oflags2   = true;
      } else if(level=="level0"){
	this->oflags0   = true;
      } else if(level=="level1"){
	this->oflags    = true;
      } else if(level!="level1"){
	string msg("MFront::treatOMake : ");
	msg += "unsupported value '"+level+
	  "' for the --omake option";
	throw(runtime_error(msg));
      }
    } else {
      this->oflags  = true;
    }
  } // end of MFront::treatOMake

  void
  MFront::treatOBuild(void)
  {
    using namespace std;
    this->genMake   = true;
    this->buildLibs = true;
    string level = this->currentArgument->getOption();
    if(!level.empty()){
      if(level=="level2"){
	this->oflags2   = true;
      } else if(level=="level0"){
	this->oflags0   = true;
      } else if(level=="level1"){
	this->oflags    = true;
      } else if(level!="level1"){
	string msg("MFront::treatOBuild : ");
	msg += "unsupported value '"+level+
	  "' for the --obuild option";
	throw(runtime_error(msg));
      }
    } else {
      this->oflags    = true;
    }
  } // end of MFront::treatOBuild

#endif /* MFRONT_MAKE_SUPPORT */
  
  void
  MFront::treatListParsers(void)
  {
    using namespace std;
    cout << "available dsl : " << endl;
    auto& parserFactory = DSLFactory::getDSLFactory();
    const auto& parsers = parserFactory.getRegistredParsers();
    auto p = parsers.begin();
    while(p!=parsers.end()){
      auto tmp = "- " + *p;
      if(tmp.size()<=32){
	tmp.insert(tmp.size(),32-tmp.size(),' ');
      }
      cout << tmp << " : " << parserFactory.getParserDescription(*p) << ".\n";
      ++p;
    }
    exit(EXIT_SUCCESS);
  } // end of MFront::treatListParsers

#ifdef MFRONT_MAKE_SUPPORT
  
  void
  MFront::treatSilentBuild(void)
  {
    using namespace std;
    const auto& o = this->currentArgument->getOption();
    if(o.empty()){
      string msg("MFront::treatSilentBuild : ");
      msg += "no argument given to the --silentBuild option";
      throw(runtime_error(msg));
    }
    if(o=="on"){
      this->silentBuild=true;
    } else if(o=="off"){
      this->silentBuild=false;
    } else {
      string msg("MFront::treatSilentBuild : ");
      msg += "unsupported argument '"+o+"' given to the --silentBuild option";
      throw(runtime_error(msg));
    }
  } // end of MFront::treatSilentBuild

  void
  MFront::treatTarget(void)
  {
    using namespace std;
    using tfel::utilities::tokenize;
    const auto& t = tokenize(this->currentArgument->getOption(),',');
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

#endif /* MFRONT_MAKE_SUPPORT */
  
#if !(defined _WIN32 || defined _WIN64 ||defined __CYGWIN__)
  void
  MFront::treatWin32(void)
  {
    this->sys = "win32";
  } // end of MFront::treatWin32
#endif /* __CYGWIN__ */

  void 
  MFront::registerArgumentCallBacks(void)
  {
    this->registerNewCallBack("--verbose",&MFront::treatVerbose,
			      "set verbose output",true);
    this->registerNewCallBack("--list-parsers",&MFront::treatListParsers,"list all available domain specific languages (deprecated)");
    this->registerNewCallBack("--list-dsl",&MFront::treatListParsers,"list all available domain specific languages");
    this->registerNewCallBack("--help-commands-list",&MFront::treatHelpCommandsList,
			      "list all keywords for the given domain specific language and exits",true);
    this->registerNewCallBack("--help-keywords-list",&MFront::treatHelpCommandsList,
			      "list all keywords for the given domain specific language and exits",true);
    this->registerNewCallBack("--include","-I",&MFront::treatSearchPath,
			      "add a new path at the beginning of the search paths",true);
    this->registerNewCallBack("--search-path",&MFront::treatSearchPath,
			      "add a new path at the beginning of the search paths",true);
    this->registerNewCallBack("--help-keyword",&MFront::treatHelpCommand,
			      "display the help associated for the given domain specific language and exits",true);
    this->registerNewCallBack("--help-command",&MFront::treatHelpCommand,
			      "display the help associated for the given domain specific language and exits",true);
    this->registerNewCallBack("--debug",&MFront::treatDebug,
			      "set debug mode (remove references to initial file)");
    this->registerNewCallBack("--warning","-W",&MFront::treatWarning,"print warnings");
    this->registerNewCallBack("--pedantic",&MFront::treatPedantic,
			      "print pedantic warning message");
    this->registerNewCallBack("--interface","-i",&MFront::treatInterface,
			      "specify which interface to use",true);
#if (defined _WIN32 || defined _WIN64 ||defined __CYGWIN__)
    this->registerNewCallBack("--def-file",&MFront::treatDefFile,
			      "outputs def file associated with the libraries given in arguments (separated by commas)",true);
#endif    
#ifdef MFRONT_MAKE_SUPPORT
    this->registerNewCallBack("--make",&MFront::treatMake,
			      "generate MakeFile (see also --build)");
    this->registerNewCallBack("--build",&MFront::treatBuild,
			      "generate MakeFile and build libraries");
    this->registerNewCallBack("--omake","-m",&MFront::treatOMake,
			      "generate MakeFile with optimized compilations flags (see also --obuild)",true);
    this->registerNewCallBack("--obuild","-b",&MFront::treatOBuild,
			      "generate MakeFile with optimized compilations flags and build libraries",true);
    this->registerNewCallBack("--target","-t",&MFront::treatTarget,
			      "generate MakeFile and build the specified target",true);
    this->registerNewCallBack("--otarget",&MFront::treatOTarget,
			      "generate MakeFile with optimized compilations flags and build the specified target",true);
    this->registerNewCallBack("--clean",&MFront::treatClean,
			      "generate MakeFile and clean libraries");
    this->registerNewCallBack("--silent-build",&MFront::treatSilentBuild,
			      "active or desactivate silent build",true);
#if (defined _WIN32 || defined _WIN64 ||defined __CYGWIN__)
    this->registerNewCallBack("--nodeps",&MFront::treatNoDeps,
			      "don't generate compilation dependencies");
#endif /* __CYGWIN__ */
#endif /* MFRONT_MAKE_SUPPORT */    
    this->registerNewCallBack("--nomelt",&MFront::treatNoMelt,
			      "don't melt librairies sources");
#if !(defined _WIN32 || defined _WIN64 ||defined __CYGWIN__)
    this->registerNewCallBack("--win32",&MFront::treatWin32,
			      "specify that the target system is win32");
#endif /* __CYGWIN__ */
  } // end of MFront::registerArgumentCallBacks

  MFront::MFront()
    : tfel::utilities::ArgumentParserBase<MFront>()
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
    ,sys("win32")
#elif defined __APPLE__
    ,sys("apple")
#else
    ,sys("default")
#endif /* __CYGWIN__ */
  {} // end of MFront::MFront

  MFront::MFront(const int argc, const char *const *const argv)
    : MFront()
  {
    this->setArguments(argc,argv);
    this->registerArgumentCallBacks();
    this->parseArguments();
  } // end of MFront::MFront

  void
  MFront::treatHelpCommandsList(void)
  {
    using namespace std;
    using namespace tfel::utilities;
    auto& f = DSLFactory::getDSLFactory();
    const auto& o = this->currentArgument->getOption();
    if(o.empty()){
      string msg("MFront::treatHelpCommandsList : ");
      msg += "no parser name given";
      throw(runtime_error(msg));
    }
    shared_ptr<AbstractDSL> p{f.createNewParser(o)};
    vector<string> k;
    vector<string>::const_iterator pk;
    p->getKeywordsList(k);
    string::size_type msize = 0;
    for(pk=k.begin();pk!=k.end();++pk){
      msize = max(msize,pk->size());
    }
    for(pk=k.begin();pk!=k.end();++pk){
      string fp = getDocumentationFilePath(o,*pk);
      string key = *pk;
      key.resize(msize,' ');
      cout << key << "  ";
      if(!fp.empty()){
	cout.write(TerminalColors::Green,sizeof(TerminalColors::Green));
	cout << "(documented)";
      } else {
	cout.write(TerminalColors::Red,sizeof(TerminalColors::Red));
	cout << "(undocumented)";
      }
      cout.write(TerminalColors::Reset,sizeof(TerminalColors::Reset));
      cout << endl;
    }
    exit(EXIT_SUCCESS);
  } // end of MFront::treatHelpCommandsList

  void
  MFront::treatHelpCommand(void)
  {
    using namespace std;
    auto& f = DSLFactory::getDSLFactory();
    const auto& o = this->currentArgument->getOption();
    if(o.empty()){
      string msg("MFront::treatHelpCommand : ");
      msg += "no argument given";
      throw(runtime_error(msg));
    }
    string pn;
    string k;
    string::size_type pos = o.rfind(':');
    if((pos==string::npos)||(pos+1==o.size())){
      string msg("MFront::treatHelpCommand : ");
      msg += "ill-formed argument, expected 'parser:@keyword'";
      throw(runtime_error(msg));
    }
    pn = o.substr(0,pos); // parser name
    k  = o.substr(pos+1); // key
    if((pn.empty())||(k.empty())){
      string msg("MFront::treatHelpCommand : ");
      msg += "ill-formed argument, expected 'parser:@keyword'";
      throw(runtime_error(msg));
    }
    if(k[0]!='@'){
      string msg("MFront::treatHelpCommand : ");
      msg += "ill-formed argument, expected 'parser:@keyword'";
      throw(runtime_error(msg));
    }
    auto p = f.createNewParser(pn);
    vector<string> keys;
    p->getKeywordsList(keys);
    if(find(keys.begin(),keys.end(),k)==keys.end()){
      string msg("MFront::treatHelpCommand : ");
      msg += "keyword '"+k+"' is not declared ";
      throw(runtime_error(msg));
    }
    string fp = getDocumentationFilePath(pn,k);
    if(fp.empty()){
      cout << "no description available for keyword '"
	   << k << "'" << endl;
    } else {
      ifstream desc(fp);
      if(!desc){
	// note, this shall never append...
	cout << "can't access to the description of keyword '"
	     << k << "'" << endl;
      } else {
	cout << desc.rdbuf();
      }
    }
    exit(EXIT_SUCCESS);
  } // end of MFront::treatHelpCommand

#ifdef MFRONT_MAKE_SUPPORT
  void
  MFront::treatNoDeps(void)
  {
    this->nodeps = true;
  } // end of MFront::treatNoDeps
#endif /* MFRONT_MAKE_SUPPORT */

  void MFront::treatNoMelt(void)
  {
    this->melt = false;
  } // end of MFront::treatNoMelt

#if (defined _WIN32 || defined _WIN64 ||defined __CYGWIN__)
  void MFront::treatDefFile(void){
    const auto& o = this->getCurrentCommandLineArgument().getOption();
    if(o.empty()){
      throw(std::runtime_error("MFrontBase::treatDefFile : "
			       "no option given to the "
			       "'--def-file' argument"));
    }
    for(const auto& l : tfel::utilities::tokenize(o,',')){
      if(l.empty()){
	throw(std::runtime_error("MFrontBase::treatDefFile : "
				 "empty library specified."));
      }
      this->defs.insert(l);
    }    
  } // end of void MFront::treatDefFile
#endif /* (defined _WIN32 || defined _WIN64 ||defined __CYGWIN__) */
  
  TargetsDescription
  MFront::treatFile(const std::string& f) const
  {
    using namespace std;
    if(getVerboseMode()>=VERBOSE_LEVEL2){
      auto& log = getLogStream();
      log << "Treating file : '" << f << "'" <<  endl;
    }
    shared_ptr<AbstractDSL> dsl = MFrontBase::getDSL(f);
    if(!this->interfaces.empty()){
      dsl->setInterfaces(this->interfaces);
    }
    dsl->analyseFile(f,this->ecmds);
    dsl->generateOutputFiles();
    return dsl->getTargetsDescription();
  } // end of MFront::treatFile(void)
  
  void
  MFront::analyseSources(const std::string& name)
  {
    using namespace std;
    using namespace tfel::system;
    ifstream file;
    ofstream f;
    set<string> files; // list of files contained in the file
    string fName;
    set<string>::iterator p;
    set<string>::iterator p2;
    bool erased = false;
    fName = "src"+dirStringSeparator()+ name;
    checkIfFileIsRegularAndReadable(fName);
    file.open(fName);
    if(!file){
      string msg("MFront::analyseSources : can't open file ");
      msg += "'"+fName+'\'';
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
      // check that the source has a valid extension
      auto hasValidExtension = false;
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
    }
    p=files.begin();
    while(p!=files.end()){
      const string fn = "src/"+dirStringSeparator()+*p;
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
      if(access(fn.c_str(),0)==0){
	checkIfFileIsRegularAndReadable(fn);
	++p;
      } else {
	// removing file from the list
	p2 = p;
	++p;
	files.erase(p2);
	erased = true;
      }
#else
      if(access(fn.c_str(),F_OK)==0){
	checkIfFileIsRegularAndReadable(fn);
	++p;
      } else {
	// removing file from the list
	p2 = p;
	++p;
	files.erase(p2);
	erased = true;
      }
#endif
    }
    if(erased){
      if(files.empty()){
	if(unlink(fName.c_str())!=0){
	  string msg("MFront::analyseSources : can't unlink file '"+fName+'\'');
	  msg += "\nError while analysing \'"+fName+'\'';
	  throw(runtime_error(msg));
	}
	return;
      }
      f.open(fName);
      if(!f){
	string msg("MFront::analyseSources : ");
	msg += "error while opening '"+fName+"' for writing.\n";
	msg += "\nError while analysing '"+fName+"'";
	throw(runtime_error(msg));
      }
      copy(files.begin(),files.end(),ostream_iterator<string>(f,"\n"));
      f.close();
    }
    if(!files.empty()){
      if(getVerboseMode()>=VERBOSE_LEVEL2){
	auto& log = getLogStream();
	log << "inserting library " << name.substr(0,name.size()-3) << "so "
	     << "sources : \n";
	copy(files.begin(),files.end(),ostream_iterator<string>(log," "));
	log << endl;
      }
      insert_if(this->targets[name.substr(0,name.size()-4)].sources,files);
    }
  } // end of MFront::analyseSources

  void
  MFront::analyseDependencies(const std::string& name)
  {
    using namespace std;
    using namespace tfel::system;
    ifstream file;
    vector<string> libs; // list of libraries contained in the file
    string fName;
    string line;
    fName = "src" +dirStringSeparator() + name;
    checkIfFileIsRegularAndReadable(fName);
    file.open(fName);
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
      if(getVerboseMode()>=VERBOSE_LEVEL2){
	auto& log = getLogStream();
	log << "inserting library " << name.substr(0,name.size()-4) << "so "
	     << "dependencies : \n";
	copy(libs.begin(),libs.end(),ostream_iterator<string>(log," "));
	log << endl;
      }
      insert_if(this->targets[name.substr(0,name.size()-5)].dependencies,libs);
    }
  } // end of MFront::analyseDependencies

  void
  MFront::analyseEntryPoints(const std::string& name)
  {
    using namespace std;
    using namespace tfel::system;
    const auto fName = "src" +dirStringSeparator() + name;
    checkIfFileIsRegularAndReadable(fName);
    ifstream file(fName);
    if(!file){
      string msg("MFront::analyseEntryPoints : can't open file '");
      msg += fName + "'";
      throw(runtime_error(msg));
    }
    auto nepts = vector<string>{}; // list of entry points contained in the file
    while(!file.eof()){
      string line;
      getline(file,line);
      nepts.push_back(line);
    }
    file.close();
    if(nepts.empty()){
      if(unlink(fName.c_str())!=0){
	string msg("MFront::analyseEntryPoints : can't unlink file '"+fName+'\'');
	throw(runtime_error(msg));
      }
      return;
    }
    if(!nepts.empty()){
      if(getVerboseMode()>=VERBOSE_LEVEL2){
	auto& log = getLogStream();
	log << "inserting library '" << name.substr(0,name.size()-4) << "' "
	     << "entry points : \n";
	copy(nepts.begin(),nepts.end(),ostream_iterator<string>(log," "));
	log << endl;
      }
      insert_if(this->targets[name.substr(0,name.size()-5)].epts,nepts);
    }
  } // end of MFront::analyseEntryPoints

  void
  MFront::analyseCppFlags()
  {
    using namespace std;
    using namespace tfel::system;
    using namespace tfel::utilities;
    ifstream file;
    string name = "src"+dirStringSeparator()+"Makefile.incs";
    string line;
    checkIfFileIsRegularAndReadable(name);
    file.open(name);
    if(!file){
      string msg("MFront::analyseCppFlags : can't open file '"+name+"'");
      throw(runtime_error(msg));
    }
    while(!file.eof()){
      getline(file,line);
      if(!line.empty()){
	this->cppflags.insert(line);
      }
    }
    file.close();
  } // end of MFront::analyseCppFlags

  void
  MFront::analyseMakefileSpecificTargets()
  {
    using namespace std;
    using namespace tfel::system;
    using namespace tfel::utilities;
    ifstream file;
    string name = "src"+dirStringSeparator()+"Makefile.spec";
    string line;
    unsigned short lineNbr;
    vector<string>::const_iterator p;
    checkIfFileIsRegularAndReadable(name);
    file.open(name);
    lineNbr = 0u;
    if(file.eof()){
      return;
    }
    getline(file,line);
    ++lineNbr;
    auto& st = this->targets.specific_targets;
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
	      if(find(st[words[0]].first.begin(),
		      st[words[0]].first.end(),*p)==st[words[0]].first.end()){
		st[words[0]].first.push_back(*p);
	      }
	    }
	    if(!file.eof()){
	      getline(file,line);
	      ++lineNbr;
	      while((!file.eof())&&(!line.empty())){
		if(find(st[words[0]].second.begin(),
			st[words[0]].second.end(),line)==st[words[0]].second.end()){
		  st[words[0]].second.push_back(line);
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
  } // end of Mfront::analyseMakefileSpecificTargets

  void
  MFront::analyseSourceDirectory(void)
  {
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
    using namespace std;
    using namespace tfel::system;
    vector<string> files;
    vector<string>::const_iterator p;
    HANDLE          hFile;                // Handle to file
    WIN32_FIND_DATA FileInformation;      // File information
    auto& l = MFrontLock::getMFrontLock();
    mlock.lock();
    try{
      hFile = ::FindFirstFile("src/*", &FileInformation);
      if(hFile == INVALID_HANDLE_VALUE){
	throw(runtime_error("MFront::analyseSourceDirectory : can't open directory 'src'"));
      }
      do {
	if(FileInformation.cFileName[0] != '.'){
	  if(!(FileInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)){
	    files.push_back(FileInformation.cFileName);
	  }
	}
      } while(::FindNextFile(hFile, &FileInformation) == TRUE);
      // Close handle
      ::FindClose(hFile);
      for(p=files.begin();p!=files.end();++p){
	if(p->size()>4){
	  if(p->substr(p->size()-4)==".src"){
	    if(getVerboseMode()>=VERBOSE_LEVEL2){
	      auto& log = getLogStream();
	      log << "treating library '" << p->substr(0,p->size()-4) << "' sources.\n";
	    }
	    this->analyseSources(*p);
	  }
	}
	if(p->size()>5){
	  if(p->substr(p->size()-5)==".epts"){
	    if(getVerboseMode()>=VERBOSE_LEVEL2){
	      auto& log = getLogStream();
	      log << "treating library '" << p->substr(0,p->size()-5) << "' entry points.\n";
	    }
	    this->analyseEntryPoints(*p);
	  }
	}
	if(*p=="Makefile.spec"){
	  this->analyseMakefileSpecificTargets();
	}
	if(*p=="Makefile.incs"){
	  this->analyseCppFlags();
	}
      }
      // we treat dependencies separetly because some file might had been
      // suppressed
      files.clear();
      hFile = ::FindFirstFile("src", &FileInformation);
      if(hFile == INVALID_HANDLE_VALUE){
	throw(runtime_error("MFront::analyseSourceDirectory : can't open directory 'src'"));
      }
      do {
	if(FileInformation.cFileName[0] != '.'){
	  if(!(FileInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)){
	    files.push_back(FileInformation.cFileName);
	  }
	}
      } while(::FindNextFile(hFile, &FileInformation) == TRUE);
      // Close handle
      ::FindClose(hFile);
      for(p=files.begin();p!=files.end();++p){
	if(p->size()>5){
	  if(p->substr(p->size()-5)==".deps"){
	    if(getVerboseMode()>=VERBOSE_LEVEL2){
	      auto& log = getLogStream();
	      log << "treating library " << p->substr(0,p->size()-5) << " dependencies.\n";
	    }
	    this->analyseDependencies(*p);
	  }
	}
      }
    }
    catch(...){
      mlock.unlock();
      throw;
    }
    mlock.unlock();
#else
    using namespace std;
    using namespace tfel::system;
    vector<string> files;
    vector<string>::const_iterator p;
    struct stat buf;
    auto& mlock = MFrontLock::getMFrontLock();
    mlock.lock();
    try{
      auto directory = opendir("src");
      if(!directory){
	throw(runtime_error("MFront::analyseSourceDirectory : can't open directory 'src'"));
      }
      auto dir=readdir(directory);
      while(dir!=nullptr){
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
	if(p->substr(p->size()-4)==".src"){
	  if(getVerboseMode()>=VERBOSE_LEVEL2){
	    auto& log = getLogStream();
	    log << "treating library " << p->substr(0,p->size()-4) << " sources.\n";
	  }
	  this->analyseSources(*p);
	}
	if(p->size()>5){
	  if(p->substr(p->size()-5)==".epts"){
	    if(getVerboseMode()>=VERBOSE_LEVEL2){
	      auto& log = getLogStream();
	      log << "treating library " << p->substr(0,p->size()-5) << " entry points.\n";
	    }
	    this->analyseEntryPoints(*p);
	  }
	}
	if(*p=="Makefile.spec"){
	  this->analyseMakefileSpecificTargets();
	}
	if(*p=="Makefile.incs"){
	  this->analyseCppFlags();
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
      while(dir!=nullptr){
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
	    if(getVerboseMode()>=VERBOSE_LEVEL2){
	      auto& log = getLogStream();
	      log << "treating library " << p->substr(0,p->size()-5) << " dependencies.\n";
	    }
	    this->analyseDependencies(*p);
	  }
	}
      }
    }
    catch(...){
      mlock.unlock();
      throw;
    }
    mlock.unlock();
#endif
  } // end of MFront::analyseSourceDirectory
  
  void
  MFront::writeSourcesLists(void) const
  {
    using namespace std;
    using namespace tfel::system;
    ofstream file;
    auto& mlock = MFrontLock::getMFrontLock();
    mlock.lock();
    try{
      for(const auto& l : this->targets){
	if(getVerboseMode()>=VERBOSE_LEVEL2){
	  auto& log = getLogStream();
	  log << "writing sources list for library '"
	      << l.name << "." << l.suffix << "'\n";
	}
	file.open("src"+dirStringSeparator()+l.name+".src");
	if(!file){
	  string msg("MFront::writeSourcesLists : ");
	  msg += "can't open file 'src"+dirStringSeparator()+l.name+".src";
	  throw(runtime_error(msg));
	}
	copy(l.sources.begin(),l.sources.end(),
	     ostream_iterator<string>(file,"\n"));
	file.close();
      }
    }
    catch(...){
      mlock.unlock();
      throw;
    }
    mlock.unlock();
  } // end of MFront::writeSourceLists

  void
  MFront::writeEntryPointsLists(void) const
  {
    using namespace std;
    using namespace tfel::system;
    auto& mlock = MFrontLock::getMFrontLock();
    mlock.lock();
    try{
      for(const auto& l : this->targets){
	if(getVerboseMode()>=VERBOSE_LEVEL2){
	  auto& log = getLogStream();
	  log << "writing entry points list for library '"
	      << l.name << "." << l.suffix << "'\n";
	}
	ofstream file("src"+dirStringSeparator()+l.name+".epts");
	if(!file){
	  string msg("MFront::writeEntryPointsLists : ");
	  msg += "can't open file 'src"+dirStringSeparator()+l.name+".epts'";
	  throw(runtime_error(msg));
	}
	copy(l.epts.begin(),l.epts.end(),ostream_iterator<string>(file,"\n"));
	file.close();
      }
    }
    catch(...){
      mlock.unlock();
      throw;
    }
    mlock.unlock();
  } // end of MFront::writeEntryPointsLists
  
  void
  MFront::writeDependenciesLists(void) const
  {
    using namespace std;
    using namespace tfel::system;
    ofstream file;
    auto& mlock = MFrontLock::getMFrontLock();
    mlock.lock();
    try{
      for(const auto& l : this->targets){
	if(getVerboseMode()>=VERBOSE_LEVEL2){
	  auto& log = getLogStream();
	  log << "writing dependencies list for library "
	      << l.name << "." << l.suffix << "\n";
	}
	file.open(("src"+dirStringSeparator()+l.name+".deps").c_str());
	copy(l.dependencies.begin(),l.dependencies.end(),ostream_iterator<string>(file,"\n"));
	file.close();
      }
    }
    catch(...){
      mlock.unlock();
      throw;
    }
    mlock.unlock();
  } // end of MFront::writeDependenciesLists

  void
  MFront::writeSpecificTargets(void) const
  {
    using namespace std;
    using namespace tfel::system;
    typedef map<string,pair<vector<string>,vector<string> > > Target;
    ofstream file;
    Target::const_iterator p;
    auto& mlock = MFrontLock::getMFrontLock();
    mlock.lock();
    try{
      const auto& st = this->targets.specific_targets;
      if(!st.empty()){
	file.open(("src"+dirStringSeparator()+"Makefile.spec").c_str());
	if(!file){
	  string msg("MFront::writeSpecificTargets : can't open file 'src"+dirStringSeparator()+"Makefile.spec'");
	  throw(runtime_error(msg));
	}
	for(p=st.begin();p!=st.end();++p){
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
      mlock.unlock();
      throw;
    }
    mlock.unlock();
  } // end of MFront::writeSpecificTargets

  void
  MFront::writeCppFlags(void) const
  {
    using namespace std;
    using namespace tfel::system;
    auto& mlock = MFrontLock::getMFrontLock();
    mlock.lock();
    try{
      if(!this->cppflags.empty()){
	ofstream file{"src"+dirStringSeparator()+"Makefile.incs"};
	if(!file){
	  string msg("MFront::writeCppFlags : can't open file 'src"+
		     dirStringSeparator()+"Makefile.incs'");
	  throw(runtime_error(msg));
	}
	copy(this->cppflags.begin(),
	     this->cppflags.end(),ostream_iterator<string>(file,"\n"));
      }
    }
    catch(...){
      mlock.unlock();
      throw;
    }
    mlock.unlock();
  } // end of MFront::writeCppFlags

  std::pair<bool,std::pair<std::string,std::string> >
  MFront::getLibraryDependencies(const std::string& name) const
  {
    using namespace std;
    const auto& l = this->targets[name];
    auto res = pair<bool,pair<string,string> >{};
    res.first = false;
    for(const auto& s : l.sources){
      if(s.size()>4){
	const auto ext = s.substr(s.size()-4);
	if((ext==".cpp")||(ext==".cxx")){
	  res.first = true;
	  res.second.first += s.substr(0,s.size()-4)+".o ";
	}
      }
      if(s.size()>2){
	if(s.substr(s.size()-2)==".c"){
	  res.second.first += s.substr(0,s.size()-2)+".o ";
	}
      }
    }
    for(const auto& d : l.dependencies){
      if(d.size()>2){
	if(d.substr(0,2)=="-l"){
	  auto lib = d.substr(2);
	  if(describes(this->targets,lib)){
	    if(this->melt){
	      auto dep = this->getLibraryDependencies("lib"+lib);
	      res.first = res.first || dep.first;
	      res.second.first  += dep.second.first;
	      res.second.second += dep.second.second;
	    } else {
	      res.second.second += "lib";
	      res.second.second += lib;
	      res.second.second +=  + "."+l.suffix+" ";
	    }
	  }
	}
      }
    }
    return res;
  } // end of MFront::getLibraryDependencies

#ifdef MFRONT_MAKE_SUPPORT
  std::string
  MFront::getLibraryLinkDependencies(const std::string& name)
  {
    if(!describes(this->targets,name)){
      throw(std::runtime_error("MFront::getLibraryDependencies : no library named '"+name+"'.\n"
			       "Internal Error."));
    }
    const auto& l = this->targets[name];
    if(l.dependencies.empty()){
      return {};
    }
    auto res = std::string{};
    this->makeFile << "-L. ";
    for(const auto& d : l.dependencies){
      if(!d.empty()){
	if(d.size()>2){
	  if(d.substr(0,2)=="-l"){
	    if(describes(this->targets,"lib"+d.substr(2))){
	      if(!this->melt){
		res += this->getLibraryLinkDependencies(d) + " " + d + " ";
	      }
	    } else {
	      res += d + " ";
	    }
	  } else {
	    res += d + " ";
	  }
	} else {
	  res += d + " ";
	}
      }
    }
    return this->sortLibraryList(res);
  } // end of MFront::getLibraryLinkDependencies(const std::string&)
#endif /* MFRONT_MAKE_SUPPORT */
  
  std::string
  MFront::sortLibraryList(const std::string& lib) const
  {
    std::istringstream tokenizer{lib};
    auto libs = std::vector<std::string>{};
    auto vres = std::vector<std::string>{};
    copy(std::istream_iterator<std::string>(tokenizer),
	 std::istream_iterator<std::string>(),back_inserter(libs));
    auto p  = libs.crbegin();
    const auto pe = libs.crend();
    auto res = std::string{};
    while(p!=pe){
      if(find(vres.begin(),vres.end(),*p)==vres.end()){
	vres.push_back(*p);
	res = *p+" "+res; 
      }
      ++p;
    }
    return res;
  } // end of sortLibraryList

#ifdef MFRONT_MAKE_SUPPORT  
  void
  MFront::generateMakeFile(void)
  {
    using namespace std;
    using namespace tfel::system;
    set<string> cppSources;
    set<string> cSources;
    string sb;
    string cc;
    string cxx;
    const char * const env_cc  = ::getenv("CC");
    const char * const env_cxx = ::getenv("CXX");
    const char * const inc = ::getenv("INCLUDES");
    const char * const cxxflags = ::getenv("CXXFLAGS");
    const char * const cflags   = ::getenv("CFLAGS");
    const char * const ldflags  = ::getenv("LDFLAGS");
    if(this->silentBuild){
      sb = "@"; 
    }
    if(getVerboseMode()>=VERBOSE_LEVEL2){
      auto& log = getLogStream();
      log << "generating Makefile\n";
    }
    if(env_cxx==nullptr){
      cxx = "$(CXX)";
    } else {
      cxx = env_cxx;
    }
    if(env_cc==nullptr){
      cc = "$(CC)";
    } else {
      cc = env_cc;
    }
    auto& mlock = MFrontLock::getMFrontLock();
    mlock.lock();
    try{
      this->makeFile.open("src"+dirStringSeparator()+"Makefile.mfront");
      this->makeFile.exceptions(ios::badbit|ios::failbit);
      if(!this->makeFile){
	throw(runtime_error("MFront::generateMakeFile : can't open file Makefile.mfront"));
      }
      for(const auto& l : this->targets){
	for(const auto& s : l.sources){
	  if(s.size()>4){
	    if((s.substr(s.size()-4)==".cpp")||
	       (s.substr(s.size()-4)==".cxx")){
	      cppSources.insert(s);
	    }
	  }
	  if(s.size()>2){
	    if(s.substr(s.size()-2)==".c"){
	      cSources.insert(s);
	    }
	  }
	}
      }
      this->makeFile << "# Makefile generated by mfront.\n"
		     << MFrontHeader::getHeader("# ") << "\n";
      this->makeFile << "export LD_LIBRARY_PATH:=$(PWD):$(LD_LIBRARY_PATH)\n\n";
      // COMPILERS
      if(env_cc!=nullptr){
	this->makeFile << "CC := "  << env_cc  << "\n";
      }
      if(env_cxx!=nullptr){
	this->makeFile << "CXX := " << env_cxx << "\n";
      }
      if((env_cc!=nullptr)||(env_cxx!=nullptr)){
	this->makeFile << endl;
      }
      // INCLUDES
      this->makeFile << "INCLUDES := ";
      if(inc!=nullptr){
	this->makeFile << inc << " ";
      }
      this->makeFile << "-I../include `tfel-config --includes`";
      if(!this->cppflags.empty()){
	this->makeFile << " \\\n";
	for(auto p7=this->cppflags.begin();p7!=this->cppflags.end();){
	  this->makeFile << "\t     " << *p7;
	  if(++p7!=this->cppflags.end()){
	    this->makeFile << " \\\n";
	  }
	}
      }
      // adding the mfront search path to the include files
      if(!SearchFile::getSearchPaths().empty()){
	const auto& paths = SearchFile::getSearchPaths();
	for(const auto& path : paths){
	  this->makeFile << "\\\n\t     -I" << path;
	}
      }
      //
      this->makeFile << endl << endl;
      // LDFLAGS
      if(ldflags!=nullptr){
	this->makeFile << "LDFLAGS := " << ldflags  << endl;
      }
      // CXXFLAGS
      if(!cppSources.empty()){
	this->makeFile << "CXXFLAGS := -Wall -Wfatal-errors ";
#if ! (defined _WIN32 || defined _WIN64 ||defined __CYGWIN__)
	this->makeFile << "-ansi ";
#endif /* __CYGWIN__ */
	if(cxxflags!=nullptr){
	  this->makeFile << cxxflags << " ";
	} else if(this->oflags0||this->oflags||this->oflags2){
	  if(this->oflags2){
	    this->makeFile << "`tfel-config --compiler-flags --oflags --oflags2` ";
	  } else if(this->oflags){
	    this->makeFile << "`tfel-config --compiler-flags --oflags` ";
	  } else {
	    this->makeFile << "`tfel-config --compiler-flags --oflags0` ";
	  }
	} else {
	  this->makeFile << "-O2 `tfel-config --compiler-flags`";
	}
	if(this->sys=="win32"){
	  this->makeFile << "-D'F77_FUNC(X,Y)=X\\#\\#_' -D'F77_FUNC_(X,Y)=X\\#\\#__' -DWIN32 $(INCLUDES) \n\n";
	} else {
	  this->makeFile << "-D'F77_FUNC(X,Y)=X\\#\\#_' -D'F77_FUNC_(X,Y)=X\\#\\#_' -fPIC $(INCLUDES) \n\n";
	}
      }
      // CFLAGS
      if(!cSources.empty()){
	this->makeFile << "CFLAGS := -W -Wall -Wfatal-errors ";
#if ! (defined _WIN32 || defined _WIN64 ||defined __CYGWIN__)
	this->makeFile << "-ansi -std=c99 ";
#endif /* __CYGWIN__ */
	if(cflags!=nullptr){
	  this->makeFile << cflags << " ";
	} else if(this->oflags0||this->oflags||this->oflags2){
	  if(this->oflags2){
	    this->makeFile << "`tfel-config --oflags --oflags2` ";
	  } else if(this->oflags){
	    this->makeFile << "`tfel-config --compiler-flags --oflags` ";
	  } else {
	    this->makeFile << "`tfel-config --compiler-flags --oflags0` ";
	  }
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
	auto p4=cppSources.begin();
	while(p4!=cppSources.end()){
	  this->makeFile << *p4;
	  if(++p4!=cppSources.end()){
	    this->makeFile << " ";
	  }
	}
	this->makeFile << "\n\n";
      }
      if(!cSources.empty()){
	this->makeFile << "SRC = ";
	auto p4=cSources.begin();
	while(p4!=cSources.end()){
	  this->makeFile << *p4;
	  if(++p4!=cSources.end()){
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
      for(const auto& l : this->targets){
	this->makeFile << l.name << "." << l.suffix << " ";
      }
      for(const auto& t : this->targets.specific_targets){
	if((t.first!="all")&&(t.first!="clean")){
	  this->makeFile << t.first << " ";
	}
      }
      this->makeFile << "\n\n";
      this->makeFile << "all : ";
      for(const auto& l : this->targets){
	this->makeFile << l.name << "." << l.suffix << " ";
      }
      auto p5=this->targets.specific_targets.find("all");
      if(p5!=this->targets.specific_targets.end()){
	copy(p5->second.first.begin(),p5->second.first.end(),
	     ostream_iterator<string>(this->makeFile," "));
	this->makeFile << "\n";
	for(const auto& cmd : p5->second.second){
	  this->makeFile << "\t" << cmd << "\n";
	}
      }
      this->makeFile << "\n";
      for(const auto& t : this->targets.specific_targets){
	if((t.first!="all")&&(t.first!="clean")){
	  this->makeFile << t.first << " : ";
	  copy(t.second.first.begin(),t.second.first.end(),
	       ostream_iterator<string>(this->makeFile," "));
	  this->makeFile << endl;
	  for(const auto& cmd : t.second.second){
	    this->makeFile << "\t" << cmd << endl;
	  }
	  this->makeFile << "\n";
	}
      }
      for(const auto& l : this->targets){
	this->makeFile << l.name << "." << l.suffix << " : ";
	auto dep = this->getLibraryDependencies(l.name);
	const auto hasCxxSources = dep.first;
	if(!dep.second.first.empty()){
	  this->makeFile << dep.second.first;
	} 
	if(!dep.second.second.empty()){
	  this->makeFile << this->sortLibraryList(dep.second.second);
	}
	this->makeFile << "\n\t";
	if(hasCxxSources){
	  this->makeFile << sb << cxx << " ";
	} else {
	  this->makeFile << sb << cc  << " ";
	}
	if(ldflags!=nullptr){
	  this->makeFile << "$(LDFLAGS) ";
	}
	if(this->sys=="win32"){
	  this->makeFile << "-shared -Wl,--add-stdcall-alias,--out-implib," << l.name << "_dll.a,-no-undefined ";
	} else if(this->sys=="apple"){
	  this->makeFile << "-bundle ";
	} else {
	  this->makeFile << "-shared ";
	}
	this->makeFile << "$^  -o $@ ";
	this->makeFile << this->getLibraryLinkDependencies(l.name);
	this->makeFile << "\n\n";
      }
      this->makeFile << "clean : ";
      p5=this->targets.specific_targets.find("clean");
      if(p5!=this->targets.specific_targets.end()){
	copy(p5->second.first.begin(),
	     p5->second.first.end(),ostream_iterator<string>(this->makeFile," "));
      }
      this->makeFile << "\n";
      if(this->sys=="win32"){
	this->makeFile << "\t"+sb+"rm -f *.o *.dll *.d\n";
      } else if(this->sys=="apple"){
	this->makeFile << "\t"+sb+"rm -f *.o *.bundle *.d\n";
      } else {
	this->makeFile << "\t"+sb+"rm -f *.o *.so *.d\n";
      }
      if(p5!=this->targets.specific_targets.end()){
	for(const auto& cmd : p5->second.second){
	  this->makeFile << "\t" << cmd << endl;
	}
      }
      this->makeFile << "\n";
      if(!this->nodeps){
	this->makeFile << "-include $(makefiles)\n\n";
      }
      // generic rules for objects file generation
      if(!cppSources.empty()){
	this->makeFile << "%.o:%.cxx\n";
	this->makeFile << "\t" << sb << cxx << " $(CXXFLAGS) $< -o $@ -c\n\n";
	this->makeFile << "%.o:%.cpp\n";
	this->makeFile << "\t" << sb << cxx << " $(CXXFLAGS) $< -o $@ -c\n\n";
      }
      if(!cSources.empty()){
	this->makeFile << "%.o:%.c\n";
	this->makeFile << "\t" << sb << cc << " $(CFLAGS) $< -o $@ -c\n\n";
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
      mlock.unlock();
      throw;
    }
    mlock.unlock();
  } // end of MFront::generateMakeFile
  
  void
  MFront::buildLibraries(const std::string& target)
  {
    using namespace std;
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
    const char *const argv[] = {"make.exe","-C","src",
				"-f","Makefile.mfront",
				target.c_str(),nullptr};
    if(_spawnvp(_P_WAIT,"make.exe",argv)!=0){
      string msg("MFront::buildLibraries : ");
      msg += "can't build target '"+target+"'";
      throw(runtime_error(msg));
    }
#else
    const char *const argv[] = {"make","-f",
				"Makefile.mfront",target.c_str(),
				nullptr};
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
#endif
  } // end of MFront::buildLibraries

  void
  MFront::cleanLibraries(void)
  {
    using namespace std;
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
    const char *const argv[] = {"make.exe","-C","src",
				"-f","Makefile.mfront",
				"clean",nullptr};
    if(_spawnvp(_P_WAIT,"make.exe",argv)!=0){
      string msg("MFront::cleanLibraries : ");
      msg += "can't clean libraries";
      throw(runtime_error(msg));
    }
#else
    const char *const argv[] = {"make","-f",
				"Makefile.mfront",
				"clean",nullptr};
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
#endif
  } // end of MFront::cleanLibraries
#endif /* MFRONT_MAKE_SUPPORT */

#if (defined _WIN32 || defined _WIN64 ||defined __CYGWIN__)
  void MFront::generateDefsFiles(void){
    auto& mlock = MFrontLock::getMFrontLock();
    mlock.lock();
    try{
      for(const auto& d:this->defs){
	const auto p = this->epts.find(d);
	if(p==this->epts.end()){
	  throw(std::runtime_error("MFront::generateDefsFile : "
				   "no entry points for library '"+d+"'"));
	}
	const auto f = std::string{"src"+tfel::system::dirStringSeparator()+
				   d+".def"};
	std::ofstream def{f};
	def.exceptions(std::ios::badbit|std::ios::failbit);
	if(!def){
	  throw(std::runtime_error("MFront::generateDefsFile : "
				   "can't open file '"+f+"'"));
	}
	def << "LIBRARY " << d << "\n"
	    << "EXPORTS\n";
	std::copy(p->second.begin(),p->second.end(),
		  std::ostream_iterator<std::string>(def,"\n"));
      }
    }
    catch(...){
      mlock.unlock();
      throw;
    }
    mlock.unlock();
  } // end of MFront::generateDefsFile
#endif /* (defined _WIN32 || defined _WIN64 ||defined __CYGWIN__) */
  
  void
  MFront::exe(void)
  {
    using namespace std;
    using namespace tfel::system;
    systemCall::mkdir("src");
    systemCall::mkdir("include");
    if(this->specifiedTargets.empty()){
      this->specifiedTargets.insert("all");
    }
    bool w = false; // something to be done 
    w = w || (!this->inputs.empty());
#ifdef MFRONT_MAKE_SUPPORT
    w = w || this->genMake;
#endif /* MFRONT_MAKE_SUPPORT */
#if (defined _WIN32 || defined _WIN64 ||defined __CYGWIN__)
    w = w || (!this->defs.empty());
#endif /* (defined _WIN32 || defined _WIN64 ||defined __CYGWIN__) */
    if(!w){
      throw(runtime_error("MFront::exe : no file name specified and nothing to be done\n"+
			  this->getUsageDescription()));
    }
    // get file generated by previous sessions
    this->analyseSourceDirectory();
    auto errors = vector<pair<string,string>>{};
    if(!this->inputs.empty()){
      for(const auto& i : this->inputs){
	try{
	  const auto td = this->treatFile(i);
	  mergeTargetsDescription(this->targets,td);
#pragma message("HERE : cppflags must be wiped out !")
	  for(const auto& l : td){
	    this->cppflags.insert(l.cppflags.begin(),l.cppflags.end());
	  }
	} catch(exception& e){
	  errors.push_back({i,e.what()});
	}
      }
      for(auto& t : this->targets.specific_targets){
	auto tmp = vector<string>{};
	for(auto p2=t.second.first.cbegin();p2!=t.second.first.cend();++p2){
	  const auto p4 = p2+1;
	  if(find(p4,t.second.first.cend(),*p2)==t.second.first.cend()){
	    tmp.push_back(*p2);
	  }
	}
	t.second.first.swap(tmp);
	tmp.clear();
	for(auto p2=t.second.second.cbegin();
	    p2!=t.second.second.cend();++p2){
	  const auto p4 = p2+1;
	  if(find(p4,t.second.second.cend(),*p2)==t.second.second.cend()){
	    tmp.push_back(*p2);
	  }
	}
	t.second.second.swap(tmp);
      }
      // get file generated by previous sessions
      this->analyseSourceDirectory();
      // save generated files
      this->writeSourcesLists();
      this->writeEntryPointsLists();
      this->writeDependenciesLists();
      this->writeSpecificTargets();
      this->writeCppFlags();
    }
    if(!errors.empty()){
      auto msg = string{};
      auto p6=errors.begin();
      while(p6!=errors.end()){
	msg += "Error while treating file '"+p6->first+"'\n";
	msg += p6->second;
	if(++p6!=errors.end()){
	  msg += "\n\n";
	}
      }
      throw(runtime_error(msg));
    }
#if (defined _WIN32 || defined _WIN64 ||defined __CYGWIN__)
    this->generateDefsFiles();
#endif /* (defined _WIN32 || defined _WIN64 ||defined __CYGWIN__) */
#ifdef MFRONT_MAKE_SUPPORT
    const auto has_libs = this->targets.begin()!=this->targets.end();
    if((this->genMake)&&((has_libs)||(!this->targets.specific_targets.empty()))){
      this->generateMakeFile();
    }
    if(this->cleanLibs){
      this->cleanLibraries();
    }
    if((this->buildLibs)&&((has_libs)||(!this->targets.specific_targets.empty()))){
      for(const auto t : this->specifiedTargets){
	cout << "Treating target : " << t << endl;
	this->buildLibraries(t);
      }
      if(has_libs){
	if(this->targets.end()-this->targets.begin()==1){
	  cout << "The following library has been built :\n";
	} else {
	  cout << "The following libraries have been built :\n";
	}
	for(const auto& l:this->targets){
	  cout << "- " << l.name << "." << l.suffix << " : ";
	  for(const auto& pts:l.epts){
	    cout << " " << pts;
	  }
	  cout << endl;
	}
      }
      if(!this->targets.specific_targets.empty()){
	auto pt2=this->targets.specific_targets.find("all");
	if(pt2!=this->targets.specific_targets.end()){	      
	  if(pt2->second.first.size()==1){
	    cout << "The following main target has been build :\n";
	  } else {
	    cout << "The following main targets have been build :\n";
	  }
	  for(const auto& t : pt2->second.first){
	    cout << "- " << t << endl;
	  }
	}
      }
    }
#endif /* MFRONT_MAKE_SUPPORT */
  } // end of MFront::exe

  MFront::~MFront()
  {} // end of MFront::~MFront

} // end of namespace mfront
