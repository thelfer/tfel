/*!
 * \file   include/TFEL/UnicodeSupport/UnicodeSupport.hxx
 * \brief
 * \author Thomas Helfer
 * \date   15/07/2019
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_UNICODE_UNICODESUPPORT_HXX
#define LIB_TFEL_UNICODE_UNICODESUPPORT_HXX

#include <vector>
#include <string>

#include "TFEL/Config/TFELConfig.hxx"

namespace tfel {

  namespace unicode {

    /*!
     * \brief list of categories
     */
    enum struct UnicodeCharacterCategory {
      MATHEMATICAL_SYMBOL,
      MATHEMATICAL_OPERATOR,
      GREEK_LETTER_CAPITAL,
      GREEK_LETTER_SMALL,
      GREEK_LETTER_SUBSCRIPT_SMALL,
      LATIN_LETTER_SUPERSCRIPT_CAPITAL,
      LATIN_LETTER_SUPERSCRIPT_SMALL,
      LATIN_LETTER_SUBSCRIPT_CAPITAL,
      LATIN_LETTER_SUBSCRIPT_SMALL,
      ARABIC_NUMBER_SUPERSCRIPT,
      ARABIC_NUMBER_SUBSCRIPT
    };  // end of UnicodeCharacterCategory

    /*!
     * \brief description of an unicode character
     */
    struct UnicodeCharacterDescription {
      //! \brief unicode character
      const char* const uc;
      //! \brief TFEL' mangling
      const char* const m;
      //! \brief description
      const char* const d;
      //! \brief category
      const UnicodeCharacterCategory c;
    };

    /*!
     * \return the list of supported unicode characters
     */
    TFELUNICODESUPPORT_VISIBILITY_EXPORT const
        std::vector<UnicodeCharacterDescription>&
        getSupportedUnicodeCharactersDescriptions();
    /*!
     * \return a mangled version of the given string
     * \param[in] s: string to be mangled
     */
    TFELUNICODESUPPORT_VISIBILITY_EXPORT std::string getMangledString(
        const std::string&);

  }  // end of namespace unicode

}  // end of namespace tfel

#endif /* LIB_TFEL_UNICODE_UNICODESUPPORT_HXX */
