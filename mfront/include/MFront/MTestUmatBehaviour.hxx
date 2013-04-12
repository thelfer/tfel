/*! 
 * \file  MTestUmatBehaviour.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 07 avril 2013
 */

#ifndef _LIB_MFRONT_MTESTUMATBEHAVIOUR_H_
#define _LIB_MFRONT_MTESTUMATBEHAVIOUR_H_ 

#include"TFEL/System/ExternalFunctionsPrototypes.hxx"
#include"MFront/MTestBehaviour.hxx"

namespace mfront
{

  /*!
   * A class to handle mechanical beheaviours written using the umat
   * interface
   */
  struct MTestUmatBehaviour
    : public MTestBehaviour
  {
    /*!
     * \param[in] l : library name
     * \param[in] b : behaviour name
     */
    MTestUmatBehaviour(const std::string&,
		       const std::string&);
    /*!
     * \return the number of material properties
     */
    virtual std::vector<std::string>
    getMaterialPropertiesNames(void) const;
    /*!
     * \return the number of internal variables
     */
    virtual std::vector<std::string>
    getInternalStateVariablesNames(void) const;
    /*!
     * \return the size of the array of internal variables
     * \param[in] d : space dimension
     */
    virtual size_t
    getInternalStateVariablesSize(const unsigned short) const;
    /*!
     * \return the descriptions the internal variables
     * \param[in] d : space dimension
     */
    virtual std::vector<std::string>
    getInternalStateVariablesDescriptions(const unsigned short) const;
    /*!
     * \return the type of an internal variable
     * \param[in] n : internal variable name
     */
    virtual unsigned short
    getInternalStateVariableType(const std::string&) const;
    /*!
     * \return the position of an internal variable
     * \param[in] n : internal variable name
     */
    virtual unsigned short
    getInternalStateVariablePosition(const unsigned short,
				     const std::string&) const;
    /*!
     * \return the number of external variables
     */
    virtual std::vector<std::string>
    getExternalStateVariablesNames(void) const;
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
    virtual ~MTestUmatBehaviour();
  protected:
    //! names of the material properties
    std::vector<std::string> mpnames;
    //! names of the internal state variables
    std::vector<std::string> ivnames;
    //! types of the internal state variables
    std::vector<int> ivtypes;
    //! names of the external state variables
    std::vector<std::string> evnames;
    //! the umat fonction
    tfel::system::UMATFctPtr fct;
    //! type of of law (isotropic or orthotropic)
    unsigned short type;
  }; // end of struct MTestBehaviour
  
} // end of namespace mfront

#endif /* _LIB_MFRONT_MTESTUMATBEHAVIOUR_H */

