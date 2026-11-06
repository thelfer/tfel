/*!
 * \file MFront/GenericParallel/MaterialProperty/CUDABackendBase.hxx
 *
 * \brief  This file declares the `CUDABackendBase` class
 * \author Thomas Helfer
 * \date 07/04/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_GENERICPARALLEL_MATERIALPROPERTY_CUDABACKENDBASE_HXX
#define LIB_MFRONT_GENERICPARALLEL_MATERIALPROPERTY_CUDABACKENDBASE_HXX

#include <optional>
#include "TFEL/Utilities/Data.hxx"
#include "MFront/MFrontConfig.hxx"
#include "MFront/GenericParallel/MaterialProperty/BackendBase.hxx"

namespace mfront::generic_parallel::material_property {

  /*!
   * \brief base class of the CUDA and HIP backends: each point is evaluated
   * by one thread of the device
   *
   * Options:
   *
   * - `number_of_threads_per_block`: number of threads per block, rounded up
   *   to a multiple of the warp size (128 by default).
   */
  struct MFRONT_VISIBILITY_EXPORT CUDABackendBase : BackendBase {
    //! \brief default constructor
    CUDABackendBase();
    /*!
     * \brief constructor
     *
     * \param[in] opts: options
     */
    CUDABackendBase(const tfel::utilities::DataMap&);
    //
    void writeSpecificIncludesInHeaderFile(
        std::ostream&,
        const GenericParallelMaterialPropertyInterface&,
        const MaterialPropertyDescription&) const override;
    void writeCxxDeclarations(
        std::ostream&,
        const GenericParallelMaterialPropertyInterface&,
        const MaterialPropertyDescription&) const override;
    void writeGlobalFunctions(std::ostream&,
                              const GenericParallelMaterialPropertyInterface&,
                              const MaterialPropertyDescription&,
                              const FileDescription&) const override;
    void writeSpecificIncludesInSourceFile(
        std::ostream&,
        const GenericParallelMaterialPropertyInterface&,
        const MaterialPropertyDescription&) const override;
    void writeCImplementations(std::ostream&,
                               const GenericParallelMaterialPropertyInterface&,
                               const MaterialPropertyDescription&,
                               const FileDescription&) const override;
    void writeCxxImplementations(
        std::ostream&,
        const GenericParallelMaterialPropertyInterface&,
        const MaterialPropertyDescription&,
        const FileDescription&) const override;
    [[nodiscard]] std::string getMemorySpace() const override;
    [[nodiscard]] std::string getExecutionPolicyType() const override;
    [[nodiscard]] std::string getExecutionPolicyHeader() const override;
    //! \brief destructor
    ~CUDABackendBase() noexcept override;

   protected:
    //! \brief the statuses of the points are merged with atomic operations
    [[nodiscard]] bool mergesPointStatusesInKernels() const override;
    [[nodiscard]] std::string getAtomicIncrement(
        const std::string_view) const override;
    [[nodiscard]] std::string getAtomicMinimum(
        const std::string_view, const std::string_view) const override;
    //! \return the programming model (`hip` or `cuda`)
    [[nodiscard]] virtual std::string getProgrammingModel() const noexcept = 0;
    /*!
     * \brief write the kernel
     *
     * \param[in] os: output stream
     * \param[in] i: interface
     * \param[in] mpd: material property description
     * \param[in] fd: file description
     * \param[in] handleStrides: if true, the kernel handles the strides of
     * the output and of the arguments. Otherwise, the name of the kernel ends
     * with `_without_strides`.
     */
    virtual void writeGlobalFunctions2(
        std::ostream&,
        const GenericParallelMaterialPropertyInterface&,
        const MaterialPropertyDescription&,
        const FileDescription&,
        const bool) const;
    /*!
     * \brief write the calls to the kernels inside the body of the material
     * property (see `writeKernelSelection`)
     *
     * \param[in] os: output stream
     * \param[in] i: interface
     * \param[in] mpd: material property description
     */
    virtual void writeKernelCall(
        std::ostream&,
        const GenericParallelMaterialPropertyInterface&,
        const MaterialPropertyDescription&) const;
    /*!
     * \brief number of threads per block, rounded up to a multiple of the
     * warp size at runtime (128 by default)
     */
    std::optional<int> number_of_threads_per_block;
  };  // end of CUDABackendBase

}  // end of namespace mfront::generic_parallel::material_property

#endif /* LIB_MFRONT_GENERICPARALLEL_MATERIALPROPERTY_CUDABACKENDBASE_HXX \
        */
