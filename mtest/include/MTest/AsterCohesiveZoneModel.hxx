/*!
 * \file  mtest/include/MTest/AsterCohesiveZoneModel.hxx
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

#ifndef LIB_MTEST_MTESTASTERCOHESIVEZONEMODELBEHAVIOUR_HXX
#define LIB_MTEST_MTESTASTERCOHESIVEZONEMODELBEHAVIOUR_HXX

#include "TFEL/System/ExternalFunctionsPrototypes.hxx"
#include "MTest/StandardBehaviourBase.hxx"

namespace mtest {

  /*!
   * A class to handle mechanical beheaviours written using the aster
   * interface
   */
  struct TFEL_VISIBILITY_LOCAL AsterCohesiveZoneModel
      : public StandardBehaviourBase {
    /*!
     * \param[in] h : modelling hypothesis
     * \param[in] l : library name
     * \param[in] b : behaviour name
     */
    AsterCohesiveZoneModel(const Hypothesis,
                           const std::string&,
                           const std::string&);
    //
    tfel::math::tmatrix<3u, 3u, real> getRotationMatrix(
        const tfel::math::vector<real>&,
        const tfel::math::tmatrix<3u, 3u, real>&) const override;
    void getGradientsDefaultInitialValues(
        tfel::math::vector<real>&) const override;
    void allocateWorkSpace(BehaviourWorkSpace&) const override;
    StiffnessMatrixType getDefaultStiffnessMatrixType() const override;
    std::pair<bool, real> computePredictionOperator(
        BehaviourWorkSpace&,
        const CurrentState&,
        const StiffnessMatrixType) const override;
    std::pair<bool, real> integrate(CurrentState&,
                                    BehaviourWorkSpace&,
                                    const real,
                                    const StiffnessMatrixType) const override;
    std::vector<std::string> getOptionalMaterialProperties() const override;
    void setOptionalMaterialPropertiesDefaultValues(
        EvolutionManager&, const EvolutionManager&) const override;
    //! \brief destructor
    ~AsterCohesiveZoneModel() override;

   protected:
    std::pair<bool, real> call_behaviour(tfel::math::matrix<real>&,
                                         CurrentState&,
                                         BehaviourWorkSpace&,
                                         const real,
                                         const StiffnessMatrixType,
                                         const bool) const;
    //! \brief the aster fonction
    tfel::system::AsterFctPtr fct;
    //! \brief pointer to function returing the integration error message
    const char* (*emsg)();
  };  // end of struct AsterCohesiveZoneModel

}  // end of namespace mtest

#endif /* LIB_MTEST_MTESTASTERCOHESIVEZONEMODELBEHAVIOUR_HXX */
