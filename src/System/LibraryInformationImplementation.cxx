/*!
 * \file   LibraryInformationImplementation.cxx
 * \brief    
 * \author Thomas Helfer
 * \date   28/03/2017
 *
 * \note This file contains code that has been extracted from the
 * boost/dll library version 1.63 and has been originally written by
 * Antony Polukhin, Renato Tegon Forti and Antony Polukhin.
 */

struct LibraryInformation::LibraryInformation::Implementation
{
  virtual std::vector<std::string> sections() = 0;
  virtual std::vector<std::string> symbols() = 0;
  virtual std::vector<std::string> symbols(const char*) = 0;
  virtual ~Implementation() = default;
}; // end of struct LibraryInformation::LibraryInformation::Implementation
