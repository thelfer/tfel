/*!
 * \file   mfront/include/MFront/VariableDescriptionBase.hxx
 * \brief  This file declares the VariableDescriptionBase class
 * \author Thomas Helfer
 * \date   17 Jan 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_VARIABLEDESCRIPTIONBASE_HXX
#define LIB_MFRONT_VARIABLEDESCRIPTIONBASE_HXX

#include <map>
#include <vector>
#include <string>
#include <initializer_list>
#include "MFront/MFrontConfig.hxx"
#include "MFront/SupportedTypes.hxx"
#include "MFront/VariableAttribute.hxx"

namespace mfront {

  /*!
   * \brief structure standing for a variable.
   * The support of fixed-sized arrays has been added lately.
   * This variable is considered as an array if arraySize is greater
   * than 1.
   */
  struct MFRONT_VISIBILITY_EXPORT VariableDescriptionBase {
    //! default constructor
    VariableDescriptionBase();
    //! copy constructor
    VariableDescriptionBase(const VariableDescriptionBase&);
    //! move constructor
    VariableDescriptionBase(VariableDescriptionBase&&);
    //! move operator
    VariableDescriptionBase& operator=(VariableDescriptionBase&&);
    //! assignement operator
    VariableDescriptionBase& operator=(const VariableDescriptionBase&);
    /*!
     * \brief constructor
     * \param[in] t: variable type
     * \param[in] n: variable name
     * \param[in] as: if greater than 1, this variable will
     * be considered as an array
     * \param[in] l: line number
     */
    VariableDescriptionBase(const std::string&,
                            const std::string&,
                            const unsigned short,
                            const size_t);
    /*!
     * \brief constructor
     * \param[in] t: variable type
     * \param[in] s: variable name
     * \param[in] n: variable name
     * \param[in] as: if greater than 1, this variable will
     * be considered as an array
     * \param[in] l: line number
     */
    VariableDescriptionBase(const std::string&,
                            const std::string&,
                            const std::string&,
                            const unsigned short,
                            const size_t);
    //! \return the type flag associated to a the variable
    SupportedTypes::TypeFlag getTypeFlag() const;
    //! \return the size of the variable
    SupportedTypes::TypeSize getTypeSize() const;
    //! \brief destructor
    ~VariableDescriptionBase();
    /*!
     * \brief type of the variable
     *
     *  If the variable has been declared as an array (see below),
     *  this field holds the type contained by the array.
     */
    std::string type;
    //! symbolic representation of the variable
    std::string symbolic_form;
    //! name of the variable
    std::string name;
    //! description
    std::string description;
    //! if the variable has been declared as an array, this field
    //  contains a value greater than 1
    unsigned short arraySize;
    //! line at wich the variable has been declared
    size_t lineNumber;
  };  // end of struct VariableDescriptionBase

  /*!
   * \return the symbolic representation of the variable if defined or the
   * variable name.
   * \param[in] v: variable
   */
  MFRONT_VISIBILITY_EXPORT const std::string& displayName(
      const VariableDescriptionBase&);

}  // end of namespace mfront

#endif /* LIB_MFRONT_VARIABLEDESCRIPTIONBASE_HXX */
