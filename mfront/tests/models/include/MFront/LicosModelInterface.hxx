/*!
 * \file   MFrontModelInterface.hxx
 * \brief  This file declares the MFrontModelInterface class
 * \author Helfer Thomas
 * \date   10 Nov 2006
 */

#ifndef _LIB_MFRONTMODELINTERFACE_H_
#define _LIB_MFRONTMODELINTERFACE_H_ 

#include"MFront/MFrontModelVirtualInterface.hxx"

namespace mfront{

  struct MFrontModelInterface
    : public MFrontModelVirtualInterface
  {
    static std::string 
    getName(void);

    MFrontModelInterface();

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
    void writeOutputFiles(const MFrontFileDescription&,
			  const MFrontModelData&) ;

    /*!
     * \param pdata : processing data
     */
    virtual std::map<std::string,std::vector<std::string> >
    getGlobalIncludes(const MFrontModelData&);

    /*!
     * \param pdata : processing data
     */
    virtual std::map<std::string,std::vector<std::string> >
    getGlobalDependencies(const MFrontModelData&);

    /*!
     * \param pdata : processing data
     */
    virtual std::map<std::string,std::vector<std::string> >
    getGeneratedSources(const MFrontModelData&);

    /*!
     * \param pdata : processing data
     */
    virtual std::vector<std::string>
    getGeneratedIncludes(const MFrontModelData&);

    /*!
     * \param pdata : processing data
     */
    virtual std::map<std::string,std::vector<std::string> >
    getLibrariesDependencies(const MFrontModelData&);

  private:

    void
    openOutputFiles(void);
    
    /*!
     * \param pdata : processing data
     * \param data  : model data
     */
    void
    generateOutputFiles(const MFrontFileDescription&,
			const MFrontModelData&);

    /*!
     * \param pdata : processing data
     * \param data  : model data
     */
    void
    writeHeaderFile(const MFrontFileDescription&,
		    const MFrontModelData&);

    /*!
     * \param pdata : processing data
     * \param data  : model data
     */
    void
    writeSrcFile(const MFrontFileDescription&,
		 const MFrontModelData&);
    
    void
    writeInitializeIntputsVariablesInitialValues(const MFrontModelData&);

    void
    writeInitializeConstantMaterialProperties(const MFrontModelData&);

    void
    writeInitializeInputsVariablesDepths(const MFrontModelData&);

    void
    writeInitializeOutputsVariablesInitialValues(const MFrontModelData&);

    void
    writeInitializeOutputsVariablesDepths(const MFrontModelData&);

    void
    writeStaticVariableInitialization(const MFrontFileDescription&,
				      const MFrontModelData&,
				      const StaticVariableDescription&);

    void
    writeAssignDefaultValue(const VariableDescriptionContainer::const_iterator,
			    const std::map<std::string,std::string>::const_iterator);

    void
    closeOutputFiles(void);

    static VariableDescriptionContainer::const_iterator
    findVariableDescription(const VarContainer&,
			    const std::string&);

    std::string
    getGenTypeGetMethod(const std::string&) const;

    std::string
    getGenTypeIsMethod(const std::string&) const;

    std::string
    getVariableName(const std::string&,
			    const MFrontModelData&) const;

    std::ofstream headerFile;
    std::ofstream srcFile;
    std::string headerFileName;
    std::string srcFileName;

    bool hasDefaultConstructor;
    bool verboseMode;
    bool debugMode;
    bool warningMode;
  }; // end of class MFrontModelInterface

} // end of namespace mfront  

#endif /* _LIB_MFRONTMODELINTERFACE_H */

