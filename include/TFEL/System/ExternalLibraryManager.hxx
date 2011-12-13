/*!
 * \file   ExternalLibraryManager.hxx
 * \brief    
 * \author Helfer Thomas
 * \date   31 Oct 2007
 */

#ifndef _LIB_EXTERNALLIBRARYMANAGER_H_
#define _LIB_EXTERNALLIBRARYMANAGER_H_ 

#include<map>
#include<vector>
#include<string>

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/System/ExternalFunctionsPrototypes.hxx"

namespace tfel
{

  namespace system
  {

    struct TFEL_VISIBILITY_EXPORT ExternalLibraryManager
    {

      static ExternalLibraryManager&
      getExternalLibraryManager();

      /*!
       * \brief a wrapper around the ::dlopen system call
       * \param[in] name : name of the library
       * \param[in] b : boolean allowing ::dlopen to fail. If ::dlopen
       * fails, a null pointer is returned. This library is *not*
       * registred.
       * \return a pointer to the library
       * \note on success, the pointer is registred in a map using its
       * name as a key. This name is used in the methods of this call 
       */
      void *
      loadLibrary(const std::string&,
		  const bool = false);

      /*!
       * \note the loadLibrary must have already been called with this name
       */
      bool
      contains(const std::string&,
	       const std::string&);

      /*!
       * \note the loadLibrary must have already been called with this name
       */
      bool
      isUMATBehaviourUsableInPurelyImplicitResolution(const std::string&,
						      const std::string&);
      
      /*!
       * \note the loadLibrary must have already been called with this name
       */
      std::vector<std::string>
      getUMATMaterialPropertiesNames(const std::string&,
				     const std::string&);

      /*!
       * \note the loadLibrary must have already been called with this name
       */
      std::vector<std::string>
      getUMATInternalStateVariablesNames(const std::string&,
					 const std::string&);

      /*!
       * \note the loadLibrary must have already been called with this name
       */
      std::vector<int>
      getUMATInternalStateVariablesTypes(const std::string& l,
					 const std::string& f);

      /*!
       * \note the loadLibrary must have already been called with this name
       */
      std::vector<std::string>
      getUMATExternalStateVariablesNames(const std::string&,
					 const std::string&);

      /*!
       * \note the loadLibrary must have already been called with this name
       */
      unsigned short
      getCastemFunctionNumberOfVariables(const std::string&,
					 const std::string&);

      /*!
       * \note the loadLibrary must have already been called with this name
       */
      std::vector<std::string>
      getCastemFunctionVariables(const std::string&,
				 const std::string&);

      /*!
       * \note the loadLibrary must have already been called with this name
       */
      void
      getCastemFunctionVariables(std::vector<std::string>&,
				 const std::string&,
				 const std::string&);
  
      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       * \note the loadLibrary must have already been called with this name
       */
      CastemFunctionPtr
      getCastemFunction(const std::string&,
			const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       * \note the loadLibrary must have already been called with this name
       */
      CFunction0Ptr
      getCFunction0(const std::string&,
		    const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       * \note the loadLibrary must have already been called with this name
       */
      CFunction1Ptr
      getCFunction1(const std::string&,
		    const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       * \note the loadLibrary must have already been called with this name
       */
      CFunction2Ptr
      getCFunction2(const std::string&,
		    const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       * \note the loadLibrary must have already been called with this name
       */
      CFunction3Ptr
      getCFunction3(const std::string&,
		    const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       * \note the loadLibrary must have already been called with this name
       */
      CFunction4Ptr
      getCFunction4(const std::string&,
		    const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       * \note the loadLibrary must have already been called with this name
       */
      CFunction5Ptr
      getCFunction5(const std::string&,
		    const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       * \note the loadLibrary must have already been called with this name
       */
      CFunction6Ptr
      getCFunction6(const std::string&,
		    const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       * \note the loadLibrary must have already been called with this name
       */
      CFunction7Ptr
      getCFunction7(const std::string&,
		    const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       * \note the loadLibrary must have already been called with this name
       */
      CFunction8Ptr
      getCFunction8(const std::string&,
		    const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       * \note the loadLibrary must have already been called with this name
       */
      CFunction9Ptr
      getCFunction9(const std::string&,
		    const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       * \note the loadLibrary must have already been called with this name
       */
      CFunction10Ptr
      getCFunction10(const std::string&,
		     const std::string&);
      
      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       * \note the loadLibrary must have already been called with this name
       */
      CFunction11Ptr
      getCFunction11(const std::string&,
		     const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       * \note the loadLibrary must have already been called with this name
       */
      CFunction12Ptr
      getCFunction12(const std::string&,
		     const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       * \note the loadLibrary must have already been called with this name
       */
      CFunction13Ptr
      getCFunction13(const std::string&,
		     const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       * \note the loadLibrary must have already been called with this name
       */
      CFunction14Ptr
      getCFunction14(const std::string&,
		     const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       * \note the loadLibrary must have already been called with this name
       */
      CFunction15Ptr
      getCFunction15(const std::string&,
		     const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       * \note the loadLibrary must have already been called with this name
       */
      FortranFunction0Ptr
      getFortranFunction0(const std::string&,
			  const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       * \note the loadLibrary must have already been called with this name
       */
      FortranFunction1Ptr
      getFortranFunction1(const std::string&,
			  const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       * \note the loadLibrary must have already been called with this name
       */
      FortranFunction2Ptr
      getFortranFunction2(const std::string&,
			  const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       * \note the loadLibrary must have already been called with this name
       */
      FortranFunction3Ptr
      getFortranFunction3(const std::string&,
			  const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       * \note the loadLibrary must have already been called with this name
       */
      FortranFunction4Ptr
      getFortranFunction4(const std::string&,
			  const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       * \note the loadLibrary must have already been called with this name
       */
      FortranFunction5Ptr
      getFortranFunction5(const std::string&,
			  const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       * \note the loadLibrary must have already been called with this name
       */
      FortranFunction6Ptr
      getFortranFunction6(const std::string&,
			  const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       * \note the loadLibrary must have already been called with this name
       */
      FortranFunction7Ptr
      getFortranFunction7(const std::string&,
			  const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       * \note the loadLibrary must have already been called with this name
       */
      FortranFunction8Ptr
      getFortranFunction8(const std::string&,
			  const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       * \note the loadLibrary must have already been called with this name
       */
      FortranFunction9Ptr
      getFortranFunction9(const std::string&,
			  const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       * \note the loadLibrary must have already been called with this name
       */
      FortranFunction10Ptr
      getFortranFunction10(const std::string&,
			   const std::string&);
      
      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       * \note the loadLibrary must have already been called with this name
       */
      FortranFunction11Ptr
      getFortranFunction11(const std::string&,
			   const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       * \note the loadLibrary must have already been called with this name
       */
      FortranFunction12Ptr
      getFortranFunction12(const std::string&,
			   const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       * \note the loadLibrary must have already been called with this name
       */
      FortranFunction13Ptr
      getFortranFunction13(const std::string&,
			   const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       * \note the loadLibrary must have already been called with this name
       */
      FortranFunction14Ptr
      getFortranFunction14(const std::string&,
			   const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       * \note the loadLibrary must have already been called with this name
       */
      FortranFunction15Ptr
      getFortranFunction15(const std::string&,
			   const std::string&);

      ~ExternalLibraryManager();

    private:

      TFEL_VISIBILITY_LOCAL ExternalLibraryManager();

      TFEL_VISIBILITY_LOCAL ExternalLibraryManager(const ExternalLibraryManager&);

      TFEL_VISIBILITY_LOCAL ExternalLibraryManager&
      operator=(const ExternalLibraryManager&);

      TFEL_VISIBILITY_LOCAL void
      getUMATNames(std::vector<std::string>&,
		   const std::string&,
		   const std::string&,
		   const std::string&);

      std::map<std::string,void *> librairies;

    }; // end of struct LibraryManager
    
  } // end of namespace system

} // end of namespace tfel

#endif /* _LIB_EXTERNALLIBRARYMANAGER_H */
