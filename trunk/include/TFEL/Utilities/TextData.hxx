/*!
 * \file   TextData.hxx
 * \brief    
 * \author Helfer Thomas
 * \date   13 Nov 2007
 */

#ifndef _LIB_TFEL_UTILITIES_TEXTDATA_H_
#define _LIB_TFEL_UTILITIES_TEXTDATA_H_ 

#include<vector>
#include<string>

#include"TFEL/Config/TFELConfig.hxx"

namespace tfel
{

  namespace utilities
  {

    /*!
     * \brief class in charge of reading data in a text file
     */
    struct TFEL_VISIBILITY_EXPORT TextData
    {
      /*!
       * \brief helper class in charge of storing of line of data line
       * starting with '#' ared considered comments and not stored.
       * If the first line begins with a '#', the following words are
       * intepreted as the title of each column
       */
      struct Line
      {
	//! line number
	unsigned short nbr;
	//! line tokens
	std::vector<std::string> tokens;
      }; // end of struct Line
      /*!
       * \brief constructor
       * \param[in] file : file name
       */
      TextData(const std::string&);
      /*!
       * \param[in] i : column number
       * \return the specified column, converting all values to double
       * \throw std::runtime_error if a line is shorter than the
       * specified column number or if a value can't be converted to
       * a double
       */
      std::vector<double>
      getColumn(const unsigned short) const;
      /*!
       * \brief extract the specified column, converting all values to
       * double
       * \param[out] tab : column values
       * \param[in]  i   : column number
       * \throw std::runtime_error if a line is shorter than the
       * specified column number or if a value can't be converted to
       * a double
       */
      void
      getColumn(std::vector<double>&,
		const unsigned short) const;
      /*!
       * \return the column having the specified title
       * \param[in] name : column title
       * \throw std::runtime_error if no column with the specified
       * title is found
       */
      unsigned short
      findColumn(const std::string&) const;
      /*!
       * \return the title of the specified column
       * \param[in] c : column number
       * \throw std::runtime_error if no title is found
       */
      std::string
      getLegend(const unsigned short c) const;
      /*!
       * \return an iterator to the first line
       */
      std::vector<Line>::const_iterator
      begin() const;
      /*!
       * \return an iterator past the last line
       */
      std::vector<Line>::const_iterator
      end() const;
    private:
      /*!
       * \brief default constructor (disabled)
       */
      TFEL_VISIBILITY_LOCAL 
      TextData();
      /*!
       * \brief copy constructor (disabled)
       */
      TextData(const TextData&);
      /*!
       * \brief assignement operator (disabled)
       */
      TFEL_VISIBILITY_LOCAL 
      TextData&
      operator = (const TextData&);
      /*!
       * \brief extract a double from a token
       * \param[in] current : iterator to the token
       * \param[in] l       : line number
       * \return the extracted value
       * \throw std::runtime_error if the token can't be converted to
       * a double
       */
      TFEL_VISIBILITY_LOCAL double
      readDouble(const std::vector<std::string>::const_iterator,
		 const unsigned short) const;
      //! list of all tokens of the file, sorted by line
      std::vector<Line>        lines;
      //! list of column titles
      std::vector<std::string> legends;
    }; // end of struct TextData

  } // end of namespace utilities

} // end of namespace tfel

#endif /* _LIB_TFEL_UTILITIES_TEXTDATA_H */
