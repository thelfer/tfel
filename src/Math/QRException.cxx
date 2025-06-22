/*!
 * \file  src/Math/QRException.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 27 mai 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "TFEL/Math/QR/QRException.hxx"

namespace tfel {

  namespace math {

    QRException::QRException() = default;
    QRException::QRException(QRException&&) = default;
    QRException::QRException(const QRException&) = default;
    QRException::~QRException() noexcept = default;

    QRUnmatchedSize::QRUnmatchedSize() = default;
    QRUnmatchedSize::QRUnmatchedSize(QRUnmatchedSize&&) = default;
    QRUnmatchedSize::QRUnmatchedSize(const QRUnmatchedSize&) = default;
    QRUnmatchedSize::~QRUnmatchedSize() noexcept = default;

    const char* QRUnmatchedSize::what() const noexcept {
      return "QRDecomp : entry size does not match";
    }  // end of QRUnmatchedSize::what

    QRInvalidMatrixSize::QRInvalidMatrixSize() = default;
    QRInvalidMatrixSize::QRInvalidMatrixSize(QRInvalidMatrixSize&&) = default;
    QRInvalidMatrixSize::QRInvalidMatrixSize(const QRInvalidMatrixSize&) =
        default;
    QRInvalidMatrixSize::~QRInvalidMatrixSize() noexcept = default;

    const char* QRInvalidMatrixSize::what() const noexcept {
      return "QRDecomp : null matrix size";
    }  // end of QRInvalidMatrixSize::what

    QRNullPivot::QRNullPivot() = default;
    QRNullPivot::QRNullPivot(QRNullPivot&&) = default;
    QRNullPivot::QRNullPivot(const QRNullPivot&) = default;
    QRNullPivot::~QRNullPivot() noexcept = default;

    const char* QRNullPivot::what() const noexcept {
      return "QRDecomp::back_substitute : null pivot";
    }  // end of QRNullPivot::QRNullPivot

  }  // end of namespace math

}  // end of namespace tfel
