/*!
 * \file   TFEL/Metaprogramming/Concepts.hxx
 * \brief
 * \author Thomas Helfer
 * \date 03/09/2025
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of theses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_METAPROGRAMMING_CONCEPTS_HXX
#define LIB_TFEL_METAPROGRAMMING_CONCEPTS_HXX

#include <type_traits>

namespace tfel::meta {

  template <typename ValueType>
  concept ReferenceConcept = std::is_reference_v<ValueType>;

}  // end of namespace tfel::meta

#endif /* LIB_TFEL_METAPROGRAMMING_CONCEPTS_HXX */
