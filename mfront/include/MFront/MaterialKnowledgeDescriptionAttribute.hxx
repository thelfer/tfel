/*!
 * \file  mfront/include/MFront/MaterialKnowledgeDescriptionAttribute.hxx
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

#ifndef LIB_MFRONT_MATERIALKNOWLEDGEDESCRIPTIONATTRIBUTE_HXX
#define LIB_MFRONT_MATERIALKNOWLEDGEDESCRIPTIONATTRIBUTE_HXX

#include <map>
#include <string>
#include <vector>
#include "TFEL/Metaprogramming/GenerateTypeList.hxx"
#include "TFEL/Utilities/GenTypeBase.hxx"
#include "MFront/MFrontConfig.hxx"

namespace mfront {

  //! \brief types that can be stored in a behaviour attribute
  using MaterialKnowledgeDescriptionAttributeTypes =
      tfel::meta::GenerateTypeList<bool,
                                   unsigned short,
                                   std::string,
                                   std::vector<std::string>>::type;
  //! \brief a class storing mechanical behaviour attribute
  using MaterialKnowledgeDescriptionAttribute =
      tfel::utilities::GenTypeBase<MaterialKnowledgeDescriptionAttributeTypes>;

  template <typename T>
  constexpr bool isMaterialKnowledgeDescriptionAttributeType() {
    return tfel::meta::TLCountNbrOfT<
               T, MaterialKnowledgeDescriptionAttributeTypes>::value == 1;
    ;
  }  // end of isMaterialKnowledgeDescriptionAttributeType

  /*!
   * \brief an helper structure to handle behaviours' attributes.
   */
  struct MFRONT_VISIBILITY_EXPORT
      MaterialKnowledgeDescriptionAttributesHandler {
    //! \brief attribute name
    static const char* const parametersAsStaticVariables;
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
                      const MaterialKnowledgeDescriptionAttribute&,
                      const bool);
    /*!
     * \brief update an existing new attribute
     * \param[in] n: name
     * \param[in] a: attribute
     */
    void updateAttribute(const std::string&,
                         const MaterialKnowledgeDescriptionAttribute&);
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
    typename std::enable_if<isMaterialKnowledgeDescriptionAttributeType<T>(),
                            T&>::type
    getAttribute(const std::string&);
    /*!
     * \return the attribute with the given name
     * \param[in] n : name
     */
    template <typename T>
    typename std::enable_if<isMaterialKnowledgeDescriptionAttributeType<T>(),
                            const T&>::type
    getAttribute(const std::string&) const;
    /*!
     * \return the attribute with the given name
     * \param[in] n: name
     */
    template <typename T>
    typename std::enable_if<isMaterialKnowledgeDescriptionAttributeType<T>(),
                            T>::type
    getAttribute(const std::string&, const T&) const;
    /*!
     * \return all the attribute registred
     * \param[in] n : name
     */
    const std::map<std::string, MaterialKnowledgeDescriptionAttribute>&
    getAttributes() const;

   protected:
    //! \brief behaviour attributes
    std::map<std::string, MaterialKnowledgeDescriptionAttribute> attributes;
  };  // end of struct MaterialKnowledgeDescriptionAttributesHandler

  /*!
   * \brief this function returns the value of the
   * `MaterialKnowledgeDescriptionAttributesHandler::parametersAsStaticVariables`
   * attribute if it is defined, `false` otherwise.
   * \return if the parameters are treated as static variables.
   * \param[in] h: attribute handler
   */
  MFRONT_VISIBILITY_EXPORT bool areParametersTreatedAsStaticVariables(
      const MaterialKnowledgeDescriptionAttributesHandler&);

}  // end of namespace mfront

#include "MFront/MaterialKnowledgeDescriptionAttribute.ixx"

#endif /* LIB_MFRONT_MATERIALKNOWLEDGEDESCRIPTIONATTRIBUTE_HXX */
