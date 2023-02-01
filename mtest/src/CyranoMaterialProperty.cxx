/*!
 * \file   mtest/src/CyranoMaterialProperty.cxx
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
#include "MTest/CyranoMaterialProperty.hxx"

namespace mtest {

  CyranoMaterialProperty::CyranoMaterialProperty(const std::string& l,
                                                 const std::string& f)
      : MaterialPropertyBase(l, f) {
    using ELM = tfel::system::ExternalLibraryManager;
    if (this->mfront_interface != "Cyrano") {
      tfel::raise(
          "CyranoMaterialProperty::CyranoMaterialProperty: "
          "function '" +
          f + "' in library '" + l +
          "' has not been generated using the `Cyrano` interface");
    }
    auto& elm = ELM::getExternalLibraryManager();
    this->fct = elm.getCyranoMaterialProperty(l, f);
  }  // end of CyranoMaterialProperty

  real CyranoMaterialProperty::getValue() const {
    ::CyranoOutputStatus s;
    const auto v = (*this->fct)(
        &s, this->arguments_values.data(),
        static_cast<CyranoIntegerType>(this->arguments_values.size()),
        CYRANO_NONE_POLICY);
    if (s.status != 0) {
      tfel::raise("CyranoMaterialProperty::getValue: evaluation failed");
    }
    return v;
  }  // end of getValue

  CyranoMaterialProperty::~CyranoMaterialProperty() = default;

}  // end of namespace mtest
