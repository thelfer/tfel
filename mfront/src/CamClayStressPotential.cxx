/*!
 * \file  mfront/src/CamClayStressPotential.cxx
 * \brief
 * \author Thomas Helfer
 * \date   20/11/2014
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 * <!-- Local IspellDict: english -->
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
#include "MFront/BehaviourBrick/BrickUtilities.hxx"
#include "MFront/BehaviourBrick/OptionDescription.hxx"
#include "MFront/BehaviourBrick/CamClayStressPotential.hxx"

namespace mfront::bbrick {

  CamClayStressPotential::CamClayStressPotential() = default;

  std::string CamClayStressPotential::getName() const { return "CamClay"; }

  std::vector<CamClayStressPotential::Hypothesis>
  CamClayStressPotential::getSupportedModellingHypotheses(
      const BehaviourDescription&, const AbstractBehaviourDSL& dsl) const {
    const auto dmh = dsl.getDefaultModellingHypotheses();
    std::vector<Hypothesis> mh(dmh.begin(), dmh.end());
    return mh;
  }  // end of getSupportedModellingHypotheses

  void CamClayStressPotential::initialize(BehaviourDescription& bd,
                                          AbstractBehaviourDSL& dsl,
                                          const DataMap& d) {
    StressPotentialBase::initialize(bd, dsl, d);
  }  // end CamClayStressPotential

  void CamClayStressPotential::completeVariableDeclaration(
      BehaviourDescription& bd, const AbstractBehaviourDSL& dsl) const {
    constexpr auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    if (getVerboseMode() >= VERBOSE_DEBUG) {
      getLogStream()
          << "CamClayStressPotential::completeVariableDeclaration: begin\n";
    }
    // elastic strain
    StressPotentialBase::declareElasticStrainIfRequired(bd);
    //
    if (getVerboseMode() >= VERBOSE_DEBUG) {
      getLogStream()
          << "CamClayStressPotential::completeVariableDeclaration: end\n";
    }
  }

  void CamClayStressPotential::endTreatment(BehaviourDescription& bd,
                                            const AbstractBehaviourDSL&) const {
    // modelling hypotheses supported by the behaviour
    constexpr auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    const auto bmh = bd.getModellingHypotheses();
    if (getVerboseMode() >= VERBOSE_DEBUG) {
      getLogStream() << "CamClayStressPotentialBase::endTreatment: begin\n";
    }
    // implicit equation associated with the elastic strain
    CodeBlock integrator;
    auto broken_test = getBrokenTest(bd, false);
    if (!broken_test.empty()) {
      integrator.code += "if(" + broken_test + "){\n";
      integrator.code += "  this->deel = -(this->eel);\n";
      integrator.code += "  return true;\n";
      integrator.code += "}\n";
    }
    integrator.code += "feel -= this->deto;\n";
    bd.setCode(uh, BehaviourData::Integrator, integrator,
               BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
    //     // modelling hypotheses supported by the behaviour
    //     // plane stress support
    //     if (this->pss) {
    //       const bool agps =
    //           bmh.count(
    //               ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS)
    //               != 0;
    //       const bool ps = bmh.count(ModellingHypothesis::PLANESTRESS) != 0;
    //       if (ps) {
    //         this->addPlaneStressSupport(bd, dsl);
    //       }
    //       if (agps) {
    //         this->addAxisymmetricalGeneralisedPlaneStressSupport(bd, dsl);
    //       }
    //     }
    //     // declaring the computeElasticPrediction member
    //     this->declareComputeElasticPredictionMethod(bd);
    //     // prediction operator
    //     // look if the broken variable has defined
    //     if (this->gpo) {
    //       this->addBrokenPredictionOperatorSupport(bd);
    //       this->addGenericPredictionOperatorSupport(bd);
    //     }
    //     // tangent operator
    //     if (this->gto) {
    //       this->addBrokenTangentOperatorSupport(bd);
    //       this->addGenericTangentOperatorSupport(bd, dsl);
    //     }
    if (getVerboseMode() >= VERBOSE_DEBUG) {
      getLogStream() << "CamClayStressPotentialBase::endTreatment: end\n";
    }
  }  // end of endTreatment

  std::vector<
      std::tuple<std::string, std::string, mfront::SupportedTypes::TypeFlag>>
  CamClayStressPotential::getStressDerivatives(
      const BehaviourDescription&) const {
    auto r = std::vector<std::tuple<std::string, std::string,
                                    mfront::SupportedTypes::TypeFlag>>{};
    r.push_back(std::make_tuple("theta * dsig_deel", std::string("eel"),
                                SupportedTypes::STENSOR));
    return r;
  }  // end of getStressDerivatives

  std::string CamClayStressPotential::generateImplicitEquationDerivatives(
      const BehaviourDescription& bd,
      const std::string& t,
      const std::string& v,
      const std::string& dfv_ds,
      const bool b) const {
    auto c = std::string{};
    const auto vf = SupportedTypes::getTypeFlag(t);
    c = "df" + v + "_ddeel += ";
    if (vf == SupportedTypes::SCALAR) {
      c += "(this->theta) * ((" + dfv_ds + ") | (this->dsig_deel));\n";
    } else if (vf == SupportedTypes::STENSOR) {
      c += "(this->theta) * (" + dfv_ds + ") * (this->dsig_deel);\n";
    } else {
      tfel::raise(
          "CamClayStressPotential::generateImplicitEquationDerivatives: "
          "unsupported variable type");
    }
    return c;
}  // end of generateImplicitEquationDerivatives

  void CamClayStressPotential::computeElasticPrediction(
      BehaviourDescription&) const {}  // end of computeElasticPrediction

  std::string CamClayStressPotential::getStressNormalisationFactor(
      const BehaviourDescription&) const {
  }  // end of getStressNormalisationFactor

  std::string CamClayStressPotential::getEquivalentStressLowerBound(
      const BehaviourDescription&) const {

  }  // end of getEquivalentStressLowerBound

  std::vector<OptionDescription> CamClayStressPotential::getOptions(
      const BehaviourDescription& bd, const bool b) const {
    auto opts = std::vector<OptionDescription>{};//StressPotentialBase::getOptions(bd, b);
    return opts;
  }  // end of getOptions

  //   void CamClayStressPotential::addGenericTangentOperatorSupport(
  //       BehaviourDescription& bd, const AbstractBehaviourDSL& dsl) const {
  //     auto throw_if = [](const bool b, const std::string& m) {
  //       tfel::raise_if(
  //           b, "CamClayStressPotential::addGenericTangentOperatorSupport: " +
  //           m);
  //     };
  //     //
  //     const auto& idsl = dynamic_cast<const ImplicitDSLBase&>(dsl);
  //     bd.checkVariablePosition("eel", "IntegrationVariable", 0u);
  //     CodeBlock to;
  //     if (idsl.getSolver().usesJacobian()) {
  //       to.attributes["requires_jacobian_decomposition"] = true;
  //       to.attributes["uses_get_partial_jacobian_invert"] = true;
  //     }
  //     // modelling hypotheses supported by the behaviour
  //             "} else if (smt==CONSISTENTTANGENTOPERATOR){\n"
  //             "  Stensor4 Je;\n"
  //             "  getPartialJacobianInvert(Je);\n"
  //             "  this->Dt = dsig_deel" +
  //             D + " * Je;\n";
  //   }

  //   void CamClayStressPotential::addGenericPredictionOperatorSupport(
  //       BehaviourDescription& bd) const {
  //     auto throw_if = [](const bool b, const std::string& m) {
  //       tfel::raise_if(
  //           b, "CamClayStressPotential::addGenericPredictionOperatorSupport:
  //           " + m);
  //     };
  //     CodeBlock to;
  //     // modelling hypotheses supported by the behaviour
  //     const auto bmh = bd.getModellingHypotheses();
  //     if ((bd.getAttribute(BehaviourDescription::requiresStiffnessTensor,
  //                          false)) ||
  //         (bd.getAttribute(BehaviourDescription::computesStiffnessTensor,
  //                          false))) {
  //       const bool agps =
  //           bmh.count(
  //               ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS) !=
  //               0;
  //       const bool ps = bmh.count(ModellingHypothesis::PLANESTRESS) != 0;
  //       if (agps || ps) {
  //         if (bd.getAttribute(BehaviourDescription::requiresStiffnessTensor,
  //                             false)) {
  //           if (!bd.hasAttribute(
  //                   BehaviourDescription::requiresUnAlteredStiffnessTensor))
  //                   {
  //             bd.setAttribute(
  //                 BehaviourDescription::requiresUnAlteredStiffnessTensor,
  //                 true, false);
  //           }
  //           throw_if(!bd.getAttribute<bool>(
  //                        BehaviourDescription::requiresUnAlteredStiffnessTensor),
  //                    "genertic tangent operator support for "
  //                    "plane stress hypotheses requires the use of an "
  //                    "unaltered "
  //                    "stiffness tensor");
  //         }
  //       }
  //       const std::string D =
  //           bd.getAttribute(BehaviourDescription::requiresStiffnessTensor,
  //           false)
  //               ? "this->D"
  //               : "this->D_tdt";
  //       to.code =
  //           "if((smt==ELASTIC)||(smt==SECANTOPERATOR)){\n"
  //           "  this->Dt = " +
  //           D +
  //           ";\n"
  //           "} else {\n"
  //           "  return FAILURE;\n"
  //           "}";
  //     } else {
  //       if (bd.getElasticSymmetryType() == mfront::ISOTROPIC) {
  //         auto b = bd.getAttribute(
  //             "CamClayStressPotentialBase::UseLocalLameCoeficients", false);
  //         const std::string lambda =
  //             b ? "this->sebdata.lambda" : "this->lambda_tdt";
  //         const std::string mu = b ? "this->sebdata.mu" : "this->mu_tdt";
  //         to.code =
  //             "if((smt==ELASTIC)||(smt==SECANTOPERATOR)){\n"
  //             "  "
  //             "computeAlteredElasticStiffness<hypothesis, stress>::exe(Dt," +
  //             lambda + "," + mu +
  //             ");\n"
  //             "} else {\n"
  //             "  return FAILURE;\n"
  //             "}";
  //       } else if (bd.getElasticSymmetryType() == mfront::ORTHOTROPIC) {
  //         throw_if(!bd.getAttribute<bool>(
  //                      BehaviourDescription::computesStiffnessTensor, false),
  //                  "orthotropic behaviour shall require the stiffness
  //                  tensor");
  //         to.code =
  //             "if((smt==ELASTIC)||(smt==SECANTOPERATOR)){\n"
  //             "  this->Dt = this->D_tdt;\n"
  //             "} else {\n"
  //             "  return FAILURE;\n"
  //             "}";
  //       } else {
  //         throw_if(true, "unsupported elastic symmetry type");
  //       }
  //     }
  //     bd.setAttribute(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
  //                     BehaviourData::hasPredictionOperator, true, true);
  //     bd.setCode(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
  //                BehaviourData::ComputePredictionOperator, to,
  //                BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
  //   }

  CamClayStressPotential::~CamClayStressPotential() = default;

}  // end of namespace mfront::bbrick
