/*!
 * \file MFront/GenericParallel/MaterialProperty/ParallelSTLBackend.hxx
 *
 * \brief  This file declares the `ParallelSTLBackend` class
 * \author Thomas Helfer
 * \date   07/04/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_GENERICPARALLEL_MATERIALPROPERTY_PARALLELSTLBACKEND_HXX
#define LIB_MFRONT_GENERICPARALLEL_MATERIALPROPERTY_PARALLELSTLBACKEND_HXX

#include "TFEL/Utilities/Data.hxx"
#include "MFront/MFrontConfig.hxx"
#include "MFront/GenericParallel/MaterialProperty/CxxProgrammingModelBackendBase.hxx"

namespace mfront::generic_parallel::material_property {

  /*!
   * \brief This class implements a backend for the parallel STL algorithms
   */
  struct MFRONT_VISIBILITY_EXPORT ParallelSTLBackend
      : CxxProgrammingModelBackendBase {
    //! \brief default constructor
    ParallelSTLBackend();
    /*!
     * \brief constructor
     *
     * \param[in] opts: options
     */
    ParallelSTLBackend(const tfel::utilities::DataMap&);
    //
    [[nodiscard]] std::string getName() const override;
    void writeSpecificIncludesInSourceFile(
        std::ostream&,
        const GenericParallelMaterialPropertyInterface&,
        const MaterialPropertyDescription&) const override;
    //! \brief destructor
    ~ParallelSTLBackend() noexcept override;

   protected:
    //
    void writeKernelCall(std::ostream&,
                         const GenericParallelMaterialPropertyInterface&,
                         const MaterialPropertyDescription&,
                         const FileDescription&,
                         const bool) const override;
    //! \brief execution policy
    std::string execution_policy;
  };  // end of ParallelSTLBackend

}  // end of namespace mfront::generic_parallel::material_property

#endif /* LIB_MFRONT_GENERICPARALLEL_MATERIALPROPERTY_PARALLELSTLBACKEND_HXX \
        */
