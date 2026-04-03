/*!
 * \file   TensorConvertion.cxx
 * \brief
 * \author Thomas Helfer
 * \date   16 déc. 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <iostream>

template <unsigned short N>
struct index_base;

template <>
struct index_base<1u> {
  static int exe(const unsigned short i, const unsigned short j) {
    if ((i == j) && (i < 3)) {
      return i;
    }
    return -1;
  }
};

template <>
struct index_base<2u> {
  static int exe(const unsigned short i, const unsigned short j) {
    if ((i == j) && (i < 3)) {
      return i;
    } else if ((i == 0) && (j == 1)) {
      return 3;
    } else if ((i == 1) && (j == 0)) {
      return 4;
    }
    return -1;
  }
};

template <>
struct index_base<3u> {
  static int exe(const unsigned short i, const unsigned short j) {
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
    } else if ((i == 2) && (j == 1)) {
      return 8;
    }
    return -1;
  }
};

template <unsigned short N>
struct index : public index_base<N> {
 protected:
  using size_type = unsigned short;
  static int row_index(size_type i, size_type j) {
    if (index_base<N>::exe(i, j) != -1) {
      // i,j are valid for the space dimension considered
      if ((i == j) && (i < 3)) {
        return i;
      } else if ((i == 0) && (j == 1)) {
        return 3;
      } else if ((i == 1) && (j == 0)) {
        return -1;
      } else if ((i == 0) && (j == 2)) {
        return 4;
      } else if ((i == 2) && (j == 0)) {
        return -1;
      } else if ((i == 1) && (j == 2)) {
        return 5;
      } else if ((i == 2) && (j == 1)) {
        return -1;
      }
      return -1;
    }
    return -1;
  }
  static int row_index2(size_type i, size_type j) {
    if (index_base<N>::exe(i, j) != -1) {
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
      } else if ((i == 2) && (j == 1)) {
        return 5;
      }
      return -1;
    }
    return -1;
  }
};

// Compute a st2tost2 from a st2tost2 satisfying
// C^{\tau}_{ijkl}=F_{im}F_{jn}F_{kp}F_{lq}C_{mnpq}
template <unsigned short N>
struct converter : public index<N> {
  using size_type = unsigned short;
  static void exe() {
    for (size_type i = 0; i != 3; ++i) {
      for (size_type j = 0; j != 3; ++j) {
        for (size_type k = 0; k != 3; ++k) {
          for (size_type l = 0; l != 3; ++l) {
            const auto rij = index<N>::row_index(i, j);
            const auto rkl = index<N>::row_index(k, l);
            if ((rij == -1) || (rkl == -1)) {
              continue;
            }
            std::cout << "Ct(" << rij << "," << rkl << ") = ";
            int nb = 0;
            for (size_type m = 0; m != 3; ++m) {
              const auto rim = index<N>::exe(i, m);
              if (rim == -1) {
                continue;
              }
              for (size_type n = 0; n != 3; ++n) {
                const auto rjn = index<N>::exe(j, n);
                if (rjn == -1) {
                  continue;
                }
                for (size_type p = 0; p != 3; ++p) {
                  const auto rkp = index<N>::exe(k, p);
                  if (rkp == -1) {
                    continue;
                  }
                  for (size_type q = 0; q != 3; ++q) {
                    const auto rlq = index<N>::exe(l, q);
                    if (rlq == -1) {
                      continue;
                    }
                    // row_index2 takes into account minor symmetries of
                    // C_{mnpq}
                    const auto rmn = index<N>::row_index2(m, n);
                    const auto rpq = index<N>::row_index2(p, q);
                    if (nb != 0) {
                      std::cout << "+";
                    };
                    if ((nb % 2 == 0) && (nb != 0)) {
                      std::cout << "\n";
                    };
                    std::cout << "F[" << rim << "]*F[" << rjn << "]*F[" << rkp
                              << "]*F[" << rlq << "]*C(" << rmn << "," << rpq
                              << ")";
                    if (((rij > 2) && (rkl < 3)) || ((rkl > 2) && (rij < 3))) {
                      // on multiplie par cste, sauf si..
                      if ((rmn > 2) && (rpq > 2)) {
                        // le coefficient initial devait être divisé
                        // par 2, au final on divise par racine de 2
                        std::cout << "*icste";
                      } else if (!(((rmn > 2) && (rpq < 3)) ||
                                   ((rpq > 2) && (rmn < 3)))) {
                        std::cout << "*cste";
                      }
                    } else if ((rij > 2) && (rkl > 2)) {
                      // on multiplie par 2, sauf si..
                      if ((((rmn > 2) && (rpq < 3)) ||
                           ((rpq > 2) && (rmn < 3)))) {
                        // on devait diviser par racine de 2, donc au
                        // final on multiplie par racine de 2
                        std::cout << "*cste";
                      } else if (!((rmn > 2) && (rpq > 2))) {
                        std::cout << "*2";
                      }
                    } else {
                      if ((((rmn > 2) && (rpq < 3)) ||
                           ((rpq > 2) && (rmn < 3)))) {
                        std::cout << "*icste";
                      } else if ((rmn > 2) && (rpq > 2)) {
                        std::cout << "/2";
                      }
                    }
                    ++nb;
                  }
                }
              }
            }
            std::cout << ";\n";
          }
        }
      }
    }
  }  // end of exe
};

int main() {
  std::cout << "// in 1D: " << std::endl;
  converter<1u>::exe();
  std::cout << "// in 2D: " << std::endl;
  converter<2u>::exe();
  std::cout << "// in 3D: " << std::endl;
  converter<3u>::exe();
  return EXIT_SUCCESS;
}
