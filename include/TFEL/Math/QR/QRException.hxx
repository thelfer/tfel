/*!
 * \file  include/TFEL/Math/QR/QRException.hxx
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

#ifndef LIB_TFEL_MATH_QREXCEPTION_HXX
#define LIB_TFEL_MATH_QREXCEPTION_HXX

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Math/MathException.hxx"

namespace tfel {

  namespace math {

    /*!
     * Base class for the exception thrown by
     * the QRDecomp class
     */
    struct TFELMATH_VISIBILITY_EXPORT QRException
        : public MathRunTimeException {
      QRException();
      QRException(QRException&&);
      QRException(const QRException&);
      //! destructor
      virtual ~QRException() noexcept;
    };  // end of struct QRException

    /*!
     * Base class for the exception thrown by
     * the QRDecomp class
     */
    struct TFELMATH_VISIBILITY_EXPORT QRUnmatchedSize : public QRException {
      QRUnmatchedSize();
      QRUnmatchedSize(QRUnmatchedSize&&);
      QRUnmatchedSize(const QRUnmatchedSize&);
      //! \return a string describing the error
      virtual const char* what() const noexcept override final;
      //! destructor
      virtual ~QRUnmatchedSize() noexcept;
    };  // end of struct QRException

    /*!
     * Base class for the exception thrown by
     * the QRDecomp class
     */
    struct TFELMATH_VISIBILITY_EXPORT QRInvalidMatrixSize : public QRException {
      QRInvalidMatrixSize();
      QRInvalidMatrixSize(QRInvalidMatrixSize&&);
      QRInvalidMatrixSize(const QRInvalidMatrixSize&);
      //! \return a string describing the error
      virtual const char* what() const noexcept override final;
      //! destructor
      virtual ~QRInvalidMatrixSize() noexcept;
    };  // end of struct QRException

    /*!
     * Base class for the exception thrown by
     * the QRDecomp class
     */
    struct TFELMATH_VISIBILITY_EXPORT QRNullPivot : public QRException {
      QRNullPivot();
      QRNullPivot(QRNullPivot&&);
      QRNullPivot(const QRNullPivot&);
      //! \return a string describing the error
      virtual const char* what() const noexcept override final;
      //! destructor
      virtual ~QRNullPivot() noexcept;
    };  // end of struct QRException

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_QREXCEPTION_HXX */
