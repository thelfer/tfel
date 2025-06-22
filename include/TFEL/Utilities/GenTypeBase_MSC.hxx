/*!
 * \file   include/TFEL/Utilities/GenTypeBase_MSV.hxx
 * \brief  This file implements the GenTypeBase class and some
 * internal classes used by the GenType class.
 * \see    TL
 * \author Helfer Thomas
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

    namespace internals {

      //! An helper class to fill the runtime methods of a GenType.
      /*
       * \param unsigned short N, the current index.
       * \param typename List, a list of types.
       * \author Helfer Thomas.
       * \date   20 Apr. 2007.
       */
      template <unsigned short N, typename List>
      struct TFEL_VISIBILITY_LOCAL GenTypeRunTimeMethods
#ifndef DOXYGEN_SHOULD_SKIP_THIS
          : public GenTypeRunTimeMethods<N - 1, List>
#endif /* DOXYGEN_SHOULD_SKIP_THIS */
      {
        //! Default constructor
        /*
         * This constructor fills the arrays of function pointers
         * associated with the N-1 type in the typelist.
         */
        GenTypeRunTimeMethods() {
          this->destroyMethod[N - 1] = &GenTypeDestroy::exe;
          this->copyMethod[N - 1] = &GenTypeCopy<Current>::exe;
          this->assignMethod[N - 1] = &GenTypeAssign<Current>::exe;
        }

       private:
        //! a simple alias.
        typedef typename tfel::meta::TLFindNthElt<List, N - 1>::type Current;
        //! choose the function used to destroy the N-1 type of the typelist.
        typedef typename std::conditional<std::is_trivial<Current>::value,
                                          GenTypeTrivialDestroy<Current>,
                                          GenTypeGenericDestroy<Current>>::type
            GenTypeDestroy;
      };

      //! A partial specialisation to end the recursion.
      /*
       * \author Helfer Thomas.
       * \date   20 Apr. 2007.
       */
      template <typename List>
      struct TFEL_VISIBILITY_LOCAL GenTypeRunTimeMethods<0u, List> {
        //! a simple alias.
        typedef void (*DestroyMethod)(void* const);
        //! a simple alias.
        typedef void (*AssignMethod)(void* const, const void* const);
        //! a simple alias.
        typedef void (*CopyMethod)(void* const, const void* const);
        //! array containing function pointers to call a specific destructor.
        DestroyMethod destroyMethod[tfel::meta::TLSize<List>::value];
        //! array containing function pointers to call a specific assignement
        //! operator.
        AssignMethod assignMethod[tfel::meta::TLSize<List>::value];
        //! array containing function pointers to call a specific copy
        //! constructor.
        CopyMethod copyMethod[tfel::meta::TLSize<List>::value];
      };

    }  // end of namespace internals

    //! The base class of GenType.
    /*
     * \param  typename List, the list of types contained in the GenType.
     * \author Helfer Thomas.
     * \date   20 Apr. 2007.
     */
    template <typename List>
    struct GenTypeBase
        : public tfel::utilities::internals::
              GenTypeSpecializedMethods<GenTypeBase<List>, List> {
      //! Default constructor.
      TFEL_INLINE GenTypeBase() : index(tfel::meta::TLSize<List>::value) {}
      template <typename T1,
                typename std::enable_if<
                    tfel::meta::TLCountNbrOfT<T1, List>::value == 1,
                    bool>::type = true>
      TFEL_INLINE GenTypeBase(const T1& v)
          : index(tfel::meta::TLSize<List>::value) {
        this->template set<T1>(v);
      }
      //! Copy constructor.
      /*
       * \param const GenTypeBase&, the source.
       */
      TFEL_INLINE GenTypeBase(const GenTypeBase& src) : index(src.index) {
        using namespace tfel::meta;
        if (this->index != TLSize<List>::value) {
          // create a new object by copy
          void* tmp = reinterpret_cast<void*>(&buffer);
          const void* tmp2 = reinterpret_cast<const void*>(&(src.buffer));
          (*(GenTypeBase::methods.copyMethod[this->index]))(tmp, tmp2);
        }
      }
      //! The assignement operator.
      /*
       * \param const GenTypeBase&, the right-hand side.
       */
      TFEL_INLINE GenTypeBase& operator=(const GenTypeBase& src) {
        using namespace tfel::meta;
        // check for self-assignement
        if (this != &src) {
          if (this->index == src.index) {
            if (src.index != TLSize<List>::value) {
              // the two GenTypes hold the same objects
              // we then use the assignement operator.
              void* tmp = reinterpret_cast<void*>(&buffer);
              const void* tmp2 = reinterpret_cast<const void*>(&(src.buffer));
              (*(GenTypeBase::methods.assignMethod[this->index]))(tmp, tmp2);
            } else {
              // src is not initialized,
              // we then destroy holded object
              if (this->index != TLSize<List>::value) {
                void* tmp = reinterpret_cast<void*>(&buffer);
                (*(GenTypeBase::methods.destroyMethod[this->index]))(tmp);
              }
              this->index = TLSize<List>::value;
            }
          } else {
            // the two GenType have two differents objects.
            // we first destroy the previous object and create a new
            // one by copy.
            if (this->index != TLSize<List>::value) {
              void* tmp = reinterpret_cast<void*>(&buffer);
              (*(GenTypeBase::methods.destroyMethod[this->index]))(tmp);
            }
            this->index = src.index;
            if (this->index != TLSize<List>::value) {
              void* tmp = reinterpret_cast<void*>(&buffer);
              const void* tmp2 = reinterpret_cast<const void*>(&(src.buffer));
              (*(GenTypeBase::methods.copyMethod[this->index]))(tmp, tmp2);
            }
          }
        }
        return *this;
      }
      //! \return true if empty
      bool empty(void) const {
        return this->index == tfel::meta::TLSize<List>::value;
      }
      //! copy a GenType (calls the assignement operator).
      /*
       * \param const GenTypeBase&, the object to which it is compared.
       */
      bool copy(const GenTypeBase& src) {
        using namespace tfel::meta;
        this->operator=(src);
        return true;
      }
      //! set the value of the GenType.
      /*
       * \param const T1&, the value affected to the GenType.
       * \pre   T1 must be a type that the GenType can hold.
       */
      template <typename T1>
      TFEL_INLINE typename std::
          enable_if<tfel::meta::TLCountNbrOfT<T1, List>::value == 1, void>::type
          set(const T1& src) {
        using namespace tfel::meta;
        using namespace tfel::utilities::internals;
        if (this->index == TLFindEltPos<T1, List>::value) {
          // a silly trick to avoir a gcc warning
          union {
            storage_t* c;
            T1* ptr;
          } ptr;
          // The GenType already holds an object of type T1
          ptr.c = &(this->buffer);
          T1& tmp = *(ptr.ptr);
          tmp = src;
        } else {
          // We create a new object of type T1 by calling the copy constructor
          this->template set_uninitialised<T1>();
          void* p = reinterpret_cast<void*>(&(this->buffer));
          // the magic of placement new...
          new (p) T1(src);
        }
      }
      //! assignement operator.
      /*
       * \param  const T1&, the value affected to the GenType.
       * \return GenTypeBase&, a reference to this.
       * \pre    T1 must be a type that the GenType can hold.
       */
      template <typename T1>
      TFEL_INLINE
          typename std::enable_if<tfel::meta::TLCountNbrOfT<T1, List>::value ==
                                      1,
                                  GenTypeBase&>::type
          operator=(const T1& src) {
        this->template set<T1>(src);
        return *this;
      }
      template <typename T1>
      typename std::enable_if<tfel::meta::TLCountNbrOfT<T1, List>::value == 1,
                              bool>::type
      is() const {
        using namespace tfel::meta;
        return this->index == TLPosition<T1, List>::value;
      }  // end of Value::is
      //! get the value contained in the GenType (const version).
      /*
       * \return const T1&, the value affected to the GenType.
       * \pre    T1 must be a type that the GenType can hold.
       * \throw  GenTypeCastError, if the type contained in the GenType does
       * not match.
       */
      template <typename T1>
      TFEL_INLINE
          typename std::enable_if<tfel::meta::TLCountNbrOfT<T1, List>::value ==
                                      1,
                                  const T1&>::type
          get(void) const {
        // a silly trick to avoir a gcc warning
        union {
          const storage_t* c;
          const T1* ptr;
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
      TFEL_INLINE typename std::
          enable_if<tfel::meta::TLCountNbrOfT<T1, List>::value == 1, T1&>::type
          get(void) {
        using namespace std;
        using namespace tfel::meta;
        using namespace tfel::utilities::internals;
        // a silly trick to avoir a gcc warning
        union {
          storage_t* c;
          T1* ptr;
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
      TFEL_INLINE unsigned short getTypeIndex(void) const {
        return this->index;
      }
      /*
       * Destructor
       */
      ~GenTypeBase() {
        using namespace tfel::meta;
        if (this->index != TLSize<List>::value) {
          // This calls the destructor of the type hold
          void* tmp = reinterpret_cast<void*>(&buffer);
          (*(GenTypeBase::methods.destroyMethod[this->index]))(tmp);
        }
      }
      //! number of object that the GenType can hold
      static constexpr unsigned short ListSize =
          tfel::meta::TLSize<List>::value;
      //! a simple alias.
      typedef internals::GenTypeRunTimeMethods<ListSize, List> RunTimeMethods;

     protected:
      //! RunTimeMethods used by the GenType.
      static const RunTimeMethods methods;
      //! memory where objects are holded
      //! set the value of the GenType.
      /*
       * \param const T1&, the value affected to the GenType.
       * \pre   T1 must be a type that the GenType can hold.
       */
      template <typename T1>
      TFEL_INLINE typename std::
          enable_if<tfel::meta::TLCountNbrOfT<T1, List>::value == 1, void>::type
          set_uninitialised(void) {
        using namespace tfel::meta;
        if (this->index != TLSize<List>::value) {
          // destroy the current object
          void* tmp = reinterpret_cast<void*>(&buffer);
          (*(GenTypeBase::methods.destroyMethod[this->index]))(tmp);
        }
        this->index = TLFindEltPos<T1, List>::value;
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

    //! RunTimeMethods used by the GenType.
    template <typename List>
    const typename GenTypeBase<List>::RunTimeMethods GenTypeBase<List>::methods;

  }  // end of namespace utilities

}  // end of namespace tfel
