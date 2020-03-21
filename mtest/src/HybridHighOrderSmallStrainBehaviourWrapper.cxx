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
#include "MTest/HybridHighOrder/SmallStrainBehaviourWrapper.hxx"

namespace mtest{

  namespace hho {

    SmallStrainBehaviourWrapper::SmallStrainBehaviourWrapper(
        const mtest::Behaviour& b)
        : BehaviourWrapperBase(b) {
      if (this->behaviour.getBehaviourType() !=
          tfel::material::MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR){
        tfel::raise(
            "SmallStrainBehaviourWrapper::SmallStrainBehaviourWrapper: "
            "invalid behaviour type");
      }
      if (this->behaviour.getBehaviourKinematic() !=
          tfel::material::MechanicalBehaviourBase::SMALLSTRAINKINEMATIC) {
        tfel::raise(
            "SmallStrainBehaviourWrapper::SmallStrainBehaviourWrapper: "
            "invalid behaviour kinematic");
      }
    }  // end of SmallStrainBehaviourWrapper::SmallStrainBehaviourWrapper

    ::hho::element::Behaviour::BehaviourType
   SmallStrainBehaviourWrapper::get_behaviour_type() const {
      return ::hho::element::Behaviour::STANDARDSTRAINBASEDBEHAVIOUR;
    }  // end ofSmallStrainBehaviourWrapper::get_behaviour_type

    ::hho::element::Behaviour::Kinematic
   SmallStrainBehaviourWrapper::get_behaviour_kinematic() const {
      return ::hho::element::Behaviour::SMALLSTRAINKINEMATIC;
    }  // end ofSmallStrainBehaviourWrapper::get_behaviour_kinematic

    SmallStrainBehaviourWrapper::~SmallStrainBehaviourWrapper() noexcept = default;

  }  // end of namespace hho

} // end of namespace mtest

