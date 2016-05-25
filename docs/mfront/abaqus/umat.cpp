/*!
 * \file   umat.cpp
 * \brief    
 * \author THOMAS HELFER
 * \date   08 oct. 2015
 */

#include<iostream>
#include<cstdlib>

#include<map>
#include<mutex>
#include<string>
#include<vector>
#include<utility>
#include<cstring>
#include<stdexcept>
#include<algorithm>

#if (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__)
#include<windows.h>
using libptr = HINSTANCE__*;
#else
#include<dlfcn.h>
using libptr = void*;
#endif

using abaqus_real = double;
using abaqus_int  = int;

using umatptr = void (*)(abaqus_real *const,
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

extern "C" {
  void umat_(abaqus_real *const,
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

  void umat2_(abaqus_real *const,
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
}

static umatptr load(const char* n){
  struct LibrariesHandler
    : public std::map<std::string,libptr>
  {
    ~LibrariesHandler(){
      for(auto l:*this){
#if (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__)
	::FreeLibrary(l.second);
#else
	::dlclose(l.second);
#endif
      }
    } // end of ~LibrariesHandler
  }; // end of LibrariesHandler
  struct UmatPtrHandler
  {
    std::string name;
    umatptr ptr;
  }; // end of UmatPtrHandler
  using UmatPtrContainer = std::vector<UmatPtrHandler>;
  static std::mutex m;
  static LibrariesHandler libraries;
  static UmatPtrContainer fcts;
  std::lock_guard<std::mutex> lock(m);
  auto report = [&n](const std::string& ln,
		     const std::string& fn,
		     const std::string& en){
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
  };
  try{
    auto p = std::find_if(fcts.begin(),fcts.end(),[&n](const UmatPtrHandler& h){
	return ::strncmp(h.name.data(),n,std::min(h.name.size(),
						  std::string::size_type(79)))==0;
      });
    if(p==fcts.end()){
      const auto ne = n+80;
      auto pos = std::find(n,ne,'_');
      if((pos==ne)||(pos+1==ne)){
	report("","","");
	return nullptr;
      }
      auto lib = "lib"+std::string(n,pos)+".so";
      auto pos2 = std::find(pos+1,ne,' ');
      if(pos2-pos-1<=0){
	report("","","");
	return nullptr;
      }
      auto fct = std::string(pos+1,pos2);
#if (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__)
      libptr l = ::LoadLibrary(TEXT (lib.c_str()));
#else
      libptr l = ::dlopen(lib.c_str(),RTLD_NOW);
#endif
      if(l==nullptr){
	report(lib,"",::dlerror());
	return nullptr;
      }
      libraries.insert(std::make_pair(lib,l));
      union {
	void *ptr;
	umatptr f;
      } r;
#if (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__)
      r.ptr = ::GetProcAddress(l,fct.c_str());
#else
      r.ptr = ::dlsym(l,fct.c_str());
#endif
      if(r.ptr==nullptr){
	report(lib,fct,::dlerror());
	return nullptr;
      }
      UmatPtrHandler h;
      h.name = std::string(n,ne);
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
  return nullptr;
}

extern "C" {
  
  void umat_(abaqus_real *const STRESS,
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
	     const int size){
    umatptr f = load(CMNAME);
    if(f!=nullptr){
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
    
    //     umat2_(STRESS,STATEV,DDSDDE,SSE,SPD,SCD,RPL,
    // 	   DDSDDT,DRPLDE,DRPLDT,STRAN,DSTRAN,TIME,
    // 	   DTIME,TEMP,DTEMP,PREDEF,DPRED,CMNAME,
    // 	   NDI,NSHR,NTENS,NSTATV,PROPS,NPROPS,
    // 	   COORDS,DROT,PNEWDT,CELENT,DFGRD0,DFGRD1,
    // 	   NOEL,NPT,LAYER,KSPT,KSTEP,KINC,size);
    
  } // end of umat_
  
} // end of extern "C"
