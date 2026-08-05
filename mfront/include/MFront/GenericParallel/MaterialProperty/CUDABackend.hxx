/*!
 * \file MFront/GenericParallel/MaterialProperty/CUDABackend.hxx
 *
 * \brief  This file declares the `CUDABackend` class
 * \author Thomas Helfer
 * \date   07/04/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_GENERICPARALLEL_MATERIALPROPERTY_CUDABACKEND_HXX
#define LIB_MFRONT_GENERICPARALLEL_MATERIALPROPERTY_CUDABACKEND_HXX

#include <optional>
#include "TFEL/Utilities/Data.hxx"
#include "MFront/MFrontConfig.hxx"
#include "MFront/GenericParallel/MaterialProperty/CUDABackendBase.hxx"

namespace mfront::generic_parallel::material_property {

  //! \brief This class implements a backend for CUDA programming model
  struct MFRONT_VISIBILITY_EXPORT CUDABackend final : CUDABackendBase {
    // constructors
    using CUDABackendBase::CUDABackendBase;
    //

    [[nodiscard]] std::string getName() const override;
    [[nodiscard]] std::string getHeaderFileExtension() const override;
    [[nodiscard]] std::string getSourceFileExtension() const override;
    //! \brief destructor
    ~CUDABackend() noexcept override;

   protected:
    //
    [[nodiscard]] std::string getProgrammingModel() const noexcept override;
  };  // end of CUDABackend

}  // end of namespace mfront::generic_parallel::material_property

#endif /* LIB_MFRONT_GENERICPARALLEL_MATERIALPROPERTY_CUDABACKEND_HXX \
        */
