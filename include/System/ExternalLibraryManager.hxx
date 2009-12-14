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

namespace tfel
{

  namespace system
  {

    struct ExternalLibraryManager
    {

      static ExternalLibraryManager&
      getExternalLibraryManager();

      void *
      loadLibrary(const std::string&);

      bool
      contains(const std::string&,
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
  
      double (*getCastemFunction(const std::string&,const std::string&))(const double*const);

      double (*getCFunction0(const std::string&,const std::string&))(void);

      double (*getCFunction1(const std::string&,const std::string&))(double);

      double (*getCFunction2(const std::string&,const std::string&))(double,double);

      double (*getCFunction3(const std::string&,const std::string&))(double,double,
								     double);

      double (*getCFunction4(const std::string&,const std::string&))(double,double,
								     double,double);

      double (*getCFunction5(const std::string&,const std::string&))(double,double,
								     double,double,
								     double);

      double (*getCFunction6(const std::string&,const std::string&))(double,double,
								     double,double,
								     double,double);

      double (*getCFunction7(const std::string&,const std::string&))(double,double,
								     double,double,
								     double,double,
								     double);

      double (*getCFunction8(const std::string&,const std::string&))(double,double,
								     double,double,
								     double,double,
								     double,double);

      double (*getCFunction9(const std::string&,const std::string&))(double,double,
								     double,double,
								     double,double,
								     double,double,
								     double);

      double (*getCFunction10(const std::string&,const std::string&))(double,double,
								      double,double,
								      double,double,
								      double,double,
								      double,double);

      double (*getCFunction11(const std::string&,const std::string&))(double,double,
								      double,double,
								      double,double,
								      double,double,
								      double,double,
								      double);

      double (*getCFunction12(const std::string&,const std::string&))(double,double,
								      double,double,
								      double,double,
								      double,double,
								      double,double,
								      double,double);

      double (*getCFunction13(const std::string&,const std::string&))(double,double,
								      double,double,
								      double,double,
								      double,double,
								      double,double,
								      double,double,
								      double);

      double (*getCFunction14(const std::string&,const std::string&))(double,double,
								      double,double,
								      double,double,
								      double,double,
								      double,double,
								      double,double,
								      double,double);

      double (*getCFunction15(const std::string&,const std::string&))(double,double,
								      double,double,
								      double,double,
								      double,double,
								      double,double,
								      double,double,
								      double,double,
								      double);

      double (*getFortranFunction0(const std::string&,const std::string&))(void);

      double (*getFortranFunction1(const std::string&,const std::string&))(const double*);

      double (*getFortranFunction2(const std::string&,const std::string&))(const double*,const double*);

      double (*getFortranFunction3(const std::string&,const std::string&))(const double*,const double*,
									   const double*);

      double (*getFortranFunction4(const std::string&,const std::string&))(const double*,const double*,
									   const double*,const double*);

      double (*getFortranFunction5(const std::string&,const std::string&))(const double*,const double*,
									   const double*,const double*,
									   const double*);

      double (*getFortranFunction6(const std::string&,const std::string&))(const double*,const double*,
									   const double*,const double*,
									   const double*,const double*);

      double (*getFortranFunction7(const std::string&,const std::string&))(const double*,const double*,
									   const double*,const double*,
									   const double*,const double*,
									   const double*);

      double (*getFortranFunction8(const std::string&,const std::string&))(const double*,const double*,
									   const double*,const double*,
									   const double*,const double*,
									   const double*,const double*);

      double (*getFortranFunction9(const std::string&,const std::string&))(const double*,const double*,
									   const double*,const double*,
									   const double*,const double*,
									   const double*,const double*,
									   const double*);

      double (*getFortranFunction10(const std::string&,const std::string&))(const double*,const double*,
									    const double*,const double*,
									    const double*,const double*,
									    const double*,const double*,
									    const double*,const double*);

      double (*getFortranFunction11(const std::string&,const std::string&))(const double*,const double*,
									    const double*,const double*,
									    const double*,const double*,
									    const double*,const double*,
									    const double*,const double*,
									    const double*);

      double (*getFortranFunction12(const std::string&,const std::string&))(const double*,const double*,
									    const double*,const double*,
									    const double*,const double*,
									    const double*,const double*,
									    const double*,const double*,
									    const double*,const double*);

      double (*getFortranFunction13(const std::string&,const std::string&))(const double*,const double*,
									    const double*,const double*,
									    const double*,const double*,
									    const double*,const double*,
									    const double*,const double*,
									    const double*,const double*,
									    const double*);

      double (*getFortranFunction14(const std::string&,const std::string&))(const double*,const double*,
									    const double*,const double*,
									    const double*,const double*,
									    const double*,const double*,
									    const double*,const double*,
									    const double*,const double*,
									    const double*,const double*);

      double (*getFortranFunction15(const std::string&,const std::string&))(const double*,const double*,
									    const double*,const double*,
									    const double*,const double*,
									    const double*,const double*,
									    const double*,const double*,
									    const double*,const double*,
									    const double*,const double*,
									    const double*);

      ~ExternalLibraryManager();

    private:

      ExternalLibraryManager();

      ExternalLibraryManager(const ExternalLibraryManager&);

      ExternalLibraryManager&
      operator=(const ExternalLibraryManager&);

      void
      getUMATNames(std::vector<std::string>&,
		   const std::string&,
		   const std::string&,
		   const std::string&);

      std::map<std::string,void *> librairies;

    }; // end of struct LibraryManager
    
  } // end of namespace system

} // end of namespace tfel

#endif /* _LIB_EXTERNALLIBRARYMANAGER_H */
