/*!
 * \file   CUDABackend.cxx
 * \brief  This file implements the `CUDABackend` class
 * \author Thomas Helfer
 * \date   07/04/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <ostream>
#include "MFront/VariableDescription.hxx"
#include "MFront/MaterialPropertyDescription.hxx"
#include "MFront/MaterialPropertyParametersHandler.hxx"
#include "MFront/GenericParallelMaterialPropertyInterface.hxx"
#include "MFront/GenericParallel/MaterialProperty/CUDABackend.hxx"

namespace mfront::generic_parallel::material_property {

  std::string CUDABackend::getName() const { return "cuda"; }  // end of getName

  std::string CUDABackend::getProgrammingModel() const noexcept {
    return this->getName();
  }

  std::string CUDABackend::getHeaderFileExtension() const {
    return "hxx";
  }  // end of getHeaderFileExtension

  std::string CUDABackend::getSourceFileExtension() const {
    return "cu";
  }  // end of getSourceFileExtension

  CUDABackend::~CUDABackend() = default;

}  // end of namespace mfront::generic_parallel::material_property
