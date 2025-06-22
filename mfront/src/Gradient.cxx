/*!
 * \file  mfront/src/Gradient.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 11 mai 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "MFront/Gradient.hxx"

namespace mfront {

  void Gradient::setIsIncrementKnownAttribute(VariableDescription& v,
                                              const bool b) {
    return v.setAttribute("Gradient::is_increment_known", b, false);
  }  // end of Gradient::setIsIncrementKnownAttribute

  bool Gradient::isIncrementKnown(const VariableDescription& v) {
    return v.getAttribute<bool>("Gradient::is_increment_known", false);
  }  // end of Gradient::isIncrementKnown

  Gradient::Gradient(const std::string& t, const std::string& n)
      : VariableDescription(t, n, 1u, 0u) {}  // end of Gradient::Gradient

  Gradient::Gradient(const std::string& t,
                     const std::string& s,
                     const std::string& n)
      : VariableDescription(t, s, n, 1u, 0u) {}  // end of Gradient::Gradient

  Gradient::Gradient(const VariableDescription& v)
      : VariableDescription(v) {}  // end of Gradient::Gradient

  Gradient::Gradient(Gradient&&) = default;
  Gradient::Gradient(const Gradient&) = default;
  Gradient& Gradient::operator=(Gradient&&) = default;
  Gradient& Gradient::operator=(const Gradient&) = default;

  Gradient::~Gradient() noexcept = default;

}  // end of namespace mfront
