/*!
 * \file   include/TFEL/Utilities/TextData.hxx
 * \brief
 * \author Helfer Thomas
 * \date   13 Nov 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_UTILITIES_TEXTDATA_H_
#define LIB_TFEL_UTILITIES_TEXTDATA_H_

#include <vector>
#include <string>

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Utilities/Token.hxx"

namespace tfel {

  namespace utilities {

    /*!
     * \brief class in charge of reading data in a text file
     */
    struct TFELUTILITIES_VISIBILITY_EXPORT TextData {
      //! a simple alias
      using size_type = Token::size_type;
      /*!
       * \brief helper class in charge of storing of line of data line
       * starting with '#' ared considered comments and not stored.
       * If the first line begins with a '#', the following words are
       * intepreted as the title of each column
       */
      struct Line {
        //! line tokens
        std::vector<Token> tokens;
      };  // end of struct Line
      /*!
       * \brief constructor
       * \param[in] file   : file name
       * \param[in] format : file format
       * Available file formats include:
       * - the default. If the first line is a comment, column legends
       *   are extracted out of it. If the next lines are comments,
       *   they are added to the preamble.
       * - gnuplot (also called alcyone). The first line is not a
       *   comment but column legends are extracted out of it. This is
       *   the behaviour of gnuplot when the following command is
       *   used:
       *   set key autotitle columnhead
       */
      TextData(const std::string&, const std::string& = "");
      /*!
       * \param[in] i : column number
       * \return the specified column, converting all values to double
       * \throw std::runtime_error if a line is shorter than the
       * specified column number or if a value can't be converted to
       * a double
       */
      std::vector<double> getColumn(const size_type) const;
      /*!
       * \brief extract the specified column, converting all values to
       * double
       * \param[out] tab : column values
       * \param[in]  i   : column number
       * \throw std::runtime_error if a line is shorter than the
       * specified column number or if a value can't be converted to
       * a double
       */
      void getColumn(std::vector<double>&, const size_type) const;
      /*!
       * \return the column having the specified title
       * \param[in] name : column title
       * \throw std::runtime_error if no column with the specified
       * title is found
       */
      size_type findColumn(const std::string&) const;
      /*!
       * \return the legend associated to the curves
       */
      const std::vector<std::string>& getLegends(void) const;
      /*!
       * \return the title of the specified column
       * \param[in] c : column number
       * \throw std::runtime_error if no title is found
       */
      std::string getLegend(const size_type c) const;
      /*!
       * \return an iterator to the first line
       */
      std::vector<Line>::const_iterator begin() const;
      /*!
       * \return an iterator past the last line
       */
      std::vector<Line>::const_iterator end() const;
      /*!
       * get the first commented lines
       */
      const std::vector<std::string>& getPreamble(void) const;
      /*!
       * skip the first lines of the file
       * \param[in] n : number of lines to be skipped
       */
      void skipLines(const Token::size_type);

     private:
      TextData() = delete;
      TextData(TextData&&) = delete;
      TextData(const TextData&) = delete;
      TextData& operator=(TextData&&) = delete;
      TextData& operator=(const TextData&) = delete;
      //! list of all tokens of the file, sorted by line
      std::vector<Line> lines;
      //! list of column titles
      std::vector<std::string> legends;
      //! first commented lines
      std::vector<std::string> preamble;
    };  // end of struct TextData

  }  // end of namespace utilities

}  // end of namespace tfel

#endif /* LIB_TFEL_UTILITIES_TEXTDATA_H_ */
