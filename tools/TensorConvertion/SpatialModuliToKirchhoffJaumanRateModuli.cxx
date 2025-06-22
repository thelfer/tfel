/*!
 * \file   TensorConvertion.cxx
 * \brief
 * \author THOMAS HELFER
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
// C_tJ_{ijlk}=C_s_{ijkl}+I_{ik}\tau_{jl}+I_{jl}\tau_{ik}
template <unsigned short N>
struct converter : public index<N> {
  using size_type = unsigned short;
  static void exe(void) {
    for (size_type i = 0; i != 3; ++i) {
      for (size_type j = 0; j != 3; ++j) {
        for (size_type k = 0; k != 3; ++k) {
          for (size_type l = 0; l != 3; ++l) {
            const auto rij = index<N>::row_index(i, j);
            const auto rkl = index<N>::row_index(k, l);
            if ((rij == -1) || (rkl == -1)) {
              continue;
            }
            std::cout << "C_tJ(" << rij << "," << rkl << ") = "
                      << "C_s(" << rij << "," << rkl << ") ";
            const auto rjl = index<N>::row_index2(j, l);
            const auto rik = index<N>::row_index2(i, k);
            if (i == k) {
              if (((rij > 2) && (rkl < 2)) || ((rij < 2) && (rkl > 2))) {
                // *cste sauf si..
                if (rjl > 2) {
                  std::cout << "+tau[" << rjl << "]";
                } else {
                  std::cout << "+tau[" << rjl << "]*cste";
                }
              } else if ((rij > 2) && (rkl > 2)) {
                // *2 sauf si...
                if (rjl > 2) {
                  std::cout << "+tau[" << rjl << "]*cste";
                } else {
                  std::cout << "+tau[" << rjl << "]*2";
                }
              } else {  // if((rij<2)&&(rkl<2)){
                // *1 sauf si...
                if (rjl > 2) {
                  std::cout << "+tau[" << rjl << "]*icste";
                } else {
                  std::cout << "+tau[" << rjl << "]";
                }
              }
            }
            if (j == l) {
              if (((rij > 2) && (rkl < 2)) || ((rij < 2) && (rkl > 2))) {
                // *cste sauf si..
                if (rik > 2) {
                  std::cout << "+tau[" << rik << "]";
                } else {
                  std::cout << "+tau[" << rik << "]*cste";
                }
              } else if ((rij > 2) && (rkl > 2)) {
                // *2 sauf si...
                if (rik > 2) {
                  std::cout << "+tau[" << rik << "]*cste";
                } else {
                  std::cout << "+tau[" << rik << "]*2";
                }
              } else {  // if((rij<2)&&(rkl<2)){
                // *1 sauf si...
                if (rjl > 2) {
                  std::cout << "+tau[" << rik << "]*icste";
                } else {
                  std::cout << "+tau[" << rik << "]";
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

int main(void) {
  std::cout << "// in 1D: " << std::endl;
  converter<1u>::exe();
  std::cout << "// in 2D: " << std::endl;
  converter<2u>::exe();
  std::cout << "// in 3D: " << std::endl;
  converter<3u>::exe();
  return EXIT_SUCCESS;
}
