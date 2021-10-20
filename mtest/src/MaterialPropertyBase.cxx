/*!
 * \file   mtest/src/MaterialPropertyBase.cxx
 * \brief    
 * \author Thomas Helfer
 * \date   10/08/2021
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <iterator>
#include <algorithm>
#include "TFEL/Raise.hxx"
#include "TFEL/System/ExternalLibraryManager.hxx"
#include "MTest/MaterialPropertyBase.hxx"

namespace mtest {

  MaterialPropertyBase::MaterialPropertyBase(const std::string& l,
                                                 const std::string& f)
      : tfel::system::ExternalMaterialPropertyDescription(l, f) {
    this->arguments_values.resize(this->arguments.size());
  }  // end of MaterialPropertyBase

  std::size_t MaterialPropertyBase::getNumberOfVariables() const {
    return this->arguments.size();
  }  // end of getNumberOfVariables

  std::vector<std::string> MaterialPropertyBase::getVariablesNames() const {
    return this->arguments;
  } // end of getVariablesNames

  std::vector<std::string> MaterialPropertyBase::getParametersNames() const {
    return this->parameters;
  } // end of getParametersNames

  void MaterialPropertyBase::setVariableValue(const std::string& n,
                                              const real v) {
    const auto pb = std::begin(this->arguments);
    const auto pe = std::end(this->arguments);
    const auto p = std::find(pb, pe, n);
    if (p == pe) {
      tfel::raise(
          "MaterialPropertyBase::setVariableValue: "
          "no variable named '" +n+"'");
    }
    this->arguments_values[p - pb] = v;
  }  // end of setVariableValue

  void MaterialPropertyBase::setVariableValue(const std::size_t i,
                                              const real v) {
    if (i >= this->arguments.size()) {
      tfel::raise(
          "MaterialPropertyBase::setVariableValue: "
          "invalid offset");
    }
    this->arguments_values[i] = v;
  }  // end of setVariableValue

  void MaterialPropertyBase::setParameter(const std::string& n,
                                          const real v) {
    auto& elm =
        tfel::system::ExternalLibraryManager::getExternalLibraryManager();
    const auto pe = std::end(this->parameters);
    const auto p = std::find(std::begin(this->parameters), pe, n);
    if (p == pe) {
      tfel::raise(
          "MaterialPropertyBase::setParameter: "
          "no parameter named '" +
          n + "'");
    }
    elm.setParameter(this->library, this->material_property, n, v);
  } // end of setParameter

  MaterialPropertyBase::~MaterialPropertyBase() = default;

} // end of namespace mtest

