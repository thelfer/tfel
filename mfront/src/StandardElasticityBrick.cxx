/*!
 * \file  StandardElasticityBrick.cxx
 * \brief
 * \author Thomas Helfer
 * \date   20 oct. 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <sstream>
#include <stdexcept>

#include "TFEL/Raise.hxx"
#include "TFEL/Glossary/Glossary.hxx"
#include "TFEL/Glossary/GlossaryEntry.hxx"
#include "TFEL/Utilities/Data.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MFront/AbstractBehaviourDSL.hxx"
#include "MFront/LocalDataStructure.hxx"
#include "MFront/BehaviourDescription.hxx"
#include "MFront/ImplicitDSLBase.hxx"
#include "MFront/NonLinearSystemSolver.hxx"
#include "MFront/BehaviourBrick/StressPotential.hxx"
#include "MFront/BehaviourBrick/StressPotentialFactory.hxx"
#include "MFront/StandardElasticityBrick.hxx"

namespace mfront {

  StandardElasticityBrick::StandardElasticityBrick(AbstractBehaviourDSL& dsl_,
                                                   BehaviourDescription& mb_,
                                                   const Parameters& p,
                                                   const DataMap& d)
      : BehaviourBrickBase(dsl_, mb_) {
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(b,
                     "StandardElasticityBrick::StandardElasticityBrick: " + m);
    };
    auto& spf = mfront::bbrick::StressPotentialFactory::getFactory();
    this->hooke = spf.generate("Hooke");
    this->hooke->initialize(this->bd, this->dsl, d);
    // parameters
    for (const auto& pp : p) {
      if (pp.first == "Isotropic") {
        this->checkThatParameterHasNoValue(pp);
        setElasticSymmetryType(this->bd, mfront::ISOTROPIC);
      } else if (pp.first == "Orthotropic") {
        // this is also checked later during the call of
        // setElasticSymmetryType call
        throw_if(this->bd.getSymmetryType() != mfront::ORTHOTROPIC,
                 "using the orthotropic behaviour requires that the "
                 "behaviour has not been declared orthotropic");
        this->checkThatParameterHasNoValue(pp);
        setElasticSymmetryType(this->bd, mfront::ORTHOTROPIC);
      } else {
        throw_if(true, "unsupported parameter '" + pp.first + "'");
      }
    }
  }  // end of StandardElasticityBrick::StandardElasticityBrick

  std::string StandardElasticityBrick::getName() const { return "Elasticity"; }

  void StandardElasticityBrick::completeVariableDeclaration() const {
    this->hooke->completeVariableDeclaration(this->bd, this->dsl);
  }

  void StandardElasticityBrick::endTreatment() const {
    this->hooke->endTreatment(this->bd, this->dsl);
  }  // end of StandardElasticityBrick::endTreatment

  std::vector<StandardElasticityBrick::Hypothesis>
  StandardElasticityBrick::getSupportedModellingHypotheses() const {
    return this->hooke->getSupportedModellingHypotheses(this->bd, this->dsl);
  }  // end of StandardElasticityBrick::getSupportedModellingHypothesis

  StandardElasticityBrick::~StandardElasticityBrick() = default;

}  // end of namespace mfront
