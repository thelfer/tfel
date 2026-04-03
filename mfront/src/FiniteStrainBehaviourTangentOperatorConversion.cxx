/*!
 * \file  mfront/src/FiniteStrainBehaviourTangentOperatorConversion.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 18 juin 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <ostream>
#include "MFront/FiniteStrainBehaviourTangentOperatorConversion.hxx"

namespace mfront {

  FiniteStrainBehaviourTangentOperatorConversion::
      FiniteStrainBehaviourTangentOperatorConversion(
          const TangentOperatorFlag f,
          const TangentOperatorFlag t,
          const std::string& s,
          const std::string& s2)
      : b(f),
        e(t),
        c(s),
        cf(s2) {
  }  // end of
     // FiniteStrainBehaviourTangentOperatorConversion::FiniteStrainBehaviourTangentOperatorConversion

  FiniteStrainBehaviourTangentOperatorConversion::TangentOperatorFlag
  FiniteStrainBehaviourTangentOperatorConversion::from() const {
    return this->b;
  }  // end of FiniteStrainBehaviourTangentOperatorConversion::from

  FiniteStrainBehaviourTangentOperatorConversion::TangentOperatorFlag
  FiniteStrainBehaviourTangentOperatorConversion::to() const {
    return this->e;
  }  // end of FiniteStrainBehaviourTangentOperatorConversion::to

  const std::string&
  FiniteStrainBehaviourTangentOperatorConversion::getIntermediateConversion()
      const {
    return this->c;
  }

  const std::string&
  FiniteStrainBehaviourTangentOperatorConversion::getFinalConversion() const {
    return this->cf;
  }

  std::vector<FiniteStrainBehaviourTangentOperatorConversion>
  FiniteStrainBehaviourTangentOperatorConversion::
      getAvailableFiniteStrainBehaviourTangentOperatorConversions() {
    using TangentOperator =
        tfel::material::FiniteStrainBehaviourTangentOperatorBase;
    using Converter = FiniteStrainBehaviourTangentOperatorConversion;
    auto converters = std::vector<Converter>{};
    auto add = [&converters](Converter&& converter) {
      converters.push_back(converter);
    };
    auto std_add = [&add](const TangentOperator::Flag op1,
                          const TangentOperator::Flag op2) {
      const auto s1 =
          convertFiniteStrainBehaviourTangentOperatorFlagToString(op1);
      const auto s2 =
          convertFiniteStrainBehaviourTangentOperatorFlagToString(op2);
      add({op1, op2,
           "const auto tangentOperator_" + s2 + " = convert<" + s2 + "," + s1 +
               ">(tangentOperator_" + s1 + ",this->F0,this->F1,this->sig);",
           "this->Dt = convert<" + s2 + "," + s1 + ">(tangentOperator_" + s1 +
               ",this->F0,this->F1,this->sig);"});
    };
    // for all this converters, a specialisation of the
    // FiniteStrainBehaviourTangentOperatorConverter exist.  Here, we
    // indicate the source and then the target. The template
    // parameters of the FiniteStrainBehaviourTangentOperatorConverter
    // are given in the reverse order.
    std_add(TangentOperator::DS_DF, TangentOperator::DTAU_DF);
    std_add(TangentOperator::DTAU_DF, TangentOperator::C_TAU_JAUMANN);
    std_add(TangentOperator::DTAU_DF, TangentOperator::SPATIAL_MODULI);
    std_add(TangentOperator::DTAU_DF, TangentOperator::ABAQUS);
    std_add(TangentOperator::DTAU_DF, TangentOperator::DSIG_DF);
    std_add(TangentOperator::DTAU_DF, TangentOperator::DTAU_DDF);
    std_add(TangentOperator::DTAU_DDF, TangentOperator::DTAU_DF);
    std_add(TangentOperator::SPATIAL_MODULI, TangentOperator::DTAU_DF);
    std_add(TangentOperator::SPATIAL_MODULI, TangentOperator::C_TRUESDELL);
    std_add(TangentOperator::SPATIAL_MODULI, TangentOperator::ABAQUS);
    std_add(TangentOperator::SPATIAL_MODULI, TangentOperator::DS_DEGL);
    std_add(TangentOperator::DSIG_DF, TangentOperator::DSIG_DDF);
    std_add(TangentOperator::DSIG_DDF, TangentOperator::DSIG_DF);
    std_add(TangentOperator::DS_DEGL, TangentOperator::DS_DC);
    std_add(TangentOperator::DS_DEGL, TangentOperator::SPATIAL_MODULI);
    std_add(TangentOperator::DS_DEGL, TangentOperator::ABAQUS);
    std_add(TangentOperator::DS_DEGL, TangentOperator::DS_DF);
    std_add(TangentOperator::DS_DC, TangentOperator::DS_DF);
    std_add(TangentOperator::DS_DC, TangentOperator::DS_DEGL);
    std_add(TangentOperator::C_TRUESDELL, TangentOperator::SPATIAL_MODULI);
    std_add(TangentOperator::ABAQUS, TangentOperator::SPATIAL_MODULI);
    std_add(TangentOperator::ABAQUS, TangentOperator::C_TAU_JAUMANN);
    std_add(TangentOperator::ABAQUS, TangentOperator::DSIG_DF);
    std_add(TangentOperator::ABAQUS, TangentOperator::DTAU_DF);
    std_add(TangentOperator::C_TAU_JAUMANN, TangentOperator::ABAQUS);
    std_add(TangentOperator::C_TAU_JAUMANN, TangentOperator::DTAU_DF);
    std_add(TangentOperator::DT_DELOG, TangentOperator::DS_DC);
    std_add(TangentOperator::DT_DELOG, TangentOperator::SPATIAL_MODULI);
    std_add(TangentOperator::DT_DELOG, TangentOperator::C_TRUESDELL);
    std_add(TangentOperator::DSIG_DF, TangentOperator::DPK1_DF);
    std_add(TangentOperator::DS_DEGL, TangentOperator::DPK1_DF);
    std_add(TangentOperator::DPK1_DF, TangentOperator::DTAU_DF);
    std_add(TangentOperator::DPK1_DF, TangentOperator::DSIG_DF);
    return converters;
  }  // end of
     // FiniteStrainBehaviourTangentOperatorConversion::getAvailableFiniteStrainBehaviourTangentOperatorConversions

  FiniteStrainBehaviourTangentOperatorConversion::
      ~FiniteStrainBehaviourTangentOperatorConversion() noexcept = default;

}  // end of namespace mfront
