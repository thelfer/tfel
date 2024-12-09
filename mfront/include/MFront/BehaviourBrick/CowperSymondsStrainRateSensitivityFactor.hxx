/*!
 * \file   MFront/BehaviourBrick/CowperSymondsStrainRateSensitivityFactor.hxx
 * \brief
 * \author Thomas Helfer
 * \date   27/11/2024
 */

#ifndef LIB_MFRONT_BBRICKS_COWPERSYMONDSSTRAINRATESENSITIVITYFACTOR_HXX
#define LIB_MFRONT_BBRICKS_COWPERSYMONDSSTRAINRATESENSITIVITYFACTOR_HXX

#include "MFront/BehaviourBrick/StrainRateSensitivityFactor.hxx"

namespace mfront::bbrick {

  /*!
   * \brief This class describes the Cowper-Symonds strain rate sensitivity
   * factor defined as:
   * \f[
   * 1 + A\,\left(\frac{\dot{p}}{\dot{p}_{0}}\right)^{n}
   * \f]
   */
  struct CowperSymondsStrainRateSensitivityFactor final
      : StrainRateSensitivityFactor {
    void initialize(BehaviourDescription&,
                    AbstractBehaviourDSL&,
                    const std::string&,
                    const std::string&,
                    const DataMap&) override;
    std::vector<OptionDescription> getOptions() const override;
    std::string computeStrainRateSensitivityFactor(
        const BehaviourDescription&,
        const std::string&,
        const std::string&) const override;
    std::string computeStrainRateSensitivityFactorAndDerivative(
        const BehaviourDescription&,
        const std::string&,
        const std::string&) const override;
    void endTreatment(BehaviourDescription&,
                      const AbstractBehaviourDSL&,
                      const std::string&,
                      const std::string&) const override;
    //! \brief destructor
    ~CowperSymondsStrainRateSensitivityFactor() override;

   protected:
    //! \brief coefficient
    BehaviourDescription::MaterialProperty A;
    //! \brief reference strain rate
    BehaviourDescription::MaterialProperty dp0;
    //! \brief exponent
    BehaviourDescription::MaterialProperty n;
    //! \brief threshold in the rate sensitiviy factor
    BehaviourDescription::MaterialProperty Rs_eps;
  };  // end of struct CowperSymondsStrainRateSensitivityFactor

}  // end of namespace mfront::bbrick

#endif /* LIB_MFRONT_BBRICKS_COWPERSYMONDSSTRAINRATESENSITIVITYFACTOR_HXX */
