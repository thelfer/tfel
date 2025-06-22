/*!
 * \file   mfront/src/AbstractBehaviourBrickFactory.cxx
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
#include "MFront/AbstractBehaviourBrickFactory.hxx"
#include "MFront/StandardElasticityBrick.hxx"
#include "MFront/StandardElastoViscoPlasticityBrick.hxx"
#include "MFront/DDIF2Brick.hxx"
#include "MFront/FiniteStrainSingleCrystalBrick.hxx"

namespace mfront {

  template <typename T>
  static std::shared_ptr<AbstractBehaviourBrick> buildBehaviourBrickConstructor(
      AbstractBehaviourDSL& dsl,
      BehaviourDescription& mb,
      const AbstractBehaviourBrick::Parameters& bp,
      AbstractBehaviourBrick::tokens_iterator& p,
      const AbstractBehaviourBrick::tokens_iterator pe) {
    const auto d = [&p, pe] {
      using DataMap = std::map<std::string, tfel::utilities::Data>;
      if ((p != pe) && (p->value == "{")) {
        tfel::utilities::DataParsingOptions o;
        o.allowMultipleKeysInMap = true;
        return tfel::utilities::Data::read(p, pe, o).get<DataMap>();
      }
      return DataMap();
    }();
    return std::make_shared<T>(dsl, mb, bp, d);
  }  // end of buildAlgoritmConstructor

  AbstractBehaviourBrickFactory& AbstractBehaviourBrickFactory::getFactory() {
    static AbstractBehaviourBrickFactory f;
    return f;
  }

  std::shared_ptr<AbstractBehaviourBrick> AbstractBehaviourBrickFactory::get(
      const std::string& a,
      AbstractBehaviourDSL& dsl,
      BehaviourDescription& mb,
      const AbstractBehaviourBrick::Parameters& bp,
      tokens_iterator& p,
      const tokens_iterator pe) const {
    const auto pc = this->constructors.find(a);
    if (pc == this->constructors.end()) {
      auto msg = std::string{};
      msg += "AbstractBehaviourBrickFactory::getAbstractBehaviourBrick: ";
      msg += "no behaviour brick named '" + a + "' registred. ";
      msg += "Available bricks are:";
      for (const auto& c : this->constructors) {
        msg += "\n- " + c.first;
      }
      tfel::raise(msg);
    }
    return (*(pc->second))(dsl, mb, bp, p, pe);
  }

  AbstractBehaviourBrickFactory::AbstractBehaviourBrickFactory() {
    auto add = [this](const char* n, const constructor c) {
      this->registerAbstractBehaviourBrick(n, c);
    };
    add("DDIF2", buildBehaviourBrickConstructor<DDIF2Brick>);
    add("StandardElasticity",
        buildBehaviourBrickConstructor<StandardElasticityBrick>);
    add("StandardElastoViscoPlasticity",
        buildBehaviourBrickConstructor<StandardElastoViscoPlasticityBrick>);
    add("FiniteStrainSingleCrystal",
        buildBehaviourBrickConstructor<FiniteStrainSingleCrystalBrick>);
  }  // end of AbstractBehaviourBrickFactory::AbstractBehaviourBrickFactory

  std::vector<std::string> AbstractBehaviourBrickFactory::getRegistredBricks()
      const {
    auto names = std::vector<std::string>{};
    std::for_each(std::begin(this->constructors), std::end(this->constructors),
                  [&names](const std::pair<std::string, constructor>& c) {
                    names.push_back(c.first);
                  });
    return names;
  }  // end of AbstractBehaviourBrickFactory::getRegistredBricks

  void AbstractBehaviourBrickFactory::registerAbstractBehaviourBrick(
      const std::string& a, const constructor c) {
    tfel::raise_if(
        !this->constructors.insert({a, c}).second,
        "AbstractBehaviourBrickFactory::registerAbstractBehaviourBrick : "
        "BehaviourBrick '" +
            a + "' already declared");
  }  // end of AbstractBehaviourBrickFactory::registerAbstractBehaviourBrick

}  // end of namespace mfront
