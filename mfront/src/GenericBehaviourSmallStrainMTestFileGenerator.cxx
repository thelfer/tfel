/*!
 * \file   mfront/src/GenericBehaviourSmallStrainMTestFileGenerator.cxx
 * \brief
 * \author Thomas Helfer
 * \brief  10 juil. 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cmath>
#include <ostream>
#include <stdexcept>
#include <algorithm>
#include "TFEL/Raise.hxx"
#include "TFEL/Math/General/MathConstants.hxx"
#include "MFront/GenericBehaviourSmallStrainMTestFileGenerator.hxx"

namespace mfront {

  GenericBehaviourSmallStrainMTestFileGenerator::
      GenericBehaviourSmallStrainMTestFileGenerator(const char* const l,
                                                    const char* const b)
      : library(l), behaviour(b) {
    std::fill(this->eto0, this->eto0 + 6, 0.);
    std::fill(this->eto1, this->eto1 + 6, 0.);
    std::fill(this->stress, this->stress + 6, 0.);
  }  // end of GenericBehaviourSmallStrainMTestFileGenerator

  void GenericBehaviourSmallStrainMTestFileGenerator::writeBehaviourDeclaration(
      std::ostream& os) const {
#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
    os << "@Behaviour<generic> '" << this->library << ".dll' '"
       << this->behaviour << "';" << std::endl;
#else
    os << "@Behaviour<generic> '" << this->library << ".so' '"
       << this->behaviour << "';" << std::endl;
#endif
  }  // end of writeBehaviourDeclaration

  void GenericBehaviourSmallStrainMTestFileGenerator::
      setStrainTensorAtTheBeginningOfTheTimeStep(const double* const e) {
    std::copy(e, e + this->getStensorSize(), this->eto0);
  }  // end of setStrainTensorAtTheBeginningOfTheTimeStep

  void GenericBehaviourSmallStrainMTestFileGenerator::
      setStrainTensorAtTheEndOfTheTimeStep(const double* const e) {
    std::copy(e, e + this->getStensorSize(), this->eto1);
  }  // end of setStrainTensorAtTheEndOfTheTimeStep

  void GenericBehaviourSmallStrainMTestFileGenerator::setStressTensor(
      const double* const s) {
    std::copy(s, s + this->getStensorSize(), this->stress);
  }  // end of setStressTensor

  void GenericBehaviourSmallStrainMTestFileGenerator::writeGradients(
      std::ostream& os) const {
    using namespace tfel::material;
    const auto& n = this->getStrainComponentsNames();
    unsigned short i;
    tfel::raise_if(
        this->times.size() != 2,
        "GenericBehaviourSmallStrainMTestFileGenerator::writeGradients: "
        "invalid number of times");
    const real t0 = *(this->times.begin());
    const real t1 = *(this->times.rbegin());
    os << "@Stress {";
    os.precision(14);
    for (i = 0; i != this->getStensorSize();) {
      os << this->stress[i];
      if (++i != this->getStensorSize()) {
        os << ",";
      }
    }
    os << "};\n\n";
    os << "@Strain {";
    auto p = n.begin();
    for (i = 0; p != n.end(); ++i) {
      os.precision(14);
      os << this->eto0[i];
      if (++p != n.end()) {
        os << ",";
      }
    }
    os << "};\n\n";
    for (p = n.begin(), i = 0; p != n.end(); ++p, ++i) {
      os.precision(14);
      if (!((i == 2) &&
            (this->hypothesis == ModellingHypothesis::PLANESTRAIN))) {
        os << "@ImposedStrain<evolution> '" << *p << "' {" << t0 << ":"
           << this->eto0[i] << "," << t1 << ":" << this->eto1[i] << "};\n";
      }
    }
    os << '\n';
  }  // end of writeGradients

  GenericBehaviourSmallStrainMTestFileGenerator::
      ~GenericBehaviourSmallStrainMTestFileGenerator() = default;

}  // end of namespace mfront
