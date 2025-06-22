/*!
 * \file  include/TFEL/Math/Array/MutableRuntimeArrayBase.hxx
 * \brief
 * \author Thomas Helfer
 * \date 01/01/2021
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_ARRAY_MUTABLERUNTIMEARRAYBASE_HXX
#define LIB_TFEL_MATH_ARRAY_MUTABLERUNTIMEARRAYBASE_HXX

#include "TFEL/Math/Array/ArrayPolicies.hxx"
#include "TFEL/Math/Array/ArrayCommonMethods.hxx"
#include "TFEL/Math/Array/RuntimeArrayPolicies.hxx"
#include "TFEL/Math/Array/StandardTemplateLibraryCompatibilityLayers.hxx"

namespace tfel::math {

  /*!
   * \brief a base class for mutable arrays with runtime sizes.
   */
  template <typename Child, typename ArrayPolicy>
  struct MutableRuntimeArrayBase
      : public ArrayPolicy::IndexingPolicy,
        public MutableArrayCommonMethods<Child, ArrayPolicy>,
        public MutableArraySTLCompatibilityLayer<Child, ArrayPolicy> {
    //! \brief default constructor
    MutableRuntimeArrayBase() = default;
    //! \brief constructor from an indexing policy
    MutableRuntimeArrayBase(const typename ArrayPolicy::IndexingPolicy&);
    //! \brief copy constructor
    MutableRuntimeArrayBase(const MutableRuntimeArrayBase&) = default;
    //! \brief move constructor
    MutableRuntimeArrayBase(MutableRuntimeArrayBase&&) = default;
    //! \brief copy assignement
    MutableRuntimeArrayBase& operator=(const MutableRuntimeArrayBase&) =
        default;
    //! \brief move assignement
    MutableRuntimeArrayBase& operator=(MutableRuntimeArrayBase&&) = default;
    // exposing MutableArrayCommonMethods assignement operators
    using MutableArrayCommonMethods<Child, ArrayPolicy>::operator=;
    //! \return the current indexing policy
    TFEL_HOST_DEVICE constexpr typename ArrayPolicy::IndexingPolicy&
    getIndexingPolicy() const noexcept;
    /*!
     * \brief apply a multi-indices functor
     * \param[in] f: functor
     * \tparam Function: type of the functor
     */
    template <typename Functor>
    void iterate(const Functor&);
  };  // end of struct MutableRuntimeArrayBase

}  // end of namespace tfel::math

#include "TFEL/Math/Array/MutableRuntimeArrayBase.ixx"

#endif /* LIB_TFEL_MATH_ARRAY_MUTABLERUNTIMEARRAYBASE_HXX */
