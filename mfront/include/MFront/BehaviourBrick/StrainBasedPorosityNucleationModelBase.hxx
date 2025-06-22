/*!
 * \file
 * include/MFront/BehaviourBrick/StrainBasedPorosityNucleationModelBase.hxx
 * \brief
 * \author Thomas Helfer
 * \date   05/04/2020
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_BEHAVIOURBRICK_STRAINBASEDPOROSITYNUCLEATIONMODELBASE_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_STRAINBASEDPOROSITYNUCLEATIONMODELBASE_HXX

#include "MFront/BehaviourBrick/PorosityNucleationModelBase.hxx"

namespace mfront::bbrick {

  /*!
   * \brief A base class for nucleation models of the form:
   * \f[
   * \dfrac{\mathrm{d}f}{\mathrm{d}t}=An(p)\,\dfrac{\mathrm{d}p}{\mathrm{d}t}
   * \f]
   * where:
   * - \f$f\f$ is the nucleated porosity.
   * - \f$p\f$ is the total equivalent plastic strain.
   */
  struct StrainBasedPorosityNucleationModelBase : PorosityNucleationModelBase {
    std::vector<OptionDescription> getOptions() const override;
    void initialize(BehaviourDescription&,
                    AbstractBehaviourDSL&,
                    const std::string&,
                    const DataMap&) override;
    void endTreatment(
        BehaviourDescription&,
        const AbstractBehaviourDSL&,
        const StressPotential&,
        const std::map<std::string, std::shared_ptr<bbrick::InelasticFlow>>&,
        const std::string&) const override;
    std::string updateNextEstimateOfThePorosityIncrement(
        const BehaviourDescription&,
        const std::map<std::string, std::shared_ptr<bbrick::InelasticFlow>>&,
        const std::string&) const override;
    bool requiresSavingNucleatedPorosity() const override final;
    //! \brief destructor
    ~StrainBasedPorosityNucleationModelBase() override;

   protected:
    /*!
     * \return the name of the underlying nucleation model.
     */
    virtual std::string getModelName() const = 0;
    /*!
     * \return a boolean stating if the nucleated porosity of the underlying
     * nucleation model must be limited.
     */
    virtual bool requiresLimitOnNucleationPorosity() const = 0;
    //!
    BehaviourDescription::MaterialProperty fmax;
  };  // end of struct StrainBasedPorosityNucleationModelBase

}  // end of namespace mfront::bbrick

#endif /* LIB_MFRONT_BEHAVIOURBRICK_STRAINBASEDPOROSITYNUCLEATIONMODELBASE_HXX \
        */
