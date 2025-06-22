/*!
 * \file   include/TFEL/Math/CubicSpline.hxx
 * \author Castelier Etienne
 * \date   07/08/2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_CUBICSPLINE_HXX
#define LIB_TFEL_MATH_CUBICSPLINE_HXX 1

#include <vector>

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Math/MathException.hxx"

namespace tfel {

  namespace math {

    /*!
     * Exception thrown when calling any of the getValue method before
     * calling setCollocationPoints.
     */
    struct TFELMATHCUBICSPLINE_VISIBILITY_EXPORT CubicSplineUninitialised final
        : public MathRunTimeException {
      CubicSplineUninitialised() = default;
      CubicSplineUninitialised(const CubicSplineUninitialised&) = default;
      CubicSplineUninitialised(CubicSplineUninitialised&&) = default;
      const char* what() const noexcept override final;
      ~CubicSplineUninitialised() noexcept override;
    };  // end of struct CubicSplineUninitialised

    /*!
     * Exception thrown when the abscissa vector given to the
     * CubicSpline constructor has a size smaller than 3.
     */
    struct TFELMATHCUBICSPLINE_VISIBILITY_EXPORT
        CubicSplineInvalidAbscissaVectorSize final
        : public MathRunTimeException {
      CubicSplineInvalidAbscissaVectorSize() = default;
      CubicSplineInvalidAbscissaVectorSize(
          const CubicSplineInvalidAbscissaVectorSize&) = default;
      CubicSplineInvalidAbscissaVectorSize(
          CubicSplineInvalidAbscissaVectorSize&&) = default;
      const char* what() const noexcept override final;
      ~CubicSplineInvalidAbscissaVectorSize() noexcept override;
    };  // end of struct CubicSplineInvalidAbscissaVectorSize

    /*!
     * Exception thrown when the linear system solved leads to a null pivot
     */
    struct TFELMATHCUBICSPLINE_VISIBILITY_EXPORT CubicSplineNullPivot final
        : public MathRunTimeException {
      CubicSplineNullPivot() = default;
      CubicSplineNullPivot(const CubicSplineNullPivot&) = default;
      CubicSplineNullPivot(CubicSplineNullPivot&&) = default;
      const char* what() const noexcept override final;
      ~CubicSplineNullPivot() noexcept override;
    };  // end of struct CubicSplineInvalidAbscissaVectorSize

    /*!
     * Exception thrown when the ordinate vector given to the
     * CubicSpline constructor has a size smaller than 3.
     */
    struct TFELMATHCUBICSPLINE_VISIBILITY_EXPORT
        CubicSplineInvalidOrdinateVectorSize final
        : public MathRunTimeException {
      CubicSplineInvalidOrdinateVectorSize() = default;
      CubicSplineInvalidOrdinateVectorSize(
          const CubicSplineInvalidOrdinateVectorSize&) = default;
      CubicSplineInvalidOrdinateVectorSize(
          CubicSplineInvalidOrdinateVectorSize&&) = default;
      const char* what() const noexcept override final;
      ~CubicSplineInvalidOrdinateVectorSize() noexcept override;
    };  // end of struct CubicSplineInvalidOrdinateVectorSize

    /*!
     * Exception thrown when the abscissa and the ordinate vectors
     * don't have the same size
     */
    struct TFELMATHCUBICSPLINE_VISIBILITY_EXPORT CubicSplineInvalidInputs final
        : public MathRunTimeException {
      CubicSplineInvalidInputs() = default;
      CubicSplineInvalidInputs(const CubicSplineInvalidInputs&) = default;
      CubicSplineInvalidInputs(CubicSplineInvalidInputs&&) = default;
      const char* what() const noexcept override final;
      ~CubicSplineInvalidInputs() noexcept override;
    };  // end of struct CubicSplineInvalidInputs

    /*!
     * Exception thrown when the abscissa vector is not ordered
     */
    struct TFELMATHCUBICSPLINE_VISIBILITY_EXPORT
        CubicSplineUnorderedAbscissaVector final : public MathRunTimeException {
      CubicSplineUnorderedAbscissaVector() = default;
      CubicSplineUnorderedAbscissaVector(
          const CubicSplineUnorderedAbscissaVector&) = default;
      CubicSplineUnorderedAbscissaVector(CubicSplineUnorderedAbscissaVector&&) =
          default;
      const char* what() const noexcept override final;
      ~CubicSplineUnorderedAbscissaVector() noexcept override;
    };  // end of struct CubicSplineUnorderedAbscissaVector

    /*!
     * structure in charge of computing the cubic spline of a series
     * of collocation points
     *
     * \param real  : floatting type number used for the computations
     * \param value : result type
     */
    template <typename real, typename value = real>
    struct CubicSpline {
      /*!
       * \param[in] px  : iterator to the first abscissa
       * \param[in] pxe : iterator past the last abscissa
       * \param[in] py  : iterator to the first ordinate
       */
      template <typename AIterator, typename OIterator>
      void setCollocationPoints(AIterator, AIterator, OIterator);

      /*!
       * \param[in] x : abscissa
       * \param[in] y : ordinates
       */
      template <typename AContainer, typename OContainer>
      void setCollocationPoints(const AContainer&, const OContainer&);
      /*!
       * \return the spline value at the given point
       * \param[in] x : point at which the spline is evaluated
       */
      value operator()(const real) const;
      /*!
       * \return the spline integral
       * \param[in] xa : left end point
       * \param[in] xb : right end point
       */
      value computeIntegral(const real, const real) const;
      /*!
       * \return the spline mean value
       * \param[in] xa : point at which the spline is evaluated
       * \param[in] xb : point at which the spline is evaluated
       */
      value computeMeanValue(const real, const real) const;
      /*!
       * \return the spline value at the given point
       * \param[in] x : point at which the spline is evaluated
       */
      value getValue(real) const;
      /*!
       * \return the value of the spline and the value of its
       * derivative at the given point
       *
       * \param[out] f  : spline value
       * \param[out] df : spline derivative value
       * \param[in]  x  : point at which the spline is evaluated
       *
       */
      void getValues(value&, value&, real) const;
      /*!
       * \return the value of the spline and the value of its
       * derivative at the given point
       *
       * \param[out] f   : spline value
       * \param[out] df  : spline derivative value
       * \param[out] d2f : spline derivative value
       * \param[in]  x   : point at which the spline is evaluated
       *
       */
      void getValues(value&, value&, value&, real) const;

     protected:
      /*!
       * internal structure which represents a collocation point
       */
      struct Point {
        //! abscissa
        real x;
        //! ordinate
        value y;
        //! derivate
        value d;
      };  // end of struct Point

      /*!
       * internal structure to compare collocation points
       */
      struct PointComparator {
        /*!
         * \return true if p->x < x
         * \param p : first point
         * \param x : abscissa
         */
        bool operator()(const Point&, const real&) const;
      };  // end of struct PointComparator

      /*!
       * http://en.wikipedia.org/wiki/Tridiagonal_matrix_algorithm
       * n - number of equations
       * c - sup-diagonal (means it is the diagonal above the main diagonal) --
       * indexed from 0..n-2 b - the main diagonal x - the answer
       */
      void solveTridiagonalLinearSystem(const real* const, real* const);

      /*!
       * an helper function to ease integral computations
       */
      static value computeLocalIntegral(
          const real,
          const real,
          const typename std::vector<Point>::const_iterator);

      /*!
       * an helper function to ease factorize computations
       */
      static void computeLocalCoefficients(
          value&, value&, const typename std::vector<Point>::const_iterator);

      /*!
       * build the spline interpolation
       */
      void buildInterpolation();

      /*!
       * collocation points
       */
      std::vector<Point> values;

    };  // end of struct CubicSpline

  }  // end of namespace math

}  // end of namespace tfel

#include "TFEL/Math/CubicSpline.ixx"

#endif /* LIB_TFEL_MATH_CUBICSPLINE_HXX */
