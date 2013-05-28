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

namespace mfront{

  struct MFrontRungeKuttaParser
    : public MFrontBehaviourParserBase<MFrontRungeKuttaParser>
  {
    static std::string 
    getName(void);

    static std::string 
    getDescription(void);

    MFrontRungeKuttaParser();

    ~MFrontRungeKuttaParser();

  private:

    enum ErrorEvaluation{
      DEFAULTERROREVALUATION,
      ERRORSUMMATIONEVALUATION,
      MAXIMUMVALUEERROREVALUATION
    }; // end of ErrorEvaluation

    std::string
    computeStressVariableModifier1(const std::string&,
				   const bool);

    std::string
    computeStressVariableModifier2(const std::string&,
				   const bool);

    virtual void
    treatUnknownVariableMethod(const std::string&);

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

    void
    writeBehaviourUpdateAuxiliaryStateVars(void);

    virtual void
    writeBehaviourStaticVars(void);

    virtual void
    writeBehaviourIntegrator(void);

    virtual void
    writeBehaviourComputeTangentOperator(void);

    virtual void
    endsInputFileProcessing(void);

    friend class MFrontBehaviourParserBase<MFrontRungeKuttaParser>;

    // error normalisation factors
    std::map<std::string,std::string> enf;

    std::string algorithm;
    std::string computeFinalStress;
    std::string computeStress;
    std::string derivative;
    std::string tangentOperator;
    //! how the integration error is evaluated for the RK54 and RK42
    //! integrators
    ErrorEvaluation eev;
    //! number of evaluations used by the algorithm
    unsigned short nbrOfEvaluation;
  };

} // end of namespace mfront  

#endif /* _LIB_MFRONTRUNGEKUTTAPARSER_H */

