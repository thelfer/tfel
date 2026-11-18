/*!
 * \file   mfront/src/PorousStressCriterionEquivalentStress.cxx
 * \brief  This file implements the PorousStressCriterionEquivalentStress class
 * \author Tristan Chenaille
 * \date   14/11/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "TFEL/Raise.hxx"
#include "TFEL/Glossary/Glossary.hxx"
#include "TFEL/Glossary/GlossaryEntry.hxx"
#include "TFEL/Utilities/Data.hxx"
#include "MFront/BehaviourBrick/OptionDescription.hxx"
#include "MFront/BehaviourBrick/StressPotential.hxx"
#include "MFront/BehaviourBrick/StressCriterionFactory.hxx"
#include "MFront/BehaviourBrick/PorousStressCriterionEquivalentStress.hxx"

namespace mfront::bbrick {

  /*!
   * \return the role of the equivalent stress
   * \param[in] r: role of the porous stress criterion
   */
  static StressCriterion::Role getEquivalentStressRole(
      const StressCriterion::Role r) {
    return (r == StressCriterion::FLOWCRITERION)
               ? StressCriterion::FLOWCRITERION
               : StressCriterion::STRESSCRITERION;
  }  // end of getEquivalentStressRole

  /*!
   * \return the identifier of the equivalent stress
   * \param[in] id: flow id
   * \param[in] r: role of the porous stress criterion
   */
  static std::string getEquivalentStressId(const std::string& id,
                                           const StressCriterion::Role r) {
    return id + ((r == StressCriterion::FLOWCRITERION) ? "_esf" : "_es");
  }  // end of getEquivalentStressId

  /*!
   * \brief structure gathering the names of the variables defined by the
   * code computing the equivalent stress
   */
  struct EquivalentStressVariables {
    /*!
     * \param[in] id: flow id
     * \param[in] r: role of the porous stress criterion
     */
    EquivalentStressVariables(const std::string& id,
                              const StressCriterion::Role r)
        : eid(getEquivalentStressId(id, r)),
          s("s" + eid),
          seq((r == StressCriterion::FLOWCRITERION) ? "seqf" + eid
                                                    : "seq" + eid),
          dseq((r == StressCriterion::FLOWCRITERION)
                   ? "n" + eid
                   : "dseq" + eid + "_ds" + eid),
          d2seq((r == StressCriterion::FLOWCRITERION)
                    ? "dn" + eid + "_ds" + eid
                    : "d2seq" + eid + "_ds" + eid + "ds" + eid) {}
    //! \brief identifier of the equivalent stress
    const std::string eid;
    //! \brief stress
    const std::string s;
    //! \brief equivalent stress
    const std::string seq;
    //! \brief derivative of the equivalent stress
    const std::string dseq;
    //! \brief second derivative of the equivalent stress
    const std::string d2seq;
  };  // end of EquivalentStressVariables

  //! \return the name of the porosity
  static std::string getPorosityName(const BehaviourDescription& bd) {
    constexpr auto uh =
        tfel::material::ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    return bd.getBehaviourData(uh)
        .getStateVariableDescriptionByExternalName(
            tfel::glossary::Glossary::Porosity)
        .name;
  }  // end of getPorosityName

  OptionDescription PorousStressCriterionEquivalentStress::getOption() {
    return OptionDescription("equivalent_stress",
                             "choice of the equivalent stress which replaces "
                             "the von Mises stress",
                             OptionDescription::DATASTRUCTURE);
  }  // end of getOption

  PorousStressCriterionEquivalentStress::PorousStressCriterionEquivalentStress(
      const std::string& n)
      : name(n) {}  // end of PorousStressCriterionEquivalentStress

  void PorousStressCriterionEquivalentStress::initialize(
      BehaviourDescription& bd,
      AbstractBehaviourDSL& dsl,
      const std::string& id,
      const tfel::utilities::DataMap& d,
      const Role r) {
    auto raise = [this](const std::string& m) {
      tfel::raise("PorousStressCriterionEquivalentStress::initialize: " + m +
                  " (criterion '" + this->name + "')");
    };  // end of raise
    if (d.count("equivalent_stress") == 0) {
      raise("no equivalent stress defined");
    }
    const auto& e = d.at("equivalent_stress");
    const auto ds = [&e, &raise] {
      if (e.is<std::string>()) {
        tfel::utilities::DataStructure nds;
        nds.name = e.get<std::string>();
        return nds;
      }
      if (!e.is<tfel::utilities::DataStructure>()) {
        raise("invalid data type for entry 'equivalent_stress'");
      }
      return e.get<tfel::utilities::DataStructure>();
    }();
    auto& cf = StressCriterionFactory::getFactory();
    this->sc = cf.generate(ds.name);
    if (this->sc->isCoupledWithPorosityEvolution()) {
      raise("the stress criterion '" + ds.name +
            "' can't be used as an equivalent stress "
            "because it is coupled with the porosity evolution");
    }
    if (!this->sc->isNormalDeviatoric()) {
      raise("the stress criterion '" + ds.name +
            "' can't be used as an equivalent stress "
            "because its normal is not deviatoric");
    }
    this->sc->initialize(bd, dsl, getEquivalentStressId(id, r), ds.data,
                         getEquivalentStressRole(r));
  }  // end of initialize

  void PorousStressCriterionEquivalentStress::endTreatment(
      BehaviourDescription& bd,
      const AbstractBehaviourDSL& dsl,
      const std::string& id,
      const Role r) {
    this->sc->endTreatment(bd, dsl, getEquivalentStressId(id, r),
                           getEquivalentStressRole(r));
  }  // end of endTreatment

  std::string PorousStressCriterionEquivalentStress::computeElasticPrediction(
      const std::string& id,
      const BehaviourDescription& bd,
      const StressPotential& sp) const {
    const auto eid =
        getEquivalentStressId(id, StressCriterion::STRESSCRITERION);
    const auto params = StressCriterion::getVariableId(
        "sscb_parameters", id, StressCriterion::STRESSCRITERION);
    auto c = "const auto& sel" + eid + " = sel" + id + ";\n";
    c += this->sc->computeElasticPrediction(eid, bd, sp);
    c += "const auto seqel" + id + " = compute" + this->name + "Stress(sel" +
         id + ", seqel" + eid + ", this->" + getPorosityName(bd) + ", this->" +
         params + ", " + sp.getEquivalentStressLowerBound(bd) + ");\n";
    return c;
  }  // end of computeElasticPrediction

  std::string PorousStressCriterionEquivalentStress::computeCriterion(
      const std::string& id,
      const BehaviourDescription& bd,
      const StressPotential& sp) const {
    const auto v =
        EquivalentStressVariables(id, StressCriterion::STRESSCRITERION);
    const auto params = StressCriterion::getVariableId(
        "sscb_parameters", id, StressCriterion::STRESSCRITERION);
    auto c = "const auto& " + v.s + " = s" + id + ";\n";
    c += this->sc->computeCriterion(v.eid, bd, sp);
    c += "const auto seq" + id + " = compute" + this->name + "Stress(s" + id +
         ", " + v.seq + ", " + getPorosityName(bd) + "_, this->" + params +
         ", " + sp.getEquivalentStressLowerBound(bd) + ");\n";
    return c;
  }  // end of computeCriterion

  std::string PorousStressCriterionEquivalentStress::computeNormal(
      const std::string& id,
      const BehaviourDescription& bd,
      const StressPotential& sp,
      const Role r) const {
    const auto v = EquivalentStressVariables(id, r);
    const auto params =
        StressCriterion::getVariableId("sscb_parameters", id, r);
    const auto f = getPorosityName(bd);
    const auto n = "n" + id;
    const auto s = "s" + id;
    const auto seq = "seq" + id;
    const auto seqf = "seqf" + id;
    auto c = "const auto& " + v.s + " = " + s + ";\n";
    c += this->sc->computeNormal(v.eid, bd, sp, getEquivalentStressRole(r));
    if (r == StressCriterion::FLOWCRITERION) {
      c += "const auto [" + seqf +  //
           ", " + n +               //
           ", d" + seqf + "_d" + f + "] = ";
    } else {
      c += "const auto [" + seq + ", d" + seq + "_d" + s + ", d" + seq + "_d" +
           f + "] = ";
    }
    c += "compute" + this->name + "StressNormal(" + s + ", " + v.seq + ", " +
         v.dseq + ", " + f + "_, this->" + params + ", " +
         sp.getEquivalentStressLowerBound(bd) + ");\n";
    if (r == StressCriterion::STRESSANDFLOWCRITERION) {
      c += "const auto& " + n + " = d" + seq + "_d" + s + ";\n";
    }
    return c;
  }  // end of computeNormal

  std::string PorousStressCriterionEquivalentStress::computeNormalDerivative(
      const std::string& id,
      const BehaviourDescription& bd,
      const StressPotential& sp,
      const Role r) const {
    const auto v = EquivalentStressVariables(id, r);
    const auto params =
        StressCriterion::getVariableId("sscb_parameters", id, r);
    const auto f = getPorosityName(bd);
    const auto n = "n" + id;
    const auto s = "s" + id;
    const auto seq = "seq" + id;
    const auto seqf = "seqf" + id;
    auto c = "const auto& " + v.s + " = " + s + ";\n";
    c += this->sc->computeNormalDerivative(v.eid, bd, sp,
                                           getEquivalentStressRole(r));
    if (r == StressCriterion::FLOWCRITERION) {
      c += "const auto [" + seqf +    //
           ", " + n +                 //
           ", d" + seqf + "_d" + f +  //
           ", d" + n + "_d" + s +     //
           ", d" + n + "_d" + f + "] = ";
    } else {
      c += "const auto [" + seq +               //
           ", d" + seq + "_d" + s +             //
           ", d" + seq + "_d" + f +             //
           ", d2" + seq + "_d" + s + "d" + s +  //
           ", d2" + seq + "_d" + s + "d" + f + "] = ";
    }
    c += "compute" + this->name + "StressSecondDerivative(" + s + ", " + v.seq +
         ", " + v.dseq + ", " + v.d2seq + ", " + f + "_, this->" + params +
         ", " + sp.getEquivalentStressLowerBound(bd) + ");\n";
    if (r == StressCriterion::STRESSANDFLOWCRITERION) {
      c += "const auto& " + n + " = d" + seq + "_d" + s + ";\n";
      c += "const auto& d" + n + "_d" + s + " = ";
      c += "d2" + seq + "_d" + s + "d" + s + ";\n";
      c += "const auto& d" + n + "_d" + f + " = ";
      c += "d2" + seq + "_d" + s + "d" + f + ";\n";
    }
    return c;
  }  // end of computeNormalDerivative

  PorousStressCriterionEquivalentStress::
      ~PorousStressCriterionEquivalentStress() = default;

}  // end of namespace mfront::bbrick
