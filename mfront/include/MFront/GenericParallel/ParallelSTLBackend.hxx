/*!
 * \file mfront/include/MFront/GenericParallel/ParallelSTLBackend.hxx
 * \brief  This file declares the backend of the `generic-parallel` interfaces
 * based on the parallel algorithms of the C++ standard library
 * \author Thomas Helfer
 * \date   07/04/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_GENERICPARALLEL_PARALLELSTLBACKEND_HXX
#define LIB_MFRONT_GENERICPARALLEL_PARALLELSTLBACKEND_HXX

#include "TFEL/Utilities/Data.hxx"
#include "MFront/MFrontConfig.hxx"
#include "MFront/GenericParallel/AbstractBackend.hxx"

namespace mfront::generic_parallel {

  /*!
   * \brief backend based on the parallel algorithms of the C++ standard
   * library.
   *
   * Options:
   *
   * - `execution_policy`: `sequenced_policy` (or `seq`),
   *   `unsequenced_policy` (or `unseq`), `parallel_policy` (or `par`) or
   *   `parallel_unsequenced_policy` (or `par_unseq`, default).
   * - `device_stack_size`: minimal stack size, in bytes, of the threads of the
   *   device when the parallel algorithms are offloaded to a GPU by
   *   `nvc++ -stdpar=gpu` (ignored otherwise). The default stack size of
   *   CUDA threads (1024 bytes) is too small for some kernels.
   */
  struct MFRONT_VISIBILITY_EXPORT ParallelSTLBackend final : AbstractBackend {
    //! \brief default constructor
    ParallelSTLBackend();
    /*!
     * \brief constructor
     * \param[in] opts: options
     */
    ParallelSTLBackend(const tfel::utilities::DataMap&);
    //
    [[nodiscard]] std::string getName() const override;
    [[nodiscard]] std::string getSourceFileExtension() const override;
    void writeSpecificIncludes(std::ostream&) const override;
    [[nodiscard]] std::string getApply() const override;
    [[nodiscard]] std::string getMemorySpace() const override;
    [[nodiscard]] std::string getExecutionPolicyType() const override;
    //! \brief destructor
    ~ParallelSTLBackend() noexcept override;

   private:
    //! \brief execution policy
    std::string execution_policy = "std::execution::par_unseq";
    //! \brief stack size of the threads of the device (0 if not specified)
    int device_stack_size = 0;
  };  // end of ParallelSTLBackend

}  // end of namespace mfront::generic_parallel

#endif /* LIB_MFRONT_GENERICPARALLEL_PARALLELSTLBACKEND_HXX */
