/*!
 * \file   mfront/tests/models/include/MFront/LicosModelInterface.hxx
 * \brief  This file declares the MFrontModelInterface class
 * \author Helfer Thomas
 * \date   10 Nov 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_MFRONTMODELINTERFACE_H_
#define _LIB_MFRONTMODELINTERFACE_H_ 

#include"MFront/AbstractModelInterface.hxx"

namespace mfront{

  struct MFrontModelInterface
    : public AbstractModelInterface
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
    void writeOutputFiles(const FileDescription&,
			  const ModelData&) ;

    /*!
     * \param pdata : processing data
     */
    virtual std::map<std::string,std::vector<std::string> >
    getGlobalIncludes(const ModelData&);

    /*!
     * \param pdata : processing data
     */
    virtual std::map<std::string,std::vector<std::string> >
    getGlobalDependencies(const ModelData&);

    /*!
     * \param pdata : processing data
     */
    virtual std::map<std::string,std::vector<std::string> >
    getGeneratedSources(const ModelData&);

    /*!
     * \param pdata : processing data
     */
    virtual std::vector<std::string>
    getGeneratedIncludes(const ModelData&);

    /*!
     * \param pdata : processing data
     */
    virtual std::map<std::string,std::vector<std::string> >
    getLibrariesDependencies(const ModelData&);

  private:

    void
    openOutputFiles(void);
    
    /*!
     * \param pdata : processing data
     * \param data  : model data
     */
    void
    generateOutputFiles(const FileDescription&,
			const ModelData&);

    /*!
     * \param pdata : processing data
     * \param data  : model data
     */
    void
    writeHeaderFile(const FileDescription&,
		    const ModelData&);

    /*!
     * \param pdata : processing data
     * \param data  : model data
     */
    void
    writeSrcFile(const FileDescription&,
		 const ModelData&);
    
    void
    writeInitializeIntputsVariablesInitialValues(const ModelData&);

    void
    writeInitializeConstantMaterialProperties(const ModelData&);

    void
    writeInitializeInputsVariablesDepths(const ModelData&);

    void
    writeInitializeOutputsVariablesInitialValues(const ModelData&);

    void
    writeInitializeOutputsVariablesDepths(const ModelData&);

    void
    writeStaticVariableInitialization(const FileDescription&,
				      const ModelData&,
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
		    const ModelData&) const;

    ~MFrontModelInterface();

    private:

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

