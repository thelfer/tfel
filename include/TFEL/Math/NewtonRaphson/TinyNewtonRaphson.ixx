/*!
 * \file   include/TFEL/Math/Newton-Raphson/TinyNewtonRaphson.ixx
 * \brief  This file implements the TinyNewtonMatrix
 * \author Thomas Helfer
 * \date   09 Aug 2006
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_TINYNEWTONRAPHSON_IXX
#define LIB_TFEL_MATH_TINYNEWTONRAPHSON_IXX

#include <cmath>
#include <limits>

namespace tfel::math {

  template <unsigned short N, typename T, typename F>
  class TinyNewtonRaphson {
    T epsilon;
    unsigned short iter_max;
    unsigned short iter;
    bool convergence;

    TinyNewtonRaphson(const TinyNewtonRaphson&);
    TinyNewtonRaphson& operator=(const TinyNewtonRaphson&);

   protected:
    /*
     * Jacobian matrix
     */
    tmatrix<N, N, T> J;
    /*
     * Residual vector
     */
    tvector<N, T> f;
    /*
     *
     */
    tvector<N, T> x;

   public:
    TinyNewtonRaphson() : convergence(false) {}

    void setInitialGuess(const T* const init) {
      tfel::fsalgo::copy<N>::exe(init, x.begin());
    }

    void setInitialGuess(const tvector<N, T>& init) { x = init; }

    void setPrecision(const T e) { this->epsilon = e; }

    void setMaximumIteration(const unsigned short n) { this->iter_max = n; }

    void exe() {
      iter = 0;
      while ((iter < iter_max) && (convergence == false)) {
        static_cast<F&>(*this).computeFdF();
        if (norm(f) <= epsilon) {
          convergence = true;
        } else {
          TinyMatrixSolve<N, T>::exe(J, f);
          x -= f;
          ++(iter);
        }
        if (iter == iter_max) {
          throw(MaximumNumberOfIterationsReachedException());
        }
      }
    }

    tvector<N, T> getX() const { return this->x; }

    unsigned short getIter() const { return this->iter; }
  };

  /*
   * \brief Partial Specialisation in 1D
   * \param typename T, numerical type
   * \param typename F, class to which the newton raphson is applied.
   * \pre   F must have a computeFdF method.
   */
  template <typename T, typename F>
  class TinyNewtonRaphson<1u, T, F> {
    unsigned short iter_max;
    unsigned short iter;
    bool convergence;

    TinyNewtonRaphson(const TinyNewtonRaphson&);
    TinyNewtonRaphson& operator=(const TinyNewtonRaphson&);

   protected:
    T J;
    T f;
    T x;
    T epsilon;

   public:
    TinyNewtonRaphson() : convergence(false) {}

    void setInitialGuess(const T init) { this->x = init; }

    void setPrecision(const T e) { this->epsilon = e; }

    void setMaximumIteration(const unsigned short n) { this->iter_max = n; }

    void exe() {
      iter = 0;
      while ((iter < iter_max) && (convergence == false)) {
        static_cast<F&>(*this).computeFdF();
        if (std::abs(f) <= epsilon) {
          convergence = true;
        } else {
          if (std::abs(J) < 100 * std::numeric_limits<T>::min()) {
            throw(SingularJacobianException());
          }
          f /= J;
          x -= f;
          ++(iter);
        }
        if (iter == iter_max) {
          throw(MaximumNumberOfIterationsReachedException());
        }
      }
    }

    T getX() const { return this->x; }

    unsigned short getIter() const { return this->iter; }
  };

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_TINYNEWTONRAPHSON_IXX */
