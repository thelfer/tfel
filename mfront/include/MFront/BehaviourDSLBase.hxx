/*!
 * \file   mfront/include/MFront/BehaviourDSLBase.hxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   08 nov 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONTPARSERBASE_HXX
#define LIB_MFRONTPARSERBASE_HXX

#include "MFront/BehaviourDSLCommon.hxx"

namespace mfront {

  /*!
   * \brief an helper structure.
   * In versions of `TFEL` prior to 3.3, keywords were associated with member
   * functions and CRTP was used to provide this functionnality. This mechanism
   * has been deprecated in favor of using `std::function`. As a consequence,
   * `BehaviourDSLBase` is now mostly useless.
   */
  template <typename Child>
  struct BehaviourDSLBase : public BehaviourDSLCommon {
   protected:
    //! \brief a simple alias
    using MemberFuncPtr = void (Child::*)();
    /*!
     * \brief constructor
     * \param[in] opts: options passed to the DSL
     */
    BehaviourDSLBase(const DSLOptions&);
    /*!
     * \brief associate a call-back to a member function of the derived class
     * (using CRTP).
     * \param[in] k: keyword
     * \param[in] f: member function
     * \param[in] b: if true, allow the given member function to override an
     * existing call-back, if any.
     */
    void registerNewCallBack(const std::string&,
                             const MemberFuncPtr,
                             const bool = false);
    //! \brief destructor
    ~BehaviourDSLBase() override;
  };  // end of struct BehaviourDSLBase

}  // end of namespace mfront

#include "MFront/BehaviourDSLBase.ixx"

#endif /* LIB_MFRONTPARSERBASE_HXX */
