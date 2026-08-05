/*!
 * \file   HIPBackend.cxx
 * \brief  This file implements the `HIPBackend` class
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
#include "MFront/GenericParallel/MaterialProperty/HIPBackend.hxx"

namespace mfront::generic_parallel::material_property {

  std::string HIPBackend::getName() const { return "hip"; }  // end of getName

  std::string HIPBackend::getProgrammingModel() const noexcept {
    return this->getName();
  }

  std::string HIPBackend::getHeaderFileExtension() const {
    return "hxx";
  }  // end of getHeaderFileExtension

  std::string HIPBackend::getSourceFileExtension() const {
    return "hip";
  }  // end of getSourceFileExtension

  void HIPBackend::writeSpecificIncludesInSourceFile(
      std::ostream& os,
      const GenericParallelMaterialPropertyInterface& i,
      const MaterialPropertyDescription& mpd) const {
    CUDABackendBase::writeSpecificIncludesInSourceFile(os, i, mpd);
    os << "#include \"hip/hip_runtime.h\"\n";
  }  // end of writeSpecificIncludesInSourceFile

  HIPBackend::~HIPBackend() = default;

}  // end of namespace mfront::generic_parallel::material_property
