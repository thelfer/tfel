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

      void *
      loadLibrary(const std::string&);

      bool
      contains(const std::string&,
	       const std::string&);

      bool
      isUMATBehaviourUsableInPurelyImplicitResolution(const std::string&,
						      const std::string&);
      
      std::vector<std::string>
      getUMATMaterialPropertiesNames(const std::string&,
				     const std::string&);

      std::vector<std::string>
      getUMATInternalStateVariablesNames(const std::string&,
					 const std::string&);

      std::vector<int>
      getUMATInternalStateVariablesTypes(const std::string& l,
					 const std::string& f);

      std::vector<std::string>
      getUMATExternalStateVariablesNames(const std::string&,
					 const std::string&);

      unsigned short
      getCastemFunctionNumberOfVariables(const std::string&,
					 const std::string&);

      std::vector<std::string>
      getCastemFunctionVariables(const std::string&,
				 const std::string&);

      void
      getCastemFunctionVariables(std::vector<std::string>&,
				 const std::string&,
				 const std::string&);
  
      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       */
      CastemFunctionPtr
      getCastemFunction(const std::string&,
			const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       */
      CFunction0Ptr
      getCFunction0(const std::string&,
		    const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       */
      CFunction1Ptr
      getCFunction1(const std::string&,
		    const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       */
      CFunction2Ptr
      getCFunction2(const std::string&,
		    const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       */
      CFunction3Ptr
      getCFunction3(const std::string&,
		    const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       */
      CFunction4Ptr
      getCFunction4(const std::string&,
		    const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       */
      CFunction5Ptr
      getCFunction5(const std::string&,
		    const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       */
      CFunction6Ptr
      getCFunction6(const std::string&,
		    const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       */
      CFunction7Ptr
      getCFunction7(const std::string&,
		    const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       */
      CFunction8Ptr
      getCFunction8(const std::string&,
		    const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       */
      CFunction9Ptr
      getCFunction9(const std::string&,
		    const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       */
      CFunction10Ptr
      getCFunction10(const std::string&,
		     const std::string&);
      
      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       */
      CFunction11Ptr
      getCFunction11(const std::string&,
		     const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       */
      CFunction12Ptr
      getCFunction12(const std::string&,
		     const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       */
      CFunction13Ptr
      getCFunction13(const std::string&,
		     const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       */
      CFunction14Ptr
      getCFunction14(const std::string&,
		     const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       */
      CFunction15Ptr
      getCFunction15(const std::string&,
		     const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       */
      FortranFunction0Ptr
      getFortranFunction0(const std::string&,
			  const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       */
      FortranFunction1Ptr
      getFortranFunction1(const std::string&,
			  const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       */
      FortranFunction2Ptr
      getFortranFunction2(const std::string&,
			  const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       */
      FortranFunction3Ptr
      getFortranFunction3(const std::string&,
			  const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       */
      FortranFunction4Ptr
      getFortranFunction4(const std::string&,
			  const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       */
      FortranFunction5Ptr
      getFortranFunction5(const std::string&,
			  const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       */
      FortranFunction6Ptr
      getFortranFunction6(const std::string&,
			  const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       */
      FortranFunction7Ptr
      getFortranFunction7(const std::string&,
			  const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       */
      FortranFunction8Ptr
      getFortranFunction8(const std::string&,
			  const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       */
      FortranFunction9Ptr
      getFortranFunction9(const std::string&,
			  const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       */
      FortranFunction10Ptr
      getFortranFunction10(const std::string&,
			   const std::string&);
      
      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       */
      FortranFunction11Ptr
      getFortranFunction11(const std::string&,
			   const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       */
      FortranFunction12Ptr
      getFortranFunction12(const std::string&,
			   const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       */
      FortranFunction13Ptr
      getFortranFunction13(const std::string&,
			   const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       */
      FortranFunction14Ptr
      getFortranFunction14(const std::string&,
			   const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
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
