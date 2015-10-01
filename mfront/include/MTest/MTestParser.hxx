/*! 
 * \file  mfront/include/MTest/MTestParser.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 09 avril 2013
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MFRONT_MTESTPARSER_H_
#define LIB_MFRONT_MTESTPARSER_H_ 

#include<map>
#include<string>
#include<vector>
#include<memory>
#include<fstream>

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Utilities/CxxTokenizer.hxx"

#include"MTest/MTestConfig.hxx"
#include"MTest/MTestTypes.hxx"

namespace mfront
{

  // forward declaration
  struct MTest;
  // forward declaration
  struct Evolution;

  /*!
   * MTestParser is used to parse mtest file.
   */
  struct MFRONT_MTEST_VISIBILITY_EXPORT MTestParser
    : public tfel::utilities::CxxTokenizer
  {
    /*!
     * default constructor
     * \param[in] : mt structure to be filled
     */
    MTestParser(MTest&);
    /*!
     * execute mtest parser on a file
     * \param[in] f : file name
     */
    void execute(const std::string&);
    /*!
     * execute mtest parser on a string
     * \param[in] s : string
     */
    void parseString(const std::string&);
    /*!
     * display the list of keywords
     */
    virtual void
    displayKeyWordsList() const;
    /*!
     * display the list of keywords
     */
    virtual void
    displayKeyWordsHelp() const;
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
    typedef void (MTestParser::* CallBack)(TokensContainer::const_iterator&);
    /*!
     * execute mtest parser after reading a file or parsing a string
     */
    void execute();
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
    void handleReal(TokensContainer::const_iterator&);
    /*!
     * handle the @CompareToNumericalTangentOperator keyword
     * \param[in,out] p : position in the input file
     */
    void handleCompareToNumericalTangentOperator(TokensContainer::const_iterator&);
    /*!
     * handle the @TangentOperatorComparisonCriterium keyword
     * \param[in,out] p : position in the input file
     */
    void handleTangentOperatorComparisonCriterium(TokensContainer::const_iterator&);
    /*!
     * handle the @NumericalTangentOperatorPerturbationValue keyword
     * \param[in,out] p : position in the input file
     */
    void handleNumericalTangentOperatorPerturbationValue(TokensContainer::const_iterator&);
    /*!
     * handle the @StiffnessMatrixType keyword
     * \param[in,out] p : position in the input file
     */
    void handleStiffnessMatrixType(TokensContainer::const_iterator&);
    /*!
     * handle the @StiffnessUpdatePolicy keyword
     * \param[in,out] p : position in the input file
     */
    void handleStiffnessUpdatePolicy(TokensContainer::const_iterator&);
    /*!
     * handle the @UseCastemAccelerationAlgorithm keyword
     * \param[in,out] p : position in the input file
     */
    void
    handleUseCastemAccelerationAlgorithm(TokensContainer::const_iterator&);
    /*!
     * handle the @CastemAccelerationTrigger keyword
     * \param[in,out] p : position in the input file
     */
    void
    handleCastemAccelerationTrigger(TokensContainer::const_iterator&);
    /*!
     * handle the @CastemAccelerationAlgorithm keyword
     * \param[in,out] p : position in the input file
     */
    void
    handleCastemAccelerationPeriod(TokensContainer::const_iterator&);
    /*!
     * handle the @AccelerationAlgorithm keyword
     * \param[in,out] p : position in the input file
     */
    void
    handleAccelerationAlgorithm(TokensContainer::const_iterator&);
    /*!
     * handle the @AccelerationAlgorithmParameter keyword
     * \param[in,out] p : position in the input file
     */
    void
    handleAccelerationAlgorithmParameter(TokensContainer::const_iterator&);
    /*!
     * handle the @Test keyword
     * \param[in,out] p : position in the input file
     */
    void handleTest(TokensContainer::const_iterator&);
    /*!
     * handle the @RotationMatrix keyword
     * \param[in,out] p : position in the input file
     */
    void handleRotationMatrix(TokensContainer::const_iterator&);
    /*!
     * handle the @MaximumNumberOfIterations keyword
     * \param[in,out] p : position in the input file
     */
    void handleMaximumNumberOfIterations(TokensContainer::const_iterator&);
    /*!
     * handle the @MaximumNumberOfSubSteps keyword
     * \param[in,out] p : position in the input file
     */
    void handleMaximumNumberOfSubSteps(TokensContainer::const_iterator&);
    /*!
     * handle the @StrainEpsilon keyword
     * \param[in,out] p : position in the input file
     */
    void handleStrainEpsilon(TokensContainer::const_iterator&);
    /*!
     * handle the @DeformationGradientEpsilon keyword
     * \param[in,out] p : position in the input file
     */
    void handleDeformationGradientEpsilon(TokensContainer::const_iterator&);
    /*!
     * handle the @OpeningDisplacementEpsilon keyword
     * \param[in,out] p : position in the input file
     */
    void handleOpeningDisplacementEpsilon(TokensContainer::const_iterator&);
    /*!
     * handle the @DrivingVariableEpsilon keyword
     * \param[in,out] p : position in the input file
     */
    void handleDrivingVariableEpsilon(TokensContainer::const_iterator&);
    /*!
     * handle the @StressEpsilon keyword
     * \param[in,out] p : position in the input file
     */
    void handleStressEpsilon(TokensContainer::const_iterator&);
    /*!
     * handle the @CohesiveForceEpsilon keyword
     * \param[in,out] p : position in the input file
     */
    void handleCohesiveForceEpsilon(TokensContainer::const_iterator&);
    /*!
     * handle the @ThermodynamicForceEpsilon keyword
     * \param[in,out] p : position in the input file
     */
    void handleThermodynamicForceEpsilon(TokensContainer::const_iterator&);
    /*!
     * handle the @Parameter keyword
     * \param[in,out] p : position in the input file
     */
    void handleParameter(TokensContainer::const_iterator&);
    /*!
     * handle the @IntegerParameter keyword
     * \param[in,out] p : position in the input file
     */
    void handleIntegerParameter(TokensContainer::const_iterator&);
    /*!
     * handle the @UnsignedIntegerParameter keyword
     * \param[in,out] p : position in the input file
     */
    void handleUnsignedIntegerParameter(TokensContainer::const_iterator&);
    /*!
     * handle the @OutputFile keyword
     * \param[in,out] p : position in the input file
     */
    void handleOutputFile(TokensContainer::const_iterator&);
    /*!
     * handle the @OutputFilePrecision keyword
     * \param[in,out] p : position in the input file
     */
    void handleOutputFilePrecision(TokensContainer::const_iterator&);
    /*!
     * handle the @ResidualFile keyword
     * \param[in,out] p : position in the input file
     */
    void handleResidualFile(TokensContainer::const_iterator&);
    /*!
     * handle the @ResidualFilePrecision keyword
     * \param[in,out] p : position in the input file
     */
    void handleResidualFilePrecision(TokensContainer::const_iterator&);
    /*!
     * handle the @ModellingHypothesis keyword
     * \param[in,out] p : position in the input file
     */
    void handleModellingHypothesis(TokensContainer::const_iterator&);
    /*!
     * handle the @Times keyword
     * \param[in,out] p : position in the input file
     */
    void handleTimes(TokensContainer::const_iterator&);
    /*!
     * handle the @Strain keyword
     * \param[in,out] p : position in the input file
     */
    void handleStrain(TokensContainer::const_iterator&);
    /*!
     * handle the @DeformationGradient keyword
     * \param[in,out] p : position in the input file
     */
    void handleDeformationGradient(TokensContainer::const_iterator&);
    /*!
     * handle the @OpeningDisplacement keyword
     * \param[in,out] p : position in the input file
     */
    void handleOpeningDisplacement(TokensContainer::const_iterator&);
    /*!
     * handle the @DrivingVariable keyword
     * \param[in,out] p : position in the input file
     */
    void handleDrivingVariable(TokensContainer::const_iterator&);
    /*!
     * handle the @Stress keyword
     * \param[in,out] p : position in the input file
     */
    void handleStress(TokensContainer::const_iterator&);
    /*!
     * handle the @CohesiveForce keyword
     * \param[in,out] p : position in the input file
     */
    void handleCohesiveForce(TokensContainer::const_iterator&);
    /*!
     * handle the @ThermodynamicForce keyword
     * \param[in,out] p : position in the input file
     */
    void handleThermodynamicForce(TokensContainer::const_iterator&);
    /*!
     * handle the @Behaviour keyword
     * \param[in,out] p : position in the input file
     */
    void handleBehaviour(TokensContainer::const_iterator&);
    /*!
     * handle the @MaterialProperty keyword
     * \param[in,out] p : position in the input file
     */
    void handleMaterialProperty(TokensContainer::const_iterator&);
    /*!
     * handle the @InternalStateVariable keyword
     * \param[in,out] p : position in the input file
     */
    void handleInternalStateVariable(TokensContainer::const_iterator&);
    /*!
     * handle the @ExternalStateVariable keyword
     * \param[in,out] p : position in the input file
     */
    void handleExternalStateVariable(TokensContainer::const_iterator&);
    /*!
     * handle the @ImposedStress keyword
     * \param[in,out] p : position in the input file
     */
    void handleImposedStress(TokensContainer::const_iterator&);
    /*!
     * handle the @ImposedCohesiveForce keyword
     * \param[in,out] p : position in the input file
     */
    void handleImposedCohesiveForce(TokensContainer::const_iterator&);
    /*!
     * handle the @ImposedThermodynamicForce keyword
     * \param[in,out] p : position in the input file
     */
    void handleImposedThermodynamicForce(TokensContainer::const_iterator&);
    /*!
     * handle the @ImposedStrain keyword
     * \param[in,out] p : position in the input file
     */
    void handleImposedStrain(TokensContainer::const_iterator&);
    /*!
     * handle the @ImposedDeformationGradient keyword
     * \param[in,out] p : position in the input file
     */
    void handleImposedDeformationGradient(TokensContainer::const_iterator&);
    /*!
     * handle the @ImposedOpeningDisplacement keyword
     * \param[in,out] p : position in the input file
     */
    void handleImposedOpeningDisplacement(TokensContainer::const_iterator&);
    /*!
     * handle the @ImposedDrivingVariable keyword
     * \param[in,out] p : position in the input file
     */
    void handleImposedDrivingVariable(TokensContainer::const_iterator&);
    /*!
     * handle the @Author keyword
     * \param[in,out] p : position in the input file
     */
    void handleAuthor(TokensContainer::const_iterator&);
    /*!
     * handle the @OutOfBoundsPolicy keyword
     * \param[in,out] p : position in the input file
     */
    void handleOutOfBoundsPolicy(TokensContainer::const_iterator&);
    /*!
     * handle the @Date keyword
     * \param[in,out] p : position in the input file
     */
    void handleDate(TokensContainer::const_iterator&);
    /*!
     * handle the @Description keyword
     * \param[in,out] p : position in the input file
     */
    void handleDescription(TokensContainer::const_iterator&);
    /*!
     * handle the @PredictionPolicy keyword
     * \param[in,out] p : position in the input file
     */
    void 
    handlePredictionPolicy(TokensContainer::const_iterator&);
    /*!
     * handle the @Evolution keyword
     * \param[in,out] p : position in the input file
     */
    void 
    handleEvolution(TokensContainer::const_iterator&);
    /*!
     * handle the @HandleThermalExpansion keyword
     * \param[in,out] p : position in the input file
     */
    void 
    handleHandleThermalExpansion(TokensContainer::const_iterator&);
    /*!
     * \return everything from the given starting point up to the next
     * semi-colon.
     * \param[in,out] p : position in the input file
     */
    std::string
    readUntilEndOfInstruction(TokensContainer::const_iterator&);
    /*!
     * \return a real value
     * \param[in,out] p : position in the input file
     */
    real readDouble(TokensContainer::const_iterator&);
    /*!
     * read a time
     * \param[in,out] p : position in the input file
     */
    real
    readTime(TokensContainer::const_iterator&);
    /*!
     * \brief parse an evolution
     * \param[in]     type : evolution type
     * \param[in,out] p    : position in the input file
     */
    std::shared_ptr<Evolution>
    parseEvolution(const std::string&,
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
     * \param[in,out] p : position in the input file
     */
    void
    readArrayOfSpecifiedSize(std::vector<real>&,
			     TokensContainer::const_iterator&);
    /*!
     * \param[in,out] p : position in the input file
     * \param[out]    n : name of the variable
     */
    void
    setInternalStateVariableValue(TokensContainer::const_iterator&,
				  const std::string&);
    //! t : MTest object
    MTest& t;
    //! callbacks
    std::map<std::string,CallBack> callbacks;
    //! input file
    std::string file;
  }; // end of struct MTestParser

} // end of namespace mfront

#endif /* LIB_MFRONT_MTESTPARSER_H_ */

