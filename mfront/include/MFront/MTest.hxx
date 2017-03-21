/*! 
 * \file  mfront/include/MFront/MTest.hxx
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
#include"TFEL/Math/LUSolve.hxx"
#include"TFEL/Utilities/SmartPtr.hxx"
#include"TFEL/Material/ModellingHypothesis.hxx"
#include"TFEL/Material/OutOfBoundsPolicy.hxx"

#include"MFront/MTestConfig.hxx"
#include"MFront/MTestTypes.hxx"
#include"MFront/MTestConstraint.hxx"
#include"MFront/MTestEvolution.hxx"
#include"MFront/MTestBehaviour.hxx"
#include"MFront/MTestAccelerationAlgorithm.hxx"

#include"TFEL/Tests/Test.hxx"

namespace mfront
{
  
  /*!
   * \brief MTest is a simple class to test mfront behaviours.
   *
   * The unknowns are made of the driving variables stensor (for small
   * strain behaviours) followed by a number of lagrange multiplier.
   */
  struct MFRONT_MTEST_VISIBILITY_EXPORT MTest
    : public tfel::tests::Test
  {
    /*!
     * structure containing the state of the material.
     * This structure is used internally and is declared public only
     * for the python bindings. In particular, the description of the
     * variables given here is only valid during the computations.
     */
    struct MTestCurrentState
    {
      /*!
       * default constructor
       */
      MTestCurrentState();
      /*!
       * copy constructor
       */
      MTestCurrentState(const MTestCurrentState&);
      // vector of unknows at 
      // the beginning of the
      // previous time step.
      tfel::math::vector<real> u_1;
      // vector of unknows at the beginning of the
      // time step.
      tfel::math::vector<real> u0;
      // vector of unknows at the end of the
      // time step
      tfel::math::vector<real> u1;
      // thermodynamic forces at the beginning of the previous of the time step
      tfel::math::vector<real> s_1;
      // thermodynamic forces at the beginning of the time step
      tfel::math::vector<real> s0;
      // thermodynamic forces at the end of the time step
      tfel::math::vector<real> s1;
      // driving variables at the beginning of the time step
      tfel::math::vector<real> e0;
      // driving variables at the end of the time step
      tfel::math::vector<real> e1;
      // thermal strain at the beginning of the time step
      // (small strain behaviours)
      tfel::math::vector<real> e_th0;
      // thermal strain at the end of the time step
      // (small strain behaviours)
      tfel::math::vector<real> e_th1;
      // material properties at
      // the beginning of the
      // time step
      tfel::math::vector<real> mprops0;
      // material properties at
      // the end of the
      // time step
      tfel::math::vector<real> mprops1;
      // internal variables at
      // the beginning of the
      // previous time step
      tfel::math::vector<real> iv_1;
      // internal variables at
      // the beginning of the
      // time step
      tfel::math::vector<real> iv0;
      // internal variables at
      // the end of the
      // time step
      tfel::math::vector<real> iv1;
      // external variables at
      // the beginning of the
      // time step
      tfel::math::vector<real> esv0;
      // external variables increments
      tfel::math::vector<real> desv;
      // current period number
      unsigned int period;
      // previous time step
      real dt_1;
      // reference Temperature
      real Tref;
    private:
      MTestCurrentState&
      operator=(const MTestCurrentState&);
    };
    /*!
     * structure where usefull variables for the computations are
     * defined.
     * This structure is used internally and is declared
     * public only for the python bindings.
     */
    struct MTestWorkSpace
    {
      MTestWorkSpace();
      // stiffness tensor
      tfel::math::matrix<real> Kt;
      // numertical stiffness tensor
      tfel::math::matrix<real> nKt;
      // temporary variable used for numerical tangent operator
      // computation
      tfel::math::matrix<real> tKt;
      // prediction tensor
      tfel::math::matrix<real> Kp;
      //! stiffness matrix
      tfel::math::matrix<real> K;
      // residual
      tfel::math::vector<real> r;
      // unknowns correction
      tfel::math::vector<real> du;
      // permuation matrix
      tfel::math::Permutation<tfel::math::matrix<real>::size_type> p_lu;
      // temporary vector used by the LUSolve::exe function
      tfel::math::vector<real> x;
      // temporary variable used for numerical tangent operator
      // computation
      tfel::math::vector<real> s1;
      // temporary variable used for numerical tangent operator
      // computation
      tfel::math::vector<real> s2;
      // temporary variable used for numerical tangent operator
      // computation
      tfel::math::vector<real> statev;
      //
      bool first;
      real a;
    private:
      MTestWorkSpace(const MTestWorkSpace&);
      MTestWorkSpace&
      operator=(const MTestWorkSpace&);
    };
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
      ELASTICPREDICTIONFROMMATERIALPROPERTIES,
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
       * \param[in] e  : driving variabless
       * \param[in] s  : thermodynamic forces
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
     * complete the initialisation. This method must be called once.
     * \note this method is called automatically by the execute method.
     */ 
    virtual void
    completeInitialisation(void);
    /*!
     * \brief initialize the current state
     * \param[in] s : current state
     */
    virtual void
    initializeCurrentState(MTestCurrentState&) const;
    /*!
     * \brief initialize the workspace
     * \param[in] wk : workspace
     */
    virtual void
    initializeWorkSpace(MTestWorkSpace&) const;
    /*!
     * integrate the behaviour
     * along the loading path
     */ 
    virtual tfel::tests::TestResult
    execute(void);
    /*!
     * integrate the behaviour over one step
     */ 
    virtual void
    execute(MTestCurrentState&,
	    MTestWorkSpace&,
	    const real,
	    const real);
    /*!
     * \brief set the description
     * \param[in] d : description
     */
    virtual void setDescription(const std::string&);
    /*!
     * \brief set the author
     * \param[in] a : author
     */
    virtual void setAuthor(const std::string&);
    /*!
     * \brief set the out of bounds policy
     * \param[in] a : out of bounds policy
     */
    virtual void setOutOfBoundsPolicy(const tfel::material::OutOfBoundsPolicy);
    /*!
     * \brief set the date
     * \param[in] d : date
     */
    virtual void setDate(const std::string&);
    /*!
     * \brief set the prediction policy
     * \param[in] p : prediction policy
     */
    virtual void setPredictionPolicy(const MTest::PredictionPolicy);
    /*!
     * \brief set the stiffness matrix type
     * \param[in] k : stiffness matrix type
     */
    virtual void setStiffnessMatrixType(const MTestStiffnessMatrixType::mtype);
    /*!
     * \brief ask the comparison to the numerical tangent operator
     * \param[in] bo : boolean
     */
    virtual void setCompareToNumericalTangentOperator(const bool);
    /*!
     * \brief set the value used to compare the tangent operator given
     * by the behaviour to the numerical tangent operator
     * \param[in] v : value
     */
    virtual void setTangentOperatorComparisonCriterium(const real);
    /*!
     * \brief set the value used to build the numerical tangent operator
     * \param[in] v : perturbation value
     */
    virtual void setNumericalTangentOperatorPerturbationValue(const real);
    /*!
     * \brief set the stiffness updating policy
     * \param[in] b : boolean
     */
    virtual void
    setStiffnessUpdatingPolicy(const StiffnessUpdatingPolicy);
    /*!
     * \brief set the use of the castem acceleration algorithm
     * \param[in] b : boolean
     */
    virtual void setUseCastemAccelerationAlgorithm(const bool);
    /*!
     * \brief set if mtest shall handle thermal expansion coefficient
     * If true, the thermal expansion will be handled if the thermal
     * expansion coefficients are defined.
     * \param[in] b : boolean
     */
    virtual void setHandleThermalExpansion(const bool);
    /*!
     * \brief set the acceleration algorithm to be used
     * \param[in] a : acceleration algorithm
     */
    virtual void setAccelerationAlgorithm(const std::string&);
    /*!
     * \brief set a parameter of the acceleration algorithm
     * \param[in] p : parameter name
     * \param[in] v : parameter value
     */
    virtual void setAccelerationAlgorithmParameter(const std::string&,
						   const std::string&);
    /*!
     * \brief set at which iteration the use of the castem
     * acceleration algorithm  will begin
     * \param[in] i : iteration number
     */
    virtual void setCastemAccelerationTrigger(const int);
    /*!
     * \brief set at which period the use of the castem
     * acceleration algorithm  will take place
     * \param[in] p : period
     */
    virtual void setCastemAccelerationPeriod(const int);
    virtual void setMaximumNumberOfIterations(const unsigned int);
    virtual void setMaximumNumberOfSubSteps(const unsigned int);
    virtual void setRotationMatrix(const tfel::math::tmatrix<3u,3u,real>&);
    /*!
     * \brief set the behaviour
     * \param[in] i : interface
     * \param[in] l : library name
     * \param[in] f : function
     */
    virtual void
    setBehaviour(const std::string&,
		 const std::string&,
		 const std::string&);
    /*!
     * \return true if the output file name has been set
     */
    virtual bool hasOutputFileName() const;
    /*!
     * \brief set the output file
     * \param[in] f : file name
     */
    virtual void
    setOutputFileName(const std::string&);
    /*!
     * \brief set the output file precision
     * \param[in] p : precision
     */
    virtual void
    setOutputFilePrecision(const unsigned int);
    /*!
     * \return true if the output file name has been set
     */
    virtual bool hasResidualFileName() const;
    /*!
     * \brief set the residual file
     * \param[in] f : file name
     */
    virtual void
    setResidualFileName(const std::string&);
    /*!
     * \brief set the residual file precision
     * \param[in] p : precision
     */
    virtual void
    setResidualFilePrecision(const unsigned int);
    /*!
     * \brief set criterium value for the convergence test on the on
     * the driving variable
     * \param[in] e : criterium
     */
    virtual void
    setDrivingVariableEpsilon(const double);
    /*!
     * \brief set criterium value for the convergence test on the
     * thermodynamic forces
     * \param[in] e : criterium
     */
    virtual void
    setThermodynamicForceEpsilon(const double);
    /*!
     * \param[in] n : parameter name
     * \param[in] v : parameter value
     */
    virtual void
    setParameter(const std::string&,
		 const double);
    /*!
     * \param[in] n : parameter name
     * \param[in] v : parameter value
     */
    virtual void
    setIntegerParameter(const std::string&,
			const int);
    /*!
     * \param[in] n : parameter name
     * \param[in] v : parameter value
     */
    virtual void
    setUnsignedIntegerParameter(const std::string&,
				const unsigned int);
    /*!
     * \param[in] h : modelling hypothesis
     */
    virtual void
    setModellingHypothesis(const std::string&);
    /*!
     * \param[in] t : times
     */
    virtual void
    setTimes(const std::vector<real>&);
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
     * \return the list of evolutions
     */
    tfel::utilities::shared_ptr<MTestEvolutionManager>
    getEvolutions() const;
    /*!
     * \brief add a new test
     * \param[in] t : test to be added
     */
    virtual void
    addTest(tfel::utilities::shared_ptr<UTest>);
    /*!
     * \brief add a new evolution
     * \param[in] n  : evolution name
     * \param[in] p  : evolution pointer
     * \param[in] b1 : declare the variable.
     * \param[in] b2 : check if the evolution already exists.
     * \note if b1 is false, we check that the variable is already
     * declared. Otherwise, an exeception is thrown.
     * \note if b2 is true and the evolution already exists, an
     * exeception is thrown.
     */
    virtual void
    addEvolution(const std::string&,
		 const MTestEvolutionPtr,
		 const bool,
		 const bool);
    /*!
     * \brief set evolution value for a given time
     * \param[in] n  : evolution name
     * \param[in] t  : time
     * \param[in] v  : value
     *
     * \note the evolution *must* be of type MTestLPIEvolution
     */
    virtual void
    setEvolutionValue(const std::string&,
		      const real,
		      const real);
    /*!
     * \brief define a material property
     * \param[in] n     : evolution name
     * \param[in] p     : evolution pointer
     * \param[in] check : check if the evolution already exists.
     * \note if check is true and the evolution already exists, an
     * exeception is thrown.
     */
    virtual void
    setMaterialProperty(const std::string&,
			const MTestEvolutionPtr,
			const bool);
    /*!
     * \brief define an external state variable
     * \param[in] n     : evolution name
     * \param[in] p     : evolution pointer
     * \param[in] check : check if the evolution already exists.
     * \note if check is true and the evolution already exists, an
     * exeception is thrown.
     */
    virtual void
    setExternalStateVariable(const std::string&,
			     const MTestEvolutionPtr,
			     const bool);
    /*!
     * \brief set the inital values of the driving variable
     * \param[in] v : values
     */
    virtual void
    setDrivingVariablesInitialValues(const std::vector<real>&);
    /*!
     * \brief set the inital values of the thermodynamic forces
     * \param[in] v : values
     */
    virtual void
    setThermodynamicForcesInitialValues(const std::vector<real>&);
    /*!
     * \brief set the inital value of a scalar variable
     * \param[in] v : value
     */
    virtual void
    setScalarInternalStateVariableInitialValue(const std::string&,
					       const real);
    /*!
     * \brief set the inital values of a symetric tensor variable
     * \param[in] v : values
     */
    virtual void
    setStensorInternalStateVariableInitialValues(const std::string&,
						 const std::vector<real>&);
    /*!
     * \brief set the inital values of a tensor variable
     * \param[in] v : values
     */
    virtual void
    setTensorInternalStateVariableInitialValues(const std::string&,
						const std::vector<real>&);
    /*!
     * \brief add a new constraint
     * \param[in] c     : constraint
     */
    virtual void
    addConstraint(const tfel::utilities::shared_ptr<MTestConstraint>);
    /*!
     * \return the behaviour type
     */
    virtual tfel::material::MechanicalBehaviourBase::BehaviourType
    getBehaviourType(void) const;
    /*!
     * \return the behaviour
     */
    virtual tfel::utilities::shared_ptr<MTestBehaviour>
    getBehaviour(void);
    /*!
     * \return the modelling hypothesis
     */
    virtual tfel::material::ModellingHypothesis::Hypothesis
    getModellingHypothesis(void) const;
    /*!
     * \return the dimension
     */
    virtual unsigned short
    getDimension(void) const;
    /*!
     * \brief print the driving variable, 
     * \param[in] t  : time
     * \param[in] u  : unknowns (driving variable)
     * \param[in] s  : thermodynamic forces
     * \param[in] iv : internal state variables
     */
    virtual void
    printOutput(const real,const MTestCurrentState&);
    /*!
     * destructor
     */
    ~MTest();
  protected:
    /*!
     * \brief declare a new variable
     * \param[in] v : variable name
     */
    void declareVariable(const std::string&,
			 const bool);
    /*!
     * \brief declare a list of new variables
     * \param[in] v : variable names
     */
    void declareVariables(const std::vector<std::string>&,
			  const bool);
    /*!
     * \brief set the modelling hypothesis to the default one
     */
    void setDefaultHypothesis(void);
    /*!
     * \return the number of unknowns (size of driving variables plus
     * the number of lagrangian multipliers)
     */
    size_t getNumberOfUnknowns(void) const;
    //! output file precision
    int oprec;
    //! residual file precision
    int rprec;
    //! list of tests
    std::vector<tfel::utilities::shared_ptr<UTest> > tests;
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
    tfel::utilities::shared_ptr<MTestEvolutionManager> evm;
    //! default values for material properties as given by the behaviour
    tfel::utilities::shared_ptr<MTestEvolutionManager> defaultMaterialPropertiesValues;
    //! output file name
    std::string output;
    //! output file
    std::ofstream out;
    //! residual file name
    std::string residualFileName;
    //! file where residuals evolutions as a function of the iteration
    //! number are saved
    std::ofstream residual;
    //! dimension used for the computation
    unsigned short dimension;
    //! modelling hypothesis
    tfel::material::ModellingHypothesis::Hypothesis hypothesis;
    //! list of internal variable names, including their suffixes
    std::vector<std::string> ivfullnames;
    // inital values of the driving variables
    std::vector<real> e_t0;
    // inital values of the thermodynamic forces
    std::vector<real> s_t0;
    // inital values of the internal state variables
    std::vector<real> iv_t0;
    /*!
     * criterium value on driving variables used to stop the
     * Newton-Raphson algorithm.
     * By default, a value of 1.e-12 is used.
     */
    real eeps;
    /*!
     * criterium value on thermodynamic forces used to stop the
     * Newton-Raphson algorithm.
     * By default, a value of 1.e-3 is used. This value is suitable
     * for thermodynamic forces expresses in Pa (small strain behaviours).
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
    //! acceleration algorithm
    tfel::utilities::shared_ptr<MTestAccelerationAlgorithm> aa;
    //! handle the computation of thermal expansion
    bool handleThermalExpansion;
    //! use castem acceleration
    bool useCastemAcceleration;
    //! description of the test
    std::string description;
    //! author
    std::string author;
    //! date
    std::string date;
    //! test name
    std::string tname;
    //! tangent operator comparison criterium
    real toeps;
    //! perturbation value
    real pv;
    //! compare to numerical jacobian
    bool cto;
    //! initilisation stage
    bool initialisationFinished;
  }; // end of struct MTest

} // end of namespace mfront

#endif /* _LIB_MFRONT_MTEST_H */

