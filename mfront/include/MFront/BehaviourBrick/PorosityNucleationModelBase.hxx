/*!
 * \file   include/MFront/BehaviourBrick/PorosityNucleationModelBase.hxx
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

#ifndef LIB_MFRONT_BEHAVIOURBRICK_POROSITYNUCLEATIONMODELBASE_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_POROSITYNUCLEATIONMODELBASE_HXX

#include "MFront/BehaviourBrick/PorosityEvolutionAlgorithm.hxx"
#include "MFront/BehaviourBrick/PorosityNucleationModel.hxx"

namespace mfront {

  namespace bbrick {

    //! \brief class describing an inelastic flow.
    struct MFRONT_VISIBILITY_EXPORT PorosityNucleationModelBase
        : PorosityNucleationModel {
      std::vector<OptionDescription> getOptions() const override;
      void initialize(BehaviourDescription&,
                      AbstractBehaviourDSL&,
                      const std::string&,
                      const DataMap&) override;
      void completeVariableDeclaration(
          BehaviourDescription&,
          const AbstractBehaviourDSL&,
          const std::map<std::string, std::shared_ptr<bbrick::InelasticFlow>>&,
          const std::string&) const override;
      void endTreatment(
          BehaviourDescription&,
          const AbstractBehaviourDSL&,
          const StressPotential&,
          const std::map<std::string, std::shared_ptr<bbrick::InelasticFlow>>&,
          const std::string&) const override;
      //! \brief destructor
      ~PorosityNucleationModelBase() override;

     protected:
      /*!
       * \brief material coefficients associated with a nucleation model
       */
      struct MaterialCoefficientDescription {
        //! \brief type of the material coefficient
        std::string type;
        //! \brief name of the material coefficient
        std::string name;
        //! \brief description of the material coefficient
        std::string description;
      };  // end of struct MaterialCoefficientDescripion
          /*!
           * \return a list of material coefficients
           * Those material coefficients are:
           * - automatically declared as options to the nucleation model
           * - initialized in the `@InitLocalVariables` code block
           */
      virtual std::vector<MaterialCoefficientDescription>
      getMaterialCoefficientDescriptions() const;
      /*!
       * \return a boolean stating if the nucleated porosity must be saved
       */
      virtual bool requiresSavingNucleatedPorosity() const = 0;
      /*!
       * \brief list of material coefficients.
       *
       * Those list of material coefficients are initialized in the `initialize`
       * method using the information retrieved from the
       * `getMaterialCoefficients` method
       */
      std::map<std::string, BehaviourDescription::MaterialProperty>
          material_coefficients;
      /*!
       * \brief flag stating if the contribution to the porosity nucleation
       * associated with this model must be saved in a dedicated
       * auxiliary state variable.
       */
      bool save_porosity_increase = false;
      //! \brief algorithm used to handle the porosity evolution
      PorosityEvolutionAlgorithm porosity_evolution_algorithm =
          PorosityEvolutionAlgorithm::STAGGERED_SCHEME;
    };  // end of struct PorosityNucleationModelBase

  }  // end of namespace bbrick

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURBRICK_POROSITYNUCLEATIONMODELBASE_HXX */
