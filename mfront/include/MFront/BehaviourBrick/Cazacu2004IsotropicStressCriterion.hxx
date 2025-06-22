/*!
 * \file   include/MFront/BehaviourBrick/Cazacu2004IsotropicStressCriterion.hxx
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

#ifndef LIB_MFRONT_BEHAVIOURBRICK_CAZACU2004ISOTROPICSTRESSCRITERION_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_CAZACU2004ISOTROPICSTRESSCRITERION_HXX

#include "MFront/BehaviourBrick/StressCriterionBase.hxx"

namespace mfront {

  namespace bbrick {

    /*!
     * \brief This class describes the Cazacu2004 orthotropic stress criterion
     * defined by:
     * \f[
     * \sigma_{\mathrm{eq}}^{C}=
     * \sqrt[3]{\left(J_{2}^{O}\right)^{3/2} - c\,\left(J_{3}^{O}\right)}
     * \f]
     * where \(J_{2}^{O}\) and \(J_{3}^{O}\) are the orthotropic generalizations
     * of the first and second invariants of the deviatoric part of the stress
     * tensor \f$\underline{\sigma}\f$.
     *
     * Cazacu, Oano and Barlat, Frédéric. 2004.
     * “A Criterion for Description of Anisotropy and Yield Differential
     * Effects in Pressure-Insensitive Metals.”
     * International Journal of Plasticity,
     * Daniel c. Drucker memorial issue, 20 (11):2027–45.
     * https://doi.org/10.1016/j.ijplas.2003.11.021.
     */
    struct Cazacu2004IsotropicStressCriterion final : StressCriterionBase {
      std::vector<OptionDescription> getOptions() const override;
      std::vector<BehaviourSymmetry> getSupportedBehaviourSymmetries()
          const override;
      void initialize(BehaviourDescription&,
                      AbstractBehaviourDSL&,
                      const std::string&,
                      const DataMap&,
                      const Role) override;
      void endTreatment(BehaviourDescription&,
                        const AbstractBehaviourDSL&,
                        const std::string&,
                        const Role) override;
      std::string computeElasticPrediction(
          const std::string&,
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
      //! destructor
      ~Cazacu2004IsotropicStressCriterion() override;

     protected:
      //! \brief c parameter
      BehaviourDescription::MaterialProperty cp;
    };  // end of struct Cazacu2004IsotropicStressCriterion

  }  // end of namespace bbrick

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURBRICK_CAZACU2004ISOTROPICSTRESSCRITERION_HXX */
