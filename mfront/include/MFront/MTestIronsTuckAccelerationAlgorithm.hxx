/*! 
 * \file  mfront/include/MFront/MTestIronsTuckAccelerationAlgorithm.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 10 juin 2014
 */

#ifndef _LIB_MFRONT_MTEST_MTESTIRONSTUCKACCELERATIONALGORITHM_H_
#define _LIB_MFRONT_MTEST_MTESTIRONSTUCKACCELERATIONALGORITHM_H_ 

#include"MFront/MTestConfig.hxx"
#include"MFront/MTestAccelerationAlgorithm.hxx"

namespace mfront
{

  /*!
   * \brief This class implements the Irons-Tuck acceleration algorithm.
   */
  struct MFRONT_MTEST_VISIBILITY_EXPORT MTestIronsTuckAccelerationAlgorithm
    : public MTestAccelerationAlgorithm
  {
    //! default constructor
    MTestIronsTuckAccelerationAlgorithm();
    /*!
     * \return the name of the acceleration algorithm
     */
    virtual std::string
    getName(void) const;
    /*!
     * \param[in] psz : total number of unknowns (problem size)
     */
    virtual void
    initialize(const unsigned short);
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
     * \param[in]     u0   : previous estimate of the unknowns
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
    virtual ~MTestIronsTuckAccelerationAlgorithm();
  protected:
    // Irons and Tuck acceleration algorithm
    tfel::math::vector<real> ita_u0;
    tfel::math::vector<real> ita_u1;
    tfel::math::vector<real> ita_u2;
    tfel::math::vector<real> ita_du;
    tfel::math::vector<real> ita_ddu;
    //! IronsTuck acceleration trigger
    int itat;
  }; // end of struct MTestIronsTuckAccelerationAlgorithm

} // end of namespace mfront

#endif /* _LIB_MFRONT_MTEST_MTESTIRONSTUCKACCELERATIONALGORITHM_H */

