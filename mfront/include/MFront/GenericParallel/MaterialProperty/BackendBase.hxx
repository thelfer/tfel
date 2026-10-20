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
    // do nothing implementation
    void writeProxyObjects(std::ostream&,
                           const GenericParallelMaterialPropertyInterface&,
                           const MaterialPropertyDescription&) const override;
    //! \brief destructor
    ~BackendBase() noexcept override;

   protected:
    /*!
     * \brief structure describing the extra arguments of the C functions
     */
    struct ExtraArgumentOfCFunctions {
      //! \brief type of the argument
      std::string type;
      //! \brief name of the argument. Shall start with `mfront_`
      std::string name;
      //! \brief description of the argument.
      std::string description;
      //! \brief boolean stating if the argument is passed as a pointer
      bool is_pointer;
      //! \brief boolean stating if the argument is mutable
      bool is_mutable;
    };
    //! \return the list of extra argument to the C functions
    [[nodiscard]] virtual std::vector<ExtraArgumentOfCFunctions>
    getExtraArgumentsOfCFunctions() const;
    //
    virtual void writePhysicalBounds(std::ostream&,
                                     const VariableDescription&,
                                     const size_t,
                                     const bool) const;
    virtual void writeBounds(std::ostream&,
                             const std::string&,
                             const VariableDescription&,
                             const size_t,
                             const bool) const;
    /*!
     * \brief write the code analysing the array `mfront_bounds_statuses`
     *
     * \param[in] os: output stream
     * \param[in] i: interface
     * \param[in] mpd: material property description
     */
    virtual void writeBoundsStatusesAnalysis(
        std::ostream&,
        const GenericParallelMaterialPropertyInterface&,
        const MaterialPropertyDescription&) const;
    /*!
     * \return the size of the array `mfront_bounds_statuses`
     *
     * This array contains the statuses of the physical bounds and of the
     * standard bounds of the inputs and of the output, followed by a status
     * reporting that the result is not finite at one point at least.
     *
     * \param[in] mpd: material property description
     */
    static std::size_t getStatusesArraySize(const MaterialPropertyDescription&);
    /*!
     * \brief write the code reporting, in the array
     * `mfront_bounds_statuses`, that the result is not finite
     *
     * \param[in] os: output stream
     * \param[in] mpd: material property description
     */
    void writeInvalidResultCheck(std::ostream&,
                                 const MaterialPropertyDescription&) const;
    /*!
     * \brief return if the backend shall handle the data transfer from a host
     * to a device
     *
     * If this method returns true, the argument of the C function
     * implementations containing pointers to values of the arguments
     * is named `mfront_args_host`. The backend shall
     * then define a pointer named `mfront_args` to the beginning of an
     * internal array of pointers. The output argument is then named
     * `mfront_output_host`, and the backend shall define a pointer named
     * `mfront_output`.
     *
     * If this method returns false, those arguments are directly named
     * `mfront_args` and `mfront_output`.
     *
     * \note by default, no data transfer is handled
     */
    virtual bool handlesDataTransfer() const;
  };

}  // end of namespace mfront::generic_parallel::material_property

#endif /* LIB_MFRONT_GENERICPARALLEL_MATERIALPROPERTY_BACKENDBASE_HXX */
