/*!
 * \file   mfront/include/MFront/VariableDescription.hxx
 * \brief  This file declares the VariableDescription class
 * \author Thomas Helfer
 * \date   17 Jan 2007
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_VARIABLEDESCRIPTION_HXX
#define LIB_MFRONT_VARIABLEDESCRIPTION_HXX

#include <map>
#include <vector>
#include <string>
#include <variant>
#include <optional>
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
    //
    template <typename T>
    static constexpr bool isVariableAttribute =
        tfel::meta::TLCountNbrOfT<T, VariableAttributeTypes>::value == 1;
    //! \brief standard attribute name
    static const char* const depth;
    //! \brief standard attribute name
    static const char* const initialValue;
    //! \brief standard attribute name
    static const char* const defaultValue;
    //! \brief error normalisation factor
    static const char* const errorNormalisationFactor;
    /*!
     * \brief attribute stating that a variable has already been declared in a
     * based class.
     *
     * This attribute has been introduced for parameters corresponding to
     * numerical parameters declared in base classes describing the resolution
     * algorithms.
     */
    static const char* const variableDeclaredInBaseClass;
    //! \brief default constructor
    VariableDescription();
    //! \brief copy constructor
    VariableDescription(const VariableDescription&);
    //! \brief move constructor
    VariableDescription(VariableDescription&&);
    //! \brief move operator
    VariableDescription& operator=(VariableDescription&&);
    //! \brief assignement operator
    VariableDescription& operator=(const VariableDescription&);
    /*!
     * \brief throw an exception saying that no attribute with the
     * given name exists
     * \param[in] n: name of the attribute
     */
    [[noreturn]] static void throwUndefinedAttribute(const std::string&);
    /*!
     * \brief constructor
     * \param[in] t: variable type
     * \param[in] n: variable name
     * \param[in] as: if greater than 1, this variable will
     * be considered as an array
     * \param[in] l: line number
     */
    VariableDescription(const std::string&,
                        const std::string&,
                        const unsigned short,
                        const size_t);
    /*!
     * \brief constructor
     * \param[in] t: variable type
     * \param[in] s: symbolic name
     * \param[in] n: variable name
     * \param[in] as: if greater than 1, this variable will
     * be considered as an array
     * \param[in] l: line number
     */
    VariableDescription(const std::string&,
                        const std::string&,
                        const std::string&,
                        const unsigned short,
                        const size_t);
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
    T& getAttribute(const std::string&) requires(isVariableAttribute<T>);
    /*!
     * \return the attribute with the given name
     * \param[in] n : name
     */
    template <typename T>
    const T& getAttribute(const std::string&) const
        requires(isVariableAttribute<T>);
    /*!
     * \return the attribute with the given name or the given default
     * value if the variable does not exists
     * \param[in] n : name
     * \param[in] v : value
     */
    template <typename T>
    T getAttribute(const std::string&, const T&) const
        requires(isVariableAttribute<T>);
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
     * \brief associate a new glossary name to the variable
     * \param[in] g: glossary name
     */
    void resetGlossaryName(const std::string&);
    /*!
     * \brief associate an entry name to the variable
     * \param[in] e: entry name
     */
    void setEntryName(const std::string&);
    /*!
     * \brief associate a new entry name to the variable
     * \param[in] e: entry name
     */
    void resetEntryName(const std::string&);
    //! \return true if the variable has a glossary name
    bool hasGlossaryName() const;
    //! \return true if the variable has a glossary name
    bool hasEntryName() const;
    //! \return the external name of the variable
    const std::string& getExternalName() const;
    //! \return if the variable describes a scalar
    bool isScalar() const;
    /*!
     * \return an integer describing the type of variable
     * \see `SupportedTypes::getVariableTypeIdentifier` for details.
     */
    int getVariableTypeIdentifier() const;
    //! \brief destructor
    ~VariableDescription();

   private:
    //! \brief a simple alias
    using OptionalVariableBoundsDescription =
        std::variant<std::monostate,
                     VariableBoundsDescription,
                     std::map<unsigned short, VariableBoundsDescription>>;
    //! \brief standard bounds
    OptionalVariableBoundsDescription bounds;
    //! \brief standard bounds
    OptionalVariableBoundsDescription physicalBounds;
    //! \brief glossary name
    std::optional<std::string> glossaryName;
    //! \brief entry name
    std::optional<std::string> entryName;
    //! \brief variable attributes
    std::map<std::string, VariableAttribute> attributes;
  };  // end of struct VariableDescription

  /*!
   * \brief declare the physical bounds of a variable if:
   *
   * - the variable is associated with a glossary name and the glossary entry
   *   declares lower or upper physical bounds
   * - physical bounds were not already declared
   *
   * This method also checks if the declaration of the physical bounds are
   * consistent with the one of the glossary entry.
   *
   * \param[in] v: variable
   * \param[in] s: unit system
   */
  MFRONT_VISIBILITY_EXPORT void checkAndCompletePhysicalBoundsDeclaration(
      VariableDescription&, const std::string_view);
  /*!
   * \return the size of the variable
   * \param[in] v: variable description
   */
  MFRONT_VISIBILITY_EXPORT SupportedTypes::TypeSize getTypeSize(
      const VariableDescription&);
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

  /*!
   * \brief retrieve the symbol of the given variable description.
   * \param[out] symbols: map between symbols and replacement strings.
   * \param[in] v: variable description
   */
  MFRONT_VISIBILITY_EXPORT void getSymbol(std::map<std::string, std::string>&,
                                          const VariableDescription&);
  /*!
   * \brief retrieve the symbols associated with the increment of the given
   * variable.
   * \param[out] symbols: map between symbols and replacement strings.
   * \param[in] v: variable description
   */
  MFRONT_VISIBILITY_EXPORT void getIncrementSymbol(
      std::map<std::string, std::string>&, const VariableDescription&);
  /*!
   * \brief retrieve the symbols associated with the time derivative of the
   * given variable.
   * \param[out] symbols: map between symbols and replacement strings.
   * \param[in] v: variable description
   */
  MFRONT_VISIBILITY_EXPORT void getTimeDerivativeSymbol(
      std::map<std::string, std::string>&, const VariableDescription&);

  //! \brief a simple alias for backward compatibility
  typedef VariableDescription VarHandler;

  //! \brief a simple wrapper around the std::vector class
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
    /*!
     * \return the sum of the sizes of all the variables
     */
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
   * \return an iterator to the variable with the given external name
   * \param[in] c: variable container
   * \param[in] n: external name
   */
  MFRONT_VISIBILITY_EXPORT VariableDescriptionContainer::const_iterator
  findByExternalName(const VariableDescriptionContainer&, const std::string&);
  /*!
   * \return the sum of the sizes of all the variables
   * \param[in] c: variable description container
   */
  MFRONT_VISIBILITY_EXPORT SupportedTypes::TypeSize getTypeSize(
      const VariableDescriptionContainer&);
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
  /*!
   * \brief retrieve all symbols in the given variables.
   * \param[out] symbols: map between symbols and replacement strings.
   * \param[in] c: variable description container
   */
  MFRONT_VISIBILITY_EXPORT void getSymbols(std::map<std::string, std::string>&,
                                           const VariableDescriptionContainer&);
  /*!
   * \brief retrieve all symbols associated with the increment of the given
   * variables.
   * \param[out] symbols: map between symbols and replacement strings.
   * \param[in] c: variable description container
   */
  MFRONT_VISIBILITY_EXPORT void getIncrementSymbols(
      std::map<std::string, std::string>&, const VariableDescriptionContainer&);
  /*!
   * \brief retrieve all symbols associated with the time derivative of the
   * given variables.
   * \param[out] symbols: map between symbols and replacement strings.
   * \param[in] c: variable description container
   */
  MFRONT_VISIBILITY_EXPORT void getTimeDerivativeSymbols(
      std::map<std::string, std::string>&, const VariableDescriptionContainer&);
  /*!
   * \return the offset of the given variable.
   * \param[in] c: variable description container
   * \param[in] n: name of the variable
   */
  MFRONT_VISIBILITY_EXPORT SupportedTypes::TypeSize getOffset(
      const VariableDescriptionContainer&, const std::string&);
  /*!
   * \brief declare the physical bounds of all variables using the glossary
   * \param[in] variables: list of variables
   * \param[in] s: unit system
   */
  MFRONT_VISIBILITY_EXPORT void checkAndCompletePhysicalBoundsDeclaration(
      VariableDescriptionContainer&, const std::string_view);

  //! \brief a simple alias for backward compatibility with previous version of
  //! TFEL
  using VarContainer = VariableDescriptionContainer;

}  // end of namespace mfront

#include "MFront/VariableDescription.ixx"

#endif /* LIB_MFRONT_VARIABLEDESCRIPTION_HXX */
