/*!
 * \file  mfront/include/MFront/CodeBlock.ixx
 * \brief
 * \author Thomas Helfer
 * \brief 18 mai 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_CODEBLOCK_IXX
#define LIB_MFRONT_CODEBLOCK_IXX

namespace mfront {

  template <typename T>
  bool hasAttribute(const CodeBlock& cb, const std::string& n) {
    const auto p = cb.attributes.find(n);
    if (p != cb.attributes.end()) {
      if (!p->second.template is<T>()) {
        CodeBlock::throwUnmatchedAttributeType(n);
      }
      return true;
    }
    return false;
  }  // end of hasAttribute

  template <typename T>
  const T& getAttribute(const CodeBlock& cb, const std::string& n) {
    const auto p = cb.attributes.find(n);
    if (p == cb.attributes.end()) {
      CodeBlock::throwUndeclaredAttribute(n);
    }
    if (!p->second.template is<T>()) {
      CodeBlock::throwUnmatchedAttributeType(n);
    }
    return p->second.template get<T>();
  }  // end of getAttribute

  template <typename T>
  T getAttribute(const CodeBlock& cb, const std::string& n, const T& v) {
    const auto p = cb.attributes.find(n);
    if (p != cb.attributes.end()) {
      if (!p->second.template is<T>()) {
        CodeBlock::throwUnmatchedAttributeType(n);
      }
      return p->second.template get<T>();
    }
    return v;
  }  // end of getAttribute

}  // end of namespace mfront

#endif /* LIB_MFRONT_CODEBLOCK_IXX */
