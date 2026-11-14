/*!
 * \file  mtest/include/MTest/CastemAccelerationAlgorithm.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 10 juin 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_MTEST_MTESTCASTEMACCELERATIONALGORITHM_HXX
#define LIB_MTEST_MTEST_MTESTCASTEMACCELERATIONALGORITHM_HXX

#include "MTest/Config.hxx"
#include "MTest/AccelerationAlgorithm.hxx"

namespace mtest {

  /*!
   * \brief This class implements the Cast3M acceleration algorithm.
   */
  struct MTEST_VISIBILITY_EXPORT CastemAccelerationAlgorithm final
      : public AccelerationAlgorithm {
    //! \brief default constructor
    CastemAccelerationAlgorithm();
    [[nodiscard]] std::string getName() const override;
    void initialize(const unsigned short) override;
    void setParameter(const std::string&, const std::string&) override;
    void preExecuteTasks() override;
    void execute(tfel::math::vector<real>&,
                 const tfel::math::vector<real>&,
                 const tfel::math::vector<real>&,
                 const real,
                 const real,
                 const unsigned short) override;
    void postExecuteTasks() override;
    //! \brief destructor
    ~CastemAccelerationAlgorithm() override;

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

#endif /* LIB_MTEST_MTEST_MTESTCASTEMACCELERATIONALGORITHM_HXX */
