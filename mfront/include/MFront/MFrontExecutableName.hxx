/*! 
 * \file  MFrontExecutableName.hxx
 * \brief
 * \author Helfer Thomas
 * \date   03 mars 2015
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MFRONT_MFRONTEXECUTABLENAME_H
#define LIB_MFRONT_MFRONTEXECUTABLENAME_H 

#include<string>
#include"MFront/MFrontConfig.hxx"

namespace mfront{

  /*!
   * \brief set the name of the MFront executable (given by argv[0]).
   * \param[in] n : the name of the MFront executable
   */
  MFRONT_VISIBILITY_EXPORT void
  setMFrontExecutableName(const char* const);

  /*!
   * \return the name of the MFront executable.
   */
  MFRONT_VISIBILITY_EXPORT const std::string&
  getMFrontExecutableName(void);

} // end of namespace mfront

#endif /* LIB_MFRONT_MFRONTEXECUTABLENAME_H */

