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
    //! \brief destructor
    ~CUDABackendBase() noexcept override;

   protected:
    //! \return the programming model (`hip` or `cuda`)
    [[nodiscard]] virtual std::string getProgrammingModel() const noexcept = 0;
    /*!
     * \brief write the kernel
     *
     * \param[in] os: output stream
     * \param[in] i: interface
     * \param[in] mpd: material property description
     * \param[in] fd: file description
     * \param[in] treatStrides: if true, the kernel handles the strides of the
     * output and of the arguments. Otherwise, the name of the kernel ends
     * with `_without_strides`.
     */
    virtual void writeGlobalFunctions2(
        std::ostream&,
        const GenericParallelMaterialPropertyInterface&,
        const MaterialPropertyDescription&,
        const FileDescription&,
        const bool) const;
    /*!
     * \brief write the C implementations of the material property
     *
     * \param[in] os: output stream
     * \param[in] i: interface
     * \param[in] mpd: material property description
     * \param[in] fd: file description
     * \param[in] treatStrides: if true, the function handles the strides of
     * the output and of the arguments. Otherwise, the arguments and the
     * output are contiguous arrays and the name of the function ends with
     * `2`.
     */
    virtual void writeCImplementations2(
        std::ostream&,
        const GenericParallelMaterialPropertyInterface&,
        const MaterialPropertyDescription&,
        const FileDescription&,
        const bool) const;
    /*!
     * \brief write the call to the kernel inside the body of the material
     * property
     *
     * \param[in] os: output stream
     * \param[in] i: interface
     * \param[in] mpd: material property description
     * \param[in] treatStrides: boolean stating if strides are treated (see
     * `writeCImplementations2`)
     */
    virtual void writeKernelCall(
        std::ostream&,
        const GenericParallelMaterialPropertyInterface&,
        const MaterialPropertyDescription&,
        const bool) const;
    //! \brief number of threads per block
    std::optional<int> number_of_threads_per_block;
  };  // end of CUDABackendBase

}  // end of namespace mfront::generic_parallel::material_property

#endif /* LIB_MFRONT_GENERICPARALLEL_MATERIALPROPERTY_CUDABACKENDBASE_HXX \
        */
