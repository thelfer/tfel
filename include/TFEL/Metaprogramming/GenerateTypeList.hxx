/*!
 * \file   include/TFEL/Metaprogramming/GenerateTypeList.hxx
 * \see    GenerateTypeList.cxx to see how this file was generated.
 * \brief  This file declares the GenerateTypeList template class.
 * \author Thomas Helfer
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_METAPROGRAMMING_GENERATETYPELIST_HXX
#define LIB_TFEL_METAPROGRAMMING_GENERATETYPELIST_HXX

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Metaprogramming/TypeList.hxx"

namespace tfel::meta {

  /*!
   * \class GenerateTypeList
   * GenerateTypeList is an helper class which can generate TL.  The
   * idea of this class was taken from Andrei Alexandrescu
   * works
   * \latexonly\cite{alexandrescu01:_c_desig,alexandrescu02}\endlatexonly
   * \htmlonly(http://www.ddj.com/dept/cpp/184403813)\endhtmlonly.
   * \author Thomas Helfer
   */
  template <typename... Types>
  struct TFEL_VISIBILITY_LOCAL GenerateTypeList;

  /*!
   * Partial specialisation to start the recursion
   */
  template <typename CurrentType, typename... Types>
  struct TFEL_VISIBILITY_LOCAL GenerateTypeList<CurrentType, Types...> {
    typedef TLNode<CurrentType, typename GenerateTypeList<Types...>::type> type;
  };

  /*!
   * Partial specialisation to end the recursion
   */
  template <>
  struct TFEL_VISIBILITY_LOCAL GenerateTypeList<> {
    typedef TLE type;
  };

}  // end of namespace tfel::meta

#endif /* LIB_TFEL_METAPROGRAMMING_GENERATETYPELIST_HXX */
