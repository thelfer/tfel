/*!
 * \file   mfront/src/ImplicitCZMDSL.cxx
 * \brief
 * \author Thomas Helfer
 * \date   08 nov 2006
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
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
#include "MFront/ImplicitCZMDSL.hxx"

namespace mfront {

  ImplicitCZMDSL::ImplicitCZMDSL(const DSLOptions& opts)
      : ImplicitDSLBase(opts) {
    //    const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    this->mb.setDSLName("ImplicitCZM");
    this->mb.declareAsACohesiveZoneModel();
    this->registerNewCallBack("@ComputeCohesizeForce",
                              &ImplicitCZMDSL::treatComputeThermodynamicForces);
    this->registerNewCallBack(
        "@ComputeFinalCohesizeForce",
        &ImplicitCZMDSL::treatComputeFinalThermodynamicForces);
    this->registerNewCallBack("@ComputeCohesiveForce",
                              &ImplicitCZMDSL::treatComputeThermodynamicForces);
    this->registerNewCallBack(
        "@ComputeFinalCohesiveForce",
        &ImplicitCZMDSL::treatComputeFinalThermodynamicForces);
    //     this->mb.addLocalVariable(h, VariableDescription("real&", "u_n", 1u,
    //     0u)); this->mb.addLocalVariable(h, VariableDescription("real&",
    //     "du_n", 1u, 0u)); this->mb.addLocalVariable(h,
    //     VariableDescription("real&", "t_n", 1u, 0u));
    //     this->mb.addLocalVariable(
    //         h,
    //         VariableDescription("tfel::math::View<tfel::math::tvector<N-1,real>>",
    //                             "u_t", 1u, 0u));
    //     this->mb.addLocalVariable(
    //         h,
    //         VariableDescription("tfel::math::View<tfel::math::tvector<N-1,real>>",
    //                             "du_t", 1u, 0u));
    //     this->mb.addLocalVariable(
    //         h,
    //         VariableDescription("tfel::math::View<tfel::math::tvector<N-1,real>>",
    //                             "t_t", 1u, 0u));
    //     this->mb.addLocalVariable(h, VariableDescription("real&", "Dt_nn",
    //     1u, 0u)); this->mb.addLocalVariable(
    //         h,
    //         VariableDescription("tfel::math::View<tfel::math::tvector<N-1,real>>",
    //                             "Dt_nt", 1u, 0u));
    //     this->mb.addLocalVariable(
    //         h,
    //         VariableDescription("tfel::math::View<tfel::math::tvector<N-1,real>,"
    //                                "tfel::math::FixedSizeVectorIndexingPolicy<"
    //                                "unsigned short, N-1, N>>",
    //                                "Dt_tn", 1u, 0u));
    //     this->mb.addLocalVariable(
    //         h, VariableDescription(
    //                "tfel::math::View<tfel::math::tmatrix<N-1, N-1, real>,"
    //                "tfel::math::FixedSizeRowMajorMatrixIndexingPolicy<unsigned
    //                " "short, N-1, N-1, N>>", "Dt_tt", 1u, 0u));
  }

  std::string ImplicitCZMDSL::getDescription() {
    return "this domain specific language is dedicated "
           "to the implicit integration of cohesive zone models";
  }  // end of ImplicitCZMDSL::getDescription

  std::string ImplicitCZMDSL::getName() { return "ImplicitCZMDSL"; }

  BehaviourDSLDescription ImplicitCZMDSL::getBehaviourDSLDescription() const {
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
  }  // end of ImplicitCZMDSL::getBehaviourDSLDescription

  //   void ImplicitCZMDSL::writeBehaviourParserSpecificIncludes(
  //       std::ostream& os) const {
  //     ImplicitDSLBase::writeBehaviourParserSpecificIncludes(os);
  //     os << "#include\"TFEL/Math/tmatrix.hxx\"\n"
  //        << "#include\"TFEL/Math/tvector.hxx\"\n"
  //        << "#include\"TFEL/Math/Vector/tvectorIO.hxx\"\n"
  //        << "#include\"TFEL/Math/Matrix/tmatrixIO.hxx\"\n";
  //   }  // end of ImplicitCZMDSL::writeBehaviourParserSpecificIncludes
  //
  //   std::string ImplicitCZMDSL::getLocalVariablesInitializers(
  //       const Hypothesis) const {
  //     return "u_n(this->u(0)),\n"
  //            "du_n(this->du(0)),\n"
  //            "t_n(this->t(0)),\n"
  //            "u_t(tfel::math::map<tfel::math::tvector<N-1,real>,
  //            1>(this->u)),\n"
  //            "du_t(tfel::math::map<tfel::math::tvector<N-1,real>, "
  //            "1>(this->du)),\n"
  //            "t_t(tfel::math::map<tfel::math::tvector<N-1,real>,
  //            1>(this->t)),\n" "Dt_nn(this->Dt(0,0)),\n"
  //            "Dt_nt(this->Dt.template row_view<0, 1, N-1>()),\n"
  //            "Dt_tn(this->Dt.template column_view<0, 1, N-1>()),\n"
  //            "Dt_tt(this->Dt.template submatrix_view<1, 1, N-1, N-1>())";
  //   }  // end of ImplicitCZMDSL::getLocalVariablesInitializers

  ImplicitCZMDSL::~ImplicitCZMDSL() = default;

}  // end of namespace mfront
