/*!
 * \file   mfront/src/DefaultCZMDSL.cxx
 * \brief
 * \author Thomas Helfer
 * \date   08 nov 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <string>
#include <fstream>
#include <stdexcept>

#include "MFront/AbstractBehaviourInterface.hxx"
#include "MFront/BehaviourInterfaceFactory.hxx"
#include "MFront/DefaultCZMCodeGenerator.hxx"
#include "MFront/DefaultCZMDSL.hxx"

namespace mfront {

  DefaultCZMDSL::DefaultCZMDSL(const DSLOptions& opts) : DefaultDSLBase(opts) {
    const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    this->mb.setDSLName("DefaultCZM");
    this->mb.declareAsACohesiveZoneModel();
    this->mb.addLocalVariable(h, VariableDescription("real&", "u_n", 1u, 0u));
    this->mb.addLocalVariable(h, VariableDescription("real&", "du_n", 1u, 0u));
    this->mb.addLocalVariable(h, VariableDescription("real&", "t_n", 1u, 0u));
    this->mb.addLocalVariable(
        h,
        VariableDescription("tfel::math::View<tfel::math::tvector<N-1,real>>",
                            "u_t", 1u, 0u));
    this->mb.addLocalVariable(
        h,
        VariableDescription("tfel::math::View<tfel::math::tvector<N-1,real>>",
                            "du_t", 1u, 0u));
    this->mb.addLocalVariable(
        h,
        VariableDescription("tfel::math::View<tfel::math::tvector<N-1,real>>",
                            "t_t", 1u, 0u));
    this->mb.addLocalVariable(h, VariableDescription("real&", "Dt_nn", 1u, 0u));
    this->mb.addLocalVariable(
        h,
        VariableDescription("tfel::math::View<tfel::math::tvector<N-1,real>>",
                            "Dt_nt", 1u, 0u));
    this->mb.addLocalVariable(
        h, VariableDescription("tfel::math::View<tfel::math::tvector<N-1,real>,"
                               "tfel::math::FixedSizeVectorIndexingPolicy<"
                               "unsigned short, N-1, N>>",
                               "Dt_tn", 1u, 0u));
    this->mb.addLocalVariable(
        h, VariableDescription(
               "tfel::math::View<tfel::math::tmatrix<N-1, N-1, real>,"
               "tfel::math::FixedSizeRowMajorMatrixIndexingPolicy<unsigned "
               "short, N-1, N-1, N>>",
               "Dt_tt", 1u, 0u));
  }

  std::unique_ptr<AbstractBehaviourCodeGenerator>
  DefaultCZMDSL::getCodeGenerator() const {
    return std::make_unique<DefaultCZMCodeGenerator>(this->fd, this->mb,
                                                     this->interfaces);
  }  // end of getCodeGenerator

  std::string DefaultCZMDSL::getDescription() {
    return "this parser is the most generic one as it does not make any "
           "restriction "
           "on the behaviour or the integration method that may be used";
  }  // end of DefaultCZMDSL::getDescription

  std::string DefaultCZMDSL::getName() { return "DefaultCZMDSL"; }

  BehaviourDSLDescription DefaultCZMDSL::getBehaviourDSLDescription() const {
    auto d = BehaviourDSLDescription{};
    d.behaviourType =
        tfel::material::MechanicalBehaviourBase::COHESIVEZONEMODEL;
    d.integrationScheme = IntegrationScheme::USERDEFINEDSCHEME;
    d.typicalCodeBlocks = {BehaviourData::ComputePredictionOperator,
                           BehaviourData::Integrator,
                           BehaviourData::ComputeTangentOperator};
    d.allowCrystalStructureDefinition = false;
    d.minimalMFrontFileBody = "@Integrator{}\n\n";
    return d;
  }  // end of DefaultCZMDSL::getBehaviourDSLDescription

  DefaultCZMDSL::~DefaultCZMDSL() = default;

}  // end of namespace mfront
