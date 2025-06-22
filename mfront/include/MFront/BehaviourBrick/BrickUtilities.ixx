/*!
 * \file   BrickUtilities.ixx
 * \brief
 * \author Thomas Helfer
 * \date   26/03/2018
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_BEHAVIOURBRICK_BRICKUTILITIES_IXX
#define LIB_MFRONT_BEHAVIOURBRICK_BRICKUTILITIES_IXX

namespace mfront {

  namespace bbrick {

    template <std::size_t N>
    std::array<BehaviourDescription::MaterialProperty, N>
    getArrayOfBehaviourDescriptionMaterialProperties(
        AbstractBehaviourDSL& dsl,
        const std::string& n,
        const tfel::utilities::Data& d) {
      static_assert(N != 0, "invalid array size");
      std::array<BehaviourDescription::MaterialProperty, N> mps;
      if (!d.is<std::vector<tfel::utilities::Data>>()) {
        tfel::raise(
            "getArrayOfBehaviourDescriptionMaterialProperties: "
            "error while extracting array of material properties '" +
            n + "', invalid type for the given data");
      }
      const auto& vd = d.get<std::vector<tfel::utilities::Data>>();
      if (vd.size() != N) {
        tfel::raise(
            "getArrayOfBehaviourDescriptionMaterialProperties: "
            "error while extracting array of material properties '" +
            n +
            "', invalid size for the given data (expected an array of size '" +
            std::to_string(N) + "', but an array of size '" +
            std::to_string(vd.size()) + "' was given)");
      }
      auto i = typename std::array<BehaviourDescription::MaterialProperty,
                                   N>::size_type{};
      for (const auto& e : vd) {
        mps[i] = getBehaviourDescriptionMaterialProperty(dsl, n, e);
        ++i;
      }
      return mps;
    }

    template <std::size_t N>
    bool areAllConstantMaterialProperties(
        const std::array<BehaviourDescription::MaterialProperty, N>& mps) {
      static_assert(N != 0, "invalid array size");
      for (const auto& mp : mps) {
        if (!mp.template is<BehaviourDescription::ConstantMaterialProperty>()) {
          return false;
        }
      }
      return true;
    }  // end of areAllConstantMaterialProperties

    template <std::size_t N>
    void declareParameterOrLocalVariable(
        BehaviourDescription& bd,
        std::array<BehaviourDescription::MaterialProperty, N>& mps,
        const std::string& t,
        const std::string& n) {
      static_assert(N != 0, "invalid array size");
      constexpr const auto h =
          tfel::material::ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      if (areAllConstantMaterialProperties(mps)) {
        for (auto& mp : mps) {
          auto& cmp =
              mp.template get<BehaviourDescription::ConstantMaterialProperty>();
          cmp.name = n;
        }
        // declare associated parameter
        VariableDescription m(t, n, N, 0u);
        bd.addParameter(h, m);
        if (N == 1) {
          const auto& cmp =
              mps[0]
                  .template get<
                      BehaviourDescription::ConstantMaterialProperty>();
          bd.setParameterDefaultValue(h, n, cmp.value);
        } else {
          for (decltype(mps.size()) i = 0; i != mps.size(); ++i) {
            const auto& cmp =
                mps[i]
                    .template get<
                        BehaviourDescription::ConstantMaterialProperty>();
            bd.setParameterDefaultValue(h, n, i, cmp.value);
          }
        }
      } else {
        VariableDescription m(t, n, N, 0u);
        bd.addLocalVariable(h, m);
      }
    }  // end of declareParameterOrLocalVariable

    template <std::size_t N>
    void declareParameterOrLocalVariable(
        BehaviourDescription& bd,
        std::array<BehaviourDescription::MaterialProperty, N>& mps,
        const std::string& t,
        const std::string& n,
        const std::string& en) {
      static_assert(N != 0, "invalid array size");
      constexpr const auto h =
          tfel::material::ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      declareParameterOrLocalVariable(bd, mps, t, n);
      if (areAllConstantMaterialProperties(mps)) {
        bd.setEntryName(h, n, en);
      }
    }  // end of declareParameterOrLocalVariable

    template <std::size_t N>
    void declareParameterOrLocalVariable(
        BehaviourDescription& bd,
        std::array<BehaviourDescription::MaterialProperty, N>& mps,
        const std::string& t,
        const std::string& n,
        const tfel::glossary::GlossaryEntry& g) {
      constexpr const auto h =
          tfel::material::ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      declareParameterOrLocalVariable(bd, mps, t, n);
      if (areAllConstantMaterialProperties(mps)) {
        bd.setGlossaryName(h, n, g);
      }
    }  // end of declareParameterOrLocalVariable

    template <std::size_t N>
    std::string generateMaterialPropertiesInitializationCode(
        const AbstractBehaviourDSL& dsl,
        const BehaviourDescription& bd,
        const std::string& n,
        const std::array<BehaviourDescription::MaterialProperty, N>& mps) {
      auto c = std::string{};
      if (!areAllConstantMaterialProperties(mps)) {
        for (decltype(mps.size()) i = 0; i != mps.size(); ++i) {
          const auto& mp = mps[i];
          const auto vn = n + "[" + std::to_string(i) + "]";
          if (!mp.template is<
                  BehaviourDescription::ConstantMaterialProperty>()) {
            c += generateMaterialPropertyInitializationCode(dsl, bd, vn, mp);
          } else {
            const auto& cmp = mp.template get<
                BehaviourDescription::ConstantMaterialProperty>();
            c += "this->" + vn + " = " + std::to_string(i) +
                 "] = " + std::to_string(cmp.value) + ";\n";
          }
        }
      }
      return c;
    }  // end of generateMaterialPropertiesInitializationCode

  }  // end of namespace bbrick

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURBRICK_BRICKUTILITIES_IXX */
