/*!
 * \file  mtest/include/MTest/CastemAccelerationAlgorithm.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 10 juin 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_MTEST_MTESTCASTEMACCELERATIONALGORITHM_H_
#define LIB_MTEST_MTEST_MTESTCASTEMACCELERATIONALGORITHM_H_

#include "MTest/Config.hxx"
#include "MTest/AccelerationAlgorithm.hxx"

namespace mtest {

  /*!
   * \brief This class implements the Cast3M acceleration algorithm.
   */
  struct MTEST_VISIBILITY_EXPORT CastemAccelerationAlgorithm final
      : public AccelerationAlgorithm {
    //! default constructor
    CastemAccelerationAlgorithm();
    /*!
     * \return the name of the acceleration algorithm
     */
    virtual std::string getName(void) const override;
    /*!
     * \param[in] psz : total number of unknowns (problem size)
     */
    virtual void initialize(const unsigned short) override;
    /*!
     * set a parameter of the acceleration algorithm
     * \param[in] p : parameter name
     * \param[in] v : parameter value
     */
    virtual void setParameter(const std::string&, const std::string&) override;
    /*!
     * called at each time step, before the beginning of the Newton
     * algorithm
     */
    virtual void preExecuteTasks(void) override;
    /*!
     * \param[in,out] u1   : current estimate of the unknowns
     * \param[in]     u0   : previous estimate of the unknowns
     * \param[in]     r    : Newton residual
     * \param[in]     ueps : criterium on the unknowns
     * \param[in]     seps : criterium on the thermodynamic forces
     * \param[in]     iter : current iteration number
     */
    virtual void execute(tfel::math::vector<real>&,
                         const tfel::math::vector<real>&,
                         const tfel::math::vector<real>&,
                         const real,
                         const real,
                         const unsigned short) override;
    /*!
     * called at each time step, once convergence is reached
     */
    virtual void postExecuteTasks(void) override;
    //! destructor
    virtual ~CastemAccelerationAlgorithm();

   protected:
    tfel::math::vector<real> ca_u0;
    tfel::math::vector<real> ca_u1;
    tfel::math::vector<real> ca_u2;
    tfel::math::vector<real> ca_r0;
    tfel::math::vector<real> ca_r1;
    tfel::math::vector<real> ca_r2;
    tfel::math::vector<real> ca_n0;
    tfel::math::vector<real> ca_n1;
    tfel::math::vector<real> ca_tmp0;
    tfel::math::vector<real> ca_tmp1;
    //! acceleration trigger
    int cat;
    //! acceleration period
    int cap;
  };  // end of struct CastemAccelerationAlgorithm

}  // end of namespace mtest

#endif /* LIB_MTEST_MTEST_MTESTCASTEMACCELERATIONALGORITHM_H_ */
