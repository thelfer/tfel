/*!
 * \file   GteSymmetricEigenSolver.hxx
 * \brief
 * \author David Eberly, Geometric Tools, Redmond WA 98052
 * \date   20/09/2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 *
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 *
 * This file was adapted from Geometric Tools, which is
 * distributed under the Boost Software License, Version 1.0.
 * http://www.boost.org/LICENSE_1_0.txt
 * http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
 *
 * The document
 * http://www.geometrictools.com/Documentation/RobustEigenSymmetric3x3.pdf
 * describes algorithms for solving the eigensystem associated with a 3x3
 * symmetric real-valued matrix.  The iterative algorithm is implemented
 * by class SymmmetricEigensolver3x3.  The noniterative algorithm is
 * implemented by class NISymmetricEigensolver3x3.  The code does not use
 * GTEngine objects.
 */

#ifndef LIB_TFEL_MATH_GTESYMMETRICEIGENSOLVER_HXX
#define LIB_TFEL_MATH_GTESYMMETRICEIGENSOLVER_HXX

#include <cmath>
#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/tmatrix.hxx"

namespace tfel::math::internals {

  template <typename real>
  struct GteSymmetricEigensolver3x3 {
    // The input matrix must be symmetric, so only the unique elements must
    // be specified: a00, a01, a02, a11, a12, and a22.
    //
    // If 'aggressive' is 'true', the iterations occur until a superdiagonal
    // entry is exactly zero.  If 'aggressive' is 'false', the iterations
    // occur until a superdiagonal entry is effectively zero compared to the
    // sum of magnitudes of its diagonal neighbors.  Generally, the
    // nonaggressive convergence is acceptable.
    //
    // The order of the eigenvalues is specified by sortType: -1 (decreasing),
    // 0 (no sorting), or +1 (increasing).  When sorted, the eigenvectors are
    // ordered accordingly, and {m[0], m[1], m[2]} is guaranteed to
    // be a right-handed orthonormal set.  The return value is the number of
    // iterations used by the algorithm.

    static int exe(tvector<3u, real>& vp,
                   tmatrix<3u, 3u, real>& m,
                   real a00,
                   real a01,
                   real a02,
                   real a11,
                   real a12,
                   real a22,
                   bool aggressive);

   private:
    // update Q = Q*G in-place using G = {{c,0,-s},{s,0,c},{0,0,1}}.
    static void update0(real Q[3][3], real c, real s);

    // update Q = Q*G in-place using G = {{0,1,0},{c,0,s},{-s,0,c}}.
    static void update1(real Q[3][3], real c, real s);

    // update Q = Q*H in-place using H = {{c,s,0},{s,-c,0},{0,0,1}}.
    static void update2(real Q[3][3], real c, real s);

    // update Q = Q*H in-place using H = {{1,0,0},{0,c,s},{0,s,-c}}.
    static void update3(real Q[3][3], real c, real s);

    // Normalize (u,v) robustly, avoiding floating-point overflow in the sqrt
    // call.  The normalized pair is (cs,sn) with cs <= 0.  If (u,v) = (0,0),
    // the function returns (cs,sn) = (-1,0).  When used to generate a
    // Householder reflection, it does not matter whether (cs,sn) or (-cs,-sn)
    // is used.  When generating a Givens reflection, cs = cos(2*theta) and
    // sn = sin(2*theta).  Having a negative cosine for the double-angle
    // term ensures that the single-angle terms c = cos(theta) and
    // s = sin(theta) satisfy |c| <= |s|.
    static void getCosSin(real u, real v, real& cs, real& sn);

    // The convergence test.  When 'aggressive' is 'true', the superdiagonal
    // test is "bSuper == 0".  When 'aggressive' is 'false', the superdiagonal
    // test is "|bDiag0| + |bDiag1| + |bSuper| == |bDiag0| + |bDiag1|, which
    // means bSuper is effectively zero compared to the sizes of the diagonal
    // entries.
    static bool converged(bool aggressive,
                          real bDiag0,
                          real bDiag1,
                          real bSuper);

    // Support for sorting the eigenvalues and eigenvectors.  The output
    // (i0,i1,i2) is a permutation of (0,1,2) so that d[i0] <= d[i1] <= d[i2].
    // The 'bool' return indicates whether the permutation is odd.  If it is
    // not, the handedness of the Q matrix must be adjusted.
    static bool sort(const tvector<3u, real>& d, int& i0, int& i1, int& i2);
  };

}  // end of namespace tfel::math::internals

#include "TFEL/Math/Stensor/Internals/GteSymmetricEigenSolver.ixx"

#endif /* LIB_TFEL_MATH_GTESYMMETRICEIGENSOLVER_HXX */
