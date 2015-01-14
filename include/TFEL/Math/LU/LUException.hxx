/*! 
 * \file  include/TFEL/Math/LU/LUException.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 12 avr 2009
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_MATH_LUEXCEPTION_H_
#define _LIB_TFEL_MATH_LUEXCEPTION_H_ 

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Math/MathException.hxx"
 
namespace tfel
{

  namespace math
  {
    
    /*!
     * Base class for the exception thrown by
     * the LUDecomp class
     */
    struct TFELMATH_VISIBILITY_EXPORT LUException
      : public MathRunTimeException
    {
      LUException() = default;
      LUException(LUException&&) = default;
      LUException(const LUException&) = default;
      //! destructor
      virtual ~LUException() noexcept;
    }; // end of struct LUException

    /*!
     * Base class for the exception thrown by
     * the LUDecomp class
     */
    struct TFELMATH_VISIBILITY_EXPORT LUUnmatchedSize
      : public LUException
    {
      LUUnmatchedSize() = default;
      LUUnmatchedSize(LUUnmatchedSize&&) = default;
      LUUnmatchedSize(const LUUnmatchedSize&) = default;
      //! \return a string describing the error
      virtual const char* what() const noexcept final;
      //! destructor
      virtual ~LUUnmatchedSize() noexcept;
    }; // end of struct LUException

    /*!
     * Base class for the exception thrown by
     * the LUDecomp class
     */
    struct TFELMATH_VISIBILITY_EXPORT LUMatrixNotSquare
      : public LUException
    {
      LUMatrixNotSquare() = default;
      LUMatrixNotSquare(LUMatrixNotSquare&&) = default;
      LUMatrixNotSquare(const LUMatrixNotSquare&) = default;
      //! \return a string describing the error
      virtual const char* what() const noexcept final;
      //! destructor
      virtual ~LUMatrixNotSquare() noexcept;
    }; // end of struct LUException

    /*!
     * Base class for the exception thrown by
     * the LUDecomp class
     */
    struct TFELMATH_VISIBILITY_EXPORT LUInvalidMatrixSize
      : public LUException
    {
      LUInvalidMatrixSize() = default;
      LUInvalidMatrixSize(LUInvalidMatrixSize&&) = default;
      LUInvalidMatrixSize(const LUInvalidMatrixSize&) = default;
      //! \return a string describing the error
      virtual const char* what() const noexcept final;
      //! destructor
      virtual ~LUInvalidMatrixSize() noexcept;
    }; // end of struct LUException

    /*!
     * Base class for the exception thrown by
     * the LUDecomp class
     */
    struct TFELMATH_VISIBILITY_EXPORT LUNullPivot
      : public LUException
    {
      LUNullPivot() = default;
      LUNullPivot(LUNullPivot&&) = default;
      LUNullPivot(const LUNullPivot&) = default;
      //! \return a string describing the error
      virtual const char* what() const noexcept final;
      //! destructor
      virtual ~LUNullPivot() noexcept;
    }; // end of struct LUException
    
    /*!
     * Base class for the exception thrown by
     * the LUDecomp class
     */
    struct TFELMATH_VISIBILITY_EXPORT LUNullDeterminant
      : public LUException
    {
      LUNullDeterminant() = default;
      LUNullDeterminant(LUNullDeterminant&&) = default;
      LUNullDeterminant(const LUNullDeterminant&) = default;
      //! \return a string describing the error
      virtual const char* what() const noexcept final;
      //! destructor
      virtual ~LUNullDeterminant() noexcept;
    }; // end of struct LUException
    
  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_LUEXCEPTION_H */

