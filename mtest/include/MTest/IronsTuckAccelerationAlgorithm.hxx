/*!
 * \file  mtest/include/MTest/IronsTuckAccelerationAlgorithm.hxx
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

#ifndef LIB_MTEST_MTEST_MTESTIRONSTUCKACCELERATIONALGORITHM_HXX
#define LIB_MTEST_MTEST_MTESTIRONSTUCKACCELERATIONALGORITHM_HXX

#include "MTest/Config.hxx"
#include "MTest/AccelerationAlgorithm.hxx"

namespace mtest {

  /*!
   * \brief This class implements the Irons-Tuck acceleration algorithm.
   */
  struct MTEST_VISIBILITY_EXPORT IronsTuckAccelerationAlgorithm
      : public AccelerationAlgorithm {
    //! \brief default constructor
    IronsTuckAccelerationAlgorithm();
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
    ~IronsTuckAccelerationAlgorithm() override;

   protected:
    // Irons and Tuck acceleration algorithm
    tfel::math::vector<real> ita_r0;
    tfel::math::vector<real> ita_r1;
    tfel::math::vector<real> ita_dr;
    //! IronsTuck acceleration trigger
    int itat;
  };  // end of struct IronsTuckAccelerationAlgorithm

}  // end of namespace mtest

#endif /* LIB_MTEST_MTEST_MTESTIRONSTUCKACCELERATIONALGORITHM_HXX */
