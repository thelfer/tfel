/*!
 * \file   include/MFront/BehaviourBrick/MisesStressCriterion.hxx
 * \brief
 * \author Thomas Helfer
 * \date   15/03/2018
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_BEHAVIOURBRICK_MISESSTRESSCRITERION_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_MISESSTRESSCRITERION_HXX

#include "MFront/BehaviourBrick/StressCriterionBase.hxx"

namespace mfront::bbrick {

  /*!
   * \brief This class describes the von Mises criterion defined by:
   * \f[
   * \sigma_{\mathrm{eq}}
   * = \sqrt{\frac{3}{2}\,\underline{s}\,\colon\,\underline{s}}
   * = \sqrt{3\,J_{2}}
   * \f]
   * where:
   * - \f$\underline{s}\f$ is the deviatoric stress defined as follows:
   * \f[
   * \underline{s}=\underline{\sigma}-
   * \frac{1}{3}\,\mathrm{trace}\left(\underline{\sigma}\right)\,\underline{I}
   * \f]
   * - \f$J_{2}\f$ is the second invariant of \f$\underline{s}\f$.
   *
   * In terms of the eigenvalues of the stress, denoted by \f$\sigma_{1}\f$,
   * \f$\sigma_{2}\f$ and \f$\sigma_{3}\f$, the von Mises stress can also be
   * defined by:
   * \f[
   * \sigma_{\mathrm{eq}}=\sqrt{\frac{1}{2}\left(
   *   \left|\sigma_{1}-\sigma_{2}\right|^{2}+
   *   \left|\sigma_{1}-\sigma_{3}\right|^{2}+
   *   \left|\sigma_{2}-\sigma_{3}\right|^{2}\right)}
   * \f]
   */
  struct MisesStressCriterion final : StressCriterionBase {
    std::vector<BehaviourSymmetry> getSupportedBehaviourSymmetries()
        const override;
    void initialize(BehaviourDescription&,
                    AbstractBehaviourDSL&,
                    const std::string&,
                    const DataMap&,
                    const Role) override;
    std::string computeElasticPrediction(const std::string&,
                                         const BehaviourDescription&,
                                         const StressPotential&) const override;
    std::string computeCriterion(const std::string&,
                                 const BehaviourDescription&,
                                 const StressPotential&) const override;
    std::string computeNormal(const std::string&,
                              const BehaviourDescription&,
                              const StressPotential&,
                              const Role) const override;
    std::string computeNormalDerivative(const std::string&,
                                        const BehaviourDescription&,
                                        const StressPotential&,
                                        const Role) const override;
    bool isCoupledWithPorosityEvolution() const override;
    bool isNormalDeviatoric() const override;
    PorosityEffectOnFlowRule getPorosityEffectOnEquivalentPlasticStrain()
        const override;
    //! \brief destructor
    ~MisesStressCriterion() override;
  };  // end of struct MisesStressCriterion

}  // end of namespace mfront::bbrick

#endif /* LIB_MFRONT_BEHAVIOURBRICK_MISESSTRESSCRITERION_HXX */
