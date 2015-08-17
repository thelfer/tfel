/*!
 * \file   MakefileGenerator.hxx
 * \brief    
 * \author THOMAS HELFER
 * \date   16 août 2015
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_MFRONT_MAKEFILEGENERATOR_HXX_
#define _LIB_MFRONT_MAKEFILEGENERATOR_HXX_

#include<string>

namespace mfront{

  // forward declaration
  struct TargetsDescription;
  // forward declaration
  struct GeneratorOptions;
  
  /*!
   * a function generating a standard Makefile from a target
   * description
   * \param[in] t : target description
   * \param[in] o : options
   * \param[in] d : directory
   * \param[in] f : file name
   */
  void generateMakeFile(const TargetsDescription&,
			const GeneratorOptions&,
			const std::string& = "src",
			const std::string& = "Makefile.mfront");

} // end of namespace mfront

#endif /* _LIB_MFRONT_MAKEFILEGENERATOR_HXX_ */



