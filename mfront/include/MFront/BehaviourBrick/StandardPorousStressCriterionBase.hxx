/*!
 * \file   MFront/BehaviourBrick/StandardPorousStressCriterionBase.hxx
 * \brief
 * \author Thomas Helfer
 * \date   25/03/2020
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_BEHAVIOURBRICK_STANDARDPOROUSSTRESSCRITERIONBASE_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_STANDARDPOROUSSTRESSCRITERIONBASE_HXX

#include <map>
#include <memory>
#include "MFront/MFrontConfig.hxx"
#include "MFront/BehaviourBrick/StressCriterionBase.hxx"

namespace mfront {

  // forward declaration
  struct AbstractBehaviourDSL;

}  // end of namespace mfront

namespace mfront::bbrick {

  // forward declaration
  struct OptionDescription;

  /*!
   * \brief an helper class used to build a stress criterion.
   * This class is meant to interface criterion described
   * in the TFEL/Material library.
   * The criterion is assumed to be coupled with the porosity evolution.
   */
  struct MFRONT_VISIBILITY_EXPORT StandardPorousStressCriterionBase
      : StressCriterionBase {
    /*!
     * \brief constructor
     * \param[in] n: name of the stress criterion
     */
    StandardPorousStressCriterionBase(const std::string& n);

    std::string computeElasticPrediction(const std::string&,
                                         const BehaviourDescription&,
                                         const StressPotential&) const override;
    void initialize(BehaviourDescription&,
                    AbstractBehaviourDSL&,
                    const std::string&,
                    const DataMap&,
                    const Role) override;
    void endTreatment(BehaviourDescription&,
                      const AbstractBehaviourDSL&,
                      const std::string&,
                      const Role) override;
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
    bool isCoupledWithPorosityEvolution() const override final;
    bool isNormalDeviatoric() const override final;
    //! destructor
    ~StandardPorousStressCriterionBase() override;

   protected:
    /*!
     * \brief initialize a material property not defined by the user
     * \param[in] n: name of the material property
     */
    virtual void initializeMissingMaterialProperty(const std::string&);
    /*!
     * \brief generate the initialization code for a  a material property not
     * defined by the user.
     * \param[in] bd: behaviour description
     * \param[in] dsl: abstract behaviour dsl
     * \param[in] id: identifier
     * \param[in] r: role
     * \param[in] n: name of the material property
     */
    virtual std::string generateMissingMaterialPropertyInitializationCode(
        BehaviourDescription&,
        const AbstractBehaviourDSL&,
        const std::string&,
        const Role,
        const std::string&);

   private:
    //! \brief name of the stress criterion
    const std::string name;
    //! \brief material properties
    std::map<std::string, BehaviourDescription::MaterialProperty> mps;
  };  // end of StandardPorousStressCriterionBase

}  // end of namespace mfront::bbrick

#endif /* LIB_MFRONT_BEHAVIOURBRICK_STANDARDPOROUSSTRESSCRITERIONBASE_HXX */
