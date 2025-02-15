/*!
 * \file   include/TFEL/Math/ST2toST2/ST2toST2ConceptPushForward.ixx
 * \brief
 * \author Thomas Helfer
 * \date   15 avril 2016
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_ST2TOST2_PUSH_FORWARDIXX
#define LIB_TFEL_MATH_ST2TOST2_PUSH_FORWARDIXX

#include "TFEL/Math/General/MathConstants.hxx"

namespace tfel::math {

  template <ST2toST2Concept ST2toST2Type,
            ST2toST2Concept ST2toST2Type2,
            TensorConcept TensorType>
  TFEL_HOST_DEVICE constexpr void push_forward(ST2toST2Type& Ct,
                                               const ST2toST2Type2& C,
                                               const TensorType& F) noexcept  //
      requires(getSpaceDimension<ST2toST2Type>() ==
                   getSpaceDimension<ST2toST2Type2>() &&
               getSpaceDimension<ST2toST2Type>() ==
                   getSpaceDimension<TensorType>()) {
    constexpr auto N = getSpaceDimension<ST2toST2Type>();
    if constexpr (N == 1) {
      const auto C0 = F[0] * F[0];
      const auto C1 = F[1] * F[1];
      const auto C2 = F[2] * F[2];
      Ct(0, 0) = C0 * C0 * C(0, 0);
      Ct(0, 1) = C0 * C1 * C(0, 1);
      Ct(0, 2) = C0 * C2 * C(0, 2);
      Ct(1, 0) = C1 * C0 * C(1, 0);
      Ct(1, 1) = C1 * C1 * C(1, 1);
      Ct(1, 2) = C1 * C2 * C(1, 2);
      Ct(2, 0) = C2 * C0 * C(2, 0);
      Ct(2, 1) = C2 * C1 * C(2, 1);
      Ct(2, 2) = C2 * C2 * C(2, 2);
    } else if constexpr (N == 2) {
      using NumType = numeric_type<ST2toST2Type>;
      constexpr auto cste = Cste<NumType>::sqrt2;
      constexpr auto icste = Cste<NumType>::isqrt2;
      Ct(0, 0) = F[0] * F[0] * F[0] * F[0] * C(0, 0) +
                 F[0] * F[0] * F[0] * F[3] * C(0, 3) * icste +
                 F[0] * F[0] * F[3] * F[0] * C(0, 3) * icste +
                 F[0] * F[0] * F[3] * F[3] * C(0, 1) +
                 F[0] * F[3] * F[0] * F[0] * C(3, 0) * icste +
                 F[0] * F[3] * F[0] * F[3] * C(3, 3) / 2 +
                 F[0] * F[3] * F[3] * F[0] * C(3, 3) / 2 +
                 F[0] * F[3] * F[3] * F[3] * C(3, 1) * icste +
                 F[3] * F[0] * F[0] * F[0] * C(3, 0) * icste +
                 F[3] * F[0] * F[0] * F[3] * C(3, 3) / 2 +
                 F[3] * F[0] * F[3] * F[0] * C(3, 3) / 2 +
                 F[3] * F[0] * F[3] * F[3] * C(3, 1) * icste +
                 F[3] * F[3] * F[0] * F[0] * C(1, 0) +
                 F[3] * F[3] * F[0] * F[3] * C(1, 3) * icste +
                 F[3] * F[3] * F[3] * F[0] * C(1, 3) * icste +
                 F[3] * F[3] * F[3] * F[3] * C(1, 1);
      Ct(0, 3) = F[0] * F[0] * F[0] * F[4] * C(0, 0) * cste +
                 F[0] * F[0] * F[0] * F[1] * C(0, 3) +
                 F[0] * F[0] * F[3] * F[4] * C(0, 3) +
                 F[0] * F[0] * F[3] * F[1] * C(0, 1) * cste +
                 F[0] * F[3] * F[0] * F[4] * C(3, 0) +
                 F[0] * F[3] * F[0] * F[1] * C(3, 3) * icste +
                 F[0] * F[3] * F[3] * F[4] * C(3, 3) * icste +
                 F[0] * F[3] * F[3] * F[1] * C(3, 1) +
                 F[3] * F[0] * F[0] * F[4] * C(3, 0) +
                 F[3] * F[0] * F[0] * F[1] * C(3, 3) * icste +
                 F[3] * F[0] * F[3] * F[4] * C(3, 3) * icste +
                 F[3] * F[0] * F[3] * F[1] * C(3, 1) +
                 F[3] * F[3] * F[0] * F[4] * C(1, 0) * cste +
                 F[3] * F[3] * F[0] * F[1] * C(1, 3) +
                 F[3] * F[3] * F[3] * F[4] * C(1, 3) +
                 F[3] * F[3] * F[3] * F[1] * C(1, 1) * cste;
      Ct(0, 1) = F[0] * F[0] * F[4] * F[4] * C(0, 0) +
                 F[0] * F[0] * F[4] * F[1] * C(0, 3) * icste +
                 F[0] * F[0] * F[1] * F[4] * C(0, 3) * icste +
                 F[0] * F[0] * F[1] * F[1] * C(0, 1) +
                 F[0] * F[3] * F[4] * F[4] * C(3, 0) * icste +
                 F[0] * F[3] * F[4] * F[1] * C(3, 3) / 2 +
                 F[0] * F[3] * F[1] * F[4] * C(3, 3) / 2 +
                 F[0] * F[3] * F[1] * F[1] * C(3, 1) * icste +
                 F[3] * F[0] * F[4] * F[4] * C(3, 0) * icste +
                 F[3] * F[0] * F[4] * F[1] * C(3, 3) / 2 +
                 F[3] * F[0] * F[1] * F[4] * C(3, 3) / 2 +
                 F[3] * F[0] * F[1] * F[1] * C(3, 1) * icste +
                 F[3] * F[3] * F[4] * F[4] * C(1, 0) +
                 F[3] * F[3] * F[4] * F[1] * C(1, 3) * icste +
                 F[3] * F[3] * F[1] * F[4] * C(1, 3) * icste +
                 F[3] * F[3] * F[1] * F[1] * C(1, 1);
      Ct(0, 2) = F[0] * F[0] * F[2] * F[2] * C(0, 2) +
                 F[0] * F[3] * F[2] * F[2] * C(3, 2) * icste +
                 F[3] * F[0] * F[2] * F[2] * C(3, 2) * icste +
                 F[3] * F[3] * F[2] * F[2] * C(1, 2);
      Ct(3, 0) = F[0] * F[4] * F[0] * F[0] * C(0, 0) * cste +
                 F[0] * F[4] * F[0] * F[3] * C(0, 3) +
                 F[0] * F[4] * F[3] * F[0] * C(0, 3) +
                 F[0] * F[4] * F[3] * F[3] * C(0, 1) * cste +
                 F[0] * F[1] * F[0] * F[0] * C(3, 0) +
                 F[0] * F[1] * F[0] * F[3] * C(3, 3) * icste +
                 F[0] * F[1] * F[3] * F[0] * C(3, 3) * icste +
                 F[0] * F[1] * F[3] * F[3] * C(3, 1) +
                 F[3] * F[4] * F[0] * F[0] * C(3, 0) +
                 F[3] * F[4] * F[0] * F[3] * C(3, 3) * icste +
                 F[3] * F[4] * F[3] * F[0] * C(3, 3) * icste +
                 F[3] * F[4] * F[3] * F[3] * C(3, 1) +
                 F[3] * F[1] * F[0] * F[0] * C(1, 0) * cste +
                 F[3] * F[1] * F[0] * F[3] * C(1, 3) +
                 F[3] * F[1] * F[3] * F[0] * C(1, 3) +
                 F[3] * F[1] * F[3] * F[3] * C(1, 1) * cste;
      Ct(3, 3) = F[0] * F[4] * F[0] * F[4] * C(0, 0) * 2 +
                 F[0] * F[4] * F[0] * F[1] * C(0, 3) * cste +
                 F[0] * F[4] * F[3] * F[4] * C(0, 3) * cste +
                 F[0] * F[4] * F[3] * F[1] * C(0, 1) * 2 +
                 F[0] * F[1] * F[0] * F[4] * C(3, 0) * cste +
                 F[0] * F[1] * F[0] * F[1] * C(3, 3) +
                 F[0] * F[1] * F[3] * F[4] * C(3, 3) +
                 F[0] * F[1] * F[3] * F[1] * C(3, 1) * cste +
                 F[3] * F[4] * F[0] * F[4] * C(3, 0) * cste +
                 F[3] * F[4] * F[0] * F[1] * C(3, 3) +
                 F[3] * F[4] * F[3] * F[4] * C(3, 3) +
                 F[3] * F[4] * F[3] * F[1] * C(3, 1) * cste +
                 F[3] * F[1] * F[0] * F[4] * C(1, 0) * 2 +
                 F[3] * F[1] * F[0] * F[1] * C(1, 3) * cste +
                 F[3] * F[1] * F[3] * F[4] * C(1, 3) * cste +
                 F[3] * F[1] * F[3] * F[1] * C(1, 1) * 2;
      Ct(3, 1) = F[0] * F[4] * F[4] * F[4] * C(0, 0) * cste +
                 F[0] * F[4] * F[4] * F[1] * C(0, 3) +
                 F[0] * F[4] * F[1] * F[4] * C(0, 3) +
                 F[0] * F[4] * F[1] * F[1] * C(0, 1) * cste +
                 F[0] * F[1] * F[4] * F[4] * C(3, 0) +
                 F[0] * F[1] * F[4] * F[1] * C(3, 3) * icste +
                 F[0] * F[1] * F[1] * F[4] * C(3, 3) * icste +
                 F[0] * F[1] * F[1] * F[1] * C(3, 1) +
                 F[3] * F[4] * F[4] * F[4] * C(3, 0) +
                 F[3] * F[4] * F[4] * F[1] * C(3, 3) * icste +
                 F[3] * F[4] * F[1] * F[4] * C(3, 3) * icste +
                 F[3] * F[4] * F[1] * F[1] * C(3, 1) +
                 F[3] * F[1] * F[4] * F[4] * C(1, 0) * cste +
                 F[3] * F[1] * F[4] * F[1] * C(1, 3) +
                 F[3] * F[1] * F[1] * F[4] * C(1, 3) +
                 F[3] * F[1] * F[1] * F[1] * C(1, 1) * cste;
      Ct(3, 2) = F[0] * F[4] * F[2] * F[2] * C(0, 2) * cste +
                 F[0] * F[1] * F[2] * F[2] * C(3, 2) +
                 F[3] * F[4] * F[2] * F[2] * C(3, 2) +
                 F[3] * F[1] * F[2] * F[2] * C(1, 2) * cste;
      Ct(1, 0) = F[4] * F[4] * F[0] * F[0] * C(0, 0) +
                 F[4] * F[4] * F[0] * F[3] * C(0, 3) * icste +
                 F[4] * F[4] * F[3] * F[0] * C(0, 3) * icste +
                 F[4] * F[4] * F[3] * F[3] * C(0, 1) +
                 F[4] * F[1] * F[0] * F[0] * C(3, 0) * icste +
                 F[4] * F[1] * F[0] * F[3] * C(3, 3) / 2 +
                 F[4] * F[1] * F[3] * F[0] * C(3, 3) / 2 +
                 F[4] * F[1] * F[3] * F[3] * C(3, 1) * icste +
                 F[1] * F[4] * F[0] * F[0] * C(3, 0) * icste +
                 F[1] * F[4] * F[0] * F[3] * C(3, 3) / 2 +
                 F[1] * F[4] * F[3] * F[0] * C(3, 3) / 2 +
                 F[1] * F[4] * F[3] * F[3] * C(3, 1) * icste +
                 F[1] * F[1] * F[0] * F[0] * C(1, 0) +
                 F[1] * F[1] * F[0] * F[3] * C(1, 3) * icste +
                 F[1] * F[1] * F[3] * F[0] * C(1, 3) * icste +
                 F[1] * F[1] * F[3] * F[3] * C(1, 1);
      Ct(1, 3) = F[4] * F[4] * F[0] * F[4] * C(0, 0) * cste +
                 F[4] * F[4] * F[0] * F[1] * C(0, 3) +
                 F[4] * F[4] * F[3] * F[4] * C(0, 3) +
                 F[4] * F[4] * F[3] * F[1] * C(0, 1) * cste +
                 F[4] * F[1] * F[0] * F[4] * C(3, 0) +
                 F[4] * F[1] * F[0] * F[1] * C(3, 3) * icste +
                 F[4] * F[1] * F[3] * F[4] * C(3, 3) * icste +
                 F[4] * F[1] * F[3] * F[1] * C(3, 1) +
                 F[1] * F[4] * F[0] * F[4] * C(3, 0) +
                 F[1] * F[4] * F[0] * F[1] * C(3, 3) * icste +
                 F[1] * F[4] * F[3] * F[4] * C(3, 3) * icste +
                 F[1] * F[4] * F[3] * F[1] * C(3, 1) +
                 F[1] * F[1] * F[0] * F[4] * C(1, 0) * cste +
                 F[1] * F[1] * F[0] * F[1] * C(1, 3) +
                 F[1] * F[1] * F[3] * F[4] * C(1, 3) +
                 F[1] * F[1] * F[3] * F[1] * C(1, 1) * cste;
      Ct(1, 1) = F[4] * F[4] * F[4] * F[4] * C(0, 0) +
                 F[4] * F[4] * F[4] * F[1] * C(0, 3) * icste +
                 F[4] * F[4] * F[1] * F[4] * C(0, 3) * icste +
                 F[4] * F[4] * F[1] * F[1] * C(0, 1) +
                 F[4] * F[1] * F[4] * F[4] * C(3, 0) * icste +
                 F[4] * F[1] * F[4] * F[1] * C(3, 3) / 2 +
                 F[4] * F[1] * F[1] * F[4] * C(3, 3) / 2 +
                 F[4] * F[1] * F[1] * F[1] * C(3, 1) * icste +
                 F[1] * F[4] * F[4] * F[4] * C(3, 0) * icste +
                 F[1] * F[4] * F[4] * F[1] * C(3, 3) / 2 +
                 F[1] * F[4] * F[1] * F[4] * C(3, 3) / 2 +
                 F[1] * F[4] * F[1] * F[1] * C(3, 1) * icste +
                 F[1] * F[1] * F[4] * F[4] * C(1, 0) +
                 F[1] * F[1] * F[4] * F[1] * C(1, 3) * icste +
                 F[1] * F[1] * F[1] * F[4] * C(1, 3) * icste +
                 F[1] * F[1] * F[1] * F[1] * C(1, 1);
      Ct(1, 2) = F[4] * F[4] * F[2] * F[2] * C(0, 2) +
                 F[4] * F[1] * F[2] * F[2] * C(3, 2) * icste +
                 F[1] * F[4] * F[2] * F[2] * C(3, 2) * icste +
                 F[1] * F[1] * F[2] * F[2] * C(1, 2);
      Ct(2, 0) = F[2] * F[2] * F[0] * F[0] * C(2, 0) +
                 F[2] * F[2] * F[0] * F[3] * C(2, 3) * icste +
                 F[2] * F[2] * F[3] * F[0] * C(2, 3) * icste +
                 F[2] * F[2] * F[3] * F[3] * C(2, 1);
      Ct(2, 3) = F[2] * F[2] * F[0] * F[4] * C(2, 0) * cste +
                 F[2] * F[2] * F[0] * F[1] * C(2, 3) +
                 F[2] * F[2] * F[3] * F[4] * C(2, 3) +
                 F[2] * F[2] * F[3] * F[1] * C(2, 1) * cste;
      Ct(2, 1) = F[2] * F[2] * F[4] * F[4] * C(2, 0) +
                 F[2] * F[2] * F[4] * F[1] * C(2, 3) * icste +
                 F[2] * F[2] * F[1] * F[4] * C(2, 3) * icste +
                 F[2] * F[2] * F[1] * F[1] * C(2, 1);
      Ct(2, 2) = F[2] * F[2] * F[2] * F[2] * C(2, 2);
    } else {
      using NumType = numeric_type<ST2toST2Type>;
      using size_type = unsigned short;
      auto row_index = [](size_type i, size_type j) -> size_type {
        // i,j are valid for the space dimension considered
        if ((i == j) && (i < 3)) {
          return i;
        } else if ((i == 0) && (j == 1)) {
          return 3;
        } else if ((i == 1) && (j == 0)) {
          return 3;
        } else if ((i == 0) && (j == 2)) {
          return 4;
        } else if ((i == 2) && (j == 0)) {
          return 4;
        } else if ((i == 1) && (j == 2)) {
          return 5;
        }
        return 5;
      };
      auto row_index2 = [](size_type i, size_type j) -> size_type {
        // i,j are valid for the space dimension considered
        if ((i == j) && (i < 3)) {
          return i;
        } else if ((i == 0) && (j == 1)) {
          return 3;
        } else if ((i == 1) && (j == 0)) {
          return 4;
        } else if ((i == 0) && (j == 2)) {
          return 5;
        } else if ((i == 2) && (j == 0)) {
          return 6;
        } else if ((i == 1) && (j == 2)) {
          return 7;
        }
        return 8;
      };
      auto set = [&Ct](const size_type i, const size_type j, const NumType v) {
        constexpr auto cste = Cste<NumType>::sqrt2;
        if (((i > 2) && (j <= 2)) || ((j > 2) && (i <= 2))) {
          Ct(i, j) = v * cste;
        } else if ((i > 2) && (j > 2)) {
          Ct(i, j) = v * 2;
        } else {
          Ct(i, j) = v;
        }
      };
      auto get = [&C](const size_type i, const size_type j) {
        constexpr auto icste = Cste<NumType>::isqrt2;
        if (((i > 2) && (j <= 2)) || ((j > 2) && (i <= 2))) {
          return C(i, j) * icste;
        } else if ((i > 2) && (j > 2)) {
          return C(i, j) / 2;
        }
        return C(i, j);
      };
      for (size_type i = 0; i != 3; ++i) {
        for (size_type j = 0; j != 3; ++j) {
          if (((i == 1) && (j == 0)) || ((i == 2) && (j == 0)) ||
              ((i == 2) && (j == 1))) {
            continue;
          }
          const auto rij = row_index(i, j);
          for (size_type k = 0; k != 3; ++k) {
            for (size_type l = 0; l != 3; ++l) {
              const auto rkl = row_index(k, l);
              auto Cv = NumType(0);
              for (size_type m = 0; m != 3; ++m) {
                for (size_type n = 0; n != 3; ++n) {
                  for (size_type p = 0; p != 3; ++p) {
                    for (size_type q = 0; q != 3; ++q) {
                      const auto rim = row_index2(i, m);
                      const auto rjn = row_index2(j, n);
                      const auto rkp = row_index2(k, p);
                      const auto rlq = row_index2(l, q);
                      const auto rmn = row_index(m, n);
                      const auto rpq = row_index(p, q);
                      Cv += F[rim] * F[rjn] * F[rkp] * F[rlq] * get(rmn, rpq);
                    }
                  }
                }
              }
              set(rij, rkl, Cv);
            }
          }
        }
      }
    }
  }
}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_ST2TOST2_PUSH_FORWARDIXX */
