/*! 
 * \file  mfront/include/MFront/FileDescription.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 14 jun 2010
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MFRONT_MFRONTFILEDESCRIPTION_HXX
#define LIB_MFRONT_MFRONTFILEDESCRIPTION_HXX 

#include<set>
#include<string>

#include"MFront/StaticVariableDescription.hxx"

namespace mfront
{

  /*!
   * this class gathers some information about the MFront file being
   * treatd
   */
  struct FileDescription
  {
    std::string fileName;    
    std::string authorName;
    std::string date;
    std::string description;
  }; // end of struct FileDescription

  //! typedef provided
  typedef FileDescription GenericData;

} // end of namespace mfront

#endif /* LIB_MFRONT_MFRONTFILEDESCRIPTION_HXX */

