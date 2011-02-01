/*!
 * \file   MFrontRungeKuttaParser.hxx
 * \brief  This file declares the MFrontRungeKuttaParser class
 * \author Helfer Thomas
 * \date   10 Nov 2006
 */

#ifndef _LIB_MFRONTRUNGEKUTTAPARSER_H_
#define _LIB_MFRONTRUNGEKUTTAPARSER_H_ 

#include<string>
#include"MFront/MFrontBehaviourParserBase.hxx"
#include"MFront/MFrontVirtualParser.hxx"

namespace mfront{

  struct MFrontRungeKuttaParser
    : public MFrontVirtualParser,
      public MFrontBehaviourParserBase<MFrontRungeKuttaParser>
  {
    static std::string 
    getName(void);

    static std::string 
    getDescription(void);

    MFrontRungeKuttaParser();

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

    ~MFrontRungeKuttaParser();

  private:

    std::string
    variableModifier1(const std::string&,
		      const bool);

    std::string
    variableModifier2(const std::string&,
		      const bool);

    void treatStateVariables(void);

    void treatEpsilon(void);

    void treatMinimalTimeStep(void);

    void treatAlgorithm(void);

    void treatComputeStress(void);

    void
    treatUpdateAuxiliaryStateVars(void);

    void treatDerivative(void);

    void writeBehaviourParserSpecificIncludes(void);

    void writeBehaviourParserSpecificTypedefs(void);

    void writeBehaviourParserSpecificMembers(void);

    void writeBehaviourParserSpecificConstructorPart(void);

    void writeBehaviourEulerIntegrator(void);

    void writeBehaviourRK2Integrator(void);

    void writeBehaviourRK4Integrator(void);

    void writeBehaviourRK42Integrator(void);

    void writeBehaviourRK54Integrator(void);

    void writeBehaviourRKCastemIntegrator(void);

    void writeBehaviourUpdateStateVars(void);

    void writeBehaviourStaticVars(void);

    void writeBehaviourIntegrator(void);

    void generateOutputFiles(void);

    void endsInputFileProcessing(void);

    friend class MFrontBehaviourParserBase<MFrontRungeKuttaParser>;

    std::string algorithm;
    std::string computeFinalStress;
    std::string computeStress;
    std::string derivative;
    double epsilon;
    double dtmin;
    unsigned short nbrOfEvaluation;
  };

} // end of namespace mfront  

#endif /* _LIB_MFRONTRUNGEKUTTAPARSER_H */

