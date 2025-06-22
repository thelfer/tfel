/*!
 * \file  include/TFEL/Math/Array/IterateOverMultipleIndices.hxx
 * \brief
 * \author Thomas Helfer
 * \date 01/01/2021
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_ARRAY_ITERATEOVERMULTIPLEINDICES_HXX
#define LIB_TFEL_MATH_ARRAY_ITERATEOVERMULTIPLEINDICES_HXX

namespace tfel::math {

  template <typename TypeSize, TypeSize CurrentIndex, TypeSize FinalIndex>
  struct LoopUnRoller {
    template <typename Functor>
    TFEL_HOST_DEVICE constexpr static void exe(Functor& f) {
      f(CurrentIndex);
      if constexpr (CurrentIndex + 1 != FinalIndex) {
        LoopUnRoller<TypeSize, CurrentIndex + 1, FinalIndex>::exe(f);
      }
    }
  };

  template <typename TypeSize,
            TypeSize CurrentIndex,
            TypeSize Arity,
            bool UnrollLoop = false>
  struct IterateOverMultipleIndices {
    template <typename Functor, typename GetSizeFunctor, typename... Indices>
    TFEL_HOST_DEVICE constexpr static void exe(Functor& f,
                                               const GetSizeFunctor g,
                                               const Indices... indices) {
      auto iter = [&](const TypeSize i) {
        if constexpr (CurrentIndex + 1 == Arity) {
          f(indices..., i);
        } else {
          IterateOverMultipleIndices<TypeSize, CurrentIndex + 1, Arity>::exe(
              f, g, indices..., i);
        }
      };
      if constexpr (UnrollLoop && (CurrentIndex + 1 == Arity)) {
        constexpr auto s = g(CurrentIndex);
        LoopUnRoller<TypeSize, 0, s>::exe(iter);
      } else {
        for (TypeSize i = 0; i != g(CurrentIndex); ++i) {
          iter(i);
        }
      }
    }  // end of exe
  };   // end of struct

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_ARRAY_ITERATEOVERMULTIPLEINDICES_HXX */
