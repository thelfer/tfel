/*! 
 * \file  src/Math/QRException.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 27 mai 2013
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
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
