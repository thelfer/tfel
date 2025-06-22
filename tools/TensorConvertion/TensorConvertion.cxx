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
};

// Compute a st2tost2 from a t2tost2 satisfying
// CIJKL=1/J(K_{IJKN}*F_{LN}

// Being a st2tost2, CIJKL has minor symmetries:
// CIJKL = CJIKL
// CIJKL = CIJLK
// The first symmetry is satisfied as K is a t2tost2.
// The second symmetry has to be assumed.

// The terms of C must then be normalised to take into account TFEL
// conventions on symmetric tensors. This has already been done for
// the output (IJ indices) as K is a t2tost2.
template <unsigned short N>
struct converter1 : public index<N> {
  using size_type = unsigned short;
  static void exe(void) {
    for (size_type i = 0; i != 3; ++i) {
      for (size_type j = 0; j != 3; ++j) {
        const auto ri = index<N>::row_index(i, j);
        if (ri != -1) {
          for (size_type k = 0; k != 3; ++k) {
            for (size_type l = 0; l != 3; ++l) {
              const auto rj = index<N>::row_index(k, l);
              bool first = true;
              if (rj != -1) {
                if (k == l) {  // equivalent to rj<3
                  std::cout << "C(" << ri << "," << rj << ") = ";
                  writeKIJKNFLN(first, ri, k, l);
                  std::cout << ";\n";
                } else {
                  std::cout << "C(" << ri << "," << rj << ") = (";
                  writeKIJKNFLN(first, ri, k, l);
                  // swap k and l to impose the second symmetry
                  writeKIJKNFLN(first, ri, l, k);
                  // cste = 1/2*sqrt(2)
                  std::cout << ")*cste;\n";
                }
              }
            }
          }
        }
      }
    }
  }  // end of exe
 protected:
  // write KIJKN*FLN
  static void writeKIJKNFLN(bool& first,
                            const int ri,
                            const size_type k,
                            const size_type l) {
    for (size_type n = 0; n != 3; ++n) {
      const auto rkn = index_base<N>::exe(k, n);
      const auto rln = index_base<N>::exe(l, n);
      if (!((rkn == -1) || (rln == -1))) {
        if (!first) {
          std::cout << "+";
        }
        std::cout << "K(" << ri << "," << rkn << ")*F(" << rln << ")";
        first = false;
      }
    }
  }
};

//! convert a t2tot2 to t2tost2
template <unsigned short N>
struct converter2 : public index<N> {
  using size_type = unsigned short;
  static void exe(void) {
    for (size_type i = 0; i != 3; ++i) {
      for (size_type j = 0; j != 3; ++j) {
        const auto rij = index<N>::row_index(i, j);
        if (rij == -1) {
          continue;
        }
        const auto rij1 = index_base<N>::exe(i, j);
        const auto rij2 = index_base<N>::exe(j, i);
        for (size_type k = 0; k != 3; ++k) {
          for (size_type l = 0; l != 3; ++l) {
            const auto rkl = index_base<N>::exe(k, l);
            if (rkl == -1) {
              continue;
            }
            if (i == j) {  // equivalent to rij<3
              std::cout << "r(" << rij << "," << rkl << ") = "
                        << "t(" << rij << "," << rkl << ");\n";
            } else {
              // cste = sqrt(2)/2
              std::cout << "r(" << rij << "," << rkl << ") = "
                        << "(t(" << rij1 << "," << rkl << ")+"
                        << "t(" << rij2 << "," << rkl << "))*cste;\n";
            }
          }
        }
      }
    }
  }  // end of exe
};

int main(void) {
  converter2<3>::exe();
  return EXIT_SUCCESS;
}
