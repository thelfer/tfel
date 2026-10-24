/*!
 * \file MFront/GenericParallel/MaterialProperty/BackendBase.hxx
 *
 * \brief  This file declares the `BackendBase` class
 * \author Thomas Helfer
 * \date 07/04/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_GENERICPARALLEL_MATERIALPROPERTY_BACKENDBASE_HXX
#define LIB_MFRONT_GENERICPARALLEL_MATERIALPROPERTY_BACKENDBASE_HXX

#include <string>
#include <vector>
#include <functional>
#include <string_view>
#include "MFront/MFrontConfig.hxx"
#include "MFront/GenericParallel/MaterialProperty/AbstractBackend.hxx"

namespace mfront {

  // forward declaration
  struct VariableDescription;

}  // end of namespace mfront

namespace mfront::generic_parallel::material_property {

  //! \brief this class provides useful methods for most backends
  struct MFRONT_VISIBILITY_EXPORT BackendBase : AbstractBackend {
    //! \brief default constructor
    BackendBase();
    //
    void writeCDeclarations(std::ostream&,
                            const GenericParallelMaterialPropertyInterface&,
                            const MaterialPropertyDescription&) const override;
    void writeReportFunction(std::ostream&,
                             const GenericParallelMaterialPropertyInterface&,
                             const MaterialPropertyDescription&) const override;
    // do nothing implementation
    void writeProxyObjects(std::ostream&,
                           const GenericParallelMaterialPropertyInterface&,
                           const MaterialPropertyDescription&) const override;
    //! \brief destructor
    ~BackendBase() noexcept override;

   protected:
    /*!
     * \return true if the kernels report the violations of the bounds and
     * the invalid results in the statistics of the evaluation (runtime
     * checks enabled)
     * \param[in] mpd: material property description
     */
    [[nodiscard]] static bool isBoundsCheckingRequired(
        const MaterialPropertyDescription&);
    /*!
     * \brief write the beginning of the C function computing the material
     * property on a batch of points (`<name>_generic_parallel`): signature,
     * error reporting (`mfront_report`), initialization of the output
     * status and checks of the batch data. The function returns if there is
     * no point. The body of the function is enclosed in a `try` block,
     * closed by `writeCFunctionEnd`.
     *
     * The data of the batch are copied in local variables:
     * `mfront_npoints`, `mfront_output` (or `mfront_output_host`, see
     * `handlesDataTransfer`), `mfront_output_stride`, `mfront_args` (or
     * `mfront_args_host`), an array of the pointers to the values of the
     * arguments, `mfront_strides`, the array of their strides,
     * `mfront_out_of_bounds_policy` and `mfront_statuses`. The statistics
     * given by the calling code, if any, are `mfront_given_statistics`, and
     * `mfront_deferred_report` states if they are given (the output status
     * then only reports the errors of the batch). The execution policy given
     * by the calling code is checked against the policy of the backend (see
     * `getExecutionPolicyType`): `mfront_backend_policy` holds the checked
     * policy (null for the default policy), `mfront_asynchronous` and
     * `mfront_preserve_stack_size` its common options.
     *
     * \param[in] os: output stream
     * \param[in] i: interface
     * \param[in] mpd: material property description
     */
    void writeCFunctionBeginning(
        std::ostream&,
        const GenericParallelMaterialPropertyInterface&,
        const MaterialPropertyDescription&) const;
    /*!
     * \brief write the end of the C function computing the material
     * property on a batch of points: the status is returned, and the `try`
     * block opened by `writeCFunctionBeginning` is closed (an unexpected
     * exception gives the status `-7`)
     * \param[in] os: output stream
     * \param[in] i: interface
     * \param[in] mpd: material property description
     */
    static void writeCFunctionEnd(
        std::ostream&,
        const GenericParallelMaterialPropertyInterface&,
        const MaterialPropertyDescription&);
    /*!
     * \brief write the end of the `try` block of the C function: the
     * exceptions thrown on the host (for instance by the runtime of the
     * backend) must not cross the boundary of the C function and are
     * reported as runtime errors (status `-7`)
     * \param[in] os: output stream
     */
    static void writeHostExceptionsHandling(std::ostream&);
    /*!
     * \brief write the code reporting an error of the batch in the output
     * status and returning the status
     * \param[in] os: output stream
     * \param[in] status: status (`-5`, `-6` or `-7`)
     * \param[in] message: expression giving the error message
     */
    static void writeBatchError(std::ostream&,
                                const int,
                                const std::string_view);
    /*!
     * \brief write the body of a kernel, which evaluates the material
     * property at the point `mfront_idx`. The kernel reads the arguments in
     * `mfront_arg0`, `mfront_arg1`, etc. (with the strides `mfront_strides`
     * if strides are handled) and writes the result in `mfront_output`
     * (with the stride `mfront_output_stride` if strides are handled). The
     * parameters are read in `mfront_parameters`, unless they are treated
     * as static variables.
     *
     * If the runtime checks are enabled, the evaluation at the point is
     * written in a lambda function returning the status of the point
     * (`mfront::gpmp::PointStatus`), stored in `mfront_point_status`. This
     * status is then merged in the statistics `mfront_statistics` with
     * integer atomic operations, only if an event occurred (see
     * `mergesPointStatusesInKernels`), or returned by the kernel, to be
     * merged by a reduction.
     *
     * \param[in] os: output stream
     * \param[in] i: interface
     * \param[in] mpd: material property description
     * \param[in] handleStrides: if true, the arguments and the output are
     * strided
     */
    void writeKernelBody(std::ostream&,
                         const GenericParallelMaterialPropertyInterface&,
                         const MaterialPropertyDescription&,
                         const bool) const;
    /*!
     * \brief write the selection of the kernel called at each point.
     *
     * A batch of one point is evaluated by the kernel without strides, as
     * a batch whose strides are all equal to one. The kernel handling
     * strides is used otherwise.
     *
     * \param[in] os: output stream
     * \param[in] i: interface
     * \param[in] mpd: material property description
     * \param[in] write_call: function writing the evaluation of the given
     * number of points (`1` or `mfront_npoints`) by the kernel handling
     * strides if its second argument is true, or by the kernel without
     * strides otherwise
     */
    void writeKernelSelection(
        std::ostream&,
        const GenericParallelMaterialPropertyInterface&,
        const MaterialPropertyDescription&,
        const std::function<void(std::string_view, bool)>&) const;
    /*!
     * \brief write the analysis of the results, once the kernels are
     * completed (see `writeReportFunction`), if the runtime checks are
     * enabled and if the evaluation is synchronous
     *
     * \param[in] os: output stream
     * \param[in] i: interface
     * \param[in] mpd: material property description
     */
    void writeResultsAnalysis(std::ostream&,
                              const GenericParallelMaterialPropertyInterface&,
                              const MaterialPropertyDescription&) const;
    /*!
     * \return true if the kernels merge the status of each point in the
     * statistics `mfront_statistics` with integer atomic operations, only if
     * an event occurred. Otherwise, the kernels return the status of each
     * point (`mfront::gpmp::PointStatus`), and the backend merges them with
     * a reduction (see `mfront::gpmp::makeStatistics`).
     */
    [[nodiscard]] virtual bool mergesPointStatusesInKernels() const;
    /*!
     * \return the code atomically incrementing a counter of the statistics
     * (`unsigned long long`), for the backends merging the statuses in the
     * kernels
     * \param[in] c: counter
     */
    [[nodiscard]] virtual std::string getAtomicIncrement(
        const std::string_view) const;
    /*!
     * \return the code atomically replacing a counter of the statistics
     * (`unsigned long long`) by the minimum of its value and of a given
     * value, for the backends merging the statuses in the kernels
     * \param[in] c: counter
     * \param[in] v: value
     */
    [[nodiscard]] virtual std::string getAtomicMinimum(
        const std::string_view, const std::string_view) const;
    /*!
     * \brief write the declaration of the statistics `mfront_statistics`
     * updated by the kernels which merge the statuses of the points with
     * atomic operations.
     *
     * If the calling code gives statistics (deferred report, required by an
     * asynchronous evaluation), they are initialized by the calling code.
     * Otherwise, they
     * are held by a block of a pool of blocks accessible by the host and by
     * the device (see `mfront::generic_parallel::StatusBlockPool`),
     * initialized on the host: no memory is allocated per call. The device
     * only accesses the statistics if an event occurs at a point.
     *
     * \param[in] os: output stream
     * \param[in] device_type: type identifying a device
     * \param[in] device: expression identifying the current device
     * \param[in] allocation: body of a function allocating a
     * `mfront_gpmp_Statistics` structure, returning a null pointer on
     * failure
     */
    static void writeStatisticsDeclaration(std::ostream&,
                                           const std::string_view,
                                           const std::string_view,
                                           const std::string_view);
    /*!
     * \brief write the declaration of the statistics `mfront_statistics`
     * of the backends merging the statuses of the points with a reduction:
     * the statistics given by the calling code, or local statistics
     * initialized on the host
     * \param[in] os: output stream
     */
    static void writeLocalStatisticsDeclaration(std::ostream&);
    /*!
     * \brief write the code setting the minimal stack size of the threads
     * of the device (see `MFront/GenericParallel/DeviceStackSize.hxx`), or
     * checking it if the execution policy requires to preserve it
     * \param[in] os: output stream
     * \param[in] stack_size: minimal stack size, in bytes. Nothing is
     * written if null
     */
    static void writeDeviceStackSizeSetting(std::ostream&, const std::size_t);
    /*!
     * \brief write the declaration of the `mfront_report` function, writing
     * an error message in the output status `mfront_output_status`
     * \param[in] os: output stream
     */
    static void writeErrorReporting(std::ostream&);
    /*!
     * \brief return if the backend shall handle the data transfer from a host
     * to a device
     *
     * If this method returns true, the array of the pointers to the values
     * of the arguments given by the calling code is named
     * `mfront_args_host`. The backend shall then define an array of pointers
     * named `mfront_args`. The pointer to the output values given by the
     * calling code is then named `mfront_output_host`, and the backend shall
     * define a pointer named `mfront_output`.
     *
     * If this method returns false, those variables are directly named
     * `mfront_args` and `mfront_output`.
     *
     * \note by default, no data transfer is handled
     */
    virtual bool handlesDataTransfer() const;
  };

}  // end of namespace mfront::generic_parallel::material_property

#endif /* LIB_MFRONT_GENERICPARALLEL_MATERIALPROPERTY_BACKENDBASE_HXX */
