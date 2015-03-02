/*! 
 * \file  mfront/include/MTest/AsterCohesiveZoneModel.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 07 avril 2013
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MFRONT_MTESTASTERCOHESIVEZONEMODELBEHAVIOUR_H_
#define LIB_MFRONT_MTESTASTERCOHESIVEZONEMODELBEHAVIOUR_H_ 

#include"TFEL/System/ExternalFunctionsPrototypes.hxx"
#include"MTest/UmatBehaviourBase.hxx"

namespace mfront
{

  /*!
   * A class to handle mechanical beheaviours written using the aster
   * interface
   */
  struct TFEL_VISIBILITY_LOCAL AsterCohesiveZoneModel
    : public UmatBehaviourBase
  {
    /*!
     * \param[in] h : modelling hypothesis
     * \param[in] l : library name
     * \param[in] b : behaviour name
     */
    AsterCohesiveZoneModel(const tfel::material::ModellingHypothesis::Hypothesis,
					const std::string&,
					const std::string&);
    /*!
     * \brief compute the *real* rotation matrix
     * \param[in] mp : material properties
     * \param[in] r  : rotation matrix defined by the user
     * \note this method is only meaningfull for the aster (Cast3M)
     * interface
     */
    virtual tfel::math::tmatrix<3u,3u,real>
    getRotationMatrix(const tfel::math::vector<real>&,
		      const tfel::math::tmatrix<3u,3u,real>&) const;
    /*!
     * \param[out] v : initial values of the driving variables
     */
    virtual void
    getDrivingVariablesDefaultInitialValues(tfel::math::vector<real>&) const;
    /*!
     * \brief allocate internal workspace
     * \param[in] h : modelling hypothesis
     */
    virtual void
    allocate(const tfel::material::ModellingHypothesis::Hypothesis);
    /*!
     * \return the default type of stiffness matrix used by the behaviour
     */
    virtual MTestStiffnessMatrixType::mtype
    getDefaultStiffnessMatrixType(void) const;
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
			      const MTestStiffnessMatrixType::mtype) const;
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
	      const MTestStiffnessMatrixType::mtype) const;
    /*!
     * \brief some interfaces requires dummy material properties to be
     * declared. For example, the Cast3M finite element solver
     * requires the mass density and some extra material properties
     * describing orthotropic axes to be declared.  This method is
     * meant to automatically declare those if they are not defined by
     * the user.
     * \param[out] mp  : evolution manager where 
     * \param[in]  evm : evolution manager
     */
    virtual void
    setOptionalMaterialPropertiesDefaultValues(EvolutionManager&,
					       const EvolutionManager&) const;
    //! destructor
    virtual ~AsterCohesiveZoneModel();
  protected:
    /*!
     * \brief call the mechanical behaviour
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
     * \param[in] b : if true, integrate the behaviour over the time
     * step, if false compute a prediction of the stiffness matrix
     */
    virtual bool
    call_behaviour(tfel::math::matrix<real>&,
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
		   const MTestStiffnessMatrixType::mtype,
		   const bool) const;
    //! the aster fonction
    tfel::system::AsterFctPtr fct;
    //! temporary vector
    mutable tfel::math::vector<real> iv;
  }; // end of struct AsterCohesiveZoneModel
  
} // end of namespace mfront

#endif /* LIB_MFRONT_MTESTASTERCOHESIVEZONEMODELBEHAVIOUR_H_ */

