/*!
 * \file   mfront/src/GenericParallel/AbstractBackendExecutionPolicy.cxx
 * \brief  This file defines the base class of the execution policies of the
 * `generic-parallel` interfaces (`MFrontGenericParallelRuntime` library).
 * \date   22/10/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "MFront/GenericParallel/AbstractBackendExecutionPolicy.hxx"

namespace mfront::generic_parallel {

  AbstractBackendExecutionPolicy::~AbstractBackendExecutionPolicy() = default;

}  // end of namespace mfront::generic_parallel
