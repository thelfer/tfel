/*!
 * \file  mfront/include/MFront/CodeBlock.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 18 mai 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_CODEBLOCK_HXX
#define LIB_MFRONT_CODEBLOCK_HXX

#include <set>
#include <map>
#include <vector>
#include <string>
#include "TFEL/Metaprogramming/GenerateTypeList.hxx"
#include "TFEL/Utilities/GenTypeBase.hxx"
#include "MFront/MFrontConfig.hxx"
#include "MFront/VariableDescription.hxx"

namespace mfront {

  //! \brief structure describing a raw code block
  struct MFRONT_VISIBILITY_EXPORT CodeBlock {
    //! \brief a class used to store the attribute of a code block
    using Attribute = tfel::utilities::GenType<
        bool,
        std::string,
        std::vector<VariableDescription>,
        std::vector<std::pair<VariableDescription, VariableDescription>>>;
    //! \brief standard attribute name
    static const char* const requires_jacobian_decomposition;
    //! \brief standard attribute name
    static const char* const uses_get_partial_jacobian_invert;
    //! \brief standard attribute name
    static const char* const used_initialize_function_variables;
    //! \brief standard attribute name
    static const char* const used_postprocessing_variables;
    /*!
     * \brief name of an attribute associated with the use of the
     * `getIntegrationVariablesDerivatives_X` method where `X` is
     * the name of a gradient or the name of an external state variable.
     * The attribute stores the list of X's.
     */
    static const char* const used_get_integration_variables_derivatives;
    //! \brief standard attribute name
    static const char* const
        used_implicit_equations_derivatives_with_respect_to_gradients_or_external_state_variables;
    //! \brief standard attribute name
    static const char* const used_jacobian_invert_blocks;
    /*!
     * \brief raise an exception stating that an attribute has not the expected
     * type \param[in] n: name of the attribute
     */
    [[noreturn]] static void throwUnmatchedAttributeType(const std::string&);
    /*!
     * \brief raise an exception stating that an attribute does not exist
     * \param[in] n: name of the attribute
     */
    [[noreturn]] static void throwUndeclaredAttribute(const std::string&);
    //! \brief default contructor
    CodeBlock();
    //! \brief move contructor
    CodeBlock(CodeBlock&&);
    //! \brief copy contructor
    CodeBlock(const CodeBlock&);
    //! \brief move assignement
    CodeBlock& operator=(CodeBlock&&);
    //! \brief standard assignement
    CodeBlock& operator=(const CodeBlock&);
    //! \brief destructor
    ~CodeBlock();
    //! \brief code
    std::string code;
    //! \brief description
    std::string description;
    //! \brief static members used in the block
    std::set<std::string> staticMembers;
    //! \brief members used in the block
    std::set<std::string> members;
    //! \brief variable attributes
    std::map<std::string, Attribute> attributes;
  };  // end of struct CodeBlock

  /*!
   * \brief check if an attribute of the given type is defined
   * \throw if the type of the attribute does not match
   * \tparam T: type of the attribute
   * \param[in] cb: code block
   * \param[in] n: name of the attribute
   */
  template <typename T>
  bool hasAttribute(const CodeBlock&, const std::string&);
  /*!
   * \brief return the value of an attribute.
   * \throw if the attribute does not exist
   * \throw if the type of the attribute does not match
   * \tparam T: type of the attribute
   * \param[in] cb: code block
   * \param[in] n: name of the attribute
   */
  template <typename T>
  const T& getAttribute(const CodeBlock&, const std::string&);
  /*!
   * \brief return the value of an attribute or a default value if the attribute
   * is not defined.
   * \throw if the type of the attribute does not match
   * \tparam T: type of the attribute
   * \param[in] cb: code block
   * \param[in] n: name of the attribute
   * \param[in] v: default value
   */
  template <typename T>
  T getAttribute(const CodeBlock&, const std::string&, const T&);

}  // end of namespace mfront

#include "MFront/CodeBlock.ixx"

#endif /* LIB_MFRONT_CODEBLOCK_HXX */
