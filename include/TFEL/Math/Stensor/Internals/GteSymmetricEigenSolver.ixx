/*!
 * \file   GteSymmetricEigenSolver.ixx
 * \brief    
 * \author David Eberly, Geometric Tools, Redmond WA 98052
 * \date   20/09/2016
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 *
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 *
 * This file was adapted from Geometric Tools, which is
 * distributed under the Boost Software License, Version 1.0.
 * http://www.boost.org/LICENSE_1_0.txt
 * http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
 */

#ifndef LIB_TFEL_MATH_GTESYMMETRICEIGENSOLVER_IXX
#define LIB_TFEL_MATH_GTESYMMETRICEIGENSOLVER_IXX

namespace tfel{

  namespace math{

    namespace internals{

      template <typename real>
      int GteSymmetricEigensolver3x3<real>::exe(tvector<3u,real>& vp,
						tmatrix<3u,3u,real>& m,
						real a00, real a01, real a02,
						real a11, real a12, real a22,
						bool aggressive)
      {
	// compute the Householder reflection H and B = H*A*H, where b02 = 0.
	constexpr      const real zero  = real(0);
	constexpr      const real one   = real(1);
	TFEL_CONSTEXPR const real one_half  = one/2;
	bool isRotation = false;
	real c, s;
	getCosSin(a12, -a02, c, s);
	real Q[3][3] = { { c, s, zero }, { s, -c, zero }, { zero, zero, one } };
	real term0 = c * a00 + s * a01;
	real term1 = c * a01 + s * a11;
	real b00 = c * term0 + s * term1;
	real b01 = s * term0 - c * term1;
	term0 = s * a00 - c * a01;
	term1 = s * a01 - c * a11;
	real b11 = s * term0 - c * term1;
	real b12 = s * a02 - c * a12;
	real b22 = a22;

	// Givens reflections, B' = G^T*B*G, preserve tridiagonal matrices.
	int const maxIteration = 2 * (1 + std::numeric_limits<real>::digits -
				      std::numeric_limits<real>::min_exponent);
	int iteration;
	real c2, s2;

	if (std::abs(b12) <= std::abs(b01)) {
	  real saveB00, saveB01, saveB11;
	  for (iteration = 0; iteration < maxIteration; ++iteration) {
	    // compute the Givens reflection.
	    getCosSin(one_half * (b00 - b11), b01, c2, s2);
	    s = std::sqrt(one_half * (one - c2));  // >= 1/std::sqrt(2)
	    c = one_half * s2 / s;

	    // update Q by the Givens reflection.
	    update0(Q, c, s);
	    isRotation = !isRotation;

	    // update B <- Q^T*B*Q, ensuring that b02 is zero and |b12| has
	    // strictly decreased.
	    saveB00 = b00;
	    saveB01 = b01;
	    saveB11 = b11;
	    term0 = c * saveB00 + s * saveB01;
	    term1 = c * saveB01 + s * saveB11;
	    b00 = c * term0 + s * term1;
	    b11 = b22;
	    term0 = c * saveB01 - s * saveB00;
	    term1 = c * saveB11 - s * saveB01;
	    b22 = c * term1 - s * term0;
	    b01 = s * b12;
	    b12 = c * b12;

	    if (converged(aggressive, b00, b11, b01)) {
	      // compute the Householder reflection.
	      getCosSin(one_half * (b00 - b11), b01, c2, s2);
	      s = std::sqrt(one_half * (one - c2));
	      c = one_half * s2 / s;  // >= 1/std::sqrt(2)

	      // update Q by the Householder reflection.
	      update2(Q, c, s);
	      isRotation = !isRotation;

	      // update D = Q^T*B*Q.
	      saveB00 = b00;
	      saveB01 = b01;
	      saveB11 = b11;
	      term0 = c * saveB00 + s * saveB01;
	      term1 = c * saveB01 + s * saveB11;
	      b00 = c * term0 + s * term1;
	      term0 = s * saveB00 - c * saveB01;
	      term1 = s * saveB01 - c * saveB11;
	      b11 = s * term0 - c * term1;
	      break;
	    }
	  }
	} else {
	  real saveB11, saveB12, saveB22;
	  for (iteration = 0; iteration < maxIteration; ++iteration){
	    // compute the Givens reflection.
	    getCosSin(one_half * (b22 - b11), b12, c2, s2);
	    s = std::sqrt(one_half * (one - c2));  // >= 1/std::sqrt(2)
	    c = one_half * s2 / s;

	    // update Q by the Givens reflection.
	    update1(Q, c, s);
	    isRotation = !isRotation;

	    // update B <- Q^T*B*Q, ensuring that b02 is zero and |b12| has
	    // strictly decreased.  MODIFY...
	    saveB11 = b11;
	    saveB12 = b12;
	    saveB22 = b22;
	    term0 = c * saveB22 + s * saveB12;
	    term1 = c * saveB12 + s * saveB11;
	    b22 = c * term0 + s * term1;
	    b11 = b00;
	    term0 = c * saveB12 - s * saveB22;
	    term1 = c * saveB11 - s * saveB12;
	    b00 = c * term1 - s * term0;
	    b12 = s * b01;
	    b01 = c * b01;

	    if (converged(aggressive, b11, b22, b12)){
	      // compute the Householder reflection.
	      getCosSin(one_half * (b11 - b22), b12, c2, s2);
	      s = std::sqrt(one_half * (one - c2));
	      c = one_half * s2 / s;  // >= 1/std::sqrt(2)

	      // update Q by the Householder reflection.
	      update3(Q, c, s);
	      isRotation = !isRotation;

	      // update D = Q^T*B*Q.
	      saveB11 = b11;
	      saveB12 = b12;
	      saveB22 = b22;
	      term0 = c * saveB11 + s * saveB12;
	      term1 = c * saveB12 + s * saveB22;
	      b11 = c * term0 + s * term1;
	      term0 = s * saveB11 - c * saveB12;
	      term1 = s * saveB12 - c * saveB22;
	      b22 = s * term0 - c * term1;
	      break;
	    }
	  }
	}

	tvector<3u,real> diagonal = { b00, b11, b22 };
	int i0, i1, i2;
	// diagonal[i0] >= diagonal[i1] >= diagonal[i2]
	bool isOdd = sort(diagonal, i0, i1, i2);
	std::swap(i0, i2);  // (i0,i1,i2)->(i2,i1,i0) is odd
	if (isOdd){
	  isRotation = !isRotation;
	}

	vp[0] = diagonal[i0];
	vp[1] = diagonal[i1];
	vp[2] = diagonal[i2];
	m(0,0) = Q[0][i0];
	m(1,0) = Q[1][i0];
	m(2,0) = Q[2][i0];
	m(0,1) = Q[0][i1];
	m(1,1) = Q[1][i1];
	m(2,1) = Q[2][i1];
	m(0,2) = Q[0][i2];
	m(1,2) = Q[1][i2];
	m(2,2) = Q[2][i2];

	// Ensure the columns of Q form a right-handed set.
	if (!isRotation){
	  for (int j = 0; j < 3; ++j){
	    m(j,2) = -m(j,2);
	  }
	}
	return iteration;
      }

      template <typename real>
      void GteSymmetricEigensolver3x3<real>::update0(real Q[3][3], real c, real s)
      {
	for (int r = 0; r < 3; ++r){
	  const auto tmp0 = c * Q[r][0] + s * Q[r][1];
	  const auto tmp1 = Q[r][2];
	  const auto tmp2 = c * Q[r][1] - s * Q[r][0];
	  Q[r][0] = tmp0;
	  Q[r][1] = tmp1;
	  Q[r][2] = tmp2;
	}
      }

      template <typename real>
      void GteSymmetricEigensolver3x3<real>::update1(real Q[3][3], real c, real s)
      {
	for (int r = 0; r < 3; ++r){
	  const auto tmp0 = c * Q[r][1] - s * Q[r][2];
	  const auto tmp1 = Q[r][0];
	  const auto tmp2 = c * Q[r][2] + s * Q[r][1];
	  Q[r][0] = tmp0;
	  Q[r][1] = tmp1;
	  Q[r][2] = tmp2;
	}
      }

      template <typename real>
      void GteSymmetricEigensolver3x3<real>::update2(real Q[3][3], real c, real s)
      {
	for (int r = 0; r < 3; ++r){
	  const auto tmp0 = c * Q[r][0] + s * Q[r][1];
	  const auto tmp1 = s * Q[r][0] - c * Q[r][1];
	  Q[r][0] = tmp0;
	  Q[r][1] = tmp1;
	}
      }

      template <typename real>
      void GteSymmetricEigensolver3x3<real>::update3(real Q[3][3], real c, real s)
      {
	for (int r = 0; r < 3; ++r){
	  const auto tmp0 = c * Q[r][1] + s * Q[r][2];
	  const auto tmp1 = s * Q[r][1] - c * Q[r][2];
	  Q[r][1] = tmp0;
	  Q[r][2] = tmp1;
	}
      }

      template <typename real>
      void GteSymmetricEigensolver3x3<real>::getCosSin(real u, real v, real& cs,
						       real& sn)
      {
	constexpr const real zero  = real(0);
	constexpr const real one   = real(1);
	const real maxAbsComp = std::max(std::abs(u), std::abs(v));
	if (maxAbsComp > zero){
	  u /= maxAbsComp;  // in [-1,1]
	  v /= maxAbsComp;  // in [-1,1]
	  const real l = std::sqrt(u * u + v * v);
	  cs = u / l;
	  sn = v / l;
	  if (cs > zero){
	    cs = -cs;
	    sn = -sn;
	  }
	} else {
	  cs = -one;
	  sn = zero;
	}
      }

      template <typename real>
      bool GteSymmetricEigensolver3x3<real>::converged(bool aggressive, real bDiag0,
						       real bDiag1, real bSuper)
      {
	TFEL_CONSTEXPR const auto rmin = std::numeric_limits<real>::min();
	TFEL_CONSTEXPR const auto re   = std::numeric_limits<real>::epsilon();
	if (aggressive){
	  return std::abs(bSuper) < rmin;
	} else {
	  const auto sum = std::abs(bDiag0) + std::abs(bDiag1);
	  return std::abs(bSuper) < rmin+sum*re;
	}
      }

      template <typename real>
      bool GteSymmetricEigensolver3x3<real>::sort(const tvector<3u,real>& d,
						  int& i0, int& i1, int& i2)
      {
	bool odd;
	if (d[0] < d[1]){
	  if (d[2] < d[0]){
	    i0 = 2; i1 = 0; i2 = 1; odd = true;
	  } else if (d[2] < d[1]){
	    i0 = 0; i1 = 2; i2 = 1; odd = false;
	  } else {
	    i0 = 0; i1 = 1; i2 = 2; odd = true;
	  }
	} else {
	  if (d[2] < d[1]) {
	    i0 = 2; i1 = 1; i2 = 0; odd = false;
	  } else if (d[2] < d[0]) {
	    i0 = 1; i1 = 2; i2 = 0; odd = true;
	  } else {
	    i0 = 1; i1 = 0; i2 = 2; odd = false;
	  }
	}
	return odd;
      }


      template <typename real>
      void GteNISymmetricEigensolver3x3<real>::exe(tvector<3u,real>& vp,
						   tmatrix<3u,3u,real>& m,
						   real a00, real a01, real a02,
						   real a11, real a12, real a22)
      {
	TFEL_CONSTEXPR const auto rmin = std::numeric_limits<real>::min();
	constexpr const auto zero  = real(0);
	constexpr const auto one   = real(1);
	// Precondition the matrix by factoring out the maximum absolute value
	// of the components.  This guards against floating-point overflow when
	// computing the eigenvalues.
	const auto max0 = std::max(std::abs(a00), std::abs(a01));
	const auto max1 = std::max(std::abs(a02), std::abs(a11));
	const auto max2 = std::max(std::abs(a12), std::abs(a22));
	const auto maxAbsElement = std::max(std::max(max0, max1), max2);
	if (std::abs(maxAbsElement)<10*rmin){
	  // A is the zero matrix.
	  vp = tvector<3u,real>{zero};
	  m  = tmatrix<3u,3u,real>::Id();
	  return;
	}
	const auto invMaxAbsElement = one / maxAbsElement;
	a00 *= invMaxAbsElement;
	a01 *= invMaxAbsElement;
	a02 *= invMaxAbsElement;
	a11 *= invMaxAbsElement;
	a12 *= invMaxAbsElement;
	a22 *= invMaxAbsElement;
	const auto norm = a01 * a01 + a02 * a02 + a12 * a12;
	if (norm > zero){
	  // compute the eigenvalues of A.  The acos(z) function requires |z| <= 1,
	  // but will fail silently and return NaN if the input is larger than 1 in
	  // magnitude.  To avoid this condition due to rounding errors, the halfDet
	  // value is clamped to [-1,1].
	  const auto traceDiv3 = (a00 + a11 + a22)/3;
	  const auto b00 = a00 - traceDiv3;
	  const auto b11 = a11 - traceDiv3;
	  const auto b22 = a22 - traceDiv3;
	  const auto denom = std::sqrt((b00 * b00 + b11 * b11 + b22 * b22 + 2*norm)/6);
	  const auto c00 = b11 * b22 - a12 * a12;
	  const auto c01 = a01 * b22 - a12 * a02;
	  const auto c02 = a01 * a12 - b11 * a02;
	  const auto det = (b00 * c00 - a01 * c01 + a02 * c02) / (denom * denom * denom);
	  const auto halfDet = std::min(std::max(det/2,-one), one);
	  // The eigenvalues of B are ordered as beta0 <= beta1 <= beta2.  The
	  // number of digits in twoThirdsPi is chosen so that, whether float or
	  // double, the floating-point number is the closest to theoretical 2*pi/3.
	  const auto angle = std::acos(halfDet)/3;
	  const auto twoThirdsPi = real(2.09439510239319549);
	  const auto beta2 = 2*std::cos(angle);
	  const auto beta0 = 2*std::cos(angle + twoThirdsPi);
	  const auto beta1 = -(beta0 + beta2);
	  // The eigenvalues of A are ordered as alpha0 <= alpha1 <= alpha2.
	  vp[0] = traceDiv3 + denom * beta0;
	  vp[1] = traceDiv3 + denom * beta1;
	  vp[2] = traceDiv3 + denom * beta2;
	  // The index i0 corresponds to the root guaranteed to have multiplicity 1
	  // and goes with either the maximum root or the minimum root.  The index
	  // i2 goes with the root of the opposite extreme.  Root beta2 is always
	  // between beta0 and beta1.
	  int i0, i1 = 1;
	  if (halfDet >= zero){
	    i0 = 2;
	  } else {
	    i0 = 0;
	  }
	  // compute the eigenvectors.  The set { m[0], m[1], m[2] } is
	  // right handed and orthonormal.
	  tvector<3u,real> e0 = {m(i0,0),m(i0,1),m(i0,2)};
	  tvector<3u,real> e1 = {m(i1,0),m(i1,1),m(i1,2)};
	  computeEigenvector0(a00, a01, a02, a11, a12, a22, vp[i0], e0);
	  computeEigenvector1(a00, a01, a02, a11, a12, a22, e0, vp[i1],e1);
	  const auto e2 = cross_product(e0,e1);
	  m = {e0[0],e1[0],e2[0],
	       e0[1],e1[1],e2[1],
	       e0[2],e1[2],e2[2]};
	} else {
	  // The matrix is diagonal.
	  vp = {a00,a11,a22};
	  m  = tmatrix<3u,3u,real>::Id();
	}
	// The preconditioning scaled the matrix A, which scales the eigenvalues.
	// Revert the scaling.
	vp[0] *= maxAbsElement;
	vp[1] *= maxAbsElement;
	vp[2] *= maxAbsElement;
      }

      template <typename real>
      tvector<3u,real> GteNISymmetricEigensolver3x3<real>::cross_product(const tvector<3u,real>& U,
									 const tvector<3u,real>& V)
      {
	return {U[1] * V[2] - U[2] * V[1],
	    U[2] * V[0] - U[0] * V[2],
	    U[0] * V[1] - U[1] * V[0]};
      }

      template <typename real>
      void GteNISymmetricEigensolver3x3<real>::computeOrthogonalComplement(const tvector<3u,real>& W,
									   tvector<3u,real>& U,
									   tvector<3u,real>& V)
      {
	constexpr const real zero  = real(0);
	constexpr const real one   = real(1);
	// Robustly compute a right-handed orthonormal set { U, V, W }.  The
	// vector W is guaranteed to be unit-length, in which case there is no
	// need to worry about a division by zero when computing invLength.
	if (std::abs(W[0]) > std::abs(W[1])){
	  // The component of maximum absolute value is either W[0] or W[2].
	  const auto iL = one/std::sqrt(W[0] * W[0] + W[2] * W[2]);
	  U = { -W[2] * iL, zero, +W[0] * iL };
	} else {
	  // The component of maximum absolute value is either W[1] or W[2].
	  const auto iL = one/std::sqrt(W[1] * W[1] + W[2] * W[2]);
	  U = { zero, +W[2] * iL, -W[1] * iL };
	}
	V = cross_product(W, U);
      }

      template <typename real>
      void GteNISymmetricEigensolver3x3<real>::computeEigenvector0(real a00, real a01,
								   real a02, real a11,
								   real a12, real a22,
								   real& vp0, tvector<3u,real>& m0)
      {
	// compute a unit-length eigenvector for eigenvalue[i0].  The matrix is
	// rank 2, so two of the rows are linearly independent.  For a robust
	// computation of the eigenvector, select the two rows whose cross product
	// has largest length of all pairs of rows.
	tvector<3u,real> row0 = { a00 - vp0, a01, a02 };
	tvector<3u,real> row1 = { a01, a11 - vp0, a12 };
	tvector<3u,real> row2 = { a02, a12, a22 - vp0 };
	tvector<3u,real> r0xr1 = cross_product(row0, row1);
	tvector<3u,real> r0xr2 = cross_product(row0, row2);
	tvector<3u,real> r1xr2 = cross_product(row1, row2);
	const auto d0 = r0xr1|r0xr1;
	const auto d1 = r0xr2|r0xr2;
	const auto d2 = r1xr2|r1xr2;

	real dmax = d0;
	int imax = 0;
	if (d1 > dmax){
	  dmax = d1;
	  imax = 1;
	}
	if (d2 > dmax){
	  imax = 2;
	}
	if (imax == 0){
	  m0 = r0xr1/std::sqrt(d0);
	} else if (imax == 1){
	  m0 = r0xr2/std::sqrt(d1);
	} else {
	  m0 = r1xr2/std::sqrt(d2);
	}
      }

      template <typename real>
      void GteNISymmetricEigensolver3x3<real>::computeEigenvector1(real a00, real a01,
								   real a02, real a11,
								   real a12, real a22,
								   const tvector<3u,real>& m0,
								   real& vp1,
								   tvector<3u,real>& m1)
      {
	constexpr const real zero  = real(0);
	constexpr const real one   = real(1);
	// Robustly compute a right-handed orthonormal set { U, V, m0 }.
	tvector<3u,real> U, V;
	computeOrthogonalComplement(m0, U, V);

	// Let e be vp1 and let E be a corresponding eigenvector which is a
	// solution to the linear system (A - e*I)*E = 0.  The matrix (A - e*I)
	// is 3x3, not invertible (so infinitely many solutions), and has rank 2
	// when vp1 and vp are different.  It has rank 1 when vp1 and vp2
	// are equal.  Numerically, it is difficult to compute robustly the rank
	// of a matrix.  Instead, the 3x3 linear system is reduced to a 2x2 system
	// as follows.  Define the 3x2 matrix J = [U V] whose columns are the U
	// and V computed previously.  Define the 2x1 vector X = J*E.  The 2x2
	// system is 0 = M * X = (J^T * (A - e*I) * J) * X where J^T is the
	// transpose of J and M = J^T * (A - e*I) * J is a 2x2 matrix.  The system
	// may be written as
	//     +-                        -++-  -+       +-  -+
	//     | U^T*A*U - e  U^T*A*V     || x0 | = e * | x0 |
	//     | V^T*A*U      V^T*A*V - e || x1 |       | x1 |
	//     +-                        -++   -+       +-  -+
	// where X has row entries x0 and x1.

	tvector<3u,real> AU =
	  {
	    a00 * U[0] + a01 * U[1] + a02 * U[2],
	    a01 * U[0] + a11 * U[1] + a12 * U[2],
	    a02 * U[0] + a12 * U[1] + a22 * U[2]
	  };

	tvector<3u,real> AV =
	  {
	    a00 * V[0] + a01 * V[1] + a02 * V[2],
	    a01 * V[0] + a11 * V[1] + a12 * V[2],
	    a02 * V[0] + a12 * V[1] + a22 * V[2]
	  };

	real m00 = U[0] * AU[0] + U[1] * AU[1] + U[2] * AU[2] - vp1;
	real m01 = U[0] * AV[0] + U[1] * AV[1] + U[2] * AV[2];
	real m11 = V[0] * AV[0] + V[1] * AV[1] + V[2] * AV[2] - vp1;

	// For robustness, choose the largest-length row of M to compute the
	// eigenvector.  The 2-tuple of coefficients of U and V in the
	// assignments to eigenvector[1] lies on a circle, and U and V are
	// unit length and perpendicular, so eigenvector[1] is unit length
	// (within numerical tolerance).
	real absM00 = std::abs(m00);
	real absM01 = std::abs(m01);
	real absM11 = std::abs(m11);
	real maxAbsComp;
	if (absM00 >= absM11){
	  maxAbsComp = std::max(absM00, absM01);
	  if (maxAbsComp > zero){
	    if (absM00 >= absM01){
	      m01 /= m00;
	      m00 = one / std::sqrt(one + m01 * m01);
	      m01 *= m00;
	    } else {
	      m00 /= m01;
	      m01 = one / std::sqrt(one + m00 * m00);
	      m00 *= m01;
	    }
	    m1 = m01*U-m00*V;
	  } else {
	    m1 = U;
	  }
	} else {
	  maxAbsComp = std::max(absM11, absM01);
	  if (maxAbsComp > zero) {
	    if (absM11 >= absM01) {
	      m01 /= m11;
	      m11 = one / std::sqrt(one + m01 * m01);
	      m01 *= m11;
	    } else {
	      m11 /= m01;
	      m01 = one / std::sqrt(one + m11 * m11);
	      m11 *= m01;
	    }
	    m1 = m11*U-m01*V;
	  } else {
	    m1 = U;
	  }
	}
      }

    } // end of namespace internals
      
  } // end of namespace math

} // end of namespace tfel

#endif /* LIB_TFEL_MATH_GTESYMMETRICEIGENSOLVER_IXX */
