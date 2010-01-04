/*!
 * \file   MFrontImplicitParser.hxx
 * \brief  This file declares the MFrontImplicitParser class
 * \author Helfer Thomas
 * \date   10 Nov 2006
 */

#ifndef _LIB_MFRONTIMPLICITPARSER_H_
#define _LIB_MFRONTIMPLICITPARSER_H_ 

#include<string>
#include"MFrontBehaviourParserBase.hxx"
#include"MFrontVirtualParser.hxx"

namespace mfront{

  struct MFrontImplicitParser
    : public MFrontVirtualParser,
      public MFrontBehaviourParserBase<MFrontImplicitParser>
  {
    static std::string 
    getName(void);

    static std::string 
    getDescription(void);

    MFrontImplicitParser();

    void setVerboseMode();

    void setDebugMode();

    void setWarningMode();

    void treatFile(const std::string&);

    void writeOutputFiles();

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

    ~MFrontImplicitParser();

  private:

    enum Algorithm{
      NEWTONRAPHSON,
      BROYDEN,
      BROYDEN2,
      DEFAULT
    };

    std::string
    variableModifier1(const std::string&,const bool);

    std::string
    variableModifier2(const std::string&,const bool);

    void endsInputFileProcessing(void);

    void writeBehaviourIntegrator(void);

    void writeBehaviourParserSpecificIncludes(void);

    void writeBehaviourParserSpecificTypedefs(void);

    void writeBehaviourParserSpecificMembers(void);

    void writeBehaviourStateVarsIncrements(void);
    
    void writeBehaviourConstructors(void);

    void writeBehaviourStaticVars(void);

    void treatTheta(void);

    void treatIterMax(void);

    void treatEpsilon(void);

    void treatAlgorithm(void);

    void treatPredictor(void);

    void treatComputeStress(void);

    void treatStateVariables(void);

    void generateOutputFiles(void);

    friend class MFrontBehaviourParserBase<MFrontImplicitParser>;

    std::string computeStress;

    std::string computeFinalStress;

    std::string predictor;

    long double theta;

    long double epsilon;

    unsigned short iterMax;

    Algorithm algorithm;

  }; // end of struct MFrontImplicitParser

} // end of namespace mfront  

#endif /* _LIB_MFRONTIMPLICITPARSER_H */

