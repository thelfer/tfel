/*!
 * \file   mfront/include/MFront/GenericParallel/AbstractBackend.hxx
 * \brief  This file declares the base class of the backends of the
 * `generic-parallel` interfaces
 * \author Thomas Helfer
 * \date   07/04/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_GENERICPARALLEL_ABSTRACTBACKEND_HXX
#define LIB_MFRONT_GENERICPARALLEL_ABSTRACTBACKEND_HXX

#include <iosfwd>
#include <string>
#include <string_view>
#include "MFront/MFrontConfig.hxx"

namespace mfront::generic_parallel {

  /*!
   * \brief base class of the backends of the `generic-parallel` interfaces.
   *
   * The computations on a batch are implemented by function templates of
   * the runtime of an interface (`MFront/GenericParallelMaterialProperty`,
   * namespace `mfront::gpmp`, for the material properties), which take the
   * primitive of a backend as last argument (see
   * `MFront/GenericParallel/Batch.hxx`). A backend selects its headers and
   * writes the expression building its primitive.
   */
  struct MFRONT_VISIBILITY_EXPORT AbstractBackend {
    /*!
     * \return the name of the backend, which is used as a suffix for the
     * names of the generated library and files
     */
    [[nodiscard]] virtual std::string getName() const = 0;
    //! \return the extension of the generated source file
    [[nodiscard]] virtual std::string getSourceFileExtension() const = 0;
    /*!
     * \brief write the headers required by the backend: the header of its
     * primitive (`MFront/GenericParallel/<name>.hxx`) and, if the backend has
     * functions specific to the runtime of the interface, their header
     * \param[out] os: output stream
     */
    virtual void writeSpecificIncludes(std::ostream&) const = 0;
    /*!
     * \brief write the symbols describing the generated library: name of
     * the backend (`<f>_generic_parallel_backend`), memory space in which
     * the fields must be located (`<f>_generic_parallel_memory_space`),
     * identifier of the execution policy
     * (`<f>_generic_parallel_execution_policy`), and version of the data
     * structures exchanged with the generated functions
     * (`<f>_generic_parallel_api_version`, see `MFRONT_GPMP_API_VERSION`)
     * \param[out] os: output stream
     * \param[in] f: basis of the names of the symbols
     */
    void writeSymbols(std::ostream&, std::string_view) const;
    /*!
     * \return an expression building the primitive of the backend (see the
     * `makeApply` functions of the `MFront/GenericParallel` headers)
     */
    [[nodiscard]] virtual std::string getApply() const = 0;
    /*!
     * \return an expression, evaluated when the generated library is
     * compiled, giving the memory space in which the fields must be located
     * (see `MFront/GenericParallel/MemorySpace.hxx`): `host`, `device` or
     * `any`
     */
    [[nodiscard]] virtual std::string getMemorySpace() const = 0;
    /*!
     * \return the type of the execution policy of the backend (see
     * `MFront/GenericParallel/AbstractBackendExecutionPolicy.hxx`), checked
     * by the generated functions. Its header is included by the header of
     * the primitive of the backend (see `writeSpecificIncludes`).
     */
    [[nodiscard]] virtual std::string getExecutionPolicyType() const = 0;
    /*!
     * \brief write the statement returning the result of a function of the
     * runtime of the interface, for instance `evaluateBatch` of
     * `MFront/GenericParallelMaterialProperty/Evaluate.hxx`. The primitive
     * of the backend is appended to the given arguments.
     *
     * \param[out] os: output stream
     * \param[in] f: name of the function
     * \param[in] status: name of the output status, which is the first
     * argument
     * \param[in] arguments: other arguments
     */
    virtual void writeCall(std::ostream&,
                           std::string_view,
                           std::string_view,
                           std::string_view) const;
    //! \brief destructor
    virtual ~AbstractBackend() noexcept;
  };  // end of AbstractBackend

}  // end of namespace mfront::generic_parallel

#endif /* LIB_MFRONT_GENERICPARALLEL_ABSTRACTBACKEND_HXX */
