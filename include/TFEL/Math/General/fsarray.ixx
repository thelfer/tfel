/*!
 * \file   include/TFEL/Math/General/fsarray.ixx
 * \brief
 * \author Helfer Thomas
 * \date   30 Août 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_TINY_FSARRAY_IXX_
#define LIB_TFEL_TINY_FSARRAY_IXX_

namespace tfel {

  namespace math {

    namespace internals {

      /*!
       * \brief : an auxiliary structure used to initialize a
       * (constexpr) array_container from an uniform value or an
       * std::initializer_list
       *
       * Basically, its recursively build an argument pack which is
       * used to initialize the array_container in the partial
       * specialisation fsarray_initializer<1u> below.
       *
       * \tparam N : step in the initalisation algorithm_
       */
      template <unsigned short N>
      struct fsarray_initializer {
        template <typename T, typename T2, typename... Tn>
        static constexpr array_container<sizeof...(Tn) + N, T> applyFromValue(
            const T2& v, Tn... vs) {
          return fsarray_initializer<N - 1>::template applyFromValue<T, T2,
                                                                     Tn...>(
              v, v, vs...);
        }
        template <typename T, typename T2, typename... Tn>
        static constexpr array_container<sizeof...(Tn) + N, T>
        applyFromInitializerList(const std::initializer_list<T2>& v, Tn... vs) {
          return fsarray_initializer<N - 1>::template applyFromInitializerList<
              T, T2, T2, Tn...>(v, *(v.begin() + N - 1), vs...);
        }
        template <typename T, typename Container, typename... Tn>
        static constexpr array_container<sizeof...(Tn) + N, T> applyFromArray(
            const Container& v, Tn... vs) {
          return fsarray_initializer<N - 1>::template applyFromArray<
              T, Container, decltype(v[N - 1]), Tn...>(v, v[N - 1], vs...);
        }
      };  // end of struct fsarray_initializer<unsigned short N>
      /*!
       * \brief partial specialisation of fsarray_initializer to end the
       * recursion
       */
      template <>
      struct fsarray_initializer<1u> {
        template <typename T, typename T2, typename... Tn>
        static constexpr array_container<sizeof...(Tn) + 1, T> applyFromValue(
            const T2& v, Tn... vs) {
          return buildArray<T>(v, vs...);
        }
        template <typename T, typename T2, typename... Tn>
        static constexpr array_container<sizeof...(Tn) + 1, T>
        applyFromInitializerList(const std::initializer_list<T2>& v, Tn... vs) {
          return buildArray<T>(*(v.begin()), vs...);
        }
        template <typename T, typename T2, typename... Tn>
        static constexpr array_container<sizeof...(Tn) + 1, T> applyFromArray(
            const T2& v, Tn... vs) {
          return buildArray<T>(v[0], vs...);
        }
        template <typename T, typename... Tn>
        static constexpr array_container<sizeof...(Tn), T> buildArray(
            Tn... vs) {
          return array_container<sizeof...(vs), T>{{static_cast<T>(vs)...}};
        }
      };  // end of struct fsarray_initializer<1>
    }     // end of namespace internals

    template <unsigned short N, typename T>
    fsarray<N, T>::fsarray() noexcept {}

    template <unsigned short N, typename T>
    template <
        typename T2,
        typename std::enable_if<tfel::typetraits::IsAssignableTo<T2, T>::cond,
                                bool>::type>
    constexpr fsarray<N, T>::fsarray(const fsarray<N, T2>& values)
        : array_container<N, T>(tfel::math::internals::fsarray_initializer<
                                N>::template applyFromArray<T>(values)) {}

    template <unsigned short N, typename T>
    template <
        typename T2,
        typename std::enable_if<tfel::typetraits::IsAssignableTo<T2, T>::cond,
                                bool>::type>
    constexpr fsarray<N, T>::fsarray(const std::initializer_list<T2>& values)
        : array_container<N, T>(
              values.size() == 1u
                  ? tfel::math::internals::fsarray_initializer<
                        N>::template applyFromValue<T>(*(values.begin()))
                  : tfel::math::internals::fsarray_initializer<
                        N>::template applyFromInitializerList<T>(values)) {}

    template <unsigned short N, typename T>
    template <
        typename T2,
        typename std::enable_if<tfel::typetraits::IsAssignableTo<T2, T>::cond,
                                bool>::type>
    constexpr fsarray<N, T>::fsarray(const T2& value)
        : array_container<N, T>(tfel::math::internals::fsarray_initializer<
                                N>::template applyFromValue<T>(value)) {}

    template <unsigned short N, typename T>
    typename fsarray<N, T>::iterator fsarray<N, T>::begin(void) noexcept {
      return this->v;
    }

    template <unsigned short N, typename T>
    typename fsarray<N, T>::const_iterator fsarray<N, T>::begin(
        void) const noexcept {
      return this->v;
    }

    template <unsigned short N, typename T>
    typename fsarray<N, T>::const_iterator fsarray<N, T>::cbegin(
        void) const noexcept {
      return this->v;
    }

    template <unsigned short N, typename T>
    typename fsarray<N, T>::iterator fsarray<N, T>::end(void) noexcept {
      return this->v + N;
    }

    template <unsigned short N, typename T>
    typename fsarray<N, T>::const_iterator fsarray<N, T>::end(
        void) const noexcept {
      return this->v + N;
    }

    template <unsigned short N, typename T>
    typename fsarray<N, T>::const_iterator fsarray<N, T>::cend(
        void) const noexcept {
      return this->v + N;
    }

    template <unsigned short N, typename T>
    typename fsarray<N, T>::reverse_iterator fsarray<N, T>::rbegin(
        void) noexcept {
      return reverse_iterator(this->v + N);
    }

    template <unsigned short N, typename T>
    typename fsarray<N, T>::const_reverse_iterator fsarray<N, T>::rbegin(
        void) const noexcept {
      return const_reverse_iterator(this->v + N);
    }

    template <unsigned short N, typename T>
    typename fsarray<N, T>::reverse_iterator fsarray<N, T>::rend(
        void) noexcept {
      return reverse_iterator(this->v);
    }

    template <unsigned short N, typename T>
    typename fsarray<N, T>::const_reverse_iterator fsarray<N, T>::rend(
        void) const noexcept {
      return const_reverse_iterator(this->v);
    }

    template <unsigned short N, typename T>
    constexpr unsigned short fsarray<N, T>::size(void) const noexcept {
      return N;
    }

    template <unsigned short N, typename T>
    typename fsarray<N, T>::reference fsarray<N, T>::operator[](
        const size_type i) noexcept {
      return this->v[i];
    }

    template <unsigned short N, typename T>
    constexpr typename fsarray<N, T>::const_reference fsarray<N, T>::operator[](
        const size_type i) const noexcept {
      return this->v[i];
    }

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_TINY_FSARRAY_IXX_ */
