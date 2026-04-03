/*!
 * \file  mtest/include/MTest/FunctionEvolution.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 14 avril 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_MTESTFUNCTIONEVOLUTION_HXX
#define LIB_MTEST_MTESTFUNCTIONEVOLUTION_HXX

#include "TFEL/Math/Evaluator.hxx"

#include "MTest/Config.hxx"
#include "MTest/Types.hxx"
#include "MTest/Evolution.hxx"

namespace mtest {

  struct MTEST_VISIBILITY_EXPORT FunctionEvolution : public Evolution {
    /*!
     * constructor
     * \param[in] :
     */
    FunctionEvolution(const std::string&, const EvolutionManager&);
    //
    real operator()(const real) const override;
    bool isConstant() const override;
    void setValue(const real) override;
    void setValue(const real, const real) override;
    //! \brief destructor
    ~FunctionEvolution() override;

   private:
    //! \brief externally defined evolutions
    const EvolutionManager& evm;
    //! \brief Evaluator
    mutable tfel::math::Evaluator f;
  };

}  // end of namespace mtest

#endif /* LIB_MTEST_MTESTFUNCTIONEVOLUTION_HXX */
