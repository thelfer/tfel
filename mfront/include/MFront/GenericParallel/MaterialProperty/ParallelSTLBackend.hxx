/*!
 * \file
 * MFront/GenericParallel/MaterialProperty/AbstractBackend.hxx
 *
 * \brief  This file declares the `ParallelSTLBackend` class
 * \author Thomas Helfer
 * \date   07/04/2026
 */

#ifndef LIB_MFRONT_GENERICPARALLEL_MATERIALPROPERTY_PARALLELSTLBACKEND_HXX
#define LIB_MFRONT_GENERICPARALLEL_MATERIALPROPERTY_PARALLELSTLBACKEND_HXX

#include "MFront/GenericParallel/MaterialProperty/BackendBase.hxx"

namespace mfront::generic_parallel::material_property {

  /*!
   * \brief This class implements a backend for the parallel STL algorithms
   */
  struct ParallelSTLBackend : BackendBase {
    //! \brief default constructor
    ParallelSTLBackend();
    //
    void writeSpecificIncludesInHeaderFile(
        std::ostream&,
        const GenericParallelMaterialPropertyInterface&,
        const MaterialPropertyDescription&) const override;
    void writeCDeclaration(std::ostream&,
                           const GenericParallelMaterialPropertyInterface&,
                           const MaterialPropertyDescription&) const override;
    void writeCxxDeclaration(std::ostream&,
                             const GenericParallelMaterialPropertyInterface&,
                             const MaterialPropertyDescription&) const override;
    void writeGlobalFunction(std::ostream&,
                             const GenericParallelMaterialPropertyInterface&,
                             const MaterialPropertyDescription&) const override;
    void writeSpecificIncludesInSourceFile(
        std::ostream&,
        const GenericParallelMaterialPropertyInterface&,
        const MaterialPropertyDescription&) const override;
    void writeCImplementation(std::ostream&,
                              const GenericParallelMaterialPropertyInterface&,
                              const MaterialPropertyDescription&,
                              const FileDescription&) const override;
    void writeCxxImplementation(std::ostream&,
                                const GenericParallelMaterialPropertyInterface&,
                                const MaterialPropertyDescription&,
                                const FileDescription&) const override;
    //! \brief destructor
    ~ParallelSTLBackend() noexcept override;

   protected:
    /*!
     * \brief write the call the kernel inside the body of the material property
     *
     * \param[in] os: output stream
     * \param[in] i: interface
     * \param[in] mpd: material property description
     */
    virtual void writeKernelCall(
        std::ostream&,
        const GenericParallelMaterialPropertyInterface&,
        const MaterialPropertyDescription&) const;
  };  // end of ParallelSTLBackend

}  // end of namespace mfront::generic_parallel::material_property

#endif /* LIB_MFRONT_GENERICPARALLEL_MATERIALPROPERTY_PARALLELSTLBACKEND_HXX \
        */
