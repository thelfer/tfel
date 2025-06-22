/*!
 * \file  CodeBlock.cxx
 * \brief
 * \author Helfer Thomas
 * \date   21 janv. 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "MFront/CodeBlock.hxx"

namespace mfront {

  CodeBlock::CodeBlock() = default;
  CodeBlock::CodeBlock(CodeBlock&&) = default;
  CodeBlock::CodeBlock(const CodeBlock&) = default;
  CodeBlock& CodeBlock::operator=(CodeBlock&&) = default;
  CodeBlock& CodeBlock::operator=(const CodeBlock&) = default;
  CodeBlock::~CodeBlock() = default;

}  // end of namespace mfront
