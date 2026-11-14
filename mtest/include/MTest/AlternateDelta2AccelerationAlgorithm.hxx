/*!
 * \file  mtest/include/MTest/AlternateDelta2AccelerationAlgorithm.hxx
 * \brief
 * \author RAMIERE Isabelle
 * \brief 16 mars 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_MTEST_MTESTALTERNATEDELTA2ACCELERATIONALGORITHM_HXX
#define LIB_MTEST_MTEST_MTESTALTERNATEDELTA2ACCELERATIONALGORITHM_HXX

#include "MTest/Config.hxx"
#include "MTest/AccelerationAlgorithm.hxx"

namespace mtest {

  /*!
   * \brief This class implements the secant acceleration algorithm.
   */
  struct MTEST_VISIBILITY_EXPORT AlternateDelta2AccelerationAlgorithm
      : public AccelerationAlgorithm {
    //! \brief default constructor
    AlternateDelta2AccelerationAlgorithm();
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
    ~AlternateDelta2AccelerationAlgorithm() override;

   protected:
    tfel::math::vector<real> asa_u0;
    tfel::math::vector<real> asa_u1;
    tfel::math::vector<real> asa_du0;
    tfel::math::vector<real> asa_du1;
    tfel::math::vector<real> asa_d2u;
    tfel::math::vector<real> asa_r0;
    tfel::math::vector<real> asa_r1;
    tfel::math::vector<real> asa_dr0;
    tfel::math::vector<real> asa_dr1;
    tfel::math::vector<real> asa_d2r;
    //! trigger
    int asat;
  };  // end of struct AlternateDelta2AccelerationAlgorithm

}  // end of namespace mtest

#endif /* LIB_MTEST_MTEST_MTESTALTERNATEDELTA2ACCELERATIONALGORITHM_HXX */
