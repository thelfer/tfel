/*! 
 * \file  MTestSteffensenAccelerationAlgorithm.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 10 juin 2014
 */

#ifndef _LIB_MFRONT_MTEST_MTESTSTEFFENSENACCELERATIONALGORITHM_H_
#define _LIB_MFRONT_MTEST_MTESTSTEFFENSENACCELERATIONALGORITHM_H_ 

#include"MFront/MTestConfig.hxx"
#include"MFront/MTestAccelerationAlgorithm.hxx"

namespace mfront
{

  /*!
   * \brief This class implements the Steffensen acceleration algorithm.
   */
  struct MFRONT_MTEST_VISIBILITY_EXPORT MTestSteffensenAccelerationAlgorithm
    : public MTestAccelerationAlgorithm
  {
    //! default constructor
    MTestSteffensenAccelerationAlgorithm();
    /*!
     * \param[in] psz : total number of unknowns (problem size)
     */
    virtual void
    initialize(const unsigned short);
    /*!
     * \return the name of the acceleration algorithm
     */
    virtual std::string
    getName(void) const;
    /*!
     * set a parameter of the acceleration algorithm
     * \param[in] p : parameter name
     * \param[in] v : parameter value
     */
    virtual void
    setParameter(const std::string&,
		 const std::string&);
    /*!
     * called at each time step, before the beginning of the Newton
     * algorithm
     */
    virtual void
    preExecuteTasks(void);
    /*!
     * \param[in,out] u1   : current estimate of the unknowns
     * \param[in]     u0   : current estimate of the unknowns
     * \param[in]     r    : Newton residual
     * \param[in]     ueps : criterium on the unknowns
     * \param[in]     seps : criterium on the thermodynamic forces
     * \param[in]     iter : current iteration number
     */
    virtual void
    execute(tfel::math::vector<real>&,
	    const tfel::math::vector<real>&,
	    const tfel::math::vector<real>&,
	    const real,
	    const real,
	    const unsigned short);
    /*!
     * called at each time step, once convergence is reached
     */
    virtual void
    postExecuteTasks(void);
    //! destructor
    virtual ~MTestSteffensenAccelerationAlgorithm();
  protected:
    tfel::math::vector<real> sta_u0;
    tfel::math::vector<real> sta_u1;
    tfel::math::vector<real> sta_u2;
    tfel::math::vector<real> sta_du2;
    tfel::math::vector<real> sta_du1;
    //! trigger
    int stat;
  }; // end of struct MTestSteffensenAccelerationAlgorithm

} // end of namespace mfront

#endif /* _LIB_MFRONT_MTEST_MTESTSTEFFENSENACCELERATIONALGORITHM_H */

