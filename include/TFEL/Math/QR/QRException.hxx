/*! 
 * \file  include/TFEL/Math/QR/QRException.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 12 avr 2009
 */

#ifndef _LIB_TFEL_MATH_QREXCEPTION_H_
#define _LIB_TFEL_MATH_QREXCEPTION_H_ 

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Exception/TFELException.hxx"

#include<string>
 
namespace tfel
{

  namespace math
  {
    
    /*!
     * Base class for the exception thrown by
     * the QRDecomp class
     */
    struct TFELMATH_VISIBILITY_EXPORT QRException
      : public tfel::exception::TFELException
    {
      /*!
       * destructor
       */
      virtual ~QRException() throw();
    protected:
      /*!
       * constructor
       * \param[in] m : exception message
       */
      QRException(const char* const);
    }; // end of struct QRException

    /*!
     * Base class for the exception thrown by
     * the QRDecomp class
     */
    struct TFELMATH_VISIBILITY_EXPORT QRUnmatchedSize
      : public QRException
    {
      /*!
       * constructor
       */
      QRUnmatchedSize();
      /*!
       * destructor
       */
      virtual ~QRUnmatchedSize() throw();
    }; // end of struct QRException

    /*!
     * Base class for the exception thrown by
     * the QRDecomp class
     */
    struct TFELMATH_VISIBILITY_EXPORT QRInvalidMatrixSize
      : public QRException
    {
      /*!
       * constructor
       */
      QRInvalidMatrixSize();
      /*!
       * destructor
       */
      virtual ~QRInvalidMatrixSize() throw();
    }; // end of struct QRException

    /*!
     * Base class for the exception thrown by
     * the QRDecomp class
     */
    struct TFELMATH_VISIBILITY_EXPORT QRNullPivot
      : public QRException
    {
      /*!
       * constructor
       */
      QRNullPivot();
      /*!
       * destructor
       */
      virtual ~QRNullPivot() throw();
    }; // end of struct QRException
    
  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_QREXCEPTION_H */

