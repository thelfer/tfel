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
#include "TFEL/Math/General/DerivativeType.hxx"

namespace tfel::math {

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
      CubicSplineInvalidAbscissaVectorSize final : public MathRunTimeException {
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
      CubicSplineInvalidOrdinateVectorSize final : public MathRunTimeException {
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

  //! \brief structure which represents a collocation point
  template <typename AbscissaType, typename ValueType>
  struct CubicSplineCollocationPoint {
    //! \brief a simple alias
    using abscissa_type = AbscissaType;
    //! \brief a simple alias
    using value_type = ValueType;
    //! \brief abscissa
    AbscissaType x;
    //! \brief ordinate
    ValueType y;
    //! \brief derivate
    derivative_type<ValueType, AbscissaType> d;
  };  // end of struct Point

  /*!
   * \return the interpolation of a set of collocation points at the given
   * abscissa
   * \param[in] points: collocation points
   * \param[in] x: abscissa
   *
   * \tparam extrapolate: boolean stating if extrapolation must be performed
   *
   * \note the points are assumed ordered from lower to greater values.
   */
  template <bool extrapolate,
            typename CollocationPointContainer,
            typename AbscissaType>
  constexpr auto computeCubicSplineInterpolation(
      const CollocationPointContainer&, const AbscissaType);
  /*!
   * \return the interpolation of a set of collocation points at the given
   * abscissa and the derivative at this point
   * \param[in] points: collocation points
   * \param[in] x: abscissa
   *
   * \tparam extrapolate: boolean stating if extrapolation must be performed
   *
   * \note the points are assumed ordered from lower to greater values.
   */
  template <bool extrapolate,
            typename CollocationPointContainer,
            typename AbscissaType>
  constexpr auto computeCubicSplineInterpolationAndDerivative(
      const CollocationPointContainer&, const AbscissaType);

  /*!
   * \brief structure in charge of computing the cubic spline of a series
   * of collocation points
   *
   * \tparam AbscissaType: floatting type number used for the computations
   * \tparam ValueType: result type
   */
  template <typename AbscissaType, typename ValueType = AbscissaType>
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
    ValueType operator()(const AbscissaType) const;
    /*!
     * \return the spline integral
     * \param[in] xa: beginning of interval
     * \param[in] xb: end of interval
     */
    result_type<ValueType, AbscissaType, OpMult> computeIntegral(
        const AbscissaType, const AbscissaType) const;
    /*!
     * \return the spline mean value
     * \param[in] xa: beginning of interval
     * \param[in] xb: end of interval
     */
    ValueType computeMeanValue(const AbscissaType, const AbscissaType) const;
    /*!
     * \return the spline value at the given point
     * \param[in] x : point at which the spline is evaluated
     */
    ValueType getValue(const AbscissaType) const;
    /*!
     * \return the value of the spline and the value of its
     * derivative at the given point
     *
     * \param[out] f  : spline value
     * \param[out] df : spline derivative value
     * \param[in]  x  : point at which the spline is evaluated
     *
     */
    void getValues(ValueType&,
                   derivative_type<ValueType, AbscissaType>&,
                   const AbscissaType) const;
    /*!
     * \return the value of the spline and the value of its
     * derivative at the given point
     *
     * \param[out] f   : spline value
     * \param[out] df  : spline derivative value
     * \param[out] d2f : spline second derivative value
     * \param[in]  x   : point at which the spline is evaluated
     */
    void getValues(ValueType&,
                   derivative_type<ValueType, AbscissaType>&,
                   derivative_type<ValueType, AbscissaType, AbscissaType>&,
                   const AbscissaType) const;
    //! \brief return the collocation points
    const auto& getCollocationPoints() const;

   protected:
    //! \brief a simple alias
    using Point = CubicSplineCollocationPoint<AbscissaType, ValueType>;
    //! \brief internal structure to compare collocation points
    struct PointComparator {
      /*!
       * \return true if p->x < x
       * \param p : first point
       * \param x : abscissa
       */
      constexpr bool operator()(const Point&,
                                const AbscissaType&) const noexcept;
    };  // end of struct PointComparator

    /*!
     * http://en.wikipedia.org/wiki/Tridiagonal_matrix_algorithm
     * n - number of equations
     * c - sup-diagonal (means it is the diagonal above the main diagonal) --
     * indexed from 0..n-2 b - the main diagonal x - the answer
     */
    void solveTridiagonalLinearSystem(const AbscissaType* const,
                                      AbscissaType* const);

    //! \brief build the spline interpolation
    void buildInterpolation();

    //! \brief collocation points
    std::vector<Point> points;
  };  // end of struct CubicSpline

}  // end of namespace tfel::math

namespace tfel::typetraits {

  template <typename AbscissaType,
            typename ValueType,
            typename AbscissaType2,
            typename ValueType2>
  struct IsAssignableTo<
      tfel::math::CubicSplineCollocationPoint<AbscissaType, ValueType>,
      tfel::math::CubicSplineCollocationPoint<AbscissaType2, ValueType2>> {
    static constexpr bool value =
        isAssignableTo<AbscissaType, AbscissaType2>() &&
        isAssignableTo<ValueType, ValueType2>();
    static constexpr bool cond =
        isAssignableTo<AbscissaType, AbscissaType2>() &&
        isAssignableTo<ValueType, ValueType2>();
  };

}  // end of namespace tfel::typetraits

#include "TFEL/Math/CubicSpline.ixx"

#endif /* LIB_TFEL_MATH_CUBICSPLINE_HXX */
