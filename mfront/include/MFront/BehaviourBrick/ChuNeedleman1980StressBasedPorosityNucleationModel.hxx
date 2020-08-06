/*!
 * \file   include/MFront/BehaviourBrick/ChuNeedleman1980StressBasedPorosityNucleationModel.hxx
 * \brief    
 * \author Thomas Helfer
 * \date   05/04/2020
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_BEHAVIOURBRICK_CHUNEEDLEMAN1980STRESSBASEDPOROSITYNUCLEATIONMODEL_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_CHUNEEDLEMAN1980STRESSBASEDPOROSITYNUCLEATIONMODEL_HXX

#include "MFront/BehaviourBrick/PorosityNucleationModelBase.hxx"

namespace mfront {

  namespace bbrick {

    /*!
     * \brief strain version of the nucleation model
     * proposed by Chu and Needleman.
     *
     * Chu, C. C., and A. Needleman. 1980.
     * “Void Nucleation Effects in Biaxially Stretched Sheets.”
     * Journal of Engineering Materials and Technology 102 (3): 249–56
     */
    struct ChuNeedleman1980StressBasedPorosityNucleationModel
        : PorosityNucleationModelBase {
      void initialize(BehaviourDescription&,
                      AbstractBehaviourDSL&,
                      const std::string&,
                      const DataMap&) override;
      std::vector<OptionDescription> getOptions() const override;
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
      std::string updateNextEstimateOfThePorosityIncrement(
          const BehaviourDescription&,
          const std::map<std::string, std::shared_ptr<bbrick::InelasticFlow>>&,
          const std::string&) const override;
      bool requiresSavingNucleatedPorosity() const override;
      //! \brief destructor
      ~ChuNeedleman1980StressBasedPorosityNucleationModel() override;

     protected:
      //! 
      BehaviourDescription::MaterialProperty fn;
      //! 
      BehaviourDescription::MaterialProperty sigm;
      //! 
      BehaviourDescription::MaterialProperty sn;
      //! 
      BehaviourDescription::MaterialProperty fmax;
    };  // end of struct ChuNeedleman1980StressBasedPorosityNucleationModel

  }  // end of namespace bbrick

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURBRICK_CHUNEEDLEMAN1980STRESSBASEDPOROSITYNUCLEATIONMODEL_HXX */
