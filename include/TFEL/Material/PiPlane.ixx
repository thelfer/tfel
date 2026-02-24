/*!
 * \file   include/TFEL/Material/PiPlane.ixx
 * \brief
 * \author Thomas Helfer
 * \date   14/12/2017
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_PIPLANE_IXX
#define LIB_TFEL_MATERIAL_PIPLANE_IXX

namespace tfel::material {

  template <typename stress>
  std::tuple<stress, stress> projectOnPiPlane(
      const tfel::math::tvector<3u, stress>& s) {
    using namespace tfel::math;
    using real = tfel::typetraits::base_type<stress>;
    constexpr auto zero = stress{0};
    constexpr auto isqrt2 = Cste<stress>::isqrt2;
    constexpr auto isqrt3 = Cste<stress>::isqrt3;
    constexpr const auto isqrt6 = isqrt2 * isqrt3;
    const auto n0 = tvector<3u, real>{{isqrt2, -isqrt2, zero}};
    const auto n1 = tvector<3u, real>{{-isqrt6, -isqrt6, 2 * isqrt6}};
    return std::tuple<stress, stress>{(s | n0), (s | n1)};
  }  // end of projectOnPiPlane

  template <typename stress>
  std::tuple<stress, stress> projectOnPiPlane(const stress s0,
                                              const stress s1,
                                              const stress s2) {
    const auto vp = tfel::math::tvector<3u, stress>{{s0, s1, s2}};
    return projectOnPiPlane(vp);
  }  // end of projectOnPiPlane

  template <typename StressStensor, tfel::math::stensor_common::EigenSolver es>
  std::tuple<tfel::math::numeric_type<StressStensor>,
             tfel::math::numeric_type<StressStensor>>
  projectOnPiPlane(const StressStensor& s) {
    const auto vp = s.template computeEigenValues<es>();
    return projectOnPiPlane(vp);
  }  // end of projectOnPiPlane

  template <typename stress>
  std::tuple<stress, stress, stress> buildFromPiPlane(const stress s0,
                                                      const stress s1) {
    using namespace tfel::math;
    using real = tfel::typetraits::base_type<stress>;
    constexpr auto zero = stress{0};
    constexpr auto isqrt2 = Cste<stress>::isqrt2;
    constexpr auto isqrt3 = Cste<stress>::isqrt3;
    constexpr const auto isqrt6 = isqrt2 * isqrt3;
    const auto n0 = tvector<3u, real>{{isqrt2, -isqrt2, zero}};
    const auto n1 = tvector<3u, real>{{-isqrt6, -isqrt6, 2 * isqrt6}};
    const auto s = s0 * n0 + s1 * n1;
    return std::tuple<stress, stress, stress>{s[0], s[1], s[2]};
  }  // end of buildFromPiPlane

}  // end of namespace tfel::material

#endif /* LIB_TFEL_MATERIAL_PIPLANE_IXX */
