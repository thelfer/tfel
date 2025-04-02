/*!
 * \file   umat.cpp
 * \brief    
 * \author Thomas Helfer
 * \date   21/03/2016
 */

#include "omi_for_c.h"

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
      std::cerr << "umat: abandonned mutex" << std::endl;
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

#if (defined _WIN32) && (!defined _WIN64)
typedef int    fortran_string_size;
#elif defined _WIN64
typedef size_t fortran_string_size;
#else
typedef int    fortran_string_size;
#endif

typedef double abaqus_real;
typedef int    abaqus_int;

typedef  void (*umatptr)(abaqus_real *const,
			 abaqus_real *const,
			 abaqus_real *const,
			 abaqus_real *const,
			 abaqus_real *const,
			 abaqus_real *const,
			 abaqus_real *const,
			 abaqus_real *const,
			 abaqus_real *const,
			 abaqus_real *const,
			 const abaqus_real *const,
			 const abaqus_real *const,
			 const abaqus_real *const,
			 const abaqus_real *const,
			 const abaqus_real *const,
			 const abaqus_real *const,
			 const abaqus_real *const,
			 const abaqus_real *const,
			 const char        *const,
			 const abaqus_int  *const,
			 const abaqus_int  *const,
			 const abaqus_int  *const,
			 const abaqus_int  *const,
			 const abaqus_real *const,
			 const abaqus_int  *const,
			 const abaqus_real *const,
			 const abaqus_real *const,
			 abaqus_real *const,
			 const abaqus_real *const,
			 const abaqus_real *const,
			 const abaqus_real *const,
			 const abaqus_int  *const,
			 const abaqus_int  *const,
			 const abaqus_int  *const,
			 const abaqus_int  *const,
			 const abaqus_int  *const,
			 abaqus_int  *const,
			 const int);

extern "C" void FOR_NAME(umat, UMAT)(
         abaqus_real *const,
	       abaqus_real *const,
	       abaqus_real *const,
	       abaqus_real *const,
	       abaqus_real *const,
	       abaqus_real *const,
	       abaqus_real *const,
	       abaqus_real *const,
	       abaqus_real *const,
	       abaqus_real *const,
	       const abaqus_real *const,
	       const abaqus_real *const,
	       const abaqus_real *const,
	       const abaqus_real *const,
	       const abaqus_real *const,
	       const abaqus_real *const,
	       const abaqus_real *const,
	       const abaqus_real *const,
	       const char        *const,
	       const abaqus_int  *const,
	       const abaqus_int  *const,
	       const abaqus_int  *const,
	       const abaqus_int  *const,
	       const abaqus_real *const,
	       const abaqus_int  *const,
	       const abaqus_real *const,
	       const abaqus_real *const,
	       abaqus_real *const,
	       const abaqus_real *const,
	       const abaqus_real *const,
	       const abaqus_real *const,
	       const abaqus_int  *const,
	       const abaqus_int  *const,
	       const abaqus_int  *const,
	       const abaqus_int  *const,
	       const abaqus_int  *const,
	       abaqus_int  *const,
	       const fortran_string_size);

extern "C" void FOR_NAME(umat2, UMAT2)(
    abaqus_real *const,
		abaqus_real *const,
		abaqus_real *const,
		abaqus_real *const,
		abaqus_real *const,
		abaqus_real *const,
		abaqus_real *const,
		abaqus_real *const,
		abaqus_real *const,
		abaqus_real *const,
		const abaqus_real *const,
		const abaqus_real *const,
		const abaqus_real *const,
		const abaqus_real *const,
		const abaqus_real *const,
		const abaqus_real *const,
		const abaqus_real *const,
		const abaqus_real *const,
		const char        *const,
		const abaqus_int  *const,
		const abaqus_int  *const,
		const abaqus_int  *const,
		const abaqus_int  *const,
		const abaqus_real *const,
		const abaqus_int  *const,
		const abaqus_real *const,
		const abaqus_real *const,
		abaqus_real *const,
		const abaqus_real *const,
		const abaqus_real *const,
		const abaqus_real *const,
		const abaqus_int  *const,
		const abaqus_int  *const,
		const abaqus_int  *const,
		const abaqus_int  *const,
		const abaqus_int  *const,
		abaqus_int  *const,
		const fortran_string_size);

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

struct UmatPtrHandler
{
  std::string name;
  umatptr ptr;
}; // end of UmatPtrHandler

static void report(const std::string& ln,
		   const std::string& fn,
		   const std::string& en,
		   const char * const n){
  std::cerr << "umat::load : could not load behaviour '"
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

struct UMATNameCompare
{
  UMATNameCompare(const char* const s)
    : n(s)
  {} // end of UMATNameCompare
  UMATNameCompare(const UMATNameCompare &p)
    : n(p.n)
  {}
  bool operator()(const UmatPtrHandler& h) const{
    return ::strncmp(h.name.data(),n,std::min(h.name.size(),
					      std::string::size_type(79)))==0;
  }
private:
  const char* const n;
};
static void error_msg(const char* const msg){
  std::cerr << "umat: " << msg << std::endl;
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
  }
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
#ifdef MFRONT_UMAT_DEBUG
  std::cout << "library: "   << lib << std::endl;
  std::cout << "behaviour: " << fct << std::endl;
  if(pn!=pne){
    ++pn;
    if(pn!=pne){
      std::cout << "suffix: '" << std::string(pn,pne) << "'" << std::endl; 
    }
  }
#endif /* MFRONT_UMAT_DEBUG */
  return std::make_pair(lib,fct);
}

static umatptr load(const char* n){
  typedef std::vector<UmatPtrHandler> UmatPtrContainer;
  static LibrariesHandler libraries;
  static UmatPtrContainer fcts;
#ifdef HAVE_STD_MUTEX
   static std::mutex m;
   std::lock_guard<std::mutex> lock(m);
#else /* HAVE_STD_MUTEX */
   lock l;
#endif  /* HAVE_STD_MUTEX */
  try{
    UmatPtrContainer::const_iterator p;
    p = std::find_if(fcts.begin(),fcts.end(),UMATNameCompare(n));
    if(p==fcts.end()){
      const std::pair<std::string,std::string> lf = decompose(n);
      const std::string& lib = lf.first;
      const std::string& fct = lf.second;
      if(lib.empty()){
	report("","","",n);
	return NULLPTR(umatptr);
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
	return NULLPTR(umatptr);
      }
      libraries.insert(std::make_pair(lib,l));
      union {
	void *ptr;
	umatptr f;
      } r;
#if (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__)
      r.f = reinterpret_cast<umatptr>(::GetProcAddress(l,fct.c_str()));
#else
      r.ptr = ::dlsym(l,fct.c_str());
#endif
      if(r.ptr==NULLPTR(void *)){
#if (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__)
	report(lib,fct,getLastWin32Error(),n);
#else
	report(lib,fct,::dlerror(),n);
#endif	
	return NULLPTR(umatptr);
      }
      UmatPtrHandler h;
      h.name = std::string(n,n+80);
      h.ptr  = r.f;
      fcts.push_back(h);
      return r.f;
    }
    return p->ptr;
  }
  catch(const std::exception& e){
    std::cerr << "umat::load : " << e.what() << std::endl;
  }
  catch(...){
    std::cerr << "umat::load : unknown exception" << std::endl;
  }
  return NULLPTR(umatptr);
}

extern "C" void FOR_NAME(umat, UMAT)(
         abaqus_real *const STRESS,
	       abaqus_real *const STATEV,
	       abaqus_real *const DDSDDE,
	       abaqus_real *const SSE,
	       abaqus_real *const SPD,
	       abaqus_real *const SCD,
	       abaqus_real *const RPL,
	       abaqus_real *const DDSDDT,
	       abaqus_real *const DRPLDE,
	       abaqus_real *const DRPLDT,
	       const abaqus_real *const STRAN,
	       const abaqus_real *const DSTRAN,
	       const abaqus_real *const TIME,
	       const abaqus_real *const DTIME,
	       const abaqus_real *const TEMP,
	       const abaqus_real *const DTEMP,
	       const abaqus_real *const PREDEF,
	       const abaqus_real *const DPRED,
	       const char           *const CMNAME,
	       const abaqus_int  *const NDI,
	       const abaqus_int  *const NSHR,
	       const abaqus_int  *const NTENS,
	       const abaqus_int  *const NSTATV,
	       const abaqus_real *const PROPS,
	       const abaqus_int  *const NPROPS,
	       const abaqus_real *const COORDS,
	       const abaqus_real *const DROT,
	       abaqus_real *const PNEWDT,
	       const abaqus_real *const CELENT,
	       const abaqus_real *const DFGRD0,
	       const abaqus_real *const DFGRD1,
	       const abaqus_int  *const NOEL,
	       const abaqus_int  *const NPT,
	       const abaqus_int  *const LAYER,
	       const abaqus_int  *const KSPT,
	       const abaqus_int  *const KSTEP,
	       abaqus_int  *const KINC,
	       const fortran_string_size size){
    umatptr f = load(CMNAME);
    if(f!=NULLPTR(umatptr)){
      f(STRESS,STATEV,DDSDDE,SSE,SPD,SCD,RPL,
	DDSDDT,DRPLDE,DRPLDT,STRAN,DSTRAN,TIME,
	DTIME,TEMP,DTEMP,PREDEF,DPRED,CMNAME,
	NDI,NSHR,NTENS,NSTATV,PROPS,NPROPS,
	COORDS,DROT,PNEWDT,CELENT,DFGRD0,DFGRD1,
	NOEL,NPT,LAYER,KSPT,KSTEP,KINC,size);
      return;
    }

    /*
     * by default, we fail if we could not load an external function
     */
    
    std::cout << "umat : unsupported material" << std::endl;
    ::exit(-1);

    /*
     * However, if we want to combine mfront laws and standard umat,
     * it is possible:
     * - just rename your standard umat subroutine in umat2
     * - comment the two previous lines
     * - uncomment the newt lines
     */
    
    //     UMATFCT2(STRESS,STATEV,DDSDDE,SSE,SPD,SCD,RPL,
    // 	   DDSDDT,DRPLDE,DRPLDT,STRAN,DSTRAN,TIME,
    // 	   DTIME,TEMP,DTEMP,PREDEF,DPRED,CMNAME,
    // 	   NDI,NSHR,NTENS,NSTATV,PROPS,NPROPS,
    // 	   COORDS,DROT,PNEWDT,CELENT,DFGRD0,DFGRD1,
    // 	   NOEL,NPT,LAYER,KSPT,KSTEP,KINC,size);
    
  } // end of umat_
  
// int main(void){
//   char n[80] = {'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0',
// 		'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0',
// 		'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0',
// 		'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0',
// 		'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0',
// 		'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0',
// 		'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0',
// 		'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'};
//   const char * b = "UMATBEHAVIOUR_CHABOCHE_3D_1";
//   std::copy(b,b+std::strlen(b),n);
//   std::pair<std::string,std::string> r = decompose(n);
//   std::cout << r.first << std::endl;
//   std::cout << r.second << std::endl;
//   std::fill(n,n+80,'\0');
//   const char * b2 = "UMATBEHAVIOUR_CHABOCHE_PSTRAIN";
//   std::copy(b2,b2+std::strlen(b2),n);
//   r = decompose(n);
//   std::cout << r.first << std::endl;
//   std::cout << r.second << std::endl;
//   std::fill(n,n+80,'\0');
//   r = decompose(n);
//   std::cout << r.first << std::endl;
//   std::cout << r.second << std::endl;
//   return 0;
// }
