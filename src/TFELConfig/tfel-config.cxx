/*!
 * \file   tfel-config.cxx
 * \brief  
 * \author Helfer Thomas
 * \date   27 aoû 2007
 */

#include<cstdlib>

#include"tfel-config.hxx"

static CallBacksContainer callBacksContainer;
static bool oflags          = false;
static bool warning         = false;
static bool incs            = false;
static bool libs            = false;
static bool exceptions      = false;
static bool math            = false;
static bool mathCubicSpline = false;
static bool mathKriging     = false;
static bool mathParser      = false;
static bool mathInterpreter = false;
static bool utilities       = false;
static bool finiteElement   = false;
static bool material        = false;
static bool tests           = false;
#ifdef HAVE_CASTEM
static bool castem          = false;
#endif /* HAVE_CASTEM */
static bool lsystem         = false;
#ifdef USE_GRAPHICS
static bool graphics        = false;
#endif /* USE_GRAPHICS */

const std::string
libDir(void)
{
  using namespace std;
  static const string prefix(PREFIXDIR);
  static const string execPrefix(EXECPREFIXDIR);
  string lib(LIBDIR);

  const char * const path = getenv("TFELHOME");
  if(path!=0){
    return string(path)+"/lib";
  }
  
  if(lib.substr(0,14)=="${exec_prefix}"){
    if(execPrefix=="${prefix}"){
      lib = prefix + "/lib";
    } else {
      lib = execPrefix + "/lib";
    }
  }
  return lib;
} // end of libDir

const std::string
includeDir(void)
{
  using namespace std;
  static const string prefix(PREFIXDIR);
  string inc(INCLUDEDIR);
  const char * const path = getenv("TFELHOME");
  if(path!=0){
    inc =  string(path)+"/include";
  } else {
    if(inc.substr(0,9)=="${prefix}"){
      inc = prefix + "/include";
    }
  }
#ifdef COMPILER_SPECIFIC_OPTIONS
  inc += ' ';
  inc += COMPILER_SPECIFIC_OPTIONS;
#endif
  return inc;
} // end of libDir

static void
registerCallBack(const std::string& key,
		 const FuncPtr& f,
		 const std::string& description)
{
  using namespace std;
  callBacksContainer.insert(make_pair(key,make_pair(f,description)));
} // end of registerNewCallBack

static void
treatOFlags(void)
{
  oflags = true;
} // end of treatOFlags

static void
treatWarning(void)
{
  warning = true;
} // end of treatWarning

#ifdef HAVE_CASTEM
static void
treatCastem(void)
{
  castem = true;
} // end of treatCastem
#endif /* HAVE_CASTEM */

static void
treatExceptions(void)
{
  exceptions = true;
} // end of treatExceptions

static void
treatMath(void)
{
  exceptions = true;
  math       = true;
} // end of treatMath

static void
treatMathKriging(void)
{
  exceptions = true;
  math       = true;
  mathKriging = true;
} // end of treatMathKriging

static void
treatMathCubicSpline(void)
{
  exceptions = true;
  math       = true;
  mathCubicSpline = true;
} // end of treatMathCubicSpline

static void
treatMathParser(void)
{
  exceptions  = true;
  math        = true;
  mathKriging = true;
  mathParser  = true;
} // end of treatMathParser

static void
treatMathInterpreter(void)
{
  exceptions      = true;
  math            = true;
  mathKriging     = true;
  mathParser      = true;
  mathInterpreter = true;
} // end of treatMathInterpreter

static void
treatUtilities(void)
{
  utilities = true;
} // end of treatException

static void
treatFiniteElement(void)
{
  exceptions    = true;
  math          = true;
  finiteElement = true;
} // end of treatFiniteElement

static void
treatSystem(void)
{
  exceptions    = true;
  lsystem        = true;
} // end of treatFiniteElement

static void
treatMaterial(void)
{
  exceptions  = true;
  math        = true;
  material    = true;
} // end of treatMaterial

#ifdef USE_GRAPHICS
static void
treatGraphics(void)
{
  exceptions  = true;
  math        = true;
  utilities   = true;
  lsystem     = true;
} // end of treatGraphics
#endif /* USE_GRAPHICS */

static void
treatTests(void)
{
  tests  = true;
} // end of treatTests

static void
treatAll(void)
{
  exceptions  = true;
  math        = true;
  material    = true;
  utilities   = true;
  lsystem      = true;
  tests       = true;
#ifdef USE_GRAPHICS
  graphics    = true;
#endif /* USE_GRAPHICS */
} // end of treatAll

static void
treatIncludes(void)
{
  incs = true;
} // end of treatIncludes

static void
treatLibs(void)
{
  libs = true;
} // end of treatLibs

static void
listOptions(std::ostream& os)
{
  using namespace std;
  CallBacksContainer::const_iterator p;
  os << "Valid options are : " << endl;
  for(p  = callBacksContainer.begin();
      p != callBacksContainer.end(); ++p){ 
    string options(p->first);
    if(options.size()<=18){
      options.insert(options.size(),18-options.size(),' ');
    }
    os << options << " : " <<  p->second.second << endl;
  }
} // end of listOptions

static void
treatHelp(void)
{
  using namespace std;
  cout << "usage : tfel-config [options]" << endl;
  listOptions(cout);
  exit(EXIT_SUCCESS);
} // end of treatHelp

static void
treatUnknownOption(const std::string& o)
{
  using namespace std;
  cerr << "unknown option " << o << endl;
  listOptions(cerr);
  exit(EXIT_FAILURE);
} // end of treatUnknownOption

int
main(const int argc,
     const char *const *const argv)
{
  using namespace std;
  CallBacksContainer::const_iterator p;
  const char * const * p2;

  registerCallBack("--oflags",&treatOFlags,"return tfel recommended optimisation flags.");
  registerCallBack("--warning",&treatWarning,"return tfel recommended warnings.");
  registerCallBack("--includes",&treatIncludes,"return preprocessor flags.");
  registerCallBack("--libs",&treatLibs,"return linking flags.");
  registerCallBack("--help",&treatHelp,"print this help message.");
#ifdef HAVE_CASTEM
  registerCallBack("--castem",&treatCastem,"request flags for castem.");
#endif /* HAVE_CASTEM */
  registerCallBack("--exceptions",&treatExceptions,"request flags for libTFELException.");
  registerCallBack("--math-kriging",&treatMathKriging,"request flags for libTFELMathKriging.");
  registerCallBack("--math-cubic-spline",&treatMathCubicSpline,"request flags for libTFELMathCubicSpline.");
  registerCallBack("--math",&treatMath,"request flags for libTFELMath.");
  registerCallBack("--math-parser",&treatMathParser,"request flags for libTFELMathParser.");
  registerCallBack("--math-interpreter",&treatMathInterpreter,"request flags for libTFELMathInterpreter.");
  registerCallBack("--tests",&treatTests,"request flags for libTFELTests.");
  registerCallBack("--system",&treatSystem,"request flags for libTFELSystem.");
  registerCallBack("--utilities",&treatUtilities,"request flags for libTFELUtilities.");
  registerCallBack("--material",&treatMaterial,"request flags for libTFELMaterial.");
  registerCallBack("--finiteElement",&treatFiniteElement,"request flags for libTFELFiniteElement.");
  registerCallBack("--all",&treatAll,"request flags for all librairies.");
#ifdef USE_GRAPHICS
  registerCallBack("--graphics",&treatGraphics,"request flags for libTFELGraphics.");
#endif /* USE_GRAPHICS */
  for(p2=argv+1;p2!=argv+argc;++p2){
    p = callBacksContainer.find(*p2);
    if(p==callBacksContainer.end()){
      treatUnknownOption(*p2);
    }
    (*(p->second.first))();
  }

#ifdef HAVE_CASTEM
  if(castem){
    cout << CASTEMFLAGS << " " << endl;
  }
#endif /* HAVE_CASTEM */

  if(incs){
    cout << "-I" << includeDir() << " ";
  }

  if(libs){
    cout << "-L" << libDir() << " ";
#ifdef USE_GRAPHICS
    if(graphics){
      cout << "-lTFELGraphics ";
    }
#endif /* USE_GRAPHICS */
    if(material){
      cout << "-lTFELMaterial ";
    }
    if(finiteElement){
      cout << "-lTFELFiniteElement ";
    }
    if(mathInterpreter){
      cout << "-lTFELMathInterpreter ";
    }
    if(mathParser){
      cout << "-lTFELMathParser ";
    }
    if(mathCubicSpline){
      cout << "-lTFELMathCubicSpline ";
    }
    if(mathKriging){
      cout << "-lTFELMathKriging ";
    }
    if(math){
      cout << "-lTFELMath ";
    }
    if(lsystem){
      cout << "-lTFELSystem ";
    }
    if(utilities){
      cout << "-lTFELUtilities ";
    }
    if(exceptions){
      cout << "-lTFELException ";
    }
    if(tests){
      cout << "-lTFELTests ";
    }
  }

  if(oflags){
    cout << OPTIMISATION_FLAGS << " ";
  }

  if(warning){
    cout << COMPILER_WARNINGS << " ";
  }

  if(incs||libs||oflags){
    cout << endl;
  }

  return EXIT_SUCCESS;
}
