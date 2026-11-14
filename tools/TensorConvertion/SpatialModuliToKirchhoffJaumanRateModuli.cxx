/*!
 * \file   TensorConvertion.cxx
 * \brief
 * \author Thomas Helfer
 * \date 16/12/2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cmath>
#include <cstdlib>
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
// C_tJ_{ijkl}=C_s_{ijkl}+(I_{ik}\tau_{jl}+I_{il}\tau_{jk}+I_{jk}\tau_{il}+I_{jl}\tau_{ik})/2
// i.e. the relation C_tJ_{ijkl}=C_s_{ijkl}+I_{ik}\tau_{jl}+I_{jl}\tau_{ik}
// symmetrised on k and l, as C_tJ is a st2tost2.

// The terms must then be normalised to take into account TFEL
// conventions on symmetric tensors, whose off-diagonal components are
// multiplied by sqrt(2): the coefficient of tau_{mn} in the term (ij,kl)
// is multiplied by w_{ij}*w_{kl}/w_{mn}, where w is 1 for a diagonal
// component and sqrt(2) (cste) otherwise.
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
            // coefficients of the components of tau
            double c[6] = {0, 0, 0, 0, 0, 0};
            auto add = [&c, i, j, k, l](const size_type m, const size_type n) {
              c[index<N>::row_index2(m, n)] +=
                  weight(i, j) * weight(k, l) / (2 * weight(m, n));
            };
            if (i == k) {
              add(j, l);
            }
            if (i == l) {
              add(j, k);
            }
            if (j == k) {
              add(i, l);
            }
            if (j == l) {
              add(i, k);
            }
            std::cout << "C_tJ(" << rij << "," << rkl << ") = "
                      << "C_s(" << rij << "," << rkl << ") ";
            for (int m = 0; m != 6; ++m) {
              if (std::abs(c[m]) < 1e-12) {
                continue;
              }
              std::cout << "+tau[" << m << "]";
              if (std::abs(c[m] - 2) < 1e-12) {
                std::cout << "*2";
              } else if (std::abs(c[m] - std::sqrt(2.)) < 1e-12) {
                std::cout << "*cste";
              } else if (std::abs(c[m] - 1 / std::sqrt(2.)) < 1e-12) {
                std::cout << "*icste";
              } else if (std::abs(c[m] - 1) > 1e-12) {
                std::cerr << "unexpected coefficient " << c[m] << '\n';
                std::exit(EXIT_FAILURE);
              }
            }
            std::cout << ";\n";
          }
        }
      }
    }
  }  // end of exe
 private:
  //! \brief weight of the component (i,j) of a symmetric tensor
  static double weight(const size_type i, const size_type j) {
    return i == j ? 1 : std::sqrt(2.);
  }
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
