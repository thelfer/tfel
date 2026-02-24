/*!
 * \file   mfront/src/GenericBehaviourMTestFileGenerator.cxx
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
#include "MFront/GenericBehaviourMTestFileGenerator.hxx"

namespace mfront {

  GenericBehaviourMTestFileGenerator::GenericBehaviourMTestFileGenerator(
      const BehaviourVariablesDescription& bvd,
      const char* const l,
      const char* const b)
      : d(bvd),
        library(l),
        behaviour(b) {}  // end of GenericBehaviourMTestFileGenerator

  void GenericBehaviourMTestFileGenerator::writeBehaviourDeclaration(
      std::ostream& os) const {
#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
    os << "@Behaviour<generic> '" << this->library << ".dll' '"
       << this->behaviour << "';" << std::endl;
#else
    os << "@Behaviour<generic> '" << this->library << ".so' '"
       << this->behaviour << "';" << std::endl;
#endif
  }  // end of writeBehaviourDeclaration

  void
  GenericBehaviourMTestFileGenerator::setGradientsAtTheBeginningOfTheTimeStep(
      const double* const g) {
    this->gradients_bts = g;
  }  // end of setGradientsAtTheBeginningOfTheTimeStep

  void GenericBehaviourMTestFileGenerator::setGradientsAtTheEndOfTheTimeStep(
      const double* const g) {
    this->gradients_ets = g;
  }  // end of setGradientsAtTheEndOfTheTimeStep

  void GenericBehaviourMTestFileGenerator::
      setThermodynamicForcesAtTheBeginningOfTheTimeStep(
          const double* const thfs) {
    this->th_forces_bts = thfs;
  }  // end of setThermodynamicForcesAtTheBeginningOfTheTimeStep

  void GenericBehaviourMTestFileGenerator::writeGradients(
      std::ostream& os) const {
    //
    using VariableDescription =
        BehaviourVariablesDescription::VariableDescription;
    //
    tfel::raise_if(this->times.size() != 2,
                   "GenericBehaviourMTestFileGenerator::writeGradients: "
                   "invalid number of times");
    //
    auto writeArray = [&os](const double* const v, const std::size_t s) {
      for (std::size_t i = 0; i != s;) {
        os << v[i];
        if (++i != s) {
          os << ",";
        }
      }
    };
    //
    auto getSize = [this](const VariableDescription& v) -> unsigned short {
      if (v.type == SupportedTypes::SCALAR) {
        return 1;
      }
      if (v.type == SupportedTypes::TVECTOR) {
        return this->getTVectorSize();
      }
      if (v.type == SupportedTypes::STENSOR) {
        return this->getStensorSize();
      }
      if (v.type == SupportedTypes::TENSOR) {
        return this->getTensorSize();
      }
      tfel::raise(
          "GenericBehaviourMTestFileGenerator::writeGradients:"
          "unsupported variable type");
    };
    auto getComponents =
        [this](const VariableDescription& v) -> std::vector<std::string> {
      if (v.type == SupportedTypes::SCALAR) {
        return {v.name};
      }
      if (v.type == SupportedTypes::TVECTOR) {
        return this->getTVectorComponentsNames(v.name);
      }
      if (v.type == SupportedTypes::STENSOR) {
        return this->getStensorComponentsNames(v.name);
      }
      if (v.type == SupportedTypes::TENSOR) {
        return this->getTensorComponentsNames(v.name);
      }
      tfel::raise(
          "GenericBehaviourMTestFileGenerator::writeGradients:"
          "unsupported variable type");
    };
    auto gradients_size = std::size_t{};
    auto th_forces_size = std::size_t{};
    for (std::size_t i = 0; i != d.ngradients; ++i) {
      gradients_size += getSize(this->d.gradients[i]);
      th_forces_size += getSize(this->d.thermodynamic_forces[i]);
    }
    os.precision(14);
    os << "@ThermodynamicForce {";
    writeArray(this->th_forces_bts, th_forces_size);
    os << "};\n\n";
    os << "@Gradient{";
    writeArray(this->gradients_bts, gradients_size);
    os << "};\n\n";
    //
    const auto t0 = *(this->times.begin());
    const auto t1 = *(this->times.rbegin());
    auto pos = std::size_t{};
    for (std::size_t i = 0; i != d.ngradients; ++i) {
      const auto& g = (this->d.gradients[i]);
      for (const auto& c : getComponents(g)) {
        os << "@ImposedGradient<evolution> '" << c << "' {" << t0 << ":"
           << this->gradients_bts[pos] << "," << t1 << ":"
           << this->gradients_ets[pos] << "};\n";
        ++pos;
      }
    }
  }  // end of writeGradients

  GenericBehaviourMTestFileGenerator::~GenericBehaviourMTestFileGenerator() =
      default;

}  // end of namespace mfront
