/*!
 * \file   include/TFEL/Utilities/GenTypeBase_MSC.hxx
 * \brief  This file implements the GenTypeBase class and some
 * internal classes used by the GenType class.
 * \see    TL
 * \author Thomas Helfer
 * \date   19 Apr. 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

namespace tfel {

  namespace utilities {

    //! The base class of GenType.
    /*
     * \param  typename List, the list of types contained in the GenType.
     * \author Thomas Helfer.
     * \date   20 Apr. 2007.
     */
    template <typename List>
    struct GenTypeBase
        : public tfel::utilities::internals::
              GenTypeSpecializedMethods<GenTypeBase<List>, List>,
          public tfel::utilities::internals::GenTypeRunTimeMethods<List> {
      //! Default constructor.
      TFEL_INLINE GenTypeBase() : index(tfel::meta::TLSize<List>::value) {}
      template <typename T1,
                typename std::enable_if<
                    tfel::meta::TLCountNbrOfT<typename std::decay<T1>::type,
                                              List>::value == 1,
                    bool>::type = true>
      TFEL_INLINE GenTypeBase(const T1 &v)
          : index(tfel::meta::TLSize<List>::value) {
        this->template set<T1>(v);
      }
      template <typename T1,
                typename std::enable_if<
                    tfel::meta::TLCountNbrOfT<typename std::decay<T1>::type,
                                              List>::value == 1,
                    bool>::type = true>
      TFEL_INLINE GenTypeBase(T1 &&v) : index(tfel::meta::TLSize<List>::value) {
        this->template set<T1>(v);
      }
      //! Copy constructor.
      /*
       * \param const GenTypeBase&, the source.
       */
      TFEL_INLINE GenTypeBase(const GenTypeBase &src) : index(src.index) {
        if (!this->empty()) {
          // create a new object by copy
          auto *tmp = reinterpret_cast<void *>(&buffer);
          auto *tmp2 = reinterpret_cast<const void *>(&(src.buffer));
          (*(this->get_copy_constructor(this->index)))(tmp, tmp2);
        }
      }
      //! The assignement operator.
      /*
       * \param const GenTypeBase&, the right-hand side.
       */
      TFEL_INLINE GenTypeBase &operator=(const GenTypeBase &src) {
        // check for self-assignement
        if (this == &src) {
          return *this;
        }
        if (this->index == src.index) {
          if (src.index != ListSize) {
            // the two GenTypes hold the same objects
            // we then use the assignement operator.
            auto *tmp = reinterpret_cast<void *>(&buffer);
            auto *tmp2 = reinterpret_cast<const void *>(&(src.buffer));
            (*(this->get_assignement_operator(this->index)))(tmp, tmp2);
          } else {
            // src is not initialized,
            // we then destroy holded object
            this->clear();
          }
        } else {
          // the two GenType have two differents objects.
          // we first destroy the previous object and create a new
          // one by copy.
          this->clear();
          this->index = src.index;
          if (!this->empty()) {
            auto *tmp = reinterpret_cast<void *>(&buffer);
            auto *tmp2 = reinterpret_cast<const void *>(&(src.buffer));
            (*(this->get_copy_constructor(this->index)))(tmp, tmp2);
          }
        }
        return *this;
      }
      //! \return true if empty
      bool empty() const {
        return this->index == tfel::meta::TLSize<List>::value;
      }
      //! copy a GenType (calls the assignement operator).
      /*
       * \param const GenTypeBase&, the object to which it is compared.
       */
      bool copy(const GenTypeBase &src) {
        this->operator=(src);
        return true;
      }
      /*!
       * \param[in] args: argument passed to the constructor of the
       * object
       */
      template <typename T1, typename... Args>
      TFEL_INLINE typename std::enable_if<
          tfel::meta::TLCountNbrOfT<typename std::decay<T1>::type,
                                    List>::value == 1,
          T1 &>::type
      emplace(Args &&...args) {
        // We create a new object of type T1 by calling the copy constructor
        this->template set_uninitialised<T1>();
        void *p = reinterpret_cast<void *>(&(this->buffer));
        // the magic of placement new...
        return *(new (p) T1(std::forward<Args>(args)...));
      }  // end of emplace
         //! set the value of the GenType.
         /*
          * \param const T1&, the value affected to the GenType.
          * \pre   T1 must be a type that the GenType can hold.
          */
      template <typename T1>
      TFEL_INLINE typename std::enable_if<
          tfel::meta::TLCountNbrOfT<typename std::decay<T1>::type,
                                    List>::value == 1,
          void>::type
      set(const T1 &src) {
        using type = typename std::decay<T1>::type;
        if (this->template is<type>()) {
          if (&(this->template get<type>()) == &src) {
            return;
          }
        }
        // We create a new object of type T1 by calling the copy constructor
        this->template set_uninitialised<type>();
        void *p = reinterpret_cast<void *>(&(this->buffer));
        // the magic of placement new...
        new (p) type(src);
      }
      template <typename T1>
      TFEL_INLINE typename std::enable_if<
          tfel::meta::TLCountNbrOfT<typename std::decay<T1>::type,
                                    List>::value == 1,
          void>::type
      set(T1 &&src) {
        using type = typename std::decay<T1>::type;
        if (this->template is<type>()) {
          if (&(this->template get<type>()) == &src) {
            return;
          }
        }
        // We create a new object of type T1 by calling the copy constructor
        this->template set_uninitialised<type>();
        void *p = reinterpret_cast<void *>(&(this->buffer));
        // the magic of placement new...
        new (p) type(src);
      }
      //! assignement operator.
      /*
       * \param  const T1&, the value affected to the GenType.
       * \return GenTypeBase&, a reference to this.
       * \pre    T1 must be a type that the GenType can hold.
       */
      template <typename T1>
      TFEL_INLINE typename std::enable_if<
          tfel::meta::TLCountNbrOfT<typename std::decay<T1>::type,
                                    List>::value == 1,
          GenTypeBase &>::type
      operator=(const T1 &src) {
        this->template set<T1>(src);
        return *this;
      }
      /*
       * \param  const T1&, the value affected to the GenType.
       * \return GenTypeBase&, a reference to this.
       * \pre    T1 must be a type that the GenType can hold.
       */
      template <typename T1>
      TFEL_INLINE typename std::enable_if<
          tfel::meta::TLCountNbrOfT<typename std::decay<T1>::type,
                                    List>::value == 1,
          GenTypeBase &>::type
      operator=(T1 &&src) {
        this->template set<T1>(src);
        return *this;
      }
      template <typename T1>
      typename std::enable_if<
          tfel::meta::TLCountNbrOfT<typename std::decay<T1>::type,
                                    List>::value == 1,
          bool>::type
      is() const {
        return this->index == tfel::meta::TLPosition<T1, List>::value;
      }  // end of Value::is
         //! get the value contained in the GenType (const version).
         /*
          * \return const T1&, the value affected to the GenType.
          * \pre    T1 must be a type that the GenType can hold.
          * \throw  GenTypeCastError, if the type contained in the GenType does
          * not match.
          */
      template <typename T1>
      TFEL_INLINE typename std::enable_if<
          tfel::meta::TLCountNbrOfT<typename std::decay<T1>::type,
                                    List>::value == 1,
          const T1 &>::type
      get() const {
        // a silly trick to avoir a gcc warning
        union {
          const storage_t *c;
          const T1 *ptr;
        } ptr;
        if (this->index != tfel::meta::TLFindEltPos<T1, List>::value) {
          throw(GenTypeCastError());
        }
        ptr.c = &(this->buffer);
        return *(ptr.ptr);
      }
      //! get the value contained in the GenType.
      /*
       * \return T1&, the value affected to the GenType.
       * \pre    T1 must be a type that the GenType can hold.
       * \throw  GenTypeCastError, if the type contained in the GenType does not
       * match.
       */
      template <typename T1>
      TFEL_INLINE typename std::enable_if<
          tfel::meta::TLCountNbrOfT<typename std::decay<T1>::type,
                                    List>::value == 1,
          T1 &>::type
      get() {
        using namespace std;
        using namespace tfel::meta;
        using namespace tfel::utilities::internals;
        // a silly trick to avoir a gcc warning
        union {
          storage_t *c;
          T1 *ptr;
        } ptr;
        if (this->index != TLFindEltPos<T1, List>::value) {
          throw(GenTypeCastError());
        }
        ptr.c = &(this->buffer);
        return *(ptr.ptr);
      }
      /*!
       * \return the type index of the object
       */
      TFEL_INLINE unsigned short getTypeIndex() const { return this->index; }
      /*
       * Destructor
       */
      TFEL_INLINE ~GenTypeBase() { this->clear(); }
      //! number of object that the GenType can hold
      static constexpr unsigned short ListSize =
          tfel::meta::TLSize<List>::value;

     protected:
      //! clear the GenType
      TFEL_INLINE void clear() {
        if (!this->empty()) {
          // destroy the current object
          auto *tmp = reinterpret_cast<void *>(&buffer);
          (*(this->get_destructor(this->index)))(tmp);
        }
        this->index = ListSize;
      }
      //! memory where objects are holded
      //! set the value of the GenType.
      /*
       * \param const T1&, the value affected to the GenType.
       * \pre   T1 must be a type that the GenType can hold.
       */
      template <typename T1>
      TFEL_INLINE typename std::enable_if<
          tfel::meta::TLCountNbrOfT<typename std::decay<T1>::type,
                                    List>::value == 1,
          void>::type
      set_uninitialised() {
        this->clear();
        this->index = tfel::meta::TLFindEltPos<T1, List>::value;
      }
      //! container type
      typedef typename std::aligned_storage<
          tfel::meta::TLMaxSize<List>::value,
          tfel::meta::TLMaxAlign<List>::value>::type storage_t;
      //! the buffer where objects are hold.
      storage_t buffer;
      //! index to the current type hold by the GenType.
      unsigned short index;

     private:
      //! a simple assertion
      TFEL_STATIC_ASSERT((tfel::meta::TLElementsAreUnique<List>::cond));
    };

  }  // end of namespace utilities

}  // end of namespace tfel
