/*!
 * \file   mtest/src/GenericMaterialProperty.cxx
 * \brief
 * \author Thomas Helfer
 * \date   10/08/2021
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cstring>
#include <iterator>
#include <algorithm>
#include "TFEL/Raise.hxx"
#include "TFEL/System/ExternalLibraryManager.hxx"
#include "MTest/GenericMaterialProperty.hxx"

namespace mtest {

  GenericMaterialProperty::GenericMaterialProperty(const std::string& l,
                                                   const std::string& f)
      : MaterialPropertyBase(l, f) {
    using ELM = tfel::system::ExternalLibraryManager;
    if (this->mfront_interface != "Generic") {
      tfel::raise(
          "GenericMaterialProperty::GenericMaterialProperty: "
          "function '" +
          f + "' in library '" + l +
          "' has not been generated using the `Generic` interface");
    }
    auto& elm = ELM::getExternalLibraryManager();
    this->fct = elm.getGenericMaterialProperty(l, f);
  }  // end of GenericMaterialProperty

  real GenericMaterialProperty::getValue() const {
    ::mfront_gmp_OutputStatus s;
    const auto v = (*this->fct)(
        &s, this->arguments_values.data(),
        static_cast<mfront_gmp_size_type>(this->arguments_values.size()),
        GENERIC_MATERIALPROPERTY_NONE_POLICY);
    if (s.status != 0) {
      auto e = std::string{s.msg, ::strnlen(s.msg, 512)};
      tfel::raise("GenericMaterialProperty::getValue: evaluation failed (" + e +
                  ")");
    }
    return v;
  }  // end of getValue

  GenericMaterialProperty::~GenericMaterialProperty() = default;

}  // end of namespace mtest
