/*! 
 * \file  mfront/include/MFront/MTestCyranoBehaviour.hxx
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

#ifndef _LIB_MFRONT_MTESTCYRANOBEHAVIOUR_H_
#define _LIB_MFRONT_MTESTCYRANOBEHAVIOUR_H_ 

#include"TFEL/Math/tmatrix.hxx"

#include"TFEL/System/ExternalFunctionsPrototypes.hxx"
#include"MFront/MTestUmatBehaviourBase.hxx"

namespace mfront
{

  /*!
   * A class to handle mechanical beheaviours written using the umat
   * interface
   */
  struct TFEL_VISIBILITY_LOCAL MTestCyranoBehaviour
    : public MTestUmatBehaviourBase
  {
    /*!
     * \param[in] h : modelling hypothesis
     * \param[in] l : library name
     * \param[in] b : behaviour name
     */
    MTestCyranoBehaviour(const tfel::material::ModellingHypothesis::Hypothesis,
			 const std::string&,
			 const std::string&);
    /*!
     * \brief compute the *real* rotation matrix
     * \param[in] mp : material properties
     * \param[in] r  : rotation matrix defined by the user
     * \note this method is only meaningfull for the umat (Cast3M)
     * interface
     */
    virtual tfel::math::tmatrix<3u,3u,real>
    getRotationMatrix(const tfel::math::vector<real>&,
		      const tfel::math::tmatrix<3u,3u,real>&) const;
    /*!
     * \return the default type of stiffness matrix used by the behaviour
     */
    virtual MTestStiffnessMatrixType::mtype
    getDefaultStiffnessMatrixType(void) const;
    /*!
     * \brief allocate internal workspace
     * \param[in] h : modelling hypothesis
     */
    virtual void
    allocate(const tfel::material::ModellingHypothesis::Hypothesis);
    /*!
     * \param[out] v : initial values of the driving variables
     */
    virtual void
    getDrivingVariablesDefaultInitialValues(tfel::math::vector<real>&) const;
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
    //! destructor
    virtual ~MTestCyranoBehaviour();
  protected:
    /*!
     * \brief integrate the mechanical behaviour over the time step
     * \return true if the integration was successfull, false otherwise
     * \param[out] Kt    : tangent operator
     * \param[out] s1    : stresses at the end of the time step
     * \param[out] iv1   : internal state variables at the end of the time step
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
     * \param[in]  b     : if false, only compute a prediction operator (no integration)
     */
    virtual bool
      call_behaviour(tfel::math::matrix<real>&,
		     tfel::math::vector<real>&,
		     tfel::math::vector<real>&,
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
    /*!
     * \brief compute the elastic stiffness
     * \param[out] Kt   : tangent operator
     * \param[in]  mp   : material properties
     * \param[in]  h    : modelling hypothesis
     */
    virtual void
      computeElasticStiffness(tfel::math::matrix<real>&,
			      const tfel::math::vector<real>&,
			      const tfel::material::ModellingHypothesis::Hypothesis) const;
  protected:
    //! the umat fonction
    tfel::system::CyranoFctPtr fct;
    //! temporary vector
    mutable tfel::math::vector<real> iv;
  }; // end of struct MTestBehaviour
  
} // end of namespace mfront

#endif /* _LIB_MFRONT_MTESTCYRANOBEHAVIOUR_H */
