/*!
 * \file  mtest/include/MTest/SecantAccelerationAlgorithm.hxx
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

#ifndef LIB_MTEST_MTEST_MTESTSECANTACCELERATIONALGORITHM_HXX
#define LIB_MTEST_MTEST_MTESTSECANTACCELERATIONALGORITHM_HXX

#include "MTest/Config.hxx"
#include "MTest/AccelerationAlgorithm.hxx"

namespace mtest {

  /*!
   * \brief This class implements the secant acceleration algorithm.
   */
  struct MTEST_VISIBILITY_EXPORT SecantAccelerationAlgorithm
      : public AccelerationAlgorithm {
    //! default constructor
    SecantAccelerationAlgorithm();
    //
    void initialize(const unsigned short) override;
    [[nodiscard]] std::string getName() const override;
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
    ~SecantAccelerationAlgorithm() override;

   protected:
    // secant acceleration algorithm
    tfel::math::vector<real> sa_u0;
    tfel::math::vector<real> sa_u1;
    tfel::math::vector<real> sa_r0;
    tfel::math::vector<real> sa_r1;
    tfel::math::vector<real> sa_r;
    tfel::math::vector<real> sa_dr;
    //! \brief parameter of the secant algorithm
    real sa_w;
    //! \brief trigger
    int sat;
  };  // end of struct SecantAccelerationAlgorithm

}  // end of namespace mtest

#endif /* LIB_MTEST_MTEST_MTESTSECANTACCELERATIONALGORITHM_HXX */
