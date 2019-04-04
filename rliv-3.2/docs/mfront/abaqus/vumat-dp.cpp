/*!
 * \file   vumat-dp.cpp
 * \brief    
 * \author Thomas Helfer
 * \date   21/03/2016
 */

#include<iostream>
#include<cstdlib>

#include<map>
#include<string>
#include<cctype>
#include<vector>
#include<utility>
#include<cstring>
#include<stdexcept>
#include<algorithm>

#if __cplusplus >= 201103L
#include<mutex>
#define HAVE_STD_MUTEX
#define NULLPTR(X) nullptr
#else  /* __cplusplus >= 201103L */
#define NULLPTR(X) static_cast<X>(0)
#endif /* __cplusplus >= 201103L */

#if (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include<windows.h>
#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif
typedef HINSTANCE__*  libptr;

#ifndef HAVE_STD_MUTEX

struct Mutex
{
  static Mutex& getMutex(){
    static Mutex m;
    return m;
  }
  HANDLE m;
private:
  Mutex(){
    this->m = CreateMutex(NULL,  // default security attributes
			FALSE, // initially not owned
			NULL); // unnamed mutex
  }
};

struct lock
{
  lock(){
    DWORD r = WaitForSingleObject(Mutex::getMutex().m,
				  INFINITE);
    if(r==WAIT_ABANDONED){
      std::cerr << "vumat: abandonned mutex" << std::endl;
      std::exit(EXIT_FAILURE);
    }
  }
  ~lock(){
    CloseHandle(Mutex::getMutex().m);
  }
};
#endif /* HAVE_STD_MUTEX */

// code retrieved from
// http://www.codeproject.com/Tips/479880/GetLastError-as-std-string
static std::string getLastWin32Error()
{
  const DWORD error = GetLastError();
  if (error){
    LPVOID lpMsgBuf;
    DWORD bufLen = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | 
				 FORMAT_MESSAGE_FROM_SYSTEM |
				 FORMAT_MESSAGE_IGNORE_INSERTS,
				 NULL,error,
				 MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				 (LPTSTR) &lpMsgBuf,0,NULL );
    if (bufLen){
      LPCSTR lpMsgStr = (LPTSTR) lpMsgBuf;
      std::string result(lpMsgStr, lpMsgStr+bufLen);
      LocalFree(lpMsgBuf);
      return result;
    }
  }
  return std::string();
}

#else
#include<dlfcn.h>
typedef void * libptr;
#ifndef HAVE_STD_MUTEX
#error "Unsupported platform"
#endif /* HAVE_STD_MUTEX */
#endif

#if (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__)
#define VUMATFCT  vumat
#define VUMATFCT2 vumat2
#else
#define VUMATFCT  vumat_
#define VUMATFCT2 vumat2_
#endif

#if (defined _WIN32) && (!defined _WIN64)
typedef int    fortran_string_size;
#elif defined _WIN64
typedef size_t fortran_string_size;
#else
typedef int    fortran_string_size;
#endif

typedef double abaqus_real;
typedef int    abaqus_int;

typedef  void (*vumatptr)(const abaqus_int *const,
			  const abaqus_int *const,
			  const abaqus_int *const,
			  const abaqus_int *const,
			  const abaqus_int *const,
			  const abaqus_int *const,
			  const abaqus_int *const,
			  const abaqus_real* const,
			  const abaqus_real* const,
			  const abaqus_real* const,
			  const char* const,
			  const abaqus_real* const,
			  const abaqus_real* const,
			  const abaqus_real* const,
			  const abaqus_real* const,
			  const abaqus_real* const,
			  const abaqus_real* const,
			  const abaqus_real* const,
			  const abaqus_real* const,
			  const abaqus_real* const,
			  const abaqus_real* const,
			  const abaqus_real* const,
			  const abaqus_real* const,
			  const abaqus_real* const,
			  const abaqus_real* const,
			  const abaqus_real* const,
			  const abaqus_real* const,
			  const abaqus_real* const,
			  const abaqus_real* const,
			  const abaqus_real*,
			  const abaqus_real*,
			  const abaqus_real*,
			  const abaqus_real*,
			  const fortran_string_size);

extern "C" {
  void VUMATFCT(const abaqus_int *const,
	       const abaqus_int *const,
	       const abaqus_int *const,
	       const abaqus_int *const,
	       const abaqus_int *const,
	       const abaqus_int *const,
	       const abaqus_int *const,
	       const abaqus_real* const,
	       const abaqus_real* const,
	       const abaqus_real* const,
	       const char* const,
	       const abaqus_real* const,
	       const abaqus_real* const,
	       const abaqus_real* const,
	       const abaqus_real* const,
	       const abaqus_real* const,
	       const abaqus_real* const,
	       const abaqus_real* const,
	       const abaqus_real* const,
	       const abaqus_real* const,
	       const abaqus_real* const,
	       const abaqus_real* const,
	       const abaqus_real* const,
	       const abaqus_real* const,
	       const abaqus_real* const,
	       const abaqus_real* const,
	       const abaqus_real* const,
	       const abaqus_real* const,
	       const abaqus_real* const,
	       const abaqus_real*,
	       const abaqus_real*,
	       const abaqus_real*,
	       const abaqus_real*,
	       const fortran_string_size);

  void VUMATFCT2(const abaqus_int *const,
		const abaqus_int *const,
		const abaqus_int *const,
		const abaqus_int *const,
		const abaqus_int *const,
		const abaqus_int *const,
		const abaqus_int *const,
		const abaqus_real* const,
		const abaqus_real* const,
		const abaqus_real* const,
		const char* const,
		const abaqus_real* const,
		const abaqus_real* const,
		const abaqus_real* const,
		const abaqus_real* const,
		const abaqus_real* const,
		const abaqus_real* const,
		const abaqus_real* const,
		const abaqus_real* const,
		const abaqus_real* const,
		const abaqus_real* const,
		const abaqus_real* const,
		const abaqus_real* const,
		const abaqus_real* const,
		const abaqus_real* const,
		const abaqus_real* const,
		const abaqus_real* const,
		const abaqus_real* const,
		const abaqus_real* const,
		const abaqus_real*,
		const abaqus_real*,
		const abaqus_real*,
		const abaqus_real*,
		const fortran_string_size);
}

struct LibrariesHandler
  : public std::map<std::string,libptr>
{
  ~LibrariesHandler(){
    for(iterator pl=this->begin();pl!=this->end();++pl){
#if (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__)
      ::FreeLibrary(pl->second);
#else
      ::dlclose(pl->second);
#endif
    }
  } // end of ~LibrariesHandler
}; // end of LibrariesHandler

struct VUMATPtrHandler
{
  std::string name;
  vumatptr ptr;
}; // end of VUMATPtrHandler

static void report(const std::string& ln,
		   const std::string& fn,
		   const std::string& en,
		   const char * const n){
  std::cerr << "vumat::load : could not load behaviour '"
  << std::string(n,n+80) << '\'';
  if(!ln.empty()){
    std::cerr << "\nTried library '" << ln << '\'';
  }
  if(!fn.empty()){
    std::cerr << "\nTried function '" << fn << '\'';
  }
  if(!en.empty()){
    std::cerr << '\n' << en;
  }
  std::cerr << std::endl;
}

struct VUMATNameCompare
{
  VUMATNameCompare(const char* const s)
    : n(s)
  {} // end of VUMATNameCompare
  VUMATNameCompare(const VUMATNameCompare &p)
    : n(p.n)
  {}
  bool operator()(const VUMATPtrHandler& h) const{
    return ::strncmp(h.name.data(),n,std::min(h.name.size(),
					      std::string::size_type(79)))==0;
  }
private:
  const char* const n;
};

static void error_msg(const char* const msg){
  std::cerr << "vumat: " << msg << std::endl;
}

static std::string extract(const char*& p,
			   const char* const pe,
			   const char* const msg)
{
  if(p==pe){
    error_msg(msg);
    return "";
  }
  const char* const n = std::find(p,pe,'_');
  if(n==p){
    error_msg(msg);
    return "";
  }
  std::string r = std::string(p,n);
  p=n;
  return r;
}

static std::pair<std::string,std::string> decompose(const char * const n)
{
  const char* pn = n;
  const char* pne = n+80;
  // removing spaces at the end
  while((pne!=pn)&&((std::isspace(*(pne-1)))||(*(pne-1)=='\0'))){
    --pne;
  }
  if(pn==pne){
    error_msg("empty string");
    return std::make_pair(std::string(),std::string());
  };
  const std::string ln = extract(pn,pne,"can't extract library name");
#ifdef _WIN32			 
  std::string lib = "lib" + ln + ".dll";
#else
  std::string lib = "lib" + ln + ".so";
#endif
  if(pn==pne){
    error_msg("extract function name");
    return std::make_pair(std::string(),std::string());
  }
  ++pn;
  std::string fct = extract(pn,pne,"can't extract function name");
  if(fct.empty()){
    return std::make_pair(std::string(),std::string());
  }
  if(pn!=pne){
    ++pn;
    fct += '_'+extract(pn,pne,"can't extract hypothesis");
  }
#ifdef MFRONT_VUMAT_DEBUG
  std::cout << "library: "   << lib << std::endl;
  std::cout << "behaviour: " << fct << std::endl;
  if(pn!=pne){
    ++pn;
    if(pn!=pne){
      std::cout << "suffix: '" << std::string(pn,pne) << "'" << std::endl; 
    }
  }
#endif /* MFRONT_VUMAT_DEBUG */
  return std::make_pair(lib,fct);
}

static vumatptr load(const char* n){
  typedef std::vector<VUMATPtrHandler> VUMATPtrContainer;
  static LibrariesHandler libraries;
  static VUMATPtrContainer fcts;
#ifdef HAVE_STD_MUTEX
   static std::mutex m;
   std::lock_guard<std::mutex> lock(m);
#else /* HAVE_STD_MUTEX */
   lock l;
#endif  /* HAVE_STD_MUTEX */
  try{
    VUMATPtrContainer::const_iterator p;
    p = std::find_if(fcts.begin(),fcts.end(),VUMATNameCompare(n));
    if(p==fcts.end()){
      const std::pair<std::string,std::string> lf = decompose(n);
      const std::string& lib = lf.first;
      const std::string& fct = lf.second;
      if(lib.empty()){
	report("","","",n);
	return NULLPTR(vumatptr);
      }
#if (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__)
      libptr l = ::LoadLibrary(TEXT (lib.c_str()));
#else
      libptr l = ::dlopen(lib.c_str(),RTLD_NOW);
#endif
      if(l==NULLPTR(libptr)){
#if (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__)
	report(lib,"",getLastWin32Error(),n);
#else
	report(lib,"",::dlerror(),n);
#endif	
	return NULLPTR(vumatptr);
      }
      libraries.insert(std::make_pair(lib,l));
      union {
	void *ptr;
	vumatptr f;
      } r;
#if (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__)
      r.f = reinterpret_cast<vumatptr>(::GetProcAddress(l,fct.c_str()));
#else
      r.ptr = ::dlsym(l,fct.c_str());
#endif
      if(r.ptr==NULLPTR(void *)){
#if (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__)
	report(lib,fct,getLastWin32Error(),n);
#else
	report(lib,fct,::dlerror(),n);
#endif	
	return NULLPTR(vumatptr);
      }
      VUMATPtrHandler h;
      h.name = std::string(n,n+80);
      h.ptr  = r.f;
      fcts.push_back(h);
      return r.f;
    }
    return p->ptr;
  }
  catch(const std::exception& e){
    std::cerr << "vumat::load : " << e.what() << std::endl;
  }
  catch(...){
    std::cerr << "vumat::load : unknown exception" << std::endl;
  }
  return NULLPTR(vumatptr);
}

extern "C" {

  void VUMATFCT(const abaqus_int *const nblock,
		const abaqus_int *const ndir,
		const abaqus_int *const nshr,
		const abaqus_int *const nstatev,
		const abaqus_int *const nfieldv,
		const abaqus_int *const nprops,
		const abaqus_int * const lanneal,
		const abaqus_real* const stepTime,
		const abaqus_real* const totalTime,
		const abaqus_real* const dt,
		const char* const cmname,
		const abaqus_real* const coordMp,
		const abaqus_real* const charLength,
		const abaqus_real* const props,
		const abaqus_real* const density,
		const abaqus_real* const strainInc,
		const abaqus_real* const relSpinInc,
		const abaqus_real* const tempOld,
		const abaqus_real* const stretchOld,
		const abaqus_real* const defgradOld,
		const abaqus_real* const fieldOld,
		const abaqus_real* const stressOld,
		const abaqus_real* const stateOld,
		const abaqus_real* const enerInternOld,
		const abaqus_real* const enerInelasOld,
		const abaqus_real* const tempNew,
		const abaqus_real* const stretchNew,
		const abaqus_real* const defgradNew,
		const abaqus_real* const fieldNew,
		const abaqus_real* stressNew,
		const abaqus_real* stateNew,
		const abaqus_real* enerInternNew,
		const abaqus_real* enerInelasNew,
	       const fortran_string_size size){
    const vumatptr f = load(cmname);
    if(f!=NULLPTR(vumatptr)){
      f(nblock,ndir,nshr,nstatev,nfieldv,nprops,
	lanneal,stepTime,totalTime,dt,
	cmname,coordMp,charLength,props,
	density,strainInc,relSpinInc,tempOld,
	stretchOld,defgradOld,fieldOld,stressOld,
	stateOld,enerInternOld,enerInelasOld,tempNew,
	stretchNew,defgradNew,fieldNew,stressNew,
	stateNew,enerInternNew,enerInelasNew,size);
      return;
    }

    /*
     * by default, we fail if we could not load an external function
     */
    
    std::cout << "vumat : unsupported material" << std::endl;
    ::exit(-1);

    /*
     * However, if we want to combine mfront laws and standard vumat,
     * it is possible:
     * - just rename your standard vumat subroutine in vumat2
     * - comment the two previous lines
     * - uncomment the newt lines
     */
    
    // VUMATFCT2(nblock,ndir,nshr,nstatev,nfieldv,nprops,
    // 	         lanneal,stepTime,totalTime,dt,
    // 	         cmname,coordMp,charLength,props,
    // 	         density,strainInc,relSpinInc,tempOld,
    // 	         stretchOld,defgradOld,fieldOld,stressOld,
    // 	         stateOld,enerInternOld,enerInelasOld,tempNew,
    // 	         stretchNew,defgradNew,fieldNew,stressNew,
    // 	         stateNew,enerInternNew,enerInelasNew,size);
    
  } // end of vumat_
  
} // end of extern "C"
