/*!
 * \file   BehaviourMaterialProperty.cxx
 * \brief
 * \author Thomas Helfer
 * \date   08/07/2018
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <ostream>
#include "TFEL/Raise.hxx"
#include "TFEL/Glossary/Glossary.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MFront/BehaviourDescription.hxx"
#include "MFront/BehaviourMaterialProperty.hxx"

namespace mfront {

  BehaviourMaterialProperty::BehaviourMaterialProperty(
      const VariableDescription& v,
      const SupportedTypes::TypeSize o,
      const bool d)
      : VariableDescription(v),
        offset(o),
        dummy(d) {
  }  // end olf BehaviourMaterialProperty::BehaviourMaterialProperty

  BehaviourMaterialProperty::BehaviourMaterialProperty(
      const BehaviourMaterialProperty&) = default;
  BehaviourMaterialProperty::BehaviourMaterialProperty(
      BehaviourMaterialProperty&&) = default;
  BehaviourMaterialProperty& BehaviourMaterialProperty::operator=(
      const BehaviourMaterialProperty&) = default;
  BehaviourMaterialProperty& BehaviourMaterialProperty::operator=(
      BehaviourMaterialProperty&&) = default;

  bool BehaviourMaterialProperty::isScalar() const {
    return SupportedTypes::isScalarType(this->type);
  }  // end of isScalar

  BehaviourMaterialProperty::~BehaviourMaterialProperty() = default;

  const BehaviourMaterialProperty& findBehaviourMaterialProperty(
      const std::vector<BehaviourMaterialProperty>& mprops,
      const std::string& n) {
    for (const auto& m : mprops) {
      if ((m.getExternalName() == n) && (!m.dummy)) {
        return m;
      }
    }
    tfel::raise(
        "findBehaviourMaterialProperty: "
        "no material property associated with the "
        "glossary name '" +
        n + "'");
  }  // end of findBehaviourMaterialProperty

  std::pair<std::vector<BehaviourMaterialProperty>, SupportedTypes::TypeSize>
  buildMaterialPropertiesList(
      const BehaviourDescription& bd,
      const std::set<tfel::material::ModellingHypothesis::Hypothesis>& mh,
      const BuildMaterialPropertiesListOptions& opts) {
    constexpr auto h =
        BehaviourMaterialProperty::ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    std::set<BehaviourMaterialProperty::Hypothesis> uh;
    for (const auto& lh : mh) {
      if (!bd.hasSpecialisedMechanicalData(lh)) {
        uh.insert(lh);
      }
    }
    if (uh.empty()) {
      tfel::raise(
          "internal error : the mechanical behaviour says that not "
          "all handled mechanical data are specialised, but we found none.");
    }
    // material properties for all the selected hypothesis
    auto mpositions =
        std::vector<std::pair<std::vector<BehaviourMaterialProperty>,
                              SupportedTypes::TypeSize>>{};
    for (const auto& lh : uh) {
      mpositions.push_back(buildMaterialPropertiesList(bd, lh, opts));
    }
    auto ph = uh.begin();
    auto pum = mpositions.begin();
    const auto& mfirst = *pum;
    ++ph;
    ++pum;
    for (; ph != uh.end(); ++ph, ++pum) {
      const auto& d = bd.getBehaviourData(h);
      const auto& mps = d.getMaterialProperties();
      for (const auto& mp : mps) {
        const auto& mp1 = findBehaviourMaterialProperty(
            mfirst.first, bd.getExternalName(h, mp.name));
        const auto& mp2 = findBehaviourMaterialProperty(
            pum->first, bd.getExternalName(h, mp.name));
        auto o1 = mp1.offset;
        o1 += pum->second;
        auto o2 = mp2.offset;
        o2 += mfirst.second;
        if (o1 != o2) {
          tfel::raise("incompatible offset for material property '" + mp.name +
                      "' (aka '" + mp1.getExternalName() +
                      "'). This is one pitfall of the umat interface. "
                      "To by-pass this limitation, you may want to explicitely "
                      "specialise some modelling hypotheses");
        }
      }
    }
    return mfirst;
  }  // end of buildMaterialProperties

  std::pair<std::vector<BehaviourMaterialProperty>, SupportedTypes::TypeSize>
  buildMaterialPropertiesList(
      const BehaviourDescription& bd,
      const tfel::material::ModellingHypothesis::Hypothesis h,
      const BuildMaterialPropertiesListOptions& opts) {
    using ModellingHypothesis = BehaviourMaterialProperty::ModellingHypothesis;
    auto raise = [](const std::string& m) {
      tfel::raise("buildMaterialPropertiesList: " + m);
    };
    auto throw_if = [&raise](const bool b, const char* const m) {
      if (b) {
        raise(m);
      }
    };
    throw_if(h == ModellingHypothesis::UNDEFINEDHYPOTHESIS,
             "buildMaterialPropertiesList: invalid hypotheis");
    auto res = std::pair<std::vector<BehaviourMaterialProperty>,
                         SupportedTypes::TypeSize>{};
    auto& mprops = res.first;
    auto add_mp = [&mprops](const char* const type, const char* const gn,
                            const char* const vn, const bool b) {
      appendToMaterialPropertiesList(mprops, type, gn, vn, b);
    };
    if ((h != ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN) &&
        (h != ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS) &&
        (h != ModellingHypothesis::GENERALISEDPLANESTRAIN) &&
        (h != ModellingHypothesis::AXISYMMETRICAL) &&
        (h != ModellingHypothesis::PLANESTRAIN) &&
        (h != ModellingHypothesis::PLANESTRESS) &&
        (h != ModellingHypothesis::TRIDIMENSIONAL)) {
      if (h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
        raise("unexepected undefined hypothesis");
      } else {
        raise("unexepected hypothesis '" + ModellingHypothesis::toString(h) +
              "'");
      }
    }
    if (opts.useMaterialPropertiesToBuildStiffnessTensor) {
      if (bd.getAttribute(BehaviourDescription::requiresStiffnessTensor,
                          false)) {
        if (bd.getSymmetryType() == mfront::ISOTROPIC) {
          add_mp("stress", "YoungModulus", "youn", false);
          add_mp("real", "PoissonRatio", "nu", false);
        } else if (bd.getSymmetryType() == mfront::ORTHOTROPIC) {
          add_mp("stress", "YoungModulus1", "yg1", false);
          add_mp("stress", "YoungModulus2", "yg2", false);
          add_mp("stress", "YoungModulus3", "yg3", false);
          add_mp("real", "PoissonRatio12", "nu12", false);
          add_mp("real", "PoissonRatio23", "nu23", false);
          add_mp("real", "PoissonRatio13", "nu13", false);
          if ((h == ModellingHypothesis::GENERALISEDPLANESTRAIN) ||
              (h == ModellingHypothesis::AXISYMMETRICAL) ||
              (h == ModellingHypothesis::PLANESTRAIN) ||
              (h == ModellingHypothesis::PLANESTRESS)) {
            add_mp("stress", "ShearModulus12", "g12", false);
          } else if (h == ModellingHypothesis::TRIDIMENSIONAL) {
            add_mp("stress", "ShearModulus12", "g12", false);
            add_mp("stress", "ShearModulus23", "g23", false);
            add_mp("stress", "ShearModulus13", "g13", false);
          }
        } else {
          throw_if(true, "unsupported behaviour symmetry type");
        }
      }
    }
    if (opts.useMaterialPropertiesToBuildThermalExpansionCoefficientTensor) {
      if (bd.getAttribute(
              BehaviourDescription::requiresThermalExpansionCoefficientTensor,
              false)) {
        if (bd.getSymmetryType() == mfront::ISOTROPIC) {
          add_mp("thermalexpansion", "ThermalExpansion", "alph", false);
        } else if (bd.getSymmetryType() == mfront::ORTHOTROPIC) {
          add_mp("thermalexpansion", "ThermalExpansion1", "alp1", false);
          add_mp("thermalexpansion", "ThermalExpansion2", "alp2", false);
          add_mp("thermalexpansion", "ThermalExpansion3", "alp3", false);
        } else {
          throw_if(true, "unsupported behaviour symmetry type");
        }
      }
    }
    if (!mprops.empty()) {
      const auto& m = mprops.back();
      res.second = m.offset;
      res.second += SupportedTypes::getTypeSize(m.type, m.arraySize);
    }
    completeMaterialPropertiesList(mprops, bd, h);
    return res;
  }  // end of buildMaterialPropertiesList

  void appendToMaterialPropertiesList(std::vector<BehaviourMaterialProperty>& l,
                                      const std::string& t,
                                      const std::string& n,
                                      const std::string& v,
                                      const bool b) {
    const auto flag = SupportedTypes::getTypeFlag(t);
    const auto& g = tfel::glossary::Glossary::getGlossary();
    tfel::raise_if(flag != SupportedTypes::SCALAR,
                   "appendToMaterialPropertiesList: "
                   "material properties shall be scalars");
    auto o = SupportedTypes::TypeSize{};
    if (!l.empty()) {
      const auto& m = l.back();
      o = m.offset;
      o += SupportedTypes::getTypeSize(t, 1u);
    }
    auto vd = VariableDescription{t, v, 1u, 0u};
    if (g.contains(n)) {
      vd.setGlossaryName(n);
    } else {
      vd.setEntryName(n);
    }
    l.emplace_back(vd, o, b);
  }  // end of appendToMaterialPropertiesList

  void completeMaterialPropertiesList(
      std::vector<BehaviourMaterialProperty>& mprops,
      const BehaviourDescription& mb,
      const BehaviourMaterialProperty::Hypothesis h) {
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(b, "completeMaterialPropertiesList: " + m);
    };
    const auto& d = mb.getBehaviourData(h);
    const auto& mp = d.getMaterialProperties();
    for (auto p = mp.begin(); p != mp.end(); ++p) {
      const auto& n = mb.getExternalName(h, p->name);
      bool found = false;
      const auto flag = SupportedTypes::getTypeFlag(p->type);
      throw_if(flag != SupportedTypes::SCALAR,
               "Invalid type for material property '" + p->name + "' (" +
                   p->type +
                   ").\n"
                   "Material properties shall be scalars");
      for (auto pum = mprops.begin(); (pum != mprops.end()) && (!found);
           ++pum) {
        if (!pum->dummy) {
          if (pum->getExternalName() == n) {
            // type check
            if (mb.useQt()) {
              throw_if(p->type != pum->type,
                       "completeMaterialPropertiesList: "
                       "incompatible type for variable '" +
                           n + "' ('" + p->type + "' vs '" + pum->type + "')");
            } else {
              // don't use quantity
              throw_if(SupportedTypes::getTypeFlag(p->type) !=
                           SupportedTypes::getTypeFlag(pum->type),
                       "incompatible type for variable '" + n + "' ('" +
                           p->type + "' vs '" + pum->type + "')");
              if (p->type != pum->type) {
                auto& log = getLogStream();
                log << "completeMaterialPropertiesList: "
                    << "inconsistent type for variable '" << n << "' ('"
                    << p->type << "' vs '" << pum->type << "')\n";
              }
            }
            throw_if(p->arraySize != pum->arraySize,
                     "incompatible array size for variable '" + n + "' ('" +
                         p->type + "' vs '" + pum->type + "')");
            found = true;
          }
        }
      }
      if (!found) {
        SupportedTypes::TypeSize o;
        if (!mprops.empty()) {
          const auto& m = mprops.back();
          o = m.offset;
          o += SupportedTypes::getTypeSize(m.type, m.arraySize);
        }
        mprops.emplace_back(*p, o, false);
      }
    }
  }  // end of completeMaterialPropertiesList

}  // end of namespace mfront
