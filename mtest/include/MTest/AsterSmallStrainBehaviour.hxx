/*!
 * \file  mtest/include/MTest/AsterSmallStrainBehaviour.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 07 avril 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_MTESTASTERSMALLSTRAINBEHAVIOUR_HXX
#define LIB_MTEST_MTESTASTERSMALLSTRAINBEHAVIOUR_HXX

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/System/ExternalFunctionsPrototypes.hxx"
#include "MTest/AsterStandardBehaviour.hxx"

namespace mtest {

  /*!
   * A class to handle mechanical beheaviours written using the aster
   * interface
   */
  struct TFEL_VISIBILITY_LOCAL AsterSmallStrainBehaviour
      : public AsterStandardBehaviour {
    /*!
     * \param[in] h : modelling hypothesis
     * \param[in] l : library name
     * \param[in] b : behaviour name
     */
    AsterSmallStrainBehaviour(const Hypothesis,
                              const std::string&,
                              const std::string&);
    //
    void getGradientsDefaultInitialValues(
        tfel::math::vector<real>&) const override;
    //! \brief destructor
    ~AsterSmallStrainBehaviour() override;

   protected:
    std::pair<bool, real> call_behaviour(tfel::math::matrix<real>&,
                                         CurrentState&,
                                         BehaviourWorkSpace&,
                                         const real,
                                         const StiffnessMatrixType,
                                         const bool) const override;
  };  // end of struct Behaviour

}  // end of namespace mtest

#endif /* LIB_MTEST_MTESTASTERSMALLSTRAINBEHAVIOUR_HXX */
