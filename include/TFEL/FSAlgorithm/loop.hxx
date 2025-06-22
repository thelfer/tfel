/*!
 * \file   include/TFEL/FSAlgorithm/loop.hxx
 * \brief  this file implements the loop class.
 * \author Thomas Helfer
 * \date   18/04/2018
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_FSALGORITHM_LOOP_HXX
#define LIB_TFEL_FSALGORITHM_LOOP_HXX

#include "TFEL/Config/TFELConfig.hxx"
#include <type_traits>
#include "TFEL/TypeTraits/IsRandomAccessIterator.hxx"

namespace tfel {

  namespace fsalgo {

    namespace internals {

      /*!
       * \brief an helper structure to unroll the loop at compile time.
       * \tparam I: current indice
       * \tparam N: indice corresponding to the end of the loop
       */
      template <unsigned int I, unsigned int N>
      struct do_loop {
        /*!
         * \brief perform on step of the loop and call the next one
         * \tparam F: type of the callable
         * \param[in,out] f: callable describing the body of the loop
         */
        template <typename F>
        void exe(F& f) {
          f(I);
          do_loop<I + 1, N>::exe(f);
        }  // end of exe
        static_assert(I < N, "invalid loop index");
      };  // end of struct do_loop<I, N>

      /*!
       * \brief partial specialisation of the `do_loop` structure to end the
       * loop.
       * \tparam N: indice corresponding to the  end of the loop
       */
      template <unsigned int N>
      struct do_loop<N, N> {
        /*!
         * \tparam F: type of the callable
         */
        template <typename F>
        void exe(F&) {}  // end of exe
      };                 // end of struct do_loop<N, N>

    }  // namespace internals

    /*!
     * \class loop
     * \author Thomas Helfer
     * \date   18/04/2018
     * This class is meant to manually unroll a loop a compile time.
     * The body of the loop is passed as a callable taking the
     * current indice as argument.
     */
    template <unsigned int N>
    struct loop {
      /*!
       * \tparam F: type of the callable
       * \param[in,out] f: callable describing the body of the loop
       */
      template <typename F>
      void exe(F& f) {
        tfel::fsalgo::internals::do_loop<0, N>::exe(f);
      }  // end of exe
    };

  }  // end of namespace fsalgo

}  // end of namespace tfel

#endif /* LIB_TFEL_FSALGORITHM_LOOP_HXX */
