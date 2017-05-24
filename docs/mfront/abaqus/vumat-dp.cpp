/*!
 * \file   vumat-dp.cpp
 * \brief    
 * \author Thomas Helfer
 * \date   21/03/2016
 */

#include<iostream>
#include<cstdlib>

#include<map>
#include<mutex>
#include<string>
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

using vumatptr = void (*)(const abaqus_int *const,
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
			  const int);

extern "C" {

  void vumat_(const abaqus_int *const,
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
	      const int);

  void vumat2_(const abaqus_int *const,
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
	       const int);
}

static vumatptr load(const char* n){
  struct LibrariesHandler
    : public std::map<std::string,libptr>
  {
    ~LibrariesHandler(){
      for(auto& l:*this){
#if (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__)
	::FreeLibrary(l.second);
#else
	::dlclose(l.second);
#endif
      }
    } // end of ~LibrariesHandler
  }; // end of LibrariesHandler
  struct VUMATPtrHandler
  {
    std::string name;
    vumatptr ptr;
  }; // end of VUMATPtrHandler
  static std::mutex m;
  static LibrariesHandler libraries;
  static std::vector<VUMATPtrHandler> fcts;
  std::lock_guard<std::mutex> lock(m);
  auto report = [&n](const std::string& ln,
		     const std::string& fn,
		     const std::string& en){
    std::cerr << "umat::load : could not load behaviour '"
    << std::string{n,n+80} << '\'';
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
    auto p = std::find_if(fcts.begin(),fcts.end(),[&n](const VUMATPtrHandler& h){
	return ::strncmp(h.name.data(),n,std::min(h.name.size(),
						  std::string::size_type{79}))==0;
      });
    if(p==fcts.end()){
      const auto ne = n+80;
      auto pos = std::find(n,ne,'_');
      if((pos==ne)||(pos+1==ne)){
	report("","","");
	return nullptr;
      }
      auto lib = "lib"+std::string{n,pos}+".so";
      auto pos2 = std::find(pos+1,ne,' ');
      if(pos2-pos-1<=0){
	report("","","");
	return nullptr;
      }
      auto fct = std::string{pos+1,pos2};
#if (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__)
      libptr l = ::LoadLibrary(TEXT (lib.c_str()));
#else
      libptr l = ::dlopen(lib.c_str(),RTLD_NOW);
#endif
      if(l==nullptr){
	report(lib,"",::dlerror());
	return nullptr;
      }
      libraries.insert({lib,l});
      union {
	void *ptr;
	vumatptr f;
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
      VUMATPtrHandler h;
      h.name = std::string(n,ne);
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
  return nullptr;
}

extern "C" {
  
  void vumat_(const abaqus_int *const nblock,
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
	      const int size){
  vumatptr f = load(cmname);
  if(f!=nullptr){
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
   * - just rename your standard umat subroutine in umat2
   * - comment the two previous lines
   * - uncomment the newt lines
   */
  // vumat2_(nblock,ndir,nshr,nstatev,nfieldv,nprops,
  // 	    lanneal,stepTime,totalTime,dt,
  // 	    cmname,coordMp,charLength,props,
  // 	    density,strainInc,relSpinInc,tempOld,
  // 	    stretchOld,defgradOld,fieldOld,stressOld,
  // 	    stateOld,enerInternOld,enerInelasOld,tempNew,
  // 	    stretchNew,defgradNew,fieldNew,stressNew,
  // 	    stateNew,enerInternNew,enerInelasNew,size);
    
} // end of vumat_
  
} // end of extern "C"
