/*!
 * \file   mtest/include/MTest/AccelerationAlgorithm.hxx
 * \brief
 * \author Helfer Thomas
 * \brief  10 juin 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_MTEST_MTESTACCELERATIONALGORITHM_H_
#define LIB_MTEST_MTEST_MTESTACCELERATIONALGORITHM_H_

#include "TFEL/Math/vector.hxx"
#include "MTest/Config.hxx"
#include "MTest/Types.hxx"

namespace mtest {

  /*!
   * \brief abstract class for acceleration algorithm.
   */
  struct MTEST_VISIBILITY_EXPORT AccelerationAlgorithm {
    /*!
     * \brief extract an unsigned short from a string
     * \param[in] m : calling method
     * \param[in] v : value
     * \return an unsigned short
     */
    static unsigned short convertToUnsignedShort(const std::string&,
                                                 const std::string&);
    /*!
     * \return the name of the acceleration algorithm
     */
    virtual std::string getName(void) const = 0;
    /*!
     * \param[in] psz : total number of unknowns (problem size)
     */
    virtual void initialize(const unsigned short) = 0;
    /*!
     * set a parameter of the acceleration algorithm
     * \param[in] p : parameter name
     * \param[in] v : parameter value
     */
    virtual void setParameter(const std::string&, const std::string&) = 0;
    /*!
     * called at each time step, before the beginning of the Newton
     * algorithm
     */
    virtual void preExecuteTasks() = 0;
    /*!
     * \param[in,out] u1   : current estimate of the unknowns (after
     * incrementation) \param[in]     ru   : fixed point residual on the
     * unknowns \param[in]     rf   : Newton residual \param[in]     ueps :
     * criterium on the unknowns \param[in]     seps : criterium on the
     * thermodynamic forces \param[in]     iter : current iteration number
     */
    virtual void execute(tfel::math::vector<real>&,
                         const tfel::math::vector<real>&,
                         const tfel::math::vector<real>&,
                         const real,
                         const real,
                         const unsigned short) = 0;
    /*!
     * called at each time step, once convergence is reached
     */
    virtual void postExecuteTasks() = 0;
    //! destructor
    virtual ~AccelerationAlgorithm();
  };  // end of struct struct AccelerationAlgorithm

}  // end of namespace mtest

#endif /* LIB_MTEST_MTEST_MTESTACCELERATIONALGORITHM_H_ */
