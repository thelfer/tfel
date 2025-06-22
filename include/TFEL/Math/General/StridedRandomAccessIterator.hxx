/*!
 * \file   StridedRandomAccessIterator.hxx
 * \brief
 * \author Thomas Helfer
 * \date   08 avril 2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_STRIDEDRANDOMACCESSITERATOR_HXX
#define LIB_TFEL_MATH_STRIDEDRANDOMACCESSITERATOR_HXX

#include "TFEL/Config/TFELConfig.hxx"
#include <iterator>

namespace tfel {

  namespace math {

    /*!
     * \brief base class of the StridedRandomAccessIterator containing
     * some static common utility methods.
     */
    struct TFELMATH_VISIBILITY_EXPORT StridedRandomAccessIteratorBase {
      //!
      TFEL_NORETURN static void throwNullStrideException();
      //!
      TFEL_NORETURN static void throwUmatchedStrideException();
    };

    /*!
     * \tparam iterator_base:
     * \brief an adaptator to random access iterator
     */
    template <typename iterator_base>
    struct StridedRandomAccessIterator
        : public StridedRandomAccessIteratorBase {
      /*!
       * \brief a proxy class
       *
       * This proxy is used so that an expression such as &p[o], where
       * p is a StridedRandomAccessIterator and o is an offset,
       * returns a StridedRandomAccessIterator.
       *
       * Such expressions are widely used in MFront to initialize
       * variables and we can not change this easily.
       */
      struct proxy {
        //! a simple alias
        using traits = std::iterator_traits<iterator_base>;
        //! a simple alias
        using reference = typename traits::reference;
        /*! default construtor
         * \param[in] p_ : value of the StridedRandomAccessIterator
         */
        proxy(const StridedRandomAccessIterator p_) : p(p_) {}
        //! move construtor
        proxy(proxy&&) = default;
        //! copy construtor
        proxy(const proxy&) = default;
        //! move assignement
        proxy& operator=(proxy&&) = default;
        //! standard assignement
        proxy& operator=(const proxy&) = default;
        //! assignement
        template <typename T>
        proxy& operator=(const T& v) {
          *(p.current) = v;
          return *this;
        }
        //! implicit conversion to value_type
        inline operator reference() { return *(p.current); }
        //! implicit conversion to value_type
        inline operator const reference&() const { return *(p.current); }
        //! adress operator
        inline StridedRandomAccessIterator operator&() const { return p; }

       private:
        const StridedRandomAccessIterator p;
      };
      //! a simple alias
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
      using reference = typename traits::reference;
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
      explicit StridedRandomAccessIterator(const iterator_base c,
                                           const difference_type s)
          : current(c), stride(s) {
        if (this->stride == 0) {
          throwNullStrideException();
        };
      }  // end of StridedRandomAccessIterator
      //! default constructor
      explicit StridedRandomAccessIterator()
          : current(iterator_base()), stride(difference_type()) {}
      //! move constructor
      StridedRandomAccessIterator(StridedRandomAccessIterator&&) = default;
      //! copy constructor
      StridedRandomAccessIterator(const StridedRandomAccessIterator&) = default;
      //! move assignement
      StridedRandomAccessIterator& operator=(StridedRandomAccessIterator&&) =
          default;
      //! standard assignement
      StridedRandomAccessIterator& operator=(
          const StridedRandomAccessIterator&) = default;
      //! dereference operator
      proxy operator*() const {
        if (this->stride == 0) {
          throwNullStrideException();
        };
        return {*this};
      }
      //! access operator
      proxy operator[](const difference_type n) const {
        if (this->stride == 0) {
          throwNullStrideException();
        };
        return {*this + n};
      }

      StridedRandomAccessIterator operator++() {
        if (this->stride == 0) {
          throwNullStrideException();
        };
        std::advance(this->current, this->stride);
        return *this;
      }

      StridedRandomAccessIterator operator++(int) {
        if (this->stride == 0) {
          throwNullStrideException();
        };
        auto c = *this;
        std::advance(this->current, this->stride);
        return c;
      }

      StridedRandomAccessIterator operator+(const difference_type n) const {
        if (this->stride == 0) {
          throwNullStrideException();
        };
        auto c = *this;
        std::advance(c.current, n * (c.stride));
        return c;
      }

      StridedRandomAccessIterator operator-(const difference_type n) const {
        return *this + (-n);
      }

      StridedRandomAccessIterator operator--() {
        if (this->stride == 0) {
          throwNullStrideException();
        };
        std::advance(this->current, -(this->stride));
        return *this;
      }

      StridedRandomAccessIterator operator+=(const difference_type n) {
        if (this->stride == 0) {
          throwNullStrideException();
        };
        std::advance(this->current, n * (this->stride));
        return *this;
      }

      bool operator==(const StridedRandomAccessIterator& o) const {
        if (this->stride == 0) {
          throwNullStrideException();
        };
        return this->current == o.current;
      }  // end of operator==

      bool operator!=(const StridedRandomAccessIterator& o) const {
        return !(this->operator==(o));
      }  // end of operator==

      difference_type operator-(const StridedRandomAccessIterator& s) const {
        if (this->stride == 0) {
          throwNullStrideException();
        };
        if (this->stride != s.stride) {
          throwUmatchedStrideException();
        };
        return (this->current - s.current) / (this->stride);
      }  // end of operator==
     private:
      //! current position
      iterator_base current;
      //! stride value
      difference_type stride;
    };

    template <typename iterator_base>
    StridedRandomAccessIterator<iterator_base> operator+(
        const typename StridedRandomAccessIterator<
            iterator_base>::difference_type n,
        const StridedRandomAccessIterator<iterator_base> p) {
      return p + n;
    }

    template <typename iterator_base>
    StridedRandomAccessIterator<iterator_base> operator-(
        const typename StridedRandomAccessIterator<
            iterator_base>::difference_type n,
        const StridedRandomAccessIterator<iterator_base> p) {
      return p - n;
    }

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_STRIDEDRANDOMACCESSITERATOR_HXX */
