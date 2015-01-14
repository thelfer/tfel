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
    
    QRException::~QRException() noexcept
    {} // end of QRException::~QRException

    const char*
    QRUnmatchedSize::what() const noexcept 
    {
      return "QRDecomp : entry size does not match";
    } // end of QRUnmatchedSize::what

    QRUnmatchedSize::~QRUnmatchedSize() noexcept
    {} // end of QRUnmatchedSize::~QRUnmatchedSize

    const char*
    QRInvalidMatrixSize::what() const noexcept
    {
      return "QRDecomp : null matrix size";
    } // end of QRInvalidMatrixSize::what

    QRInvalidMatrixSize::~QRInvalidMatrixSize() noexcept
    {} // end of QRInvalidMatrixSize::~QRInvalidMatrixSize

    const char*
    QRNullPivot::what() const noexcept
    {
      return "QRDecomp::back_substitute : null pivot";
    } // end of QRNullPivot::QRNullPivot

    QRNullPivot::~QRNullPivot() noexcept
    {} // end of QRNullPivot::~QRNullPivot

  } // end of namespace math

} // end of namespace tfel
