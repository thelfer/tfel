/*!
 * \file   DifferenceRandomAccessIterator.hxx
 * \brief
 * \author Thomas Helfer
 * \date 08/04/2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_DIFFERENCERANDOMACCESSITERATOR_HXX
#define LIB_TFEL_MATH_DIFFERENCERANDOMACCESSITERATOR_HXX

#include "TFEL/Config/TFELConfig.hxx"
#include <iterator>

namespace tfel::math {

  /*!
   * \brief base class of the DifferenceRandomAccessIterator containing
   * some static common utility methods.
   */
  struct TFELMATH_VISIBILITY_EXPORT DifferenceRandomAccessIteratorBase {
    //!
    [[noreturn]] static void throwInconsistentIteratorDifference();
  };

  /*!
   * \tparam iterator_base:
   * \brief an adaptator to random access iterator
   */
  template <typename iterator_base>
  struct DifferenceRandomAccessIterator
      : public DifferenceRandomAccessIteratorBase {
    /*!
     * \brief a proxy class
     *
     * This proxy is used so that an expression such as &p[o], where
     * p is a DifferenceRandomAccessIterator and o is an offset,
     * returns a DifferenceRandomAccessIterator.
     *
     * Such expressions are widely used in MFront to initialize
     * variables and we can not change this easily.
     */
    struct proxy {
      //! \brief a simple alias
      using traits = std::iterator_traits<iterator_base>;
      //! \brief a simple alias
      using value_type = typename traits::value_type;
      /*! default construtor
       * \param[in] p_ : value of the DifferenceRandomAccessIterator
       */
      explicit proxy(const DifferenceRandomAccessIterator p_) : p(p_) {}
      //! \brief move construtor
      proxy(proxy&&) = default;
      //! \brief copy construtor
      proxy(const proxy&) = default;
      //! \brief move assignement
      proxy& operator=(proxy&&) = default;
      //! \brief standard assignement
      proxy& operator=(const proxy&) = default;
      // NOLINTBEGIN(google-explicit-constructor)
      //! \brief implicit conversion to value_type
      inline operator const value_type() const {
        return *(p.first) - *(p.second);
      }
      // NOLINTEND(google-explicit-constructor)
      //! \brief adress operator
      inline DifferenceRandomAccessIterator operator&() const { return p; }

     private:
      const DifferenceRandomAccessIterator p;
    };
    //! \brief a simple alias
    using traits = std::iterator_traits<iterator_base>;
    /*!
     * \brief STL requirements
     * https://www.sgi.com/tech/stl/trivial.html
     *
     * The requirement for the return type of *x is specified as
     * "convertible to T", rather than simply T, because it
     * sometimes makes sense for an iterator to return some sort of
     * proxy object instead of the object that the iterator
     * conceptually points to. Proxy objects are implementation
     * details rather than part of an interface (one use of them,
     * for example, is to allow an iterator to behave differently
     * depending on whether its value is being read or written), so
     * the value type of an iterator that returns a proxy is still
     * T.
     */
    using value_type = typename traits::value_type;
    //! \brief STL requirement
    using reference = proxy;
    //! \brief STL requirement
    using pointer = typename traits::pointer;
    //! \brief STL requirement
    using iterator_category = typename traits::iterator_category;
    //! \brief STL requirement
    using difference_type = typename traits::difference_type;
    /*!
     * \brief constructor
     * \param[in] c: current position
     * \param[in] s: stride
     */
    explicit DifferenceRandomAccessIterator(const iterator_base i1,
                                            const iterator_base i2)
        : first(i1), second(i2) {}  // end of DifferenceRandomAccessIterator
    //! \brief default constructor
    DifferenceRandomAccessIterator()
        : first(iterator_base()), second(iterator_base()) {}
    //! \brief move constructor
    DifferenceRandomAccessIterator(DifferenceRandomAccessIterator&&) = default;
    //! \brief copy constructor
    DifferenceRandomAccessIterator(const DifferenceRandomAccessIterator&) =
        default;
    //! \brief move assignement
    DifferenceRandomAccessIterator& operator=(
        DifferenceRandomAccessIterator&&) = default;
    //! \brief standard assignement
    DifferenceRandomAccessIterator& operator=(
        const DifferenceRandomAccessIterator&) = default;
    //! \brief dereference operator
    auto operator*() const { return proxy{*this}; }
    //! \brief access operator
    auto operator[](const difference_type n) const { return proxy{*this + n}; }

    DifferenceRandomAccessIterator operator++() {
      std::advance(this->first, 1);
      std::advance(this->second, 1);
      return *this;
    }

    DifferenceRandomAccessIterator operator++(int) {
      auto c = *this;
      std::advance(this->first, 1);
      std::advance(this->second, 1);
      return c;
    }

    DifferenceRandomAccessIterator operator+(const difference_type n) const {
      auto c = *this;
      std::advance(c.first, n);
      std::advance(c.second, n);
      return c;
    }

    DifferenceRandomAccessIterator operator-(const difference_type n) const {
      return *this + (-n);
    }

    DifferenceRandomAccessIterator operator+=(const difference_type n) {
      std::advance(this->first, n);
      std::advance(this->second, n);
      return *this;
    }

    bool operator==(const DifferenceRandomAccessIterator& o) const {
      return ((this->first == o.first) && (this->second == o.second));
    }  // end of operator==

    bool operator!=(const DifferenceRandomAccessIterator& o) const {
      return !(this->operator==(o));
    }  // end of operator==

    difference_type operator-(const DifferenceRandomAccessIterator& s) const {
      const difference_type d = this->first - s.first;
      if (d != (this->second - s.second)) {
        throwInconsistentIteratorDifference();
      }
      return d;
    }  // end of operator==
   private:
    //! \brief first iterator of the operation
    iterator_base first;
    //! \brief second iterator of the operation
    iterator_base second;
  };

  template <typename iterator_base>
  DifferenceRandomAccessIterator<iterator_base> operator+(
      const typename DifferenceRandomAccessIterator<
          iterator_base>::difference_type n,
      const DifferenceRandomAccessIterator<iterator_base> p) {
    return p + n;
  }

  template <typename iterator_base>
  DifferenceRandomAccessIterator<iterator_base> operator-(
      const typename DifferenceRandomAccessIterator<
          iterator_base>::difference_type n,
      const DifferenceRandomAccessIterator<iterator_base> p) {
    return p - n;
  }

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_DIFFERENCERANDOMACCESSITERATOR_HXX */
