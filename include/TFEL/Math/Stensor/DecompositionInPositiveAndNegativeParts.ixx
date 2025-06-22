
/*!
 * \file   include/TFEL/Math/Stensor/DecompositionInPositiveAndNegativeParts.ixx
 * \brief
 * \author Thomas Helfer
 * \date   22 nov. 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_STENSOR_DECOMPOSITIONINPOSITIVEANDNEGATIVEPARTSIXX
#define LIB_TFEL_MATH_STENSOR_DECOMPOSITIONINPOSITIVEANDNEGATIVEPARTSIXX

#include "TFEL/Math/General/MathConstants.hxx"

namespace tfel::math::internals {

  template <typename NumType>
  constexpr auto stensor_ppos(const NumType x) {
    return x >= NumType(0) ? x : NumType(0);
  }

  template <typename NumType>
  constexpr auto stensor_pneg(const NumType x) {
    return x <= NumType(0) ? x : NumType(0);
  }

}  // end of namespace tfel::math::internals

namespace tfel::math {

  template <typename DPPType, typename PPType, typename StensorType>
  std::enable_if_t<
      (implementsST2toST2Concept<DPPType>() &&
       implementsStensorConcept<PPType>() &&
       implementsStensorConcept<StensorType>() &&
       getSpaceDimension<DPPType>() == getSpaceDimension<StensorType>() &&
       getSpaceDimension<PPType>() == getSpaceDimension<StensorType>() &&
       getSpaceDimension<StensorType>() == 1u &&
       isAssignableTo<numeric_type<StensorType>, numeric_type<PPType>>() &&
       isAssignableTo<base_type<numeric_type<StensorType>>,
                      numeric_type<DPPType>>()),
      void>
  computeStensorPositivePartAndDerivative(DPPType& dpp,
                                          PPType& pp,
                                          const StensorType& s,
                                          const numeric_type<StensorType> eps) {
    using tfel::math::abs;
    typedef numeric_type<StensorType> NumType;
    typedef base_type<NumType> real;
    constexpr const auto one_half = real(1) / (real(2));
    dpp(0, 1) = dpp(0, 2) = dpp(1, 0) = dpp(1, 2) = dpp(2, 0) = dpp(2, 1) =
        real(0);
    if (abs(s(0)) < eps) {
      dpp(0, 0) = one_half;
      pp(0) = real(0);
    } else if (s(0) > NumType(0)) {
      dpp(0, 0) = real(1);
      pp(0) = s(0);
    } else {
      dpp(0, 0) = real(0);
      pp(0) = NumType(0);
    }
    if (abs(s(1)) < eps) {
      dpp(1, 1) = one_half;
      pp(1) = real(0);
    } else if (s(1) > NumType(0)) {
      dpp(1, 1) = real(1);
      pp(1) = s(1);
    } else {
      dpp(1, 1) = real(0);
      pp(1) = NumType(0);
    }
    if (abs(s(2)) < eps) {
      dpp(2, 2) = one_half;
      pp(2) = real(0);
    } else if (s(2) > NumType(0)) {
      dpp(2, 2) = real(1);
      pp(2) = s(2);
    } else {
      dpp(2, 2) = real(0);
      pp(2) = NumType(0);
    }
  }

  template <typename DPPType, typename PPType, typename StensorType>
  std::enable_if_t<
      (implementsST2toST2Concept<DPPType>() &&
       implementsStensorConcept<PPType>() &&
       implementsStensorConcept<StensorType>() &&
       getSpaceDimension<DPPType>() == getSpaceDimension<StensorType>() &&
       getSpaceDimension<PPType>() == getSpaceDimension<StensorType>() &&
       getSpaceDimension<StensorType>() == 2u &&
       isAssignableTo<numeric_type<StensorType>, numeric_type<PPType>>() &&
       isAssignableTo<base_type<numeric_type<StensorType>>,
                      numeric_type<DPPType>>()),
      void>
  computeStensorPositivePartAndDerivative(DPPType& dpp,
                                          PPType& pp,
                                          const StensorType& s,
                                          const numeric_type<StensorType> eps) {
    using tfel::math::abs;
    using tfel::math::internals::stensor_pneg;
    using tfel::math::internals::stensor_ppos;
    typedef numeric_type<StensorType> NumType;
    typedef base_type<NumType> real;
    constexpr auto cste = Cste<real>::sqrt2;
    constexpr const auto one_half = real(1) / (real(2));
    stensor<2u, NumType> ls(s);  // local copy of s
    stensor<2u, real> n0;
    stensor<2u, real> n1;
    stensor<2u, real> n2;
    tvector<3u, NumType> vp;
    tmatrix<3u, 3u, real> m;
    ls.computeEigenVectors(vp, m);
    stensor<2u, real>::computeEigenTensors(n0, n1, n2, m);
    const tvector<3u, real> v0 = m.template column_view<0u>();
    const tvector<3u, real> v1 = m.template column_view<1u>();
    const stensor<2u, real> n01 =
        stensor<2u, real>::buildFromVectorsSymmetricDiadicProduct(v0, v1) /
        cste;
    if (abs(vp(0) - vp(1)) < eps) {
      const NumType vpm = (vp(0) + vp(1)) * one_half;
      if (abs(vpm) < eps) {
        dpp = ((n0 ^ n0) + (n1 ^ n1) + (n01 ^ n01)) * one_half;
        pp = stensor<2u, NumType>(real(0));
      } else if (vpm > NumType(0)) {
        dpp = (n0 ^ n0) + (n1 ^ n1) + (n01 ^ n01);
        pp = vpm * (n0 + n1);
      } else {
        dpp = st2tost2<2u, real>(real(0));
        pp = stensor<2u, NumType>(real(0));
      }
    } else {
      if (abs(vp(0)) < eps) {
        dpp = (n0 ^ n0) * one_half;
        pp = stensor<2u, NumType>(real(0));
      } else if (vp(0) > NumType(0)) {
        dpp = (n0 ^ n0);
        pp = vp(0) * n0;
      } else {
        dpp = st2tost2<2u, real>(real(0));
        pp = stensor<2u, NumType>(real(0));
      }
      if (abs(vp(1)) < eps) {
        dpp += (n1 ^ n1) * one_half;
      } else if (vp(1) > NumType(0)) {
        dpp += (n1 ^ n1);
        pp += vp(1) * n1;
      }
      dpp += (stensor_ppos(vp(1)) - stensor_ppos(vp(0))) / (vp(1) - vp(0)) *
             (n01 ^ n01);
    }
    if (abs(vp(2)) < eps) {
      dpp += (n2 ^ n2) * one_half;
    } else if (vp(2) > 0) {
      dpp += (n2 ^ n2);
      pp(2) += vp(2);
    }
  }

  template <typename DPPType, typename PPType, typename StensorType>
  typename std::enable_if<
      (implementsST2toST2Concept<DPPType>() &&
       implementsStensorConcept<PPType>() &&
       implementsStensorConcept<StensorType>() &&
       getSpaceDimension<DPPType>() == getSpaceDimension<StensorType>() &&
       getSpaceDimension<PPType>() == getSpaceDimension<StensorType>() &&
       getSpaceDimension<StensorType>() == 3u &&
       isAssignableTo<numeric_type<StensorType>, numeric_type<PPType>>() &&
       isAssignableTo<base_type<numeric_type<StensorType>>,
                      numeric_type<DPPType>>()),
      void>::type
  computeStensorPositivePartAndDerivative(DPPType& dpp,
                                          PPType& pp,
                                          const StensorType& s,
                                          const numeric_type<StensorType> eps) {
    using tfel::math::internals::stensor_pneg;
    using tfel::math::internals::stensor_ppos;
    typedef numeric_type<StensorType> NumType;
    typedef base_type<NumType> real;
    constexpr auto cste = Cste<real>::sqrt2;
    constexpr const auto one_half = real(1) / (real(2));
    stensor<3u, NumType> ls(s);  // local copy of s
    stensor<3u, real> n0;
    stensor<3u, real> n1;
    stensor<3u, real> n2;
    tvector<3u, NumType> vp;
    tmatrix<3u, 3u, real> m;
    ls.computeEigenVectors(vp, m);
    stensor<3u, real>::computeEigenTensors(n0, n1, n2, m);
    if ((abs(vp(0) - vp(1)) < eps) && (abs(vp(0) - vp(2)) < eps)) {
      NumType vpm = (vp(0) + vp(1) + vp(2)) / 3;
      if (abs(vpm) < eps) {
        dpp = st2tost2<3u, real>::Id() * one_half;
        pp = stensor<3u, NumType>(NumType(0));
      } else if (vpm > NumType(0)) {
        dpp = st2tost2<3u, real>::Id();
        pp = s;
      } else {
        dpp = st2tost2<3u, real>(real(0));
        pp = stensor<3u, real>(real(0));
      }
    } else {
      const tvector<3u, real> v0 = m.template column_view<0u>();
      const tvector<3u, real> v1 = m.template column_view<1u>();
      const tvector<3u, real> v2 = m.template column_view<2u>();
      const stensor<3u, real> n01 =
          stensor<3u, real>::buildFromVectorsSymmetricDiadicProduct(v0, v1) /
          cste;
      const stensor<3u, real> n02 =
          stensor<3u, real>::buildFromVectorsSymmetricDiadicProduct(v0, v2) /
          cste;
      const stensor<3u, real> n12 =
          stensor<3u, real>::buildFromVectorsSymmetricDiadicProduct(v1, v2) /
          cste;
      if (abs(vp(0) - vp(1)) < eps) {
        NumType vpm = (vp(0) + vp(1)) * one_half;
        if (abs(vpm) < eps) {
          dpp = ((n0 ^ n0) + (n1 ^ n1) + (n01 ^ n01)) * one_half;
          pp = stensor<3u, NumType>(NumType(0));
        } else if (vpm > NumType(0)) {
          dpp = (n0 ^ n0) + (n1 ^ n1) + (n01 ^ n01);
          pp = vpm * (n0 + n1);
        } else {
          dpp = st2tost2<3u, real>(real(0));
          pp = stensor<3u, NumType>(NumType(0));
        }
        if (abs(vp(2)) < eps) {
          dpp += (n2 ^ n2) * one_half;
        } else if (vp(2) > NumType(0)) {
          dpp += n2 ^ n2;
          pp += vp(2) * n2;
        }
        dpp += (stensor_ppos(vpm) - stensor_ppos(vp(2))) / (vpm - vp(2)) *
               ((n02 ^ n02) + (n12 ^ n12));
      } else if (abs(vp(0) - vp(2)) < eps) {
        NumType vpm = (vp(0) + vp(2)) * one_half;
        if (abs(vpm) < eps) {
          dpp = ((n0 ^ n0) + (n2 ^ n2) + (n02 ^ n02)) * one_half;
          pp = stensor<3u, NumType>(NumType(0));
        } else if (vpm > NumType(0)) {
          dpp = (n0 ^ n0) + (n2 ^ n2) + (n02 ^ n02);
          pp = vpm * (n0 + n2);
        } else {
          dpp = st2tost2<3u, real>(real(0));
          pp = stensor<3u, NumType>(NumType(0));
        }
        if (abs(vp(1)) < eps) {
          dpp += (n1 ^ n1) * one_half;
        } else if (vp(1) > NumType(0)) {
          dpp += (n1 ^ n1);
          pp += vp(1) * n1;
        }
        dpp += (stensor_ppos(vpm) - stensor_ppos(vp(1))) / (vpm - vp(1)) *
               ((n01 ^ n01) + (n12 ^ n12));
      } else if (abs(vp(1) - vp(2)) < eps) {
        NumType vpm = (vp(1) + vp(2)) * one_half;
        if (abs(vpm) < eps) {
          dpp = ((n1 ^ n1) + (n2 ^ n2) + (n12 ^ n12)) * one_half;
          pp = stensor<3u, NumType>(NumType(0));
        } else if (vpm > NumType(0)) {
          dpp = (n1 ^ n1) + (n2 ^ n2) + (n12 ^ n12);
          pp = vpm * (n1 + n2);
        } else {
          dpp = st2tost2<3u, real>(real(0));
          pp = stensor<3u, NumType>(NumType(0));
        }
        if (abs(vp(0)) < eps) {
          dpp += (n0 ^ n0) * one_half;
        } else if (vp(0) > NumType(0)) {
          dpp += (n0 ^ n0);
          pp += vp(0) * n0;
        }
        dpp += (stensor_ppos(vpm) - stensor_ppos(vp(0))) / (vpm - vp(0)) *
               ((n01 ^ n01) + (n02 ^ n02));
      } else {
        // all eigenvalues are distincts
        if (abs(vp(0)) < eps) {
          dpp = (n0 ^ n0) * one_half;
          pp = stensor<3u, NumType>(NumType(0));
        } else if (vp(0) > NumType(0)) {
          dpp = (n0 ^ n0) + vp(0) / (vp(0) - vp(1)) * (n01 ^ n01) +
                vp(0) / (vp(0) - vp(2)) * (n02 ^ n02);
          pp = vp(0) * n0;
        } else {
          dpp = st2tost2<3u, real>(real(0));
          pp = stensor<3u, NumType>(NumType(0));
        }
        if (abs(vp(1)) < eps) {
          dpp += (n1 ^ n1) * one_half;
        } else if (vp(1) > NumType(0)) {
          dpp += (n1 ^ n1) + vp(1) / (vp(1) - vp(0)) * (n01 ^ n01) +
                 vp(1) / (vp(1) - vp(2)) * (n12 ^ n12);
          pp += vp(1) * n1;
        } else {
          dpp += st2tost2<3u, real>(real(0));
          pp += stensor<3u, NumType>(NumType(0));
        }
        if (abs(vp(2)) < eps) {
          dpp += (n2 ^ n2) * one_half;
        } else if (vp(2) > NumType(0)) {
          dpp += (n2 ^ n2) + vp(2) / (vp(2) - vp(0)) * (n02 ^ n02) +
                 vp(2) / (vp(2) - vp(1)) * (n12 ^ n12);
          pp += vp(2) * n2;
        } else {
          dpp += st2tost2<3u, real>(real(0));
          pp += stensor<3u, NumType>(NumType(0));
        }
      }
    }
  }

  template <typename DPPType,
            typename DNPType,
            typename PPType,
            typename NPType,
            typename StensorType>
  typename std::enable_if<
      (implementsST2toST2Concept<DPPType>() &&
       implementsST2toST2Concept<DNPType>() &&
       implementsStensorConcept<PPType>() &&
       implementsStensorConcept<NPType>() &&
       implementsStensorConcept<StensorType>() &&
       getSpaceDimension<DPPType>() == getSpaceDimension<StensorType>() &&
       getSpaceDimension<DNPType>() == getSpaceDimension<StensorType>() &&
       getSpaceDimension<PPType>() == getSpaceDimension<StensorType>() &&
       getSpaceDimension<NPType>() == getSpaceDimension<StensorType>() &&
       getSpaceDimension<StensorType>() == 1u &&
       isAssignableTo<numeric_type<StensorType>, numeric_type<PPType>>() &&
       isAssignableTo<numeric_type<StensorType>, numeric_type<NPType>>() &&
       isAssignableTo<base_type<numeric_type<StensorType>>,
                      numeric_type<DPPType>>() &&
       isAssignableTo<base_type<numeric_type<StensorType>>,
                      numeric_type<DNPType>>()),
      void>::type
  computeStensorDecompositionInPositiveAndNegativeParts(
      DPPType& dpp,
      DNPType& dnp,
      PPType& pp,
      NPType& np,
      const StensorType& s,
      const numeric_type<StensorType> eps) {
    using tfel::math::abs;
    typedef numeric_type<StensorType> NumType;
    typedef base_type<NumType> real;
    const real one_half = real(1) / (real(2));
    dpp(0, 1) = dpp(0, 2) = dpp(1, 0) = dpp(1, 2) = dpp(2, 0) = dpp(2, 1) =
        real(0);
    dnp(0, 1) = dnp(0, 2) = dnp(1, 0) = dnp(1, 2) = dnp(2, 0) = dnp(2, 1) =
        real(0);
    if (abs(s(0)) < eps) {
      dpp(0, 0) = one_half;
      dnp(0, 0) = one_half;
      pp(0) = np(0) = real(0);
    } else if (s(0) > NumType(0)) {
      dpp(0, 0) = real(1);
      dnp(0, 0) = real(0);
      pp(0) = s(0);
      np(0) = NumType(0);
    } else {
      dpp(0, 0) = real(0);
      dnp(0, 0) = real(1);
      pp(0) = NumType(0);
      np(0) = s(0);
    }
    if (abs(s(1)) < eps) {
      dpp(1, 1) = one_half;
      dnp(1, 1) = one_half;
      pp(1) = np(1) = real(0);
    } else if (s(1) > NumType(0)) {
      dpp(1, 1) = real(1);
      dnp(1, 1) = real(0);
      pp(1) = s(1);
      np(1) = NumType(0);
    } else {
      dpp(1, 1) = real(0);
      dnp(1, 1) = real(1);
      pp(1) = NumType(0);
      np(1) = s(1);
    }
    if (abs(s(2)) < eps) {
      dpp(2, 2) = one_half;
      dnp(2, 2) = one_half;
      pp(2) = np(2) = real(0);
    } else if (s(2) > NumType(0)) {
      dpp(2, 2) = real(1);
      dnp(2, 2) = real(0);
      pp(2) = s(2);
      np(2) = NumType(0);
    } else {
      dpp(2, 2) = real(0);
      dnp(2, 2) = real(1);
      pp(2) = NumType(0);
      np(2) = s(2);
    }
  }  // end of computeStensorDecompositionInPositiveAndNegativeParts

  template <typename DPPType,
            typename DNPType,
            typename PPType,
            typename NPType,
            typename StensorType>
  typename std::enable_if<
      (implementsST2toST2Concept<DPPType>() &&
       implementsST2toST2Concept<DNPType>() &&
       implementsStensorConcept<PPType>() &&
       implementsStensorConcept<NPType>() &&
       implementsStensorConcept<StensorType>() &&
       getSpaceDimension<DPPType>() == getSpaceDimension<StensorType>() &&
       getSpaceDimension<DNPType>() == getSpaceDimension<StensorType>() &&
       getSpaceDimension<PPType>() == getSpaceDimension<StensorType>() &&
       getSpaceDimension<NPType>() == getSpaceDimension<StensorType>() &&
       getSpaceDimension<StensorType>() == 2u &&
       isAssignableTo<numeric_type<StensorType>, numeric_type<PPType>>() &&
       isAssignableTo<numeric_type<StensorType>, numeric_type<NPType>>() &&
       isAssignableTo<base_type<numeric_type<StensorType>>,
                      numeric_type<DPPType>>() &&
       isAssignableTo<base_type<numeric_type<StensorType>>,
                      numeric_type<DNPType>>()),
      void>::type
  computeStensorDecompositionInPositiveAndNegativeParts(
      DPPType& dpp,
      DNPType& dnp,
      PPType& pp,
      NPType& np,
      const StensorType& s,
      const numeric_type<StensorType> eps) {
    using tfel::math::abs;
    using tfel::math::internals::stensor_pneg;
    using tfel::math::internals::stensor_ppos;
    typedef numeric_type<StensorType> NumType;
    typedef base_type<NumType> real;
    constexpr auto cste = Cste<real>::sqrt2;
    constexpr const auto one_half = real(1) / (real(2));
    stensor<2u, NumType> ls(s);  // local copy of s
    stensor<2u, real> n0;
    stensor<2u, real> n1;
    stensor<2u, real> n2;
    tvector<3u, NumType> vp;
    tmatrix<3u, 3u, real> m;
    ls.computeEigenVectors(vp, m);
    stensor<2u, real>::computeEigenTensors(n0, n1, n2, m);
    const tvector<3u, real> v0 = m.template column_view<0u>();
    const tvector<3u, real> v1 = m.template column_view<1u>();
    const stensor<2u, real> n01 =
        stensor<2u, real>::buildFromVectorsSymmetricDiadicProduct(v0, v1) /
        cste;
    if (abs(vp(0) - vp(1)) < eps) {
      const NumType vpm = (vp(0) + vp(1)) * one_half;
      if (abs(vpm) < eps) {
        dpp = dnp = ((n0 ^ n0) + (n1 ^ n1) + (n01 ^ n01)) * one_half;
        pp = np = stensor<2u, NumType>(real(0));
      } else if (vpm > NumType(0)) {
        dpp = (n0 ^ n0) + (n1 ^ n1) + (n01 ^ n01);
        dnp = st2tost2<2u, real>(real(0));
        pp = vpm * (n0 + n1);
        np = stensor<2u, NumType>(real(0));
      } else {
        dpp = st2tost2<2u, real>(real(0));
        dnp = (n0 ^ n0) + (n1 ^ n1) + (n01 ^ n01);
        pp = stensor<2u, NumType>(real(0));
        np = vpm * (n0 + n1);
      }
    } else {
      if (abs(vp(0)) < eps) {
        dpp = dnp = (n0 ^ n0) * one_half;
        pp = np = stensor<2u, NumType>(real(0));
      } else if (vp(0) > NumType(0)) {
        dpp = (n0 ^ n0);
        dnp = st2tost2<2u, real>(real(0));
        pp = vp(0) * n0;
        np = stensor<2u, NumType>(real(0));
      } else {
        dpp = st2tost2<2u, real>(real(0));
        dnp = (n0 ^ n0);
        pp = stensor<2u, NumType>(real(0));
        np = vp(0) * n0;
      }
      if (abs(vp(1)) < eps) {
        dpp += (n1 ^ n1) * one_half;
        dnp += (n1 ^ n1) * one_half;
      } else if (vp(1) > NumType(0)) {
        dpp += (n1 ^ n1);
        pp += vp(1) * n1;
      } else {
        dnp += (n1 ^ n1);
        np += vp(1) * n1;
      }
      dpp += (stensor_ppos(vp(1)) - stensor_ppos(vp(0))) / (vp(1) - vp(0)) *
             (n01 ^ n01);
      dnp += (stensor_pneg(vp(1)) - stensor_pneg(vp(0))) / (vp(1) - vp(0)) *
             (n01 ^ n01);
    }
    if (abs(vp(2)) < eps) {
      dpp += (n2 ^ n2) * one_half;
      dnp += (n2 ^ n2) * one_half;
    } else if (vp(2) > 0) {
      dpp += (n2 ^ n2);
      pp(2) += vp(2);
    } else {
      dnp += (n2 ^ n2);
      np(2) += vp(2);
    }
  }  // end of computeStensorDecompositionInPositiveAndNegativeParts

  template <typename DPPType,
            typename DNPType,
            typename PPType,
            typename NPType,
            typename StensorType>
  std::enable_if_t<
      (implementsST2toST2Concept<DPPType>() &&
       implementsST2toST2Concept<DNPType>() &&
       implementsStensorConcept<PPType>() &&
       implementsStensorConcept<NPType>() &&
       implementsStensorConcept<StensorType>() &&
       getSpaceDimension<DPPType>() == getSpaceDimension<StensorType>() &&
       getSpaceDimension<DNPType>() == getSpaceDimension<StensorType>() &&
       getSpaceDimension<PPType>() == getSpaceDimension<StensorType>() &&
       getSpaceDimension<NPType>() == getSpaceDimension<StensorType>() &&
       getSpaceDimension<StensorType>() == 3u &&
       isAssignableTo<numeric_type<StensorType>, numeric_type<PPType>>() &&
       isAssignableTo<numeric_type<StensorType>, numeric_type<NPType>>() &&
       isAssignableTo<base_type<numeric_type<StensorType>>,
                      numeric_type<DPPType>>() &&
       isAssignableTo<base_type<numeric_type<StensorType>>,
                      numeric_type<DNPType>>()),
      void>
  computeStensorDecompositionInPositiveAndNegativeParts(
      DPPType& dpp,
      DNPType& dnp,
      PPType& pp,
      NPType& np,
      const StensorType& s,
      const numeric_type<StensorType> eps) {
    using tfel::math::internals::stensor_pneg;
    using tfel::math::internals::stensor_ppos;
    typedef numeric_type<StensorType> NumType;
    typedef base_type<NumType> real;
    constexpr auto cste = Cste<real>::sqrt2;
    constexpr const auto one_half = real(1) / (real(2));
    stensor<3u, NumType> ls(s);  // local copy of s
    stensor<3u, real> n0;
    stensor<3u, real> n1;
    stensor<3u, real> n2;
    tvector<3u, NumType> vp;
    tmatrix<3u, 3u, real> m;
    ls.computeEigenVectors(vp, m);
    stensor<3u, real>::computeEigenTensors(n0, n1, n2, m);
    if ((abs(vp(0) - vp(1)) < eps) && (abs(vp(0) - vp(2)) < eps)) {
      NumType vpm = (vp(0) + vp(1) + vp(2)) / 3;
      if (abs(vpm) < eps) {
        dpp = dnp = st2tost2<3u, real>::Id() * one_half;
        pp = np = stensor<3u, NumType>(NumType(0));
      } else if (vpm > NumType(0)) {
        dpp = st2tost2<3u, real>::Id();
        pp = s;
        dnp = st2tost2<3u, real>(real(0));
        np = stensor<3u, real>(real(0));
      } else {
        dpp = st2tost2<3u, real>(real(0));
        pp = stensor<3u, real>(real(0));
        dnp = st2tost2<3u, real>::Id();
        np = s;
      }
    } else {
      const tvector<3u, real> v0 = m.template column_view<0u>();
      const tvector<3u, real> v1 = m.template column_view<1u>();
      const tvector<3u, real> v2 = m.template column_view<2u>();
      const stensor<3u, real> n01 =
          stensor<3u, real>::buildFromVectorsSymmetricDiadicProduct(v0, v1) /
          cste;
      const stensor<3u, real> n02 =
          stensor<3u, real>::buildFromVectorsSymmetricDiadicProduct(v0, v2) /
          cste;
      const stensor<3u, real> n12 =
          stensor<3u, real>::buildFromVectorsSymmetricDiadicProduct(v1, v2) /
          cste;
      if (abs(vp(0) - vp(1)) < eps) {
        NumType vpm = (vp(0) + vp(1)) * one_half;
        if (abs(vpm) < eps) {
          dpp = dnp = ((n0 ^ n0) + (n1 ^ n1) + (n01 ^ n01)) * one_half;
          pp = np = stensor<3u, NumType>(NumType(0));
        } else if (vpm > NumType(0)) {
          dpp = (n0 ^ n0) + (n1 ^ n1) + (n01 ^ n01);
          dnp = st2tost2<3u, real>(real(0));
          pp = vpm * (n0 + n1);
          np = stensor<3u, NumType>(NumType(0));
        } else {
          dpp = st2tost2<3u, real>(real(0));
          dnp = (n0 ^ n0) + (n1 ^ n1) + (n01 ^ n01);
          pp = stensor<3u, NumType>(NumType(0));
          np = vpm * (n0 + n1);
        }
        if (abs(vp(2)) < eps) {
          dpp += (n2 ^ n2) * one_half;
          dnp += (n2 ^ n2) * one_half;
        } else if (vp(2) > NumType(0)) {
          dpp += n2 ^ n2;
          pp += vp(2) * n2;
        } else {
          dnp += n2 ^ n2;
          np += vp(2) * n2;
        }
        dpp += (stensor_ppos(vpm) - stensor_ppos(vp(2))) / (vpm - vp(2)) *
               ((n02 ^ n02) + (n12 ^ n12));
        dnp += (stensor_pneg(vpm) - stensor_pneg(vp(2))) / (vpm - vp(2)) *
               ((n02 ^ n02) + (n12 ^ n12));
      } else if (abs(vp(0) - vp(2)) < eps) {
        NumType vpm = (vp(0) + vp(2)) * one_half;
        if (abs(vpm) < eps) {
          dpp = dnp = ((n0 ^ n0) + (n2 ^ n2) + (n02 ^ n02)) * one_half;
          pp = np = stensor<3u, NumType>(NumType(0));
        } else if (vpm > NumType(0)) {
          dpp = (n0 ^ n0) + (n2 ^ n2) + (n02 ^ n02);
          dnp = st2tost2<3u, real>(real(0));
          pp = vpm * (n0 + n2);
          np = stensor<3u, NumType>(NumType(0));
        } else {
          dpp = st2tost2<3u, real>(real(0));
          dnp = (n0 ^ n0) + (n2 ^ n2) + (n02 ^ n02);
          pp = stensor<3u, NumType>(NumType(0));
          np = vpm * (n0 + n2);
        }
        if (abs(vp(1)) < eps) {
          dpp += (n1 ^ n1) * one_half;
          dnp += (n1 ^ n1) * one_half;
        } else if (vp(1) > NumType(0)) {
          dpp += (n1 ^ n1);
          pp += vp(1) * n1;
        } else {
          dnp += (n1 ^ n1);
          np += vp(1) * n1;
        }
        dpp += (stensor_ppos(vpm) - stensor_ppos(vp(1))) / (vpm - vp(1)) *
               ((n01 ^ n01) + (n12 ^ n12));
        dnp += (stensor_pneg(vpm) - stensor_pneg(vp(1))) / (vpm - vp(1)) *
               ((n01 ^ n01) + (n12 ^ n12));
      } else if (abs(vp(1) - vp(2)) < eps) {
        NumType vpm = (vp(1) + vp(2)) * one_half;
        if (abs(vpm) < eps) {
          dpp = dnp = ((n1 ^ n1) + (n2 ^ n2) + (n12 ^ n12)) * one_half;
          pp = np = stensor<3u, NumType>(NumType(0));
        } else if (vpm > NumType(0)) {
          dpp = (n1 ^ n1) + (n2 ^ n2) + (n12 ^ n12);
          dnp = st2tost2<3u, real>(real(0));
          pp = vpm * (n1 + n2);
          np = stensor<3u, NumType>(NumType(0));
        } else {
          dpp = st2tost2<3u, real>(real(0));
          dnp = (n1 ^ n1) + (n2 ^ n2) + (n12 ^ n12);
          pp = stensor<3u, NumType>(NumType(0));
          np = vpm * (n1 + n2);
        }
        if (abs(vp(0)) < eps) {
          dpp += (n0 ^ n0) * one_half;
          dnp += (n0 ^ n0) * one_half;
        } else if (vp(0) > NumType(0)) {
          dpp += (n0 ^ n0);
          pp += vp(0) * n0;
        } else {
          dnp += (n0 ^ n0);
          np += vp(0) * n0;
        }
        dpp += (stensor_ppos(vpm) - stensor_ppos(vp(0))) / (vpm - vp(0)) *
               ((n01 ^ n01) + (n02 ^ n02));
        dnp += (stensor_pneg(vpm) - stensor_pneg(vp(0))) / (vpm - vp(0)) *
               ((n01 ^ n01) + (n02 ^ n02));
      } else {
        // all eigenvalues are distincts
        if (abs(vp(0)) < eps) {
          dpp = (n0 ^ n0) * one_half;
          dnp = (n0 ^ n0) * one_half;
          pp = stensor<3u, NumType>(NumType(0));
          np = stensor<3u, NumType>(NumType(0));
        } else if (vp(0) > NumType(0)) {
          dpp = (n0 ^ n0) + vp(0) / (vp(0) - vp(1)) * (n01 ^ n01) +
                vp(0) / (vp(0) - vp(2)) * (n02 ^ n02);
          dnp = st2tost2<3u, real>(real(0));
          pp = vp(0) * n0;
          np = stensor<3u, NumType>(NumType(0));
        } else {
          dpp = st2tost2<3u, real>(real(0));
          dnp = (n0 ^ n0) + vp(0) / (vp(0) - vp(1)) * (n01 ^ n01) +
                vp(0) / (vp(0) - vp(2)) * (n02 ^ n02);
          pp = stensor<3u, NumType>(NumType(0));
          np = vp(0) * n0;
        }
        if (abs(vp(1)) < eps) {
          dpp += (n1 ^ n1) * one_half;
          dnp += (n1 ^ n1) * one_half;
        } else if (vp(1) > NumType(0)) {
          dpp += (n1 ^ n1) + vp(1) / (vp(1) - vp(0)) * (n01 ^ n01) +
                 vp(1) / (vp(1) - vp(2)) * (n12 ^ n12);
          dnp += st2tost2<3u, real>(real(0));
          pp += vp(1) * n1;
          np += stensor<3u, NumType>(NumType(0));
        } else {
          dpp += st2tost2<3u, real>(real(0));
          dnp += (n1 ^ n1) + vp(1) / (vp(1) - vp(0)) * (n01 ^ n01) +
                 vp(1) / (vp(1) - vp(2)) * (n12 ^ n12);
          pp += stensor<3u, NumType>(NumType(0));
          np += vp(1) * n1;
        }
        if (abs(vp(2)) < eps) {
          dpp += (n2 ^ n2) * one_half;
          dnp += (n2 ^ n2) * one_half;
        } else if (vp(2) > NumType(0)) {
          dpp += (n2 ^ n2) + vp(2) / (vp(2) - vp(0)) * (n02 ^ n02) +
                 vp(2) / (vp(2) - vp(1)) * (n12 ^ n12);
          dnp += st2tost2<3u, real>(real(0));
          pp += vp(2) * n2;
          np += stensor<3u, NumType>(NumType(0));
        } else {
          dpp += st2tost2<3u, real>(real(0));
          dnp += (n2 ^ n2) + vp(2) / (vp(2) - vp(1)) * (n02 ^ n02) +
                 vp(2) / (vp(2) - vp(1)) * (n12 ^ n12);
          pp += stensor<3u, NumType>(NumType(0));
          np += vp(2) * n2;
        }
      }
    }
  }  // end of computeStensorDecompositionInPositiveAndNegativeParts

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_STENSOR_DECOMPOSITIONINPOSITIVEANDNEGATIVEPARTSIXX */
