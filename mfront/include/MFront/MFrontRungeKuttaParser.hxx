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

    virtual void treatStateVariables(void);

    virtual void treatEpsilon(void);

    virtual void treatMinimalTimeStep(void);

    virtual void treatAlgorithm(void);

    virtual void treatComputeStress(void);

    void
    treatUpdateAuxiliaryStateVars(void);

    virtual void treatTangentOperator(void);

    virtual void treatIsTangentOperatorSymmetric(void);


    virtual void treatDerivative(void);

    virtual void writeBehaviourParserSpecificIncludes(void);

    virtual void writeBehaviourParserSpecificTypedefs(void);

    virtual void writeBehaviourParserSpecificMembers(void);

    virtual void writeBehaviourParserSpecificConstructorPart(void);

    virtual void writeBehaviourEulerIntegrator(void);

    virtual void writeBehaviourRK2Integrator(void);

    virtual void writeBehaviourRK4Integrator(void);

    virtual void writeBehaviourRK42Integrator(void);

    virtual void writeBehaviourRK54Integrator(void);

    virtual void writeBehaviourRKCastemIntegrator(void);

    virtual void writeBehaviourUpdateStateVars(void);

    virtual void
    writeBehaviourUpdateAuxiliaryStateVars(void);

    virtual void
    writeBehaviourStaticVars(void);

    virtual void
    writeBehaviourIntegrator(void);

    virtual void
    writeBehaviourComputeTangentOperator(void);

    virtual void
    endsInputFileProcessing(void);

    friend struct MFrontBehaviourParserBase<MFrontRungeKuttaParser>;

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
    
    bool isConsistantTangentOperatorSymmetricDefined;
  };

} // end of namespace mfront  

#endif /* _LIB_MFRONTRUNGEKUTTAPARSER_H */

