/*! 
 * \file  MTestParser.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 09 avril 2013
 */

#ifndef _LIB_MFRONT_MTESTPARSER_H_
#define _LIB_MFRONT_MTESTPARSER_H_ 

#include<map>
#include<string>
#include<vector>
#include<fstream>

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Utilities/SmartPtr.hxx"
#include"TFEL/Utilities/CxxTokenizer.hxx"

#include"MFront/MTestConfig.hxx"
#include"MFront/MTestTypes.hxx"

namespace mfront
{

  // forward declaration
  struct MTest;

  // forward declaration
  struct MTestEvolution;

  /*!
   * MTestParser is a simple class 
   * to test mfront behaviours
   *
   * The unknowns are made of
   * the strain stensor followed 
   * by a number of lagrange
   * multiplier
   */
  struct MFRONT_MTEST_VISIBILITY_EXPORT MTestParser
    : public tfel::utilities::CxxTokenizer
  {
    /*!
     * default constructor
     */
    MTestParser();
    /*!
     * default constructor
     * \param[in] t : MTest object to be filled
     * \param[in] f : file name
     */
    void
    execute(MTest&,const std::string&);
    /*!
     * display the list of keywords
     */
    virtual void
    displayKeyWordsList() const;
    /*!
     * display the description of a keyword
     */
    virtual void
    displayKeyWordDescription(const std::string&) const;
    /*!
     * destructor
     */
    virtual ~MTestParser();
  protected:
    //! a simple alias
    typedef void (MTestParser::* CallBack)(MTest&,TokensContainer::const_iterator&);
    /*!
     * register a call back
     * \param[in] k : key word
     * \param[in] p : pointer to a member function
     */
    void
    registerCallBack(const std::string&,
		     const CallBack&);
    /*!
     * register the call backs associated with each command
     */
    void
    registerCallBacks(void);
    /*!
     * handle the @Real keyword
     * \param[in,out] p : position in the input file
     */
    void handleReal(MTest&,TokensContainer::const_iterator&);
    /*!
     * handle the @CompareToNumericalTangentOperator keyword
     * \param[in,out] p : position in the input file
     */
    void handleCompareToNumericalTangentOperator(MTest&,TokensContainer::const_iterator&);
    /*!
     * handle the @TangentOperatorComparisonCriterium keyword
     * \param[in,out] p : position in the input file
     */
    void handleTangentOperatorComparisonCriterium(MTest&,TokensContainer::const_iterator&);
    /*!
     * handle the @NumericalTangentOperatorPerturbationValue keyword
     * \param[in,out] p : position in the input file
     */
    void handleNumericalTangentOperatorPerturbationValue(MTest&,TokensContainer::const_iterator&);
    /*!
     * handle the @StiffnessMatrixType keyword
     * \param[in,out] p : position in the input file
     */
    void handleStiffnessMatrixType(MTest&,TokensContainer::const_iterator&);
    /*!
     * handle the @StiffnessUpdatePolicy keyword
     * \param[in,out] p : position in the input file
     */
    void handleStiffnessUpdatePolicy(MTest&,TokensContainer::const_iterator&);
    /*!
     * handle the @UseCastemAccelerationAlgorithm keyword
     * \param[in,out] p : position in the input file
     */
    void
    handleUseCastemAccelerationAlgorithm(MTest&,TokensContainer::const_iterator&);
    /*!
     * handle the @CastemAccelerationTrigger keyword
     * \param[in,out] p : position in the input file
     */
    void
    handleCastemAccelerationTrigger(MTest&,TokensContainer::const_iterator&);
    /*!
     * handle the @CastemAccelerationAlgorithm keyword
     * \param[in,out] p : position in the input file
     */
    void
    handleCastemAccelerationPeriod(MTest&,TokensContainer::const_iterator&);
    /*!
     * handle the @UseIronsTuckAccelerationAlgorithm keyword
     * \param[in,out] p : position in the input file
     */
    void
    handleUseIronsTuckAccelerationAlgorithm(MTest&,TokensContainer::const_iterator&);
    /*!
     * handle the @IronsTuckAccelerationTrigger keyword
     * \param[in,out] p : position in the input file
     */
    void
    handleIronsTuckAccelerationTrigger(MTest&,TokensContainer::const_iterator&);
    /*!
     * handle the @UseSteffensenAccelerationAlgorithm keyword
     * \param[in,out] p : position in the input file
     */
    void
    handleUseSteffensenAccelerationAlgorithm(MTest&,TokensContainer::const_iterator&);
    /*!
     * handle the @SteffensenAccelerationTrigger keyword
     * \param[in,out] p : position in the input file
     */
    void
    handleSteffensenAccelerationTrigger(MTest&,TokensContainer::const_iterator&);
    /*!
     * handle the @UseSecantAccelerationAlgorithm keyword
     * \param[in,out] p : position in the input file
     */
    void
    handleUseSecantAccelerationAlgorithm(MTest&,TokensContainer::const_iterator&);
    /*!
     * handle the @SecantAccelerationTrigger keyword
     * \param[in,out] p : position in the input file
     */
    void
    handleSecantAccelerationTrigger(MTest&,TokensContainer::const_iterator&);
    /*!
     * handle the @Test keyword
     * \param[in,out] p : position in the input file
     */
    void handleTest(MTest&,TokensContainer::const_iterator&);
    /*!
     * handle the @RotationMatrix keyword
     * \param[in,out] p : position in the input file
     */
    void handleRotationMatrix(MTest&,TokensContainer::const_iterator&);
    /*!
     * handle the @MaximumNumberOfIterations keyword
     * \param[in,out] p : position in the input file
     */
    void handleMaximumNumberOfIterations(MTest&,TokensContainer::const_iterator&);
    /*!
     * handle the @MaximumNumberOfSubSteps keyword
     * \param[in,out] p : position in the input file
     */
    void handleMaximumNumberOfSubSteps(MTest&,TokensContainer::const_iterator&);
    /*!
     * handle the @StrainEpsilon keyword
     * \param[in,out] p : position in the input file
     */
    void handleStrainEpsilon(MTest&,TokensContainer::const_iterator&);
    /*!
     * handle the @DeformationGradientEpsilon keyword
     * \param[in,out] p : position in the input file
     */
    void handleDeformationGradientEpsilon(MTest&,TokensContainer::const_iterator&);
    /*!
     * handle the @OpeningDisplacementEpsilon keyword
     * \param[in,out] p : position in the input file
     */
    void handleOpeningDisplacementEpsilon(MTest&,TokensContainer::const_iterator&);
    /*!
     * handle the @DrivingVariableEpsilon keyword
     * \param[in,out] p : position in the input file
     */
    void handleDrivingVariableEpsilon(MTest&,TokensContainer::const_iterator&);
    /*!
     * handle the @StressEpsilon keyword
     * \param[in,out] p : position in the input file
     */
    void handleStressEpsilon(MTest&,TokensContainer::const_iterator&);
    /*!
     * handle the @CohesiveForceEpsilon keyword
     * \param[in,out] p : position in the input file
     */
    void handleCohesiveForceEpsilon(MTest&,TokensContainer::const_iterator&);
    /*!
     * handle the @ThermodynamicForceEpsilon keyword
     * \param[in,out] p : position in the input file
     */
    void handleThermodynamicForceEpsilon(MTest&,TokensContainer::const_iterator&);
    /*!
     * handle the @Parameter keyword
     * \param[in,out] p : position in the input file
     */
    void handleParameter(MTest&,TokensContainer::const_iterator&);
    /*!
     * handle the @IntegerParameter keyword
     * \param[in,out] p : position in the input file
     */
    void handleIntegerParameter(MTest&,TokensContainer::const_iterator&);
    /*!
     * handle the @UnsignedIntegerParameter keyword
     * \param[in,out] p : position in the input file
     */
    void handleUnsignedIntegerParameter(MTest&,TokensContainer::const_iterator&);
    /*!
     * handle the @OutputFile keyword
     * \param[in,out] p : position in the input file
     */
    void handleOutputFile(MTest&,TokensContainer::const_iterator&);
    /*!
     * handle the @OutputFilePrecision keyword
     * \param[in,out] p : position in the input file
     */
    void handleOutputFilePrecision(MTest&,TokensContainer::const_iterator&);
    /*!
     * handle the @ResidualFile keyword
     * \param[in,out] p : position in the input file
     */
    void handleResidualFile(MTest&,TokensContainer::const_iterator&);
    /*!
     * handle the @ResidualFilePrecision keyword
     * \param[in,out] p : position in the input file
     */
    void handleResidualFilePrecision(MTest&,TokensContainer::const_iterator&);
    /*!
     * handle the @ModellingHypothesis keyword
     * \param[in,out] p : position in the input file
     */
    void handleModellingHypothesis(MTest&,TokensContainer::const_iterator&);
    /*!
     * handle the @Times keyword
     * \param[in,out] p : position in the input file
     */
    void handleTimes(MTest&,TokensContainer::const_iterator&);
    /*!
     * handle the @Strain keyword
     * \param[in,out] p : position in the input file
     */
    void handleStrain(MTest&,TokensContainer::const_iterator&);
    /*!
     * handle the @DeformationGradient keyword
     * \param[in,out] p : position in the input file
     */
    void handleDeformationGradient(MTest&,TokensContainer::const_iterator&);
    /*!
     * handle the @OpeningDisplacement keyword
     * \param[in,out] p : position in the input file
     */
    void handleOpeningDisplacement(MTest&,TokensContainer::const_iterator&);
    /*!
     * handle the @DrivingVariable keyword
     * \param[in,out] p : position in the input file
     */
    void handleDrivingVariable(MTest&,TokensContainer::const_iterator&);
    /*!
     * handle the @Stress keyword
     * \param[in,out] p : position in the input file
     */
    void handleStress(MTest&,TokensContainer::const_iterator&);
    /*!
     * handle the @CohesiveForce keyword
     * \param[in,out] p : position in the input file
     */
    void handleCohesiveForce(MTest&,TokensContainer::const_iterator&);
    /*!
     * handle the @ThermodynamicForce keyword
     * \param[in,out] p : position in the input file
     */
    void handleThermodynamicForce(MTest&,TokensContainer::const_iterator&);
    /*!
     * handle the @Behaviour keyword
     * \param[in,out] p : position in the input file
     */
    void handleBehaviour(MTest&,TokensContainer::const_iterator&);
    /*!
     * handle the @MaterialProperty keyword
     * \param[in,out] p : position in the input file
     */
    void handleMaterialProperty(MTest&,TokensContainer::const_iterator&);
    /*!
     * handle the @InternalStateVariable keyword
     * \param[in,out] p : position in the input file
     */
    void handleInternalStateVariable(MTest&,TokensContainer::const_iterator&);
    /*!
     * handle the @ExternalStateVariable keyword
     * \param[in,out] p : position in the input file
     */
    void handleExternalStateVariable(MTest&,TokensContainer::const_iterator&);
    /*!
     * handle the @ImposedStress keyword
     * \param[in,out] p : position in the input file
     */
    void handleImposedStress(MTest&,TokensContainer::const_iterator&);
    /*!
     * handle the @ImposedCohesiveForce keyword
     * \param[in,out] p : position in the input file
     */
    void handleImposedCohesiveForce(MTest&,TokensContainer::const_iterator&);
    /*!
     * handle the @ImposedThermodynamicForce keyword
     * \param[in,out] p : position in the input file
     */
    void handleImposedThermodynamicForce(MTest&,TokensContainer::const_iterator&);
    /*!
     * handle the @ImposedStrain keyword
     * \param[in,out] p : position in the input file
     */
    void handleImposedStrain(MTest&,TokensContainer::const_iterator&);
    /*!
     * handle the @ImposedDeformationGradient keyword
     * \param[in,out] p : position in the input file
     */
    void handleImposedDeformationGradient(MTest&,TokensContainer::const_iterator&);
    /*!
     * handle the @ImposedOpeningDisplacement keyword
     * \param[in,out] p : position in the input file
     */
    void handleImposedOpeningDisplacement(MTest&,TokensContainer::const_iterator&);
    /*!
     * handle the @ImposedDrivingVariable keyword
     * \param[in,out] p : position in the input file
     */
    void handleImposedDrivingVariable(MTest&,TokensContainer::const_iterator&);
    /*!
     * handle the @Author keyword
     * \param[in,out] p : position in the input file
     */
    void handleAuthor(MTest&,TokensContainer::const_iterator&);
    /*!
     * handle the @Date keyword
     * \param[in,out] p : position in the input file
     */
    void handleDate(MTest&,TokensContainer::const_iterator&);
    /*!
     * handle the @Description keyword
     * \param[in,out] p : position in the input file
     */
    void handleDescription(MTest&,TokensContainer::const_iterator&);
    /*!
     * handle the @PredictionPolicy keyword
     * \param[in,out] p : position in the input file
     */
    void 
    handlePredictionPolicy(MTest&,TokensContainer::const_iterator&);
    /*!
     * handle the @Evolution keyword
     * \param[in,out] p : position in the input file
     */
    void 
    handleEvolution(MTest&,TokensContainer::const_iterator&);
    /*!
     * handle the @HandleThermalExpansion keyword
     * \param[in,out] p : position in the input file
     */
    void 
    handleHandleThermalExpansion(MTest&,TokensContainer::const_iterator&);
    /*!
     * \return everything from the given starting point up to the next
     * semi-colon.
     * \param[in,out] p : position in the input file
     */
    std::string
    readUntilEndOfInstruction(TokensContainer::const_iterator&);
    /*!
     * \return a real value
     * \param[in]     t : MTest object
     * \param[in,out] p : position in the input file
     */
    real
    readDouble(const MTest& t,
	       TokensContainer::const_iterator&);
    /*!
     * read a time
     * \param[in]     t : MTest object
     * \param[in,out] p : position in the input file
     */
    real
    readTime(const MTest&,TokensContainer::const_iterator&);
    /*!
     * \brief parse an evolution
     * \param[in]     type : evolution type
     * \param[in]     t    : MTest object
     * \param[in,out] p    : position in the input file
     */
    tfel::utilities::shared_ptr<MTestEvolution>
    parseEvolution(const std::string&,
		   const MTest&,
		   TokensContainer::const_iterator&);
    /*!
     * \brief try to read the type of an evolution
     * \param[in,out] p : position in the input file
     */
    std::string
    readEvolutionType(TokensContainer::const_iterator&);
    /*!
     * read an array
     * The expected size of the array is given by the output vector
     * \param[out]    v : array to be read
     * \param[in]     t : MTest object
     * \param[in,out] p : position in the input file
     */
    void
    readArrayOfSpecifiedSize(std::vector<real>&,
			     const MTest&,
			     TokensContainer::const_iterator&);
    //! callbacks
    std::map<std::string,CallBack> callbacks;
    //! input file
    std::string file;
  }; // end of struct MTestParser

} // end of namespace mfront

#endif /* _LIB_MFRONT_MTESTPARSER_H */

