/*!
 * \file  mfront/src/PowellDogLegAlgorithmBase.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 22 août 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <sstream>
#include "TFEL/Raise.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"
#include "MFront/BehaviourDescription.hxx"
#include "MFront/NonLinearSystemSolverBase.hxx"
#include "MFront/PowellDogLegAlgorithmBase.hxx"

namespace mfront {

  std::vector<std::string> PowellDogLegAlgorithmBase::getReservedNames() {
    return {"pdl_g",     "pdl_g2",   "pdl_0",
            "pdl_1",     "pdl_2",    "pdl_3",
            "pdl_alpha", "pdl_cste", "powell_dogleg_trust_region_size_inv"};
  }  // end of PowellDogLegAlgorithmBase::getReservedNames

  std::pair<bool, PowellDogLegAlgorithmBase::tokens_iterator>
  PowellDogLegAlgorithmBase::treatSpecificKeywords(BehaviourDescription& mb,
                                                   const std::string& key,
                                                   const tokens_iterator p,
                                                   const tokens_iterator pe) {
    using namespace tfel::material;
    using namespace tfel::utilities;
    const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    if (key == "@PowellDogLegTrustRegionSize") {
      CxxTokenizer::TokensContainer::const_iterator current = p;
      CxxTokenizer::checkNotEndOfLine(
          "PowellDogLegAlgorithmBase::treatSpecificKeywords", current, pe);
      CxxTokenizer::checkNotEndOfLine(
          "ImplicitDSLBase::treatPowellDogLegTrustRegionSize"
          "Cannot read pdl_trs value.",
          current, pe);
      const auto pdl_trs = convert<double>(current->value);
      tfel::raise_if(pdl_trs < 0,
                     "ImplicitDSLBase::treatPowellDogLegTrustRegionSize: "
                     "region size must be positive.");
      ++current;
      CxxTokenizer::readSpecifiedToken(
          "ImplicitDSLBase::treatPowellDogLegTrustRegionSize", ";", current,
          pe);
      mb.addParameter(
          h, VariableDescription("NumericType",
                                 "powell_dogleg_trust_region_size", 1u, 0u));
      mb.setParameterDefaultValue(h, "powell_dogleg_trust_region_size",
                                  pdl_trs);
      return {true, current};
    }
    return {false, p};
  }  // end of PowellDogLegAlgorithmBase::treatSpecificKeywords

  void PowellDogLegAlgorithmBase::completeVariableDeclaration(
      BehaviourDescription& mb) {
    using namespace tfel::material;
    const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    if (!mb.hasParameter(h, "powell_dogleg_trust_region_size")) {
      mb.addParameter(
          h, VariableDescription("NumericType",
                                 "powell_dogleg_trust_region_size", 1u, 0u));
      mb.setParameterDefaultValue(h, "powell_dogleg_trust_region_size", 1.e-4);
    }
  }  // end of PowellDogLegAlgorithmBase::completeVariableDeclaration

  void PowellDogLegAlgorithmBase::initializeNumericalParameters(
      std::ostream& os, const std::string& n) {
    const auto radius = "powell_dogleg_trust_region_size";
    os << n << "::" << radius << "= this->" << radius << ";\n";
  }  // end of initializeNumericalParameters

  void PowellDogLegAlgorithmBase::writePowellDogLegStep(
      std::ostream& out,
      const BehaviourDescription& mb,
      const Hypothesis h,
      const std::string& B,
      const std::string& f,
      const std::string& pn) {
    const auto& d = mb.getBehaviourData(h);
    const auto n = d.getIntegrationVariables().getTypeSize();
    out << "if(std::abs(" << pn << ")<(" << n
        << ")*(this->powell_dogleg_trust_region_size)){\n"
        << "// using the newton method only\n";
    NonLinearSystemSolverBase::writeLimitsOnIncrementValues(out, mb, h, pn);
    out << "this->zeros -= " << pn << ";\n"
        << "} else { \n"
        << "// computing the steepest descent step\n"
        << "tvector<" << n << ",real> pdl_g;\n"
        << "tvector<" << n << ",real> pdl_g2;\n"
        << "for(unsigned short idx=0;idx!=" << n << ";++idx){\n"
        << "pdl_g[idx]=real(0);\n"
        << "for(unsigned short idx2=0;idx2!=" << n << ";++idx2){\n"
        << "pdl_g[idx] += (" << B << "(idx2,idx)) * (" << f << "(idx2));\n"
        << "}\n"
        << "}\n"
        << "for(unsigned short idx=0;idx!=" << n << ";++idx){\n"
        << "pdl_g2[idx]=real(0);\n"
        << "for(unsigned short idx2=0;idx2!=" << n << ";++idx2){\n"
        << "pdl_g2[idx] += (" << B << "(idx,idx2)) * pdl_g(idx2);\n"
        << "}\n"
        << "}\n"
        << "const real pdl_cste = (pdl_g|pdl_g)/(pdl_g2|pdl_g2);\n"
        << "pdl_g *= pdl_cste;\n"
        << "if(abs(pdl_g)<(" << n
        << ")*(this->powell_dogleg_trust_region_size)){\n"
        << "const real pdl_0 = "
           "(this->powell_dogleg_trust_region_size)*(this->powell_dogleg_trust_"
           "region_size);\n"
        << "const real pdl_1 = (pdl_g|pdl_g);\n"
        << "const real pdl_2 = ((" << pn << ")|pdl_g);\n"
        << "const real pdl_3 = ((" << pn << ")|(" << pn << "));\n"
        << "const real pdl_alpha = "
        << "(pdl_0-pdl_1)/"
           "((pdl_2-pdl_1)+sqrt(max((pdl_2-pdl_0)*(pdl_2-pdl_0)+(pdl_3-pdl_0)*("
           "pdl_0-pdl_1),real(0))));\n"
        << "pdl_g = pdl_alpha*(" << pn << ") + (1-pdl_alpha)*pdl_g;\n"
        << "} else {\n"
        << "const real pdl_alpha = "
           "(this->powell_dogleg_trust_region_size)/(norm(pdl_g));\n"
        << "pdl_g *= pdl_alpha;\n"
        << "}\n";
    NonLinearSystemSolverBase::writeLimitsOnIncrementValues(out, mb, h,
                                                            "pdl_g");
    out << "this->zeros -= pdl_g;\n"
        << "}\n";
  }  // end of ImplicitDSLBase::writePowellDogLegStep

  PowellDogLegAlgorithmBase::~PowellDogLegAlgorithmBase() = default;

}  // end of namespace mfront
