/*!
 * \file   mfront/include/MFront/VariableDescription.hxx
 * \brief  This file declares the VariableDescription class
 * \author Thomas Helfer
 * \date   17 Jan 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_VARIABLEDESCRIPTION_HXX
#define LIB_MFRONT_VARIABLEDESCRIPTION_HXX

#include <map>
#include <vector>
#include <string>
#include <initializer_list>

#include "TFEL/Utilities/GenTypeBase.hxx"
#include "MFront/MFrontConfig.hxx"
#include "MFront/SupportedTypes.hxx"
#include "MFront/VariableAttribute.hxx"
#include "MFront/VariableBoundsDescription.hxx"
#include "MFront/VariableDescriptionBase.hxx"

namespace mfront {

  //! \brief structure standing for a standard (non static) variable
  struct MFRONT_VISIBILITY_EXPORT VariableDescription
      : public VariableDescriptionBase {
    //! standard attribute name
    static const char* const depth;
    //! standard attribute name
    static const char* const initialValue;
    //! standard attribute name
    static const char* const defaultValue;
    //! error normalisation factor
    static const char* const errorNormalisationFactor;
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
    //! \return the type flag associated to a the variable
    SupportedTypes::TypeFlag getTypeFlag() const;
    //! \return the size of the variable
    SupportedTypes::TypeSize getTypeSize() const;
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
    const std::map<std::string, VariableAttribute>& getAttributes() const;
    //! \return true if bounds has been specified for this variable
    bool hasBounds() const;
    /*!
     * \return true if bounds has been specified for this variable
     * \param[in] i: position
     */
    bool hasBounds(const unsigned short) const;
    /*!
     * \brief set bounds on this variable
     * \param[in] b: bounds
     */
    void setBounds(const VariableBoundsDescription&);
    /*!
     * \brief set bounds on this variable
     * \param[in] b: bounds
     * \param[in] i: position
     */
    void setBounds(const VariableBoundsDescription&, const unsigned short);
    //! \return the bounds of this variable
    const VariableBoundsDescription& getBounds() const;
    /*!
     * \return the bounds of this variable
     * \param[in] i: position
     */
    const VariableBoundsDescription& getBounds(const unsigned short) const;
    //! \return true if physical bounds has been specified for this variable
    bool hasPhysicalBounds() const;
    /*!
     * \return true if bounds has been specified for this variable
     * \param[in] i: position
     */
    bool hasPhysicalBounds(const unsigned short) const;
    /*!
     * \brief set physical bounds on this variable
     * \param[in] b: physical bounds
     */
    void setPhysicalBounds(const VariableBoundsDescription&);
    /*!
     * \brief set physical bounds on this variable
     * \param[in] b: physical bounds
     */
    void setPhysicalBounds(const VariableBoundsDescription&,
                           const unsigned short);
    //! \return the physical bounds of this variable
    const VariableBoundsDescription& getPhysicalBounds() const;
    /*!
     * \return the physical bounds of this variable
     * \param[in] i: position
     */
    const VariableBoundsDescription& getPhysicalBounds(
        const unsigned short) const;
    /*!
     * \brief associate a glossary name to the variable
     * \param[in] g: glossary name
     */
    void setGlossaryName(const std::string&);
    /*!
     * \brief associate an entry name to the variable
     * \param[in] e: entry name
     */
    void setEntryName(const std::string&);
    //! \return true if the variable has a glossary name
    bool hasGlossaryName() const;
    //! \return true if the variable has a glossary name
    bool hasEntryName() const;
    //! \return the external name of the variable
    const std::string& getExternalName() const;
    //! destructor
    ~VariableDescription();

   private:
    //! a simple alias
    using OptionalVariableBoundsDescription = tfel::utilities::GenType<
        VariableBoundsDescription,
        std::map<unsigned short, VariableBoundsDescription>>;
    //! a simple alias
    using OptionalString = tfel::utilities::GenType<std::string>;
    //! standard bounds
    OptionalVariableBoundsDescription bounds;
    //! standard bounds
    OptionalVariableBoundsDescription physicalBounds;
    //! glossary name
    OptionalString glossaryName;
    //! entry name
    OptionalString entryName;
    //! variable attributes
    std::map<std::string, VariableAttribute> attributes;
  };  // end of struct VariableDescription

  /*!
   * \return true if the variable has bounds.
   * \param[in] v: variable description
   * \note this is a simple wrapper around the `hasBounds` method
   */
  MFRONT_VISIBILITY_EXPORT bool hasBounds(const VariableDescription&);
  /*!
   * \return true if the variable has bounds.
   * \param[in] c: variable description container
   * \param[in] i: index
   * \note this is a simple wrapper around the `hasBounds` method
   */
  MFRONT_VISIBILITY_EXPORT bool hasBounds(const VariableDescription&,
                                          const unsigned short);
  /*!
   * \return true if the variable has physical bounds.
   * \param[in] v: variable description
   * \note this is a simple wrapper around the `hasPhysicalBounds`
   * method.
   */
  MFRONT_VISIBILITY_EXPORT bool hasPhysicalBounds(const VariableDescription&);
  /*!
   * \return true if the variable has physical bounds.
   * \param[in] c: variable description container
   * \param[in] i: index
   * \note this is a simple wrapper around the `hasPhysicalBounds`
   * method.
   */
  MFRONT_VISIBILITY_EXPORT bool hasPhysicalBounds(const VariableDescription&,
                                                  const unsigned short);

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
    using std::vector<VariableDescription>::erase;
    using std::vector<VariableDescription>::operator[];
    /*!
     * \param[in] n : variable name
     * \return true if a variable with the given name exists
     */
    bool contains(const std::string&) const;
    //! \return the sum of the sizes of all the variables
    SupportedTypes::TypeSize getTypeSize() const;
    //! \return the sum of the array sizes of all the variables
    unsigned short getNumberOfVariables() const;
    /*!
     * \return the list of external names associated with this
     * container.
     * \see VariableDescription::getExternalName
     */
    std::vector<std::string> getExternalNames() const;
    /*!
     * get the external names associated with this container.
     * \see VariableDescription::getExternalName
     * \param[out] n: glossary names of the function
     */
    void getExternalNames(std::vector<std::string>&) const;
    /*!
     * append the list of external names associated with this
     * container at the end of the given list.
     * \see VariableDescription::getExternalName
     * \param[in] n: list of names
     */
    void appendExternalNames(std::vector<std::string>&) const;
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
     */
    const VariableDescription& getVariableByExternalName(
        const std::string&) const;
    //! destructor
    ~VariableDescriptionContainer();
  };  // end of struct VariableDescriptionContainer

  /*!
   * \return true if one of the variables has bounds.
   * \param[in] c: variable description container
   */
  MFRONT_VISIBILITY_EXPORT bool hasBounds(const VariableDescriptionContainer&);
  /*!
   * \return true if one of the variables has physical bounds.
   * \param[in] c: variable description container
   */
  MFRONT_VISIBILITY_EXPORT bool hasPhysicalBounds(
      const VariableDescriptionContainer&);

  //! a simple alias for backward compatibility
  typedef VariableDescriptionContainer VarContainer;

}  // end of namespace mfront

#include "MFront/VariableDescription.ixx"

#endif /* LIB_MFRONT_VARIABLEDESCRIPTION_HXX */
