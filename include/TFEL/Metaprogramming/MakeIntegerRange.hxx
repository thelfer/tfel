/*!
 * \file   include/TFEL/Metaprogramming/MakeIntegerRange.hxx
 * \author Thomas Helfer
 * \date 14/01/2020
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_METAPROGRAMMING_MAKEINTEGERRANGE_HXX
#define LIB_TFEL_METAPROGRAMMING_MAKEINTEGERRANGE_HXX

#include <utility>

namespace tfel::meta {

  /*!
   * \brief an helper class to shift the integer sequence by the given value
   * \tparam IntegerType: integer type
   * \tparam Value: value used to shift the given integer sequence
   * \tparam IntegerSequence: integer sequence to be shifted
   */
  template <typename IntegerType, IntegerType Value, typename IntegerSequence>
  struct ShiftIntegerSequence;

  /*!
   * \brief partial specialisation for `std::integer_sequence`
   * \tparam IntegerType: integer type
   * \tparam Value: value used to shift the given integer sequence
   * \tparam Ints: integer sequence to be shifted
   */
  template <typename IntegerType, IntegerType Value, IntegerType... Ints>
  struct ShiftIntegerSequence<IntegerType,
                              Value,
                              std::integer_sequence<IntegerType, Ints...>> {
    //! \brief the resulting integer sequence
    using type = std::integer_sequence<IntegerType, Value + Ints...>;
  };

  /*!
   * \brief a simple alias
   * \tparam IntegerType: integer type
   * \tparam Begin: beginning of the range
   * \tparam End: end of the range
   */
  template <typename IntegerType, IntegerType Begin, IntegerType End>
  using make_integer_range = typename ShiftIntegerSequence<
      IntegerType,
      Begin,
      std::make_integer_sequence<IntegerType, End - Begin>>::type;

}  // end of namespace tfel::meta

#endif /* LIB_TFEL_METAPROGRAMMING_MAKEINTEGERRANGE_HXX */
