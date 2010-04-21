/*!
 * \file   TextData.hh
 * \brief    
 * \author Helfer Thomas
 * \date   13 Nov 2007
 */

#ifndef _LIB_TFEL_TEXTDATA_H_
#define _LIB_TFEL_TEXTDATA_H_ 

#include<vector>
#include<string>

#include"TFEL/Config/TFELConfig.hxx"

namespace tfel
{

  namespace utilities
  {

      struct TFEL_VISIBILITY_EXPORT TextData
      {
	struct Line
	{
	  unsigned short nbr;
	  std::vector<std::string> tokens;
	};
	TextData(const std::string&);
	std::vector<double>
	getColumn(const unsigned short) const;
	void
	getColumn(std::vector<double>&,
		  const unsigned short) const;
	unsigned short
	findColumn(const std::string&) const;
	std::string
	getLegend(const unsigned short c) const;
	std::vector<Line>::const_iterator
	begin() const;
	std::vector<Line>::const_iterator
	end() const;
      private:
	// Default constructor (disabled)
	TFEL_VISIBILITY_LOCAL 
	TextData();
	// Copy constructor (disabled)
	TextData(const TextData&);
	// assignement operator (disabled)
	TFEL_VISIBILITY_LOCAL 
	TextData&
	operator = (const TextData&);
	//! extract a double from a token
	TFEL_VISIBILITY_LOCAL 
	double
	readDouble(const std::vector<std::string>::const_iterator,
		   const unsigned short) const;
	//! list of all tokens of the file, sorted by line
	std::vector<Line>        lines;
	std::vector<std::string> legends;
      }; // end of struct TextData

  } // end of namespace utilities

} // end of namespace tfel

#endif /* _LIB_TFEL_TEXTDATA_H */

