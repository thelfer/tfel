/*!
 * \file   MFrontUtilities.hxx
 * \brief This header declares a few general functions
 * \author Thomas Helfer
 * \date   15/08/2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_MFRONTUTILITIES_HXX
#define LIB_MFRONT_MFRONTUTILITIES_HXX

#include <map>
#include <tuple>
#include <string>
#include <vector>
#include <iosfwd>
#include "MFront/MFrontConfig.hxx"

namespace tfel::glossary {

  // forward declaration
  struct GlossaryEntry;

}  // namespace tfel::glossary

namespace mfront {

  // forward declaration
  struct VariableBoundsDescription;
  // forward declaration
  struct MFrontPathSpecifier;

  /*!
   * \brief add the given symbol.
   * \param[in,out] symbols: mapping between a symbol and a replacement string
   * \param[in] s: symbol
   * \param[in] r: replacement string
   */
  MFRONT_VISIBILITY_EXPORT void addSymbol(std::map<std::string, std::string>&,
                                          const std::string_view,
                                          const std::string_view);
  /*!
   * \return the keys of a map
   * \param[in] m: map
   */
  template <typename Value>
  std::vector<std::string> getKeys(const std::map<std::string, Value>&);
  /*!
   * \brief conditionnally insert a string into
   * a vector of string : an element is inserted if not already
   * present or empty.
   * \param[out] d : destination
   * \param[in]  v : value
   */
  MFRONT_VISIBILITY_EXPORT void insert_if(std::vector<std::string>&,
                                          const std::string&);
  /*!
   * \brief conditionnally insert a string into
   * a vector of string : an element is inserted if not already
   * present or has zero size or is nulltpr.
   * \param[out] d : destination
   * \param[in]  v : value
   */
  MFRONT_VISIBILITY_EXPORT void insert_if(std::vector<std::string>&,
                                          const char* const);

  /*!
   * \brief call insert_if on every element of a
   * container.
   * \pre Container::value_type must be convertible to a string
   * \tparam Container : type of source
   * \param[out] d : destination
   * \param[in]  s : source
   */
  template <typename Container>
  void insert_if(std::vector<std::string>&, const Container&);

  /*!
   * \brief call insert_if on every element of an
   * initializer_list.
   * \pre Value must be convertible to a string
   * \tparam Value : type of the objects of the initializer list
   * \param[out] d : destination
   * \param[in]  s : source
   */
  template <typename Value>
  void insert_if(std::vector<std::string>&,
                 const std::initializer_list<Value>&);
  /*!
   * \brief write a vector of string to the output stream
   * \param[in] os : output stream
   * \param[in] v  : vector of string
   * \param[in] id : identifier
   */
  MFRONT_VISIBILITY_EXPORT void write(std::ostream&,
                                      const std::vector<std::string>&,
                                      const std::string&);

  MFRONT_VISIBILITY_EXPORT std::string makeUpperCase(std::string_view);

  MFRONT_VISIBILITY_EXPORT std::string makeLowerCase(std::string_view);

  /*!
   * \brief display the complete description of a glossary entry
   * \param[out] os : output stream
   * \param[in]  e  : glossary entry
   */
  MFRONT_VISIBILITY_EXPORT void displayGlossaryEntryCompleteDescription(
      std::ostream&, const tfel::glossary::GlossaryEntry&);
  /*!
   * \brief extract the name of a variable and an array position from a string.
   * \param[in] n: variable name and array position
   * \return a tuple giving the variable name, the fact that a
   * position was read, the read position if any.
   *
   * For example, calling this function with the `A[10]` argument
   * results in the following tuple: `{"A",true,10}`.
   *
   * With `B` as argument, the result is the following tuple:
   * `{"B",false,0}`.
   */
  MFRONT_VISIBILITY_EXPORT
  std::tuple<std::string, bool, unsigned short>
  extractVariableNameAndArrayPosition(const std::string&);

}  // namespace mfront

#include "MFront/MFrontUtilities.ixx"

#endif /* LIB_MFRONT_MFRONTUTILITIES_HXX */
