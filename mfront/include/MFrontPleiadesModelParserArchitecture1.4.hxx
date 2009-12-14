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
#include"MFrontModelParserBase.hxx"
#include"MFrontVirtualParser.hxx"

namespace mfront{

  struct MFrontPleiadesModelParserArchitecture1_4
    : public MFrontVirtualParser,
      public MFrontModelParserBase<MFrontPleiadesModelParserArchitecture1_4>
  {
    static std::string 
    getName(void);

    static std::string
    getDescription(void);

    static
    std::string 
    makeUpperCase(const std::string&);

    MFrontPleiadesModelParserArchitecture1_4();

    void setVerboseMode(void);

    void setWarningMode(void);

    void setDebugMode(void);

    void treatFile(const std::string&);

    void writeOutputFiles(void);

    void
    setInterfaces(const std::set<std::string>&);

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

    std::map<std::string,
	     std::pair<std::vector<std::string>,
		       std::vector<std::string> > >
    getSpecificTargets(void);

  private:

    void
    treatExecute(void);

    void
    treatOutput(void);

    void
    treatInput(void);

    void
    treatGlobalParameter(void);
  
    void
    treatLocalParameter(void);

    void
    treatInputMethod(void);

    void
    treatOutputMethod(void);

    void
    treatLocalParameterMethod(void);

    void
    treatGlobalParameterMethod(void);

    void
    treatLoadingVariableMethod(void);

    void
    treatFunctionMethod(void);

    void
    treatFunction(void);

    void
    treatDomain(void);

    void
    treatDomains(void);

    void
    treatInitializeOutput(void);

    void
    treatInitializeInput(void);

    void
    treatInitializeParameters(void);

    void
    treatExportCastem(void);

    void
    treatBindDomainToParameter(void);

    void
    treatBindDomainsToParameters(void);

    void
    treatLoadingVariable(void);

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

    void
    writeCastemHeaderFile(void);

    void
    writeCastemSrcFile(void);

    std::string
    getGenTypeMethod(const std::string&) const;

    void
    writeAssignDefaultValue(const VarContainer::const_iterator,
			    const std::map<std::string,std::string>::const_iterator);

    void
    readDefaultValue(void);

    template<typename Type>
    struct Property;

    struct Function;

    struct LoadingVariable;
    
    typedef std::vector<Function> FunctionContainer;
    VarContainer outputs;
    VarContainer inputs;
    VarContainer globalParameters;
    VarContainer localParameters;
    FunctionContainer functions;
    std::vector<LoadingVariable> loadingVariables;
    std::set<std::string> functionNames;
    std::set<std::string> fieldNames;
    std::set<std::string> domains;
    std::set<std::string> domainsParameters;
    std::map<std::string,std::vector<Property<double> > > fieldDoubleProperties;
    std::map<std::string,std::vector<Property<std::string> > > fieldStringProperties;
    std::map<std::string,std::string> glossaryNames;
    std::map<std::string,std::string> entryNames;
    std::map<std::string,std::string> defaultValues;
    std::map<std::string,double> initialValues;
    std::map<std::string,unsigned short> depth;
    std::ofstream headerFile;
    std::ofstream srcFile;
    std::ofstream castemHeaderFile;
    std::ofstream castemSrcFile;
    std::string headerFileName;
    std::string srcFileName;
    std::string castemHeaderFileName;
    std::string castemSrcFileName;
    std::string execute;
    std::string currentVar;
    std::string initializeParameters;
    std::string initializeOutput;
    std::string initializeInput;
    bool exportCastem;
    bool useTabStringToDefineDomain;
    bool hasDefaultConstructor;
  }; // end of class MFrontPleiadesModelParserArchitecture1_4

} // end of namespace mfront  

#endif /* _LIB_MFRONTPLEIADESMODELPARSERARCHITECTURE_1_4_H */

