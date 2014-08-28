/*! 
 * \file   mfront/include/MFront/MTestAccelerationAlgorithm.hxx
 * \brief
 * \author Helfer Thomas
 * \brief  10 juin 2014
 */

#ifndef _LIB_MFRONT_MTEST_MTESTACCELERATIONALGORITHM_H_
#define _LIB_MFRONT_MTEST_MTESTACCELERATIONALGORITHM_H_ 

#include"TFEL/Math/vector.hxx"
#include"MFront/MTestConfig.hxx"
#include"MFront/MTestTypes.hxx"

namespace mfront{

  /*!
   * \brief abstract class for acceleration algorithm.
   */
  struct MFRONT_MTEST_VISIBILITY_EXPORT MTestAccelerationAlgorithm
  {
    /*!
     * \brief extract an unsigned short from a string
     * \param[in] m : calling method
     * \param[in] v : value
     * \return an unsigned short
     */
    static unsigned short
    convertToUnsignedShort(const std::string&,
			   const std::string&);
    /*!
     * \return the name of the acceleration algorithm
     */
    virtual std::string
    getName(void) const = 0;
    /*!
     * \param[in] psz : total number of unknowns (problem size)
     */
    virtual void
    initialize(const unsigned short) = 0;
    /*!
     * set a parameter of the acceleration algorithm
     * \param[in] p : parameter name
     * \param[in] v : parameter value
     */
    virtual void
    setParameter(const std::string&,
		 const std::string&) = 0;
    /*!
     * called at each time step, before the beginning of the Newton
     * algorithm
     */
    virtual void
    preExecuteTasks() = 0;
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
	    const unsigned short) = 0;
    /*!
     * called at each time step, once convergence is reached
     */
    virtual void
    postExecuteTasks() = 0;
    //! destructor
    virtual ~MTestAccelerationAlgorithm();
  }; // end of struct struct MTestAccelerationAlgorithm

} // end of namespace mfront

#endif /* _LIB_MFRONT_MTEST_MTESTACCELERATIONALGORITHM_H */

