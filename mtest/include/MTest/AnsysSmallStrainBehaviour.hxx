/*!
 * \file  mtest/include/MTest/AnsysSmallStrainBehaviour.hxx
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

#ifndef LIB_MTEST_MTESTANSYSSMALLSTRAINBEHAVIOUR_HXX
#define LIB_MTEST_MTESTANSYSSMALLSTRAINBEHAVIOUR_HXX

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/System/ExternalFunctionsPrototypes.hxx"
#include "MTest/AnsysStandardBehaviour.hxx"

namespace mtest {

  /*!
   * A class to handle mechanical beheaviours written using the ansys
   * interface
   */
  struct TFEL_VISIBILITY_LOCAL AnsysSmallStrainBehaviour
      : public AnsysStandardBehaviour {
    /*!
     * \param[in] h : modelling hypothesis
     * \param[in] l : library name
     * \param[in] b : behaviour name
     */
    AnsysSmallStrainBehaviour(const Hypothesis,
                              const std::string&,
                              const std::string&);
    //
    void getGradientsDefaultInitialValues(
        tfel::math::vector<real>&) const override;
    //! \brief destructor
    ~AnsysSmallStrainBehaviour() override;

   protected:
    std::pair<bool, real> call_behaviour(tfel::math::matrix<real>&,
                                         CurrentState&,
                                         BehaviourWorkSpace&,
                                         const real,
                                         const StiffnessMatrixType,
                                         const bool) const override;
  };  // end of struct Behaviour

}  // end of namespace mtest

#endif /* LIB_MTEST_MTESTANSYSSMALLSTRAINBEHAVIOUR_HXX */
