/*!
 * \file  mfront/include/MFront/FileDescription.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 14 jun 2010
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_MFRONTFILEDESCRIPTION_HXX
#define LIB_MFRONT_MFRONTFILEDESCRIPTION_HXX

#include <string>
#include "MFront/MFrontConfig.hxx"

namespace mfront {

  /*!
   * this class gathers some information about the MFront file being
   * treatd
   */
  struct MFRONT_VISIBILITY_EXPORT FileDescription {
    FileDescription();
    FileDescription(FileDescription&&);
    FileDescription(const FileDescription&);
    FileDescription& operator=(FileDescription&&);
    FileDescription& operator=(const FileDescription&);
    std::string fileName;
    std::string authorName;
    std::string date;
    std::string description;
    ~FileDescription();
  };  // end of struct FileDescription

  //! typedef provided for backward compatibility
  using GenericData = FileDescription;

}  // end of namespace mfront

#endif /* LIB_MFRONT_MFRONTFILEDESCRIPTION_HXX */
