/*!
 * \file   src/Math/TensorConcept.cxx
 *
 * \brief
 * \author Thomas Helfer
 * \date   09 Mar 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "TFEL/Math/Tensor/TensorConcept.hxx"

namespace tfel {

  namespace math {

    const char* TensorInvalidIndexException::what() const noexcept {
      return "invalid index used in tensor access";
    }  // end of TensorInvalidIndexException::what

    TensorInvalidIndexException::~TensorInvalidIndexException() noexcept =
        default;

    const char* TensorInvalidIndexesException::what() const noexcept {
      return "invalid index used in tensor access";
    }  // end of TensorInvalidIndexesException::what() const

    TensorInvalidIndexesException::~TensorInvalidIndexesException() noexcept =
        default;

    const char* TensorInvalidInitializerListSizeException::what()
        const noexcept {
      return "invalid std::initializer_list size";
    }  // end of TensorInvalidInitializerListSizeException::what() const

    TensorInvalidInitializerListSizeException::
        ~TensorInvalidInitializerListSizeException() noexcept = default;

  }  // end of namespace math

}  // end of namespace tfel
