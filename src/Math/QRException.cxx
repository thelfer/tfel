/*! 
 * \file  src/Math/QRException.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 27 mai 2013
 */

#include"TFEL/Math/QR/QRException.hxx"
 
namespace tfel
{

  namespace math
  {
    
    QRException::~QRException() throw()
    {} // end of QRException::~QRException

    QRException::QRException(const char* const m)
      : tfel::exception::TFELException(m)
    {} // end of QRException::QRException

    QRUnmatchedSize::QRUnmatchedSize() 
      : QRException("QRDecomp : entry size does not match")
    {} // end of QRUnmatchedSize::QRUnmatchedSize

    QRUnmatchedSize::~QRUnmatchedSize() throw()
    {} // end of QRUnmatchedSize::~QRUnmatchedSize

    QRInvalidMatrixSize::QRInvalidMatrixSize()
      : QRException("QRDecomp : null matrix size")
    {} // end of QRInvalidMatrixSize::QRInvalidMatrixSize

    QRInvalidMatrixSize::~QRInvalidMatrixSize() throw()
    {} // end of QRInvalidMatrixSize::~QRInvalidMatrixSize

    QRNullPivot::QRNullPivot()
      : QRException("QRDecomp::back_substitute : "
		    "null pivot")
    {} // end of QRNullPivot::QRNullPivot

    QRNullPivot::~QRNullPivot() throw()
    {} // end of QRNullPivot::~QRNullPivot

  } // end of namespace math

} // end of namespace tfel
