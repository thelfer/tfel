/*!
 * \file
 * MFront/GenericParallel/MaterialProperty/AbstractBackend.hxx
 *
 * \brief  This file declares the `AbstractBackend` class
 * \author Thomas Helfer
 * \date   07/04/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_GENERICPARALLEL_MATERIALPROPERTY_ABSTRACTBACKEND_HXX
#define LIB_MFRONT_GENERICPARALLEL_MATERIALPROPERTY_ABSTRACTBACKEND_HXX

#include <iosfwd>
#include "MFront/MFrontConfig.hxx"

namespace mfront {

  // forward declarations
  struct FileDescription;
  struct MaterialPropertyDescription;
  struct GenericParallelMaterialPropertyInterface;

}  // end of namespace mfront

namespace mfront::generic_parallel::material_property {

  /*!
   * \brief an abstract class which aims at writing code for a specific backend
   * (CUDA, SYCL, parallel STL)
   */
  struct MFRONT_VISIBILITY_EXPORT AbstractBackend {
    //! \brief return the name of the backend
    [[nodiscard]] virtual std::string getName() const = 0;
    //! \brief return the extension of the header file
    [[nodiscard]] virtual std::string getHeaderFileExtension() const = 0;
    /*!
     * \brief write additional headers in the header file
     *
     * \param[in] os: output stream
     * \param[in] i: interface
     * \param[in] mpd: material property description
     */
    virtual void writeSpecificIncludesInHeaderFile(
        std::ostream&,
        const GenericParallelMaterialPropertyInterface&,
        const MaterialPropertyDescription&) const = 0;
    /*!
     * \brief write the C declarations of the material property
     *
     * \param[in] os: output stream
     * \param[in] i: interface
     * \param[in] mpd: material property description
     */
    virtual void writeCDeclarations(
        std::ostream&,
        const GenericParallelMaterialPropertyInterface&,
        const MaterialPropertyDescription&) const = 0;
    /*!
     * \brief write the C++ declarations of the material property
     *
     * \param[in] os: output stream
     * \param[in] i: interface
     * \param[in] mpd: material property description
     */
    virtual void writeCxxDeclarations(
        std::ostream&,
        const GenericParallelMaterialPropertyInterface&,
        const MaterialPropertyDescription&) const = 0;
    //! \brief return the extension of the source file
    [[nodiscard]] virtual std::string getSourceFileExtension() const = 0;
    /*!
     * \brief write additional headers in the source file
     *
     * \param[in] os: output stream
     * \param[in] i: interface
     * \param[in] mpd: material property description
     */
    virtual void writeSpecificIncludesInSourceFile(
        std::ostream&,
        const GenericParallelMaterialPropertyInterface&,
        const MaterialPropertyDescription&) const = 0;
    /*!
     * \brief write the global functions, if required
     *
     * \param[in] os: output stream
     * \param[in] i: interface
     * \param[in] mpd: material property description
     * \param[in] fd: file description
     */
    virtual void writeGlobalFunctions(
        std::ostream&,
        const GenericParallelMaterialPropertyInterface&,
        const MaterialPropertyDescription&,
        const FileDescription&) const = 0;
    /*!
     * \brief write the C++ implementations of the material property
     *
     * \param[in] os: output stream
     * \param[in] i: interface
     * \param[in] mpd: material property description
     * \param[in] fd: file description
     */
    virtual void writeCxxImplementations(
        std::ostream&,
        const GenericParallelMaterialPropertyInterface&,
        const MaterialPropertyDescription&,
        const FileDescription&) const = 0;
    /*!
     * \brief write the C implementations of the material property
     *
     * \param[in] os: output stream
     * \param[in] i: interface
     * \param[in] mpd: material property description
     * \param[in] fd: file description
     */
    virtual void writeCImplementations(
        std::ostream&,
        const GenericParallelMaterialPropertyInterface&,
        const MaterialPropertyDescription&,
        const FileDescription&) const = 0;
    /*!
     * \brief write a proxy object to declare the function in a virtual factory,
     * if required
     *
     * \param[in] os: output stream
     * \param[in] i: interface
     * \param[in] mpd: material property description
     */
    virtual void writeProxyObjects(
        std::ostream&,
        const GenericParallelMaterialPropertyInterface&,
        const MaterialPropertyDescription&) const = 0;
    //! \brief destructor
    virtual ~AbstractBackend() noexcept;
  };  // end of AbstractBackend

}  // end of namespace mfront::generic_parallel::material_property

#endif /* LIB_MFRONT_GENERICPARALLEL_MATERIALPROPERTY_ABSTRACTBACKEND_HXX */
