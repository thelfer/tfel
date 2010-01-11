/*!
 * \file   tfel-config.cxx
 * \brief  
 * \author Helfer Thomas
 * \date   27 aoû 2007
 */

#include<cstdlib>

#include"tfel-config.hxx"

static CallBacksContainer callBacksContainer;
static bool oflags        = false;
static bool incs          = false;
static bool libs          = false;
static bool exceptions    = false;
static bool math          = false;
static bool utilities     = false;
static bool finiteElement = false;
static bool material      = false;
static bool lsystem       = false;
static bool graphics      = false;

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
#ifdef CASTEM
  inc += ' ';
  inc += CASTEMFLAGS;
#endif
  return inc;
} // end of libDir

void
registerCallBack(const std::string& key,
		 const FuncPtr& f,
		 const std::string& description)
{
  using namespace std;
  callBacksContainer.insert(make_pair(key,make_pair(f,description)));
} // end of registerNewCallBack

void
treatOFlags(void)
{
  oflags = true;
} // end of treatExceptions


void
treatExceptions(void)
{
  exceptions = true;
} // end of treatExceptions

void
treatMath(void)
{
  exceptions = true;
  math      = true;
} // end of treatMath

void
treatUtilities(void)
{
  utilities = true;
} // end of treatException

void
treatFiniteElement(void)
{
  exceptions    = true;
  math          = true;
  finiteElement = true;
} // end of treatFiniteElement

void
treatLsystem(void)
{
  exceptions    = true;
  lsystem        = true;
} // end of treatFiniteElement

void
treatMaterial(void)
{
  exceptions  = true;
  math        = true;
  material = true;
} // end of treatMaterial

void
treatGraphics(void)
{
  exceptions  = true;
  math        = true;
  utilities   = true;
  lsystem      = true;
} // end of treatMaterial

void
treatAll(void)
{
  exceptions  = true;
  math        = true;
  material = true;
  utilities   = true;
} // end of treatAll

void
treatIncludes(void)
{
  incs = true;
} // end of treatIncludes

void
treatLibs(void)
{
  libs = true;
} // end of treatLibs

void
listOptions(std::ostream& os)
{
  using namespace std;
  CallBacksContainer::const_iterator p;
  os << "Valid options are : " << endl;
  for(p  = callBacksContainer.begin();
      p != callBacksContainer.end(); ++p){ 
    string options(p->first);
    if(options.size()<=16){
      options.insert(options.size(),16-options.size(),' ');
    }
    os << options << " : " <<  p->second.second << endl;
  }
} // end of listOptions

void
treatHelp(void)
{
  using namespace std;
  cout << "usage : tfel-config [options]" << endl;
  listOptions(cout);
  exit(EXIT_SUCCESS);
} // end of treatHelp

void
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
  registerCallBack("--includes",&treatIncludes,"return preprocessor flags.");
  registerCallBack("--libs",&treatLibs,"return linking flags.");
  registerCallBack("--help",&treatHelp,"print this help message.");
  registerCallBack("--exceptions",&treatExceptions,"request flags for libTFELException.");
  registerCallBack("--math",&treatMath,"request flags for libTFELMath.");
  registerCallBack("--graphics",&treatGraphics,"request flags for libTFELGraphics.");
  registerCallBack("--system",&treatLsystem,"request flags for libTFELSystem.");
  registerCallBack("--utilities",&treatUtilities,"request flags for libTFELUtilities.");
  registerCallBack("--material",&treatMaterial,"request flags for libTFELMaterial.");
  registerCallBack("--finiteElement",&treatFiniteElement,"request flags for libTFELFiniteElement.");
  registerCallBack("--all",&treatAll,"request flags for all librairies.");

  for(p2=argv+1;p2!=argv+argc;++p2){
    p = callBacksContainer.find(*p2);
    if(p==callBacksContainer.end()){
      treatUnknownOption(*p2);
    }
    (*(p->second.first))();
  }

  if(incs){
    cout << "-I" << includeDir() << " ";
  }

  if(libs){
    cout << "-L" << libDir() << " ";
    if(exceptions){
      cout << "-lTFELException ";
    }
    if(utilities){
      cout << "-lTFELUtilities ";
    }
    if(math){
      cout << "-lTFELMath ";
    }
    if(lsystem){
      cout << "-lTFELSystem ";
    }
    if(graphics){
      cout << "-lTFELGraphics ";
    }
    if(material){
      cout << "-lTFELMaterial ";
    }
    if(finiteElement){
      cout << "-lTFELFiniteElement ";
    }
  }

  if(oflags){
    cout << OPTIMISATION_FLAGS << " ";
  }

  if(incs||libs||oflags){
    cout << endl;
  }

  return EXIT_SUCCESS;
}
