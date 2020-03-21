/*!
 * \file   mtest/src/PipeHybridHighOrderElement.cxx
 * \brief
 * \author Thomas Helfer
 * \date   10 déc. 2015
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <iostream>
#include <algorithm>

#include "HybridHighOrder/Geometry/Line.hxx"
#include "HybridHighOrder/Element/Element.hxx"
#include "HybridHighOrder/MonodimensionalElements/ElementFactory.hxx"

#include "TFEL/Raise.hxx"
#include "TFEL/Material/ModellingHypothesis.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MTest/RoundingMode.hxx"
#include "MTest/Behaviour.hxx"
#include "MTest/BehaviourWorkSpace.hxx"
#include "MTest/CurrentState.hxx"
#include "MTest/StructureCurrentState.hxx"
#include "MTest/PipeMesh.hxx"
#include "MTest/HybridHighOrder/SmallStrainBehaviourWrapper.hxx"
#include "MTest/HybridHighOrder/FiniteStrainBehaviourWrapper.hxx"
#include "MTest/PipeHybridHighOrderElement.hxx"

namespace mtest {

  struct PipeHybridHighOrderElement::PrivateMembers {
    //! \brief geometric element
    std::unique_ptr<::hho::geometry::Line<1u>> geometric_element;
    //! \brief geometric element
    std::unique_ptr<::hho::element::Behaviour> behaviour;
    //     //! \brief HHO element
    //     std::unique_ptr<::hho::element::Element> hho_element;
  };  // end of struct PipeHybridHighOrderElement::PrivateMembers

  PipeHybridHighOrderElement::PipeHybridHighOrderElement(const PipeMesh& m,
                                                         const Behaviour& b,
                                                         const size_t n)
      : PipeElementBase(m, b, n) {
    using BehaviourBase = tfel::material::MechanicalBehaviourBase;
    const auto& f = ::hho::monodimensional_elements::ElementFactory::get();
    // mandatory keys
    const auto mkeys = std::vector<const char*>{"cell_polynomial_degree"};
    // optional keys
    const auto okeys = std::vector<const char*>{};
    auto raise = [](const std::string& msg) {
      tfel::raise("PipeHybridHighOrderElement::PipeHybridHighOrderElement: " +
                  std::string(msg));
    };
    const auto& ed = m.edata;
    if (!ed.is<std::map<std::string, tfel::utilities::Data>>()) {
      raise("internal error, invalid element data type");
    }
    const auto& md = ed.get<std::map<std::string, tfel::utilities::Data>>();
    // some checks
    for (const auto& d : md) {
      if ((std::find(mkeys.cbegin(), mkeys.cend(), d.first) == mkeys.cend()) &&
          (std::find(okeys.cbegin(), okeys.cend(), d.first) == okeys.cend())) {
        raise("invalid key '" + d.first + "'");
      }
    }
    for (const auto& k : mkeys) {
      if (md.find(k) == md.end()) {
        raise("missing key '" + std::string{k} + "'");
      }
    }
    // polynomial degree
    const auto kc = md.at(mkeys[0]);
    if ((!kc.is<int>()) || (kc.get<int>() < 0)) {
      raise("invalid value for '" + std::string{mkeys[0]} + "'");
    }
    this->pm = std::unique_ptr<PrivateMembers>{new PrivateMembers};
    this->pm->geometric_element = std::unique_ptr<::hho::geometry::Line<1u>>(
        new ::hho::geometry::Line<1u>{
            ::hho::geometry::Point<1u>{this->inner_radius},
            ::hho::geometry::Point<1u>{this->outer_radius}});
    if ((b.getBehaviourType() == BehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR) &&
        (b.getBehaviourKinematic() == BehaviourBase::SMALLSTRAINKINEMATIC)) {
      this->pm->behaviour = std::unique_ptr<::hho::element::Behaviour>(
          new mtest::hho::SmallStrainBehaviourWrapper(b));
    } else if ((b.getBehaviourType() ==
                BehaviourBase::STANDARDFINITESTRAINBEHAVIOUR) &&
               (b.getBehaviourKinematic() ==
                BehaviourBase::FINITESTRAINKINEMATIC_ETO_PK1)) {
      this->pm->behaviour = std::unique_ptr<::hho::element::Behaviour>(
          new mtest::hho::FiniteStrainBehaviourWrapper(b));
    } else {
      tfel::raise("Unsupported behaviour type");
    }
    //     this->pm->hho_element = f.generate("FiniteStrainSolidMechanics",
    //                                        "AxisymmetricalGeneralisedPlaneStrain",
    //                                        "StandardElement_" +
    //                                        std::to_string{kc},
    //                                        this->pm->geometric_element,
    //                                        this->pm->behaviour);
  }  // end of PipeHybridHighOrderElement::PipeHybridHighOrderElement

  size_type PipeHybridHighOrderElement::getNumberOfNodes() const {
    return 2;
  }  // end of PipeHybridHighOrderElement::getNumberOfNodes

  size_type PipeHybridHighOrderElement::getNumberOfIntegrationPoints() const {
    return 4;
  }  // end of PipeHybridHighOrderElement::getNumberOfIntegrationPoints

  void PipeHybridHighOrderElement::setIntegrationPointsPositions(
      StructureCurrentState& scs) const {
  }

  void PipeHybridHighOrderElement::computeStrain(
      StructureCurrentState& scs,
      const tfel::math::vector<real>& u,
      const bool b) const {
  }  // end of PipeHybridHighOrderElement::computeStrain

  std::pair<bool, real>
  PipeHybridHighOrderElement::updateStiffnessMatrixAndInnerForces(
      tfel::math::matrix<real>& k,
      tfel::math::vector<real>& r,
      StructureCurrentState& scs,
      const tfel::math::vector<real>& u1,
      const real dt,
      const StiffnessMatrixType mt) const {
    return {true, 1};
  }

  PipeHybridHighOrderElement::~PipeHybridHighOrderElement() = default;

}  // end of namespace mtest
