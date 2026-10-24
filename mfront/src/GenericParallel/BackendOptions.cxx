/*!
 * \file   mfront/src/GenericParallel/BackendOptions.cxx
 * \brief  This file implements the options shared by the backends of the
 * `generic-parallel` interface for material properties.
 * \date   06/10/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "TFEL/Raise.hxx"
#include "MFront/GenericParallel/BackendOptions.hxx"

namespace mfront::generic_parallel {

  void declareExecutionPolicyOption(tfel::utilities::DataMapValidator& v) {
    v.addDataTypeValidator<std::string>("execution_policy");
  }  // end of declareExecutionPolicyOption

  std::string getExecutionPolicy(const tfel::utilities::DataMap& opts) {
    if (!contains(opts, "execution_policy")) {
      return "std::execution::par_unseq";
    }
    const auto p = get<std::string>(opts, "execution_policy");
    if ((p == "seq") || (p == "sequenced_policy")) {
      return "std::execution::seq";
    } else if ((p == "unseq") || (p == "unsequenced_policy")) {
      return "std::execution::unseq";
    } else if ((p == "par") || (p == "parallel_policy")) {
      return "std::execution::par";
    } else if ((p != "par_unseq") && (p != "parallel_unsequenced_policy")) {
      tfel::raise(
          "getExecutionPolicy: unsupported execution policy '" + p +
          "'. Valid execution policies are: 'sequenced_policy' (or "
          "'seq'), 'unsequenced_policy' (or 'unseq'), 'parallel_policy' (or "
          "'par') and 'parallel_unsequenced_policy' (or 'par_unseq')");
    }
    return "std::execution::par_unseq";
  }  // end of getExecutionPolicy

  void declareDeviceStackSizeOption(tfel::utilities::DataMapValidator& v) {
    v.addStrictlyPositiveIntegerCheck("device_stack_size");
  }  // end of declareDeviceStackSizeOption

  int getDeviceStackSize(const tfel::utilities::DataMap& opts) {
    if (!contains(opts, "device_stack_size")) {
      return 0;
    }
    return get<int>(opts, "device_stack_size");
  }  // end of getDeviceStackSize

  void declareDataTransferPolicyOption(tfel::utilities::DataMapValidator& v) {
    v.addDataTypeValidator<std::string>("data_transfer_policy");
  }  // end of declareDataTransferPolicyOption

  bool areDataTransfersAutomatic(const tfel::utilities::DataMap& opts) {
    if (!contains(opts, "data_transfer_policy")) {
      return true;
    }
    const auto p = get<std::string>(opts, "data_transfer_policy");
    if ((p != "auto") && (p != "automatic") && (p != "none")) {
      tfel::raise("areDataTransfersAutomatic: invalid data transfer policy '" +
                  p +
                  "'. Valid policies are 'automatic' (or 'auto') and 'none'");
    }
    return p != "none";
  }  // end of areDataTransfersAutomatic

}  // end of namespace mfront::generic_parallel
