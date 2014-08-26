/*! 
 * \file  MTestBehaviour.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 05 avril 2013
 */

#ifndef _LIB_MFRONT_MTESTBEHAVIOUR_H_
#define _LIB_MFRONT_MTESTBEHAVIOUR_H_ 

#include<vector>
#include<string>

#include"TFEL/Math/matrix.hxx"
#include"TFEL/Math/tmatrix.hxx"
#include"TFEL/Math/vector.hxx"

#include"TFEL/Material/ModellingHypothesis.hxx"
#include"TFEL/Material/MechanicalBehaviour.hxx"

#include"MFront/MTestTypes.hxx"

namespace mfront
{

  /*!
   * A simple wrapper around mechanical beheaviours
   */
  struct MTestBehaviour
  {
    /*!
     * \return the type of the behaviour
     */
    virtual tfel::material::MechanicalBehaviourBase::BehaviourType
    getBehaviourType(void) const = 0;
    /*!
     * \return the size of a vector able to contain all the components of the driving variables
     * \param[in] h : modelling hypothesis
     */
    virtual unsigned short
    getDrivingVariablesSize(const tfel::material::ModellingHypothesis::Hypothesis) const = 0;
    /*!
     * \param[out] v : initial values of the driving variables
     * \note : the vector shall have been correctly allocated
     */
    virtual void
    getDrivingVariablesDefaultInitialValues(tfel::math::vector<real>&) const = 0;
    /*!
     * \return the size of a vector able to contain all the components of the thermodynamic forces
     * \param[in] h : modelling hypothesis
     */
    virtual unsigned short
    getThermodynamicForcesSize(const tfel::material::ModellingHypothesis::Hypothesis) const = 0;
    /*!
     * \param[in] c : components
     * \param[in] h : modelling hypothesis
     */
    virtual void
    getStensorComponentsSuffixes(std::vector<std::string>&,
				 const tfel::material::ModellingHypothesis::Hypothesis) const = 0;
    /*!
     * \param[in] c : components
     * \param[in] h : modelling hypothesis
     */
    virtual void
    getDrivingVariablesComponents(std::vector<std::string>&,
				  const tfel::material::ModellingHypothesis::Hypothesis) const = 0;
    /*!
     * \param[in] c : components
     * \param[in] h : modelling hypothesis
     */
    virtual void
    getThermodynamicForcesComponents(std::vector<std::string>&,
				     const tfel::material::ModellingHypothesis::Hypothesis) const = 0;
    /*!
     * \param[in] h : modelling hypothesis
     * \param[in] c : component
     */
    virtual unsigned short
    getDrivingVariableComponentPosition(const tfel::material::ModellingHypothesis::Hypothesis,
					const std::string&) const = 0;
    /*!
     * \param[in] h : modelling hypothesis
     * \param[in] c : component
     */
    virtual unsigned short
    getThermodynamicForceComponentPosition(const tfel::material::ModellingHypothesis::Hypothesis,
					   const std::string&) const = 0;
    /*!
     * \return the type of the behaviour
     * 0 means that the behaviour is isotropic.
     * 1 means that the behaviour is orthotropic.
     */
    virtual unsigned short
    getSymmetryType(void) const = 0;
    /*!
     * \return the number of material properties
     */
    virtual std::vector<std::string>
    getMaterialPropertiesNames(void) const = 0;
    /*!
     * \return the number of internal variables
     */
    virtual std::vector<std::string>
    getInternalStateVariablesNames(void) const  = 0;
    /*!
     * \return the size of the array of internal variables
     * \param[in] h : modelling hypothesis
     */
    virtual size_t
    getInternalStateVariablesSize(const tfel::material::ModellingHypothesis::Hypothesis) const = 0;
    /*!
     * \return the descriptions the internal variables
     * \param[in] h : modelling hypothesis
     */
    virtual std::vector<std::string>
    getInternalStateVariablesDescriptions(const tfel::material::ModellingHypothesis::Hypothesis) const = 0;
    /*!
     * \return the type of an internal variable
     * \param[in] n : internal variable name
     */
    virtual unsigned short
    getInternalStateVariableType(const std::string&) const = 0;
    /*!
     * \return the position of an internal variable
     * \param[in] h : modelling hypothesis
     * \param[in] n : internal variable name
     */
    virtual unsigned short
    getInternalStateVariablePosition(const tfel::material::ModellingHypothesis::Hypothesis,
				     const std::string&) const = 0;
    /*!
     * \return the number of external variables
     */
    virtual std::vector<std::string>
    getExternalStateVariablesNames(void) const  = 0;
    /*!
     * \brief set the value of a parameter
     * \param[in] n : name of the parameter
     * \param[in] v : value
     */
    virtual void
    setParameter(const std::string&,
		 const real) const  = 0;
    /*!
     * \brief set the value of a parameter
     * \param[in] n : name of the parameter
     * \param[in] v : value
     */
    virtual void
    setIntegerParameter(const std::string&,
			const int) const  = 0;
    /*!
     * \brief set the value of a parameter
     * \param[in] n : name of the parameter
     * \param[in] v : value
     */
    virtual void
    setUnsignedIntegerParameter(const std::string&,
				const unsigned short) const  = 0;
    /*!
     * \brief allocate internal workspace
     * \param[in] h : modelling hypothesis
     */
    virtual void
    allocate(const tfel::material::ModellingHypothesis::Hypothesis) = 0;
    /*!
     * \return the default type of stiffness matrix used by the behaviour
     */
    virtual MTestStiffnessMatrixType::mtype
    getDefaultStiffnessMatrixType(void) const = 0;
    /*!
     * \brief integrate the mechanical behaviour over the time step
     * \return true if the integration was successfull, false otherwise
     * \param[out] Kt    : tangent operator
     * \param[in]  r     : rotation matrix
     * \param[in]  e0    : strain at the beginning of the time step
     * \param[in]  s0    : stresses at the beginning of the time step
     * \param[in]  mp    : material properties at the beginning of the time step
     * \param[in]  iv0   : internal state variables at the beginning of the time step
     * \param[in]  ev0   : external state variables at the beginning of the time step
     * \param[in]  h     : modelling hypothesis
     * \param[in]  dt    : time increment
     * \param[in]  ktype : type of the stiffness matrix
     */
    virtual bool
    computePredictionOperator(tfel::math::matrix<real>&,
			      const tfel::math::tmatrix<3u,3u,real>&,
			      const tfel::math::vector<real>&,
			      const tfel::math::vector<real>&,
			      const tfel::math::vector<real>&,
			      const tfel::math::vector<real>&,
			      const tfel::math::vector<real>&,
			      const tfel::material::ModellingHypothesis::Hypothesis,
			      const MTestStiffnessMatrixType::mtype) const = 0;
    /*!
     * \brief integrate the mechanical behaviour over the time step
     * \return true if the integration was successfull, false otherwise
     * \param[out] Kt    : tangent operator
     * \param[out] s1    : stresses at the end of the time step
     * \param[out] iv1   : internal state variables at the end of the time step
     * \param[in]  r     : rotation matrix
     * \param[in]  e0    : strain at the beginning of the time step
     * \param[in]  de    : strain increment
     * \param[in]  s0    : stresses at the beginning of the time step
     * \param[in]  mp    : material properties
     * \param[in]  iv0   : internal state variables at the beginning of the time step
     * \param[in]  ev0   : external state variables at the beginning of the time step
     * \param[in]  dev   : external state variables increments
     * \param[in]  h     : modelling hypothesis
     * \param[in]  dt    : time increment
     * \param[in]  ktype : type of the stiffness matrix
     */
    virtual bool
    integrate(tfel::math::matrix<real>&,
	      tfel::math::vector<real>&,
	      tfel::math::vector<real>&,
	      const tfel::math::tmatrix<3u,3u,real>&,
	      const tfel::math::vector<real>&,
	      const tfel::math::vector<real>&,
	      const tfel::math::vector<real>&,
	      const tfel::math::vector<real>&,
	      const tfel::math::vector<real>&,
	      const tfel::math::vector<real>&,
	      const tfel::math::vector<real>&,
	      const tfel::material::ModellingHypothesis::Hypothesis,
	      const real,
	      const MTestStiffnessMatrixType::mtype) const = 0;
    //! destructor
    virtual ~MTestBehaviour();
  }; // end of struct MTestBehaviour
  
} // end of namespace mfront

#endif /* _LIB_MFRONT_MTESTBEHAVIOUR_H */

