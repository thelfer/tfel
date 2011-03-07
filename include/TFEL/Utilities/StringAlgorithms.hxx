/*! 
 * \file  StringAlgorithms.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 03 mar 2011
 */

#ifndef _LIB_TFEL_UTILITIES_STRINGALGORITHMS_H_
#define _LIB_TFEL_UTILITIES_STRINGALGORITHMS_H_ 

#include<string>

#include"TFEL/Config/TFELConfig.hxx"

namespace tfel{

  namespace utilities{

    /*!
     * \brief replace all occurences of substring s1 by s2
     * \param s  : original string
     * \param s1 : substring to be changed
     * \param s2 : replacement string
     * \param ps : beginning of the search
     * \return a copy of s where all occurences of substring s1 have
     * been replaced by s2
     */
    TFEL_VISIBILITY_EXPORT std::string
    replace_all(const std::string&,
		const std::string&,
		const std::string&,
		const std::string::size_type = 0u);

    /*!
     * \brief replace all occurences of substring s1 by s2
     * \param r  : result
     * \param s  : original string
     * \param s1 : substring to be changed
     * \param s2 : replacement string
     * \param ps : beginning of the search
     */
    TFEL_VISIBILITY_EXPORT void
    replace_all(std::string&,
		const std::string&,
		const std::string&,
		const std::string&,
		const std::string::size_type = 0u);

  } // end of namespace utilities

} // end of namespace tfel

#endif /* _LIB_TFEL_UTILITIES_STRINGALGORITHMS_H */

