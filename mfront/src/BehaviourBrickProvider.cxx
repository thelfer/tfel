/*!
 * \file   BehaviourBrickProvider.cxx
 * \brief
 * \author THOMAS HELFER
 * \date   26 juin 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 * <!-- Local IspellDict: english -->
 */

#include <sstream>
#include <algorithm>
#include <stdexcept>
#include "MFront/SupportedTypes.hxx"
#include "MFront/VariableDescription.hxx"
#include "MFront/StaticVariableDescription.hxx"
#include "MFront/BehaviourBrick/Requirement.hxx"
#include "MFront/BehaviourBrick/Provider.hxx"

namespace mfront {

  namespace bbrick {

    Provider::~Provider() = default;

    bool ProviderBase::handleRequirement(const Requirement& r,
                                         const bool b) const {
      using namespace std;
      if (this->getExternalName() != r.name) {
        return false;
      }
      if (this->getArraySize() != r.asize) {
        ostringstream msg;
        msg << "ProviderBase::handleRequirement : "
            << "unmatched array size for requirement '"
            << this->getExternalName() << "' "
            << "(requirement array size is " << r.asize << " and "
            << " provided variable array size is " << this->getArraySize()
            << ")";
        throw(runtime_error(msg.str()));
      }
      // check for type
      if (this->getVariableType() != r.type) {
        const auto s = SupportedTypes{};
        const auto f1 = s.getTypeFlag(r.type);
        const auto f2 = s.getTypeFlag(this->getVariableType());
        const bool b1 = f1 == f2;
        if ((!b1) || (b)) {
          throw(runtime_error(
              "ProviderBase::handleRequirement : "
              "provided value of type '" +
              this->getVariableType() + "' does not math requirement '" +
              r.name + "' type ('" + r.type +
              ""
              "')"));
        }
      }
      // check if provider is allowed
      const auto id = this->getIdentifier();
      if (find(r.aproviders.begin(), r.aproviders.end(), id) ==
          r.aproviders.end()) {
        throw(
            runtime_error("ProviderBase::handleRequirement : "
                          "a provider of type '" +
                          convertProviderIdentifierToString(id) +
                          "' is not allowed for "
                          "requirement '" +
                          r.name + "'"));
      }
      return true;
    }  // end of ProviderBase::handleRequirement

    ProviderBase::~ProviderBase() = default;

    StandardProvider::StandardProvider(const std::string& t,
                                       const std::string& n,
                                       const std::string& e,
                                       const unsigned short s,
                                       const bool b)
        : type(t), name(n), ename(e), asize(s) {
      const auto st = SupportedTypes{};
      if (b) {
        if (!st.isSupportedType(this->type)) {
          throw(
              std::runtime_error("StandardProvider::StandardProvider : "
                                 "unsupported type '" +
                                 this->type + "'"));
        }
      }
    }  // end of StandardProvider::StandardProvider

    StandardProvider::StandardProvider(const mfront::VariableDescription& v,
                                       const std::string& e,
                                       const bool b)
        : type(v.type), name(v.name), ename(e), asize(v.arraySize) {
      const auto st = SupportedTypes{};
      if (b) {
        if (!st.isSupportedType(this->type)) {
          throw(
              std::runtime_error("StandardProvider::StandardProvider : "
                                 "unsupported type '" +
                                 this->type + "'"));
        }
      }
    }  // end of StandardProvider::StandardProvider

    std::string StandardProvider::getVariableType() const { return this->type; }

    std::string StandardProvider::getExternalName() const {
      return this->ename;
    }

    unsigned short StandardProvider::getArraySize() const {
      return this->asize;
    }

    StandardProvider::~StandardProvider() = default;

    MaterialPropertyProvider::MaterialPropertyProvider(const std::string& t,
                                                       const std::string& n,
                                                       const std::string& e,
                                                       const unsigned short s)
        : StandardProvider(t, n, e, s, true) {
    }  // end of MaterialPropertyProvider::MaterialPropertyProvider

    MaterialPropertyProvider::MaterialPropertyProvider(
        const mfront::VariableDescription& v, const std::string& e)
        : StandardProvider(v, e, true) {
    }  // end of MaterialPropertyProvider::MaterialPropertyProvider

    ProviderIdentifier MaterialPropertyProvider::getIdentifier() const {
      return ProviderIdentifier::MATERIALPROPERTY;
    }  // end of MaterialPropertyProvider::getIdentifier

    MaterialPropertyProvider::~MaterialPropertyProvider() = default;

    // std::string
    // MaterialLawProvider::getVariableType(void) const{
    //   return this->type;
    // } // end of MaterialLawProvider::getVariableType

    // std::string
    // MaterialLawProvider::getExternalName(void) const{
    //   return this->name;
    // }

    // unsigned short
    // MaterialLawProvider::getArraySize(void) const{
    //   return 1u;
    // }

    // ProviderIdentifier
    // MaterialLawProvider::getIdentifier(void) const{
    //   return ProviderIdentifier::MATERIALLAW;
    // } // end of MaterialLawProvider::getIdentifier

    // MaterialLawProvider::~MaterialLawProvider() = default;

    AuxiliaryStateVariableProvider::AuxiliaryStateVariableProvider(
        const std::string& t,
        const std::string& n,
        const std::string& e,
        const unsigned short s)
        : StandardProvider(t, n, e, s, true) {
    }  // end of AuxiliaryStateVariableProvider::AuxiliaryStateVariableProvider

    AuxiliaryStateVariableProvider::AuxiliaryStateVariableProvider(
        const mfront::VariableDescription& v, const std::string& e)
        : StandardProvider(v, e, true) {
    }  // end of AuxiliaryStateVariableProvider::AuxiliaryStateVariableProvider

    ProviderIdentifier AuxiliaryStateVariableProvider::getIdentifier() const {
      return ProviderIdentifier::AUXILIARYSTATEVARIABLE;
    }  // end of AuxiliaryStateVariableProvider::getIdentifier

    AuxiliaryStateVariableProvider::~AuxiliaryStateVariableProvider() = default;

    ExternalStateVariableProvider::ExternalStateVariableProvider(
        const std::string& t,
        const std::string& n,
        const std::string& e,
        const unsigned short s)
        : StandardProvider(t, n, e, s, true) {
    }  // end of ExternalStateVariableProvider::ExternalStateVariableProvider

    ExternalStateVariableProvider::ExternalStateVariableProvider(
        const mfront::VariableDescription& v, const std::string& e)
        : StandardProvider(v, e, true) {
    }  // end of ExternalStateVariableProvider::ExternalStateVariableProvider

    ProviderIdentifier ExternalStateVariableProvider::getIdentifier() const {
      return ProviderIdentifier::EXTERNALSTATEVARIABLE;
    }  // end of ExternalStateVariableProvider::getIdentifier

    ExternalStateVariableProvider::~ExternalStateVariableProvider() = default;

    IntegrationVariableProvider::IntegrationVariableProvider(
        const std::string& t,
        const std::string& n,
        const std::string& e,
        const unsigned short s)
        : StandardProvider(t, n, e, s, true) {
    }  // end of IntegrationVariableProvider::IntegrationVariableProvider

    IntegrationVariableProvider::IntegrationVariableProvider(
        const mfront::VariableDescription& v, const std::string& e)
        : StandardProvider(v, e, true) {
    }  // end of IntegrationVariableProvider::IntegrationVariableProvider

    ProviderIdentifier IntegrationVariableProvider::getIdentifier() const {
      return ProviderIdentifier::INTEGRATIONVARIABLE;
    }  // end of IntegrationVariableProvider::getIdentifier

    IntegrationVariableProvider::~IntegrationVariableProvider() = default;

    LocalVariableProvider::LocalVariableProvider(const std::string& t,
                                                 const std::string& n,
                                                 const std::string& e,
                                                 const unsigned short s)
        : StandardProvider(t, n, e, s, false) {
    }  // end of LocalVariableProvider::LocalVariableProvider

    LocalVariableProvider::LocalVariableProvider(
        const mfront::VariableDescription& v, const std::string& e)
        : StandardProvider(v, e, false) {
    }  // end of LocalVariableProvider::LocalVariableProvider

    ProviderIdentifier LocalVariableProvider::getIdentifier() const {
      return ProviderIdentifier::LOCALVARIABLE;
    }  // end of LocalVariableProvider::getIdentifier

    LocalVariableProvider::~LocalVariableProvider() = default;

    StaticVariableProvider::StaticVariableProvider(const std::string& t,
                                                   const std::string& n,
                                                   const std::string& e)
        : type(t), name(n), ename(e) {
      const auto s = SupportedTypes{};
      if (!s.isSupportedType(this->type)) {
        throw(std::runtime_error(
            "StaticVariableProvider::StaticVariableProvider : "
            "unsupported type '" +
            this->type + "'"));
      }
    }  // end of StaticVariableProvider::StaticVariableProvider

    StaticVariableProvider::StaticVariableProvider(
        const mfront::StaticVariableDescription& v, const std::string& e)
        : type(v.type),
          name(v.name),
          ename(e) {}  // end of StaticVariableProvider::StaticVariableProvider

    std::string StaticVariableProvider::getVariableType() const {
      return this->type;
    }

    std::string StaticVariableProvider::getExternalName() const {
      return this->ename;
    }

    unsigned short StaticVariableProvider::getArraySize() const { return 1u; }

    ProviderIdentifier StaticVariableProvider::getIdentifier() const {
      return ProviderIdentifier::STATICVARIABLE;
    }  // end of StaticVariableProvider::getIdentifier

    StaticVariableProvider::~StaticVariableProvider() = default;

    ParameterProvider::ParameterProvider(const std::string& t,
                                         const std::string& n,
                                         const std::string& e)
        : type(t), name(n), ename(e) {
      const auto s = SupportedTypes{};
      if (!s.isSupportedType(this->type)) {
        throw(
            std::runtime_error("ParameterProvider::ParameterProvider : "
                               "unsupported type '" +
                               this->type + "'"));
      }
    }  // end of ParameterProvider::ParameterProvider

    ParameterProvider::ParameterProvider(const mfront::VariableDescription& v,
                                         const std::string& e)
        : type(v.type), name(v.name), ename(e) {
      const auto s = SupportedTypes{};
      if (!s.isSupportedType(this->type)) {
        throw(
            std::runtime_error("ParameterProvider::ParameterProvider : "
                               "unsupported type '" +
                               this->type + "'"));
      }
    }  // end of ParameterProvider::ParameterProvider

    std::string ParameterProvider::getVariableType() const {
      return this->type;
    }

    std::string ParameterProvider::getExternalName() const {
      return this->ename;
    }

    unsigned short ParameterProvider::getArraySize() const { return 1u; }

    ProviderIdentifier ParameterProvider::getIdentifier() const {
      return ProviderIdentifier::PARAMETER;
    }  // end of ParameterProvider::getIdentifier

    ParameterProvider::~ParameterProvider() = default;

  }  // end of namespace bbrick

}  // end of namespace mfront
