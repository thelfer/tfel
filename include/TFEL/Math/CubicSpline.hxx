/*!
 * \file   CubicSpline.hxx
 * \author Castelier Etienne
 * \date   07/08/2007
 */

#ifndef _LIB_TFEL_MATH_CUBICSPLINE_HXX
#define _LIB_TFEL_MATH_CUBICSPLINE_HXX 1

#include <vector>

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Exception/TFELException.hxx"

namespace tfel
{

  namespace math
  {

    /*!
     * Exception thrown when calling any of the getValue method before
     * calling setCollocationPoints.
     */
    struct TFEL_VISIBILITY_EXPORT CubicSplineUninitialised
      : public tfel::exception::TFELException
    {
      /*!
       * default constructor
       */
      CubicSplineUninitialised();
    }; // end of struct CubicSplineUninitialised

    /*!
     * Exception thrown when the abscissa vector given to the
     * CubicSpline constructor has a size smaller than 3.
     */
    struct TFEL_VISIBILITY_EXPORT CubicSplineInvalidAbscissaVectorSize
      : public tfel::exception::TFELException
    {
      /*!
       * default constructor
       */
      CubicSplineInvalidAbscissaVectorSize();
    }; // end of struct CubicSplineInvalidAbscissaVectorSize

    /*!
     * Exception thrown when the linear system solved leads to a null pivot
     */
    struct TFEL_VISIBILITY_EXPORT CubicSplineNullPivot
      : public tfel::exception::TFELException
    {
      /*!
       * default constructor
       */
      CubicSplineNullPivot();
    }; // end of struct CubicSplineInvalidAbscissaVectorSize

    /*!
     * Exception thrown when the ordinate vector given to the
     * CubicSpline constructor has a size smaller than 3.
     */
    struct TFEL_VISIBILITY_EXPORT CubicSplineInvalidOrdinateVectorSize
      : public tfel::exception::TFELException
    {
      /*!
       * default constructor
       */
      CubicSplineInvalidOrdinateVectorSize();
    }; // end of struct CubicSplineInvalidOrdinateVectorSize

    /*!
     * Exception thrown when the abscissa and the ordinate vectors
     * don't have the same size
     */
    struct TFEL_VISIBILITY_EXPORT CubicSplineInvalidInputs
      : public tfel::exception::TFELException
    {
      /*!
       * default constructor
       */
      CubicSplineInvalidInputs();
    }; // end of struct CubicSplineInvalidInputs

    /*!
     * Exception thrown when the abscissa vector is not ordered
     */
    struct TFEL_VISIBILITY_EXPORT CubicSplineUnorderedAbscissaVector
      : public tfel::exception::TFELException
    {
      /*!
       * default constructor
       */
      CubicSplineUnorderedAbscissaVector();
    }; // end of struct CubicSplineUnorderedAbscissaVector

    /*!
     * structure in charge of computing the cubic spline of a series
     * of collocation points
     *
     * \param real  : floatting type number used for the computations
     * \param value : result type
     */
    template<typename real,
	     typename value = real>
    struct CubicSpline
    {
      
      /*!
       * \param[in] px  : iterator to the first abscissa
       * \param[in] pxe : iterator past the last abscissa
       * \param[in] py  : iterator to the first ordinate
       */
      template<typename AIterator,
	       typename OIterator>
      void
      setCollocationPoints(AIterator,
			   AIterator,
			   OIterator);

      /*!
       * \param[in] x : abscissa
       * \param[in] y : ordinates
       */
      template<typename AContainer,
	       typename OContainer>
      void
      setCollocationPoints(const AContainer&,
			   const OContainer&);

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
      value computeIntegral(const real,
			    const real) const;

      /*!
       * \return the spline mean value
       * \param[in] xa : point at which the spline is evaluated
       * \param[in] xb : point at which the spline is evaluated
       */
      value computeMeanValue(const real,
			     const real) const;

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
      void getValues(value&,value&,real) const;
      
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
      void getValues(value&,value&,value&,real) const;
            
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
      }; // end of struct Point

      /*!
       * internal structure to compare collocation points
       */
      struct PointComparator
      {
	/*!
	 * \return true if p->x < x
	 * \param p : first point
	 * \param x : abscissa
	 */
      	bool
	operator()(const Point&,const real&) const; 
      }; //end of struct PointComparator

      /*!
       * http://en.wikipedia.org/wiki/Tridiagonal_matrix_algorithm
       * n - number of equations
       * c - sup-diagonal (means it is the diagonal above the main diagonal) -- indexed from 0..n-2
       * b - the main diagonal
       * x - the answer
       */
      void
      solveTridiagonalLinearSystem(std::vector<value>&,
				   const real * const,
				   real * const);
      
      /*!
       * an helper function to ease integral computations
       */
      static value computeLocalIntegral(const real,
					const real,
					const typename std::vector<Point>::const_iterator);

      /*!
       * an helper function to ease factorize computations
       */
      static void computeLocalCoefficients(value&,
					   value&,
					   const typename std::vector<Point>::const_iterator);
      
      /*!
       * build the spline interpolation
       */
      void buildInterpolation();
    
      /*!
       * collocation points
       */
      std::vector<Point> values;
  
    }; // end of struct CubicSpline

  } // end of namespace math

} // end of namespace tfel

#include"TFEL/Math/CubicSpline.ixx"

#endif /* _LIB_TFEL_MATH_CUBICSPLINE_HXX */

