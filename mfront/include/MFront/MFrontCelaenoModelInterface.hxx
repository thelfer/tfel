/*!
 * \file   MFrontCelaenoModelInterface.hxx
 * \brief  This file declares the MFrontCelaenoModelInterface class
 * \author Helfer Thomas
 * \date   10 Nov 2006
 */

#ifndef _LIB_MFRONTCELAENOMODELINTERFACE_H_
#define _LIB_MFRONTCELAENOMODELINTERFACE_H_ 

#include"MFront/MFrontModelVirtualInterface.hxx"

namespace mfront{

  struct MFrontCelaenoModelInterface
    : public MFrontModelVirtualInterface
  {
    static std::string 
    getName(void);

    MFrontCelaenoModelInterface();

    virtual void setVerboseMode();

    virtual void setDebugMode();

    virtual void setWarningMode();

    virtual std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
    treatKeyword(const std::string&,
		 tfel::utilities::CxxTokenizer::TokensContainer::const_iterator,
		 const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator);

    virtual
    void reset(void);

    virtual
    void declareReservedNames(std::set<std::string>&);

    /*!
     * \param pdata : processing data
     * \param data  : model data
     */
    virtual
    void writeOutputFiles(const MFrontGenericData&,
			  const MFrontModelData&) ;

    /*!
     * \param pdata : processing data
     */
    virtual std::map<std::string,std::vector<std::string> >
    getGlobalIncludes(const MFrontGenericData&);

    /*!
     * \param pdata : processing data
     */
    virtual std::map<std::string,std::vector<std::string> >
    getGlobalDependencies(const MFrontGenericData&);

    /*!
     * \param pdata : processing data
     */
    virtual std::map<std::string,std::vector<std::string> >
    getGeneratedSources(const MFrontGenericData&);

    /*!
     * \param pdata : processing data
     */
    virtual std::vector<std::string>
    getGeneratedIncludes(const MFrontGenericData&);

    /*!
     * \param pdata : processing data
     */
    virtual std::map<std::string,std::vector<std::string> >
    getLibrariesDependencies(const MFrontGenericData&);

  private:

    void
    openOutputFiles(void);
    
    /*!
     * \param pdata : processing data
     * \param data  : model data
     */
    void
    generateOutputFiles(const MFrontGenericData&,
			const MFrontModelData&);

    /*!
     * \param pdata : processing data
     * \param data  : model data
     */
    void
    writeHeaderFile(const MFrontGenericData&,
		    const MFrontModelData&);

    /*!
     * \param pdata : processing data
     * \param data  : model data
     */
    void
    writeSrcFile(const MFrontGenericData&,
		 const MFrontModelData&);
    
    void
    writeInitializeIntputsVariablesInitialValues(const MFrontGenericData&,
						 const MFrontModelData&);

    void
    writeInitializeConstantMaterialProperties(const MFrontGenericData&,
					      const MFrontModelData&);

    void
    writeInitializeInputsVariablesDepths(const MFrontGenericData&,
					 const MFrontModelData&);

    void
    writeInitializeOutputsVariablesInitialValues(const MFrontGenericData&,
						 const MFrontModelData&);

    void
    writeInitializeOutputsVariablesDepths(const MFrontGenericData&,
					  const MFrontModelData&);

    void
    writeStaticVariableInitialization(const MFrontGenericData& pdata,
				      const StaticVarHandler&);

    void
    writeAssignDefaultValue(const MFrontGenericData& pdata,
			    const VarContainer::const_iterator,
			    const std::map<std::string,std::string>::const_iterator);

    void
    closeOutputFiles(void);

    static VarContainer::const_iterator
    findVariableDescription(const VarContainer&,
			    const std::string&);

    std::string
    getGenTypeGetMethod(const std::string&) const;

    std::string
    getGenTypeIsMethod(const std::string&) const;

    std::string
    getCelaenoVariableName(const std::string&,
			    const MFrontModelData&) const;

    bool
    hasSpecializedConstructor(const MFrontModelData&) const;
    
    std::ofstream headerFile;
    std::ofstream srcFile;
    std::string headerFileName;
    std::string srcFileName;

    bool hasDefaultConstructor;
    bool verboseMode;
    bool debugMode;
    bool warningMode;
  }; // end of class MFrontCelaenoModelInterface

} // end of namespace mfront  

#endif /* _LIB_MFRONTCELAENOMODELINTERFACE_H */

