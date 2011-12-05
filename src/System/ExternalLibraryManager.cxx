/*!
 * \file   ExternalLibraryManager.cpp
 * \brief    
 * \author Helfer Thomas
 * \date   31 Oct 2007
 */

#include<stdexcept>
#include<dlfcn.h>

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

    void *
    ExternalLibraryManager::loadLibrary(const std::string& name)
    {
      using namespace std;
      typedef map<string,void *>::value_type MVType;
      map<string,void *>::const_iterator p;
      p=this->librairies.find(name);
      if(p==librairies.end()){
	// this library has not been 
	void * lib;
	lib = ::dlopen(name.c_str(),RTLD_NOW);
	if(lib==0){
	  string msg("ExternalLibraryManager::loadLibrary : library '");
	  msg += name;
	  msg += "' could not be loaded, (";
	  msg += dlerror();
	  msg += ")";
	  throw(runtime_error(msg));
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
      void * lib = this->loadLibrary(l);
      void * p   = ::dlsym(lib,s.c_str());
      return p!=static_cast<void *>(0);
    } // end of ExternalLibraryManager::contains
        
    unsigned short
    ExternalLibraryManager::getCastemFunctionNumberOfVariables(const std::string& l,
							       const std::string& f)
    {
      using namespace std;
      int res;
      void * lib = this->loadLibrary(l);
      res = ::tfel_getCastemFunctionNumberOfVariables(lib,f.c_str());
      if(res<0){
	string msg("ExternalLibraryManager::getCastemFunctionNumberOfVariables : ");
	msg += " number of variables could not be read (";
	msg += dlerror();
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
      void * lib = this->loadLibrary(l);
      unsigned short nb = this->getCastemFunctionNumberOfVariables(l,f);
      char ** res = ::tfel_getCastemFunctionVariables(lib,f.c_str());
      char **p;
      if(res==0){
	string msg("ExternalLibraryManager::getCastemFunctionNumberOfVariables : ");
	msg += " variables names could not be read (";
	msg += dlerror();
	msg += ")";
	throw(runtime_error(msg));
      }
      for(p=res;p!=res+nb;++p){
	vars.push_back(*p);
      }
    } // end of ExternalLibraryManager::getCastemFunctionVariables

    void
    ExternalLibraryManager::getUMATNames(std::vector<std::string>& vars,
					 const std::string& l,
					 const std::string& f,
					 const std::string& n)
    {
      using namespace std;
      void * lib = this->loadLibrary(l);
      int nb = ::tfel_getUnsignedShort(lib,(f+"_n"+n).c_str());
      char ** res;
      if(nb==-1){
	string msg("ExternalLibraryManager::getUMATNames : ");
	msg += " number of variables names could not be read (";
	msg += dlerror();
	msg += ")";
	throw(runtime_error(msg));
      }
      res = ::tfel_getArrayOfStrings(lib,(f+'_'+n).c_str());
      if(res==0){
	string msg("ExternalLibraryManager::getUMATNames : ");
	msg += "variables names could not be read (";
	msg += dlerror();
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
      void * lib = this->loadLibrary(l);
      int b = ::tfel_getBool(lib,(f+"_UsableInPurelyImplicitResolution").c_str());
      if(b==-1){
	return false;
      }
      if(b==1){
	return true;
      }
      return false;
    } // end of ExternalLibraryManager::isUMATBehaviourUsableInPurelyImplicitResolution

    std::vector<int>
    ExternalLibraryManager::getUMATInternalStateVariablesTypes(const std::string& l,
							       const std::string& f)
    {
      using namespace std;
      vector<int> types;
      void * lib = this->loadLibrary(l);
      int nb = ::tfel_getUnsignedShort(lib,(f+"_nInternalStateVariables").c_str());
      int * res;
      if(nb==-1){
	string msg("ExternalLibraryManager::getUMATInternalStateVariablesTypes : ");
	msg += " number of variables names could not be read (";
	msg += dlerror();
	msg += ")";
	throw(runtime_error(msg));
      }
      res = ::tfel_getArrayOfInt(lib,(f+"_InternalStateVariablesTypes").c_str());
      if(res==0){
	string msg("ExternalLibraryManager::getUMATInternalStateVariablesTypes : ");
	msg += "internal state variables types could not be read (";
	msg += dlerror();
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
      void * lib = this->loadLibrary(l);
      CastemFunctionPtr fct = ::tfel_getCastemFunction(lib,f.c_str());
      if(fct==0){
	string msg("ExternalLibraryManager::getCastemFunction : ");
	msg += " could not load castem function '"+f+"' (";
	msg += dlerror();
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
      void * lib = this->loadLibrary(l);
      CFunction0Ptr fct = ::tfel_getCFunction0(lib,f.c_str());
      if(fct==0){
	string msg("ExternalLibraryManager::getCFunction0 : ");
	msg += " could not load function '"+f+"' (";
	msg += dlerror();
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
      void * lib = this->loadLibrary(l);
      CFunction1Ptr fct = ::tfel_getCFunction1(lib,f.c_str());
      if(fct==0){
	string msg("ExternalLibraryManager::getCFunction1 : ");
	msg += " could not load function '"+f+"' (";
	msg += dlerror();
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
      void * lib = this->loadLibrary(l);
      CFunction2Ptr fct = ::tfel_getCFunction2(lib,f.c_str());
      if(fct==0){
	string msg("ExternalLibraryManager::getCFunction2 : ");
	msg += " could not load function '"+f+"' (";
	msg += dlerror();
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
      void * lib = this->loadLibrary(l);
      CFunction3Ptr fct = ::tfel_getCFunction3(lib,f.c_str());
      if(fct==0){
	string msg("ExternalLibraryManager::getCFunction3 : ");
	msg += " could not load function '"+f+"' (";
	msg += dlerror();
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
      void * lib = this->loadLibrary(l);
      CFunction4Ptr fct  = ::tfel_getCFunction4(lib,f.c_str());
      if(fct==0){
	string msg("ExternalLibraryManager::getCFunction4 : ");
	msg += " could not load function '"+f+"' (";
	msg += dlerror();
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
      void * lib = this->loadLibrary(l);
      CFunction5Ptr fct = ::tfel_getCFunction5(lib,f.c_str());
      if(fct==0){
	string msg("ExternalLibraryManager::getCFunction5 : ");
	msg += " could not load function '"+f+"' (";
	msg += dlerror();
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
      void * lib = this->loadLibrary(l);
      CFunction6Ptr fct = ::tfel_getCFunction6(lib,f.c_str());
      if(fct==0){
	string msg("ExternalLibraryManager::getCFunction6 : ");
	msg += " could not load function '"+f+"' (";
	msg += dlerror();
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
      void * lib = this->loadLibrary(l);
      CFunction7Ptr fct = ::tfel_getCFunction7(lib,f.c_str());
      if(fct==0){
	string msg("ExternalLibraryManager::getCFunction7 : ");
	msg += " could not load function '"+f+"' (";
	msg += dlerror();
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
      void * lib = this->loadLibrary(l);
      CFunction8Ptr fct = ::tfel_getCFunction8(lib,f.c_str());
      if(fct==0){
	string msg("ExternalLibraryManager::getCFunction8 : ");
	msg += " could not load function '"+f+"' (";
	msg += dlerror();
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
      void * lib = this->loadLibrary(l);
      CFunction9Ptr fct = ::tfel_getCFunction9(lib,f.c_str());
      if(fct==0){
	string msg("ExternalLibraryManager::getCFunction9 : ");
	msg += " could not load function '"+f+"' (";
	msg += dlerror();
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
      void * lib = this->loadLibrary(l);
      CFunction10Ptr fct = ::tfel_getCFunction10(lib,f.c_str());
      if(fct==0){
	string msg("ExternalLibraryManager::getCFunction10 : ");
	msg += " could not load function '"+f+"' (";
	msg += dlerror();
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
      void * lib = this->loadLibrary(l);
      CFunction11Ptr fct = ::tfel_getCFunction11(lib,f.c_str());
      if(fct==0){
	string msg("ExternalLibraryManager::getCFunction11 : ");
	msg += " could not load function '"+f+"' (";
	msg += dlerror();
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
      void * lib = this->loadLibrary(l);
      CFunction12Ptr fct = ::tfel_getCFunction12(lib,f.c_str());
      if(fct==0){
	string msg("ExternalLibraryManager::getCFunction12 : ");
	msg += " could not load function '"+f+"' (";
	msg += dlerror();
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
      void * lib = this->loadLibrary(l);
      CFunction13Ptr fct = ::tfel_getCFunction13(lib,f.c_str());
      if(fct==0){
	string msg("ExternalLibraryManager::getCFunction13 : ");
	msg += " could not load function '"+f+"' (";
	msg += dlerror();
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
      void * lib = this->loadLibrary(l);
      CFunction14Ptr fct = ::tfel_getCFunction14(lib,f.c_str());
      if(fct==0){
	string msg("ExternalLibraryManager::getCFunction14 : ");
	msg += " could not load function '"+f+"' (";
	msg += dlerror();
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
      void * lib = this->loadLibrary(l);
      CFunction15Ptr fct = ::tfel_getCFunction15(lib,f.c_str());
      if(fct==0){
	string msg("ExternalLibraryManager::getCFunction15 : ");
	msg += " could not load function '"+f+"' (";
	msg += dlerror();
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
      void * lib = this->loadLibrary(l);
      FortranFunction0Ptr fct = ::tfel_getFortranFunction0(lib,f.c_str());
      if(fct==0){
	string msg("ExternalLibraryManager::getFortranFunction0 : ");
	msg += " could not load function '"+f+"' (";
	msg += dlerror();
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
      void * lib = this->loadLibrary(l);
      FortranFunction1Ptr fct = ::tfel_getFortranFunction1(lib,f.c_str());
      if(fct==0){
	string msg("ExternalLibraryManager::getFortranFunction1 : ");
	msg += " could not load function '"+f+"' (";
	msg += dlerror();
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
      void * lib = this->loadLibrary(l);
      FortranFunction2Ptr fct = ::tfel_getFortranFunction2(lib,f.c_str());
      if(fct==0){
	string msg("ExternalLibraryManager::getFortranFunction2 : ");
	msg += " could not load function '"+f+"' (";
	msg += dlerror();
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
      void * lib = this->loadLibrary(l);
      FortranFunction3Ptr fct = ::tfel_getFortranFunction3(lib,f.c_str());
      if(fct==0){
	string msg("ExternalLibraryManager::getFortranFunction3 : ");
	msg += " could not load function '"+f+"' (";
	msg += dlerror();
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
      void * lib = this->loadLibrary(l);
      FortranFunction4Ptr fct  = ::tfel_getFortranFunction4(lib,f.c_str());
      if(fct==0){
	string msg("ExternalLibraryManager::getFortranFunction4 : ");
	msg += " could not load function '"+f+"' (";
	msg += dlerror();
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
      void * lib = this->loadLibrary(l);
      FortranFunction5Ptr fct = ::tfel_getFortranFunction5(lib,f.c_str());
      if(fct==0){
	string msg("ExternalLibraryManager::getFortranFunction5 : ");
	msg += " could not load function '"+f+"' (";
	msg += dlerror();
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
      void * lib = this->loadLibrary(l);
      FortranFunction6Ptr fct = ::tfel_getFortranFunction6(lib,f.c_str());
      if(fct==0){
	string msg("ExternalLibraryManager::getFortranFunction6 : ");
	msg += " could not load function '"+f+"' (";
	msg += dlerror();
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
      void * lib = this->loadLibrary(l);
      FortranFunction7Ptr fct = ::tfel_getFortranFunction7(lib,f.c_str());
      if(fct==0){
	string msg("ExternalLibraryManager::getFortranFunction7 : ");
	msg += " could not load function '"+f+"' (";
	msg += dlerror();
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
      void * lib = this->loadLibrary(l);
      FortranFunction8Ptr fct = ::tfel_getFortranFunction8(lib,f.c_str());
      if(fct==0){
	string msg("ExternalLibraryManager::getFortranFunction8 : ");
	msg += " could not load function '"+f+"' (";
	msg += dlerror();
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
      void * lib = this->loadLibrary(l);
      FortranFunction9Ptr fct = ::tfel_getFortranFunction9(lib,f.c_str());
      if(fct==0){
	string msg("ExternalLibraryManager::getFortranFunction9 : ");
	msg += " could not load function '"+f+"' (";
	msg += dlerror();
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
      void * lib = this->loadLibrary(l);
      FortranFunction10Ptr fct = ::tfel_getFortranFunction10(lib,f.c_str());
      if(fct==0){
	string msg("ExternalLibraryManager::getFortranFunction10 : ");
	msg += " could not load function '"+f+"' (";
	msg += dlerror();
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
      void * lib = this->loadLibrary(l);
      FortranFunction11Ptr fct = ::tfel_getFortranFunction11(lib,f.c_str());
      if(fct==0){
	string msg("ExternalLibraryManager::getFortranFunction11 : ");
	msg += " could not load function '"+f+"' (";
	msg += dlerror();
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
      void * lib = this->loadLibrary(l);
      FortranFunction12Ptr fct = ::tfel_getFortranFunction12(lib,f.c_str());
      if(fct==0){
	string msg("ExternalLibraryManager::getFortranFunction12 : ");
	msg += " could not load function '"+f+"' (";
	msg += dlerror();
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
      void * lib = this->loadLibrary(l);
      FortranFunction13Ptr fct = ::tfel_getFortranFunction13(lib,f.c_str());
      if(fct==0){
	string msg("ExternalLibraryManager::getFortranFunction13 : ");
	msg += " could not load function '"+f+"' (";
	msg += dlerror();
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
      void * lib = this->loadLibrary(l);
      FortranFunction14Ptr fct = ::tfel_getFortranFunction14(lib,f.c_str());
      if(fct==0){
	string msg("ExternalLibraryManager::getFortranFunction14 : ");
	msg += " could not load function '"+f+"' (";
	msg += dlerror();
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
      void * lib = this->loadLibrary(l);
      FortranFunction15Ptr fct = ::tfel_getFortranFunction15(lib,f.c_str());
      if(fct==0){
	string msg("ExternalLibraryManager::getFortranFunction15 : ");
	msg += " could not load function '"+f+"' (";
	msg += dlerror();
	msg += ")";
	throw(runtime_error(msg));
      }
      return fct;
    }
    
    ExternalLibraryManager::~ExternalLibraryManager()
    {} // end of ExternalLibraryManager::~ExternalLibraryManager

  } // end of namespace system

} // end of namespace tfel
