/*!
 * \file   mtest/src/CastemMaterialProperty.cxx
 * \brief
 * \author Thomas Helfer
 * \date   10/08/2021
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <iostream>
#include <iterator>
#include <algorithm>
#include "TFEL/Raise.hxx"
#include "TFEL/System/ExternalLibraryManager.hxx"
#include "MTest/CastemMaterialProperty.hxx"

namespace mtest {

  CastemMaterialProperty::CastemMaterialProperty(const std::string& l,
                                                 const std::string& f)
      : MaterialPropertyBase(l, f) {
    using ELM = tfel::system::ExternalLibraryManager;
    if (this->mfront_interface != "Castem") {
      tfel::raise(
          "CastemMaterialProperty::CastemMaterialProperty: "
          "function '" +
          f + "' in library '" + l +
          "' has not been generated using the `Cast3M` interface");
    }
    auto& elm = ELM::getExternalLibraryManager();
    this->fct = elm.getCastemFunction(l, f);
  }  // end of CastemMaterialProperty

  real CastemMaterialProperty::getValue() const {
    return (*this->fct)(this->arguments_values.data());
  }  // end of getValue

  CastemMaterialProperty::~CastemMaterialProperty() = default;

}  // end of namespace mtest
