/*!
 * \file   src/TFELConfig/tfel-config.cxx
 * \brief  
 * \author Helfer Thomas
 * \date   27 août 2007
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<map>
#include<string>
#include<cstdlib>
#include<iostream>
#include<algorithm>
#include<stdexcept>

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Config/GetInstallPath-defines.hxx"
#include"tfel-config.hxx"

#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include<windows.h>
#endif

using FuncPtr = void (*)();
using CallBacksContainer = std::map<std::string,std::pair<FuncPtr,std::string>>;

static std::string handleSpace(const std::string& p)
{
  if(find(p.begin(),p.end(),' ')!=p.end()){
#if defined _WIN32 || defined _WIN64
    throw(std::runtime_error("tfel-config handleSpace: "
			     "path to TFEL shall not contain space as "
			     "MinGW can't handle it (Found '"+p+"'). "
			     "Please change TFEL installation directory"));
#else
    return '"'+p+'"';
#endif
  }
  return p;
}

static std::string getTFELHOME();

static std::string libDir();

static std::string includeDir();

static void registerCallBack(const std::string&,
			     const FuncPtr&,
			     const std::string&);

#ifdef HAVE_CASTEM
static void treatCastem();
#endif /* HAVE_CASTEM */

#ifdef HAVE_ZMAT
static void treatZMAT();
#endif /* HAVE_ZMAT */

static void treatExceptions();

static void treatMath();

static void treatMathCubicSpline();

static void treatMathKriging();

static void treatMathParser();

static void treatMathInterpreter();

static void treatUtilities();

static void treatGlossary();

static void treatAll();

static void listOptions(std::ostream&);

TFEL_NORETURN static void
treatUnknownOption(const std::string&);

TFEL_NORETURN static void
treatHelp();

TFEL_NORETURN static void
treatLicences();

static CallBacksContainer callBacksContainer;
static bool compilerflags   = false;
static bool oflags0         = false;
static bool oflags          = false;
static bool oflags2         = false;
static bool warning         = false;
static bool incs            = false;
static bool incspath        = false;
static bool cppflags        = false;
static bool libspath        = false;
static bool ldflags         = false;
static bool exceptions      = false;
static bool math            = false;
static bool mathCubicSpline = false;
static bool mathKriging     = false;
static bool mathParser      = false;
static bool mathInterpreter = false;
static bool utilities       = false;
static bool glossary        = false;
static bool numodis         = false;
static bool material        = false;
static bool tests           = false;
static bool mfront_profiling    = false;
#ifdef HAVE_CASTEM
static bool castem          = false;
#endif /* HAVE_CASTEM */
#ifdef HAVE_ZMAT
static bool zmat          = false;
#endif /* HAVE_ZMAT */
static bool lsystem         = false;

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

static std::string getTFELHOME()
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
#else
  return "";
#endif
}

static std::string libDir()
{
  using namespace std;
  const string prefix(PREFIXDIR);
  const string execPrefix(EXECPREFIXDIR);
  string lib(LIBDIR);
#if (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__)
  const string ldir("/bin");
#else 
#ifdef LIB_SUFFIX
  const string ldir("/lib"LIB_SUFFIX);
#else
  const string ldir("/lib");
#endif
#endif
  const auto& th = getTFELHOME();
  if(!th.empty()){
    return th+ldir;
  }
  if(lib.substr(0,14)=="${exec_prefix}"){
    if(execPrefix=="${prefix}"){
      lib = prefix+ldir;
    } else {
      lib = execPrefix+ldir;
    }
  }
  return handleSpace(lib);
} // end of libDir

static std::string includeDir()
{
  const std::string prefix(PREFIXDIR);
  std::string inc(INCLUDEDIR);
  const auto& th = getTFELHOME();
  if(!th.empty()){
    return th+"/include";
  } else {
    if(inc.substr(0,9)=="${prefix}"){
      inc = handleSpace(prefix + "/include");
    }
  }
  return inc;
} // end of libDir

static void registerCallBack(const std::string& key,
			     const FuncPtr& f,
			     const std::string& description)
{
  callBacksContainer.insert({key,{f,description}});
} // end of registerNewCallBack

#ifdef HAVE_CASTEM
static void treatCastem()
{
  castem = true;
} // end of treatCastem
#endif /* HAVE_CASTEM */

#ifdef HAVE_ZMAT
static void treatZMAT()
{
  zmat = true;
} // end of treatZMAT
#endif /* HAVE_ZMAT */

static void treatExceptions()
{
  exceptions = true;
} // end of treatExceptions

static void treatMath()
{
  exceptions = true;
  math       = true;
} // end of treatMath

static void treatMathKriging()
{
  exceptions = true;
  math       = true;
  mathKriging = true;
} // end of treatMathKriging

static void treatMathCubicSpline()
{
  exceptions = true;
  math       = true;
  mathCubicSpline = true;
} // end of treatMathCubicSpline

static void treatMathParser()
{
  exceptions  = true;
  math        = true;
  mathKriging = true;
  mathParser  = true;
} // end of treatMathParser

static void treatMathInterpreter()
{
  exceptions      = true;
  math            = true;
  mathKriging     = true;
  mathParser      = true;
  mathInterpreter = true;
} // end of treatMathInterpreter

static void treatUtilities()
{
  utilities = true;
} // end of treatUtilities

static void treatGlossary()
{
  glossary = true;
} // end of treatGlossary

static void treatSystem()
{
  exceptions    = true;
  lsystem        = true;
} // end of treatSystem

static void treatNumodis()
{
  numodis  = true;
} // end of treatNumodis

static void treatMaterial()
{
  numodis     = true;
  exceptions  = true;
  math        = true;
  utilities   = true;
  material    = true;
} // end of treatMaterial

static void treatMFrontProfiling()
{
  mfront_profiling  = true;
} // end of treatMFrontProfiling

static void treatTests()
{
  tests  = true;
} // end of treatTests

static void treatAll()
{
  exceptions   = true;
  math         = true;
  numodis      = true;
  material     = true;
  utilities    = true;
  glossary     = true;
  lsystem      = true;
  tests        = true;
  mfront_profiling = true;
} // end of treatAll

static void listOptions(std::ostream& os)
{
  os << "Valid options are:\n";
  for(const auto& c : callBacksContainer){ 
    auto opt = c.first;
    if(opt.size()<=18){
      opt.insert(opt.size(),18-opt.size(),' ');
    }
    os << opt << ": " <<  c.second.second << '\n';
  }
} // end of listOptions

TFEL_NORETURN static void treatVersion()
{
  using namespace std;
  cout << "tfel-config " << VERSION 
       << " (svn revision : " << TFEL_SVN_REVISION 
       << ")" << endl;
  exit(EXIT_SUCCESS);
} // end of treatHelp

TFEL_NORETURN static void treatHelp()
{
  using namespace std;
  cout << "usage : tfel-config [options]" << endl;
  listOptions(cout);
  exit(EXIT_SUCCESS);
} // end of treatHelp

TFEL_NORETURN static void treatUnknownOption(const std::string& o)
{
  using namespace std;
  cerr << "unknown option " << o << endl;
  listOptions(cerr);
  exit(EXIT_FAILURE);
} // end of treatUnknownOption

TFEL_NORETURN static void treatLicences()
{
  using namespace std;
  cout << "Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights reserved." << endl;
  cout << "This project is publicly released under either the GNU GPL Licence or the" << endl;
  cout << "CECILL-A licence. A copy of thoses licences are delivered with the sources of" << endl;
  cout << "TFEL. CEA or EDF may also distribute this project under specific licensing" << endl;
  cout << "conditions." << endl;
  cout << endl;
  cout << "For further information, please read the licences files or visit the following" << endl;
  cout << "websites:" << endl;
  cout << "    http://www.gnu.org/licenses" << endl;
  cout << "    http://www.cecill.info/licences.en.html" << endl;
  cout << "    http://www.cecill.info/licences.fr.html" << endl;
  exit(EXIT_SUCCESS);
} // end of treatLicences

static void listLibraries(const char* p)
{
  if(mfront_profiling){
    std::cout << p << "MFrontProfiling ";
  }
  if(material){
    std::cout << p << "TFELMaterial ";
  }
  if(mathInterpreter){
    std::cout << p << "TFELMathInterpreter ";
  }
  if(mathParser){
    std::cout << p << "TFELMathParser ";
  }
  if(mathCubicSpline){
    std::cout << p << "TFELMathCubicSpline ";
  }
  if(mathKriging){
    std::cout << p << "TFELMathKriging ";
  }
  if(math){
    std::cout << p << "TFELMath ";
  }
  if(lsystem){
    std::cout << p << "TFELSystem ";
  }
  if(utilities){
    std::cout << p << "TFELUtilities ";
  }
  if(glossary){
    std::cout << p << "TFELGlossary ";
  }
  if(exceptions){
    std::cout << p << "TFELException ";
  }
  if(numodis){
    std::cout << p << "TFELNUMODIS ";
  }
  if(tests){
    std::cout << p << "TFELTests ";
  }
}

/* coverity [UNCAUGHT_EXCEPT]*/
int main(const int argc,
	 const char *const *const argv)
{
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
  try{
#endif /* __CYGWIN__ */

  registerCallBack("--compiler-flags",[](){compilerflags=true;},
		   "return tfel recommended compiler flags.");
  registerCallBack("--oflags0",[](){oflags0=true;},
		   "return tfel recommended optimisation "
		   "flags without architecture specific flags.");
  registerCallBack("--oflags",[](){oflags0=oflags=true;},
		   "return tfel recommended optimisation "
		   "flags with architecture specific flags.");
  registerCallBack("--oflags2",[](){oflags2=true;},
		   "return some aggressive optimisation flags, "
		   "possibly at the expense of numerical precision. "
		   "This shall be added to `--oflags` results.");
  registerCallBack("--warning",[](){warning=true;},
		   "return tfel recommended warnings.");
  registerCallBack("--includes",[](){incs=true;},
		   "return tfel include path.");
  registerCallBack("--cppflags",[](){cppflags=true;},
		   "return preprocessor flags.");
  registerCallBack("--ldflags",[](){ldflags=true;},
		   "return linking flags.");
  registerCallBack("--libs",[](){ldflags=true;},
		   "return linking flags.");
  registerCallBack("--include-path",[](){incspath=true;},
		   "return the path to the tfel headers.");
  registerCallBack("--library-path",[](){libspath=true;},
		   "return  to the library.");
  registerCallBack("--help",&treatHelp
,"print this help message.");
#ifdef HAVE_CASTEM
  registerCallBack("--castem",&treatCastem
,"request flags for castem.");
#endif /* HAVE_CASTEM */
#ifdef HAVE_ZMAT
  registerCallBack("--zmat",&treatZMAT,"request flags for zmat.");
#endif /* HAVE_ZMAT */
  registerCallBack("--exceptions",&treatExceptions,
		   "request flags for libTFELException.");
  registerCallBack("--math-kriging",&treatMathKriging,
		   "request flags for libTFELMathKriging.");
  registerCallBack("--math-cubic-spline",&treatMathCubicSpline,
		   "request flags for libTFELMathCubicSpline.");
  registerCallBack("--math",&treatMath,
		   "request flags for libTFELMath.");
  registerCallBack("--math-parser",&treatMathParser,
		   "request flags for libTFELMathParser.");
  registerCallBack("--math-interpreter",&treatMathInterpreter,
		   "request flags for libTFELMathInterpreter.");
  registerCallBack("--tests",&treatTests,
		   "request flags for libTFELTests.");
  registerCallBack("--system",&treatSystem,
		   "request flags for libTFELSystem.");
  registerCallBack("--utilities",&treatUtilities,
		   "request flags for libTFELUtilities.");
  registerCallBack("--glossary",&treatGlossary,
		   "request flags for libTFELGlossary.");
  registerCallBack("--material",&treatMaterial,
		   "request flags for libTFELMaterial.");
  registerCallBack("--numodis",&treatNumodis,
		   "request flags for libTFELNUMODIS.");
  registerCallBack("--mfront-profiling",&treatMFrontProfiling,
		   "request flags for libMFrontProfiling.");
  registerCallBack("--all",&treatAll,
		   "request flags for all librairies.");
  registerCallBack("--version",&treatVersion,
		   "print tfel version and svn revision.");
  registerCallBack("--licence",&treatLicences,
		   "print tfel licences.");

  if(argc==1){
    treatHelp();
  }

  for(auto p2=argv+1;p2!=argv+argc;++p2){
    const auto p = callBacksContainer.find(*p2);
    if(p==callBacksContainer.end()){
      treatUnknownOption(*p2);
    }
    (*(p->second.first))();
  }

  if(cppflags){
#ifdef TFEL_ARCH32
    std::cout << "-DTFEL_ARCH32" << " ";
#endif
#ifdef TFEL_ARCH64
    std::cout << "-DTFEL_ARCH64" << " ";
#endif
  }

  if(incs){
    std::cout << "-I" << includeDir() << " ";
  }
  if(incspath){
    std::cout << includeDir() << " ";
  }
  
#ifdef HAVE_CASTEM
#ifdef LOCAL_CASTEM_HEADER
  if(castem){
    if(!incs){
      std::cout << "-I" << includeDir() << " ";
      std::cout << CASTEMFLAGS1 << " ";
    }
  }
#else  /* LOCAL_CASTEM_HEADER */
  if(castem){
    if(!incs){
      std::cout << CASTEMFLAGS1 << " ";
    }
    const char * const castempath = getenv("CASTEM_ROOT");
    if(castempath!=nullptr){
      std::cout << "-I" << castempath << "/include ";
    } else {
      std::cout << CASTEMFLAGS2 << " ";
    }
  }
#endif /* LOCAL_CASTEM_HEADER */
#endif /* HAVE_CASTEM */

#ifdef HAVE_ZMAT
  if(zmat){
    if(!incs){
      std::cout << ZMATFLAGS1 << " ";
    }
    const char * const zmatpath = getenv("ZSET_ROOT");
    if(zmatpath!=nullptr){
      std::cout << "-I" << zmatpath << "/include ";
    } else {
#ifndef ZSET_ROOT
      throw(std::runtime_error("tfel-config : no installation path "
			       "to the ZSET softwares.\n"
			       "Please define the 'ZSET_ROOT' "
			       "variable environnement"));
#else
      std::cout << ZMATFLAGS2 << " ";
#endif
    }
  }
#endif /* HAVE_ZMAT */

  if(libspath){
    std::cout << libDir() << " ";
  }
  
  if(ldflags){
    std::cout << "-L" << libDir() << " ";
    listLibraries("-l");
  }

  if(compilerflags){
    std::cout << COMPILER_FLAGS << " " << COMPILER_CXXFLAGS << " ";
  }

  if(oflags0){
    std::cout << OPTIMISATION_FLAGS0 << " ";
  }

  if(oflags){
    std::cout << OPTIMISATION_FLAGS << " ";
  }

  if(oflags2){
    std::cout << OPTIMISATION_FLAGS2 << " ";
  }

  if(warning){
    std::cout << COMPILER_WARNINGS << " ";
  }

  std::cout << std::endl;

#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
  }
  catch(exception& e){
    MessageBox(nullptr,e.what(),
	       "mfront",0);
    return EXIT_FAILURE;
  }
  catch(...){
    return EXIT_FAILURE;
  }
#endif /* __CYGWIN__ */

  return EXIT_SUCCESS;
}
