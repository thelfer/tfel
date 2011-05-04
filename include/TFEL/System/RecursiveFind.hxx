/*! 
 * \file  RecursiveFind.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 03 mai 2011
 */

#ifndef _LIB_TFEL_SYSTEM_RECURSIVEFIND_H_
#define _LIB_TFEL_SYSTEM_RECURSIVEFIND_H_ 

#include<map>
#include<vector>
#include<string>

#include<sys/types.h>
#include<regex.h>

#include"TFEL/Config/TFELConfig.hxx"

namespace tfel
{

  namespace system
  {

    /*!
     * \brief recursively search files matching a given regular
     * expression from the given directory.
     * \param re     : a regular expression
     * \param name   : name of the directory from which the search starts
     * \param depth  : depth reached in the recursive search
     * \param mdepth : maximum depth allowed
     * \return the matching files, ordered by directories
     */
    TFEL_VISIBILITY_EXPORT std::map<std::string,std::vector<std::string> >
    recursiveFind(const std::string&,
		  const std::string&,
		  const unsigned short = 0u,
		  const unsigned short = 100u);

    /*!
     * \brief recursively search files matching a given regular
     * expression from the given directory.
     * \param r      : the matching files, ordered by directories
     * \param re     : a regular expression
     * \param name   : name of the directory from which the search starts
     * \param depth  : depth reached in the recursive search
     * \param mdepth : maximum depth allowed
     */
    TFEL_VISIBILITY_EXPORT void
    recursiveFind(std::map<std::string,std::vector<std::string> >&,
		  const std::string&,
		  const std::string&,
		  const unsigned short = 0u,
		  const unsigned short = 100u);

    /*!
     * \brief recursively search files matching a given regular
     * expression from the given directory.
     * \param re     : a regular expression
     * \param name   : name of the directory from which the search starts
     * \param depth  : depth reached in the recursive search
     * \param mdepth : maximum depth allowed
     * \return the matching files, ordered by directories
     */
    TFEL_VISIBILITY_EXPORT std::map<std::string,std::vector<std::string> >
    recursiveFind(const regex_t&,
		  const std::string&,
		  const unsigned short = 0u,
		  const unsigned short = 100u);

    /*!
     * \brief recursively search files matching a given regular
     * expression from the given directory.
     * \param r      : the matching files, ordered by directories
     * \param re     : a regular expression
     * \param name   : name of the directory from which the search starts
     * \param depth  : depth reached in the recursive search
     * \param mdepth : maximum depth allowed
     */
    TFEL_VISIBILITY_EXPORT void
    recursiveFind(std::map<std::string,std::vector<std::string> >&,
		  const regex_t&,
		  const std::string&,
		  const unsigned short = 0u,
		  const unsigned short = 100u);
 
  } // end of namespace system

} // end of namespace tfel

#endif /* _LIB_TFEL_SYSTEM_RECURSIVEFIND_H */

