/*!
 * \file   TypeList.hxx
 * \brief  This file declares some tools to work with TypeList.
 * \author Helfer Thomas
 * \date   30 August 2006
 */

#ifndef _LIB_TFEL_TYPELIST_H_
#define _LIB_TFEL_TYPELIST_H_ 

#include <string>

#include "Utilities/Name.hxx"

namespace tfel{
  
  namespace meta{
    
    /*! 
     *\brief An empty class used as parent for all typelist.
     * The idea of this class was taken from 
     * Andrei Alexandrescu works
     * \latexonly
     * \cite{alexandrescu01:_c_desig,alexandrescu02}
     * \endlatexonly
     * \htmlonly
     * (http://www.ddj.com/dept/cpp/184403813)
     * \endhtmlonly.
     *
     * \author Helfer Thomas
     * \date   30 August 2006
     */
    struct TypeList{};

    /*!
     * \class TypeListNode
     * \brief Nodes of TypeList.
     * TypeLists are simply linked list of types. This class describe its nodes.
     * \author Helfer Thomas
     * \date   30 August 2006
     */
    template<typename T, typename U>
    struct TypeListNode;

    /*!
     * \class  TypeListEndType
     * \brief  Type used to end a TypeList.
     * \author Helfer Thomas
     * \date   30 August 2006
     */
    struct TypeListEndType;

    /*!
     * \class TypeListTransform
     * \brief Metafunction which transform a TypeList
     * into another by applying the Transform metafunction 
     * to each type in the TypeList.
     * 
     * \param Tlist, a TypeList.
     * \param Transform, a metafunction taking a type and returning a type.
     *
     * \return type, the TypeList resulting from the transformation.
     *
     * \author Helfer Thomas
     * \date   30 August 2006
     */
    template<typename Tlist, template<typename> class Transform>
    struct TypeListTransform;

    /*!
     * \class TypeListSize
     * \brief A metafunction which counts the number of elements in a TypeList. 
     * 
     * \param T, a TypeList
     * \return value, the number of elements in T.
     *
     * \author Helfer Thomas
     * \date   30 August 2006
     */
    template<typename T>
    struct TypeListSize;

    /*!
     * \class TypeListCountNbrOfT
     * \brief A metafunction which counts the number of 
     * times T appears in a TypeList. 
     * 
     * \param T, a type.
     * \param U, a TypeList.
     * \return value, the number of times T appears in U.
     *
     * \author Helfer Thomas
     * \date   30 August 2006
     */
    template<typename T, typename U>
    struct TypeListCountNbrOfT;
    
    /*!
     * \class TypeListFindEltPos
     * \brief A metafunction which returns the position of the 
     * first appearance of a type in a TypeList. 
     * 
     * \param T, a type.
     * \param List, a TypeList.
     * \return value, the position of T in List.
     *
     * \author Helfer Thomas
     * \date   30 August 2006
     */
    template<typename T, typename List>
    struct TypeListFindEltPos;

    /*!
     * \class TypeListPrepend
     * \brief A metafunction which returns a new typelist by adding 
     * an element at the beginnig of a typelist. 
     * \param List, a TypeList.
     * \param T, a type.
     * \return type, a new typelist.
     * \author Helfer Thomas
     * \date   09 September 2006
     */
    template<typename List,typename T>
    struct TypeListPrepend;

    /*!
     * \class TypeListAppend
     * \brief A metafunction which returns a new typelist by adding
     * an element at the end of a typelist. 
     * \param List, a TypeList.
     * \param T, a type.
     * \return type, a new typelist.
     * \author Helfer Thomas
     * \date   09 September 2006
     */
    template<typename List,typename T>
    struct TypeListAppend;

    /*!
     * \class TypeListConcatenate
     * \brief A metafunction which returns the concatenation of two TypeLists
     * 
     * \param  typename First,  a TypeList. 
     * \param  typename Second, a TypeList. 
     * \return type, the resulting TypeList.
     *
     * \author Helfer Thomas
     * \date   15 Sept. 2006
     */
    template<typename First,typename Second>
    struct TypeListConcatenate;    

    /*!
     * \class TypeListFindNthElt
     * \brief A metafunction which returns the Nth
     * element of the TypeList.
     * 
     * \param T, a TypeList.
     * \param N, a positive integer. 
     * \return type, the Nth element of T or TypeListEndType 
     * if T contains less than N elements.
     *
     * \author Helfer Thomas
     * \date   30 August 2006
     */
    template<typename T, unsigned int N>
    struct TypeListFindNthElt;
    
    /*!
     * \class TypeListRemoveNthFirstElt
     * \brief A metafunction which remove the first Nth
     * elements of the TypeList.
     * 
     * \param T, a TypeList.
     * \param N, a positive integer. 
     * \return type, a TypeList or TypeListEndType 
     * if T contains less than N elements.
     *
     * \author Helfer Thomas
     * \date   30 August 2006
     */
    template<typename T, unsigned int N>
    struct TypeListRemoveNthFirstElt;

    /*!
     * \class TypeListElementIsUnique
     * \brief A metafunction which returns true if a 
     * type appears one and only one time in a TypeList.
     * 
     * \param T, a type.
     * \param U, a TypeList. 
     * \return cond, true if T  appears one and only one time in U.
     *
     * \author Helfer Thomas
     * \date   30 August 2006
     */
    template<typename T,typename U>
    struct TypeListElementIsUnique;

    /*!
     * \class TypeListElementsAreUnique
     * \brief A metafunction which returns true if all elements 
     * in a TypeList are unique.
     * 
     * \param T, a TypeList. 
     * \return cond, true if all elements in T are unique
     *
     * \author Helfer Thomas
     * \date   30 August 2006
     */
    template<typename T>
    struct TypeListElementsAreUnique;    

    /*!
     * \class TypeListExtractSubClassesOf
     * \brief A metafunction which returns a new typelist from a
     * typelist.  All types in the new typelist inherit from the 
     * type given in argument.
     * \param List, a TypeList.
     * \param BaseType, a type.
     * \return type, a new typelist.
     * \author Helfer Thomas
     * \date   09 September 2006
     */
    template<typename List,typename BaseType>
    struct TypeListExtractSubClassesOf;
    
    /*!
     * \class TypeListUnique
     * \brief A metafunction which returns a new typelist from a
     * typelist where all elements are unique.
     *
     * \param List, a TypeList.
     * \return type, a new typelist.
     * \author Helfer Thomas
     * \date   08 Januar 2006
     */
    template<typename List>
    struct TypeListUnique;

    /*!
     * \class TypeListGetNames
     * \brief A metafunction which returns the name of each element.
     *
     * \param typename List, a TypeList.
     * 
     * \return std::string, the name of each element.
     * \author Helfer Thomas
     * \date   08 Januar 2006
     */
    template<typename List>
    class TypeListGetNames;

    /*!
     * \class TypeListContainsInvalidType
     * \brief A metafunction which returns true if the list
     * contains InvalidType.
     *
     * \param typename List, a TypeList.
     * 
     * \return const bool.
     * \author Helfer Thomas
     * \date   10 March 2007
     */
    template<typename List>
    class TypeListContainsInvalidType;

    //! Computes the maximum size of all types in a TypeList.
    /*
     * \class TypeListMaxSize.
     * \param  List, the list of types evaluated.
     * \return cons size_t value, the result.
     * The size is given by the sizeof operator.
     * The minimum size is 1.
     * \author Helfer Thomas.
     * \date   22 Apr. 2007.
     */
    template<typename List> 
    class TypeListMaxSize;

    /*
     * \class TypeListComputeAlignBound.
     * Extracts from a type list all the types which have a size
     * inferior than a given value.
     * \param List, a TypeList.
     * \param size_t, the thresold value.
     * \return type, a result under the form of a TypeList.
     * \author Helfer Thomas.
     * \date   22 Apr. 2007.
     */
    template<typename List,size_t size>
    class TypeListComputeAlignBound;

  } // end of namespace meta

} // end of namespace tfel

#include"Metaprogramming/TypeList.ixx"
#include"Metaprogramming/GenerateTypeList.hxx"

#endif /* _LIB_TFEL_TYPELIST_H_ */

