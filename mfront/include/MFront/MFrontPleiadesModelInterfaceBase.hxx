/*!
 * \file   MFrontPleiadesModelinterface.hxx
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
    writeSrcFile(const MFrontGenericData&,
		 const MFrontModelData&);

    virtual void
    writeAssignDefaultValue(const MFrontGenericData&,
			    const VarContainer::const_iterator,
			    const std::map<std::string,std::string>::const_iterator);

    virtual std::string
    getGenTypeMethod(const std::string& type) const;

    virtual void
    setOutputFileNames(const MFrontGenericData&) = 0;
    
    virtual void
    writeGetGlobalParameter(const VarHandler&,
			    const MFrontGenericData&,
			    const MFrontModelData&);

    virtual void
    writeGetConstantMaterialProperty(const VarHandler&,
				     const MFrontGenericData&,
				     const MFrontModelData&);

    std::ofstream headerFile;
    std::ofstream srcFile;
    std::string headerFileName;
    std::string srcFileName;

    bool hasDefaultConstructor;
    bool verboseMode;
    bool debugMode;
    bool warningMode;
  }; // end of class MFrontPleiadesModelInterface

} // end of namespace mfront  

#endif /* _LIB_MFRONTPLEIADESMODELINTERFACE_H */

