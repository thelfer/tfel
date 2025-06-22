/*!
 * \file   include/TFEL/Metaprogramming/TypeList.hxx
 * \brief  This file declares some tools to work with TL.
 * \author Helfer Thomas
 * \date   30 August 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_TYPELIST_H_
#define LIB_TFEL_TYPELIST_H_

#include <cstddef>
#include "TFEL/Config/TFELConfig.hxx"

namespace tfel {

  namespace meta {

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
    struct TFEL_VISIBILITY_LOCAL TL {};

    /*!
     * \class TLNode
     * \brief Nodes of TL.
     * TLs are simply linked list of types. This class describe its nodes.
     * \author Helfer Thomas
     * \date   30 August 2006
     */
    template <typename T, typename U>
    struct TLNode;

    /*!
     * \class  TLE
     * \brief  Type used to end a TL.
     * \author Helfer Thomas
     * \date   30 August 2006
     */
    struct TLE;

    /*!
     * \class TLTransform
     * \brief Metafunction which transform a TL
     * into another by applying the Transform metafunction
     * to each type in the TL.
     *
     * \param Tlist, a TL.
     * \param Transform, a metafunction taking a type and returning a type.
     *
     * \return type, the TL resulting from the transformation.
     *
     * \author Helfer Thomas
     * \date   30 August 2006
     */
    template <typename Tlist, template <typename> class Transform>
    struct TLTransform;

    /*!
     * \class TLSize
     * \brief A metafunction which counts the number of elements in a TL.
     *
     * \param T, a TL
     * \return value, the number of elements in T.
     *
     * \author Helfer Thomas
     * \date   30 August 2006
     */
    template <typename T>
    struct TLSize;

    /*!
     * \class TLCountNbrOfT
     * \brief A metafunction which counts the number of
     * times T appears in a TL.
     *
     * \param T, a type.
     * \param U, a TL.
     * \return value, the number of times T appears in U.
     *
     * \author Helfer Thomas
     * \date   30 August 2006
     */
    template <typename T, typename U>
    struct TLCountNbrOfT;

    /*!
     * \class TLFindEltPos
     * \brief A metafunction which returns the position of the
     * first appearance of a type in a TL.
     *
     * \param T, a type.
     * \param List, a TL.
     * \return value, the position of T in List.
     *
     * \author Helfer Thomas
     * \date   30 August 2006
     */
    template <typename T, typename List>
    struct TLFindEltPos;

    /*!
     * \class TLPrepend
     * \brief A metafunction which returns a new typelist by adding
     * an element at the beginnig of a typelist.
     * \param List, a TL.
     * \param T, a type.
     * \return type, a new typelist.
     * \author Helfer Thomas
     * \date   09 September 2006
     */
    template <typename List, typename T>
    struct TLPrepend;

    /*!
     * \class TLAppend
     * \brief A metafunction which returns a new typelist by adding
     * an element at the end of a typelist.
     * \param List, a TL.
     * \param T, a type.
     * \return type, a new typelist.
     * \author Helfer Thomas
     * \date   09 September 2006
     */
    template <typename List, typename T>
    struct TLAppend;

    /*!
     * \class TLConcatenate
     * \brief A metafunction which returns the concatenation of two TLs
     *
     * \param  typename First,  a TL.
     * \param  typename Second, a TL.
     * \return type, the resulting TL.
     *
     * \author Helfer Thomas
     * \date   15 Sept. 2006
     */
    template <typename First, typename Second>
    struct TLConcatenate;

    /*!
     * \class TLFindNthElt
     * \brief A metafunction which returns the Nth
     * element of the TL.
     *
     * \param T, a TL.
     * \param N, a positive integer.
     * \return type, the Nth element of T or TLE
     * if T contains less than N elements.
     *
     * \author Helfer Thomas
     * \date   30 August 2006
     */
    template <typename T, unsigned int N>
    struct TLFindNthElt;

    /*!
     * \class TLRemoveNthFirstElt
     * \brief A metafunction which remove the first Nth
     * elements of the TL.
     *
     * \param T, a TL.
     * \param N, a positive integer.
     * \return type, a TL or TLE
     * if T contains less than N elements.
     *
     * \author Helfer Thomas
     * \date   30 August 2006
     */
    template <typename T, unsigned int N>
    struct TLRemoveNthFirstElt;

    /*!
     * \class TLElementIsUnique
     * \brief A metafunction which returns true if a
     * type appears one and only one time in a TL.
     *
     * \param T, a type.
     * \param U, a TL.
     * \return cond, true if T  appears one and only one time in U.
     *
     * \author Helfer Thomas
     * \date   30 August 2006
     */
    template <typename T, typename U>
    struct TLElementIsUnique;

    /*!
     * \class TLElementsAreUnique
     * \brief A metafunction which returns true if all elements
     * in a TL are unique.
     *
     * \param T, a TL.
     * \return cond, true if all elements in T are unique
     *
     * \author Helfer Thomas
     * \date   30 August 2006
     */
    template <typename T>
    struct TLElementsAreUnique;

    /*!
     * \class TLExtractSubClassesOf
     * \brief A metafunction which returns a new typelist from a
     * typelist.  All types in the new typelist inherit from the
     * type given in argument.
     * \param List, a TL.
     * \param BaseType, a type.
     * \return type, a new typelist.
     * \author Helfer Thomas
     * \date   09 September 2006
     */
    template <typename List, typename BaseType>
    struct TLExtractSubClassesOf;

    /*!
     * \class TLUnique
     * \brief A metafunction which returns a new typelist from a
     * typelist where all elements are unique.
     *
     * \param List, a TL.
     * \return type, a new typelist.
     * \author Helfer Thomas
     * \date   08 Januar 2006
     */
    template <typename List>
    struct TLUnique;

    /*!
     * \class TLContainsInvalidType
     * \brief A metafunction which returns true if the list
     * contains InvalidType.
     *
     * \param typename List, a TL.
     *
     * \return const bool.
     * \author Helfer Thomas
     * \date   10 March 2007
     */
    template <typename List>
    struct TLContainsInvalidType;

    //! Computes the maximum size of all types in a TL.
    /*
     * \class TLMaxSize.
     * \param  List, the list of types evaluated.
     * \return cons size_t value, the result.
     * The size is given by the sizeof operator.
     * The minimum size is 1.
     * \author Helfer Thomas.
     * \date   22 Apr. 2007.
     */
    template <typename List>
    class TLMaxSize;

    /*
     * \class TLMaxSize.
     * \param  List, the list of types evaluated.
     * \return cons size_t value, the result.
     * The size is given by the sizeof operator.
     * The minimum size is alignof(char).
     * \author Helfer Thomas.
     * \date   16/12/2014
     */
    template <typename List>
    class TLMaxAlign;

    /*
     * \class TLComputeAlignBound.
     * Extracts from a type list all the types which have a size
     * inferior than a given value.
     * \param List, a TL.
     * \param size_t, the thresold value.
     * \return type, a result under the form of a TL.
     * \author Helfer Thomas.
     * \date   22 Apr. 2007.
     */
    template <typename List, size_t size>
    class TLComputeAlignBound;

  }  // end of namespace meta

}  // end of namespace tfel

#include "TFEL/Metaprogramming/TypeList.ixx"
#include "TFEL/Metaprogramming/GenerateTypeList.hxx"

#endif /* LIB_TFEL_TYPELIST_H_ */
