/*!
 * \file   mfront/tests/models/include/MFront/MFrontPleiadesModelInterfaceBase.hxx
 * \brief  This file declares the MFrontPleiadesModelinterface class
 * \author Helfer Thomas
 * \date   10 Nov 2006
 */

#ifndef _LIB_MFRONTPLEIADESMODELINTERFACE_H_
#define _LIB_MFRONTPLEIADESMODELINTERFACE_H_ 

#include"MFront/MFrontModelVirtualInterface.hxx"

namespace mfront{

  struct MFrontPleiadesModelInterfaceBase
    : public MFrontModelVirtualInterface
  {

    MFrontPleiadesModelInterfaceBase();

    virtual void setVerboseMode();

    virtual void setDebugMode();

    virtual void setWarningMode();

    virtual std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
    treatKeyword(const std::string&,
		 tfel::utilities::CxxTokenizer::TokensContainer::const_iterator,
		 const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator);

    virtual void
    reset(void);

    virtual
    void declareReservedNames(std::set<std::string>&);

    /*!
     * \param pdata : processing data
     * \param data  : model data
     */
    virtual
    void writeOutputFiles(const MFrontGenericData&,
			  const MFrontModelData&);
    /*!
     * \param pdata : generic data
     */
    virtual std::map<std::string,std::vector<std::string> >
    getGlobalIncludes(const MFrontModelData&);
    /*!
     * \param pdata : generic data
     */
    virtual std::map<std::string,std::vector<std::string> >
    getGlobalDependencies(const MFrontModelData&);
    /*!
     * \param pdata : generic data
     */
    virtual std::map<std::string,std::vector<std::string> >
    getGeneratedSources(const MFrontModelData&);
    /*!
     * \param pdata : generic data
     */
    virtual std::vector<std::string>
    getGeneratedIncludes(const MFrontModelData&);
    /*!
     * \param pdata : generic data
     */
    virtual std::map<std::string,std::vector<std::string> >
    getLibrariesDependencies(const MFrontModelData&);

  protected:

    virtual void
    openOutputFiles(void);

    virtual void
    closeOutputFiles(void);

    virtual void
    generateOutputFiles(const MFrontGenericData&,
			const MFrontModelData&);
    
    virtual void
    writeHeaderFile(const MFrontGenericData&,
		    const MFrontModelData&);

    virtual void
    writeSpecificPrivateMethodDeclaration(const MFrontGenericData&,
					  const MFrontModelData&);
    
    virtual void
    writeSrcFile(const MFrontGenericData&,
		 const MFrontModelData&);

    virtual void
    writeAssignDefaultValue(const VarContainer::const_iterator,
			    const std::map<std::string,std::string>::const_iterator);

    virtual std::string
    getGenTypeMethod(const std::string& type) const;

    virtual void
    setOutputFileNames(const MFrontModelData&) = 0;

    virtual void
    writeGetGlobalParameter(const VarHandler&,
			    const MFrontModelData&);

    virtual void
    writeGetConstantMaterialProperty(const VarHandler&,
				     const MFrontModelData&);

    virtual void
    buildDomainName(void) = 0;

    virtual std::string
    getApplicationName(void) const = 0;

    virtual void
    generateDomainsList(const MFrontModelData&);

    virtual void
    writeFunctionCall(const MFrontModelData&,
		      const std::vector<MFrontModelData::Function>&);

    virtual void
    writeInitializeMethod(const MFrontModelData&);

    virtual void
    writeInitializeOutputMethod(const MFrontModelData&);

    virtual void
    writeInitializeInputMethod(const MFrontModelData&);

    virtual void
    writeInitializeParametersMethod(const MFrontModelData&);

    virtual bool
    initializeDefaultDomainListInConstrutor(const MFrontModelData&) const;

    std::set<std::string> domains;

    std::ofstream headerFile;
    std::ofstream srcFile;
    std::string headerFileName;
    std::string srcFileName;

    bool hasDefaultConstructor;
    bool hasSpecializedDomain;
    bool verboseMode;
    bool debugMode;
    bool warningMode;
  }; // end of class MFrontPleiadesModelInterface

} // end of namespace mfront  

#endif /* _LIB_MFRONTPLEIADESMODELINTERFACE_H */

