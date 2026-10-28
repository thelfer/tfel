/*!
 * \file   mfront/include/MFront/GenericParallel/BackendOptions.hxx
 * \brief  This file declares the options shared by the backends of the
 * `generic-parallel` interface for material properties.
 * \date   06/10/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_GENERICPARALLEL_BACKENDOPTIONS_HXX
#define LIB_MFRONT_GENERICPARALLEL_BACKENDOPTIONS_HXX

#include <string>
#include "TFEL/Utilities/Data.hxx"
#include "MFront/MFrontConfig.hxx"

namespace mfront::generic_parallel {

  /*!
   * \brief declare the `execution_policy` option of the backends based on
   * the parallel algorithms of the standard library
   * \param[in,out] v: validator of the options of a backend
   */
  MFRONT_VISIBILITY_EXPORT void declareExecutionPolicyOption(
      tfel::utilities::DataMapValidator&);
  /*!
   * \return the execution policy given by the `execution_policy` option, as
   * a C++ expression (`std::execution::par_unseq` by default)
   * \param[in] opts: options of a backend
   */
  MFRONT_VISIBILITY_EXPORT std::string getExecutionPolicy(
      const tfel::utilities::DataMap&);
  /*!
   * \brief declare the `device_stack_size` option, which gives the minimal
   * stack size, in bytes, of the threads of the device
   * \param[in,out] v: validator of the options of a backend
   */
  MFRONT_VISIBILITY_EXPORT void declareDeviceStackSizeOption(
      tfel::utilities::DataMapValidator&);
  /*!
   * \return the value of the `device_stack_size` option (null if the option
   * is not given)
   * \param[in] opts: options of a backend
   */
  MFRONT_VISIBILITY_EXPORT int getDeviceStackSize(
      const tfel::utilities::DataMap&);
  /*!
   * \brief declare the `data_transfer_policy` option of the SYCL backends
   * \param[in,out] v: validator of the options of a backend
   */
  MFRONT_VISIBILITY_EXPORT void declareDataTransferPolicyOption(
      tfel::utilities::DataMapValidator&);
  /*!
   * \return true if the `data_transfer_policy` option is `automatic` (or
   * `auto`, default), false if it is `none`
   * \param[in] opts: options of a backend
   */
  MFRONT_VISIBILITY_EXPORT bool areDataTransfersAutomatic(
      const tfel::utilities::DataMap&);

}  // end of namespace mfront::generic_parallel

#endif /* LIB_MFRONT_GENERICPARALLEL_BACKENDOPTIONS_HXX */
