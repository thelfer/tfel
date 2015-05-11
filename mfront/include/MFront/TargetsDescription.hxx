/*! 
 * \file  TargetsDescription.hxx
 * \brief
 * \author Helfer Thomas
 * \date   17 mars 2015
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MFRONT_TARGETSDESCRIPTION_H
#define LIB_MFRONT_TARGETSDESCRIPTION_H 

#include<map>
#include<string>
#include<vector>

#include"MFront/MFrontConfig.hxx"

namespace mfront{

  /*!
   * \brief structure containing the results of the analysis of a
   * MFront file. This structure is used to build the compilation
   * rules for the libraryies and specific targets defined by MFront
   * interfaces.
   * - the generated sources and headers, sorted by libraries
   * - the dependencies of the generated libraries
   * - the generated entry points (function or class names), sorted by library
   * - the specific targets
   */
  struct TargetsDescription{
    //! generated sources, sorted by library
    std::map<std::string,std::vector<std::string>> sources;
    //! generated headers
    std::vector<std::string> headers;
    //! additional preprocessor flags, sorted by library
    std::map<std::string,std::vector<std::string>> cppflags;
    //! the dependencies of libraries on mfront generated libraries, sorted by library
    std::map<std::string,std::vector<std::string>> dependencies;
    //! generated entry points, sorted by library
    std::map<std::string,std::vector<std::string> > epts;
    /*!
     * Specific targets and associated dependencies to other targets
     * (for example, mfront generated library) and commands required
     * to build the target.
     *
     * A specific target will define the following Makefile rule:
     * \code{.txt}
     * target : dep1 dep2 ...
     *    cmd1
     *    cmd2
     *    ....
     * \endcode
     */
    std::map<std::string,                           //< target name
	     std::pair<std::vector<std::string>,    //< dependencies to other targets
		       std::vector<std::string>     //< commands
		       >> specific_targets; 
    //! targets description of mfront dependencies (material properties)
    std::vector<TargetsDescription> tds;
  }; // end of struct TargetsDescription

  /*!
   * \brief merge two targets description
   * \param[out] d : destination
   * \param[in]  s : source
   * \note the targets description shall not
   */
  MFRONT_VISIBILITY_EXPORT
  void mergeTargetsDescription(TargetsDescription&,
			       const TargetsDescription&);

} // end of namespace mfront

#endif /* LIB_MFRONT_TARGETSDESCRIPTION_H */

