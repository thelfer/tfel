/*!
 * \file   mfront/src/DefaultCodeGeneratorBase.cxx
 * \brief
 * \author Thomas Helfer
 * \date   03/08/2022
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <ostream>
#include "MFront/DefaultCodeGeneratorBase.hxx"

namespace mfront {

  DefaultCodeGeneratorBase::DefaultCodeGeneratorBase(
      const FileDescription& f,
      const BehaviourDescription& d,
      const BehaviourInterfaceMap& bim)
      : BehaviourCodeGeneratorBase(f, d, bim) {
  }  // end of DefaultCZMCodeGenerator

  void DefaultCodeGeneratorBase::writeBehaviourParserSpecificIncludes(
      std::ostream& os) const {
    BehaviourCodeGeneratorBase::writeBehaviourParserSpecificIncludes(os);
  }  // end of writeBehaviourParserSpecificIncludes

  void DefaultCodeGeneratorBase::writeBehaviourLocalVariablesInitialisation(
      std::ostream& os, const Hypothesis h) const {
    using Modifier = std::function<std::string(const MaterialPropertyInput&)>;
    Modifier ets = [this](const MaterialPropertyInput& i) -> std::string {
      if ((i.category == MaterialPropertyInput::TEMPERATURE) ||
          (i.category ==
           MaterialPropertyInput::AUXILIARYSTATEVARIABLEFROMEXTERNALMODEL) ||
          (i.category == MaterialPropertyInput::EXTERNALSTATEVARIABLE)) {
        return "this->" + i.name + "+this->d" + i.name;
      } else if ((i.category == MaterialPropertyInput::MATERIALPROPERTY) ||
                 (i.category == MaterialPropertyInput::PARAMETER)) {
        return "this->" + i.name;
      } else if (i.category == MaterialPropertyInput::STATICVARIABLE) {
        return this->bd.getClassName() + "::" + i.name;
      } else {
        tfel::raise(
            "DefaultCodeGeneratorBase::"
            "writeBehaviourLocalVariablesInitialisation: "
            "unsupported input type for variable '" +
            i.name + "'");
      }
    };
    if (this->bd.getAttribute(BehaviourDescription::computesStiffnessTensor,
                              false)) {
      os << "// stiffness tensor at the end of the time step\n";
      this->writeStiffnessTensorComputation(os, "this->D", ets);
    }
    BehaviourCodeGeneratorBase::writeBehaviourLocalVariablesInitialisation(os,
                                                                           h);
  }

  DefaultCodeGeneratorBase::~DefaultCodeGeneratorBase() = default;

}  // end of namespace mfront
