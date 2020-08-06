/*!
 * \file   include/MFront/BehaviourBrick/PorosityNucleationModelBase.hxx
 * \brief
 * \author Thomas Helfer
 * \date   15/03/2018
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
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
      void initialize(BehaviourDescription&,
                      AbstractBehaviourDSL&,
                      const std::string&,
                      const DataMap&) override;
      std::vector<OptionDescription> getOptions() const override;
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
       * \return a boolean stating if the nucleated porosity must be saved
       */
      virtual bool requiresSavingNucleatedPorosity() const = 0;
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
