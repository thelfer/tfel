/*!
 * \file   include/TFEL/Utilities/GenTypeBase.hxx
 * \brief  This file implements the GenTypeBase class and some 
 * internal classes used by the GenType class.
 * \see    TL
 * \author Helfer Thomas
 * \date   19 Apr. 2007
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_GENTYPEBASE_H_
#define _LIB_TFEL_GENTYPEBASE_H_ 

#include<cmath>
#include<limits>

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Metaprogramming/StaticAssert.hxx"
#include"TFEL/Metaprogramming/TypeList.hxx"
#include"TFEL/Metaprogramming/GenerateTypeList.hxx"
#include"TFEL/Metaprogramming/EnableIf.hxx"
#include"TFEL/Utilities/RecursiveUnion.hxx"
#include"TFEL/Utilities/GenTypeCastError.hxx"

namespace tfel{
  
  namespace utilities{
    
    namespace internals{
      
      /*
       * \class  Structify
       * \brief  A class whose role is obscur.
       * \param  U, a type.
       * \see    http://www.ddj.com/dept/cpp/184403828 for details.
       * \author Helfer Thomas
       * \date   20 Apr. 2007
       */
      template<typename U> 
      struct TFEL_VISIBILITY_LOCAL Structify
      { 
	// a structure
	U dummy_;
      };

      //! a simple alias
      typedef tfel::meta::TLE TLEnd;

      //! a usefull list of types for computing alignements
      typedef tfel::meta::GenerateTypeList<char,short int,int,long int,float,double,long double,
					   char*,short int*,int*,long int*,float*,double*,
					   long double*,void*,TLEnd (*)(TLEnd),
					   TLEnd* TLEnd::*,TLEnd (TLEnd::*)(TLEnd),
					   Structify<char>,Structify<short int>,
					   Structify<int>,Structify<long int>,
					   Structify<float>,Structify<double>,
					   Structify<long double>,Structify<char*>,
					   Structify<short int*>,Structify<int*>,
					   Structify<long int*>,Structify<float*>,
					   Structify<double*>,Structify<long double*>,
					   Structify<void*>,Structify<TLEnd (*)(TLEnd)>,
					   Structify<TLEnd* TLEnd::*>,
					   Structify<TLEnd (TLEnd::*)(TLEnd)> >::type 
                                           TypesOfAllAlignments;
      
      /*
       * \class  AlignedPOD
       * \author Helfer Thomas
       * \date   20 Apr. 2007
       * \param  TList, a list of types.
       * \return type, a properly aligned type.
       * a class which aims to provide the correct alignement for a
       * memory zone which may contain any of the types in the
       * typelist given in parameter.
       * \see    http://www.ddj.com/dept/cpp/184403828 for details.
       * \author Helfer Thomas.
       * \date   20 Apr. 2007.
       */
      template<typename TList>
      class AlignedPOD
      {
	//! maximum of all the sizes of the list.
	static const size_t size = tfel::meta::TLMaxSize<TList>::value;
	//! a simple alias.
	typedef tfel::meta::TLComputeAlignBound<TypesOfAllAlignments,size> CABounds;
	//! a list of types whose size may serve to compute the alignement.
	typedef typename CABounds::type AlignedTypes;
      public:
	//! the result.
	typedef RecursiveUnion<AlignedTypes> type;
      };

      //! a simple trait class for some minimalistic optimisation.
      /*
       * \see GenTypePODDestroy, for an example of such optimisation.
       * \author Helfer Thomas.
       * \date   20 Apr. 2007.
       */
      template<typename T>
      struct TFEL_VISIBILITY_LOCAL GenTypeTraits
      {
	/*
	 * a boolean saying if the type in argument is a plain old
	 * data type.
	 */ 
	static const bool isFundamentalType = false;
      };

      //! a helper function for destroying a plain old data type.
      /*
       * \author Helfer Thomas.
       * \date   20 Apr. 2007.
       */
      template<typename T>
      struct TFEL_VISIBILITY_LOCAL GenTypePODDestroy
      {
	//! the helper function.
	/*
	 * \param void *const, a pointer (not used).
	 */
	static void
	exe(void *const)
	{}
      };

      //! an helper function to call the destructor of T.
      /*
       * \author Helfer Thomas.
       * \date   20 Apr. 2007.
       */
      template<typename T>
      struct TFEL_VISIBILITY_LOCAL GenTypeGenericDestroy
      {
	//! the helper function.
	/*
	 * \param void *const, a pointer to a T object.
	 */
	static void
	exe(void *const p)
	{
	  T& tmp = *(static_cast<T *>(p)); 
	  tmp.~T();
	}
      };

      //! an helper function to call the assignement operator of T.
      /*
       * \author Helfer Thomas.
       * \date   20 Apr. 2007.
       */
      template<typename T>
      struct TFEL_VISIBILITY_LOCAL GenTypeAssign
      {
	//! the helper function.
	/*
	 * \param void *const, a pointer to a T-object.
	 * \param const void *const, a pointer to a T-object.
	 * \return const bool, the result of the comparison.
	 */
	static void
	exe(void *const p,const void *const p2)
	{
	  T& tmp = *(static_cast<T *>(p)); 
	  const T& tmp2 = *(static_cast<const T *>(p2)); 
	  tmp = tmp2;
	}
      };

      //! an helper function to call the copy constructor of T.
      /*
       * \author Helfer Thomas.
       * \date   20 Apr. 2007.
       */
      template<typename T>
      struct TFEL_VISIBILITY_LOCAL GenTypeCopy
      {
	//! the helper function.
	/*
	 * \param void *const, a pointer to a T-object.
	 * \param const void *const, a pointer to a T-object.
	 */
	static void
	exe(void *const p,const void *const p2)
	{
	  const T& tmp2 = *(static_cast<const T *>(p2)); 
	  // placement new
	  new (p) T(tmp2);
	}
      };
      
      //! An helper class to fill the runtime methods of a GenType.
      /*
       * \param unsigned short N, the current index.
       * \param typename List, a list of types.
       * \author Helfer Thomas.
       * \date   20 Apr. 2007.
       */
      template<unsigned short N,typename List>
      struct TFEL_VISIBILITY_LOCAL GenTypeRunTimeMethods
	: public GenTypeRunTimeMethods<N-1,List>
      {
	//! Default constructor
	/*
	 * This constructor fills the arrays of function pointers
	 * associated with the N-1 type in the typelist.
	 */
	GenTypeRunTimeMethods()
	{
	  using namespace tfel::utilities;
	  using namespace tfel::meta;
	  this->destroyMethod[N-1] = &GenTypeDestroy::exe;
	  this->copyMethod[N-1]    = &GenTypeCopy<Current>::exe;
	  this->assignMethod[N-1]  = &GenTypeAssign<Current>::exe;
	}
      private:
	//! a simple alias.
	typedef typename tfel::meta::TLFindNthElt<List,N-1>::type Current;
	//! choose the function used to destroy the N-1 type of the typelist.
	typedef typename tfel::meta::IF<GenTypeTraits<Current>::isFundamentalType,
					GenTypePODDestroy<Current>,
					GenTypeGenericDestroy<Current> >::type GenTypeDestroy; 
      };

      //! A partial specialisation to end the recursion.
      /*
       * \author Helfer Thomas.
       * \date   20 Apr. 2007.
       */
      template<typename List>
      struct TFEL_VISIBILITY_LOCAL GenTypeRunTimeMethods<0u,List>
      {
	//! a simple alias.
	typedef void (*DestroyMethod)(void *const);
	//! a simple alias.
	typedef void (*AssignMethod)(void *const,const void* const);
	//! a simple alias.
	typedef void (*CopyMethod)(void *const,const void* const);
	//! array containing function pointers to call a specific destructor.
	DestroyMethod destroyMethod[tfel::meta::TLSize<List>::value];
	//! array containing function pointers to call a specific assignement operator.
	AssignMethod  assignMethod[tfel::meta::TLSize<List>::value];
	//! array containing function pointers to call a specific copy constructor.
	CopyMethod copyMethod[tfel::meta::TLSize<List>::value];
      };

      //! an helper class to a add methods to a GenType for a specific type.
      /*
       * This class is based on the curiously recurring template
       * pattern for achieving this.
       * \param typename Child, the Child of this class.
       * \param typename T, the specific type.
       * \see GenTypeSpecialisation.ixx for examples.
       * \see http://en.wikipedia.org/wiki/Curiously_Recurring_Template_Pattern
       * \author Helfer Thomas.
       * \date   20 Apr. 2007.
       */
      template<typename Child,typename T>
      class GenTypeSpecializedAccessor
      {};

      //! an helper class to a add methods to a GenType.
      /*
       * This class is based on the curiously recurring template
       * pattern for achieving this.
       * \param typename Child, the Child of this class.
       * \param typename List, the List of types contained in the GenType.
       * \see GenTypeSpecialisation.ixx for examples.
       * \see http://en.wikipedia.org/wiki/Curiously_Recurring_Template_Pattern
       * \author Helfer Thomas.
       * \date   20 Apr. 2007.
       */
      template<typename Child,typename List>
      class GenTypeSpecializedMethods
#ifndef DOXYGEN_SHOULD_SKIP_THIS
	: public GenTypeSpecializedMethods<Child,typename List::Next>,
	  public GenTypeSpecializedAccessor<Child,typename List::Current>
#else /* DOXYGEN_SHOULD_SKIP_THIS */
	: public GenTypeSpecializedAccessor<Child,typename List::Current>
#endif
      {
	//! a simple alias.
	typedef typename List::Current Current;
      public:
	//! cast operator.
	TFEL_INLINE
	operator Current&()
	{
	  return static_cast<Child *>(this)->template get<Current>();
	} 
	//! cast operator (const version).
	TFEL_INLINE
	operator const Current&() const
	{
	  return static_cast<const Child *>(this)->template get<Current>();
	}
      };
      
      //! a partial specialisation to end the recursion.
      /*
       * \author Helfer Thomas.
       * \date   20 Apr. 2007.
       */
      template<typename Child>
      class GenTypeSpecializedMethods<Child,tfel::meta::TLE>
      {};

    } // end of namespace internals

    //! The base class of GenType.
    /*
     * \param  typename List, the list of types contained in the GenType.
     * \author Helfer Thomas.
     * \date   20 Apr. 2007.
     */
    template<typename List>
    struct GenTypeBase
      : public tfel::utilities::internals::GenTypeSpecializedMethods<GenTypeBase<List>,List>
    {
      //! Default constructor.
      TFEL_INLINE
      GenTypeBase()
	: index(tfel::meta::TLSize<List>::value)
      {}
      template<typename T1>
      TFEL_INLINE
      GenTypeBase(const T1& v,
		  typename tfel::meta::EnableIf<
		  tfel::meta::TLCountNbrOfT<T1,List>::value==1, 
		  bool>::type = true)
	: index(tfel::meta::TLSize<List>::value)
      {
	this->template set<T1>(v);
      }
      //! Copy constructor.
      /*
       * \param const GenTypeBase&, the source.
       */
      TFEL_INLINE
      GenTypeBase(const GenTypeBase& src)
	: index(src.index)
      {
	using namespace tfel::meta;
	if(this->index!=TLSize<List>::value){
	  // create a new object by copy
	  void  * tmp       = reinterpret_cast<void*>(&(this->container.buffer[0]));
	  const void * tmp2 = reinterpret_cast<const void*>(&(src.container.buffer[0]));
	  (*(GenTypeBase::methods.copyMethod[this->index]))(tmp,tmp2);
	}
      }
      //! The assignement operator.
      /*
       * \param const GenTypeBase&, the right-hand side.
       */
      TFEL_INLINE
      GenTypeBase&
      operator=(const GenTypeBase& src)
      {
	using namespace tfel::meta;
	// check for self-assignement
	if(this!=&src){
	  if(this->index==src.index){
	    if(src.index!=TLSize<List>::value){
	      // the two GenTypes hold the same objects
	      // we then use the assignement operator.
	      void  * tmp       = reinterpret_cast<void*>(&(this->container.buffer[0]));
	      const void * tmp2 = reinterpret_cast<const void*>(&(src.container.buffer[0]));
	      (*(GenTypeBase::methods.assignMethod[this->index]))(tmp,tmp2);
	    } else {
	      // src is not initialized,
	      // we then destroy holded object
	      if(this->index!=TLSize<List>::value){
		void * tmp = reinterpret_cast<void*>(&(this->container.buffer[0]));
		(*(GenTypeBase::methods.destroyMethod[this->index]))(tmp);
	      }
	      this->index = TLSize<List>::value;
	    }
	  } else {
	    // the two GenType have two differents objects.
	    // we first destroy the previous object and create a new
	    // one by copy.
	    if(this->index!=TLSize<List>::value){
	      void * tmp = reinterpret_cast<void*>(&(this->container.buffer[0]));
	      (*(GenTypeBase::methods.destroyMethod[this->index]))(tmp);
	    }
	    this->index = src.index;
	    if(this->index!=TLSize<List>::value){
	      void  * tmp       = reinterpret_cast<void*>(&(this->container.buffer[0]));
	      const void * tmp2 = reinterpret_cast<const void*>(&(src.container.buffer[0]));
	      (*(GenTypeBase::methods.copyMethod[this->index]))(tmp,tmp2);
	    }
	  }
	}
	return *this;
      }
      //! copy a GenType (calls the assignement operator).
      /*
       * \param const GenTypeBase&, the object to which it is compared.
       */
      bool 
      copy(const GenTypeBase& src)
      {
	using namespace tfel::meta;
	this->operator=(src);
	return true;
      }
      //! set the value of the GenType.
      /*
       * \param const T1&, the value affected to the GenType.
       * \pre   T1 must be a type that the GenType can hold.
       */
      template<typename T1>
      TFEL_INLINE
      typename tfel::meta::EnableIf<
	tfel::meta::TLCountNbrOfT<T1,List>::value==1, 
	void >::type 
      set(const T1& src)
      {
	using namespace tfel::meta;
	using namespace tfel::utilities::internals;
	if(this->index==TLFindEltPos<T1,List>::value){
	  // a silly trick to avoir a gcc warning
	  union{
	    unsigned char *c;
	    T1 *ptr;
	  } ptr;
	  // The GenType already holds an object of type T1
	  ptr.c = this->container.buffer;
	  T1& tmp  = *(ptr.ptr);
	  tmp = src;
	} else {
	  // We create a new object of type T1 by calling the copy constructor
	  this->template set_uninitialised<T1>();
	  void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
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
      template<typename T1>
      TFEL_INLINE
      typename tfel::meta::EnableIf<
	tfel::meta::TLCountNbrOfT<T1,List>::value==1, 
	GenTypeBase&>::type 
      operator=(const T1& src)
      {
	this->template set<T1>(src);
	return *this;
      }
      template<typename T1>
      typename tfel::meta::EnableIf<tfel::meta::TLCountNbrOfT<T1,List>::value==1,
				    bool>::type
      is() const
      {
	using namespace tfel::meta;
	return this->index==TLPosition<T1,List>::value;
      } // end of Value::is
      //! get the value contained in the GenType (const version).
      /*
       * \return const T1&, the value affected to the GenType.
       * \pre    T1 must be a type that the GenType can hold.
       * \throw  GenTypeCastError, if the type contained in the GenType doesnot match.
       */
      template<typename T1>
      TFEL_INLINE
      typename tfel::meta::EnableIf<
	tfel::meta::TLCountNbrOfT<T1,List>::value==1, 
	const T1& >::type 
      get(void) const
      {	
	using namespace std;
	using namespace tfel::meta;
	using namespace tfel::utilities::internals;
	// a silly trick to avoir a gcc warning
	union{
	  const unsigned char *c;
	  const T1 *ptr;
	} ptr;
	if(this->index!=TLFindEltPos<T1,List>::value){
	  throw(GenTypeCastError());
	}
	ptr.c = this->container.buffer;
	return *(ptr.ptr);
      }
      //! get the value contained in the GenType.
      /*
       * \return T1&, the value affected to the GenType.
       * \pre    T1 must be a type that the GenType can hold.
       * \throw  GenTypeCastError, if the type contained in the GenType does not match.
       */
      template<typename T1>
      TFEL_INLINE 
      typename tfel::meta::EnableIf<
	tfel::meta::TLCountNbrOfT<T1,List>::value==1, 
	T1&>::type 
      get(void)
      {
	using namespace std;
	using namespace tfel::meta;
	using namespace tfel::utilities::internals;
	// a silly trick to avoir a gcc warning
	union{
	  unsigned char *c;
	  T1 *ptr;
	} ptr;
	if(this->index!=TLFindEltPos<T1,List>::value){
	  throw(GenTypeCastError());
	}
	ptr.c = this->container.buffer;
	return *(ptr.ptr);
      }
      /*!
       * \return the type index of the object 
       */
      TFEL_INLINE unsigned short
      getTypeIndex(void) const{
	return this->index;
      }
      /*
       * Destructor
       */
      ~GenTypeBase()
      {
	using namespace tfel::meta;
	if(this->index!=TLSize<List>::value){
	  //This calls the destructor of the type hold
	  void * tmp = reinterpret_cast<void*>(&(this->container.buffer[0]));
	  (*(GenTypeBase::methods.destroyMethod[this->index]))(tmp);
	}
      }
      //! number of object that the GenType can hold
      static const unsigned short ListSize = tfel::meta::TLSize<List>::value;
      //! a simple alias.
      typedef tfel::utilities::internals::GenTypeRunTimeMethods<ListSize,List> RunTimeMethods;
    protected:
      //! RunTimeMethods used by the GenType.
      static const RunTimeMethods methods;
      //! maximum size of the types hold by the GenType.
      static const size_t size = tfel::meta::TLMaxSize<List>::value;
      //! a type properly aligned.
      typedef typename tfel::utilities::internals::AlignedPOD<List>::type Align;
      //! memory where objects are holded
      //! set the value of the GenType.
      /*
       * \param const T1&, the value affected to the GenType.
       * \pre   T1 must be a type that the GenType can hold.
       */
      template<typename T1>
      TFEL_INLINE
      typename tfel::meta::EnableIf<
	tfel::meta::TLCountNbrOfT<T1,List>::value==1, 
	void >::type 
      set_uninitialised(void)
      {
	using namespace tfel::meta;
	if(this->index!=TLSize<List>::value){
	  // destroy the current object
	  void * tmp = reinterpret_cast<void*>(&(this->container.buffer[0]));
	  (*(GenTypeBase::methods.destroyMethod[this->index]))(tmp);
	}
	this->index=TLFindEltPos<T1,List>::value;
      }
      union 
      {
	//! the buffer where objects are hold.
	unsigned char buffer[size];
	//! Align is here to ensure correct alignement of the memory.
	//  see for http://www.ddj.com/dept/cpp/184403821 and 
	//  http://www.ddj.com/dept/cpp/184403828 
	//  for more details...
	Align dummy;
      } container;
      //! index to the current type hold by the GenType.
      unsigned short index;
    private:
      //! a simple assertion
      TFEL_STATIC_ASSERT((tfel::meta::TLElementsAreUnique<List>::cond));
    };

    //! Declaration of GenTypeBase static member RunTimeMethod
    template<typename List>
    const typename GenTypeBase<List>::RunTimeMethods GenTypeBase<List>::methods;

    /*!
     * Apply function T::apply to a GenTypeBase for the type holded by the
     * GenTypeBase.
     *
     * \param T    : class containing the function to be applied.
     * \param List : list of types contained in the GenTypeBase class.
     * \param v    : GenTypeBase class argument.
     *
     * \pre class T must contain a static function name 'apply' for
     * each type in List.
     * \pre class T must define 'return_type' as a class or a typedef.
     *
     * \throw a GenTypeCastError if the GenTypeBase does not hold any
     * object.
     */
    template<typename T,
	     typename List>
    typename T::return_type
    apply(const GenTypeBase<List>&);

    /*!
     * Apply functor T to a GenTypeBase for the type holded by the
     * GenTypeBase.
     *
     * \param T    : class containing the function to be applied.
     * \param List : list of types contained in the GenTypeBase class.
     * \param f    : functor.
     * \param v    : GenTypeBase class argument.
     *
     * \pre class T must contain the opertor () for each type in List.
     * \pre class T must define 'return_type' as a class or a typedef.
     *
     * \throw a GenTypeCastError if the GenTypeBase does not hold any
     * object.
     */
    template<typename T,
	     typename List>
    typename T::return_type
    apply(T&,const GenTypeBase<List>&);

    /*!
     * Apply function T::apply to a GenTypeBase for the types holded
     * by the two GenTypeBase.
     *
     * \param T    : class containing the function to be applied.
     * \param List : list of types contained in the GenTypeBase class.
     * \param v1   : GenTypeBase class argument.
     * \param v2   : GenTypeBase class argument.
     *
     * \pre class T must contain a static function name 'apply' for
     * each pair of types in List.
     * \pre class T must define 'return_type' as a class or a typedef.
     *
     * \throw a GenTypeCastError if one of the GenTypeBase does not
     * hold any object.
     */
    template<typename T,
	     typename List>
    typename T::return_type
    apply(const GenTypeBase<List>&,
	  const GenTypeBase<List>&);

    /*!
     * Apply functor T to a GenTypeBase for the types holded by the
     * two GenTypeBase.
     *
     * \param T    : class containing the function to be applied.
     * \param List : list of types contained in the GenTypeBase class.
     * \param f    : functor.
     * \param v1   : GenTypeBase class argument.
     * \param v2   : GenTypeBase class argument.
     *
     * \pre class T must contain the opertor () for each pair of types
     * in List.
     * \pre class T must define 'return_type' as a class or a typedef.
     *
     * \throw a GenTypeCastError if the GenTypeBase does not hold any
     * object.
     */
    template<typename T,
	     typename List>
    typename T::return_type
    apply(T&,const GenTypeBase<List>&,
	  const GenTypeBase<List>&);

    /*!
     * Apply function T::apply to a GenTypeBase for the type holded by the
     * GenTypeBase.
     *
     * \param T    : class containing the function to be applied.
     * \param List : list of types contained in the GenTypeBase class.
     * \param v    : GenTypeBase class argument.
     *
     * \pre class T must contain a static function name 'apply' for
     * each type in List.
     * \pre class T must define 'return_type' as a class or a typedef.
     *
     * \throw a GenTypeCastError if the GenTypeBase does not hold any
     * object.
     */
    template<typename T,
	     typename List>
    typename T::return_type
    apply(GenTypeBase<List>&);

    /*!
     * Apply functor T to a GenTypeBase for the type holded by the
     * GenTypeBase.
     *
     * \param T    : class containing the function to be applied.
     * \param List : list of types contained in the GenTypeBase class.
     * \param f    : functor.
     * \param v    : GenTypeBase class argument.
     *
     * \pre class T must contain the opertor () for each type in List.
     * \pre class T must define 'return_type' as a class or a typedef.
     *
     * \throw a GenTypeCastError if the GenTypeBase does not hold any
     * object.
     */
    template<typename T,
	     typename List>
    typename T::return_type
    apply(T&,GenTypeBase<List>&);

    /*!
     * Apply function T::apply to a GenTypeBase for the types holded
     * by the two GenTypeBase.
     *
     * \param T    : class containing the function to be applied.
     * \param List : list of types contained in the GenTypeBase class.
     * \param v1   : GenTypeBase class argument.
     * \param v2   : GenTypeBase class argument.
     *
     * \pre class T must contain a static function name 'apply' for
     * each pair of types in List.
     * \pre class T must define 'return_type' as a class or a typedef.
     *
     * \throw a GenTypeCastError if one of the GenTypeBase does not
     * hold any object.
     */
    template<typename T,
	     typename List>
    typename T::return_type
    apply(GenTypeBase<List>&,
	  GenTypeBase<List>&);

    /*!
     * Apply functor T to a GenTypeBase for the types holded by the
     * two GenTypeBase.
     *
     * \param T    : class containing the function to be applied.
     * \param List : list of types contained in the GenTypeBase class.
     * \param f    : functor.
     * \param v1   : GenTypeBase class argument.
     * \param v2   : GenTypeBase class argument.
     *
     * \pre class T must contain the opertor () for each pair of types
     * in List.
     * \pre class T must define 'return_type' as a class or a typedef.
     *
     * \throw a GenTypeCastError if the GenTypeBase does not hold any
     * object.
     */
    template<typename T,
	     typename List>
    typename T::return_type
    apply(T&,GenTypeBase<List>&,
	  GenTypeBase<List>&);

  } // end of namespace utilities

} // end of namespace tfel

#include"TFEL/Utilities/GenTypeBase.ixx"
#include"TFEL/Utilities/GenTypeSpecialisation.ixx"

#endif /* _LIB_TFEL_GENTYPEBASE_H_ */
