/*!
 * \file
 * mfront/include/MFront/BehaviourBrick/IsotropicDamageHookeStressPotentialBase.hxx
 * \brief
 * \author Thomas Helfer
 * \date   30/04/2018
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 * <!-- Local IspellDict: english -->
 */

#ifndef LIB_MFRONT_BEHAVIOURBRICK_ISOTROPICDAMAGEHOOKESTRESSPOTENTIALBASE_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_ISOTROPICDAMAGEHOOKESTRESSPOTENTIALBASE_HXX

#include "MFront/BehaviourBrick/HookeStressPotentialBase.hxx"

namespace mfront {

  // forward declaration
  struct MaterialPropertyDescription;

  namespace bbrick {

    /*!
     * \brief base class for stress potential describing the computation of the
     * stress through the Hooke law coupled with an isotropic damage.
     */
    struct IsotropicDamageHookeStressPotentialBase : HookeStressPotentialBase {
      //! \brief constructor
      IsotropicDamageHookeStressPotentialBase();
      std::vector<OptionDescription> getOptions() const override;
      void initialize(BehaviourDescription&,
                      AbstractBehaviourDSL&,
                      const DataMap&) override;
      std::string computeDerivatives(const BehaviourDescription&,
                                     const std::string&,
                                     const std::string&,
                                     const std::string&,
                                     const bool) const override;
      //! \brief destructor
      ~IsotropicDamageHookeStressPotentialBase() override;

     protected:
      void declareComputeElasticPredictionMethod(
          BehaviourDescription&) const override;
      void declareComputeStressWhenStiffnessTensorIsDefined(
          BehaviourDescription&) const override;
      void declareComputeStressForIsotropicBehaviour(
          BehaviourDescription&, LocalDataStructure&) const override;
      void addGenericTangentOperatorSupport(
          BehaviourDescription&, const AbstractBehaviourDSL&) const override;
      void addGenericPredictionOperatorSupport(
          BehaviourDescription&) const override;
    };  // end of struct IsotropicDamageHookeStressPotentialBase

  }  // end of namespace bbrick

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURBRICK_ISOTROPICDAMAGEHOOKESTRESSPOTENTIALBASE_HXX \
        */
