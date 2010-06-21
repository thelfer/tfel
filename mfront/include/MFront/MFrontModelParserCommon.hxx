/*!
 * \file   MFrontModelParserCommon.hxx
 * \brief  This file declares the MFrontModelParserCommon class
 * \author Helfer Thomas
 * \date   10 Nov 2006
 */

#ifndef _LIB_MFRONTMODELPARSERCOMMON_H_
#define _LIB_MFRONTMODELPARSERCOMMON_H_ 

#include<vector>
#include<map>
#include<set>
#include<string>
#include<fstream>

#include"TFEL/Config/TFELConfig.hxx"

#include"MFront/ParserBase.hxx"
#include"MFront/MFrontVirtualParser.hxx"
#include"MFront/MFrontModelData.hxx"

namespace mfront{

  typedef std::vector<std::string> StringContainer;

  struct TFEL_VISIBILITY_EXPORT MFrontModelParserCommon
    : public MFrontVirtualParser,
      public ParserBase,
      protected MFrontModelData
  {

    virtual void
    setVerboseMode(void);

    virtual void
    setWarningMode(void);

    virtual void
    setDebugMode(void);

    virtual void
    writeOutputFiles(void);

    virtual void
    setInterfaces(const std::set<std::string>&);

    virtual std::map<std::string,std::vector<std::string> >
    getGlobalIncludes(void);

    virtual std::map<std::string,std::vector<std::string> >
    getGlobalDependencies(void);

    virtual std::map<std::string,std::vector<std::string> >
    getGeneratedSources(void);

    virtual std::vector<std::string>
    getGeneratedIncludes(void);

    virtual std::map<std::string,std::vector<std::string> >
    getLibrariesDependencies(void);

    virtual std::map<std::string,
		     std::pair<std::vector<std::string>,
			       std::vector<std::string> > >
    getSpecificTargets(void);

    static bool
    is(const MFrontModelData&,
       const VarContainer&,
       const std::string&);

  protected:

    void
    treatModel(void);

    void
    treatMaterial(void);

    void
    treatDomain(void);

    void
    treatDomains(void);

    void
    treatUnknownKeyword(void);

    void
    treatBounds(void);

    void
    treatPhysicalBounds(void);

    void
    treatConstantMaterialProperty(void);

    void
    treatConstantMaterialPropertyMethod(void);

    void
    treatFunctionMethod(void);

    void
    treatFunction(void);

    void
    treatOutput(void);

    void
    treatOutputMethod(void);

    void
    treatInput(void);

    void
    treatInputMethod(void);

    void
    treatGlobalParameter(void);

    void
    treatGlobalParameterMethod(void);

    void
    treatLocalParameter(void);

    void
    treatLocalParameterMethod(void);

    void
    readDefaultValue(void);

    void
    registerBounds(std::vector<VariableBoundsDescription>&);

    std::pair<std::string,unsigned short>
    decomposeVariableName(const std::string&) const;

    bool
    isOutputVariable(const std::string&) const;

    bool
    isInputVariable(const std::string&) const;

    StringContainer interfaces;
    
    std::string currentVar;
    std::map<std::string,std::vector<std::string> > sourcesLibrairiesDependencies;

  }; // end of class MFrontModelParserCommon

} // end of namespace mfront  

#endif /* _LIB_MFRONTMODELPARSERCOMMON_H */

