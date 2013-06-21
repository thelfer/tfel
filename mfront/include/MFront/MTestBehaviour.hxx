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
#include"TFEL/Math/vector.hxx"
#include"TFEL/Math/stensor.hxx"
#include"TFEL/Math/tvector.hxx"
#include"TFEL/Material/ModellingHypothesis.hxx"

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
     * 0 means that the behaviour is isotropic.
     * 1 means that the behaviour is orthotropic.
     */
    virtual unsigned short
    getBehaviourType(void) const = 0;
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
     * \param[in] d : space dimension
     */
    virtual size_t
    getInternalStateVariablesSize(const unsigned short) const = 0;
    /*!
     * \return the descriptions the internal variables
     * \param[in] d : space dimension
     */
    virtual std::vector<std::string>
    getInternalStateVariablesDescriptions(const unsigned short) const = 0;
    /*!
     * \return the type of an internal variable
     * \param[in] n : internal variable name
     */
    virtual unsigned short
    getInternalStateVariableType(const std::string&) const = 0;
    /*!
     * \return the position of an internal variable
     * \param[in] n : internal variable name
     */
    virtual unsigned short
    getInternalStateVariablePosition(const unsigned short,
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
     * \brief allocate workspace
     * \param[in] ntens   : number of components of symmetric tensors
     * \param[in] nstatev : size of the internal state variables
     */
    virtual void
    allocate(const size_t,
	     const size_t) = 0;
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
			      const tfel::math::stensor<3u,real>&,
			      const tfel::math::stensor<3u,real>&,
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
	      tfel::math::stensor<3u,real>&,
	      tfel::math::vector<real>&,
	      const tfel::math::tmatrix<3u,3u,real>&,
	      const tfel::math::stensor<3u,real>&,
	      const tfel::math::stensor<3u,real>&,
	      const tfel::math::stensor<3u,real>&,
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

