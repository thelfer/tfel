/*! 
 * \file  MTestAsterBehaviour.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 07 avril 2013
 */

#ifndef _LIB_MFRONT_MTESTASTERBEHAVIOUR_H_
#define _LIB_MFRONT_MTESTASTERBEHAVIOUR_H_ 

#include"TFEL/System/ExternalFunctionsPrototypes.hxx"
#include"MFront/MTestUmatBehaviourBase.hxx"

namespace mfront
{

  /*!
   * A class to handle mechanical beheaviours written using the aster
   * interface
   */
  struct MTestAsterBehaviour
    : public MTestUmatBehaviourBase
  {
    /*!
     * \param[in] l : library name
     * \param[in] b : behaviour name
     */
    MTestAsterBehaviour(const std::string&,
			const std::string&);
    /*!
     * \brief integrate the mechanical behaviour over the time step
     * \return true if the integration was successfull, false otherwise
     * \param[out] Kt  : tangent operator
     * \param[out] s1  : stresses at the end of the time step
     * \param[out] iv1 : internal state variables at the end of the time step
     * \param[in]  r   : rotation matrix
     * \param[in]  e0  : strain at the beginning of the time step
     * \param[in]  de  : strain increment
     * \param[in]  s0  : stresses at the beginning of the time step
     * \param[in]  mp  : material properties
     * \param[in]  iv0 : internal state variables at the beginning of the time step
     * \param[in]  ev0 : external state variables at the beginning of the time step
     * \param[in]  dev : external state variables increments
     * \param[in]  h   : modelling hypothesis
     * \param[in]  dt  : time increment
     */
    virtual bool
    integrate(tfel::math::matrix<real>&,
	      tfel::math::tvector<6u,real>&,
	      tfel::math::vector<real>&,
	      const tfel::math::tmatrix<3u,3u,real>&,
	      const tfel::math::tvector<6u,real>&,
	      const tfel::math::tvector<6u,real>&,
	      const tfel::math::tvector<6u,real>&,
	      const tfel::math::vector<real>&,
	      const tfel::math::vector<real>&,
	      const tfel::math::vector<real>&,
	      const tfel::math::vector<real>&,
	      const tfel::material::ModellingHypothesis::Hypothesis,
	      const real) const;
    //! destructor
    virtual ~MTestAsterBehaviour();
  protected:
    //! the aster fonction
    tfel::system::AsterFctPtr fct;
  }; // end of struct MTestBehaviour
  
} // end of namespace mfront

#endif /* _LIB_MFRONT_MTESTASTERBEHAVIOUR_H */

