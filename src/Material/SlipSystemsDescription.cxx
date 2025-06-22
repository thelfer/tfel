/*!
 * \file   SlipSystemsDescription.cxx
 * \brief
 * \author Thomas Helfer
 * \date   09 juin 2017
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <string>
#include <stdexcept>
#include "NUMODIS/HCP.hxx"
#include "NUMODIS/BCC.hxx"
#include "NUMODIS/FCC.hxx"
#include "NUMODIS/Cubic.hxx"
#include "NUMODIS/GSystem.hxx"
#include "NUMODIS/Hardening.hxx"
#include "TFEL/Raise.hxx"
#include "TFEL/FSAlgorithm/copy.hxx"
#include "TFEL/FSAlgorithm/equal.hxx"
#include "TFEL/Math/General/MathConstants.hxx"
#include "TFEL/Material/SlipSystemsDescription.hxx"

namespace tfel::material {

  template <CrystalStructure cs>
  struct CristalStructureSlipSystems;

  template <>
  struct CristalStructureSlipSystems<CrystalStructure::HCP> {
    using numodis_handler = numodis::HCP;
    //! type of vector used to describe the slip systems
    using vector_type = SlipSystemsDescription::vec4d;
    //! type used to describe the slip systems
    using system_type = SlipSystemsDescription::system4d;
  };  // end of CristalStructureSlipSystems<CrystalStructure::HCP>

  template <>
  struct CristalStructureSlipSystems<CrystalStructure::Cubic> {
    using numodis_handler = numodis::Cubic;
    //! type of vector used to describe the slip systems
    using vector_type = SlipSystemsDescription::vec3d;
    //! type used to describe the slip systems
    using system_type = SlipSystemsDescription::system3d;
  };  // end of CristalStructureSlipSystems<CrystalStructure::Cubic>

  template <>
  struct CristalStructureSlipSystems<CrystalStructure::FCC> {
    using numodis_handler = numodis::FCC;
    //! type of vector used to describe the slip systems
    using vector_type = SlipSystemsDescription::vec3d;
    //! type used to describe the slip systems
    using system_type = SlipSystemsDescription::system3d;
  };  // end of CristalStructureSlipSystems<CrystalStructure::FCC>

  template <>
  struct CristalStructureSlipSystems<CrystalStructure::BCC> {
    using numodis_handler = numodis::BCC;
    //! type of vector used to describe the slip systems
    using vector_type = SlipSystemsDescription::vec3d;
    //! type used to describe the slip systems
    using system_type = SlipSystemsDescription::system3d;
  };  // end of CristalStructureSlipSystems<CrystalStructure::BCC>

  template <typename T, typename VectorType>
  static T to_numodis(const VectorType& v) {
    using size_type = typename T::size_type;
    auto r = T(static_cast<size_type>(v.size()));
    for (decltype(v.size()) i = 0; i != v.size(); ++i) {
      r[i] = v[i];
    }
    return r;
  }

  template <typename VectorType>
  static numodis::GSystem to_numodis(const VectorType& b, const VectorType& n) {
    return numodis::GSystem{to_numodis<numodis::IBurgers>(b),
                            to_numodis<numodis::IPlane>(n)};
  }

  template <typename VectorType, typename T>
  static VectorType from_numodis(const T& v) {
    auto r = VectorType();
    raise_if(r.size() != v.getNindices(),
             "from_numodis: unmatched object size");
    for (decltype(v.getNindices()) i = 0; i != v.getNindices(); ++i) {
      r[i] = v[i];
    }
    return r;
  }

  static SlipSystemsDescription::system from_numodis(
      const numodis::GSystem& g) {
    using vec3d = SlipSystemsDescription::vec3d;
    using vec4d = SlipSystemsDescription::vec4d;
    using system3d = SlipSystemsDescription::system3d;
    using system4d = SlipSystemsDescription::system4d;
    if (g.getIBurgers().getNindices() == 3u) {
      return system3d{from_numodis<vec3d>(g.getIBurgers()),
                      from_numodis<vec3d>(g.getIPlane())};
    }
    raise_if(g.getIBurgers().getNindices() != 4u,
             "from_numodis: unexecpected Burgers' vector size");
    return system4d{from_numodis<vec4d>(g.getIBurgers()),
                    from_numodis<vec4d>(g.getIPlane())};
  }

  template <typename VectorType>
  std::string to_string(const VectorType& v, const char b, const char e) {
    auto r = std::string{};
    r += b;
    for (typename VectorType::size_type i = 0; i != v.size();) {
      r += std::to_string(v[i]);
      if (++i != v.size()) {
        r += ',';
      }
    }
    r += e;
    return r;
  }  // end of to_string

  template <CrystalStructure cs>
  std::vector<SlipSystemsDescription::system> generateSlipSystems(
      const typename CristalStructureSlipSystems<cs>::vector_type& b,
      const typename CristalStructureSlipSystems<cs>::vector_type& p) {
    using handler = typename CristalStructureSlipSystems<cs>::numodis_handler;
    const auto cd = handler{};
    const auto nb = to_numodis<numodis::IBurgers>(b);
    const auto pp = to_numodis<numodis::IPlane>(p);
    const auto ng = numodis::GSystem{cd.InitGSystem(nb, pp)};
    std::vector<numodis::GSystem> gs;
    cd.GenerateEquivalentGSystems(ng, gs);
    auto r = std::vector<SlipSystemsDescription::system>(gs.size());
    for (decltype(gs.size()) i = 0; i != gs.size(); ++i) {
      r[i] = from_numodis(gs[i]);
    }
    return r;
  }  // end of generateSlipSystems

  template <CrystalStructure cs>
  std::vector<SlipSystemsDescription::system> generateSlipSystems(
      const SlipSystemsDescription::system& g) {
    using system_type = typename CristalStructureSlipSystems<cs>::system_type;
    raise_if(!g.is<system_type>(),
             "generateSlipSystems: unexpected system description");
    const auto& gd = g.get<system_type>();
    return generateSlipSystems<cs>(gd.burgers, gd.plane);
  }  // end of generateSlipSystems

  template <CrystalStructure cs>
  static void addSlipSystemsFamily(
      std::vector<typename CristalStructureSlipSystems<cs>::system_type>& gs,
      const typename CristalStructureSlipSystems<cs>::vector_type& b,
      const typename CristalStructureSlipSystems<cs>::vector_type& p) {
    using system_type = typename CristalStructureSlipSystems<cs>::system_type;
    for (const auto& gn : gs) {
      for (const auto& gg : generateSlipSystems<cs>(gn)) {
        const auto& g = gg.template get<system_type>();
        raise_if((std::equal(g.burgers.begin(), g.burgers.end(), b.begin())) &&
                     (std::equal(g.plane.begin(), g.plane.end(), p.begin())),
                 "SlipSystemsDescription::addSlipSystemsFamily: "
                 "slip system " +
                     to_string(g.burgers, '(', ')') +
                     to_string(g.plane, '[', ']') + ' ' +
                     "is generated by systems " + to_string(b, '<', '>') +
                     to_string(p, '{', '}') + " and " +
                     to_string(gn.burgers, '<', '>') +
                     to_string(gn.plane, '{', '}'));
      }
    }
    gs.push_back({b, p});
  }  // end of addSlipSytem

  SlipSystemsDescription::SlipSystemsDescription(const CrystalStructure s)
      : cs(s) {
    if ((s == CrystalStructure::Cubic) || (s == CrystalStructure::FCC) ||
        (s == CrystalStructure::BCC)) {
      this->gs = std::vector<system3d>();
    } else if (s == CrystalStructure::HCP) {
      this->gs = std::vector<system4d>();
    } else {
      raise(
          "SlipSystemsDescription::SlipSystemsDescription: "
          "unsupported crystal structure");
    }
  }

  SlipSystemsDescription::SlipSystemsDescription(
      const SlipSystemsDescription&) = default;
  SlipSystemsDescription::SlipSystemsDescription(SlipSystemsDescription&&) =
      default;

  CrystalStructure SlipSystemsDescription::getCrystalStructure() const {
    return this->cs;
  }  // end of SlipSystemsDescription::getCrystalStructure

  void SlipSystemsDescription::addSlipSystemsFamily(const vec3d& b,
                                                    const vec3d& p) {
    auto throw_if = [](const bool c, const std::string& msg) {
      raise_if(c, "SlipSystemsDescription::addSlipSystemsFamily: " + msg);
    };
    throw_if(!this->gs.is<std::vector<system3d>>(),
             "invalid size for Burgers' vector and normal");
    auto& gs3d = this->gs.get<std::vector<system3d>>();
    if (this->cs == CrystalStructure::Cubic) {
      tfel::material::addSlipSystemsFamily<CrystalStructure::Cubic>(gs3d, b, p);
    } else if (this->cs == CrystalStructure::BCC) {
      tfel::material::addSlipSystemsFamily<CrystalStructure::BCC>(gs3d, b, p);
    } else {
      throw_if(cs != CrystalStructure::FCC,
               "internal error (unknown cristal structure)");
      tfel::material::addSlipSystemsFamily<CrystalStructure::FCC>(gs3d, b, p);
    }
  }  // end of SlipSystemsDescription::addSlipSystemsFamily

  void SlipSystemsDescription::addSlipSystemsFamily(const vec4d& b,
                                                    const vec4d& p) {
    auto throw_if = [](const bool c, const std::string& msg) {
      raise_if(c, "SlipSystemsDescription::addSlipSystemsFamily: " + msg);
    };
    throw_if(!this->gs.is<std::vector<system4d>>(),
             "invalid size for Burgers' vector and normal");
    throw_if(cs != CrystalStructure::HCP,
             "internal error (unknown cristal structure)");
    auto& gs4d = this->gs.get<std::vector<system4d>>();
    tfel::material::addSlipSystemsFamily<CrystalStructure::HCP>(gs4d, b, p);
  }  // end of SlipSystemsDescription::addSlipSystemsFamily

  SlipSystemsDescription::size_type
  SlipSystemsDescription::getNumberOfSlipSystemsFamilies() const {
    if (this->gs.is<std::vector<system4d>>()) {
      return this->gs.get<std::vector<system4d>>().size();
    } else if (this->gs.is<std::vector<system3d>>()) {
      return this->gs.get<std::vector<system3d>>().size();
    }
    raise_if(!this->gs.empty(),
             "SlipSystemsDescription::getNumberOfSlipSystemsFamilies: "
             "internal error (unsupported slip system type)");
    return size_type{};
  }  // end of SlipSystemsDescription::getNumberOfSlipSystemsFamilies()

  SlipSystemsDescription::system SlipSystemsDescription::getSlipSystemFamily(
      const size_type i) const {
    auto throw_if = [](const bool b, const std::string& msg) {
      raise_if(b, "SlipSystemsDescription::getSlipSystem: " + msg);
    };
    if (this->gs.is<std::vector<system4d>>()) {
      const auto& v = this->gs.get<std::vector<system4d>>();
      throw_if(i >= v.size(), "out of bounds index");
      return v[i];
    }
    throw_if(!this->gs.is<std::vector<system3d>>(),
             "no slip system family defined");
    const auto& v = this->gs.get<std::vector<system3d>>();
    throw_if(i >= v.size(), "out of bounds index");
    return v[i];
  }

  std::vector<SlipSystemsDescription::system>
  SlipSystemsDescription::getSlipSystems(const size_type i) const {
    const auto& g = this->getSlipSystemFamily(i);
    auto throw_if = [](const bool b, const std::string& msg) {
      raise_if(b, "generateSlipSystems: " + msg);
    };
    if (this->cs == CrystalStructure::Cubic) {
      return generateSlipSystems<CrystalStructure::Cubic>(g);
    } else if (this->cs == CrystalStructure::FCC) {
      return generateSlipSystems<CrystalStructure::FCC>(g);
    } else if (this->cs == CrystalStructure::BCC) {
      return generateSlipSystems<CrystalStructure::BCC>(g);
    }
    throw_if(cs != CrystalStructure::HCP,
             "internal error (unknown cristal structure)");
    return generateSlipSystems<CrystalStructure::HCP>(g);
  }

  std::vector<std::vector<SlipSystemsDescription::system>>
  SlipSystemsDescription::getSlipSystems() const {
    const auto s = this->getNumberOfSlipSystemsFamilies();
    auto r = std::vector<std::vector<SlipSystemsDescription::system>>(s);
    for (size_type i = 0; i != this->getNumberOfSlipSystemsFamilies(); ++i) {
      r[i] = this->getSlipSystems(i);
    }
    return r;
  }  // end of SlipSystemsDescription::getSlipSystems

  static std::array<long double, 3u> to_array(const numodis::Vect3& v) {
    std::array<long double, 3u> r;
    r[0] = v[0];
    r[1] = v[1];
    r[2] = v[2];
    return r;
  }

  static std::array<long double, 3u> normal(
      const CrystalStructure cs, const SlipSystemsDescription::vec3d& p) {
    if (cs == CrystalStructure::FCC) {
      auto s = numodis::FCC();
      return to_array(s.normal(to_numodis<numodis::IPlane>(p)));
    } else if (cs == CrystalStructure::BCC) {
      auto s = numodis::BCC();
      return to_array(s.normal(to_numodis<numodis::IPlane>(p)));
    }
    raise_if(cs != CrystalStructure::Cubic,
             "normal: internal error "
             "(unsupported crystal structure)");
    auto s = numodis::Cubic();
    return to_array(s.normal(to_numodis<numodis::IPlane>(p)));
  }

  static std::array<long double, 3u> normal(
      const CrystalStructure cs, const SlipSystemsDescription::vec4d& p) {
    raise_if(cs != CrystalStructure::HCP,
             "normal: internal error "
             "(unsupported crystal structure)");
    auto s = numodis::HCP();
    return to_array(s.normal(to_numodis<numodis::IPlane>(p)));
  }

  static std::array<long double, 3u> burgers(
      const CrystalStructure cs, const SlipSystemsDescription::vec3d& b) {
    if (cs == CrystalStructure::FCC) {
      auto s = numodis::FCC();
      auto bv = s.burgers_vector(to_numodis<numodis::IBurgers>(b));
      bv.Normalize();
      return to_array(bv);
    } else if (cs == CrystalStructure::BCC) {
      auto s = numodis::BCC();
      auto bv = s.burgers_vector(to_numodis<numodis::IBurgers>(b));
      bv.Normalize();
      return to_array(bv);
    }
    raise_if(cs != CrystalStructure::Cubic,
             "burgers: internal error "
             "(unsupported crystal structure)");
    auto s = numodis::Cubic();
    auto bv = s.burgers_vector(to_numodis<numodis::IBurgers>(b));
    bv.Normalize();
    return to_array(bv);
  }

  static std::array<long double, 3u> burgers(
      const CrystalStructure cs, const SlipSystemsDescription::vec4d& b) {
    raise_if(cs != CrystalStructure::HCP,
             "burgers: internal error "
             "(unsupported crystal structure)");
    auto s = numodis::HCP();
    auto bv = s.burgers_vector(to_numodis<numodis::IBurgers>(b));
    bv.Normalize();
    return to_array(bv);
  }

  static SlipSystemsDescription::tensor getOrientationTensor(
      const std::array<long double, 3u>& n,
      const std::array<long double, 3u>& m) {
    using tensor = SlipSystemsDescription::tensor;
    // return m^n
    // XX YY ZZ
    // XY YX XZ
    // ZX YZ ZY
    return tensor{n[0] * m[0], n[1] * m[1], n[2] * m[2],
                  n[1] * m[0], n[0] * m[1], n[2] * m[0],
                  n[0] * m[2], n[2] * m[1], n[1] * m[2]};
  }  // end of getOrentiationTensor

  static SlipSystemsDescription::tensor getOrientationTensor(
      const CrystalStructure cs, const SlipSystemsDescription::system3d& s) {
    const auto& n = normal(cs, s.plane);
    const auto& m = burgers(cs, s.burgers);
    return tfel::material::getOrientationTensor(n, m);
  }  // end of getOrentiationTensor

  static SlipSystemsDescription::tensor getOrientationTensor(
      const CrystalStructure cs, const SlipSystemsDescription::system4d& s) {
    const auto& n = normal(cs, s.plane);
    const auto& m = burgers(cs, s.burgers);
    return tfel::material::getOrientationTensor(n, m);
  }  // end of getOrentiationTensor

  static SlipSystemsDescription::vector getSlipPlaneNormal(
      const CrystalStructure cs, const SlipSystemsDescription::system& s) {
    if (s.is<SlipSystemsDescription::system3d>()) {
      const auto& s3d = s.get<SlipSystemsDescription::system3d>();
      return normal(cs, s3d.plane);
    }
    raise_if(!s.is<SlipSystemsDescription::system4d>(),
             "getOrentiationTensor: "
             "internal error (unsupported slip system type)");
    const auto& s4d = s.get<SlipSystemsDescription::system4d>();
    return normal(cs, s4d.plane);
  }  // end of getSlipPlaneNormal

  static SlipSystemsDescription::vector getSlipDirection(
      const CrystalStructure cs, const SlipSystemsDescription::system& s) {
    if (s.is<SlipSystemsDescription::system3d>()) {
      const auto& s3d = s.get<SlipSystemsDescription::system3d>();
      return burgers(cs, s3d.burgers);
    }
    raise_if(!s.is<SlipSystemsDescription::system4d>(),
             "getOrentiationTensor: "
             "internal error (unsupported slip system type)");
    const auto& s4d = s.get<SlipSystemsDescription::system4d>();
    return burgers(cs, s4d.burgers);
  }  // end of getSlipDirection

  static SlipSystemsDescription::tensor getOrientationTensor(
      const CrystalStructure cs, const SlipSystemsDescription::system& s) {
    if (s.is<SlipSystemsDescription::system3d>()) {
      return getOrientationTensor(cs,
                                  s.get<SlipSystemsDescription::system3d>());
    }
    raise_if(!s.is<SlipSystemsDescription::system4d>(),
             "getOrentiationTensor: "
             "internal error (unsupported slip system type)");
    return getOrientationTensor(cs, s.get<SlipSystemsDescription::system4d>());
  }  // end of getOrentiationTensor

  std::vector<SlipSystemsDescription::vector>
  SlipSystemsDescription::getSlipPlaneNormals(const size_type i) const {
    const auto ss = this->getSlipSystems(i);
    auto r = std::vector<vector>(ss.size());
    for (size_type idx = 0; idx != ss.size(); ++idx) {
      r[idx] = tfel::material::getSlipPlaneNormal(this->cs, ss[idx]);
    }
    return r;
  }  // end of SlipSystemsDescription::getSlipPlaneNormals()

  std::vector<std::vector<SlipSystemsDescription::vector>>
  SlipSystemsDescription::getSlipPlaneNormals() const {
    const auto s = this->getNumberOfSlipSystemsFamilies();
    auto r = std::vector<std::vector<vector>>(s);
    for (size_type i = 0; i != this->getNumberOfSlipSystemsFamilies(); ++i) {
      r[i] = this->getSlipPlaneNormals(i);
    }
    return r;
  }  // end of SlipSystemsDescription::getSlipPlaneNormals()

  std::vector<SlipSystemsDescription::vector>
  SlipSystemsDescription::getSlipDirections(const size_type i) const {
    const auto ss = this->getSlipSystems(i);
    auto r = std::vector<vector>(ss.size());
    for (size_type idx = 0; idx != ss.size(); ++idx) {
      r[idx] = tfel::material::getSlipDirection(this->cs, ss[idx]);
    }
    return r;
  }  // end of SlipSystemsDescription::getSlipDirections()

  std::vector<std::vector<SlipSystemsDescription::vector>>
  SlipSystemsDescription::getSlipDirections() const {
    const auto s = this->getNumberOfSlipSystemsFamilies();
    auto r = std::vector<std::vector<vector>>(s);
    for (size_type i = 0; i != this->getNumberOfSlipSystemsFamilies(); ++i) {
      r[i] = this->getSlipDirections(i);
    }
    return r;
  }  // end of SlipSystemsDescription::getSlipDirections()

  std::vector<SlipSystemsDescription::tensor>
  SlipSystemsDescription::getOrientationTensors(const size_type i) const {
    const auto ss = this->getSlipSystems(i);
    auto r = std::vector<tensor>(ss.size());
    for (size_type idx = 0; idx != ss.size(); ++idx) {
      r[idx] = tfel::material::getOrientationTensor(this->cs, ss[idx]);
    }
    return r;
  }  // end of SlipSystemsDescription::getOrientationTensors()

  std::vector<std::vector<SlipSystemsDescription::tensor>>
  SlipSystemsDescription::getOrientationTensors() const {
    const auto s = this->getNumberOfSlipSystemsFamilies();
    auto r = std::vector<std::vector<tensor>>(s);
    for (size_type i = 0; i != this->getNumberOfSlipSystemsFamilies(); ++i) {
      r[i] = this->getOrientationTensors(i);
    }
    return r;
  }  // end of SlipSystemsDescription::getOrientationTensors()

  static std::array<long double, 3u> getDirection(
      const CrystalStructure cs, const SlipSystemsDescription::vec& d) {
    auto throw_if = [](const bool c, const std::string& m) {
      raise_if(c, "getDirection: " + m);
    };
    auto build3d = [](const SlipSystemsDescription::vec3d& v) {
      return numodis::IDirection({v[0], v[1], v[2]});
    };
    auto build4d = [](const SlipSystemsDescription::vec4d& v) {
      return numodis::IDirection({v[0], v[1], v[2], v[3]});
    };
    if (cs == CrystalStructure::HCP) {
      throw_if(!d.is<SlipSystemsDescription::vec4d>(),
               "invalid direction description for HCP "
               "(shall be given as a four indices' vector)");
      const auto s = numodis::HCP();
      const auto d4d = d.get<SlipSystemsDescription::vec4d>();
      const auto bv = s.direction(build4d(d4d));
      return to_array(bv);
    } else if (cs == CrystalStructure::Cubic) {
      throw_if(!d.is<SlipSystemsDescription::vec3d>(),
               "invalid direction description for HCP "
               "(shall be given as a four indices' vector)");
      const auto s = numodis::Cubic();
      const auto d3d = d.get<SlipSystemsDescription::vec3d>();
      const auto bv = s.direction(build3d(d3d));
      return to_array(bv);
    } else if (cs == CrystalStructure::FCC) {
      throw_if(!d.is<SlipSystemsDescription::vec3d>(),
               "invalid direction description for HCP "
               "(shall be given as a four indices' vector)");
      const auto s = numodis::FCC();
      const auto d3d = d.get<SlipSystemsDescription::vec3d>();
      const auto bv = s.direction(build3d(d3d));
      return to_array(bv);
    }
    throw_if(cs != CrystalStructure::BCC, "unsupported crystalline structure");
    const auto s = numodis::BCC();
    const auto d3d = d.get<SlipSystemsDescription::vec3d>();
    const auto bv = s.direction(build3d(d3d));
    return to_array(bv);
  }  // end of getDirection

  std::vector<long double> SlipSystemsDescription::getSchmidFactors(
      const vec d, const size_type i) const {
    const auto ss = this->getSlipSystems(i);
    auto r = std::vector<long double>(ss.size());
    const auto d3d = getDirection(this->cs, d);
    const auto td = tensor{d3d[0] * d3d[0], d3d[1] * d3d[1], d3d[2] * d3d[2],
                           d3d[0] * d3d[1], d3d[1] * d3d[0], d3d[0] * d3d[2],
                           d3d[2] * d3d[0], d3d[1] * d3d[2], d3d[2] * d3d[1]};
    for (size_type idx = 0; idx != ss.size(); ++idx) {
      const auto mn = tfel::material::getOrientationTensor(this->cs, ss[idx]);
      r[i] = td[8] * mn[8] + td[7] * mn[7] + td[6] * mn[6] + td[5] * mn[5] +
             td[4] * mn[4] + td[3] * mn[3] + td[2] * mn[2] + td[1] * mn[1] +
             td[0] * mn[0];
    }
    return r;
  }  // end of SlipSystemsDescription::getSchmidFactors

  std::vector<std::vector<long double>>
  SlipSystemsDescription::getSchmidFactors(const vec d) const {
    const auto s = this->getNumberOfSlipSystemsFamilies();
    auto r = std::vector<std::vector<long double>>(s);
    for (size_type i = 0; i != this->getNumberOfSlipSystemsFamilies(); ++i) {
      r[i] = this->getSchmidFactors(d, i);
    }
    return r;
  }  // end of SlipSystemsDescription::getSchmidFactors

  SlipSystemsDescription::size_type
  SlipSystemsDescription::getNumberOfSlipSystems(const size_type i) const {
    return this->getSlipSystems(i).size();
  }  // end of SlipSystemsDescription::getNumberOfSlipSystems

  SlipSystemsDescription::size_type
  SlipSystemsDescription::getNumberOfSlipSystems() const {
    auto s = size_type{};
    for (size_type i = 0; i != this->getNumberOfSlipSystemsFamilies(); ++i) {
      s += this->getNumberOfSlipSystems(i);
    }
    return s;
  }  // end of SlipSystemsDescription::getNumberOfSlipSystems

  bool SlipSystemsDescription::hasInteractionMatrix() const {
    return !this->m.empty();
  }  // end of SlipSystemsDescription::hasInteractionMatrix

  void SlipSystemsDescription::setInteractionMatrix(
      const std::vector<long double>& v) {
    auto throw_if = [](const bool c, const std::string& msg) {
      raise_if(c, "SlipSystemsDescription::setInteractionMatrix: " + msg);
    };
    const auto im = this->getInteractionMatrixStructure();
    throw_if(this->hasInteractionMatrix(),
             "the interaction matrix "
             "has already been defined");
    throw_if(im.rank() != v.size(),
             "the number of values does "
             "not match the number of independent coefficients "
             "in the interaction matrix (" +
                 std::to_string(im.rank()) + " vs " + std::to_string(v.size()) +
                 ")");
    this->m = v;
  }  // end of SlipSystemsDescription::setInteractionMatrix

  const std::vector<long double>& SlipSystemsDescription::getInteractionMatrix()
      const {
    raise_if(!this->hasInteractionMatrix(),
             "SlipSystemsDescription::getInteractionMatrix: "
             "no interaction matrix defined");
    return this->m;
  }  // end of SlipSystemsDescription::getInteractionMatrix

  bool SlipSystemsDescription::hasDislocationsMeanFreePathInteractionMatrix()
      const {
    return !this->mfpm.empty();
  }  // end of
     // SlipSystemsDescription::hasDislocationsMeanFreePathInteractionMatrix

  void SlipSystemsDescription::setDislocationsMeanFreePathInteractionMatrix(
      const std::vector<long double>& v) {
    auto throw_if = [](const bool c, const std::string& msg) {
      raise_if(c,
               "SlipSystemsDescription::"
               "setDislocationsMeanFreePathInteractionMatrix: " +
                   msg);
    };
    const auto im = this->getInteractionMatrixStructure();
    throw_if(this->hasDislocationsMeanFreePathInteractionMatrix(),
             "the interaction matrix "
             "has already been defined");
    throw_if(im.rank() != v.size(),
             "the number of values does "
             "not match the number of independent coefficients "
             "in the interaction matrix (" +
                 std::to_string(im.rank()) + " vs " + std::to_string(v.size()) +
                 ")");
    this->mfpm = v;
  }  // end of
     // SlipSystemsDescription::setDislocationsMeanFreePathInteractionMatrix

  const std::vector<long double>&
  SlipSystemsDescription::getDislocationsMeanFreePathInteractionMatrix() const {
    raise_if(!this->hasDislocationsMeanFreePathInteractionMatrix(),
             "SlipSystemsDescription::"
             "getDislocationsMeanFreePathInteractionMatrix: "
             "no interaction matrix defined");
    return this->mfpm;
  }  // end of
     // SlipSystemsDescription::getDislocationsMeanFreePathInteractionMatrix

  template <CrystalStructure cs>
  static SlipSystemsDescription::InteractionMatrixStructure
  buildInteractionMatrix(
      const std::vector<std::vector<SlipSystemsDescription::system>>& gs) {
    using SlidingSystemsInteraction = SlipSystemsDescription::
        InteractionMatrixStructure::SlidingSystemsInteraction;
    auto ngs = std::vector<numodis::GSystem>{};
    for (const auto& gs1 : gs) {
      for (const auto& g : gs1) {
        raise_if(!g.template is<
                     typename CristalStructureSlipSystems<cs>::system_type>(),
                 "buildInteractionMatrix: unexpected system description");
        const auto gg = g.template get<
            typename CristalStructureSlipSystems<cs>::system_type>();
        ngs.push_back(to_numodis(gg.burgers, gg.plane));
      }
    }
    const auto s = typename CristalStructureSlipSystems<cs>::numodis_handler{};
    const auto h = numodis::Hardening(s, ngs);
    auto ranks =
        std::map<int,
                 std::vector<std::pair<numodis::GSystem, numodis::GSystem>>>{};
    for (const auto& g1 : ngs) {
      for (const auto& g2 : ngs) {
        ranks[h.getRankInteraction(g1, g2)].push_back({g1, g2});
      }
    }
    const auto n = static_cast<size_t>(h.getNinteractions());
    auto ims = std::vector<std::vector<SlidingSystemsInteraction>>{n};
    for (decltype(ims.size()) i = 0; i != ims.size(); ++i) {
      if (ranks.count(i) == 0) {
        continue;
      }
      for (const auto& sp1 : ranks.at(i)) {
        const auto ssi = SlidingSystemsInteraction{from_numodis(sp1.first),
                                                   from_numodis(sp1.second)};
        ims[i].push_back(std::move(ssi));
      }
    }
    return {ims};
  }

  SlipSystemsDescription::InteractionMatrixStructure::
      InteractionMatrixStructure(
          const std::vector<std::vector<SlidingSystemsInteraction>>& r)
      : ranks(r) {}  // end of
  // SlipSystemsDescription::SlipSystemsDescription::InteractionMatrixStructure

  SlipSystemsDescription::InteractionMatrixStructure::
      InteractionMatrixStructure(InteractionMatrixStructure&&) = default;

  SlipSystemsDescription::size_type
  SlipSystemsDescription::InteractionMatrixStructure::rank() const {
    return this->ranks.size();
  }  // end of SlipSystemsDescription::InteractionMatrixStructure::rank()

  SlipSystemsDescription::size_type
  SlipSystemsDescription::InteractionMatrixStructure::getRank(
      const system& g1, const system& g2) const {
    auto throw_if = [](const bool b, const std::string& msg) {
      raise_if(b,
               "SlipSystemsDescription::InteractionMatrixStructure"
               "::getRank: " +
                   msg);
    };
    auto equal = [throw_if](const system& ss1, const system& ss2) {
      if (ss1.is<system3d>()) {
        if (!ss2.is<system3d>()) {
          return false;
        }
        const auto& ss1_3d = ss1.get<system3d>();
        const auto& ss2_3d = ss2.get<system3d>();
        return ((tfel::fsalgo::equal<3u>::exe(ss1_3d.burgers.begin(),
                                              ss2_3d.burgers.begin())) &&
                (tfel::fsalgo::equal<3u>::exe(ss1_3d.plane.begin(),
                                              ss2_3d.plane.begin())));
      }
      throw_if(!ss1.is<system4d>(),
               "internal error (unsupported slip system type)");
      if (!ss2.is<system4d>()) {
        return false;
      }
      const auto& ss1_4d = ss1.get<system4d>();
      const auto& ss2_4d = ss2.get<system4d>();
      return ((tfel::fsalgo::equal<4u>::exe(ss1_4d.burgers.begin(),
                                            ss2_4d.burgers.begin())) &&
              (tfel::fsalgo::equal<4u>::exe(ss1_4d.plane.begin(),
                                            ss2_4d.plane.begin())));
    };
    auto i = size_type{};
    for (const auto& r : this->ranks) {
      for (const auto& ssi : r) {
        if ((equal(ssi.g1, g1)) && (equal(ssi.g2, g2))) {
          return i;
        }
      }
      ++i;
    }
    throw_if(true, "systems pair not found");
    return i;
  }  // end of rank

  SlipSystemsDescription::InteractionMatrixStructure
  SlipSystemsDescription::getInteractionMatrixStructure() const {
    const auto ags = this->getSlipSystems();
    if (this->cs == CrystalStructure::Cubic) {
      return buildInteractionMatrix<CrystalStructure::Cubic>(ags);
    } else if (this->cs == CrystalStructure::FCC) {
      return buildInteractionMatrix<CrystalStructure::FCC>(ags);
    } else if (this->cs == CrystalStructure::BCC) {
      return buildInteractionMatrix<CrystalStructure::BCC>(ags);
    }
    raise_if(cs != CrystalStructure::HCP,
             "SlipSystemsDescription::getInteractionMatrixStructure: "
             "internal error (unknown cristal structure)");
    return buildInteractionMatrix<CrystalStructure::HCP>(ags);
  }  // end of getInteractionMatrixStructure

  const SlipSystemsDescription::InteractionMatrixStructure::SSIContainer&
  SlipSystemsDescription::InteractionMatrixStructure::
      getSlidingSystemsInteraction() const {
    return this->ranks;
  }  // end of getSlidingSystemsInteraction

  SlipSystemsDescription::InteractionMatrixStructure::
      ~InteractionMatrixStructure() = default;

  SlipSystemsDescription::~SlipSystemsDescription() = default;

}  // end of namespace tfel::material
