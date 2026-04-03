/*!
 * \file   mfront/src/BehaviourBrickFactory.cxx
 * \brief
 * \author Thomas Helfer
 * \brief  October 20, 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <stdexcept>
#include <algorithm>

#include "TFEL/Raise.hxx"
#include "TFEL/Utilities/Data.hxx"
#include "MFront/AbstractBehaviourBrick.hxx"
#include "MFront/BehaviourBrickFactory.hxx"
#include "MFront/StandardElasticityBrick.hxx"
#include "MFront/StandardElastoViscoPlasticityBrick.hxx"
#include "MFront/DDIF2Brick.hxx"
#include "MFront/FiniteStrainSingleCrystalBrick.hxx"

namespace mfront {

  template <typename T>
  static std::shared_ptr<AbstractBehaviourBrick> buildBehaviourBrickConstructor(
      AbstractBehaviourDSL& dsl, BehaviourDescription& mb) {
    return std::make_shared<T>(dsl, mb);
  }  // end of buildAlgoritmConstructor

  BehaviourBrickFactory& BehaviourBrickFactory::getFactory() {
    static BehaviourBrickFactory f;
    return f;
  }

  std::shared_ptr<AbstractBehaviourBrick> BehaviourBrickFactory::get(
      const std::string& a,
      AbstractBehaviourDSL& dsl,
      BehaviourDescription& mb) const {
    const auto pc = this->constructors.find(a);
    if (pc == this->constructors.end()) {
      auto msg = std::string{};
      msg += "BehaviourBrickFactory::getAbstractBehaviourBrick: ";
      msg += "no behaviour brick named '" + a + "' registred. ";
      msg += "Available bricks are:";
      for (const auto& c : this->constructors) {
        msg += "\n- " + c.first;
      }
      tfel::raise(msg);
    }
    return (*(pc->second))(dsl, mb);
  }

  BehaviourBrickFactory::BehaviourBrickFactory() {
    auto add = [this](const char* n, const BehaviourType t,
                      const IntegrationScheme i, const constructor c) {
      this->registerAbstractBehaviourBrick(n, t, i, c);
    };
    add("DDIF2", BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR,
        BehaviourDescription::IMPLICITSCHEME,
        buildBehaviourBrickConstructor<DDIF2Brick>);
    add("StandardElasticity",
        BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR,
        BehaviourDescription::IMPLICITSCHEME,
        buildBehaviourBrickConstructor<StandardElasticityBrick>);
    add("StandardElastoViscoPlasticity",
        BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR,
        BehaviourDescription::IMPLICITSCHEME,
        buildBehaviourBrickConstructor<StandardElastoViscoPlasticityBrick>);
    add("FiniteStrainSingleCrystal",
        BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR,
        BehaviourDescription::IMPLICITSCHEME,
        buildBehaviourBrickConstructor<FiniteStrainSingleCrystalBrick>);
  }  // end of BehaviourBrickFactory::BehaviourBrickFactory

  std::vector<std::string> BehaviourBrickFactory::getRegistredBricks() const {
    auto names = std::vector<std::string>{};
    std::for_each(std::begin(this->constructors), std::end(this->constructors),
                  [&names](const std::pair<std::string, constructor>& c) {
                    names.push_back(c.first);
                  });
    return names;
  }  // end of BehaviourBrickFactory::getRegistredBricks

  std::vector<std::string> BehaviourBrickFactory::getRegistredBricks(
      const BehaviourType t, const IntegrationScheme i) const {
    auto names = std::vector<std::string>{};
    std::for_each(
        std::begin(this->constructors), std::end(this->constructors),
        [this, &names, &t, &i](const std::pair<std::string, constructor>& c) {
          const auto pt = this->btypes.find(c.first);
          if (pt == this->btypes.end()) {
            return;
          }
          if (std::find(pt->second.cbegin(), pt->second.cend(), t) ==
              pt->second.cend()) {
            return;
          }
          const auto pi = this->ischemes.find(c.first);
          if (pi == this->ischemes.end()) {
            return;
          }
          if (std::find(pi->second.cbegin(), pi->second.cend(), i) ==
              pi->second.cend()) {
            return;
          }
          names.push_back(c.first);
        });
    return names;
  }  // end of BehaviourBrickFactory::getRegistredBricks

  void BehaviourBrickFactory::registerAbstractBehaviourBrick(
      const std::string& a, const BehaviourType t, const constructor c) {
    tfel::raise_if(!this->constructors.insert({a, c}).second,
                   "BehaviourBrickFactory::registerAbstractBehaviourBrick : "
                   "BehaviourBrick '" +
                       a + "' already declared");
    this->btypes[a] = {t};
  }  // end of BehaviourBrickFactory::registerAbstractBehaviourBrick

  void BehaviourBrickFactory::registerAbstractBehaviourBrick(
      const std::string& a,
      const BehaviourType t,
      const IntegrationScheme i,
      const constructor c) {
    tfel::raise_if(!this->constructors.insert({a, c}).second,
                   "BehaviourBrickFactory::registerAbstractBehaviourBrick : "
                   "BehaviourBrick '" +
                       a + "' already declared");
    this->btypes[a] = {t};
    this->ischemes[a] = {i};
  }  // end of BehaviourBrickFactory::registerAbstractBehaviourBrick

  void BehaviourBrickFactory::registerAbstractBehaviourBrick(
      const std::string& a,
      const std::vector<BehaviourType> ts,
      const std::vector<IntegrationScheme> is,
      const constructor c) {
    tfel::raise_if(!this->constructors.insert({a, c}).second,
                   "BehaviourBrickFactory::registerAbstractBehaviourBrick : "
                   "BehaviourBrick '" +
                       a + "' already declared");
    this->btypes[a] = ts;
    this->ischemes[a] = is;
  }  // end of BehaviourBrickFactory::registerAbstractBehaviourBrick

}  // end of namespace mfront
