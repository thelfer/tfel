/*!
 * \file  mtest/include/MTest/FAndersonAccelerationAlgorithm.hxx
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

#ifndef LIB_MTEST_MTEST_FANDERSONACCELERATIONALGORITHM_HXX
#define LIB_MTEST_MTEST_FANDERSONACCELERATIONALGORITHM_HXX

#include "TFEL/Math/AccelerationAlgorithms/FAnderson.hxx"
#include "MTest/Config.hxx"
#include "MTest/AccelerationAlgorithm.hxx"

namespace mtest {

  //! \brief This class implements the Cast3M acceleration algorithm.
  struct MTEST_VISIBILITY_EXPORT FAndersonAccelerationAlgorithm final
      : public AccelerationAlgorithm {
    //! \brief default constructor
    FAndersonAccelerationAlgorithm();
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
    ~FAndersonAccelerationAlgorithm() override;

   protected:
    using Field = tfel::math::vector<real>;
    using FAnderson = tfel::math::FAnderson<Field, long double>;
    //! the underlying algorithm
    std::unique_ptr<FAnderson> a;
    /* pointer to the internal fields */
    //! previous displacement estimate
    Field* uO = nullptr;
    //! current displacement estimate
    Field* uN = nullptr;
    //! current residual
    Field* Df = nullptr;
    //! method order
    int Nmax = -1;
    //! alternance order
    int alMax = -1;
  };  // end of struct FAndersonAccelerationAlgorithm

}  // end of namespace mtest

#endif /* LIB_MTEST_MTEST_FANDERSONACCELERATIONALGORITHM_HXX */
