/*!
 * \file   mtest/include/MTest/PipeTest.hxx
 * \brief    
 * \author THOMAS HELFER
 * \date   24 nov. 2015
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_MTEST_PIPETEST_HXX_
#define _LIB_MTEST_PIPETEST_HXX_

#include<string>
#include<vector>

#include"TFEL/Math/vector.hxx"
#include"TFEL/Material/ModellingHypothesis.hxx"

#include"MTest/Config.hxx"
#include"MTest/Types.hxx"
#include"MTest/PipeProfileHandler.hxx"
#include"MTest/PipeMesh.hxx"
#include"MTest/SingleStructureScheme.hxx"

namespace mtest{

  // forward declaration
  struct PipeProfileHandler;
  
  /*!
   * a study describing mechanical tests on pipes
   */
  struct MTEST_VISIBILITY_EXPORT PipeTest
    : public SingleStructureScheme
  {
    //! how the pipe is modelled
    enum PipeModellingHypothesis{
      DEFAULT,
      ENDCAPEFFECT,
      IMPOSEDAXIALFORCE,
      NONE
    }; // end of enum PipeModellingHypothesis
    //! a simple alias
    using size_type = tfel::math::vector<real>::size_type;
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
     * integrate the behaviour
     * along the loading path
     */ 
    virtual tfel::tests::TestResult
    execute(void) override;
    /*!
     * integrate the behaviour over one step
     * \param[out] s: current structure state
     * \param[out] wk: workspace
     * \param[in]  t:  current time
     * \param[in]  dt: time increment
     */ 
    virtual void
    execute(StudyCurrentState&,
	    SolverWorkSpace&,
	    const real,
	    const real) const;
    /*!
     * \brief set the pipe inner radius
     * \param[in] r: radius value
     */
    virtual void setInnerRadius(const real);
    /*!
     * \brief set the pipe outer radius
     * \param[in] r: radius value
     */
    virtual void setOuterRadius(const real);
    /*!
     * \brief set the number of elements
     * \param[in] n: number of elements
     */
    virtual void setNumberOfElements(const int);
    /*!
     * \brief set the pipe modelling hypothesis
     * \param[in] ph: pipe modelling hypothesis
     */
    virtual void setPipeModellingHypothesis(const PipeModellingHypothesis);
    /*!
     * \brief set the element type
     * \param[in] e: element type
     */
    virtual void setElementType(const PipeMesh::ElementType);
    /*!
     * \brief set the inner pressure evolution
     * \param[in] p : pressure evolution
     */
    virtual void setInnerPressureEvolution(std::shared_ptr<Evolution>);
    /*!
     * \brief set the outer pressure evolution
     * \param[in] p : pressure evolution
     */
    virtual void setOuterPressureEvolution(std::shared_ptr<Evolution>);
    /*!
     * \brief set the axial force evolution
     * \param[in] f : axial force evolution
     */
    virtual void setAxialForceEvolution(std::shared_ptr<Evolution>);
    /*!
     * \brief set displacement criterion value
     * \param[in] e: criterion value
     */
    virtual void setDisplacementEpsilon(const real);
    /*!
     * \brief set criterium value for the convergence test on the
     * residual
     * \param[in] e : criterion value
     */
    virtual void setResidualEpsilon(const real);
    /*!
     * complete the initialisation. This method must be called once.
     * \note this method is called automatically by the execute method.
     */ 
    virtual void completeInitialisation(void) override;
    /*!
     * \return the total number of unknowns
     */
    virtual size_type
    getNumberOfUnknowns(void) const override;
    /*!
     * \return the total number of nodes
     */
    virtual size_type
    getNumberOfNodes(void) const;
    /*!
     * \brief initialize the current state
     * \param[in] s : current state
     */
    virtual void
    initializeCurrentState(StudyCurrentState&) const override;
    /*!
     * \brief initialize the workspace
     * \param[in] wk : workspace
     */
    virtual void
    initializeWorkSpace(SolverWorkSpace&) const override;
    /*!
     * \brief update current state at the beginning of a new time step:
     * - update the material properties
     * - update the external state variables
     * - compute the thermal expansion if mandatory
     * \param[out] s: current structure state
     * \param[in]  t: current time
     * \param[in] dt: time increment
     */
    virtual void prepare(StudyCurrentState&,
			 const real,
			 const real) const override;
    /*!
     * \brief make a linear prediction of the unknows and state
     * \param[out] s: current structure state
     * \param[in] dt: time increment
     */
    virtual void makeLinearPrediction(StudyCurrentState&,
				      const real) const override;
    /*!
     * \brief compute the stiffness matrix and the residual
     * \param[out] s: current structure state
     * \param[out] K:   tangent operator
     * \param[out] r:   residual
     * \param[in]  t:   current time
     * \param[in]  dt:  time increment
     * \param[in]  smt: type of tangent operator
     * \note the memory has already been allocated
     */
    virtual bool
    computePredictionStiffnessAndResidual(StudyCurrentState&,
					  tfel::math::matrix<real>&,
					  tfel::math::vector<real>&,
					  const real&,
					  const real&,
					  const StiffnessMatrixType) const override;
    /*!
     * \brief compute the stiffness matrix and the residual
     * \param[out] s: current structure state
     * \param[out] K:   tangent operator
     * \param[out] r:   residual
     * \param[in]  t:   current time
     * \param[in]  dt:  time increment
     * \param[in]  smt: type of tangent operator
     * \note the memory has already been allocated
     */
    virtual bool
    computeStiffnessMatrixAndResidual(StudyCurrentState&,
				      tfel::math::matrix<real>&,
				      tfel::math::vector<real>&,
				      const real,
				      const real,
				      const StiffnessMatrixType) const override;
    /*!
     * \param[in] : du unknows increment difference between two iterations
     */
    virtual real getErrorNorm(const tfel::math::vector<real>&) const override;
    /*!
     * \param[in]  s: current structure state
     * \param[in] du: unknows increment estimation
     * \param[in] r:  residual
     * \param[in] o:  solver options
     * \param[in] i:  iteration number
     * \param[in] t:  current time
     * \param[in] dt: time increment
     * \return a boolean saying if all convergence criteria are met
     */
    virtual bool checkConvergence(const StudyCurrentState&,
				  const tfel::math::vector<real>&,
				  const tfel::math::vector<real>&,
				  const SolverOptions&,
				  const unsigned int,
				  const real,
				  const real) const override;
    /*!
     * \param[in]  s: current structure state
     * \param[in] du: unknows increment estimation
     * \param[in] r:  residual
     * \param[in] o:  solver options
     * \param[in] t:  current time
     * \param[in] dt: time increment
     * \return a description of all the criteria that were not met.
     */
    virtual std::vector<std::string>
    getFailedCriteriaDiagnostic(const StudyCurrentState&,
				const tfel::math::vector<real>&,
				const tfel::math::vector<real>&,
				const SolverOptions&,
				const real,
				const real) const override;
    /*!
     * \param[out] s: current structure state
     * \param[in]  t:  current time
     * \param[in]  dt: time increment
     * \param[in]  p:  period
     */
    virtual void
    postConvergence(StudyCurrentState&,
		    const real,
		    const real,
		    const unsigned int) const override;
    /*!
     * \brief add a new profile postprocessing
     * \param[in] f: file name
     * \param[in] c: components
     */
    virtual void
    addProfile(const std::string&,const std::vector<std::string>&);
    /*!
     * \param[in] h : modelling hypothesis
     */
    virtual void
    setModellingHypothesis(const std::string&) override;
    //! \brief set the modelling hypothesis to the default one
    virtual void setDefaultModellingHypothesis(void) override;
    //! \brief turn hpp to true
    virtual void performSmallStrainAnalysis(void);
    /*!
     * \brief print usefull information in the output file 
     * \param[in] t  : time
     * \param[in] state  : current state
     */
    virtual void printOutput(const real,const StudyCurrentState&) override;
    //! destructor
    virtual ~PipeTest();
  private:
    //! a simple alias
    using ModellingHypothesis = tfel::material::ModellingHypothesis;
    //! registred profile
    std::vector<PipeProfileHandler> profiles;
    //! mesh data
    PipeMesh mesh;
    //! axial force evolution
    std::shared_ptr<Evolution> axial_force;
    //! inner pressure
    std::shared_ptr<Evolution> inner_pressure;
    //! outer pressure
    std::shared_ptr<Evolution> outer_pressure;
    //! pipe modelling hypothesis
    PipeModellingHypothesis pmh = DEFAULT;
    //! element type
    //! small strain hypothesis
    bool hpp = false;
  }; // end of struct PipeTest
  
} // end of namespace mtest

#endif /* _LIB_MTEST_PIPETEST_HXX_ */
