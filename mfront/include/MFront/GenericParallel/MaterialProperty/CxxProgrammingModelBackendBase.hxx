/*!
 * \file
 * MFront/GenericParallel/MaterialProperty/CxxProgrammingModelBackendBase.hxx
 *
 * \brief  This file declares the `CxxProgrammingModelBackendBase` class
 * \author Thomas Helfer
 * \date 18/05/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_GENERICPARALLEL_MATERIALPROPERTY_CXXPROGRAMMINGMODELBACKENDBASE_HXX
#define LIB_MFRONT_GENERICPARALLEL_MATERIALPROPERTY_CXXPROGRAMMINGMODELBACKENDBASE_HXX

#include "TFEL/Utilities/Data.hxx"
#include "MFront/MFrontConfig.hxx"
#include "MFront/GenericParallel/MaterialProperty/BackendBase.hxx"

namespace mfront::generic_parallel::material_property {

  /*!
   * \brief This intermediate class helps implementing various closely related
   * backends which define the kernel as a lambda function. Those backends
   * include `ParallelSTL` and `SYCL`
   */
  struct MFRONT_VISIBILITY_EXPORT CxxProgrammingModelBackendBase : BackendBase {
    //! \brief default constructor
    CxxProgrammingModelBackendBase();
    /*!
     * \brief constructor
     *
     * \param[in] opts: options
     */
    CxxProgrammingModelBackendBase(const tfel::utilities::DataMap&);
    //
    [[nodiscard]] std::string getHeaderFileExtension() const override;
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
    [[nodiscard]] std::string getSourceFileExtension() const override;
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
    ~CxxProgrammingModelBackendBase() noexcept override;

   protected:
    /*!
     * \return if the kernels use the array of statuses
     * `mfront_bounds_statuses`, i.e. if the runtime checks are enabled. This
     * array reports the violations of the bounds and the invalid results.
     *
     * \param[in] mpd: material property description
     */
    virtual bool isBoundsCheckingRequired(
        const MaterialPropertyDescription&) const;
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
     * \brief write a kernel as a lambda function inside the body of the
     * material property
     *
     * \param[in] os: output stream
     * \param[in] i: interface
     * \param[in] mpd: material property description
     * \param[in] fd: file description
     * \param[in] handleStrides: boolean stating if one shall add
     * support for strides
     */
    virtual void writeKernel(std::ostream&,
                             const GenericParallelMaterialPropertyInterface&,
                             const MaterialPropertyDescription&,
                             const FileDescription&,
                             const bool) const;
    //
    virtual void writeKernelCall(
        std::ostream&,
        const GenericParallelMaterialPropertyInterface&,
        const MaterialPropertyDescription&,
        const FileDescription&,
        const bool) const = 0;
    /*!
     * \brief write the code transferring the data to the device
     *
     * This method is only called if the `handlesDataTransfer` method returns
     * true. This method shall define `mfront_args`, an array of pointers to
     * the values of the arguments accessible by the device, and
     * `mfront_output`, a pointer to the output values accessible by the
     * device. All memory allocated on the device shall be handled by RAII.
     *
     * \param[in] os: output stream
     * \param[in] i: interface
     * \param[in] mpd: material property description
     * \param[in] treatStrides: boolean stating if one shall add
     * support for strides
     */
    virtual void writeDataTransfersToDevice(
        std::ostream&,
        const GenericParallelMaterialPropertyInterface&,
        const MaterialPropertyDescription&,
        const bool) const;
    /*!
     * \brief write the code transferring the data to the host
     *
     * \param[in] os: output stream
     * \param[in] i: interface
     * \param[in] mpd: material property description
     * \param[in] treatStrides: boolean stating if one shall add
     * support for strides
     */
    virtual void writeDataTransfersToHost(
        std::ostream&,
        const GenericParallelMaterialPropertyInterface&,
        const MaterialPropertyDescription&,
        const bool) const;
  };  // end of CxxProgrammingModelBackendBase

}  // end of namespace mfront::generic_parallel::material_property

#endif /* LIB_MFRONT_GENERICPARALLEL_MATERIALPROPERTY_CXXPROGRAMMINGMODELBACKENDBASE_HXX \
        */
