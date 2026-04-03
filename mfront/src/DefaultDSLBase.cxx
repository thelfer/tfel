/*!
 * \file   mfront/src/DefaultDSLBase.cxx
 * \brief
 * \author Thomas Helfer
 * \date   22 jullet 2013
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
#include "TFEL/Raise.hxx"
#include "TFEL/Material/FiniteStrainBehaviourTangentOperator.hxx"
#include "MFront/MFrontWarningMode.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MFront/AbstractBehaviourInterface.hxx"
#include "MFront/BehaviourInterfaceFactory.hxx"
#include "MFront/DefaultCodeGeneratorBase.hxx"
#include "MFront/DefaultDSLBase.hxx"

namespace mfront {

  DefaultDSLBase::DefaultDSLBase(const DSLOptions& opts)
      : BehaviourDSLBase<DefaultDSLBase>(opts) {
    this->mb.setIntegrationScheme(BehaviourDescription::USERDEFINEDSCHEME);
    this->registerNewCallBack("@ProvidesTangentOperator",
                              &DefaultDSLBase::treatProvidesTangentOperator);
    this->registerNewCallBack(
        "@ProvidesSymmetricTangentOperator",
        &DefaultDSLBase::treatProvidesSymmetricTangentOperator);
    this->registerNewCallBack("@MaterialLaw",
                              &DefaultDSLBase::treatMaterialLaw);
    this->registerNewCallBack("@TangentOperator",
                              &DefaultDSLBase::treatTangentOperator);
    this->registerNewCallBack("@IsTangentOperatorSymmetric",
                              &DefaultDSLBase::treatIsTangentOperatorSymmetric);
    this->registerNewCallBack("@ComputeStiffnessTensor",
                              &DefaultDSLBase::treatComputeStiffnessTensor);
  }

  std::unique_ptr<AbstractBehaviourCodeGenerator>
  DefaultDSLBase::getCodeGenerator() const {
    return std::make_unique<DefaultCodeGeneratorBase>(this->fd, this->mb,
                                                      this->interfaces);
  }  // end of getCodeGenerator

  std::string DefaultDSLBase::getCodeBlockTemplate(
      const std::string& c, const MFrontTemplateGenerationOptions&) const {
    if (c == BehaviourData::ComputePredictionOperator) {
      return "@PredictionOperator{}\n";
    } else if (c == BehaviourData::Integrator) {
      return "@Integrator{}\n";
    } else if (c == BehaviourData::ComputeTangentOperator) {
      return "@TangentOperator{}\n";
    }
    return "";
  }  // end of DefaultDSLBase::getCodeBlockTemplate

  void DefaultDSLBase::treatProvidesTangentOperator() {
    std::set<Hypothesis> h;
    this->readHypothesesList(h);
    this->readSpecifiedToken(
        "BehaviourDSLBaseCommon::"
        "treatProvidesTangentOperator",
        ";");
    for (const auto& elem : h) {
      if (this->mb.hasAttribute(elem,
                                BehaviourData::hasConsistentTangentOperator)) {
        this->throwRuntimeError("DefaultDSLBase::treatProvidesTangentOperator",
                                "one of the key @ProvidesTangentOperator "
                                "or @ProvidesSymmetricTangentOperator "
                                "has already been called");
      }
      this->mb.setAttribute(elem, BehaviourData::hasConsistentTangentOperator,
                            true);
    }
  }  // end of DefaultDSLBase::treatProvidesTangentOperator

  void DefaultDSLBase::treatProvidesSymmetricTangentOperator() {
    std::set<Hypothesis> h;
    this->readHypothesesList(h);
    this->checkNotEndOfFile(
        "BehaviourDSLBaseCommon::"
        "treatProvidesSymmetricTangentOperator");
    this->readSpecifiedToken(
        "BehaviourDSLBaseCommon::"
        "treatProvidesSymmetricTangentOperator",
        ";");
    for (const auto& elem : h) {
      if (this->mb.hasAttribute(elem,
                                BehaviourData::hasConsistentTangentOperator)) {
        this->throwRuntimeError(
            "DefaultDSLBase::treatProvidesSymmetricTangentOperator",
            "one of the key @ProvidesTangentOperator "
            "or @ProvidesSymmetricTangentOperator "
            "has already been called");
      }
      this->mb.setAttribute(elem, BehaviourData::hasConsistentTangentOperator,
                            true);
      this->mb.setAttribute(
          elem, BehaviourData::isConsistentTangentOperatorSymmetric, true);
    }
  }  // end of DefaultDSLBase::treatProvidesSymmetricTangentOperator

  void DefaultDSLBase::completeVariableDeclaration() {
    BehaviourDSLCommon::completeVariableDeclaration();
    if (this->mb.getAttribute<bool>(
            BehaviourDescription::computesStiffnessTensor, false)) {
      const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      auto D = VariableDescription("StiffnessTensor", "D", 1u, 0u);
      D.description =
          "stiffness tensor computed from elastic "
          "material properties";
      this->mb.addLocalVariable(h, D, BehaviourData::ALREADYREGISTRED);
    }
  }  // end of DefaultDSLBase::completeVariableDeclaration

  void DefaultDSLBase::getSymbols(std::map<std::string, std::string>& symbols,
                                  const Hypothesis h,
                                  const std::string& n) {
    BehaviourDSLCommon::getSymbols(symbols, h, n);
    const auto& d = this->mb.getBehaviourData(h);
    getIncrementSymbols(symbols, d.getIntegrationVariables());
    for (const auto& mv : this->mb.getMainVariables()) {
      if (Gradient::isIncrementKnown(mv.first)) {
        getIncrementSymbol(symbols, mv.first);
      } else {
        mfront::addSymbol(symbols, displayName(mv.first) + "\u2080",
                          mv.first.name + "0");
        mfront::addSymbol(symbols, displayName(mv.first) + "\u2081",
                          mv.first.name + "1");
      }
    }
    mfront::getIncrementSymbols(symbols, d.getExternalStateVariables());
    mfront::addSymbol(symbols, "\u0394t", "dt");
  }  // end of DefaultDSLBase::getSymbols

  void DefaultDSLBase::checkIntegratorBlock(const Hypothesis h) const {
    const auto& integrator =
        this->mb.getCodeBlock(h, BehaviourData::Integrator);
    if (isSafe(integrator)) {
      return;
    }
    auto getUsedTangentOperatorBlocks = [this, &integrator] {
      auto used_tblocks =
          std::vector<std::pair<VariableDescription, VariableDescription>>{};
      const auto& tblocks = this->mb.getTangentOperatorBlocks();
      for (const auto& tblock : tblocks) {
        const auto bn = this->mb.getTangentOperatorBlockName(tblock);
        if (integrator.members.contains(bn)) {
          used_tblocks.push_back(tblock);
        }
      }
      return used_tblocks;
    };
    const auto cname = std::string{BehaviourData::Integrator};
    if (this->mb.getBehaviourType() ==
        BehaviourDescription::COHESIVEZONEMODEL) {
      const auto uses_du = integrator.members.contains("du");
      const auto uses_du_n = integrator.members.contains("du_n");
      const auto uses_du_t = integrator.members.contains("du_t");
      if ((uses_du) || (uses_du_n) || (uses_du_t)) {
        if ((uses_du) && (uses_du_n)) {
          reportWarning(
              "the increment of the opening displacement 'du' and its normal "
              "component 'du_n' are both used in the '" +
              cname + "' code block. This is unexpected.");
        }
        if ((uses_du) && (uses_du_t)) {
          reportWarning(
              "the increment of the opening displacement 'du' and its "
              "tangential component 'du_t' are both used in the '" +
              cname + "' code block. This is unexpected.");
        }
        if ((uses_du_n) && (!uses_du_t)) {
          reportWarning(
              "the increment of the normal opening displacement 'du_n' is used "
              "in the '" +
              cname +
              "' code block and not the tangential part 'du_t'. This is "
              "unexpected.");
        }
        if ((uses_du_t) && (!uses_du_n)) {
          reportWarning(
              "the increment of the tangential opening displacement 'du_t' is "
              "used in the '" +
              cname +
              "' code block and not the normal part 'du_n'. This is "
              "unexpected.");
        }
      } else {
        reportWarning(
            "neither the increment of the opening displacement 'du' nor its "
            "normal or tangential components ('du_n', 'du_t) is used in the '" +
            cname + "' code block");
      }
      const auto uses_t = integrator.members.contains("t");
      const auto uses_t_n = integrator.members.contains("t_n");
      const auto uses_t_t = integrator.members.contains("t_t");
      if ((uses_t) || (uses_t_n) || (uses_t_t)) {
        if ((uses_t) && (uses_t_n)) {
          reportWarning(
              "the increment of the cohesive force 't' and its normal "
              "component 't_n' are both used in the '" +
              cname + "' code block. This is unexpected.");
        }
        if ((uses_t) && (uses_t_t)) {
          reportWarning(
              "the increment of the cohesive force 't' and its "
              "tangential component 't_t' are both used in the '" +
              cname + "' code block. This is unexpected.");
        }
        if ((uses_t_n) && (!uses_t_t)) {
          reportWarning(
              "the increment of the normal cohesive force 't_n' is used "
              "in the '" +
              cname +
              "' code block and not the tangential part 't_t'. This is "
              "unexpected.");
        }
        if ((uses_t_t) && (!uses_t_n)) {
          reportWarning(
              "the increment of the tangential cohesive force 't_t' is "
              "used in the '" +
              cname +
              "' code block and not the normal part 't_n'. This is "
              "unexpected.");
        }
      } else {
        reportWarning(
            "neither the increment of the opening displacement 'u' nor its "
            "normal or tangential components ('u_n', 'u_t) is used in the '" +
            cname + "' code block");
      }
    } else {
      for (const auto& [g, thf] : this->mb.getMainVariables()) {
        if (Gradient::isIncrementKnown(g)) {
          if (!integrator.members.contains("d" + g.name)) {
            reportWarning("the increment of gradient '" + g.name +
                          "' is not used in the '" + cname + "' code block");
          }
        } else {
          if (!integrator.members.contains(g.name + "1")) {
            reportWarning("the value of gradient '" + g.name +
                          "' at the end of the time step is not used in the '" +
                          cname + "' code block");
          }
        }
        if (!integrator.members.contains(thf.name)) {
          reportWarning("thermodynamic force '" + thf.name +
                        "' is not used in the '" + cname + "' code block");
        }
      }
    }
    if (!this->mb.getTangentOperatorBlocks().empty()) {
      if (this->mb.getAttribute(h, BehaviourData::hasConsistentTangentOperator,
                                false)) {
        auto checkThatNoTangentOperatorBlockIsUsed =
            [this, &integrator, &cname, &getUsedTangentOperatorBlocks] {
              if (integrator.members.contains("Dt")) {
                reportWarning(
                    "the variable 'Dt' is used in the '" + cname +
                    "' code block, which is unexpected as at least one "
                    "TangentOperator block is defined.");
              }
              const auto used_tblocks = getUsedTangentOperatorBlocks();
              if (!used_tblocks.empty()) {
                reportWarning(
                    "the following tangent operator blocks are used in the '" +
                    cname + "' code block: " +
                    makeTangentOperatorBlocksList(this->mb, used_tblocks) +
                    ". This is unexpected as a at least one "
                    "TangentOperator block is defined.");
              }
            };
        if (this->mb.getBehaviourType() ==
            BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
          const auto has_tangent_operator_block = [this, h] {
            for (const auto& t : tfel::material::
                     getFiniteStrainBehaviourTangentOperatorFlags()) {
              const auto cto_name =
                  std::string(BehaviourData::ComputeTangentOperator) + '-' +
                  convertFiniteStrainBehaviourTangentOperatorFlagToString(t);
              if (this->mb.hasCode(h, cto_name)) {
                return true;
              }
            }
            return false;
          }();
          if (!has_tangent_operator_block) {
            this->checkTangentOperatorBlock(cname, integrator);
          } else {
            checkThatNoTangentOperatorBlockIsUsed();
          }
        } else {
          if (!this->mb.hasCode(h, BehaviourData::ComputeTangentOperator)) {
            this->checkTangentOperatorBlock(cname, integrator);
          } else {
            checkThatNoTangentOperatorBlockIsUsed();
          }
        }
      } else {
        const auto uses_Dt = integrator.members.contains("Dt");
        if (uses_Dt) {
          reportWarning(
              "the consistent tangent operator is used in the '" + cname +
              "' code block and the behaviour does not declare that "
              "it computes it. See the @ProvidesSymmetricTangentOperator or "
              "the @ProvidesTangentOperator keywords for details.");
        }
        if (this->mb.getBehaviourType() !=
            BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
          auto used_tblocks = getUsedTangentOperatorBlocks();
          if (!used_tblocks.empty()) {
            auto msg =
                "the following tangent operator blocks are used in the '" +
                cname +
                "' code block and the behaviour does not declare that "
                "it computes it: " +
                makeTangentOperatorBlocksList(this->mb, used_tblocks) +
                ". See the @ProvidesSymmetricTangentOperator or "
                "the @ProvidesTangentOperator keywords for details.";
            reportWarning(msg);
          }
        }
      }
    }
    for (const auto& v : this->mb.getBehaviourData(h).getStateVariables()) {
      if (!integrator.members.contains("d" + v.name)) {
        reportWarning("the increment of the state variable '" + v.name +
                      "' is not used in the '" + cname +
                      "' code block. This increment is automatically used to "
                      "update the state variable. If the increment is not "
                      "required, please consider declaring '" +
                      v.name + "' as an auxiliary state variable.");
      }
    }
  }  // end of checkIntegratorBlock

  static bool isIntegratorRequired(
      const BehaviourDescription& bd,
      const tfel::material::ModellingHypothesis::Hypothesis h) {
    if (!bd.isModel()) {
      return true;
    }
    const auto& d = bd.getBehaviourData(h);
    if (!d.getStateVariables().empty()) {
      return true;
    }
    const auto all_asvs_external_computed = [&d] {
      for (const auto& iv : d.getAuxiliaryStateVariables()) {
        if (!iv.getAttribute<bool>("ComputedByExternalModel", false)) {
          return false;
        }
      }
      return true;
    }();
    if (!all_asvs_external_computed) {
      if (!d.hasCode(BehaviourData::UpdateAuxiliaryStateVariables)) {
        return true;
      }
    }
    return false;
  }  // end of isIntegratorRequired

  void DefaultDSLBase::endsInputFileProcessing() {
    BehaviourDSLCommon::endsInputFileProcessing();
    for (const auto& h : this->mb.getDistinctModellingHypotheses()) {
      if (isIntegratorRequired(this->mb, h)) {
        if (!this->mb.hasCode(h, BehaviourData::Integrator)) {
          this->throwRuntimeError("DefaultDSLBase::endsInputFileProcessing: ",
                                  "no code block integrator defined");
        }
        this->checkIntegratorBlock(h);
      }
    }
  }  // end of endsInputFileProcessing

  DefaultDSLBase::~DefaultDSLBase() = default;

}  // end of namespace mfront
