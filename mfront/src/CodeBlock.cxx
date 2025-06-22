/*!
 * \file  CodeBlock.cxx
 * \brief
 * \author Thomas Helfer
 * \date   21 janv. 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "TFEL/Raise.hxx"
#include "MFront/CodeBlock.hxx"

namespace mfront {

  const char* const CodeBlock::requires_jacobian_decomposition =
      "requires_jacobian_decomposition";
  const char* const CodeBlock::uses_get_partial_jacobian_invert =
      "uses_get_partial_jacobian_invert";
  const char* const CodeBlock::used_get_integration_variables_derivatives =
      "used_get_integration_variables_derivatives";
  const char* const CodeBlock::
      used_implicit_equations_derivatives_with_respect_to_gradients_or_external_state_variables =
          "used_implicit_equations_derivatives_with_respect_to_gradients_or_"
          "external_state_variables";
  const char* const CodeBlock::used_jacobian_invert_blocks =
      "used_jacobian_invert_blocks";
  const char* const CodeBlock::used_initialize_function_variables =
      "used_initialize_function_variables";
  const char* const CodeBlock::used_postprocessing_variables =
      "used_postprocessing_variables";

  void CodeBlock::throwUnmatchedAttributeType(const std::string& n) {
    tfel::raise(
        "CodeBlock::throwUnmatchedAttributeType: "
        "unexpected type for attribute '" +
        n + "'");
  }  // end of CodeBlock::throwUnmatchedAttributeType

  void CodeBlock::throwUndeclaredAttribute(const std::string& n) {
    tfel::raise(
        "CodeBlock::throwUndeclaredAttribute: "
        "no attribute '" +
        n + "' declared");
  }  // end of CodeBlock::throwUndeclaredAttribute

  CodeBlock::CodeBlock() = default;
  CodeBlock::CodeBlock(CodeBlock&&) = default;
  CodeBlock::CodeBlock(const CodeBlock&) = default;
  CodeBlock& CodeBlock::operator=(CodeBlock&&) = default;
  CodeBlock& CodeBlock::operator=(const CodeBlock&) = default;
  CodeBlock::~CodeBlock() = default;

}  // end of namespace mfront
