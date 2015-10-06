/*! 
 * \file  mfront/include/MTest/MTest.hxx
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

#ifndef LIB_MFRONT_MTEST_H_
#define LIB_MFRONT_MTEST_H_ 

#include<map>
#include<string>
#include<vector>
#include<memory>
#include<fstream>

#include"TFEL/Config/TFELConfig.hxx"

#include"TFEL/Math/tmatrix.hxx"
#include"TFEL/Math/tvector.hxx"
#include"TFEL/Math/stensor.hxx"
#include"TFEL/Math/vector.hxx"
#include"TFEL/Math/LUSolve.hxx"

#include"MTest/Config.hxx"
#include"MTest/SchemeBase.hxx"
#include"MTest/Types.hxx"
#include"MTest/Constraint.hxx"
#include"MTest/AccelerationAlgorithm.hxx"

namespace mtest
{
  
  /*!
   * \brief MTest is a simple class to test mfront behaviours.
   *
   * The unknowns are made of the driving variables stensor (for small
   * strain behaviours) followed by a number of lagrange multiplier.
   */
  struct MTEST_VISIBILITY_EXPORT MTest
    : public SchemeBase
  {
    /*!
     * structure containing the state of the material.
     * This structure is used internally and is declared public only
     * for the python bindings. In particular, the description of the
     * variables given here is only valid during the computations.
     */
    struct MTEST_VISIBILITY_EXPORT MTestCurrentState
    {
      //! default constructor
      MTestCurrentState();
      //! copy constructor
      MTestCurrentState(const MTestCurrentState&);
      MTestCurrentState(MTestCurrentState&&);
      //! destructor
      ~MTestCurrentState() noexcept;
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
      // vector of unknows at the preivous iteration (end of the
      // time step)
      tfel::math::vector<real> u10;
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
      operator=(const MTestCurrentState&) = delete;
      MTestCurrentState&
      operator=(MTestCurrentState&&) = delete;
    };
    /*!
     * structure where usefull variables for the computations are
     * defined.
     * This structure is used internally and is declared
     * public only for the python bindings.
     */
    struct MTEST_VISIBILITY_EXPORT MTestWorkSpace
    {
      MTestWorkSpace();
      ~MTestWorkSpace() noexcept;
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
      MTestWorkSpace(const MTestWorkSpace&) = delete;
      MTestWorkSpace&
      operator=(const MTestWorkSpace&) = delete;
    };
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
    name(void) const override;
    /*! 
     * \return the group of the test
     */
    virtual std::string
    classname(void) const override;
    /*!
     * \param[in] f : file to be read
     */
    virtual void
    readInputFile(const std::string&);
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
    execute(void) override;
    /*!
     * integrate the behaviour over one step
     */ 
    virtual void
    execute(MTestCurrentState&,
	    MTestWorkSpace&,
	    const real,
	    const real);
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
     * \brief set if mtest shall handle thermal expansion coefficient
     * If true, the thermal expansion will be handled if the thermal
     * expansion coefficients are defined.
     * \param[in] b : boolean
     */
    virtual void setHandleThermalExpansion(const bool);
    /*!
     * \brief set the rotation matrix
     * \param[in] r : rotation matrix
     * \param[in] b : allow redefining the rotation matrix 
     */
    virtual void setRotationMatrix(const tfel::math::tmatrix<3u,3u,real>&,
				   const bool = false);
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
     * \brief add a new test
     * \param[in] t : test to be added
     */
    virtual void
    addTest(std::shared_ptr<UTest>);
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
    addConstraint(const std::shared_ptr<Constraint>);
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
    //! \brief set the modelling hypothesis to the default one
    virtual void setDefaultHypothesis(void) override;
    /*!
     * \return the number of unknowns (size of driving variables plus
     * the number of lagrangian multipliers)
     */
    virtual size_t getNumberOfUnknowns(void) const override;
    //! output file precision
    int oprec;
    //! residual file precision
    int rprec;
    //! list of tests
    std::vector<std::shared_ptr<UTest>> tests;
    //! constraints
    std::vector<std::shared_ptr<Constraint>> constraints;
    //! rotation matrix
    tfel::math::tmatrix<3u,3u,real> rm;
    //! boolean, true if the rotation matrix has been defined by the user
    bool isRmDefined;
    //! output file name
    std::string output;
    //! output file
    std::ofstream out;
    //! residual file name
    std::string residualFileName;
    //! file where residuals evolutions as a function of the iteration
    //! number are saved
    std::ofstream residual;
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
    //! handle the computation of thermal expansion
    bool handleThermalExpansion;
    //! test name
    std::string tname;
    //! tangent operator comparison criterium
    real toeps;
    //! perturbation value
    real pv;
    //! compare to numerical jacobian
    bool cto;
  }; // end of struct MTest

} // end of namespace mtest

#endif /* LIB_MFRONT_MTEST_H_ */

