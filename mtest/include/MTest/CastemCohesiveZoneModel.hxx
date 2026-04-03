/*!
 * \file  mtest/include/MTest/CastemCohesiveZoneModel.hxx
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

#ifndef LIB_MTEST_CASTEMCOHESIVEZONEMODEL_HXX
#define LIB_MTEST_CASTEMCOHESIVEZONEMODEL_HXX

#include "TFEL/System/ExternalFunctionsPrototypes.hxx"
#include "MTest/StandardBehaviourBase.hxx"
#include "MTest/CastemInterfaceVersion.hxx"

namespace mtest {

  /*!
   * \brief a class to handle cohesive zone models written using the `umat`
   * interface of the `Cast3M` finite element solver.
   */
  struct TFEL_VISIBILITY_LOCAL CastemCohesiveZoneModel
      : public StandardBehaviourBase {
    /*!
     * \param[in] h : modelling hypothesis
     * \param[in] l : library name
     * \param[in] b : behaviour name
     */
    CastemCohesiveZoneModel(const Hypothesis,
                            const std::string&,
                            const std::string&);
    tfel::math::tmatrix<3u, 3u, real> getRotationMatrix(
        const tfel::math::vector<real>&,
        const tfel::math::tmatrix<3u, 3u, real>&) const override;
    virtual void getGradientsDefaultInitialValues(
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
    //! destructor
    ~CastemCohesiveZoneModel() override;

   protected:
    //! \return the version of the `Cast3M` interface
    virtual CastemInterfaceVersion getCastemInterfaceVersion() const;
    /*!
     * \brief compute the elastic stiffness
     * \param[out] Kt   : tangent operator
     * \param[in]  mp   : material properties
     * \param[in]  drot : rotation matrix (Fortran convention)
     */
    virtual void computeElasticStiffness(
        tfel::math::matrix<real>&,
        const tfel::math::vector<real>&,
        const tfel::math::tmatrix<3u, 3u, real>&) const;
    //! the umat fonction
    tfel::system::CastemFctPtr fct;
  };  // end of struct MTest

}  // end of namespace mtest

#endif /* LIB_MTEST_CASTEMCOHESIVEZONEMODEL_HXX */
