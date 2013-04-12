/*!
 * \file   ExternalLibraryManager.cpp
 * \brief    
 * \author Helfer Thomas
 * \date   31 Oct 2007
 */

#include<stdexcept>
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
#include <windows.h>
#else
#include<dlfcn.h>
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */

#include"TFEL/System/getFunction.h"
#include"TFEL/System/ExternalLibraryManager.hxx"

namespace tfel
{
  namespace system
  {
    
    ExternalLibraryManager&
    ExternalLibraryManager::getExternalLibraryManager()
    {
      static ExternalLibraryManager externalLibraryManager;
      return externalLibraryManager;
    } // end of ExternalLibraryManager::getExternalLibraryManager()

    ExternalLibraryManager::ExternalLibraryManager()
    {} // end of ExternalLibraryManager::ExternalLibraryManager

#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
   HINSTANCE__*
#else
   void *
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */
    ExternalLibraryManager::loadLibrary(const std::string& name,
					const bool b)
    {
      using namespace std;
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
      typedef map<string,HINSTANCE__*>::value_type MVType;
      map<string,HINSTANCE__*>::const_iterator p;
#else 
      typedef map<string,void *>::value_type MVType;
      map<string,void*>::const_iterator p;
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */      
      p=this->librairies.find(name);
      if(p==librairies.end()){
	// this library has not been 
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
	HINSTANCE__* lib;
	lib = ::LoadLibrary(TEXT (name.c_str()));
#else
	void * lib;
	lib = ::dlopen(name.c_str(),RTLD_NOW);
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */
	if((lib==0)&&(!b)){
	  string msg("ExternalLibraryManager::loadLibrary : library '");
	  msg += name;
	  msg += "' could not be loaded, (";
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
	  msg += ::GetLastError();
#else
	  msg += ::dlerror();
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */
	  msg += ")";
	  throw(runtime_error(msg));
	} else if((lib==0)&&(b)){
	  return lib;
	}
	this->librairies.insert(MVType(name,lib));
	return lib;
      }
      return p->second;
    } // end of ExternalLibraryManager::loadLibrary

    bool
    ExternalLibraryManager::contains(const std::string& l,
				     const std::string& s)
    {
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
      HINSTANCE__* lib = this->loadLibrary(l);
      int (*p)(void)   = (int (*)(void)) ::GetProcAddress(lib,s.c_str());
      return p!=static_cast<int (*)(void)>(0);
#else
      void * lib = this->loadLibrary(l);
      void * p   = ::dlsym(lib,s.c_str());
      return p!=static_cast<void *>(0);
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */
    } // end of ExternalLibraryManager::contains

    std::string
    ExternalLibraryManager::getSource(const std::string& l,
				      const std::string& f)
    {
      using namespace std;
      string s;
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
      HINSTANCE__* lib = this->loadLibrary(l);
      const char* const *p  = (const char* const *) ::GetProcAddress(lib,(f+"_src").c_str());
#else
      void * lib = this->loadLibrary(l);
      void * p   = ::dlsym(lib,(f+"_src").c_str());
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */
      if(p!=0){
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
	s = *p;
#else
	s = *(static_cast<const char* const *>(p));
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */
      }
      return s;
    } // end of ExternalLibraryManager::getSource
        
    void
    ExternalLibraryManager::setParameter(const std::string& l,
					 const std::string& f,
					 const std::string& p,
					 const double v)
    {
      using namespace std;
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
      HINSTANCE__* lib = this->loadLibrary(l);
#else
      void * lib = this->loadLibrary(l);
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */
      int (*fct)(const char*const,const double);
      fct = ::tfel_getSetParameterFunction(lib,(f+"_setParameter").c_str());
      if(fct==0){
	string msg("ExternalLibraryManager::setParameter : ");
	msg += " can't get the '"+f+"_setParameter' function (";
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
	  msg += ::GetLastError();
#else
	  msg += ::dlerror();
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */
	msg += ")";
	throw(runtime_error(msg));
      }
      if(!fct(p.c_str(),v)){
	string msg("ExternalLibraryManager::setParameter : ");
	msg += " call to the '"+f+"_setParameter' function failed";
	throw(runtime_error(msg));
      }
    } // end of ExternalLibraryManager::setParameter

    void
    ExternalLibraryManager::setParameter(const std::string& l,
					 const std::string& f,
					 const std::string& p,
					 const int v)
    {
      using namespace std;
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
      HINSTANCE__* lib = this->loadLibrary(l);
#else
      void * lib = this->loadLibrary(l);
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */
      int (*fct)(const char*const,const int);
      fct = ::tfel_getSetIntegerParameterFunction(lib,(f+"_setIntegerParameter").c_str());
      if(fct==0){
	string msg("ExternalLibraryManager::setParameter : ");
	msg += " can't get the '"+f+"_setParameter' function (";
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
	  msg += ::GetLastError();
#else
	  msg += ::dlerror();
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */
	msg += ")";
	throw(runtime_error(msg));
      }
      if(!fct(p.c_str(),v)){
	string msg("ExternalLibraryManager::setParameter : ");
	msg += " call to the '"+f+"_setParameter' function failed";
	throw(runtime_error(msg));
      }
    } // end of ExternalLibraryManager::setParameter
    
    void
    ExternalLibraryManager::setParameter(const std::string& l,
					 const std::string& f,
					 const std::string& p,
					 const unsigned short v)
    {
      using namespace std;
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
      HINSTANCE__* lib = this->loadLibrary(l);
#else
      void * lib = this->loadLibrary(l);
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */
      int (*fct)(const char*const,const unsigned short);
      fct = ::tfel_getSetUnsignedShortParameterFunction(lib,(f+"_setUnsignedShortParameter").c_str());
      if(fct==0){
	string msg("ExternalLibraryManager::setParameter : ");
	msg += " can't get the '"+f+"_setParameter' function (";
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
	  msg += ::GetLastError();
#else
	  msg += ::dlerror();
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */
	msg += ")";
	throw(runtime_error(msg));
      }
      if(!fct(p.c_str(),v)){
	string msg("ExternalLibraryManager::setParameter : ");
	msg += " call to the '"+f+"_setParameter' function failed";
	throw(runtime_error(msg));
      }
    } // end of ExternalLibraryManager::setParameter
    
    unsigned short
    ExternalLibraryManager::getCastemFunctionNumberOfVariables(const std::string& l,
							       const std::string& f)
    {
      using namespace std;
      int res;
      #if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
      HINSTANCE__* lib = this->loadLibrary(l);
#else
      void * lib = this->loadLibrary(l);
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */

      res = ::tfel_getCastemFunctionNumberOfVariables(lib,f.c_str());
      if(res<0){
	string msg("ExternalLibraryManager::getCastemFunctionNumberOfVariables : ");
	msg += " number of variables could not be read (";
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
	  msg += ::GetLastError();
#else
	  msg += ::dlerror();
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */
	msg += ")";
	throw(runtime_error(msg));
      }
      return static_cast<unsigned short>(res);
    }

    std::vector<std::string>
    ExternalLibraryManager::getCastemFunctionVariables(const std::string& l,
						       const std::string& f)
    {
      using namespace std;
      vector<string> vars;
      this->getCastemFunctionVariables(vars,l,f);
      return vars;
    } // end of ExternalLibraryManager::getCastemFunctionVariables

    void
    ExternalLibraryManager::getCastemFunctionVariables(std::vector<std::string>& vars,
						       const std::string& l,
						       const std::string& f)
    {
      using namespace std;
      #if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
      HINSTANCE__* lib = this->loadLibrary(l);
#else
      void * lib = this->loadLibrary(l);
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */

      unsigned short nb = this->getCastemFunctionNumberOfVariables(l,f);
      char ** res = ::tfel_getCastemFunctionVariables(lib,f.c_str());
      char **p;
      if(res==0){
	string msg("ExternalLibraryManager::getCastemFunctionNumberOfVariables : ");
	msg += " variables names could not be read (";
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
	  msg += ::GetLastError();
#else
	  msg += ::dlerror();
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */
	msg += ")";
	throw(runtime_error(msg));
      }
      for(p=res;p!=res+nb;++p){
	vars.push_back(*p);
      }
    } // end of ExternalLibraryManager::getCastemFunctionVariables

    UMATFctPtr
    ExternalLibraryManager::getUMATFunction(const std::string& l,
					      const std::string& f)
    {
      using namespace std;
      #if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
      HINSTANCE__* lib = this->loadLibrary(l);
#else
      void * lib = this->loadLibrary(l);
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */
      UMATFctPtr fct = ::tfel_getUMATFunction(lib,f.c_str());
      if(fct==0){
	string msg("ExternalLibraryManager::getUMATFunction : ");
	msg += " could not load UMAT function '"+f+"' (";
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
	  msg += ::GetLastError();
#else
	  msg += ::dlerror();
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */
	msg += ")";
	throw(runtime_error(msg));
      }
      return fct;
    }

    void
    ExternalLibraryManager::getUMATNames(std::vector<std::string>& vars,
					 const std::string& l,
					 const std::string& f,
					 const std::string& n)
    {
      using namespace std;
      #if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
      HINSTANCE__* lib = this->loadLibrary(l);
#else
      void * lib = this->loadLibrary(l);
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */

      int nb = ::tfel_getUnsignedShort(lib,(f+"_n"+n).c_str());
      char ** res;
      if(nb==-1){
	string msg("ExternalLibraryManager::getUMATNames : ");
	msg += " number of variables names could not be read (";
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
	  msg += ::GetLastError();
#else
	  msg += ::dlerror();
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */
	msg += ")";
	throw(runtime_error(msg));
      }
      res = ::tfel_getArrayOfStrings(lib,(f+'_'+n).c_str());
      if(res==0){
	string msg("ExternalLibraryManager::getUMATNames : ");
	msg += "variables names could not be read (";
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
	  msg += ::GetLastError();
#else
	  msg += ::dlerror();
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */
	msg += ")";
	throw(runtime_error(msg));
      }
      copy(res,res+nb,back_inserter(vars));
    } // end of ExternalLibraryManager::getUMATNames

    bool
    ExternalLibraryManager::isUMATBehaviourUsableInPurelyImplicitResolution(const std::string& l,
									    const std::string& f)
    {
      using namespace std;
      vector<int> types;
      #if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
      HINSTANCE__* lib = this->loadLibrary(l);
#else
      void * lib = this->loadLibrary(l);
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */

      int b = ::tfel_getBool(lib,(f+"_UsableInPurelyImplicitResolution").c_str());
      if(b==-1){
	return false;
      }
      if(b==1){
	return true;
      }
      return false;
    } // end of ExternalLibraryManager::isUMATBehaviourUsableInPurelyImplicitResolution

    unsigned short
    ExternalLibraryManager::getUMATBehaviourType(const std::string& l,
						 const std::string& f)
    {
      using namespace std;
      vector<int> types;
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
      HINSTANCE__* lib = this->loadLibrary(l);
#else
      void * lib = this->loadLibrary(l);
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */

      int u = ::tfel_getUnsignedShort(lib,(f+"_BehaviourType").c_str());
      if(u==-1){
	string msg("ExternalLibraryManager::getUMATBehaviourType : ");
	msg += " behavour type could not be read (";
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
	  msg += ::GetLastError();
#else
	  msg += ::dlerror();
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */
	msg += ")";
	throw(runtime_error(msg));
      }
      return static_cast<unsigned short>(u);
    } // end of ExternalLibraryManager::getUMATBehaviourType

    std::vector<int>
    ExternalLibraryManager::getUMATInternalStateVariablesTypes(const std::string& l,
							       const std::string& f)
    {
      using namespace std;
      vector<int> types;
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
      HINSTANCE__* lib = this->loadLibrary(l);
#else
      void * lib = this->loadLibrary(l);
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */

      int nb = ::tfel_getUnsignedShort(lib,(f+"_nInternalStateVariables").c_str());
      int * res;
      if(nb==-1){
	string msg("ExternalLibraryManager::getUMATInternalStateVariablesTypes : ");
	msg += " number of variables names could not be read (";
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
	  msg += ::GetLastError();
#else
	  msg += ::dlerror();
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */
	msg += ")";
	throw(runtime_error(msg));
      }
      res = ::tfel_getArrayOfInt(lib,(f+"_InternalStateVariablesTypes").c_str());
      if(res==0){
	string msg("ExternalLibraryManager::getUMATInternalStateVariablesTypes : ");
	msg += "internal state variables types could not be read (";
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
	  msg += ::GetLastError();
#else
	  msg += ::dlerror();
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */
	msg += ")";
	throw(runtime_error(msg));
      }
      copy(res,res+nb,back_inserter(types));
      return types;
    } // end of ExternalLibraryManager::getUMATInternalVariablesTypes

    std::vector<std::string>
    ExternalLibraryManager::getUMATMaterialPropertiesNames(const std::string& l,
							   const std::string& f)
    {
      using namespace std;
      vector<string> vars;
      this->getUMATNames(vars,l,f,"MaterialProperties");
      return vars;
    } // end of ExternalLibraryManager::getUMATMaterialPropertiesNames


    std::vector<std::string>
    ExternalLibraryManager::getUMATInternalStateVariablesNames(const std::string& l,
							       const std::string& f)
    {
      using namespace std;
      vector<string> vars;
      this->getUMATNames(vars,l,f,"InternalStateVariables");
      return vars;
    } // end of ExternalLibraryManager::getUMATMaterialPropertiesNames

    std::vector<std::string>
    ExternalLibraryManager::getUMATExternalStateVariablesNames(const std::string& l,
							       const std::string& f)
    {
      using namespace std;
      vector<string> vars;
      this->getUMATNames(vars,l,f,"ExternalStateVariables");
      return vars;
    } // end of ExternalLibraryManager::getUMATMaterialPropertiesNames
  
    CastemFunctionPtr
    ExternalLibraryManager::getCastemFunction(const std::string& l,
					      const std::string& f)
    {
      using namespace std;
      #if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
      HINSTANCE__* lib = this->loadLibrary(l);
#else
      void * lib = this->loadLibrary(l);
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */

      CastemFunctionPtr fct = ::tfel_getCastemFunction(lib,f.c_str());
      if(fct==0){
	string msg("ExternalLibraryManager::getCastemFunction : ");
	msg += " could not load castem function '"+f+"' (";
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
	  msg += ::GetLastError();
#else
	  msg += ::dlerror();
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */
	msg += ")";
	throw(runtime_error(msg));
      }
      return fct;
    }

    CFunction0Ptr
    ExternalLibraryManager::getCFunction0(const std::string& l,
					  const std::string& f)
    {
      using namespace std;
      #if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
      HINSTANCE__* lib = this->loadLibrary(l);
#else
      void * lib = this->loadLibrary(l);
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */

      CFunction0Ptr fct = ::tfel_getCFunction0(lib,f.c_str());
      if(fct==0){
	string msg("ExternalLibraryManager::getCFunction0 : ");
	msg += " could not load function '"+f+"' (";
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
	  msg += ::GetLastError();
#else
	  msg += ::dlerror();
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */
	msg += ")";
	throw(runtime_error(msg));
      }
      return fct;
    }

    CFunction1Ptr
    ExternalLibraryManager::getCFunction1(const std::string& l,
					  const std::string& f)
    {
      using namespace std;
      #if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
      HINSTANCE__* lib = this->loadLibrary(l);
#else
      void * lib = this->loadLibrary(l);
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */

      CFunction1Ptr fct = ::tfel_getCFunction1(lib,f.c_str());
      if(fct==0){
	string msg("ExternalLibraryManager::getCFunction1 : ");
	msg += " could not load function '"+f+"' (";
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
	  msg += ::GetLastError();
#else
	  msg += ::dlerror();
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */
	msg += ")";
	throw(runtime_error(msg));
      }
      return fct;
    }

    CFunction2Ptr
    ExternalLibraryManager::getCFunction2(const std::string& l,
					  const std::string& f)
    {
      using namespace std;
      #if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
      HINSTANCE__* lib = this->loadLibrary(l);
#else
      void * lib = this->loadLibrary(l);
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */

      CFunction2Ptr fct = ::tfel_getCFunction2(lib,f.c_str());
      if(fct==0){
	string msg("ExternalLibraryManager::getCFunction2 : ");
	msg += " could not load function '"+f+"' (";
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
	  msg += ::GetLastError();
#else
	  msg += ::dlerror();
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */
	msg += ")";
	throw(runtime_error(msg));
      }
      return fct;
    }

    CFunction3Ptr
    ExternalLibraryManager::getCFunction3(const std::string& l,
					  const std::string& f)
    {
      using namespace std;
      #if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
      HINSTANCE__* lib = this->loadLibrary(l);
#else
      void * lib = this->loadLibrary(l);
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */

      CFunction3Ptr fct = ::tfel_getCFunction3(lib,f.c_str());
      if(fct==0){
	string msg("ExternalLibraryManager::getCFunction3 : ");
	msg += " could not load function '"+f+"' (";
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
	  msg += ::GetLastError();
#else
	  msg += ::dlerror();
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */
	msg += ")";
	throw(runtime_error(msg));
      }
      return fct;
    }

    CFunction4Ptr
    ExternalLibraryManager::getCFunction4(const std::string& l,
					  const std::string& f)
    {
      using namespace std;
      #if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
      HINSTANCE__* lib = this->loadLibrary(l);
#else
      void * lib = this->loadLibrary(l);
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */

      CFunction4Ptr fct  = ::tfel_getCFunction4(lib,f.c_str());
      if(fct==0){
	string msg("ExternalLibraryManager::getCFunction4 : ");
	msg += " could not load function '"+f+"' (";
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
	  msg += ::GetLastError();
#else
	  msg += ::dlerror();
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */
	msg += ")";
	throw(runtime_error(msg));
      }
      return fct;
    }

    CFunction5Ptr
    ExternalLibraryManager::getCFunction5(const std::string& l,
					  const std::string& f)
    {
      using namespace std;
      #if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
      HINSTANCE__* lib = this->loadLibrary(l);
#else
      void * lib = this->loadLibrary(l);
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */

      CFunction5Ptr fct = ::tfel_getCFunction5(lib,f.c_str());
      if(fct==0){
	string msg("ExternalLibraryManager::getCFunction5 : ");
	msg += " could not load function '"+f+"' (";
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
	  msg += ::GetLastError();
#else
	  msg += ::dlerror();
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */
	msg += ")";
	throw(runtime_error(msg));
      }
      return fct;
    }

    CFunction6Ptr
    ExternalLibraryManager::getCFunction6(const std::string& l,
					  const std::string& f)
    {
      using namespace std;
      #if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
      HINSTANCE__* lib = this->loadLibrary(l);
#else
      void * lib = this->loadLibrary(l);
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */

      CFunction6Ptr fct = ::tfel_getCFunction6(lib,f.c_str());
      if(fct==0){
	string msg("ExternalLibraryManager::getCFunction6 : ");
	msg += " could not load function '"+f+"' (";
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
	  msg += ::GetLastError();
#else
	  msg += ::dlerror();
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */
	msg += ")";
	throw(runtime_error(msg));
      }
      return fct;
    }

    CFunction7Ptr
    ExternalLibraryManager::getCFunction7(const std::string& l,
					  const std::string& f)
    {
      using namespace std;
      #if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
      HINSTANCE__* lib = this->loadLibrary(l);
#else
      void * lib = this->loadLibrary(l);
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */

      CFunction7Ptr fct = ::tfel_getCFunction7(lib,f.c_str());
      if(fct==0){
	string msg("ExternalLibraryManager::getCFunction7 : ");
	msg += " could not load function '"+f+"' (";
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
	  msg += ::GetLastError();
#else
	  msg += ::dlerror();
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */
	msg += ")";
	throw(runtime_error(msg));
      }
      return fct;
    }

    CFunction8Ptr
    ExternalLibraryManager::getCFunction8(const std::string& l,
					  const std::string& f)
    {
      using namespace std;
      #if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
      HINSTANCE__* lib = this->loadLibrary(l);
#else
      void * lib = this->loadLibrary(l);
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */

      CFunction8Ptr fct = ::tfel_getCFunction8(lib,f.c_str());
      if(fct==0){
	string msg("ExternalLibraryManager::getCFunction8 : ");
	msg += " could not load function '"+f+"' (";
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
	  msg += ::GetLastError();
#else
	  msg += ::dlerror();
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */
	msg += ")";
	throw(runtime_error(msg));
      }
      return fct;
    }

    CFunction9Ptr
    ExternalLibraryManager::getCFunction9(const std::string& l,
					  const std::string& f)
    {
      using namespace std;
      #if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
      HINSTANCE__* lib = this->loadLibrary(l);
#else
      void * lib = this->loadLibrary(l);
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */

      CFunction9Ptr fct = ::tfel_getCFunction9(lib,f.c_str());
      if(fct==0){
	string msg("ExternalLibraryManager::getCFunction9 : ");
	msg += " could not load function '"+f+"' (";
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
	  msg += ::GetLastError();
#else
	  msg += ::dlerror();
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */
	msg += ")";
	throw(runtime_error(msg));
      }
      return fct;
    }

    CFunction10Ptr
    ExternalLibraryManager::getCFunction10(const std::string& l,
					   const std::string& f)
    {
      using namespace std;
      #if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
      HINSTANCE__* lib = this->loadLibrary(l);
#else
      void * lib = this->loadLibrary(l);
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */

      CFunction10Ptr fct = ::tfel_getCFunction10(lib,f.c_str());
      if(fct==0){
	string msg("ExternalLibraryManager::getCFunction10 : ");
	msg += " could not load function '"+f+"' (";
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
	  msg += ::GetLastError();
#else
	  msg += ::dlerror();
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */
	msg += ")";
	throw(runtime_error(msg));
      }
      return fct;
    }

    CFunction11Ptr
    ExternalLibraryManager::getCFunction11(const std::string& l,
					   const std::string& f)
    {
      using namespace std;
      #if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
      HINSTANCE__* lib = this->loadLibrary(l);
#else
      void * lib = this->loadLibrary(l);
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */

      CFunction11Ptr fct = ::tfel_getCFunction11(lib,f.c_str());
      if(fct==0){
	string msg("ExternalLibraryManager::getCFunction11 : ");
	msg += " could not load function '"+f+"' (";
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
	  msg += ::GetLastError();
#else
	  msg += ::dlerror();
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */
	msg += ")";
	throw(runtime_error(msg));
      }
      return fct;
    }

    CFunction12Ptr
    ExternalLibraryManager::getCFunction12(const std::string& l,
					   const std::string& f)
    {
      using namespace std;
      #if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
      HINSTANCE__* lib = this->loadLibrary(l);
#else
      void * lib = this->loadLibrary(l);
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */

      CFunction12Ptr fct = ::tfel_getCFunction12(lib,f.c_str());
      if(fct==0){
	string msg("ExternalLibraryManager::getCFunction12 : ");
	msg += " could not load function '"+f+"' (";
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
	  msg += ::GetLastError();
#else
	  msg += ::dlerror();
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */
	msg += ")";
	throw(runtime_error(msg));
      }
      return fct;
    }

    CFunction13Ptr
    ExternalLibraryManager::getCFunction13(const std::string& l,
					   const std::string& f)
    {
      using namespace std;
      #if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
      HINSTANCE__* lib = this->loadLibrary(l);
#else
      void * lib = this->loadLibrary(l);
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */

      CFunction13Ptr fct = ::tfel_getCFunction13(lib,f.c_str());
      if(fct==0){
	string msg("ExternalLibraryManager::getCFunction13 : ");
	msg += " could not load function '"+f+"' (";
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
	  msg += ::GetLastError();
#else
	  msg += ::dlerror();
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */
	msg += ")";
	throw(runtime_error(msg));
      }
      return fct;
    }

    CFunction14Ptr
    ExternalLibraryManager::getCFunction14(const std::string& l,
					   const std::string& f)
    {
      using namespace std;
      #if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
      HINSTANCE__* lib = this->loadLibrary(l);
#else
      void * lib = this->loadLibrary(l);
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */

      CFunction14Ptr fct = ::tfel_getCFunction14(lib,f.c_str());
      if(fct==0){
	string msg("ExternalLibraryManager::getCFunction14 : ");
	msg += " could not load function '"+f+"' (";
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
	  msg += ::GetLastError();
#else
	  msg += ::dlerror();
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */
	msg += ")";
	throw(runtime_error(msg));
      }
      return fct;
    }

    CFunction15Ptr
    ExternalLibraryManager::getCFunction15(const std::string& l,
					   const std::string& f)
    {
      using namespace std;
      #if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
      HINSTANCE__* lib = this->loadLibrary(l);
#else
      void * lib = this->loadLibrary(l);
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */

      CFunction15Ptr fct = ::tfel_getCFunction15(lib,f.c_str());
      if(fct==0){
	string msg("ExternalLibraryManager::getCFunction15 : ");
	msg += " could not load function '"+f+"' (";
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
	  msg += ::GetLastError();
#else
	  msg += ::dlerror();
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */
	msg += ")";
	throw(runtime_error(msg));
      }
      return fct;
    }

    FortranFunction0Ptr
    ExternalLibraryManager::getFortranFunction0(const std::string& l,
					  const std::string& f)
    {
      using namespace std;
      #if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
      HINSTANCE__* lib = this->loadLibrary(l);
#else
      void * lib = this->loadLibrary(l);
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */

      FortranFunction0Ptr fct = ::tfel_getFortranFunction0(lib,f.c_str());
      if(fct==0){
	string msg("ExternalLibraryManager::getFortranFunction0 : ");
	msg += " could not load function '"+f+"' (";
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
	  msg += ::GetLastError();
#else
	  msg += ::dlerror();
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */
	msg += ")";
	throw(runtime_error(msg));
      }
      return fct;
    }

    FortranFunction1Ptr
    ExternalLibraryManager::getFortranFunction1(const std::string& l,
					  const std::string& f)
    {
      using namespace std;
      #if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
      HINSTANCE__* lib = this->loadLibrary(l);
#else
      void * lib = this->loadLibrary(l);
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */

      FortranFunction1Ptr fct = ::tfel_getFortranFunction1(lib,f.c_str());
      if(fct==0){
	string msg("ExternalLibraryManager::getFortranFunction1 : ");
	msg += " could not load function '"+f+"' (";
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
	  msg += ::GetLastError();
#else
	  msg += ::dlerror();
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */
	msg += ")";
	throw(runtime_error(msg));
      }
      return fct;
    }

    FortranFunction2Ptr
    ExternalLibraryManager::getFortranFunction2(const std::string& l,
					  const std::string& f)
    {
      using namespace std;
      #if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
      HINSTANCE__* lib = this->loadLibrary(l);
#else
      void * lib = this->loadLibrary(l);
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */

      FortranFunction2Ptr fct = ::tfel_getFortranFunction2(lib,f.c_str());
      if(fct==0){
	string msg("ExternalLibraryManager::getFortranFunction2 : ");
	msg += " could not load function '"+f+"' (";
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
	  msg += ::GetLastError();
#else
	  msg += ::dlerror();
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */
	msg += ")";
	throw(runtime_error(msg));
      }
      return fct;
    }

    FortranFunction3Ptr
    ExternalLibraryManager::getFortranFunction3(const std::string& l,
					  const std::string& f)
    {
      using namespace std;
      #if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
      HINSTANCE__* lib = this->loadLibrary(l);
#else
      void * lib = this->loadLibrary(l);
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */

      FortranFunction3Ptr fct = ::tfel_getFortranFunction3(lib,f.c_str());
      if(fct==0){
	string msg("ExternalLibraryManager::getFortranFunction3 : ");
	msg += " could not load function '"+f+"' (";
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
	  msg += ::GetLastError();
#else
	  msg += ::dlerror();
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */
	msg += ")";
	throw(runtime_error(msg));
      }
      return fct;
    }

    FortranFunction4Ptr
    ExternalLibraryManager::getFortranFunction4(const std::string& l,
					  const std::string& f)
    {
      using namespace std;
      #if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
      HINSTANCE__* lib = this->loadLibrary(l);
#else
      void * lib = this->loadLibrary(l);
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */

      FortranFunction4Ptr fct  = ::tfel_getFortranFunction4(lib,f.c_str());
      if(fct==0){
	string msg("ExternalLibraryManager::getFortranFunction4 : ");
	msg += " could not load function '"+f+"' (";
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
	  msg += ::GetLastError();
#else
	  msg += ::dlerror();
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */
	msg += ")";
	throw(runtime_error(msg));
      }
      return fct;
    }

    FortranFunction5Ptr
    ExternalLibraryManager::getFortranFunction5(const std::string& l,
					  const std::string& f)
    {
      using namespace std;
      #if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
      HINSTANCE__* lib = this->loadLibrary(l);
#else
      void * lib = this->loadLibrary(l);
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */

      FortranFunction5Ptr fct = ::tfel_getFortranFunction5(lib,f.c_str());
      if(fct==0){
	string msg("ExternalLibraryManager::getFortranFunction5 : ");
	msg += " could not load function '"+f+"' (";
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
	  msg += ::GetLastError();
#else
	  msg += ::dlerror();
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */
	msg += ")";
	throw(runtime_error(msg));
      }
      return fct;
    }

    FortranFunction6Ptr
    ExternalLibraryManager::getFortranFunction6(const std::string& l,
					  const std::string& f)
    {
      using namespace std;
      #if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
      HINSTANCE__* lib = this->loadLibrary(l);
#else
      void * lib = this->loadLibrary(l);
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */

      FortranFunction6Ptr fct = ::tfel_getFortranFunction6(lib,f.c_str());
      if(fct==0){
	string msg("ExternalLibraryManager::getFortranFunction6 : ");
	msg += " could not load function '"+f+"' (";
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
	  msg += ::GetLastError();
#else
	  msg += ::dlerror();
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */
	msg += ")";
	throw(runtime_error(msg));
      }
      return fct;
    }

    FortranFunction7Ptr
    ExternalLibraryManager::getFortranFunction7(const std::string& l,
					  const std::string& f)
    {
      using namespace std;
      #if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
      HINSTANCE__* lib = this->loadLibrary(l);
#else
      void * lib = this->loadLibrary(l);
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */

      FortranFunction7Ptr fct = ::tfel_getFortranFunction7(lib,f.c_str());
      if(fct==0){
	string msg("ExternalLibraryManager::getFortranFunction7 : ");
	msg += " could not load function '"+f+"' (";
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
	  msg += ::GetLastError();
#else
	  msg += ::dlerror();
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */
	msg += ")";
	throw(runtime_error(msg));
      }
      return fct;
    }

    FortranFunction8Ptr
    ExternalLibraryManager::getFortranFunction8(const std::string& l,
					  const std::string& f)
    {
      using namespace std;
      #if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
      HINSTANCE__* lib = this->loadLibrary(l);
#else
      void * lib = this->loadLibrary(l);
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */

      FortranFunction8Ptr fct = ::tfel_getFortranFunction8(lib,f.c_str());
      if(fct==0){
	string msg("ExternalLibraryManager::getFortranFunction8 : ");
	msg += " could not load function '"+f+"' (";
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
	  msg += ::GetLastError();
#else
	  msg += ::dlerror();
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */
	msg += ")";
	throw(runtime_error(msg));
      }
      return fct;
    }

    FortranFunction9Ptr
    ExternalLibraryManager::getFortranFunction9(const std::string& l,
					  const std::string& f)
    {
      using namespace std;
      #if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
      HINSTANCE__* lib = this->loadLibrary(l);
#else
      void * lib = this->loadLibrary(l);
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */

      FortranFunction9Ptr fct = ::tfel_getFortranFunction9(lib,f.c_str());
      if(fct==0){
	string msg("ExternalLibraryManager::getFortranFunction9 : ");
	msg += " could not load function '"+f+"' (";
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
	  msg += ::GetLastError();
#else
	  msg += ::dlerror();
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */
	msg += ")";
	throw(runtime_error(msg));
      }
      return fct;
    }

    FortranFunction10Ptr
    ExternalLibraryManager::getFortranFunction10(const std::string& l,
					   const std::string& f)
    {
      using namespace std;
      #if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
      HINSTANCE__* lib = this->loadLibrary(l);
#else
      void * lib = this->loadLibrary(l);
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */

      FortranFunction10Ptr fct = ::tfel_getFortranFunction10(lib,f.c_str());
      if(fct==0){
	string msg("ExternalLibraryManager::getFortranFunction10 : ");
	msg += " could not load function '"+f+"' (";
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
	  msg += ::GetLastError();
#else
	  msg += ::dlerror();
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */
	msg += ")";
	throw(runtime_error(msg));
      }
      return fct;
    }

    FortranFunction11Ptr
    ExternalLibraryManager::getFortranFunction11(const std::string& l,
					   const std::string& f)
    {
      using namespace std;
      #if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
      HINSTANCE__* lib = this->loadLibrary(l);
#else
      void * lib = this->loadLibrary(l);
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */

      FortranFunction11Ptr fct = ::tfel_getFortranFunction11(lib,f.c_str());
      if(fct==0){
	string msg("ExternalLibraryManager::getFortranFunction11 : ");
	msg += " could not load function '"+f+"' (";
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
	  msg += ::GetLastError();
#else
	  msg += ::dlerror();
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */
	msg += ")";
	throw(runtime_error(msg));
      }
      return fct;
    }

    FortranFunction12Ptr
    ExternalLibraryManager::getFortranFunction12(const std::string& l,
					   const std::string& f)
    {
      using namespace std;
      #if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
      HINSTANCE__* lib = this->loadLibrary(l);
#else
      void * lib = this->loadLibrary(l);
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */

      FortranFunction12Ptr fct = ::tfel_getFortranFunction12(lib,f.c_str());
      if(fct==0){
	string msg("ExternalLibraryManager::getFortranFunction12 : ");
	msg += " could not load function '"+f+"' (";
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
	  msg += ::GetLastError();
#else
	  msg += ::dlerror();
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */
	msg += ")";
	throw(runtime_error(msg));
      }
      return fct;
    }

    FortranFunction13Ptr
    ExternalLibraryManager::getFortranFunction13(const std::string& l,
					   const std::string& f)
    {
      using namespace std;
      #if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
      HINSTANCE__* lib = this->loadLibrary(l);
#else
      void * lib = this->loadLibrary(l);
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */

      FortranFunction13Ptr fct = ::tfel_getFortranFunction13(lib,f.c_str());
      if(fct==0){
	string msg("ExternalLibraryManager::getFortranFunction13 : ");
	msg += " could not load function '"+f+"' (";
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
	  msg += ::GetLastError();
#else
	  msg += ::dlerror();
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */
	msg += ")";
	throw(runtime_error(msg));
      }
      return fct;
    }

    FortranFunction14Ptr
    ExternalLibraryManager::getFortranFunction14(const std::string& l,
					   const std::string& f)
    {
      using namespace std;
      #if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
      HINSTANCE__* lib = this->loadLibrary(l);
#else
      void * lib = this->loadLibrary(l);
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */

      FortranFunction14Ptr fct = ::tfel_getFortranFunction14(lib,f.c_str());
      if(fct==0){
	string msg("ExternalLibraryManager::getFortranFunction14 : ");
	msg += " could not load function '"+f+"' (";
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
	  msg += ::GetLastError();
#else
	  msg += ::dlerror();
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */
	msg += ")";
	throw(runtime_error(msg));
      }
      return fct;
    }

    FortranFunction15Ptr
    ExternalLibraryManager::getFortranFunction15(const std::string& l,
					   const std::string& f)
    {
      using namespace std;
      #if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
      HINSTANCE__* lib = this->loadLibrary(l);
#else
      void * lib = this->loadLibrary(l);
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */
      FortranFunction15Ptr fct = ::tfel_getFortranFunction15(lib,f.c_str());
      if(fct==0){
	string msg("ExternalLibraryManager::getFortranFunction15 : ");
	msg += " could not load function '"+f+"' (";
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
	  msg += ::GetLastError();
#else
	  msg += ::dlerror();
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */
	msg += ")";
	throw(runtime_error(msg));
      }
      return fct;
    }
    
    ExternalLibraryManager::~ExternalLibraryManager()
    {} // end of ExternalLibraryManager::~ExternalLibraryManager

  } // end of namespace system

} // end of namespace tfel
