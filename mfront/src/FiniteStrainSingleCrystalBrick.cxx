/*!
 * \file   FiniteStrainSingleCrystalBrick.cxx
 * \brief
 * \author Thomas Helfer
 * \date   04/10/2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <fstream>
#include <sstream>
#include <stdexcept>
#include "TFEL/Raise.hxx"
#include "TFEL/Utilities/Data.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"
#include "TFEL/Glossary/Glossary.hxx"
#include "TFEL/Glossary/GlossaryEntry.hxx"
#include "TFEL/System/System.hxx"
#include "MFront/DSLUtilities.hxx"
#include "MFront/MFrontHeader.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MFront/AbstractBehaviourDSL.hxx"
#include "MFront/LocalDataStructure.hxx"
#include "MFront/BehaviourDescription.hxx"
#include "MFront/ImplicitDSLBase.hxx"
#include "MFront/NonLinearSystemSolver.hxx"
#include "MFront/FiniteStrainSingleCrystalBrick.hxx"

namespace mfront {

  const char* const
      FiniteStrainSingleCrystalBrick::shiftedDeformationGradientOption =
          "shifted_elastic_deformation_gradient";

  const char* const
      FiniteStrainSingleCrystalBrick::shiftedDeformationGradientAttribute =
          "FiniteStrainSingleCrystalBrick::shifted_elastic_deformation_"
          "gradient";

  FiniteStrainSingleCrystalBrick::FiniteStrainSingleCrystalBrick(
      AbstractBehaviourDSL& dsl_,
      BehaviourDescription& mb_,
      const Parameters& p,
      const DataMap& d)
      : BehaviourBrickBase(dsl_, mb_) {
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(
          b,
          "FiniteStrainSingleCrystalBrick::FiniteStrainSingleCrystalBrick: " +
              m);
    };
    const auto h = ModellingHypothesis::TRIDIMENSIONAL;
    // basic checks
    if (this->bd.areModellingHypothesesDefined()) {
      const auto bmh = this->bd.getModellingHypotheses();
      throw_if(bmh.size() != 1u,
               "the only supported hypothesis is 'Tridimensional'");
      throw_if(*(bmh.begin()) != ModellingHypothesis::TRIDIMENSIONAL,
               "the only supported hypothesis is 'Tridimensional'");
    } else {
      this->bd.setModellingHypotheses({ModellingHypothesis::TRIDIMENSIONAL});
    }
    throw_if(this->bd.getBehaviourType() !=
                 BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR,
             "this BehaviourBrick is only usable for small strain behaviours");
    throw_if(
        this->bd.getIntegrationScheme() != BehaviourDescription::IMPLICITSCHEME,
        "this BehaviourBrick is only usable in implicit schemes");
    throw_if(!p.empty(), "no parameters allowed");
    // material symmetry
    if (this->bd.isSymmetryTypeDefined()) {
      throw_if(this->bd.getSymmetryType() != mfront::ORTHOTROPIC,
               "material must be declared orthotropic");
    } else {
      this->bd.setSymmetryType(mfront::ORTHOTROPIC);
    }
    throw_if(this->bd.getElasticSymmetryType() != mfront::ORTHOTROPIC,
             "elastic symmetry type must be orthortropic");
    // declaring the elastic strain as the first integration variable
    throw_if(!this->bd.getBehaviourData(h).getIntegrationVariables().empty(),
             "no integration variable shall be declared before declaring the "
             "'FiniteStrainSingleCrystal' brick");
    // options
    this->checkOptionsNames(
        d, {FiniteStrainSingleCrystalBrick::shiftedDeformationGradientOption},
        this->getName());
    if (d.count(
            FiniteStrainSingleCrystalBrick::shiftedDeformationGradientOption)) {
      const auto on = std::string(
          FiniteStrainSingleCrystalBrick::shiftedDeformationGradientOption);
      const auto an = std::string(
          FiniteStrainSingleCrystalBrick::shiftedDeformationGradientAttribute);
      const auto& b = d.at(on);
      throw_if(!b.is<bool>(), "invalid type for option '" + on + "'");
      throw_if(this->bd.hasAttribute(an),
               "attribute '" + an + "' already used");
      this->bd.setAttribute(an, b.get<bool>(), false);
    }
    // elastic strain
    VariableDescription eel("StrainStensor", "eel", 1u, 0u);
    eel.description = "elastic strain";
    this->bd.addIntegrationVariable(h, eel, BehaviourData::UNREGISTRED);
    this->bd.setGlossaryName(h, "eel", tfel::glossary::Glossary::ElasticStrain);
    // declaring the plastic slip
    auto add_plastic_slips = [this, throw_if, h] {
      throw_if(
          this->bd.getBehaviourData(h).getIntegrationVariables().size() != 1u,
          "no integration variable shall be declared before declaring the "
          "'FiniteStrainSingleCrystal' brick");
      const auto& ss = this->bd.getSlipSystems();
      VariableDescription g("strain", "g", ss.getNumberOfSlipSystems(), 0u);
      g.description = "plastic slip";
      this->bd.addStateVariable(h, g, BehaviourData::UNREGISTRED);
      this->bd.setEntryName(h, "g", "PlasticSlip");
    };
    if ((!this->bd.hasCrystalStructure()) ||
        (!this->bd.areSlipSystemsDefined())) {
      // no slip systems defined, yet
      auto& idsl = dynamic_cast<ImplicitDSLBase&>(this->dsl);
      idsl.addHook("@SlipSystem", add_plastic_slips);
      idsl.addHook("@GlidingSystem", add_plastic_slips);
      idsl.addHook("@SlidingSystem", add_plastic_slips);
      idsl.addHook("@SlipSystems", add_plastic_slips);
      idsl.addHook("@GlidingSystems", add_plastic_slips);
      idsl.addHook("@SlidingSystems", add_plastic_slips);
    } else {
      add_plastic_slips();
    }
    // declaring the elastic part of the deformation gradient
    VariableDescription Fe("DeformationGradientTensor", "Fe", 1u, 0u);
    if (this->bd.getAttribute<bool>(
            FiniteStrainSingleCrystalBrick::shiftedDeformationGradientAttribute,
            false)) {
      Fe.description = "shifted elastic part of the deformation gradient";
    } else {
      Fe.description = "elastic part of the deformation gradient";
    }
    this->bd.addAuxiliaryStateVariable(h, Fe, BehaviourData::UNREGISTRED);
    if (this->bd.getAttribute<bool>(
            FiniteStrainSingleCrystalBrick::shiftedDeformationGradientAttribute,
            false)) {
      this->bd.setEntryName(h, "Fe",
                            "ShiftedElasticPartOfTheDeformationGradient");
    } else {
      this->bd.setEntryName(h, "Fe", "ElasticPartOfTheDeformationGradient");
    }
    // additional includes
    this->bd.appendToIncludes("#include\"TFEL/Math/General/CubicRoots.hxx\"");
    // set that the tangent operator is computed
    this->bd.setAttribute(h, BehaviourData::hasConsistentTangentOperator, true);
    // reserve some specific variables
    this->bd.reserveName(h, "ss");
    this->bd.reserveName(h, "fsscb_data");
    this->bd.reserveName(h, "fsscb_tprd");
    this->bd.reserveName(h, "fsscb_dfeel_dinv_dFp");
    this->bd.reserveName(h, "fsscb_dC_dFe");
    this->bd.reserveName(h, "fsscb_dS_dFe");
    this->bd.reserveName(h, "fsscb_dtau_dFe");
    this->bd.reserveName(h, "fsscb_dFe_dDF_tot");
    this->bd.reserveName(h, "fsscb_dfeel_dDF");
    this->bd.reserveName(h, "fsscb_Je");
    this->bd.reserveName(h, "fsscb_Jg");
    this->bd.reserveName(h, "fsscb_dinv_Fp_dDF");
    this->bd.reserveName(h, "fsscb_dFe_dDF");
  }  // end of FiniteStrainSingleCrystalBrick::FiniteStrainSingleCrystalBrick

  void FiniteStrainSingleCrystalBrick::completeVariableDeclaration() const {
    using tfel::glossary::Glossary;
    const auto h = ModellingHypothesis::TRIDIMENSIONAL;
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(b,
                     "FiniteStrainSingleCrystalBrick:"
                     ":completeVariableDeclaration: " +
                         m);
    };
    if (getVerboseMode() >= VERBOSE_DEBUG) {
      getLogStream() << "FiniteStrainSingleCrystalBrick::"
                        "completeVariableDeclaration: begin\n";
    }
    // various checks
    throw_if(!this->bd.hasCrystalStructure(), "no crystal structure defined");
    throw_if(!this->bd.areSlipSystemsDefined(), "no slip systems defined");
    // defining the stiffness tensor, if necessary
    if ((!this->bd.getAttribute(BehaviourDescription::requiresStiffnessTensor,
                                false)) &&
        (!this->bd.getAttribute(BehaviourDescription::computesStiffnessTensor,
                                false))) {
      this->bd.setAttribute(BehaviourDescription::requiresStiffnessTensor, true,
                            false);
    }
    LocalDataStructure d;
    d.name = "fsscb_data";
    // local data
    d.addVariable(h, {"DeformationGradientTensor", "dF"});
    d.addVariable(h, {"DeformationGradientTensor", "Fe_tr"});
    d.addVariable(h, {"DeformationGradientTensor", "Fe0"});
    d.addVariable(h, {"StressStensor", "S"});
    d.addVariable(h, {"Tensor", "inv_dFp"});
    d.addVariable(h, {"real", "J_inv_dFp"});
    d.addVariable(h, {"StrainStensor", "tmp"});
    this->bd.addLocalDataStructure(d, BehaviourData::ALREADYREGISTRED);
    if (getVerboseMode() >= VERBOSE_DEBUG) {
      getLogStream() << "FiniteStrainSingleCrystalBrick::"
                        "completeVariableDeclaration: end\n";
    }
  }

  void FiniteStrainSingleCrystalBrick::endTreatment() const {
    const auto h = ModellingHypothesis::TRIDIMENSIONAL;
    const auto cn = this->bd.getClassName() + "SlipSystems<real>";
    // local data values initialisation
    CodeBlock init;
    if (this->bd.getAttribute<bool>(
            FiniteStrainSingleCrystalBrick::shiftedDeformationGradientAttribute,
            false)) {
      init.code = "this->Fe += DeformationGradientTensor::Id();\n";
    }
    init.code +=
        "this->fsscb_data.dF    = (this->F1)*invert(this->F0);\n"
        "this->fsscb_data.Fe0   = this->Fe;\n"
        "this->fsscb_data.Fe_tr = "
        "(this->fsscb_data.dF)*(this->fsscb_data.Fe0);\n"
        "this->eel = computeGreenLagrangeTensor(this->Fe);\n";
    init.members = {"F0", "F1", "Fe", "eel"};
    this->bd.setCode(h, BehaviourData::BeforeInitializeLocalVariables, init,
                     BehaviourData::CREATEORAPPEND, BehaviourData::AT_END);
    CodeBlock integrator;
    integrator.code =
        "const auto& ss = " + cn +
        "::getSlidingSystems();\n"
        "this->fsscb_data.S = (this->D)*(this->eel+this->deel);\n"
        "this->fsscb_data.tmp = StrainStensor::Id() + "
        "2*(this->eel+this->deel);\n"
        "// Mandel stress tensor\n"
        "const auto M = eval(this->fsscb_data.tmp*(this->fsscb_data.S));\n";
    const auto& idsl = dynamic_cast<const ImplicitDSLBase&>(this->dsl);
    if ((idsl.getSolver().usesJacobian()) &&
        (!idsl.getSolver().requiresNumericalJacobian())) {
      integrator.code +=
          "// Mandel stress tensor derivative\n"
          "const auto dM_ddeel = "
          "eval(2*st2tot2<N,real>::tpld(this->fsscb_data.S)+\n"
          "			    "
          "st2tot2<N,real>::tprd(this->fsscb_data.tmp,this->D));\n";
    }
    integrator.code +=
        "this->fsscb_data.inv_dFp = Tensor::Id();\n"
        "for(unsigned short i=0;i!=" +
        cn +
        "::Nss;++i){\n"
        "  this->fsscb_data.inv_dFp -= (this->dg[i])*ss.mu[i];\n"
        "}\n"
        "this->fsscb_data.J_inv_dFp = det(this->fsscb_data.inv_dFp);\n"
        "this->fsscb_data.inv_dFp /= "
        "CubicRoots::cbrt(this->fsscb_data.J_inv_dFp);\n"
        "this->Fe = (this->fsscb_data.Fe_tr)*(this->fsscb_data.inv_dFp);\n"
        "feel = this->eel+this->deel-computeGreenLagrangeTensor(this->Fe);\n";
    if ((idsl.getSolver().usesJacobian()) &&
        (!idsl.getSolver().requiresNumericalJacobian())) {
      integrator.code +=
          "const auto fsscb_tprd = "
          "t2tot2<N,real>::tprd(this->fsscb_data.Fe_tr);\n"
          "const auto fsscb_dfeel_dinv_dFp = "
          "t2tost2<N,real>::dCdF(this->Fe)*fsscb_tprd;\n"
          "for(unsigned short i=0;i!=" +
          cn +
          "::Nss;++i){\n"
          "  dfeel_ddg(i) = (fsscb_dfeel_dinv_dFp)*ss.mu[i]/2;\n"
          "}\n";
    }
    integrator.members = {"eel", "Fe", "D"};
    this->bd.setCode(h, BehaviourData::Integrator, integrator,
                     BehaviourData::CREATEORAPPEND,
                     BehaviourData::AT_BEGINNING);
    CodeBlock fs;
    fs.code =
        "const auto& ss = " + cn +
        "::getSlidingSystems();\n"
        "this->fsscb_data.inv_dFp = Tensor::Id();\n"
        "for(unsigned short i=0;i!=" +
        cn +
        "::Nss;++i){\n"
        "  this->fsscb_data.inv_dFp -= dg[i]*ss.mu[i];\n"
        "}\n"
        "this->fsscb_data.J_inv_dFp = det(this->fsscb_data.inv_dFp);\n"
        "this->fsscb_data.inv_dFp /= "
        "CubicRoots::cbrt(this->fsscb_data.J_inv_dFp);\n"
        "this->Fe = (this->fsscb_data.Fe_tr)*(this->fsscb_data.inv_dFp);\n"
        "this->fsscb_data.S = (this->D)*(this->eel);\n"
        "this->sig = "
        "convertSecondPiolaKirchhoffStressToCauchyStress(this->fsscb_data.S,"
        "this->Fe);\n";
    if (this->bd.getAttribute<bool>(
            FiniteStrainSingleCrystalBrick::shiftedDeformationGradientAttribute,
            false)) {
      fs.code += "this->Fe -= DeformationGradientTensor::Id();\n";
    }
    fs.members = {"sig", "Fe", "D"};
    this->bd.setCode(h, BehaviourData::ComputeFinalStress, fs,
                     BehaviourData::CREATE, BehaviourData::AT_BEGINNING);
    // tangent operator
    CodeBlock to;
    to.code =
        "static_cast<void>(smt);\n"
        "const auto& ss = " +
        cn + "::getSlidingSystems();\n";
    if (this->bd.getAttribute<bool>(
            FiniteStrainSingleCrystalBrick::shiftedDeformationGradientAttribute,
            false)) {
      to.code +=
          "const auto fsscb_dC_dFe = "
          "t2tost2<N,real>::dCdF(this->Fe+DeformationGradientTensor::Id());\n";
    } else {
      to.code += "const auto fsscb_dC_dFe = t2tost2<N,real>::dCdF(this->Fe);\n";
    }
    to.code += "const auto fsscb_dS_dFe = eval((this->D)*fsscb_dC_dFe/2);\n";
    if (this->bd.getAttribute<bool>(
            FiniteStrainSingleCrystalBrick::shiftedDeformationGradientAttribute,
            false)) {
      to.code +=
          "const auto fsscb_dtau_dFe = "
          "computePushForwardDerivative(fsscb_dS_dFe,this->fsscb_data.S,this->"
          "Fe+DeformationGradientTensor::Id());\n";
    } else {
      to.code +=
          "const auto fsscb_dtau_dFe = "
          "computePushForwardDerivative(fsscb_dS_dFe,this->fsscb_data.S,this->"
          "Fe);\n";
    }
    to.code +=
        "const auto fsscb_dFe_dDF_tot = "
        "t2tot2<N,real>::tpld(this->fsscb_data.inv_dFp,"
        "                     t2tot2<N,real>::tpld(this->fsscb_data.Fe0));\n"
        "const auto fsscb_dfeel_dDF = "
        "eval(-(fsscb_dC_dFe)*(fsscb_dFe_dDF_tot)/2);\n"
        "st2tost2<N,real> fsscb_Je;\n"
        "tvector<" +
        cn +
        "::Nss,Stensor> fsscb_Jg;\n"
        "getPartialJacobianInvert(fsscb_Je,fsscb_Jg);\n"
        "t2tot2<N,real> fsscb_dinv_Fp_dDF = "
        "(ss.mu[0])^(fsscb_Jg[0]|fsscb_dfeel_dDF);\n"
        "for(unsigned short i=1;i!=" +
        cn +
        "::Nss;++i){\n"
        "  fsscb_dinv_Fp_dDF += (ss.mu[i])^(fsscb_Jg[i]|fsscb_dfeel_dDF);\n"
        "}\n"
        "const auto fsscb_dFe_dDF=\n"
        "  "
        "fsscb_dFe_dDF_tot+t2tot2<N,real>::tprd(this->fsscb_data.Fe_tr,fsscb_"
        "dinv_Fp_dDF);\n"
        "Dt = fsscb_dtau_dFe*fsscb_dFe_dDF;\n";
    to.members = {"Fe", "D"};
    const auto ton =
        std::string(BehaviourData::ComputeTangentOperator) + "-DTAU_DDF";
    this->bd.setCode(h, ton, to, BehaviourData::CREATE,
                     BehaviourData::AT_BEGINNING);
  }  // end of FiniteStrainSingleCrystalBrick::endTreatment

  std::string FiniteStrainSingleCrystalBrick::getName() const {
    return "FiniteStrainSingleCrystal";
  }

  std::vector<tfel::material::ModellingHypothesis::Hypothesis>
  FiniteStrainSingleCrystalBrick::getSupportedModellingHypotheses() const {
    return {ModellingHypothesis::TRIDIMENSIONAL};
  }  // end of FiniteStrainSingleCrystalBrick::getSupportedModellingHypothesis

  FiniteStrainSingleCrystalBrick::~FiniteStrainSingleCrystalBrick() = default;

}  // end of namespace mfront
