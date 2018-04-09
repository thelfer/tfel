/*!
 * \file   BrickUtilities.ixx
 * \brief    
 * \author Thomas Helfer
 * \date   26/03/2018
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_BEHAVIOURBRICK_BRICKUTILITIES_IXX
#define LIB_MFRONT_BEHAVIOURBRICK_BRICKUTILITIES_IXX

namespace mfront {

  namespace bbrick {

    template <unsigned short N>
    std::array<BehaviourDescription::MaterialProperty, N>
    getArrayOfBehaviourDescriptionMaterialProperties(
        AbstractBehaviourDSL& dsl,
        const std::string& n,
        const tfel::utilities::Data& d){
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

  }  // end of namespace bbrick

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURBRICK_BRICKUTILITIES_IXX */
