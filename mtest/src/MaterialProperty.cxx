/*!
 * \file   mtest/src/MaterialProperty.cxx
 * \brief
 * \author Thomas Helfer
 * \date   10/08/2021
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "TFEL/Raise.hxx"
#include "MTest/MaterialProperty.hxx"
#include "MTest/CastemMaterialProperty.hxx"
#include "MTest/CyranoMaterialProperty.hxx"

namespace mtest {

  std::shared_ptr<MaterialProperty> MaterialProperty::getMaterialProperty(
      const std::string& i, const std::string& l, const std::string& f) {
    if ((i == "castem") || (i == "Castem") || (i == "Cast3M")) {
      return std::make_shared<CastemMaterialProperty>(l, f);
    }
    if ((i == "cyrano") || (i == "Cyrano")) {
      return std::make_shared<CyranoMaterialProperty>(l, f);
    }
    tfel::raise(
        "MaterialProperty::getMaterialProperty: "
        "invalid interface '" +
        i + "'");
  }  // end of getMaterialProperty

  MaterialProperty::~MaterialProperty() = default;

  real getValue(MaterialProperty& mp,
                const std::map<std::string, real>& args,
                const bool b) {
    if (!b) {
      if (args.size() != mp.getNumberOfVariables()) {
        tfel::raise("mtest::getValue: invalid number of arguments");
      }
    }
    for (const auto& a : args) {
      mp.setVariableValue(a.first, a.second);
    }
    return mp.getValue();
  }  // end of getValue

  real getValue(MaterialProperty& mp, const std::vector<real>& values) {
    if (values.size() != mp.getNumberOfVariables()) {
      tfel::raise("mtest::getValue: invalid number of arguments");
    }
    for (std::size_t i = 0; i != values.size(); ++i) {
      mp.setVariableValue(i, values[i]);
    }
    return mp.getValue();
  }  // end of getValue

  real getValue(MaterialProperty& mp, const real v) {
    if (mp.getNumberOfVariables() != 1u) {
      tfel::raise("mtest::getValue: invalid number of arguments");
    }
    mp.setVariableValue(0, v);
    return mp.getValue();
  }

  real getValue(MaterialProperty& mp) {
    if (mp.getNumberOfVariables() != 0u) {
      tfel::raise("mtest::getValue: invalid number of arguments");
    }
    return mp.getValue();
  }

}  // end of namespace mtest
