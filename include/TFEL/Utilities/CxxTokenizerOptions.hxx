/*!
 * \file   CxxTokenizerOptions.hxx
 * \brief
 * \author Thomas Helfer
 * \date   6/02/018
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_UTILITIES_CXXTOKENIZEROPTIONS_HXX
#define LIB_TFEL_UTILITIES_CXXTOKENIZEROPTIONS_HXX

#include "TFEL/Config/TFELConfig.hxx"

namespace tfel {

  namespace utilities {

    /*!
     * \brief list of options that can be used to tune the behaviour of the
     * `CxxTokenizer` class.
     */
    struct TFELUTILITIES_VISIBILITY_EXPORT CxxTokenizerOptions {
      //! default constructor
      CxxTokenizerOptions();
      //! copy constructor
      CxxTokenizerOptions(const CxxTokenizerOptions &);
      //! move constructor
      CxxTokenizerOptions(CxxTokenizerOptions &&);
      //! standard assignement
      CxxTokenizerOptions &operator=(const CxxTokenizerOptions &);
      //! move assignement
      CxxTokenizerOptions &operator=(CxxTokenizerOptions &&);
      //! destructor
      ~CxxTokenizerOptions();
      //! \brief if true, keep the comment boundaries
      bool bKeepCommentBoundaries = false;
      //! if true, successive strings are merged
      bool shallMergeStrings = false;
      //! allow stray hash character
      bool allowStrayHashCharacter = false;
      //! the hash character can start a comment, as in python, shell, etc..
      bool treatHashCharacterAsCommentDelimiter = false;
      //! allow stray backslash
      bool allowStrayBackSlash = false;
      //! treat preprocessor directives
      bool treatPreprocessorDirectives = true;
      /*!
       * \brief treat strings and characters
       * If `false`, the `'` and '"' are treated as indivdual tokens
       */
      bool treatStrings = true;
      //! \brief extract number
      bool treatNumbers = true;
      //! \brief treat comments
      bool treatCComments = true;
      //! \brief treat comments
      bool treatCxxComments = true;
      /*!
       * In `C` and `C++`, two separator characters can be associated to
       * create a new separator: `->`, `&=`. If this flag is true, these
       * separators will be grouped in a single token.
       */
      bool joinCxxTwoCharactersSeparators = true;
      /*!
       * \brief strings and characters
       */
      bool graveAccentAsSeparator = true;
      /*!
       * if `true`, one consider that single quote can be used to
       * define a string
       * \note this only has effect if `treatStrings` is true
       */
      bool charAsString = false;
      //! treat `.` as a separator
      bool dotAsSeparator = true;
      //! treat `-` as a separator
      bool minusAsSeparator = true;
    };  // end of struct CxxTokenizerOptions

  }  // end of namespace utilities

}  // end of namespace tfel

#endif /* LIB_TFEL_UTILITIES_CXXTOKENIZEROPTIONS_HXX */
