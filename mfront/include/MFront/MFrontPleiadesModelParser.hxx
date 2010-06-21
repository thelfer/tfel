/*!
 * \file   MFrontPleiadesModelParser.hxx
 * \brief  This file declares the MFrontPleiadesModelParser class
 * \author Helfer Thomas
 * \date   10 Nov 2006
 */

#ifndef _LIB_MFRONTPLEIADESMODELPARSER_H_
#define _LIB_MFRONTPLEIADESMODELPARSER_H_ 

#include<vector>
#include<map>
#include<set>
#include<string>
#include<fstream>

#include"MFront/MFrontModelParserBase.hxx"
#include"MFront/VariableBoundsDescription.hxx"

namespace mfront{

  struct MFrontPleiadesModelParser
    : public MFrontModelParserBase<MFrontPleiadesModelParser>
  {
    static std::string 
    getName(void);

    static std::string
    getDescription(void);

    MFrontPleiadesModelParser();

    void writeOutputFiles(void);

    std::map<std::string,std::vector<std::string> >
    getGlobalIncludes(void);

    std::map<std::string,std::vector<std::string> >
    getGlobalDependencies(void);

    std::map<std::string,std::vector<std::string> >
    getGeneratedSources(void);

    std::vector<std::string>
    getGeneratedIncludes(void);

    std::map<std::string,std::vector<std::string> >
    getLibrariesDependencies(void);

  private:

    friend class MFrontModelParserBase<MFrontPleiadesModelParser>;

    static VarContainer::const_iterator
    findVariableDescription(const VarContainer&,
			    const std::string&);

    void
    openOutputFiles(void);

    void
    closeOutputFiles(void);

    void
    generateOutputFiles(void);

    void
    writeHeaderFile(void);

    void
    writeSrcFile(void);

    std::string
    getPleiadesVariableName(const std::string&) const;

    std::string
    isGenTypeMethod(const std::string&) const;

    std::string
    getGenTypeMethod(const std::string&) const;

    void
    writeInitializeOutputsVariablesDepths(void);

    void
    writeInitializeOutputsVariablesInitialValues(void);

    void
    writeInitializeInputsVariablesDepths(void);
  
    void
    writeInitializeConstantMaterialProperties(void);

    void
    writeAssignDefaultValue(const VarContainer::const_iterator,
			    const std::map<std::string,std::string>::const_iterator);

    void
    writeStaticVariableInitialization(const std::string&,
				      const StaticVarHandler&);


    bool
    hasSpecializedConstructor(void) const;

    std::ofstream headerFile;
    std::ofstream srcFile;
    std::string headerFileName;
    std::string srcFileName;
    std::string initializeParameters;
    bool hasDefaultConstructor;
  }; // end of class MFrontPleiadesModelParser

} // end of namespace mfront  

#endif /* _LIB_MFRONTPLEIADESMODELPARSER_H */

