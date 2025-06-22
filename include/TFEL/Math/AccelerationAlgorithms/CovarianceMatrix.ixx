/*!
 * \file   CovarianceMatrix.ixx
 * \brief
 * \author Étienne Castelier
 * \date   10 oct. 2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_COVARIANCEMATRIX_IXX
#define LIB_COVARIANCEMATRIX_IXX

#include <limits>
#include <stdexcept>
#include "TFEL/Config/TFELConfig.hxx"

namespace tfel {

  namespace math {

    namespace anderson {

      // Default constructor
      template <typename real>
      CovarianceMatrix<real>::CovarianceMatrix() = default;

      // Constructor
      // Nmax Matrix size
      template <typename real>
      CovarianceMatrix<real>::CovarianceMatrix(const size_type n) : N(0) {
        this->resize(n);
      }

      // Resize the matrix
      // Nmax Matrix size
      template <typename real>
      void CovarianceMatrix<real>::resize(const size_type n) {
        this->Nmax = n;
        if (this->Nmax) {
          this->C.resize((this->Nmax * (this->Nmax + 1)) / 2);
          this->a.resize((this->Nmax * (this->Nmax - 1)) / 2);
          this->ne.resize(this->Nmax);
        }
      }

      // Return the matrix size
      template <typename real>
      typename CovarianceMatrix<real>::size_type
      CovarianceMatrix<real>::getSize() const {
        return this->N;
      }

      // Shift the covariances
      template <typename real>
      void CovarianceMatrix<real>::shift() {
        pointer Cij = C.begin();
        for (size_type i = 1; i < N; ++i) {
          for (size_type j = 0; j < i; ++j, ++Cij) {
            *Cij = Cij[i + 1];
          }
        }
      }

      // N Line index
      // return the address of the ith line
      template <typename real>
      typename CovarianceMatrix<real>::pointer CovarianceMatrix<real>::getLine(
          const size_type i) {
        if (i >= this->Nmax) {
          throw(
              std::runtime_error("CovarianceMatrix<real>::getLine: "
                                 "line index cannot exceed the matrix size"));
        }
        if (i > this->N) {
          throw(
              std::runtime_error("CovarianceMatrix<real>::getLine: "
                                 "internal error (missing lines)"));
        }
        this->N = i + 1;
        return this->C.begin() + (i * (i + 1)) / 2;
      }

      // Gram-Schmidt Factorisation
      // Descendent order
      template <typename real>
      void CovarianceMatrix<real>::GSFactorD() {
        pointer ai = this->a.begin();
        const_pointer Di = this->C.begin() + ((N + 2) * (N - 1)) / 2;
        const_pointer Dii = Di;
        for (size_type i = 0; i < this->N; ++i) {
          // Ratio ei.xj/ej.ej
          pointer aij = ai;
          const_pointer Dij = Di - i;
          for (size_type j = 0; j < i; ++j, Dij -= N - j) {
            if (ne[j] > 0) {
              aij[j] = *Dij;
              const_pointer Dik = Di - i;
              const_pointer ajk = this->a.begin() + (j * (j - 1)) / 2;
              for (size_type k = 0; k < j; ++k, Dik -= N - k) {
                aij[j] -= ajk[k] * (*Dik);
              }
              aij[j] /= ne[j];
            } else {
              aij[j] = 0;
            }
          }
          // Alpha(ji) Coefficient
          aij = ai;
          for (size_type j = 0; j < i; ++j) {
            const_pointer akj = this->a.begin() + ((j + 3) * j) / 2;
            for (size_type k = j + 1; k < i; akj += k, ++k) {
              aij[j] -= ai[k] * (*akj);
            }
          }

          // Norm
          ne[i] = *Dii;
          aij = ai;
          Dij = Di - i;
          for (size_type j = 0; j < i; ++j, Dij -= N - j) {
            ne[i] -= aij[j] * (*Dij);
          }

          // Degeneration
          TFEL_CONSTEXPR const real eps =
              100 * std::numeric_limits<real>::epsilon();
          if (ne[i] < C[0] * eps * eps) {
            ne[i] = 0;
          }
          if (i + 1 != this->N) {
            ai += i;
            Dii -= N - i;
          }
        }
      }

      // Anderson weights by Gram-Schmidt process
      // Descendent order
      // w: Anderson weights
      template <typename real>
      void CovarianceMatrix<real>::weightsGSchmidtD(std::vector<real>& w) {
        this->GSFactorD();
        auto rw = w.rbegin() + (this->Nmax - this->N);
        for (size_type i = 0; i != this->N; ++i) {
          if (ne[i] > 0) {
            rw[i] = 1;
            const_pointer aij = this->a.begin() + (i * (i - 1)) / 2;
            for (size_type j = 0; j < i; ++j) {
              rw[i] -= aij[j];
            }
            rw[i] /= ne[i];
          } else {
            rw[i] = 0;
          }
        }

        real d = 0;
        for (size_type i = 0; i != N; ++i) {
          if (i + 1 != N) {
            const_pointer aji = this->a.begin() + ((i + 3) * i) / 2;
            for (size_type j = i + 1; j != this->N; ++j) {
              rw[i] -= *aji * rw[j];
              if (j + 1 != this->N) {
                aji += j;
              }
            }
          }
          d += rw[i];
        }
        for (size_type i = 0; i != this->N; ++i) {
          rw[i] /= d;
        }
      }

      //! Destructor
      template <typename real>
      CovarianceMatrix<real>::~CovarianceMatrix() = default;

    }  // end of  namespace anderson

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_COVARIANCEMATRIX_IXX */
