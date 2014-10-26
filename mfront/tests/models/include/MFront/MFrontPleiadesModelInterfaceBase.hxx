/*!
 * \file   mfront/tests/models/include/MFront/MFrontPleiadesModelInterfaceBase.hxx
 * \brief  This file declares the MFrontPleiadesModelinterface class
 * \author Helfer Thomas
 * \date   10 Nov 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_MFRONTPLEIADESMODELINTERFACE_H_
#define _LIB_MFRONTPLEIADESMODELINTERFACE_H_ 

#include"MFront/AbstractModelInterface.hxx"

namespace mfront{

  struct MFrontPleiadesModelInterfaceBase
    : public AbstractModelInterface
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
    void writeOutputFiles(const GenericData&,
			  const ModelData&);
    /*!
     * \param pdata : generic data
     */
    virtual std::map<std::string,std::vector<std::string> >
    getGlobalIncludes(const ModelData&);
    /*!
     * \param pdata : generic data
     */
    virtual std::map<std::string,std::vector<std::string> >
    getGlobalDependencies(const ModelData&);
    /*!
     * \param pdata : generic data
     */
    virtual std::map<std::string,std::vector<std::string> >
    getGeneratedSources(const ModelData&);
    /*!
     * \param pdata : generic data
     */
    virtual std::vector<std::string>
    getGeneratedIncludes(const ModelData&);
    /*!
     * \param pdata : generic data
     */
    virtual std::map<std::string,std::vector<std::string> >
    getLibrariesDependencies(const ModelData&);

  protected:

    virtual void
    openOutputFiles(void);

    virtual void
    closeOutputFiles(void);

    virtual void
    generateOutputFiles(const GenericData&,
			const ModelData&);
    
    virtual void
    writeHeaderFile(const GenericData&,
		    const ModelData&);

    virtual void
    writeSpecificPrivateMethodDeclaration(const GenericData&,
					  const ModelData&);
    
    virtual void
    writeSrcFile(const GenericData&,
		 const ModelData&);

    virtual void
    writeAssignDefaultValue(const VarContainer::const_iterator,
			    const std::map<std::string,std::string>::const_iterator);

    virtual std::string
    getGenTypeMethod(const std::string& type) const;

    virtual void
    setOutputFileNames(const ModelData&) = 0;

    virtual void
    writeGetGlobalParameter(const VarHandler&,
			    const ModelData&);

    virtual void
    writeGetConstantMaterialProperty(const VarHandler&,
				     const ModelData&);

    virtual void
    buildDomainName(void) = 0;

    virtual std::string
    getApplicationName(void) const = 0;

    virtual void
    generateDomainsList(const ModelData&);

    virtual void
    writeFunctionCall(const ModelData&,
		      const std::vector<ModelData::Function>&);

    virtual void
    writeInitializeMethod(const ModelData&);

    virtual void
    writeInitializeOutputMethod(const ModelData&);

    virtual void
    writeInitializeInputMethod(const ModelData&);

    virtual void
    writeInitializeParametersMethod(const ModelData&);

    virtual bool
    initializeDefaultDomainListInConstrutor(const ModelData&) const;

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

