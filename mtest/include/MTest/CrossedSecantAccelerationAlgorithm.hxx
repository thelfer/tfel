/*!
 * \file  mtest/include/MTest/CrossedSecantAccelerationAlgorithm.hxx
 * \brief
 * \author RAMIERE Isabelle
 * \brief 9 mars 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_MTEST_MTESTCROSSEDSECANTACCELERATIONALGORITHM_HXX
#define LIB_MTEST_MTEST_MTESTCROSSEDSECANTACCELERATIONALGORITHM_HXX

#include "MTest/Config.hxx"
#include "MTest/AccelerationAlgorithm.hxx"

namespace mtest {

  /*!
   * \brief This class implements the secant acceleration algorithm.
   */
  struct MTEST_VISIBILITY_EXPORT CrossedSecantAccelerationAlgorithm
      : public AccelerationAlgorithm {
    //! \brief default constructor
    CrossedSecantAccelerationAlgorithm();
    //
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
    ~CrossedSecantAccelerationAlgorithm() override;

   protected:
    // secant acceleration algorithm
    tfel::math::vector<real> csa_u0;
    tfel::math::vector<real> csa_u1;
    tfel::math::vector<real> csa_du;
    tfel::math::vector<real> csa_r0;
    tfel::math::vector<real> csa_r1;
    tfel::math::vector<real> csa_dr;
    //! trigger
    int csat;
  };  // end of struct CrossedSecantAccelerationAlgorithm

}  // end of namespace mtest

#endif /* LIB_MTEST_MTEST_MTESTCROSSEDSECANTACCELERATIONALGORITHM_HXX */
