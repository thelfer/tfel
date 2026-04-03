/*!
 * \file   mfront/src/DefaultCZMCodeGenerator.cxx
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
#include "MFront/DefaultCZMCodeGenerator.hxx"

namespace mfront {

  DefaultCZMCodeGenerator::DefaultCZMCodeGenerator(
      const FileDescription& f,
      const BehaviourDescription& d,
      const BehaviourInterfaceMap& bim)
      : DefaultCodeGeneratorBase(f, d, bim) {
  }  // end of DefaultCZMCodeGenerator

  void DefaultCZMCodeGenerator::writeBehaviourParserSpecificIncludes(
      std::ostream& os) const {
    DefaultCodeGeneratorBase::writeBehaviourParserSpecificIncludes(os);
    os << "#include\"TFEL/Math/tmatrix.hxx\"\n"
       << "#include\"TFEL/Math/tvector.hxx\"\n"
       << "#include\"TFEL/Math/Vector/tvectorIO.hxx\"\n"
       << "#include\"TFEL/Math/Matrix/tmatrixIO.hxx\"\n";
  }  // end of writeBehaviourParserSpecificIncludes

  std::string DefaultCZMCodeGenerator::getLocalVariablesInitializers(
      const Hypothesis) const {
    return "u_n(this->u(0)),\n"
           "du_n(this->du(0)),\n"
           "t_n(this->t(0)),\n"
           "u_t(tfel::math::map<tfel::math::tvector<N-1,real>, 1>(this->u)),\n"
           "du_t(tfel::math::map<tfel::math::tvector<N-1,real>, "
           "1>(this->du)),\n"
           "t_t(tfel::math::map<tfel::math::tvector<N-1,real>, 1>(this->t)),\n"
           "Dt_nn(this->Dt(0,0)),\n"
           "Dt_nt(this->Dt.template row_view<0, 1, N-1>()),\n"
           "Dt_tn(this->Dt.template column_view<0, 1, N-1>()),\n"
           "Dt_tt(this->Dt.template submatrix_view<1, 1, N-1, N-1>())";
  }  // end of getLocalVariablesInitializers

  DefaultCZMCodeGenerator::~DefaultCZMCodeGenerator() = default;

}  // end of namespace mfront
