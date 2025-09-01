/*!
 * \file   mfront/src/VariableDescription.cxx
 *
 * \brief
 * \author Thomas Helfer
 * \date   17 Jan 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <sstream>
#include <stdexcept>
#include <algorithm>
#include "TFEL/Raise.hxx"
#include "TFEL/Glossary/Glossary.hxx"
#include "TFEL/Glossary/GlossaryEntry.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"
#include "MFront/MFrontUtilities.hxx"
#include "MFront/SupportedTypes.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MFront/VariableDescription.hxx"

namespace mfront {

  const char* const VariableDescription::depth = "depth";
  const char* const VariableDescription::initialValue = "initialValue";
  const char* const VariableDescription::defaultValue = "defaultValue";
  const char* const VariableDescription::errorNormalisationFactor =
      "errorNormalisationFactor";
  const char* const VariableDescription::variableDeclaredInBaseClass =
      "variableDeclaredInBaseClass";

  VariableDescription::VariableDescription() = default;

  VariableDescription::VariableDescription(const std::string& t,
                                           const std::string& n,
                                           const unsigned short as,
                                           const size_t l)
      : VariableDescriptionBase(t, n, as, l) {
  }  // end of VariableDescription::VariableDescription

  VariableDescription::VariableDescription(const std::string& t,
                                           const std::string& s,
                                           const std::string& n,
                                           const unsigned short as,
                                           const size_t l)
      : VariableDescriptionBase(t, s, n, as, l) {
  }  // end of VariableDescription::VariableDescription

  VariableDescription::VariableDescription(const VariableDescription&) =
      default;
  VariableDescription::VariableDescription(VariableDescription&&) = default;
  VariableDescription& VariableDescription::operator=(VariableDescription&&) =
      default;
  VariableDescription& VariableDescription::operator=(
      const VariableDescription&) = default;

  bool VariableDescription::isScalar() const {
    return SupportedTypes::isScalarType(this->type);
  }  // end of isScalar

  int VariableDescription::getVariableTypeIdentifier() const {
    return SupportedTypes::getTypeIdentifier(this->type);
  }  // end of getVariableTypeIdentifier

  void VariableDescription::setGlossaryName(const std::string& g) {
    using tfel::glossary::Glossary;
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(b, "VariableDescription::setGlossaryName: " + m);
    };
    const auto& glossary = Glossary::getGlossary();
    throw_if(!glossary.contains(g), "'" + g + "' is not a glossary name");
    throw_if(this->hasGlossaryName(),
             "the glossary name has already been defined "
             "for variable '" +
                 this->name + "'");
    throw_if(this->hasEntryName(),
             "an entry name has already been defined "
             "for variable '" +
                 this->name + "'");
    this->glossaryName = glossary.getGlossaryEntry(g).getKey();
  }  // end of VariableDescription::setGlossaryName

  void VariableDescription::resetGlossaryName(const std::string& g) {
    using tfel::glossary::Glossary;
    const auto& glossary = Glossary::getGlossary();
    if (!glossary.contains(g)) {
      tfel::raise("VariableDescription::resetGlossaryName: '" + g +
                  "' is not a glossary name");
    }
    this->glossaryName.reset();
    this->entryName.reset();
    this->setGlossaryName(g);
  }  // end of resetGlossaryName

  void VariableDescription::setEntryName(const std::string& e) {
    using tfel::glossary::Glossary;
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(b, "VariableDescription::setEntryName: " + m);
    };
    const auto& glossary = Glossary::getGlossary();
    throw_if(glossary.contains(e), "'" + e + "' is a glossary name");
    throw_if(this->hasGlossaryName(),
             "the glossary name has already been defined "
             "for variable '" +
                 this->name + "'");
    throw_if(this->hasEntryName(),
             "an entry name has already been defined "
             "for variable '" +
                 this->name + "'");
    this->entryName = e;
  }  // end of VariableDescription::setEntryName

  void VariableDescription::resetEntryName(const std::string& g) {
    this->glossaryName.reset();
    this->entryName.reset();
    this->setEntryName(g);
  }  // end of resetEntryName

  bool VariableDescription::hasGlossaryName() const {
    return this->glossaryName.has_value();
  }

  bool VariableDescription::hasEntryName() const {
    return this->entryName.has_value();
  }

  const std::string& VariableDescription::getExternalName() const {
    if (this->hasGlossaryName()) {
      return this->glossaryName.value();
    }
    if (this->hasEntryName()) {
      return this->entryName.value();
    }
    return this->name;
  }  // end of VariableDescription::getExternalName

  void VariableDescription::throwUndefinedAttribute(const std::string& n) {
    tfel::raise(
        "VariableDescription::getAttribute : "
        "no attribute named '" +
        n + "'");
  }  // end of VariableDescription::throwUndefinedAttribute

  void VariableDescription::setAttribute(const std::string& n,
                                         const VariableAttribute& a,
                                         const bool b) {
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(c, "VariableDescription::setAttribute: " + m);
    };
    auto p = this->attributes.find(n);
    if (p != this->attributes.end()) {
      throw_if(a.getTypeIndex() != p->second.getTypeIndex(),
               "attribute already exists with a different type");
    }
    if (!this->attributes.insert({n, a}).second) {
      throw_if(!b, "attribute '" + n + "' already declared");
    }
  }  // end of VariableDescription::setAttribute

  bool VariableDescription::hasAttribute(const std::string& n) const {
    return this->attributes.count(n) != 0u;
  }  // end of VariableDescription::hasAttribute

  const std::map<std::string, VariableAttribute>&
  VariableDescription::getAttributes() const {
    return this->attributes;
  }  // end of VariableDescription::getAttributes

  static void checkBoundsCompatibility(const VariableDescription& v,
                                       const VariableBoundsDescription& b) {
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(c, "mfront::checkBoundsCompatibility: " + m);
    };
    throw_if(!SupportedTypes::hasTypeFlag(v.type),
             "can't assign bounds to variable '" + v.name +
                 "' (invalid type '" + v.type + "')");
    if ((b.component != -1) && (b.component != 0)) {
      const auto f = SupportedTypes::getTypeFlag(v.type);
      throw_if(f != SupportedTypes::SCALAR,
               "invalid component value for a scalar "
               "(" +
                   std::to_string(b.component) + ")");
    }
    if (b.boundsType == VariableBoundsDescription::LOWERANDUPPER) {
      throw_if(b.lowerBound > b.upperBound, "invalid bounds value");
    }
  }  // end of checkBoundsCompatibility

  /*!
   * \param[in] b1: standard bounds
   * \param[in] b2: physical bounds
   */
  static void checkBoundsCompatibility(const VariableBoundsDescription& b1,
                                       const VariableBoundsDescription& b2,
                                       const std::string& n) {
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(c, "mfront::checkBoundsCompatibility: " + m);
    };
    throw_if((!b1.hasLowerBound()) && (b2.hasLowerBound()),
             "standard bounds for variable '" + n +
                 "' has no lower bounds "
                 "whereas the physical bounds has. This is inconsistent.");
    if (b1.hasLowerBound()) {
      throw_if(b1.lowerBound < b2.lowerBound, "standard bounds for variable '" +
                                                  n +
                                                  "' are not contained  "
                                                  "in the physical bounds.");
    }
    throw_if((!b1.hasUpperBound()) && (b2.hasUpperBound()),
             "standard bounds for variable '" + n +
                 "' has no lower bounds "
                 "whereas the physical bounds has. This is inconsistent.");
    if (b1.hasUpperBound()) {
      throw_if(b1.upperBound > b2.upperBound, "standard bounds for variable '" +
                                                  n +
                                                  "' are not contained  "
                                                  "in the physical bounds.");
    }
  }  // end of checkBoundsCompatibility

  bool VariableDescription::hasBounds() const {
    return std::holds_alternative<VariableBoundsDescription>(this->bounds);
  }  // end of VariableDescription::hasBounds

  bool VariableDescription::hasBounds(const unsigned short i) const {
    tfel::raise_if(this->arraySize == 1u,
                   "VariableDescription::hasBounds: "
                   "invalid call on scalar variable '" +
                       this->name + "'");
    if (std::holds_alternative<VariableBoundsDescription>(this->bounds)) {
      return true;
    } else if (std::holds_alternative<
                   std::map<unsigned short, VariableBoundsDescription>>(
                   this->bounds)) {
      const auto& m =
          std::get<std::map<unsigned short, VariableBoundsDescription>>(
              this->bounds);
      return m.count(i) != 0;
    }
    return false;
  }  // end of VariableDescription::hasBounds

  void VariableDescription::setBounds(const VariableBoundsDescription& b) {
    tfel::raise_if(!std::holds_alternative<std::monostate>(this->bounds),
                   "VariableDescription::setBounds: "
                   "bounds have already been set on variable "
                   "'" +
                       this->name + "'");
    mfront::checkBoundsCompatibility(*this, b);
    if (this->arraySize == 1u) {
      if (this->hasPhysicalBounds()) {
        mfront::checkBoundsCompatibility(b, this->getPhysicalBounds(),
                                         this->name);
      }
    } else {
      for (unsigned short i = 0; i != this->arraySize; ++i) {
        if (this->hasPhysicalBounds(i)) {
          mfront::checkBoundsCompatibility(b, this->getPhysicalBounds(i),
                                           this->name);
        }
      }
    }
    this->bounds = b;
  }  // end of VariableDescription::setBounds

  void VariableDescription::setBounds(const VariableBoundsDescription& b,
                                      const unsigned short i) {
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(c, "VariableDescription::setBounds: " + m);
    };
    throw_if(this->arraySize == 1u, "invalid call on scalar variable");
    throw_if(i > this->arraySize, "invalid index");
    throw_if(this->hasBounds(),
             "bounds have already been set on variable "
             "'" +
                 this->name + "'");
    if (this->hasBounds(i)) {
      tfel::raise_if(this->hasBounds(i),
                     "VariableDescription::setBounds: "
                     "bounds have already been set on variable "
                     "'" +
                         this->name + "'");
    }
    for (unsigned short j = 0; j != this->arraySize; ++j) {
      if (this->hasPhysicalBounds(j)) {
        mfront::checkBoundsCompatibility(b, this->getPhysicalBounds(j),
                                         this->name);
      }
    }
    if (std::holds_alternative<std::monostate>(this->bounds)) {
      this->bounds = std::map<unsigned short, VariableBoundsDescription>();
    }
    mfront::checkBoundsCompatibility(*this, b);
    if (this->hasPhysicalBounds(i)) {
      mfront::checkBoundsCompatibility(b, this->getPhysicalBounds(i),
                                       this->name);
    }
    auto& m = std::get<std::map<unsigned short, VariableBoundsDescription>>(
        this->bounds);
    m.insert({i, b});
  }  // end of VariableDescription::setBounds

  const VariableBoundsDescription& VariableDescription::getBounds() const {
    tfel::raise_if(!this->hasBounds(),
                   "VariableDescription::getBounds: "
                   "no bounds set on variable "
                   "'" +
                       this->name + "'");
    return std::get<VariableBoundsDescription>(this->bounds);
  }  // end of VariableDescription::getBounds

  const VariableBoundsDescription& VariableDescription::getBounds(
      const unsigned short i) const {
    tfel::raise_if(!this->hasBounds(i),
                   "VariableDescription::getBounds: "
                   "no bounds set on variable "
                   "'" +
                       this->name + "'");
    if (std::holds_alternative<VariableBoundsDescription>(this->bounds)) {
      return std::get<VariableBoundsDescription>(this->bounds);
    }
    const auto& m =
        std::get<std::map<unsigned short, VariableBoundsDescription>>(
            this->bounds);
    return m.at(i);
  }  // end of VariableDescription::getBounds

  bool VariableDescription::hasPhysicalBounds() const {
    return std::holds_alternative<VariableBoundsDescription>(
        this->physicalBounds);
  }  // end of VariableDescription::hasPhysicalBounds

  bool VariableDescription::hasPhysicalBounds(const unsigned short i) const {
    tfel::raise_if(this->arraySize == 1u,
                   "VariableDescription::hasPhysicalBounds: "
                   "invalid call on scalar variable '" +
                       this->name + "'");
    if (std::holds_alternative<VariableBoundsDescription>(
            this->physicalBounds)) {
      return true;
    } else if (std::holds_alternative<
                   std::map<unsigned short, VariableBoundsDescription>>(
                   this->physicalBounds)) {
      const auto& m =
          std::get<std::map<unsigned short, VariableBoundsDescription>>(
              this->physicalBounds);
      return m.count(i) != 0;
    }
    return false;
  }  // end of VariableDescription::hasPhysicalBounds

  void VariableDescription::setPhysicalBounds(
      const VariableBoundsDescription& b) {
    tfel::raise_if(
        !std::holds_alternative<std::monostate>(this->physicalBounds),
        "VariableDescription::setPhysicalBounds: "
        "bounds have already been set on variable "
        "'" +
            this->name + "'");
    mfront::checkBoundsCompatibility(*this, b);
    if (this->arraySize == 1u) {
      if (this->hasBounds()) {
        mfront::checkBoundsCompatibility(this->getBounds(), b, this->name);
      }
    } else {
      for (unsigned short i = 0; i != this->arraySize; ++i) {
        if (this->hasBounds(i)) {
          mfront::checkBoundsCompatibility(this->getBounds(i), b, this->name);
        }
      }
    }
    this->physicalBounds = b;
  }  // end of VariableDescription::setPhysicalBounds

  void VariableDescription::setPhysicalBounds(
      const VariableBoundsDescription& b, const unsigned short i) {
    auto throw_if = [](const bool c, const std::string& m) {
      if (c) {
        tfel::raise("VariableDescription::setPhysicalBounds: " + m);
      }
    };
    throw_if(this->arraySize == 1u, "invalid call on scalar variable");
    throw_if(i > this->arraySize, "invalid index");
    throw_if(
        std::holds_alternative<VariableBoundsDescription>(this->physicalBounds),
        "physical bounds have already been set on variable "
        "'" +
            this->name + "'");
    tfel::raise_if(this->hasPhysicalBounds(i),
                   "VariableDescription::setPhysicalBounds: "
                   "bounds have already been set on variable "
                   "'" +
                       this->name + "'");
    mfront::checkBoundsCompatibility(*this, b);
    for (unsigned short j = 0; j != this->arraySize; ++j) {
      if (this->hasBounds(j)) {
        mfront::checkBoundsCompatibility(this->getBounds(j), b, this->name);
      }
    }
    if (std::holds_alternative<std::monostate>(this->physicalBounds)) {
      this->physicalBounds =
          std::map<unsigned short, VariableBoundsDescription>();
    }
    auto& m = std::get<std::map<unsigned short, VariableBoundsDescription>>(
        this->physicalBounds);
    m.insert({i, b});
  }  // end of VariableDescription::setPhysicalBounds

  const VariableBoundsDescription& VariableDescription::getPhysicalBounds()
      const {
    tfel::raise_if(!this->hasPhysicalBounds(),
                   "VariableDescription::getPhysicalBounds: "
                   "no bounds set on variable "
                   "'" +
                       this->name + "'");
    return std::get<VariableBoundsDescription>(this->physicalBounds);
  }  // end of VariableDescription::getPhysicalBounds

  const VariableBoundsDescription& VariableDescription::getPhysicalBounds(
      const unsigned short i) const {
    tfel::raise_if(!this->hasPhysicalBounds(i),
                   "VariableDescription::getPhysicalBounds: "
                   "no bounds set on variable "
                   "'" +
                       this->name + "'");
    if (std::holds_alternative<VariableBoundsDescription>(
            this->physicalBounds)) {
      return std::get<VariableBoundsDescription>(this->physicalBounds);
    }
    const auto& m =
        std::get<std::map<unsigned short, VariableBoundsDescription>>(
            this->physicalBounds);
    return m.at(i);
  }  // end of VariableDescription::getBounds

  VariableDescription::~VariableDescription() = default;

  SupportedTypes::TypeSize getTypeSize(const VariableDescription& v) {
    return v.getTypeSize();
  }  // end of getTypeSize

  bool hasBounds(const VariableDescription& v) {
    return v.hasBounds();
  }  // end of hasBounds

  bool hasBounds(const VariableDescription& v, const unsigned short i) {
    return v.hasBounds(i);
  }  // end of hasBounds

  bool hasPhysicalBounds(const VariableDescription& v) {
    return v.hasPhysicalBounds();
  }  // end of hasPhysicalBounds

  bool hasPhysicalBounds(const VariableDescription& v, const unsigned short i) {
    return v.hasPhysicalBounds(i);
  }  // end of hasPhysicalBounds

  void getSymbol(std::map<std::string, std::string>& symbols,
                 const VariableDescription& v) {
    if (!v.symbolic_form.empty()) {
      addSymbol(symbols, v.symbolic_form, v.name);
    }
  }  // end of getSymbol

  void getIncrementSymbol(std::map<std::string, std::string>& symbols,
                          const VariableDescription& v) {
    addSymbol(symbols, "\u0394" + displayName(v), "d" + v.name);
  }  // end of getIncrementSymbol

  void getTimeDerivativeSymbol(std::map<std::string, std::string>& symbols,
                               const VariableDescription& v) {
    addSymbol(symbols, "d\u209C" + displayName(v), "d" + v.name);
    addSymbol(symbols, "\u2202\u209C" + displayName(v), "d" + v.name);
    addSymbol(symbols, "\u2202" + displayName(v) + "\u2215\u2202t",
              "d" + v.name);
  }  // end of getIncrementSymbol

  void checkAndCompletePhysicalBoundsDeclaration(VariableDescription& v,
                                                 const std::string_view s) {
    using tfel::glossary::Glossary;
    if (!v.hasGlossaryName()) {
      return;
    }
    const auto& glossary = Glossary::getGlossary();
    const auto& e = glossary.getGlossaryEntry(v.getExternalName());
    if ((!e.hasLowerPhysicalBound(s)) && (!e.hasUpperPhysicalBound(s))) {
      return;
    }
    if (v.hasPhysicalBounds()) {
      // check
      const auto& bounds = v.getPhysicalBounds();
      if ((e.hasLowerPhysicalBound(s)) && (bounds.hasLowerBound())) {
        const auto b1 = bounds.lowerBound;
        const auto b2 =
            tfel::utilities::convert<long double>(e.getLowerPhysicalBound(s));
        if (b1 < b2) {
          auto& os = getLogStream();
          os << "checkAndCompletePhysicalBoundsDeclaration: "
             << "lower bound for variable '" << v.name
             << "' is below the lower bound of the associated glossary entry ("
             << b1 << " < " << b2 << ")";
        }
      }
      if ((e.hasUpperPhysicalBound(s)) && (bounds.hasUpperBound())) {
        const auto b1 = bounds.upperBound;
        const auto b2 =
            tfel::utilities::convert<long double>(e.getUpperPhysicalBound(s));
        if (b1 > b2) {
          auto& os = getLogStream();
          os << "checkAndCompletePhysicalBoundsDeclaration: "
             << "upper bound for variable '" << v.name
             << "' is greater than the lower bound of the associated glossary "
             << "entry (" << b1 << " > " << b2 << ")";
        }
      }
    } else {
      // complete
      if ((e.hasLowerPhysicalBound(s)) && (e.hasUpperPhysicalBound(s))) {
        auto b = VariableBoundsDescription{};
        b.boundsType = VariableBoundsDescription::LOWERANDUPPER;
        b.lowerBound =
            tfel::utilities::convert<long double>(e.getLowerPhysicalBound(s));
        b.upperBound =
            tfel::utilities::convert<long double>(e.getUpperPhysicalBound(s));
        v.setPhysicalBounds(b);
      } else if (e.hasLowerPhysicalBound(s)) {
        auto b = VariableBoundsDescription{};
        b.boundsType = VariableBoundsDescription::LOWER;
        b.lowerBound =
            tfel::utilities::convert<long double>(e.getLowerPhysicalBound(s));
        v.setPhysicalBounds(b);
      } else if (e.hasUpperPhysicalBound(s)) {
        auto b = VariableBoundsDescription{};
        b.boundsType = VariableBoundsDescription::UPPER;
        b.upperBound =
            tfel::utilities::convert<long double>(e.getUpperPhysicalBound(s));
        v.setPhysicalBounds(b);
      }
    }
  }  // end of checkAndCompletePhysicalBoundsDeclaration

  VariableDescriptionContainer::VariableDescriptionContainer() = default;

  VariableDescriptionContainer::VariableDescriptionContainer(
      const std::initializer_list<VariableDescription>& l)
      : std::vector<VariableDescription>(l) {}

  VariableDescriptionContainer::VariableDescriptionContainer(
      VariableDescriptionContainer&&) = default;
  VariableDescriptionContainer::VariableDescriptionContainer(
      const VariableDescriptionContainer&) = default;
  VariableDescriptionContainer& VariableDescriptionContainer::operator=(
      VariableDescriptionContainer&&) = default;
  VariableDescriptionContainer& VariableDescriptionContainer::operator=(
      const VariableDescriptionContainer&) = default;

  bool VariableDescriptionContainer::contains(const std::string& n) const {
    for (const auto& v : *this) {
      if (v.name == n) {
        return true;
      }
    }
    return false;
  }  // end of VariableDescriptionContainer::contains

  std::vector<std::string> VariableDescriptionContainer::getExternalNames()
      const {
    std::vector<std::string> n;
    this->appendExternalNames(n);
    return n;
  }

  void VariableDescriptionContainer::getExternalNames(
      std::vector<std::string>& n) const {
    n.clear();
    this->appendExternalNames(n);
  }

  void VariableDescriptionContainer::appendExternalNames(
      std::vector<std::string>& n) const {
    for (const auto& v : *this) {
      const auto name = v.getExternalName();
      if (v.arraySize == 1u) {
        n.push_back(name);
      } else {
        for (unsigned short i = 0; i != v.arraySize; ++i) {
          std::ostringstream nb;
          nb << '[' << i << ']';
          n.push_back(name + nb.str());
        }
      }
    }
  }  // end of VariableDescriptionContainer::appendExternalNames

  VariableDescription& VariableDescriptionContainer::getVariable(
      const std::string& n) {
    for (auto& v : *this) {
      if (v.name == n) {
        return v;
      }
    }
    tfel::raise(
        "VariableDescriptionContainer::getVariable : "
        "no variable named '" +
        n + "'");
  }

  const VariableDescription&
  VariableDescriptionContainer::getVariableByExternalName(
      const std::string& n) const {
    auto p = findByExternalName(*this, n);
    if (p == this->end()) {
      tfel::raise(
          "VariableDescriptionContainer::getVariableByExternalName : "
          "no variable with external name '" +
          n + "' found");
    }
    return *p;
  }  // end of VariableDescriptionContainer::getVariableByExternalName

  const VariableDescription& VariableDescriptionContainer::getVariable(
      const std::string& n) const {
    for (const auto& v : *this) {
      if (v.name == n) {
        return v;
      }
    }
    tfel::raise(
        "VariableDescriptionContainer::getVariable : "
        "no variable named '" +
        n + "'");
  }

  SupportedTypes::TypeSize VariableDescriptionContainer::getTypeSize() const {
    auto s = SupportedTypes::TypeSize{};
    for (const auto& v : *this) {
      s += v.getTypeSize();
    }
    return s;
  }

  unsigned short VariableDescriptionContainer::getNumberOfVariables() const {
    size_t n = 0u;
    for (const auto& v : *this) {
      n = n + v.arraySize;
    }
    return n;
  }  // end of SupportedTypes::getNumberOfVariables

  VariableDescriptionContainer::~VariableDescriptionContainer() = default;

  VariableDescriptionContainer::const_iterator findByExternalName(
      const VariableDescriptionContainer& c, const std::string& n) {
    return std::find_if(c.begin(), c.end(), [&n](const VariableDescription& v) {
      return v.getExternalName() == n;
    });
  }  // end of findByExternalName

  SupportedTypes::TypeSize getTypeSize(const VariableDescriptionContainer& c) {
    return c.getTypeSize();
  }  // end of getTypeSize

  bool hasBounds(const VariableDescriptionContainer& c) {
    for (const auto& v : c) {
      if (v.arraySize == 1u) {
        if (v.hasBounds()) {
          return true;
        }
      } else {
        for (unsigned short i = 0; i != v.arraySize; ++i) {
          if (v.hasBounds(i)) {
            return true;
          }
        }
      }
    }
    return false;
  }  // end of hasBounds

  bool hasPhysicalBounds(const VariableDescriptionContainer& c) {
    for (const auto& v : c) {
      if (v.arraySize == 1u) {
        if (v.hasPhysicalBounds()) {
          return true;
        }
      } else {
        for (unsigned short i = 0; i != v.arraySize; ++i) {
          if (v.hasPhysicalBounds(i)) {
            return true;
          }
        }
      }
    }
    return false;
  }  // end of hasPhysicalBounds

  void getSymbols(std::map<std::string, std::string>& symbols,
                  const VariableDescriptionContainer& c) {
    for (const auto& v : c) {
      getSymbol(symbols, v);
    }
  }  // end of getSymbols

  void getIncrementSymbols(std::map<std::string, std::string>& symbols,
                           const VariableDescriptionContainer& c) {
    for (const auto& v : c) {
      getIncrementSymbol(symbols, v);
    }
  }  // end of getIncrementSymbols

  void getTimeDerivativeSymbols(std::map<std::string, std::string>& symbols,
                                const VariableDescriptionContainer& c) {
    for (const auto& v : c) {
      getTimeDerivativeSymbol(symbols, v);
    }
  }  // end of getTimeDerivativeSymbols

  SupportedTypes::TypeSize getOffset(const VariableDescriptionContainer& c,
                                     const std::string& n) {
    auto o = SupportedTypes::TypeSize{};
    for (const auto& v : c) {
      if (v.name == n) {
        return o;
      }
      o += getTypeSize(v);
    }
    tfel::raise("mfront::getOffset: no variable name '" + n + "'");
  }  // end of getOffset

  void checkAndCompletePhysicalBoundsDeclaration(
      VariableDescriptionContainer& variables, const std::string_view s) {
    for (auto& v : variables) {
      checkAndCompletePhysicalBoundsDeclaration(v, s);
    }
  }  // end of checkAndCompletePhysicalBoundsDeclaration

}  // end of namespace mfront
