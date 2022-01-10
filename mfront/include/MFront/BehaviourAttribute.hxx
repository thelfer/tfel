/*!
 * \file  mfront/include/MFront/BehaviourAttribute.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 19 mars 2014
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_BEHAVIOURATTRIBUTE_HXX
#define LIB_MFRONT_BEHAVIOURATTRIBUTE_HXX

#include <map>
#include <string>
#include <vector>
#include "TFEL/Metaprogramming/GenerateTypeList.hxx"
#include "TFEL/Utilities/GenTypeBase.hxx"
#include "MFront/MFrontConfig.hxx"

namespace mfront {

  //! \brief types that can be stored in a behaviour attribute
  using BehaviourAttributeTypes =
      tfel::meta::GenerateTypeList<bool,
                                   unsigned short,
                                   std::string,
                                   std::vector<std::string>>::type;
  //! \brief a class storing mechanical behaviour attribute
  using BehaviourAttribute =
      tfel::utilities::GenTypeBase<BehaviourAttributeTypes>;

  /*!
   * \brief an helper structure to handle behaviours' attributes.
   */
  struct  MFRONT_VISIBILITY_EXPORT BehaviourAttributesHandler {
    /*!
     * \brief throw an exception saying that no attribute with the given name
     * exists
     */
    [[noreturn]] static void throwUndefinedAttribute(const std::string&);
    /*!
     * \brief insert a new attribute
     * \param[in] n : name
     * \param[in] a : attribute
     * \param[in] b : don't throw if the the
     *                attribute already exists.
     *                The attribute is left unchanged.
     *                However the type of the attribute is checked.
     */
    void setAttribute(const std::string&,
                      const BehaviourAttribute&,
                      const bool);
    /*!
     * \brief update an existing new attribute
     * \param[in] n: name
     * \param[in] a: attribute
     */
    void updateAttribute(const std::string&, const BehaviourAttribute&);
    /*!
     * \return true if an attribute with the given name as been registred
     * \param[in] n : name
     */
    bool hasAttribute(const std::string&) const;
    /*!
     * \return the attribute with the given name
     * \param[in] n : name
     */
    template <typename T>
    typename std::enable_if<
        tfel::meta::TLCountNbrOfT<T, BehaviourAttributeTypes>::value == 1,
        T&>::type
    getAttribute(const std::string&);
    /*!
     * \return the attribute with the given name
     * \param[in] n : name
     */
    template <typename T>
    typename std::enable_if<
        tfel::meta::TLCountNbrOfT<T, BehaviourAttributeTypes>::value == 1,
        const T&>::type
    getAttribute(const std::string&) const;
    /*!
     * \return all the attribute registred
     * \param[in] n : name
     */
    const std::map<std::string, BehaviourAttribute>& getAttributes() const;

   protected:
    //! \brief behaviour attributes
    std::map<std::string, BehaviourAttribute> attributes;
  };  // end of struct BehaviourAttributeHandler

}  // end of namespace mfront

#include "MFront/BehaviourAttribute.ixx"

#endif /* LIB_MFRONT_BEHAVIOURATTRIBUTE_HXX */
