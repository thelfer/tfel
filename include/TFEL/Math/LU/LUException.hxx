/*!
 * \file  include/TFEL/Math/LU/LUException.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 12 avr 2009
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_LUEXCEPTION_HXX
#define LIB_TFEL_MATH_LUEXCEPTION_HXX

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Math/MathException.hxx"

namespace tfel {

  namespace math {

    /*!
     * Base class for the exception thrown by
     * the LUDecomp class
     */
    struct TFELMATH_VISIBILITY_EXPORT LUException
        : public MathRunTimeException {
      LUException();
      LUException(LUException&&);
      LUException(const LUException&);
      //! destructor
      virtual ~LUException() noexcept;
    };  // end of struct LUException

    /*!
     * Base class for the exception thrown by
     * the LUDecomp class
     */
    struct TFELMATH_VISIBILITY_EXPORT LUUnmatchedSize : public LUException {
      LUUnmatchedSize();
      LUUnmatchedSize(LUUnmatchedSize&&);
      LUUnmatchedSize(const LUUnmatchedSize&);
      //! \return a string describing the error
      virtual const char* what() const noexcept override final;
      //! destructor
      virtual ~LUUnmatchedSize() noexcept;
    };  // end of struct LUException

    /*!
     * Base class for the exception thrown by
     * the LUDecomp class
     */
    struct TFELMATH_VISIBILITY_EXPORT LUMatrixNotSquare : public LUException {
      LUMatrixNotSquare();
      LUMatrixNotSquare(LUMatrixNotSquare&&);
      LUMatrixNotSquare(const LUMatrixNotSquare&);
      //! \return a string describing the error
      virtual const char* what() const noexcept override final;
      //! destructor
      virtual ~LUMatrixNotSquare() noexcept;
    };  // end of struct LUException

    /*!
     * Base class for the exception thrown by
     * the LUDecomp class
     */
    struct TFELMATH_VISIBILITY_EXPORT LUInvalidMatrixSize : public LUException {
      LUInvalidMatrixSize();
      LUInvalidMatrixSize(LUInvalidMatrixSize&&);
      LUInvalidMatrixSize(const LUInvalidMatrixSize&);
      //! \return a string describing the error
      virtual const char* what() const noexcept override final;
      //! destructor
      virtual ~LUInvalidMatrixSize() noexcept;
    };  // end of struct LUException

    /*!
     * Base class for the exception thrown by
     * the LUDecomp class
     */
    struct TFELMATH_VISIBILITY_EXPORT LUNullPivot : public LUException {
      LUNullPivot();
      LUNullPivot(LUNullPivot&&);
      LUNullPivot(const LUNullPivot&);
      //! \return a string describing the error
      virtual const char* what() const noexcept override final;
      //! destructor
      virtual ~LUNullPivot() noexcept;
    };  // end of struct LUException

    /*!
     * Base class for the exception thrown by
     * the LUDecomp class
     */
    struct TFELMATH_VISIBILITY_EXPORT LUNullDeterminant : public LUException {
      LUNullDeterminant();
      LUNullDeterminant(LUNullDeterminant&&);
      LUNullDeterminant(const LUNullDeterminant&);
      //! \return a string describing the error
      virtual const char* what() const noexcept override final;
      //! destructor
      virtual ~LUNullDeterminant() noexcept;
    };  // end of struct LUException

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_LUEXCEPTION_HXX */
