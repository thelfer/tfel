/*!
 * \file   mtest/src/Behaviour.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 05 avril 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <bitset>
#include <climits>
#include <ostream>
#include <stdexcept>
#include "TFEL/Raise.hxx"
#include "TFEL/System/ExternalLibraryManager.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MTest/CurrentState.hxx"
#include "MTest/Evolution.hxx"
#include "MTest/Behaviour.hxx"
#include "MTest/GenericBehaviour.hxx"
#ifdef HAVE_CASTEM
#include "MTest/CastemSmallStrainBehaviour.hxx"
#include "MTest/CastemFiniteStrainBehaviour.hxx"
#include "MTest/CastemFiniteStrainBehaviour2.hxx"
#include "MTest/CastemCohesiveZoneModel.hxx"
#include "MTest/CastemModel.hxx"
#include "MTest/MistralBehaviour.hxx"
#include "MTest/CastemUmatSmallStrainBehaviour.hxx"
#include "MTest/CastemUmatFiniteStrainBehaviour.hxx"
#endif /* HAVE_CASTEM */
#ifdef HAVE_ASTER
#include "MTest/AsterSmallStrainBehaviour.hxx"
#include "MTest/AsterFiniteStrainBehaviour.hxx"
#include "MTest/AsterCohesiveZoneModel.hxx"
#include "MTest/AsterLogarithmicStrainBehaviourWrapper.hxx"
#endif /* HAVE_ASTER  */
#ifdef HAVE_ABAQUS
#include "MTest/AbaqusStandardBehaviour.hxx"
#include "MTest/AbaqusSmallStrainBehaviour.hxx"
#include "MTest/AbaqusFiniteStrainBehaviour.hxx"
#include "MTest/AbaqusExplicitBehaviour.hxx"
#endif /* HAVE_ABAQUS  */
#ifdef HAVE_ANSYS
#include "MTest/AnsysStandardBehaviour.hxx"
#include "MTest/AnsysSmallStrainBehaviour.hxx"
#include "MTest/AnsysFiniteStrainBehaviour.hxx"
#endif /* HAVE_ANSYS  */
#ifdef HAVE_CYRANO
#include "MTest/CyranoBehaviour.hxx"
#endif /* HAVE_CYRANO  */
#ifdef HAVE_CALCULIX
#include "MTest/CalculiXSmallStrainBehaviour.hxx"
#include "MTest/CalculiXFiniteStrainBehaviour.hxx"
#endif /* HAVE_CALCULIX  */
#ifdef HAVE_DIANAFEA
#include "MTest/DianaFEASmallStrainBehaviour.hxx"
#endif /* HAVE_DIANAFEA  */

namespace mtest::internals {

  static int extractAndShift(int& v, const std::size_t s) {
    using bits = std::bitset<sizeof(int) * CHAR_BIT>;
    const auto m = (bits{}.set() << s).flip();
    bits value(v);
    const auto r = value & m;
    value >>= s;
    v = static_cast<int>(value.to_ulong());
    return static_cast<int>(r.to_ulong());
  }  // end of extractAndShift

  static size_t getVariableSize(int&, const Behaviour::Hypothesis);

  static size_t getTinyVectorVariableSize(int& v,
                                          const Behaviour::Hypothesis h) {
    // tiny vector
    const auto N = extractAndShift(v, 2);
    if (N < 0) {
      tfel::raise("invalid tensorial object dimension");
    }
    if (N == 0) {
      return tfel::material::getSpaceDimension(h);
    } else if (N > 3) {
      tfel::raise("invalid space dimension");
    }
    return static_cast<size_t>(N);
  }  // end of getVariableSize

  static size_t getSymmetricTensorVariableSize(int& v,
                                               const Behaviour::Hypothesis h) {
    const auto N = extractAndShift(v, 2);
    if (N == 0) {
      return tfel::material::getStensorSize(h);
    } else if (N == 1) {
      return 3u;
    } else if (N == 2) {
      return 4u;
    } else if (N == 3) {
      return 6u;
    } else {
      tfel::raise("invalid space dimension");
    }
  }  // end of getSymmetricTensorVariableSize

  static size_t getUnSymmetricTensorVariableSize(
      int& v, const Behaviour::Hypothesis h) {
    const auto N = extractAndShift(v, 2);
    if (N == 0) {
      return tfel::material::getTensorSize(h);
    } else if (N == 1) {
      return 3u;
    } else if (N == 2) {
      return 5u;
    } else if (N == 3) {
      return 9u;
    } else {
      tfel::raise("invalid space dimension");
    }
  }  // end of getUnSymmetricTensorVariableSize

  static size_t getArrayVariableSize(int& v, const Behaviour::Hypothesis h) {
    // number of dimension of the array
    const auto a = extractAndShift(v, 3);
    if (a == 0) {
      tfel::raise("invalid array arity");
    }
    auto n = size_t{1};
    for (int i = 0; i != a; ++i) {
      const auto d = extractAndShift(v, 7);
      if (d < 1) {
        tfel::raise("invalid array dimension");
      }
      n *= d;
    }
    return n * getVariableSize(v, h);
  }  // end of getArrayVariableSize

  static size_t getVariableSize(int& v, const Behaviour::Hypothesis h) {
    const auto type = extractAndShift(v, 3);
    if (type == 0) {
      return 1u;
    } else if (type == 1) {
      return getSymmetricTensorVariableSize(v, h);
    } else if (type == 2) {
      return getTinyVectorVariableSize(v, h);
    } else if (type == 3) {
      return getUnSymmetricTensorVariableSize(v, h);
    } else if (type == 4) {
      // derivative type
      const auto s1 = getVariableSize(v, h);
      const auto s2 = getVariableSize(v, h);
      return s1 * s2;
    } else if (type != 5) {
      tfel::raise("unsupported variable type");
    }
    return getArrayVariableSize(v, h);
  }

  static bool isScalarVariableType(int& v) {
    const auto type = extractAndShift(v, 3);
    if (type == 0) {
      return true;
    } else if (type == 4) {
      // derivative type
      const auto b1 = isScalarVariableType(v);
      const auto b2 = isScalarVariableType(v);
      return b1 && b2;
    }
    return false;
  }

  static std::vector<std::string> getVariableComponents(int&, const Behaviour&);

  static std::vector<std::string> getTinyVectorVariableComponents(
      int& v, const Behaviour& b) {
    // tiny vector
    const auto N = extractAndShift(v, 2);
    if (N == 0) {
      return b.getVectorComponentsSuffixes();
    } else if (N == 1) {
      return {"X"};
    } else if (N == 2) {
      return {"X", "Y"};
    } else if (N == 3) {
      return {"X", "Y", "Z"};
    } else {
      tfel::raise("invalid space dimension");
    }
  }  // end of getVariableComponents

  static std::vector<std::string> getSymmetricTensorVariableComponents(
      int& v, const Behaviour& b) {
    const auto N = extractAndShift(v, 2);
    if (N == 0) {
      return b.getStensorComponentsSuffixes();
    } else if (N == 1) {
      return {"XX", "YY", "ZZ"};
    } else if (N == 2) {
      return {"XX", "YY", "ZZ", "XY"};
    } else if (N == 3) {
      return {"XX", "YY", "ZZ", "XY", "XZ", "YZ"};
    } else {
      tfel::raise("invalid space dimension");
    }
  }  // end of getSymmetricTensorVariableComponents

  static std::vector<std::string> getUnSymmetricTensorVariableComponents(
      int& v, const Behaviour& b) {
    const auto N = extractAndShift(v, 2);
    if (N == 0) {
      return b.getTensorComponentsSuffixes();
    } else if (N == 1) {
      return {"XX", "YY", "ZZ"};
    } else if (N == 2) {
      return {"XX", "YY", "ZZ", "XY", "YX"};
    } else if (N == 3) {
      return {"XX", "YY", "ZZ", "XY", "YX", "XZ", "ZX", "YZ", "ZY"};
    } else {
      tfel::raise("invalid space dimension");
    }
  }  // end of getUnSymmetricTensorVariableComponents

  static std::vector<std::string> getArrayVariableComponents(
      int& v, const Behaviour& b) {
    // number of dimension of the array
    const auto a = extractAndShift(v, 3);
    if (a == 0) {
      tfel::raise("invalid array arity");
    }
    auto components = std::vector<std::string>{};
    for (int i = 0; i != a; ++i) {
      const auto d = extractAndShift(v, 7);
      if (d < 1) {
        tfel::raise("invalid array dimension");
      }
      auto ncomponents = std::vector<std::string>{};
      if (components.empty()) {
        for (int j = 0; j != d; ++j) {
          ncomponents.push_back('[' + std::to_string(d) + ']');
        }
      } else {
        for (const auto& c : components) {
          for (int j = 0; j != d; ++j) {
            ncomponents.push_back(c + '[' + std::to_string(d) + ']');
          }
        }
      }
      components.swap(ncomponents);
    }
    const auto vcomponents = getVariableComponents(v, b);
    if (vcomponents.empty()) {
      return components;
    }
    auto ncomponents = std::vector<std::string>{};
    for (const auto& c1 : components) {
      for (const auto& c2 : vcomponents) {
        ncomponents.push_back(c1 + c2);
      }
    }
    return ncomponents;
  }  // end of getArrayVariableComponents

  std::vector<std::string> getVariableComponents(int& v,
                                                 const mtest::Behaviour& b) {
    const auto type = extractAndShift(v, 3);
    if (type == 0) {
      return {};
    } else if (type == 1) {
      return getSymmetricTensorVariableComponents(v, b);
    } else if (type == 2) {
      return getTinyVectorVariableComponents(v, b);
    } else if (type == 3) {
      return getUnSymmetricTensorVariableComponents(v, b);
    } else if (type == 4) {
      // derivative type
      const auto components1 = getVariableComponents(v, b);
      const auto components2 = getVariableComponents(v, b);
      auto components = std::vector<std::string>{};
      for (const auto& c1 : components1) {
        for (const auto& c2 : components2) {
          components.push_back(c1 + c2);
        }
      }
      return components;
    } else if (type != 5) {
      tfel::raise("unsupported variable type");
    }
    return getArrayVariableComponents(v, b);
  }  // end of getVariableComponents

}  // namespace mtest::internals

namespace mtest {

  std::shared_ptr<Behaviour> Behaviour::getBehaviour(const std::string& i,
                                                     const std::string& l,
                                                     const std::string& f,
                                                     const Parameters& d,
                                                     const Hypothesis h) {
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(c, "Behaviour::getBehaviour: " + m);
    };
    [[maybe_unused]] auto check_no_parameters = [&throw_if, &d] {
      if (d.empty()) {
        return;
      }
      throw_if(!d.is<tfel::utilities::DataMap>(),
               "unsupported parameters type");
      const auto& p = d.get<tfel::utilities::DataMap>();
      throw_if(!p.empty(), "no parameter expected");
    };
    auto& elm =
        tfel::system::ExternalLibraryManager::getExternalLibraryManager();
    auto b = std::shared_ptr<Behaviour>{};
    const auto in = i.empty() ? elm.getInterface(l, f) : i;
    if ((in == "generic") || (in == "Generic")) {
      if (d.empty()) {
        b = std::make_shared<GenericBehaviour>(h, l, f);
      } else {
        throw_if(!d.is<Behaviour::ParametersMap>(),
                 "unsupported parameters type");
        const auto& p = d.get<Behaviour::ParametersMap>();
        b = std::make_shared<GenericBehaviour>(h, l, f, p);
      }
    } else {
      throw_if(!elm.contains(l, f),
               "behaviour '" + f + "' not defined in library '" + l + "'");
    }
#ifdef HAVE_CASTEM
    if ((in == "castem") || (in == "umat") || (in == "Castem") ||
        (in == "Cast3M") ||  //
        (in == "castem21") || (in == "Castem21") || (in == "Cast3M21") ||
        (in == "amitex") || (in == "Amitex")) {
      check_no_parameters();
      const auto type = elm.getUMATBehaviourType(l, f);
      const auto ktype = elm.getUMATBehaviourKinematic(l, f);
      if (type == 0u) {
        b = std::make_shared<CastemModel>(h, l, f);
      } else if (type == 1u) {
        if ((in == "castem21") || (in == "Castem21") || (in == "Cast3M21")) {
          b = std::make_shared<Castem21SmallStrainBehaviour>(h, l, f);
        } else {
          b = std::make_shared<CastemSmallStrainBehaviour>(h, l, f);
        }
      } else if (type == 2u) {
        if (ktype == 3u) {
          if ((in == "castem21") || (in == "Castem21") || (in == "Cast3M21")) {
            b = std::make_shared<Castem21FiniteStrainBehaviour>(h, l, f);
          } else {
            b = std::make_shared<CastemFiniteStrainBehaviour>(h, l, f);
          }
        } else if (ktype == 4u) {
          if ((in == "castem21") || (in == "Castem21") || (in == "Cast3M21")) {
            b = std::make_shared<Castem21FiniteStrainBehaviour2>(h, l, f);
          } else {
            b = std::make_shared<CastemFiniteStrainBehaviour2>(h, l, f);
          }
        } else {
          throw_if(true, "unsupported kinematic");
        }
      } else if (type == 3u) {
        b = std::make_shared<CastemCohesiveZoneModel>(h, l, f);
      } else {
        throw_if(true,
                 "unsupported behaviour type (" + std::to_string(type) + ")");
      }
    }
    if (in == "mistral") {
      b = MistralBehaviour::buildMistralBehaviour(l, f, d, h);
    }
    if (in == "castem_umat_small_strain") {
      b = CastemUmatSmallStrainBehaviour::buildCastemUmatSmallStrainBehaviour(
          l, f, d, h);
    }
    if (in == "castem_umat_finite_strain") {
      b = CastemUmatFiniteStrainBehaviour::buildCastemUmatFiniteStrainBehaviour(
          l, f, d, h);
    }
#endif
#ifdef HAVE_ASTER
    if ((in == "aster") || (in == "Aster")) {
      check_no_parameters();
      const auto type = elm.getUMATBehaviourType(l, f);
      if (type == 1u) {
        const auto ktype = elm.getUMATBehaviourKinematic(l, f);
        if (ktype == 6u) {
          auto wb = std::make_shared<AsterSmallStrainBehaviour>(h, l, f);
          b = std::make_shared<AsterLogarithmicStrainBehaviourWrapper>(wb);
        } else {
          throw_if((ktype != 0u) && (ktype != 1u),
                   "unsupported behaviour kinematic");
          b = std::make_shared<AsterSmallStrainBehaviour>(h, l, f);
        }
      } else if (type == 2u) {
        b = std::make_shared<AsterFiniteStrainBehaviour>(h, l, f);
      } else if (type == 3u) {
        b = std::make_shared<AsterCohesiveZoneModel>(h, l, f);
      } else {
        throw_if(true,
                 "unsupported behaviour type (" + std::to_string(type) + ")");
      }
    }
#endif
#ifdef HAVE_ABAQUS
    if ((in == "Abaqus") || (in == "abaqus") || (in == "abaqus_standard") ||
        (in == "abaqus_umat")) {
      check_no_parameters();
      const auto bn = AbaqusStandardBehaviour::extractBehaviourName(f, h);
      const auto type = elm.getUMATBehaviourType(l, bn);
      if (type == 1u) {
        b = std::make_shared<AbaqusSmallStrainBehaviour>(h, l, f);
      } else if (type == 2u) {
        b = std::make_shared<AbaqusFiniteStrainBehaviour>(h, l, f);
      } else {
        throw_if(true,
                 "unsupported behaviour type (" + std::to_string(type) + ")");
      }
    }
    if ((in == "AbaqusExplicit") || (in == "abaqus_explicit") ||
        (in == "abaqus_vumat")) {
      const auto bn = AbaqusExplicitBehaviour::extractBehaviourName(f, h);
      check_no_parameters();
      const auto type = elm.getUMATBehaviourType(l, bn);
      if (type == 2u) {
        b = std::make_shared<AbaqusExplicitBehaviour>(h, l, f);
      } else {
        throw_if(true,
                 "unsupported behaviour type (" + std::to_string(type) + ")");
      }
    }
#endif
#ifdef HAVE_ANSYS
    if ((in == "Ansys") || (in == "ansys") || (in == "ansys_usermat")) {
      check_no_parameters();
      const auto bn = AnsysStandardBehaviour::extractBehaviourName(f, h);
      const auto type = elm.getUMATBehaviourType(l, bn);
      if (type == 1u) {
        b = std::make_shared<AnsysSmallStrainBehaviour>(h, l, f);
      } else if (type == 2u) {
        b = std::make_shared<AnsysFiniteStrainBehaviour>(h, l, f);
      } else {
        throw_if(true,
                 "unsupported behaviour type (" + std::to_string(type) + ")");
      }
    }
#endif
#ifdef HAVE_CYRANO
    if ((in == "cyrano") || (in == "Cyrano")) {
      const auto btype = elm.getUMATBehaviourType(l, f);
      const auto ktype = elm.getUMATBehaviourKinematic(l, f);
      check_no_parameters();
      if (btype == 1u) {
        b = std::make_shared<CyranoBehaviour>(h, l, f);
      } else if (btype == 2u) {
        if (ktype == 4u) {
          b = std::make_shared<CyranoBehaviour>(h, l, f);
        } else {
          throw_if(true, "unsupported behaviour kinematic");
        }
      } else {
        throw_if(true, "unsupported behaviour type");
      }
    }
#endif
#ifdef HAVE_CALCULIX
    if ((in == "calculix") || (in == "CalculiX")) {
      check_no_parameters();
      const auto type = elm.getUMATBehaviourType(l, f);
      if (type == 1u) {
        b = std::make_shared<CalculiXSmallStrainBehaviour>(h, l, f);
      } else if (type == 2u) {
        b = std::make_shared<CalculiXFiniteStrainBehaviour>(h, l, f);
      } else {
        throw_if(true,
                 "unsupported behaviour type (" + std::to_string(type) + ")");
      }
    }
#endif
#ifdef HAVE_DIANAFEA
    if ((in == "dianafea") || (in == "DianaFEA")) {
      check_no_parameters();
      const auto type = elm.getUMATBehaviourType(l, f);
      if (type == 1u) {
        b = std::make_shared<DianaFEASmallStrainBehaviour>(h, l, f);
      } else {
        throw_if(true,
                 "unsupported behaviour type (" + std::to_string(type) + ")");
      }
    }
#endif
    throw_if(b == nullptr, "unknown interface '" + in + "'");
    return b;
  }

  void Behaviour::setOptionalMaterialPropertyDefaultValue(
      EvolutionManager& mp,
      const EvolutionManager& evm,
      const std::string& n,
      const real v) {
    if (evm.find(n) == evm.end()) {
      if (mfront::getVerboseMode() >= mfront::VERBOSE_LEVEL2) {
        auto& log = mfront::getLogStream();
        log << "Behaviour::setOptionalMaterialPropertiesDefaultValues : "
            << "set material property '" << n << "' to default value\n";
      }
      tfel::raise_if(!mp.insert({n, make_evolution(v)}).second,
                     "Behaviour::setOptionalMaterialPropertiesDefaultValues: "
                     "default value for material property '" +
                         n + "' already declared");
    }
  }  // end of Behaviour::setOptionalMaterialPropertyDefaultValue

  Behaviour::~Behaviour() = default;

  bool isBehaviourVariable(const Behaviour& b, const std::string& n) {
    const auto enames = b.getGradientsComponents();
    auto p = std::find(enames.begin(), enames.end(), n);
    if (p != enames.end()) {
      return true;
    }
    const auto snames = b.getThermodynamicForcesComponents();
    p = std::find(snames.begin(), snames.end(), n);
    if (p != snames.end()) {
      return true;
    }
    const auto isvnames = b.expandInternalStateVariablesNames();
    p = std::find(isvnames.begin(), isvnames.end(), n);
    if (p != isvnames.end()) {
      return true;
    }
    const auto esvnames = b.getExternalStateVariablesNames();
    p = std::find(esvnames.begin(), esvnames.end(), n);
    if (p != esvnames.end()) {
      return true;
    }
    return false;
  }  // end of canValueBeExtracted

  std::function<real(const CurrentState&)> buildValueExtractor(
      const Behaviour& b, const std::string& n) {
    const auto enames = b.getGradientsComponents();
    auto p = std::find(enames.begin(), enames.end(), n);
    if (p != enames.end()) {
      const auto pos = static_cast<unsigned short>(p - enames.begin());
      return [pos](const CurrentState& s) noexcept { return s.e1[pos]; };
    }
    const auto snames = b.getThermodynamicForcesComponents();
    p = std::find(snames.begin(), snames.end(), n);
    if (p != snames.end()) {
      const auto pos = static_cast<unsigned short>(p - snames.begin());
      return [pos](const CurrentState& s) noexcept { return s.s1[pos]; };
    }
    const auto isvnames = b.expandInternalStateVariablesNames();
    p = std::find(isvnames.begin(), isvnames.end(), n);
    if (p != isvnames.end()) {
      const auto pos = static_cast<unsigned short>(p - isvnames.begin());
      return [pos](const CurrentState& s) noexcept { return s.iv1[pos]; };
    }
    const auto esvnames = b.getExternalStateVariablesNames();
    p = std::find(esvnames.begin(), esvnames.end(), n);
    if (p != esvnames.end()) {
      const auto pos = static_cast<unsigned short>(p - esvnames.begin());
      return [pos](const CurrentState& s) noexcept {
        return s.esv0[pos] + s.desv[pos];
      };
    }
    tfel::raise("buildValueExtractor: no variable name '" + n + "'");
  }  // end of buildValueExtractor

  bool isScalarVariableType(const int t) {
    auto id = t;
    return mtest::internals::isScalarVariableType(id);
  }  // end of isScalarVariableType

  size_t getVariableSize(const int t, const Behaviour::Hypothesis h) {
    auto id = t;
    const auto s = mtest::internals::getVariableSize(id, h);
    if (id != 0) {
      tfel::raise("getVariableSize: invalid type identifier '" +
                  std::to_string(t) + "'");
    }
    return s;
  }  // end of getVariableSize

  size_t getVariablesSize(const std::vector<int>& types,
                          const Behaviour::Hypothesis h) {
    size_t s = 0;
    for (const auto& t : types) {
      s += getVariableSize(t, h);
    }
    return s;
  }  // end of getVariablesSize

  std::vector<std::string> getVariableComponents(const Behaviour& b,
                                                 const std::string& n,
                                                 const int t) {
    auto id = t;
    auto components = mtest::internals::getVariableComponents(id, b);
    if (id != 0) {
      tfel::raise("getVariableSize: invalid type identifier '" +
                  std::to_string(t) + "'");
    }
    if (components.empty()) {
      return {n};
    }
    for (auto& c : components) {
      c = n + c;
    }
    return components;
  }  // end of getVariableComponents

}  // end of namespace mtest
