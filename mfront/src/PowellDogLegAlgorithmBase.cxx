/*!
 * \file  mfront/src/PowellDogLegAlgorithmBase.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 22 août 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <iostream>
#include <sstream>

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
    using namespace std;
    using namespace tfel::material;
    using namespace tfel::utilities;
    const ModellingHypothesis::Hypothesis h =
        ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    if (key == "@PowellDogLegTrustRegionSize") {
      CxxTokenizer::TokensContainer::const_iterator current = p;
      CxxTokenizer::checkNotEndOfLine(
          "PowellDogLegAlgorithmBase::treatSpecificKeywords", current, pe);
      double pdl_trs;
      CxxTokenizer::checkNotEndOfLine(
          "ImplicitDSLBase::treatPowellDogLegTrustRegionSize"
          "Cannot read pdl_trs value.",
          current, pe);
      istringstream flux(current->value);
      flux >> pdl_trs;
      if ((flux.fail()) || (!flux.eof())) {
        string msg("ImplicitDSLBase::treatPowellDogLegTrustRegionSize",
                   "Failed to read region size.");
        throw(runtime_error(msg));
      }
      if (pdl_trs < 0) {
        string msg("ImplicitDSLBase::treatPowellDogLegTrustRegionSize",
                   "Region size must be positive.");
        throw(runtime_error(msg));
      }
      ++current;
      CxxTokenizer::readSpecifiedToken(
          "ImplicitDSLBase::treatPowellDogLegTrustRegionSize", ";", current,
          pe);
      mb.addParameter(
          h, VariableDescription("real", "powell_dogleg_trust_region_size", 1u,
                                 0u));
      mb.setParameterDefaultValue(h, "powell_dogleg_trust_region_size",
                                  pdl_trs);
      return make_pair(true, current);
    }
    return make_pair(false, p);
  }  // end of PowellDogLegAlgorithmBase::treatSpecificKeywords

  void PowellDogLegAlgorithmBase::endsInputFileProcessing(
      BehaviourDescription& mb) {
    using namespace tfel::material;
    const ModellingHypothesis::Hypothesis h =
        ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    if (!mb.hasParameter(h, "powell_dogleg_trust_region_size")) {
      mb.addParameter(
          h, VariableDescription("real", "powell_dogleg_trust_region_size", 1u,
                                 0u));
      mb.setParameterDefaultValue(h, "powell_dogleg_trust_region_size", 1.e-4);
    }
  }  // end of PowellDogLegAlgorithmBase::endsInputFileProcessing

  void PowellDogLegAlgorithmBase::writePowellDogLegStep(
      std::ostream& out,
      const BehaviourDescription& mb,
      const Hypothesis h,
      const std::string& B,
      const std::string& f,
      const std::string& pn) {
    using namespace std;
    const auto& d = mb.getBehaviourData(h);
    VariableDescriptionContainer::const_iterator p;
    SupportedTypes::TypeSize n;
    for (p = d.getIntegrationVariables().begin();
         p != d.getIntegrationVariables().end(); ++p) {
      n += mb.getTypeSize(p->type, p->arraySize);
    }
    out << "if(abs(" << pn << ")<(" << n
        << ")*(this->powell_dogleg_trust_region_size)){" << endl;
    out << "// using the newton method only" << endl;
    NonLinearSystemSolverBase::writeLimitsOnIncrementValues(out, mb, h, pn);
    out << "this->zeros -= " << pn << ";\n";
    out << "} else { " << endl;
    out << "// computing the steepest descent step\n";
    out << "tvector<" << n << ",real> pdl_g;\n";
    out << "tvector<" << n << ",real> pdl_g2;\n";
    out << "for(unsigned short idx=0;idx!=" << n << ";++idx){" << endl;
    out << "pdl_g[idx]=real(0);\n";
    out << "for(unsigned short idx2=0;idx2!=" << n << ";++idx2){" << endl;
    out << "pdl_g[idx] += (" << B << "(idx2,idx)) * (" << f << "(idx2));\n";
    out << "}" << endl;
    out << "}" << endl;
    out << "for(unsigned short idx=0;idx!=" << n << ";++idx){" << endl;
    out << "pdl_g2[idx]=real(0);\n";
    out << "for(unsigned short idx2=0;idx2!=" << n << ";++idx2){" << endl;
    out << "pdl_g2[idx] += (" << B << "(idx,idx2)) * pdl_g(idx2);\n";
    out << "}" << endl;
    out << "}" << endl;
    out << "const real pdl_cste = (pdl_g|pdl_g)/(pdl_g2|pdl_g2);" << endl;
    out << "pdl_g *= pdl_cste;" << endl;
    out << "if(abs(pdl_g)<(" << n
        << ")*(this->powell_dogleg_trust_region_size)){" << endl;
    out << "const real pdl_0 = "
           "(this->powell_dogleg_trust_region_size)*(this->powell_dogleg_trust_"
           "region_size);"
        << endl;
    out << "const real pdl_1 = (pdl_g|pdl_g);" << endl;
    out << "const real pdl_2 = ((" << pn << ")|pdl_g);" << endl;
    out << "const real pdl_3 = ((" << pn << ")|(" << pn << "));" << endl;
    out << "const real pdl_alpha = "
        << "(pdl_0-pdl_1)/"
           "((pdl_2-pdl_1)+sqrt(max((pdl_2-pdl_0)*(pdl_2-pdl_0)+(pdl_3-pdl_0)*("
           "pdl_0-pdl_1),real(0))));"
        << endl;
    out << "pdl_g = pdl_alpha*(" << pn << ") + (1-pdl_alpha)*pdl_g;" << endl;
    out << "} else {" << endl;
    out << "const real pdl_alpha = "
           "(this->powell_dogleg_trust_region_size)/(norm(pdl_g));"
        << endl;
    out << "pdl_g *= pdl_alpha;" << endl;
    out << "}" << endl;
    NonLinearSystemSolverBase::writeLimitsOnIncrementValues(out, mb, h,
                                                            "pdl_g");
    out << "this->zeros -= pdl_g;\n";
    out << "}" << endl;
  }  // end of ImplicitDSLBase::writePowellDogLegStep

  PowellDogLegAlgorithmBase::~PowellDogLegAlgorithmBase() = default;

}  // end of namespace mfront
