/*!
 * \file   mfront/src/UmatSmallStrainMTestFileGenerator.cxx
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
#include "MFront/UmatSmallStrainMTestFileGenerator.hxx"

namespace mfront {

  UmatSmallStrainMTestFileGenerator::UmatSmallStrainMTestFileGenerator(
      const std::string& i, const std::string& l, const std::string& b)
      : interface(i), library(l), behaviour(b) {
    std::fill(eto, eto + 6, 0.);
    std::fill(deto, deto + 6, 0.);
    std::fill(stress, stress + 6, 0.);
  }  // end of
     // UmatSmallStrainMTestFileGenerator::UmatSmallStrainMTestFileGenerator

  void UmatSmallStrainMTestFileGenerator::writeBehaviourDeclaration(
      std::ostream& os) const {
#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
    os << "@Behaviour<" << this->interface << "> '" << this->library
       << ".dll' '" << this->behaviour << "';" << std::endl;
#else
    os << "@Behaviour<" << this->interface << "> '" << this->library << ".so' '"
       << this->behaviour << "';" << std::endl;
#endif
  }  // end of UmatSmallStrainMTestFileGenerator::writeBehaviourDeclaration

  void UmatSmallStrainMTestFileGenerator::setStrainTensor(
      const double* const e) {
    std::copy(e, e + this->getStensorSize(), this->eto);
  }  // end of UmatSmallStrainMTestFileGenerator::setStrainTensor

  void UmatSmallStrainMTestFileGenerator::setStrainTensorIncrement(
      const double* const de) {
    std::copy(de, de + this->getStensorSize(), this->deto);
  }  // end of UmatSmallStrainMTestFileGenerator::setStrainTensorIncrement

  void UmatSmallStrainMTestFileGenerator::setStressTensor(
      const double* const s) {
    std::copy(s, s + this->getStensorSize(), this->stress);
  }  // end of UmatSmallStrainMTestFileGenerator::setStressTensor

  void UmatSmallStrainMTestFileGenerator::writeGradients(
      std::ostream& os) const {
    using namespace tfel::material;
    constexpr auto cste = tfel::math::Cste<real>::sqrt2;
    constexpr auto icste = tfel::math::Cste<real>::isqrt2;
    const auto& n = this->getStrainComponentsNames();
    unsigned short i;
    tfel::raise_if(this->times.size() != 2,
                   "UmatSmallStrainMTestFileGenerator::writeGradients: "
                   "invalid number of times");
    const real t0 = *(this->times.begin());
    const real t1 = *(this->times.rbegin());
    os << "@Stress {";
    os.precision(14);
    for (i = 0; i != this->getStensorSize();) {
      if (i < 3) {
        os << this->stress[i];
      } else {
        os << (this->stress[i]) * cste;
      }
      if (++i != this->getStensorSize()) {
        os << ",";
      }
    }
    os << "};\n\n";
    os << "@Strain {";
    auto p = n.begin();
    for (i = 0; p != n.end(); ++i) {
      os.precision(14);
      if (i < 3) {
        os << this->eto[i];
      } else {
        os << this->eto[i] * icste;
      }
      if (++p != n.end()) {
        os << ",";
      }
    }
    os << "};\n\n";
    for (p = n.begin(), i = 0; p != n.end(); ++p, ++i) {
      os.precision(14);
      if (i < 3) {
        if (!((i == 2) &&
              (this->hypothesis == ModellingHypothesis::PLANESTRAIN))) {
          os << "@ImposedStrain<evolution> '" << *p << "' {" << t0 << ":"
             << this->eto[i] << "," << t1 << ":" << this->eto[i] + this->deto[i]
             << "};\n";
        }
      } else {
        os << "@ImposedStrain<evolution> '" << *p << "' {" << t0 << ":"
           << this->eto[i] * icste << "," << t1 << ":"
           << (this->eto[i] + this->deto[i]) * icste << "};\n";
      }
    }
    os << '\n';
  }  // end of UmatSmallStrainMTestFileGenerator::writeGradients

  UmatSmallStrainMTestFileGenerator::~UmatSmallStrainMTestFileGenerator() =
      default;

}  // end of namespace mfront
