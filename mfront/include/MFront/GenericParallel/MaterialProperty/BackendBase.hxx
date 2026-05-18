/*!
 * \file MFront/GenericParallel/MaterialProperty/BackendBase.hxx
 *
 * \brief  This file declares the `BackendBase` class
 * \author Thomas Helfer
 * \date   07/04/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_GENERICPARALLEL_MATERIALPROPERTY_BACKENDBASE_HXX
#define LIB_MFRONT_GENERICPARALLEL_MATERIALPROPERTY_BACKENDBASE_HXX

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
    virtual void writePhysicalBounds(std::ostream& os,
                                     const VariableDescription& v,
                                     const size_t i,
                                     const bool useQuantities) const;
    virtual void writeBounds(std::ostream& os,
                             const std::string& iucname,
                             const VariableDescription& v,
                             const size_t i,
                             const bool useQuantities) const;
    /*!
     * \brief write the code analysying the array `mfront_bounds_statuses`
     *
     * \param[in] os: output stream
     * \param[in] i: interface
     * \param[in] mpd: material property description
     */
    virtual void writeBoundsStatusesAnalysis(
        std::ostream&,
        const GenericParallelMaterialPropertyInterface&,
        const MaterialPropertyDescription&) const;
  };

}  // end of namespace mfront::generic_parallel::material_property

#endif /* LIB_MFRONT_GENERICPARALLEL_MATERIALPROPERTY_BACKENDBASE_HXX */
