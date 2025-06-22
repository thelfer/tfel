/*!
 * \file  include/TFEL/Math/Array/ConstRuntimeArrayBase.hxx
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

#ifndef LIB_TFEL_MATH_ARRAY_CONSTRUNTIMEARRAYBASE_HXX
#define LIB_TFEL_MATH_ARRAY_CONSTRUNTIMEARRAYBASE_HXX

#include "TFEL/Math/Array/ArrayPolicies.hxx"
#include "TFEL/Math/Array/ArrayCommonMethods.hxx"
#include "TFEL/Math/Array/StandardTemplateLibraryCompatibilityLayers.hxx"

namespace tfel::math {

  /*!
   * \brief a base class for fixed sized const arrays.
   */
  template <typename Child, typename ArrayPolicyType>
  struct ConstRuntimeArrayBase
      : public ArrayPolicyType::IndexingPolicy,
        public ConstArrayCommonMethods<Child, ArrayPolicyType>,
        public ConstArraySTLCompatibilityLayer<Child, ArrayPolicyType> {
    /*!
     * \brief contructor using the indexing policy
     * \param[in] i: indexing policy
     */
    constexpr ConstRuntimeArrayBase(
        const typename ArrayPolicyType::IndexingPolicy&);
    //! \brief move constructor
    constexpr ConstRuntimeArrayBase(ConstRuntimeArrayBase&&) = default;
    //! \brief copy constructor
    constexpr ConstRuntimeArrayBase(const ConstRuntimeArrayBase&) = default;
    //! \brief move assignement
    constexpr ConstRuntimeArrayBase& operator=(ConstRuntimeArrayBase&&) =
        default;
    //! \brief standard assignement
    constexpr ConstRuntimeArrayBase& operator=(const ConstRuntimeArrayBase&) =
        default;
    //! \return the current indexing policy
    constexpr auto& getIndexingPolicy() const noexcept;
    /*!
     * \brief apply a multi-indices functor
     * \param[in] f: functor
     * \tparam Function: type of the functor
     */
    template <typename Functor>
    void iterate(const Functor&) const;
  };

}  // namespace tfel::math

#include "TFEL/Math/Array/ConstRuntimeArrayBase.ixx"

#endif /* LIB_TFEL_MATH_ARRAY_CONSTRUNTIMEARRAYBASE_HXX */
