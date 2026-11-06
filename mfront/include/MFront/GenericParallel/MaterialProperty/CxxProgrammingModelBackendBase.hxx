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
    /*!
     * \brief write the declarations of the variables captured by the kernels
     * (copies of the pointers to the arguments, named `mfront_arg0`,
     * `mfront_arg1`, etc., and values of the parameters) and the kernels
     * (without strides and with strides).
     *
     * If bounds checking is required and if the backend merges the statuses
     * of the points in the kernels, the statistics `mfront_statistics` must
     * have been declared before by the backend (see
     * `writeStatisticsDeclaration`).
     *
     * \param[in] os: output stream
     * \param[in] i: interface
     * \param[in] mpd: material property description
     * \param[in] fd: file description
     */
    void writeKernels(std::ostream&,
                      const GenericParallelMaterialPropertyInterface&,
                      const MaterialPropertyDescription&,
                      const FileDescription&) const;
    /*!
     * \brief write the declaration of the kernels (see `writeKernels`) and
     * the evaluation of the batch of points
     *
     * \param[in] os: output stream
     * \param[in] i: interface
     * \param[in] mpd: material property description
     * \param[in] fd: file description
     */
    virtual void writeKernelCall(
        std::ostream&,
        const GenericParallelMaterialPropertyInterface&,
        const MaterialPropertyDescription&,
        const FileDescription&) const = 0;
    /*!
     * \return the specifiers written between the captures and the parameters
     * of the lambda functions implementing the kernels (for instance, the
     * host and device annotations of the extended lambdas of `nvcc`)
     */
    [[nodiscard]] virtual std::string getKernelSpecifiers() const;
    /*!
     * \brief write the code preparing the execution, before the data
     * transfers and the kernels: queue of the execution policy
     * (`mfront_backend_policy`), etc. Nothing is written by default.
     *
     * \param[in] os: output stream
     * \param[in] i: interface
     * \param[in] mpd: material property description
     */
    virtual void writeExecutionPreparation(
        std::ostream&,
        const GenericParallelMaterialPropertyInterface&,
        const MaterialPropertyDescription&) const;
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
     */
    virtual void writeDataTransfersToDevice(
        std::ostream&,
        const GenericParallelMaterialPropertyInterface&,
        const MaterialPropertyDescription&) const;
    /*!
     * \brief write the code transferring the data to the host
     *
     * \param[in] os: output stream
     * \param[in] i: interface
     * \param[in] mpd: material property description
     */
    virtual void writeDataTransfersToHost(
        std::ostream&,
        const GenericParallelMaterialPropertyInterface&,
        const MaterialPropertyDescription&) const;
  };  // end of CxxProgrammingModelBackendBase

}  // end of namespace mfront::generic_parallel::material_property

#endif /* LIB_MFRONT_GENERICPARALLEL_MATERIALPROPERTY_CXXPROGRAMMINGMODELBACKENDBASE_HXX \
        */
