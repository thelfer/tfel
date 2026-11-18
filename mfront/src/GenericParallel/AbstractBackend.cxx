/*!
 * \file   mfront/src/GenericParallel/AbstractBackend.cxx
 * \brief  This file implements the `AbstractBackend` class of the
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

#include <ostream>
#include "MFront/CodeGeneratorUtilities.hxx"
#include "MFront/GenericParallel/AbstractBackend.hxx"

namespace mfront::generic_parallel {

  void AbstractBackend::writeSymbols(std::ostream& os,
                                     const std::string_view f) const {
    const auto n = std::string{f} + "_generic_parallel_";
    exportStringSymbol(os, n + "backend", this->getName());
    // version of the data structures exchanged with the generated functions
    exportSymbol(os, "unsigned short", n + "api_version",
                 "MFRONT_GPMP_API_VERSION");
    exportSymbol(os, "const char*", n + "memory_space", this->getMemorySpace());
    // identifier of the execution policy expected by the generated
    // functions. The expression is enclosed in parentheses: the type of a
    // policy may contain commas, which would split the arguments of the
    // macro exporting the symbol.
    exportSymbol(os, "const char*", n + "execution_policy",
                 "(" + this->getExecutionPolicyType() + "::identifier())");
  }  // end of writeSymbols

  void AbstractBackend::writeCall(std::ostream& os,
                                  const std::string_view f,
                                  const std::string_view status,
                                  const std::string_view arguments) const {
    os << "return mfront::gpmp::" << f << "(" << status << ", " << arguments
       << ", " << this->getApply() << ");\n";
  }  // end of writeCall

  AbstractBackend::~AbstractBackend() noexcept = default;

}  // end of namespace mfront::generic_parallel
