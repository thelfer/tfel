/*!
 * \file   TypeList.ixx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   07 Aug 2006
 */

#ifndef _LIB_TFEL_TYPELIST_I_
#define _LIB_TFEL_TYPELIST_I_ 

#include<string>

#include"TFEL/Utilities/Name.hxx"
#include"TFEL/Metaprogramming/IF.hxx"
#include"TFEL/Metaprogramming/StaticAssert.hxx"
#include"TFEL/Metaprogramming/IsSubClassOf.hxx"
#include"TFEL/TypeTraits/IsInvalid.hxx"

namespace tfel{
  
  namespace meta{

    template<typename T, typename U>
    struct TypeListNode
      : public TypeList
    {
      TFEL_STATIC_ASSERT((IsSubClassOf<U,TypeList>::cond));
      /*!
       * Return the Name of the TypeList beginning with this Node.
       * \param void.
       * \return std::string, the Name of the TypeList.
       */
      static
      const std::string
      getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("TypeListNode<")
	  +Name<T>::getName()+string(",")
	  +Name<U>::getName()+string(">");
      }
      /*!
       * Current element.
       */
      typedef T Current;
      /*!
       * Next Node.
       */
      typedef U Next;
    };
    
    struct TypeListEndType
      : public TypeList
    {
      /*
       * Return the Name of this class.
       * \param void.
       * \return std::string, the Name.
       */
      static
      const std::string
      getName(void){
	return std::string("TypeListEndType");
      }
    };

    namespace internals {

      template<typename List>
      class TypeListGetNames_
      {
	typedef typename List::Current Current;
	typedef typename List::Next Next;
      public:
	static const std::string exe(void)
	{
	  using namespace std;
	  using namespace tfel::utilities;
	  return string(",")+Name<Current>::getName()
	    +tfel::meta::internals::TypeListGetNames_<Next>::exe();
	}
      };

      template<>
      struct TypeListGetNames_<TypeListEndType>
      {
	static const std::string exe()
	{
	  return "}";
	}
      };

    } // end of namespace internals

    template<typename List>
    class TypeListGetNames
    {
      typedef typename List::Current Current;
      typedef typename List::Next Next;
    public:
      static
      const std::string
      exe(void)
      {
	using namespace std;
	using namespace tfel::utilities;
	return string("{")+Name<Current>::getName()
	  +tfel::meta::internals::TypeListGetNames_<Next>::exe();
      }
    };

    template<>
    struct TypeListGetNames<TypeListEndType>
    {
      static
      const std::string
      exe(void)
      {
	return "{}";
      }
    };

    template<typename Tlist, template<typename> class Transform>
    struct TypeListTransform
    {
      TFEL_STATIC_ASSERT((IsSubClassOf<Tlist,TypeList>::cond));
      typedef TypeListNode<typename Transform<typename Tlist::Current>::type,
			   typename TypeListTransform<typename Tlist::Next,Transform>::type> type;
    };
    
    /*!
     * Partial Specialisation to end the recursion.
     * \author Helfer Thomas
     * \date   30 August 2006
     */
    template<template<typename> class Transform>
    struct TypeListTransform<TypeListEndType,Transform>
    {
      typedef TypeListEndType type;
    };
    
    template<typename T>
    struct TypeListSize{
      static const unsigned int value = 1+TypeListSize<typename T::Next>::value;
    };

    /*!
     * Partial Specialisation to end the recursion.
     * \author Helfer Thomas
     * \date   30 August 2006
     */
    template<>
    struct TypeListSize<TypeListEndType>{
      static const unsigned int value = 0;
    };
    
    /*!
     * Partial Specialisation to end the recursion.
     * \author Helfer Thomas
     * \date   30 August 2006
     */
    template<typename T>
    struct TypeListCountNbrOfT<T,TypeListEndType>
    {
      static const unsigned int value  = 0;
    };
    
    template<typename T, typename U>
    struct TypeListCountNbrOfT
    {
      static const unsigned int value  = (IsSameType<T,typename U::Current>::cond ? 1 : 0) +  TypeListCountNbrOfT<T,typename U::Next>::value;
    };

    namespace internals{

      /*!
       * \brief  An helper class for the TypeListFindEltPos metafunction.
       * \author Helfer Thomas
       * \date   30 August 2006
       */
      template<typename T, typename List>
      struct TypeListFindEltPos_
      {
	static const unsigned int value = (TypeListCountNbrOfT<T,typename List::Next>::value==1) ? 1+ TypeListFindEltPos_<T,typename List::Next>::value : 0u; 
      };
      
      /*!
       * Partial Specialisation to end the recursion.
       * \author Helfer Thomas
       * \date   30 August 2006
       */
      template<typename T>
      struct TypeListFindEltPos_<T,TypeListEndType>
      {
	static const unsigned int value = 0u;
      };
    
    } // end of namespace internals

    template<typename T, typename List>
    struct TypeListFindEltPos
    {    
      TFEL_STATIC_ASSERT((IsSubClassOf<List,TypeList>::cond));
      TFEL_STATIC_ASSERT((TypeListCountNbrOfT<T,List>::value==1));
      /*!
       * The result of the metafunction. 
       */
      static const unsigned int value = tfel::meta::internals::TypeListFindEltPos_<T,List>::value;
    };

    template<unsigned int N>
    struct TypeListRemoveNthFirstElt<TypeListEndType,N>
    {
      typedef TypeListEndType type;
    };

    template<typename List>
    struct TypeListRemoveNthFirstElt<List,0u>
    {
      TFEL_STATIC_ASSERT((IsSubClassOf<List,TypeList>::cond));
      typedef List type;
    };

    template<>
    struct TypeListRemoveNthFirstElt<TypeListEndType,0u>
    {
      typedef TypeListEndType type;
    };

    template<typename List,unsigned int N>
    struct TypeListRemoveNthFirstElt
    {
      TFEL_STATIC_ASSERT((IsSubClassOf<List,TypeList>::cond));
      typedef typename 
      TypeListRemoveNthFirstElt<typename List::Next,N-1>::type type;
    };

    /*!
     * Partial specialisation of TypeListFindNthElt to end the recursion
     */
    template<unsigned int N>
    struct TypeListFindNthElt<TypeListEndType,N>{
      /*!
       * The result of the metafunction.
       */
      typedef TypeListEndType type;
    };

    /*!
     * Partial specialisation of TypeListFindNthElt to end the recursion
     */
    template<typename T>
    struct TypeListFindNthElt<T,0u>{
      TFEL_STATIC_ASSERT((IsSubClassOf<T,TypeList>::cond));
      /*!
       * The result of the metafunction.
       */
      typedef typename T::Current type;
    };

    /*!
     * Partial specialisation of TypeListFindNthElt to end the recursion
     */
    template<>
    struct TypeListFindNthElt<TypeListEndType,0u>{
      /*!
       * The result of the metafunction.
       */
      typedef TypeListEndType type;
    };
    
    template<typename T,unsigned int N>
    struct TypeListFindNthElt{
      TFEL_STATIC_ASSERT((IsSubClassOf<T,TypeList>::cond));
      /*!
       * The result of the metafunction. Return the Nth element of the T or TypeListEndType if T contains less than N elements.
       */
      typedef typename TypeListFindNthElt<typename T::Next,N-1>::type type;
    };
    
    template<typename T,typename U>
    struct TypeListElementIsUnique {
      /*!
       * The result of the metafunction. True if T is unique in U.
       */
      static const bool cond = TypeListCountNbrOfT<T,U>::value==1;
    };
    
    namespace internals{

      /*!
       * \brief  An helper class for the TypeListElementsAreUnique metafunction.
       * \author Helfer Thomas
       * \date   30 August 2006
       */
      template<typename T,typename U>
      struct TypeListElementsAreUniqueImpl{
	static const bool c1 = tfel::meta::TypeListElementIsUnique<typename T::Current,U>::cond;
	static const bool c2 = TypeListElementsAreUniqueImpl<typename T::Next,U>::cond;
	static const bool cond = c1 && c2;
      };
      
      /*!
       * \brief Partial specialisation of TypeListElementsAreUniqueImpl
       */
      template<typename U>
      struct TypeListElementsAreUniqueImpl<TypeListEndType,U>{
	static const bool cond = true;
      };
      
    } // end of namespace internals

    template<typename T>
    struct TypeListElementsAreUnique {
      /*!
       * The result of the metafunction. True if all elements in T are unique.
       */
      static const bool cond = tfel::meta::internals::TypeListElementsAreUniqueImpl<T,T>::cond;
    };

    template<typename List,typename T>
    struct TypeListPrepend
    {
      TFEL_STATIC_ASSERT((IsSubClassOf<List,TypeList>::cond));
      /*!
       * The result of the metafunction. 
       */
      typedef TypeListNode<T,List> type;
    };

    template<typename T>
    struct TypeListAppend<TypeListEndType,T>
    {
      /*!
       * The result of the metafunction. 
       */
      typedef TypeListNode<T,TypeListEndType> type;
    };

    template<typename List,typename T>
    struct TypeListAppend
    {
      TFEL_STATIC_ASSERT((IsSubClassOf<List,TypeList>::cond));
      /*!
       * The result of the metafunction. 
       */
      typedef TypeListNode<typename List::Current,
			   typename TypeListAppend<typename List::Next,T>::type> type;
    };

    template<typename Second>
    struct TypeListConcatenate<TypeListEndType,Second>
    {
      TFEL_STATIC_ASSERT((IsSubClassOf<Second,TypeList>::cond));
      /*!
       * The result of the metafunction. 
       */
      typedef Second type;
    };    

    template<typename First,typename Second>
    class TypeListConcatenate
    {
      typedef typename TypeListConcatenate<typename First::Next,Second>::type N_;
    public:
      TFEL_STATIC_ASSERT((IsSubClassOf<First,TypeList>::cond));
      TFEL_STATIC_ASSERT((IsSubClassOf<Second,TypeList>::cond));
      /*!
       * The result of the metafunction. 
       */
      typedef TypeListNode<typename First::Current,N_> type;
    };    

    template<typename BaseType>
    struct TypeListExtractSubClassesOf<TypeListEndType,BaseType>
    {
      /*!
       * The result of the metafunction. 
       */
      typedef TypeListEndType type;
    };

    template<typename List,typename BaseType>
    struct TypeListExtractSubClassesOf
    {
      TFEL_STATIC_ASSERT((IsSubClassOf<List,TypeList>::cond));
      /*!
       * The result of the metafunction. 
       */
      typedef typename IF<IsSubClassOf<typename List::Current,BaseType>::cond,TypeListNode<typename List::Current,typename TypeListExtractSubClassesOf<typename List::Next,BaseType>::type>,typename TypeListExtractSubClassesOf<typename List::Next,BaseType>::type>::type type;
    };

    template<>
    struct TypeListUnique<TypeListEndType>
    {
      /*!
       * The result of the metafunction. 
       */
      typedef TypeListEndType type;
    };

    template<typename List>
    class TypeListUnique
    {
      typedef typename List::Next    Next;
      typedef typename List::Current Current;
      typedef typename TypeListUnique<Next>::type UniqueNext;
      typedef typename TypeListPrepend<UniqueNext,Current>::type Choice;
      static const bool unique = TypeListElementIsUnique<Current,List>::cond;
    public:
      /*!
       * The result of the metafunction. 
       */
      typedef typename IF<unique,Choice,UniqueNext>::type type;
    };

    template<>
    struct TypeListContainsInvalidType<TypeListEndType>
    {
      /*!
       * The result of the metafunction. 
       */
      static const bool cond  = false;
    };
    
    template<typename TList>
    class TypeListContainsInvalidType
    {
      //! a simple alias.
      typedef typename TList::Current Current;
      //! a simple alias.
      typedef typename TList::Next Next;
      //! check if the current type is invalid.
      static const bool cond1 = tfel::typetraits::IsInvalid<Current>::cond;
      //! check if all the next types in the TypeLis are invalid.
      static const bool cond2 = TypeListContainsInvalidType<Next>::cond;
    public:
      //! the result.
      static const bool cond  = cond1 || cond2 ;
    };
    
    template<typename List> 
    class TypeListMaxSize
    {
      //! a simple alias
      typedef typename List::Current Current;
      //! a simple alias
      typedef typename List::Next Next;
      //! the result of the algorithm for the next types of the TypeList.
      static const size_t nextValue = TypeListMaxSize<Next>::value;
    public:
      //! the result of the algorithm.
      static const size_t value = sizeof(Current) > nextValue ? sizeof(Current) : nextValue;
    }; // end of struct TypeListMaxSize
    
    //! Partial specialisation to end the recursion.
    /*
     * \return cons size_t value, the result.
     */
    template<> 
    struct TypeListMaxSize<TypeListEndType>
    {
      //! the result
      static const size_t value =  1;
    }; // end of struct TypeListMaxSize
    
    template<typename List,size_t size>
    class TypeListComputeAlignBound
    {
      //! a simple alias
      typedef typename List::Current Current;
      //! a simple alias
      typedef typename List::Next Next;
      //! the result of the algorithm for next types of the TypeList.
      typedef typename TypeListComputeAlignBound<Next,size>::type TailResult;
    public:
      //! the result.
      typedef typename IF<(sizeof(Current) <= size),
	TypeListNode<Current, TailResult>,
	TailResult>::type type;
    };
    
    //! Partial specialisation to end the recursion.
    /*
     * \return type, the result.
     */
    template<size_t size>
    struct TypeListComputeAlignBound<TypeListEndType, size>
    {
      //! the result.
      typedef TypeListEndType type;
    };

    namespace internals{

      template<unsigned short N>
      struct IntegerValue
      {
	static const unsigned short value = N;
      }; // end of struct IntegerValue
      
      template<unsigned short N,
	       typename T,
	       typename List>
      struct TypeListPosition
      {
	typedef typename tfel::meta::IF<tfel::meta::IsSameType<T,typename List::Current>::cond,
					IntegerValue<N>,
					TypeListPosition<N+1,T,typename List::Next> >::type Select;
      static const unsigned short value = Select::value;
      private:
	TFEL_STATIC_ASSERT((IsSubClassOf<List,TypeList>::cond));
      }; // end of TypeListPosition

      template<unsigned short N,
	       typename T>
      struct TypeListPosition<N,T,TypeListEndType>
      {
	static const unsigned short value = N;
      }; // end of TypeListPosition


    } // end of namespace internals

    template<typename T,
	     typename List>
    struct TypeListPosition
    {
      static const unsigned short value = tfel::meta::internals::TypeListPosition<0u,T,List>::value;
    private:
      TFEL_STATIC_ASSERT((IsSubClassOf<List,TypeList>::cond));
    };
    
  } // end of namespace meta

} // end of namespace tfel

#endif /* _LIB_TFEL_TYPELIST_I_ */

