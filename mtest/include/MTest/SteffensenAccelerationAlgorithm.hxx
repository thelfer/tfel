/*!
 * \file  mtest/include/MTest/SteffensenAccelerationAlgorithm.hxx
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

#ifndef LIB_MTEST_MTEST_MTESTSTEFFENSENACCELERATIONALGORITHM_HXX
#define LIB_MTEST_MTEST_MTESTSTEFFENSENACCELERATIONALGORITHM_HXX

#include "MTest/Config.hxx"
#include "MTest/AccelerationAlgorithm.hxx"

namespace mtest {

  /*!
   * \brief This class implements the Steffensen acceleration algorithm.
   */
  struct MTEST_VISIBILITY_EXPORT SteffensenAccelerationAlgorithm
      : public AccelerationAlgorithm {
    //! \brief default constructor
    SteffensenAccelerationAlgorithm();
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
    ~SteffensenAccelerationAlgorithm() override;

   protected:
    tfel::math::vector<real> sta_u0;
    tfel::math::vector<real> sta_u1;
    tfel::math::vector<real> sta_u2;
    tfel::math::vector<real> sta_du2;
    tfel::math::vector<real> sta_du1;
    //! trigger
    int stat;
  };  // end of struct SteffensenAccelerationAlgorithm

}  // end of namespace mtest

#endif /* LIB_MTEST_MTEST_MTESTSTEFFENSENACCELERATIONALGORITHM_HXX */
