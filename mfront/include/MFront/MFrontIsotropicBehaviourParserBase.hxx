/*!
 * \file   MFrontIsotropicBehaviourParserBase.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   01 jui 2007
 */

#ifndef _LIB_MFRONTISOTROPICBEHAVIOURPARSERBASE_HXX_
#define _LIB_MFRONTISOTROPICBEHAVIOURPARSERBASE_HXX_ 

#include<string>

#include"TFEL/Config/TFELConfig.hxx"
#include"MFront/MFrontVirtualParser.hxx"
#include"MFront/MFrontBehaviourParserBase.hxx"

namespace mfront{

  struct TFEL_VISIBILITY_EXPORT MFrontIsotropicBehaviourParserBase
    : public MFrontVirtualParser,
      public MFrontBehaviourParserBase<MFrontIsotropicBehaviourParserBase>
  {
    MFrontIsotropicBehaviourParserBase();

    void generateOutputFiles(void);

    void setVerboseMode();

    void setDebugMode();

    void setWarningMode();

    void treatFile(const std::string&);

    void writeOutputFiles();

    void
    setInterfaces(const std::set<std::string>&);

    virtual
    void writeBehaviourParserSpecificIncludes(void);

    virtual
    void writeBehaviourParserSpecificTypedefs(void);

    virtual
    void writeBehaviourParserSpecificConstructorPart(void) = 0;

    virtual
    void writeBehaviourParserSpecificMembers(void) = 0;

    virtual
    void writeBehaviourIntegrator(void) = 0;

    virtual
    std::map<std::string,std::vector<std::string> >
    getGlobalIncludes(void);

    virtual
    std::map<std::string,std::vector<std::string> >
    getGlobalDependencies(void);

    virtual
    std::map<std::string,std::vector<std::string> >
    getGeneratedSources(void);

    virtual
    std::vector<std::string>
    getGeneratedIncludes(void);

    virtual
    std::map<std::string,std::vector<std::string> >
    getLibrariesDependencies(void);
    
    virtual
    std::map<std::string,
	     std::pair<std::vector<std::string>,
		       std::vector<std::string> > >
    getSpecificTargets(void);

    virtual ~MFrontIsotropicBehaviourParserBase();

  protected:

    std::string
    flowRuleVariableModifier(const std::string&,const bool);

    void
    treatFlowRuleBase(std::string&);

    virtual
    void
    treatFlowRule(void);

    void
    treatTheta(void);

    void
    treatEpsilon(void);

    void
    treatIterMax(void);

    void
    endsInputFileProcessing(void);

    void
    writeBehaviourStaticVars(void);

    std::string flowRule;

    //! default value
    double theta;

  private:

    friend class MFrontBehaviourParserBase<MFrontIsotropicBehaviourParserBase>;

  }; // end of class MFrontIsotropicBehaviourParserBase

} // end of namespace MFront

#endif /* _LIB_MFRONTISOTROPICBEHAVIOURPARSERBASE_HXX */

