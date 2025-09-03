/*!
 * \file   TFEL/Metaprogramming/Concepts.hxx
 * \brief    
 * \author Thomas Helfer
 * \date   03/09/2025
 */

#ifndef LIB_TFEL_METAPROGRAMMING_CONCEPTS_HXX
#define LIB_TFEL_METAPROGRAMMING_CONCEPTS_HXX

#include <type_traits>

namespace tfel::meta {

  template <typename ValueType>
  concept ReferenceConcept = std::is_reference_v<ValueType>;

} // end of namespace tfel::meta

#endif /* LIB_TFEL_METAPROGRAMMING_CONCEPTS_HXX */
