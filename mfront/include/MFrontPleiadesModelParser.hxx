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
#include"MFrontModelParserBase.hxx"
#include"MFrontVirtualParser.hxx"
#include"VariableBoundsDescription.hxx"

namespace mfront{

  struct MFrontPleiadesModelParser
    : public MFrontVirtualParser,
      public MFrontModelParserBase<MFrontPleiadesModelParser>
  {
    static std::string 
    getName(void);

    static std::string
    getDescription(void);

    static
    std::string 
    makeUpperCase(const std::string&);

    MFrontPleiadesModelParser();

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

    friend class MFrontModelParserBase<MFrontPleiadesModelParser>;

    static VarContainer::const_iterator
    findVariableDescription(const VarContainer&,
			    const std::string&);

    void
    treatExecute(void);

    void
    treatModel(void);

    void
    treatMaterial(void);

    void
    treatBounds(void);

    void
    treatPhysicalBounds(void);

    void
    treatOutput(void);

    void
    treatInput(void);

    void
    treatGlobalParameter(void);
  
    void
    treatLocalParameter(void);

    void
    treatConstantMaterialProperty(void);

    void
    treatInputMethod(void);

    void
    treatOutputMethod(void);

    void
    treatLocalParameterMethod(void);

    void
    treatGlobalParameterMethod(void);

    void
    treatConstantMaterialPropertyMethod(void);

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
    openOutputFiles(void);

    void
    closeOutputFiles(void);

    void
    generateOutputFiles(void);

    void
    writeHeaderFile(void);

    void
    writeSrcFile(void);

    std::pair<std::string,unsigned short>
    decomposeVariableName(const std::string&) const;

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

    bool
    is(const VarContainer&,
       const std::string&) const;

    bool
    isInputVariable(const std::string&) const;

    bool
    isOutputVariable(const std::string&) const;

    void
    writeAssignDefaultValue(const VarContainer::const_iterator,
			    const std::map<std::string,std::string>::const_iterator);

    void
    writeStaticVariableInitialization(const std::string&,
				      const StaticVarHandler&);

    void
    readDefaultValue(void);

    bool
    hasSpecializedConstructor(void) const;

    void
    registerBounds(std::vector<VariableBoundsDescription>&);

    struct Function;

    typedef std::vector<Function> FunctionContainer;
    VarContainer outputs;
    VarContainer inputs;
    VarContainer globalParameters;
    VarContainer localParameters;
    VarContainer constantMaterialProperties;
    FunctionContainer functions;
    std::set<std::string> functionNames;
    std::set<std::string> fieldNames;
    std::set<std::string> domains;
    std::map<std::string,std::string> glossaryNames;
    std::map<std::string,std::string> entryNames;
    std::map<std::string,std::string> defaultValues;
    std::map<std::string,double> initialValues;
    std::map<std::string,unsigned short> depth;
    std::ofstream headerFile;
    std::ofstream srcFile;
    std::string headerFileName;
    std::string srcFileName;
    std::string execute;
    std::string currentVar;
    std::string initializeParameters;
    bool hasDefaultConstructor;
    std::vector<VariableBoundsDescription> boundsDescriptions;
    std::vector<VariableBoundsDescription> physicalBoundsDescriptions;
  }; // end of class MFrontPleiadesModelParser

} // end of namespace mfront  

#endif /* _LIB_MFRONTPLEIADESMODELPARSER_H */

