/*!
 * \file   mfront/src/DefaultFiniteStrainCodeGenerator.cxx
 * \brief
 * \author Thomas Helfer
 * \date   04/08/2022
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <ostream>
#include "MFront/DefaultFiniteStrainCodeGenerator.hxx"

namespace mfront {

  DefaultFiniteStrainCodeGenerator::DefaultFiniteStrainCodeGenerator(
      const FileDescription& f,
      const BehaviourDescription& d,
      const BehaviourInterfaceMap& bim)
      : DefaultCodeGeneratorBase(f, d, bim) {
  }  // end of DefaultFiniteStrainCodeGenerator

  void DefaultFiniteStrainCodeGenerator::writeBehaviourParserSpecificIncludes(
      std::ostream& os) const {
    DefaultCodeGeneratorBase::writeBehaviourParserSpecificIncludes(os);
    os << "#include\"TFEL/Math/tensor.hxx\"\n";
  }  // end of writeBehaviourParserSpecificIncludes

  DefaultFiniteStrainCodeGenerator::~DefaultFiniteStrainCodeGenerator() =
      default;

}  // end of namespace mfront
