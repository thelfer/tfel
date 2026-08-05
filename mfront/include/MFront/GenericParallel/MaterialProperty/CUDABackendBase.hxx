/*!
 * \file MFront/GenericParallel/MaterialProperty/CUDABackendBase.hxx
 *
 * \brief  This file declares the `CUDABackendBase` class
 * \author Thomas Helfer
 * \date   07/04/2026
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
   * \brief This class implements a backend for the parallel STL algorithms
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
     * \brief write the global functions, if required
     *
     * \param[in] os: output stream
     * \param[in] i: interface
     * \param[in] mpd: material property description
     * \param[in] fd: file description
     * \param[in] treatUniformArguments: boolean stating if one shall add
     * support for uniform arguments
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
     * \param[in] treatUniformArguments: boolean stating if one shall add
     * support for uniform arguments
     */
    virtual void writeCImplementations2(
        std::ostream&,
        const GenericParallelMaterialPropertyInterface&,
        const MaterialPropertyDescription&,
        const FileDescription&,
        const bool) const;
    /*!
     * \brief write the call the kernel inside the body of the material property
     *
     * \param[in] os: output stream
     * \param[in] i: interface
     * \param[in] mpd: material property description
     * \param[in] treatUniformArguments: boolean stating if one shall add
     * support for uniform arguments
     */
    virtual void writeKernelCall(
        std::ostream&,
        const GenericParallelMaterialPropertyInterface&,
        const MaterialPropertyDescription&,
        const bool) const;
    //! \brief number of blocks
    std::optional<int> number_of_threads_per_block;
  };  // end of CUDABackendBase

}  // end of namespace mfront::generic_parallel::material_property

#endif /* LIB_MFRONT_GENERICPARALLEL_MATERIALPROPERTY_CUDABACKENDBASE_HXX \
        */
