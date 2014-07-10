/*! 
 * \file  Discretization1D.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 05 déc. 2011
 */

#ifndef _LIB_TFEL_MATH_DISCRETIZATION1D_H_
#define _LIB_TFEL_MATH_DISCRETIZATION1D_H_ 

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Math/MathException.hxx"

namespace tfel
{

  namespace math
  {
    
    /*!
     * Exception thrown when the density given to the
     * geometricDiscretization function is invalid
     */
    struct TFELMATH_VISIBILITY_EXPORT GeometricDiscretizationInvalidDensity
      : public MathRunTimeException
    {
      /*!
       * default constructor
       */
      GeometricDiscretizationInvalidDensity();
    }; // end of struct GeometricDiscretizationInvalidDensity

    /*!
     * Exception thrown when the length given to the
     * geometricDiscretization function is invalid
     */
    struct TFELMATH_VISIBILITY_EXPORT GeometricDiscretizationInvalidLength
      : public MathRunTimeException
    {
      /*!
       * default constructor
       */
      GeometricDiscretizationInvalidLength();
    }; // end of struct GeometricDiscretizationInvalidLength

    /*!
     * Exception thrown when the number of elements given to the
     * geometricDiscretization function is invalid
     */
    struct TFELMATH_VISIBILITY_EXPORT GeometricDiscretizationInvalidNumberOfElements
      : public MathRunTimeException
    {
      /*!
       * default constructor
       */
      GeometricDiscretizationInvalidNumberOfElements();
    }; // end of struct GeometricDiscretizationInvalidNumberOfElements

    /*!
     * \brief discretize a segment into a fixed number of elements
     * trying to satisfy as much as possible given discretization
     * densities at the beginning and the end of the segment. Elements
     * size grows according to a geometric progression.
     *
     * \param T  : a "stl vector like" container
     * \param xb : starting point 
     * \param xe : last point 
     * \param db : density of the discretization at the starting point 
     * \param db : density of the discretization at the last point 
     * \param n  : number of elements
     */
    template<typename T>
    void
    geometricDiscretization(T& v,
			    const typename T::value_type,
			    const typename T::value_type,
			    const typename T::value_type,
			    const typename T::value_type,
			    const typename T::size_type);
    
  } // end of namespace math

} // end of namespace tfel

#include"TFEL/Math/Discretization1D.ixx"

#endif /* _LIB_TFEL_MATH_DISCRETIZATION1D_H */

