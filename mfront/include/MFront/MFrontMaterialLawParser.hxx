/*!
 * \file   MFrontMaterialLawParser.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   06 jui 2007
 */

#ifndef _LIB_MFRONTMATERIALLAWPARSER_HXX_
#define _LIB_MFRONTMATERIALLAWPARSER_HXX_ 

#include<string>
#include<set>

#include"MFront/ParserBase.hxx"
#include"MFront/LawFunction.hxx"
#include"MFront/VariableBoundsDescription.hxx"

namespace mfront{

  struct MFrontMaterialLawParser
    : public MFrontVirtualParser,
      public ParserBase
  {
    static std::string 
    getName(void);

    static std::string
    getDescription(void);

    MFrontMaterialLawParser();

    virtual void
    setVerboseMode(void);

    virtual void
    setWarningMode(void);

    virtual void
    setDebugMode(void);

    virtual void
    treatFile(const std::string&,
	      const std::vector<std::string>&);
    
    virtual void
    getKeywordsList(std::vector<std::string>&) const;

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

  protected:

    typedef void (MFrontMaterialLawParser::* MemberFuncPtr)(void);
    typedef std::map<std::string,MemberFuncPtr> CallBackContainer;

    virtual void
    analyseFile(const std::string&,
		const std::vector<std::string>&);

    virtual void
    writeOutputFiles(void);

    void
    addInterface(const std::string&);

    void
    treatNamespace(void);

    void
    treatUseTemplate(void);

    void
    treatOutput(void);

    void
    treatInput(void);

    void
    treatLaw(void);

    void
    treatFunction(void);

    /*!
     * treat the setGlossaryName and the setEntryName method for
     * inputs or output.
     */
    void
    treatMethod(void);

    void
    treatInterface(void);

    void
    treatParameter(void);

    void
    treatConstant(void);

    void
    treatBounds(void);

    void
    treatPhysicalBounds(void);

    void
    treatUnknownKeyword(void);

    void
    registerBounds(std::vector<VariableBoundsDescription>&);
    
    LawFunction f;

    void
    registerNewCallBack(const std::string&,const MemberFuncPtr);

    VarContainer inputs;
    std::set<std::string> interfaces;
    std::vector<VariableBoundsDescription> boundsDescriptions;
    std::vector<VariableBoundsDescription> physicalBoundsDescriptions;
    std::vector<std::string> namespaces;
    std::set<std::string> registredKeyWords;
    std::map<std::string,std::vector<std::string> > sourcesLibrairiesDependencies;
    std::map<std::string,std::string> glossaryNames;
    std::map<std::string,std::string> entryNames;
    std::vector<std::string> parameters;
    std::map<std::string,double> parametersValues;
    CallBackContainer callBacks;
    std::string currentVar;
    std::string output;
    bool useTemplate;
  }; // end of class MFrontMaterialLawParser

} // end of namespace mfront  
  
#endif /* _LIB_MFRONTMATERIALLAWPARSER_HXX */

