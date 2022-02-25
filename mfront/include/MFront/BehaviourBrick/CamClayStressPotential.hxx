/*!
 * \file   mfront/include/MFront/BehaviourBrick/CamClayStressPotential.hxx
 * \brief
 * \author Thomas Helfer
 * \date   25/02/2022
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 * <!-- Local IspellDict: english -->
 */

#ifndef LIB_MFRONT_BEHAVIOURBRICK_CAMCLAYSTRESSPOTENTIAL_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_CAMCLAYSTRESSPOTENTIAL_HXX

#include "MFront/BehaviourBrick/StressPotential.hxx"

namespace mfront::bbrick {

  /*!
   * \brief Implement a modified version of the Cam-Clay stress potential
   */
  struct CamClayStressPotential : public StressPotential {
    //! \brief default constructor
    CamClayStressPotential();
    //
    std::string getName() const override;
    std::vector<OptionDescription> getOptions(const BehaviourDescription&,
                                              const bool) const override;
    void initialize(BehaviourDescription&,
                    AbstractBehaviourDSL&,
                    const DataMap&) override;
    std::vector<Hypothesis> getSupportedModellingHypotheses(
        const BehaviourDescription&,
        const AbstractBehaviourDSL&) const override;
    void completeVariableDeclaration(
        BehaviourDescription&, const AbstractBehaviourDSL&) const override;
    void endTreatment(BehaviourDescription&,
                      const AbstractBehaviourDSL&) const override;
    std::vector<
        std::tuple<std::string, std::string, mfront::SupportedTypes::TypeFlag>>
    getStressDerivatives(const BehaviourDescription& bd) const override;
    std::string generateImplicitEquationDerivatives(const BehaviourDescription&,
                                                    const std::string&,
                                                    const std::string&,
                                                    const std::string&,
                                                    const bool) const override;
    void computeElasticPrediction(BehaviourDescription&) const override;
    std::string getStressNormalisationFactor(
        const BehaviourDescription&) const override;
    std::string getEquivalentStressLowerBound(
        const BehaviourDescription&) const override;
    //! \brief destructor
    ~CamClayStressPotential();
  };  // end of struct CamClayStressPotential

}  // end of namespace mfront::bbrick

#endif /* LIB_MFRONT_BEHAVIOURBRICK_CAMCLAYSTRESSPOTENTIAL_HXX */
