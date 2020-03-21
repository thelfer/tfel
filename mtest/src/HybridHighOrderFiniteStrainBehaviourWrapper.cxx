/*!
 * \file   mtest/src/HybridHighOrderSmallStrainBehaviourWrapper.cxx
 * \brief    
 * \author Thomas Helfer
 * \date   19/03/2020
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "TFEL/Raise.hxx"
#include "MTest/Behaviour.hxx"
#include "MTest/HybridHighOrder/FiniteStrainBehaviourWrapper.hxx"

namespace mtest{

  namespace hho {

    FiniteStrainBehaviourWrapper::FiniteStrainBehaviourWrapper(
        const mtest::Behaviour& b)
        : BehaviourWrapperBase(b) {
      if (this->behaviour.getBehaviourType() !=
          tfel::material::MechanicalBehaviourBase::STANDARDFINITESTRAINBEHAVIOUR){
        tfel::raise(
            "FiniteStrainBehaviourWrapper::FiniteStrainBehaviourWrapper: "
            "invalid behaviour type");
      }
      if (this->behaviour.getBehaviourKinematic() !=
          tfel::material::MechanicalBehaviourBase::FINITESTRAINKINEMATIC_ETO_PK1) {
        tfel::raise(
            "FiniteStrainBehaviourWrapper::FiniteStrainBehaviourWrapper: "
            "invalid behaviour kinematic");
      }
    }  // end of FiniteStrainBehaviourWrapper::FiniteStrainBehaviourWrapper

    ::hho::element::Behaviour::BehaviourType
   FiniteStrainBehaviourWrapper::get_behaviour_type() const {
      return ::hho::element::Behaviour::STANDARDFINITESTRAINBEHAVIOUR;
    }  // end ofFiniteStrainBehaviourWrapper::get_behaviour_type

    ::hho::element::Behaviour::Kinematic
   FiniteStrainBehaviourWrapper::get_behaviour_kinematic() const {
      return ::hho::element::Behaviour::FINITESTRAINKINEMATIC_ETO_PK1;
    }  // end ofFiniteStrainBehaviourWrapper::get_behaviour_kinematic

    FiniteStrainBehaviourWrapper::~FiniteStrainBehaviourWrapper() noexcept = default;

  }  // end of namespace hho

} // end of namespace mtest

