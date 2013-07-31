/*! 
 * \file  MTest.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 09 avril 2013
 */

#ifndef _LIB_MFRONT_MTEST_H_
#define _LIB_MFRONT_MTEST_H_ 

#include<map>
#include<string>
#include<vector>
#include<fstream>

#include"TFEL/Config/TFELConfig.hxx"

#include"TFEL/Math/tmatrix.hxx"
#include"TFEL/Math/tvector.hxx"
#include"TFEL/Math/stensor.hxx"
#include"TFEL/Math/vector.hxx"
#include"TFEL/Material/ModellingHypothesis.hxx"
#include"TFEL/Utilities/SmartPtr.hxx"
#include"TFEL/Utilities/CxxTokenizer.hxx"

#include"MFront/MTestConfig.hxx"
#include"MFront/MTestTypes.hxx"
#include"MFront/MTestConstraint.hxx"
#include"MFront/MTestEvolution.hxx"
#include"MFront/MTestBehaviour.hxx"

#include"TFEL/Tests/Test.hxx"

namespace mfront
{

  /*!
   * MTest is a simple class 
   * to test mfront behaviours
   *
   * The unknowns are made of
   * the strain stensor followed 
   * by a number of lagrange
   * multiplier
   */
  struct MFRONT_MTEST_VISIBILITY_EXPORT MTest
    : public tfel::utilities::CxxTokenizer,
      public tfel::tests::Test
  {
    /*!
     * \brief possible algorithms used for global convergence to
     * update the stiffness matrix
     */
    enum StiffnessUpdatingPolicy{
      /*!
       * the initial prediction matrix or the initial tangent coherent
       * matrix or the initial secant matrix (normally equal to the
       * elastic stiffness) is used for all iterations
       */
      CONSTANTSTIFFNESS,
      /*!
       * the prediction matrix or the first tangent coherent matrix or
       * the first secant matrix is used for all iterations and
       * updated and factorized at at periods
       */
      CONSTANTSTIFFNESSBYPERIOD,
      /*!
       * the tangent coherent matrix or the secant matrix is computed
       * and factorized at each iteration
       */
      UPDATEDSTIFFNESSMATRIX,
      //! default value
      UNSPECIFIEDSTIFFNESSUPDATINGPOLICY
    }; // end of enum StiffnessUpdatingPolicy
    /*!
     * \brief possible prediction policies
     */
    enum PredictionPolicy{
      NOPREDICTION,
      LINEARPREDICTION,
      ELASTICPREDICTION,
      SECANTOPERATORPREDICTION,
      TANGENTOPERATORPREDICTION,
      UNSPECIFIEDPREDICTIONPOLICY
    }; // end of enum PredictionPolicy
    /*!
     * \brief base class for testing the behaviour outputs
     */
    struct UTest
    {
      /*!
       * Type of variable tested
       */
      enum TestedVariable{
	INTERNALSTATEVARIABLE,
	DRIVINGVARIABLE,
	THERMODYNAMICFORCE
      };
      /*!
       * \param[in] e  : strains
       * \param[in] s  : strains
       * \param[in] iv : internal state variables
       * \param[in] t  : time
       * \param[in] dt : time increment
       * \param[in] p  : period
       */
      virtual void
      check(const tfel::math::vector<real>&,
	    const tfel::math::vector<real>&,
	    const tfel::math::vector<real>&,
	    const real,
	    const real,
	    const unsigned int) = 0;
      /*!
       * \return the results of the test
       */
      virtual tfel::tests::TestResult
      getResults() const = 0;
      //! desctructor
      virtual ~UTest();
    };
    /*!
     * default constructor
     */
    MTest(void);
    /*!
     * read the input file
     * \param[in] f : file
     */
    MTest(const std::string&);
    /*! 
     * \return the name of the test
     */
    virtual std::string
    name(void) const;
    /*! 
     * \return the group of the test
     */
    virtual std::string
    classname(void) const;
    /*!
     * integrate the behaviour
     * along the loading path
     */ 
    virtual tfel::tests::TestResult
    execute(void);
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
  protected:
    //! a simple alias
    typedef void (MTest::* CallBack)(TokensContainer::const_iterator&);
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
     * handle the @seCastemAccelerationAlgorithm keyword
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
     * \brief declare a new variable
     * \param[in] v : variable name
     */
    void declareVariable(const std::string&);
    /*!
     * \brief declare a list of new variables
     * \param[in] v : variable names
     */
    void declareVariables(const std::vector<std::string>&);
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
    real
    readDouble(TokensContainer::const_iterator&);
    /*!
     * read a time
     * \param[in,out] p : position in the input file
     */
    real
    readTime(TokensContainer::const_iterator&);
    /*!
     * \return the type and the position of the given variable
     * \param[out] type : type of the variable
     * \param[out] pos  : position of the variable
     * \param[in]  n    : name of the variable
     */
    void
    getVariableTypeAndPosition(UTest::TestedVariable&,
			       unsigned short&,
			       const std::string&);
    /*!
     * output the results
     * \param[in] t  : time
     * \param[in] N  : tensor size
     * \param[in] u  : unknowns
     * \param[in] s  : stresses
     * \param[in] iv : internal state variables
     */
    void printOutput(const real,
		     const unsigned short,
		     const tfel::math::vector<real>&,
		     const tfel::math::vector<real>&,
		     const tfel::math::vector<real>&);
    /*!
     * \brief parse an evolution
     * \param[in]     t : evolution type
     * \param[in,out] p : position in the input file
     */
    tfel::utilities::shared_ptr<MTestEvolution>
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
     * \brief set the hypothesis to the default one
     */
    void setDefaultHypothesis(void);
    //! input file
    const std::string file;
    //! output file precision
    int oprec;
    //! list of tests
    std::vector<tfel::utilities::shared_ptr<UTest> > tests;
    //! callbacks
    std::map<std::string,CallBack> callbacks;
    //! declared variable names
    std::vector<std::string> vnames;
    //! the mechanical behaviour
    tfel::utilities::shared_ptr<MTestBehaviour> b;
    //! constraints
    std::vector<tfel::utilities::shared_ptr<MTestConstraint> > constraints;
    //! rotation matrix
    tfel::math::tmatrix<3u,3u,real> rm;
    //! boolean, true if the rotation matrix has been defined by the user
    bool isRmDefined;
    //! times
    std::vector<real> times;
    //! list of evolutions
    tfel::utilities::shared_ptr<std::map<std::string,	
				 tfel::utilities::shared_ptr<MTestEvolution> > > evs;
    //! output file name
    std::string output;
    //! output file
    std::ofstream out;
    //! dimension used for the computation
    unsigned short dimension;
    //! modelling hypothesis
    tfel::material::ModellingHypothesis::Hypothesis hypothesis;
    //! list of internal variable names, including their suffixes
    std::vector<std::string> ivfullnames;
    // inital values of the strains
    std::vector<real> e_t0;
    // inital values of the stresses
    std::vector<real> s_t0;
    // inital values of the internal state variables
    std::vector<real> iv_t0;
    /*!
     * criterium value on strains used to stop the
     * Newton-Raphson algorithm.
     * By default, a value of 1.e-12 is used.
     */
    real eeps;
    /*!
     * criterium value on stresses used to stop the
     * Newton-Raphson algorithm.
     * By default, a value of 1.e-3 is used. This value is suitable
     * for stresses expresses in Pa.
     */
    real seps;
    //! maximum number of sub steps allowed
    int mSubSteps;
    //! maximum number of iterations allowed in the Newton-Raphson algorithm
    int iterMax;
    //! stiffness updating policy
    StiffnessUpdatingPolicy ks;
    //! type of stifness matrix to use for the resolution
    MTestStiffnessMatrixType::mtype ktype;
    //! use a prediction matrix before beginning the resolution
    PredictionPolicy ppolicy;
    //! use castem acceleration
    bool useCastemAcceleration;
    //! castem acceleration trigger
    int cat;
    //! castem acceleration period
    int cap;
    //! description of the test
    std::string description;
    //! author
    std::string author;
    //! date
    std::string date;
  }; // end of struct MTest

} // end of namespace mfront

#endif /* _LIB_MFRONT_MTEST_H */

