/*!
 * \file  include/TFEL/Glossary/GlossaryEntry.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 31 mars 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_GLOSSARY_GLOSSARYENTRY_HXX
#define LIB_TFEL_GLOSSARY_GLOSSARYENTRY_HXX

#include <map>
#include <set>
#include <string>
#include <vector>
#include <string_view>

#include "TFEL/Config/TFELConfig.hxx"

namespace tfel::glossary {

  // forward declaration
  struct Glossary;

  //! \brief structure describing an entry of the glossary
  struct TFELGLOSSARY_VISIBILITY_EXPORT GlossaryEntry {
    //! \brief internal name
    static const char* separator;
    //! \brief copy constructor
    GlossaryEntry(const GlossaryEntry&);
    //! \brief move constructor
    GlossaryEntry(GlossaryEntry&&);
    //! \return the key
    const std::string& getKey() const;
    //! \return names asssociated with this key
    const std::vector<std::string>& getNames() const;
    /*!
     * \return the unit
     * \param[in] s: unit system
     */
    const std::string& getUnit(const std::string_view) const;
    //! \return the units
    std::map<std::string, std::string> getUnits() const;
    //! \return the type
    const std::string& getType() const;
    //! \return the short description
    const std::string& getShortDescription() const;
    //! \return the description
    const std::vector<std::string>& getDescription() const;
    //! \return the notes
    const std::vector<std::string>& getNotes() const;
    //! \return if lower physical bound is defined
    bool hasLowerPhysicalBound(const std::string_view) const;
    //! \return the lower physical bound, if defined
    std::string getLowerPhysicalBound(const std::string_view) const;
    //! \return if upper physical bound is defined
    bool hasUpperPhysicalBound(const std::string_view) const;
    //! \return the upper physical bound, if defined
    std::string getUpperPhysicalBound(const std::string_view) const;
    //! \brief cast operator to string. Equivalent to getKey()
    operator const std::string&() const;
    //
    // note that export symbols are required for Microsoft's compiler
    TFELGLOSSARY_VISIBILITY_FRIEND_EXPORT
    friend bool operator<(const GlossaryEntry&, const GlossaryEntry&);
    TFELGLOSSARY_VISIBILITY_FRIEND_EXPORT friend bool operator!=(
        const std::string&, const GlossaryEntry&);
    TFELGLOSSARY_VISIBILITY_FRIEND_EXPORT friend bool operator!=(
        const GlossaryEntry&, const std::string&);
    TFELGLOSSARY_VISIBILITY_FRIEND_EXPORT friend bool operator==(
        const std::string&, const GlossaryEntry&);
    TFELGLOSSARY_VISIBILITY_FRIEND_EXPORT friend bool operator==(
        const GlossaryEntry&, const std::string&);
    //! \brief destructor
    ~GlossaryEntry();

   private:
    //
    friend struct Glossary;
    /*!
     * \brief constructor
     * \param[in] k  : key used to store the entry
     * \param[in] b  : iterator to the first name
     * \param[in] e  : iterator to the last name
     * \param[in] u  : unit, sorted by system
     * \param[in] t  : type
     * \param[in] sd : short description
     * \param[in] d  : description
     * \param[in] no : note
     * \param[in] lbs : lower bounds
     * \param[in] ubs : upper bounds
     */
    GlossaryEntry(const char* const,
                  const char* const* const,
                  const char* const* const,
                  const char* const,
                  const char* const,
                  const char* const,
                  const char* const,
                  const char* const,
                  const char* const,
                  const char* const);
    //! \brief assignement operator
    GlossaryEntry& operator=(const GlossaryEntry&) = delete;
    //! \brief move assignement
    GlossaryEntry& operator=(GlossaryEntry&&) = delete;
    /*!
     * \brief check if this entry is valid.
     * Called by constructors
     */
    void check() const;
    //! \brief key used to store this entry in the glossary
    const std::string key;
    //! \brief list of aliases
    const std::vector<std::string> names;
    //! \brief units of the entry, sorted by unit system
    const std::map<std::string, std::string, std::less<>> units;
    //! \brief type of the entry
    const std::string type;
    //! \brief short description of the entry
    const std::string short_description;
    //! \brief description
    const std::vector<std::string> description;
    //! \brief notes
    const std::vector<std::string> notes;
    //! \brief optional lower physical bounds, sorted by unit systems
    const std::map<std::string, std::string, std::less<>> lower_physical_bounds;
    //! \brief optional upper physical bound
    const std::map<std::string, std::string, std::less<>> upper_physical_bounds;
  };  // end of struct GlossaryEntry

  /*!
   * \brief comparison operator
   */
  TFELGLOSSARY_VISIBILITY_EXPORT bool operator<(const GlossaryEntry&,
                                                const GlossaryEntry&);
  /*!
   * comparison operator
   */
  TFELGLOSSARY_VISIBILITY_EXPORT bool operator!=(const std::string&,
                                                 const GlossaryEntry&);
  /*!
   * comparison operator
   */
  TFELGLOSSARY_VISIBILITY_EXPORT bool operator!=(const GlossaryEntry&,
                                                 const std::string&);
  /*!
   * comparison operator
   */
  TFELGLOSSARY_VISIBILITY_EXPORT bool operator==(const std::string&,
                                                 const GlossaryEntry&);
  /*!
   * comparison operator
   */
  TFELGLOSSARY_VISIBILITY_EXPORT bool operator==(const GlossaryEntry&,
                                                 const std::string&);

}  // end of namespace tfel::glossary

#endif /* LIB_TFEL_GLOSSARY_GLOSSARYENTRY_HXX */
