/*!
 * \file   include/MFront/BehaviourBrick/ChuNeedleman1980StrainBasedPorosityNucleationModel.hxx
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

#ifndef LIB_MFRONT_BEHAVIOURBRICK_CHUNEEDLEMAN1980STRAINBASEDPOROSITYNUCLEATIONMODEL_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_CHUNEEDLEMAN1980STRAINBASEDPOROSITYNUCLEATIONMODEL_HXX

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
    struct ChuNeedleman1980StrainBasedPorosityNucleationModel
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
      //! \brief destructor
      ~ChuNeedleman1980StrainBasedPorosityNucleationModel() override;
      //! 
      BehaviourDescription::MaterialProperty fn;
      //! 
      BehaviourDescription::MaterialProperty en;
      //! 
      BehaviourDescription::MaterialProperty sn;
    };  // end of struct ChuNeedleman1980StrainBasedPorosityNucleationModel

  }  // end of namespace bbrick

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURBRICK_CHUNEEDLEMAN1980STRAINBASEDPOROSITYNUCLEATIONMODEL_HXX */
