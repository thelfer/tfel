/*!
 * \file   MFrontPleiadesModelParser.hxx
 * \brief  This file declares the MFrontPleiadesModelParserArchitecture1_4 class
 * \author Helfer Thomas
 * \date   10 Nov 2006
 */

#ifndef _LIB_MFRONTPLEIADESMODELPARSERARCHITECTURE_1_4_H_
#define _LIB_MFRONTPLEIADESMODELPARSERARCHITECTURE_1_4_H_ 

#include<vector>
#include<map>
#include<set>
#include<string>
#include<fstream>

#include"MFront/MFrontModelParserBase.hxx"

namespace mfront{

  struct MFrontPleiadesModelParserArchitecture1_4
    : public MFrontModelParserBase<MFrontPleiadesModelParserArchitecture1_4>
  {
    static std::string 
    getName(void);

    static std::string
    getDescription(void);

    MFrontPleiadesModelParserArchitecture1_4();

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

//     void
//     treatLoadingVariableMethod(void);

//     void
//     treatLoadingVariable(void);

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
    getGenTypeMethod(const std::string&) const;

    void
    writeAssignDefaultValue(const VarContainer::const_iterator,
			    const std::map<std::string,std::string>::const_iterator);

//     struct LoadingVariable;
    
//     std::vector<LoadingVariable> loadingVariables;

    std::ofstream headerFile;
    std::ofstream srcFile;
    std::string headerFileName;
    std::string srcFileName;
    std::string execute;
    std::string initializeParameters;
    std::string initializeOutput;
    std::string initializeInput;

    bool hasDefaultConstructor;
  }; // end of class MFrontPleiadesModelParserArchitecture1_4

} // end of namespace mfront  

#endif /* _LIB_MFRONTPLEIADESMODELPARSERARCHITECTURE_1_4_H */

