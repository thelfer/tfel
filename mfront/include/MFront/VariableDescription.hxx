/*!
 * \file   mfront/include/MFront/VariableDescription.hxx
 * \brief  This file declares the VariableDescription class
 * \author Helfer Thomas
 * \date   17 Jan 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_VARDESCRIPTION_H_
#define LIB_MFRONT_VARDESCRIPTION_H_

#include <map>
#include <vector>
#include <string>
#include <initializer_list>

#include "MFront/MFrontConfig.hxx"
#include "MFront/VariableAttribute.hxx"

namespace mfront {

  /*!
   * structure standing for a variable
   * The support of fixed-sized arrays has been added lately.
   * This variable is considered as an array if arraySize is greater than 1.
   */
  struct MFRONT_VISIBILITY_EXPORT VariableDescription {
    //! standard attribute name
    static const std::string depth;
    //! standard attribute name
    static const std::string bound;
    //! standard attribute name
    static const std::string physicalBound;
    //! standard attribute name
    static const std::string initialValue;
    //! standard attribute name
    static const std::string defaultValue;
    //! default constructor
    VariableDescription();
    //! copy constructor
    VariableDescription(const VariableDescription&);
    //! move constructor
    VariableDescription(VariableDescription&&);
    //! move operator
    VariableDescription& operator=(VariableDescription&&);
    //! assignement operator
    VariableDescription& operator=(const VariableDescription&);
    /*!
     * \brief throw an exception saying that no attribute with the
     * given name exists
     * \param[in] n: name of the attribute
     */
    TFEL_NORETURN static void throwUndefinedAttribute(const std::string&);
    /*!
     * Constructor
     * \param[in] t : variable type
     * \param[in] n : variable name
     * \param[in] s : if greater than 1, this variable will
     * be considered as an array
     * \param[in] l : line number
     */
    VariableDescription(const std::string&,
                        const std::string&,
                        const unsigned short,
                        const size_t);
    /*!
     * \return the name used to call the variable from outside
     * (pleiades application, mtest, etc..)
     * \param[in] gn: glossaryNames
     * \param[in] en: entryNames
     */
    const std::string& getExternalName(
        const std::map<std::string, std::string>&,
        const std::map<std::string, std::string>&) const;
    /*!
     * \brief insert a new attribute
     * \param[in] n : name
     * \param[in] a : attribute
     * \param[in] b : don't throw if the attribute already exists.  In
     * this case, the attribute is left unchanged. However the type of
     * the attribute is checked.
     */
    void setAttribute(const std::string&, const VariableAttribute&, const bool);
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
        tfel::meta::TLCountNbrOfT<T, VariableAttributeTypes>::value == 1,
        T&>::type
    getAttribute(const std::string&);
    /*!
     * \return the attribute with the given name
     * \param[in] n : name
     */
    template <typename T>
    typename std::enable_if<
        tfel::meta::TLCountNbrOfT<T, VariableAttributeTypes>::value == 1,
        const T&>::type
    getAttribute(const std::string&) const;
    /*!
     * \return the attribute with the given name or the given default
     * value if the variable does not exists
     * \param[in] n : name
     * \param[in] v : value
     */
    template <typename T>
    typename std::enable_if<
        tfel::meta::TLCountNbrOfT<T, VariableAttributeTypes>::value == 1,
        T>::type
    getAttribute(const std::string&, const T&) const;
    /*!
     * \return all the attribute registred
     * \param[in] n : name
     */
    const std::map<std::string, VariableAttribute>& getAttributes(void) const;
    //! destructor
    ~VariableDescription();
    //! type of the variable
    //  If the variable has been declared as an array (see below),
    //  this field holds the type contained by the array.
    std::string type;
    //! name of the variable
    std::string name;
    //! description
    std::string description;
    //! if the variable has been declared as an array, this field
    //  contains a value greater than 1
    unsigned short arraySize;
    //! line at wich the variable has been declared
    size_t lineNumber;

   private:
    //! variable attributes
    std::map<std::string, VariableAttribute> attributes;
  };  // end of struct VariableDescription

  //! a simple alias for backward compatibility
  typedef VariableDescription VarHandler;

  //! a simple wrapper around the std::vector class
  struct MFRONT_VISIBILITY_EXPORT VariableDescriptionContainer
      : private std::vector<VariableDescription> {
    VariableDescriptionContainer();
    VariableDescriptionContainer(
        const std::initializer_list<VariableDescription>&);
    VariableDescriptionContainer(VariableDescriptionContainer&&);
    VariableDescriptionContainer(const VariableDescriptionContainer&);
    VariableDescriptionContainer& operator=(VariableDescriptionContainer&&);
    VariableDescriptionContainer& operator=(
        const VariableDescriptionContainer&);
    // resusing method from std::vector
    using std::vector<VariableDescription>::size_type;
    using std::vector<VariableDescription>::value_type;
    using std::vector<VariableDescription>::difference_type;
    using std::vector<VariableDescription>::reference;
    using std::vector<VariableDescription>::const_reference;
    using std::vector<VariableDescription>::iterator;
    using std::vector<VariableDescription>::const_iterator;
    using std::vector<VariableDescription>::push_back;
    using std::vector<VariableDescription>::begin;
    using std::vector<VariableDescription>::end;
    using std::vector<VariableDescription>::cbegin;
    using std::vector<VariableDescription>::cend;
    using std::vector<VariableDescription>::empty;
    using std::vector<VariableDescription>::insert;
    using std::vector<VariableDescription>::front;
    using std::vector<VariableDescription>::back;
    using std::vector<VariableDescription>::clear;
    using std::vector<VariableDescription>::size;
    using std::vector<VariableDescription>::operator[];
    /*!
     * \param[in] n : variable name
     * \return true if a variable with the given name exists
     */
    bool contains(const std::string&) const;
    /*!
     * \return the list of external names associated with this
     * container.
     * \see VariableDescription::getExternalName
     * \param[in] glossaryNames : glossary names
     * \param[in] entryNames    : entry names
     */
    std::vector<std::string> getExternalNames(
        const std::map<std::string, std::string>&,
        const std::map<std::string, std::string>&) const;
    /*!
     * get the external names associated with this container.
     * \see VariableDescription::getExternalName
     * \param[out] names         : glossary names of the function
     * \param[in]  glossaryNames : glossary names
     * \param[in]  entryNames    : entry names
     */
    void getExternalNames(std::vector<std::string>&,
                          const std::map<std::string, std::string>&,
                          const std::map<std::string, std::string>&) const;
    /*!
     * append the list of external names associated with this
     * container at the end of the given list.
     * \see VariableDescription::getExternalName
     * \param[in] names         : list of names
     * \param[in] glossaryNames : glossary names
     * \param[in] entryNames    : entry names
     */
    void appendExternalNames(std::vector<std::string>&,
                             const std::map<std::string, std::string>&,
                             const std::map<std::string, std::string>&) const;
    /*!
     * \return the variable with the given name
     * \param[in] n : name
     */
    VariableDescription& getVariable(const std::string&);
    /*!
     * \return the variable with the given name
     * \param[in] n : name
     */
    const VariableDescription& getVariable(const std::string&) const;
    /*!
     * \return the variable with the given name
     * \param[in] n:  external name
     * \param[in] gn: glossary names
     * \param[in] en: entry names
     */
    const VariableDescription& getVariableByExternalName(
        const std::string&,
        const std::map<std::string, std::string>&,
        const std::map<std::string, std::string>&) const;
    //! destructor
    ~VariableDescriptionContainer();
  };  // end of struct VariableDescription

  //! a simple alias for backward compatibility
  typedef VariableDescriptionContainer VarContainer;

}  // end of namespace mfront

#include "MFront/VariableDescription.ixx"

#endif /* LIB_MFRONT_VARDESCRIPTION_H_ */
