/*!
 * \file  FileDescription.cxx
 * \brief
 * \author Helfer Thomas
 * \date   03 mars 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "MFront/FileDescription.hxx"

namespace mfront {

  FileDescription::FileDescription() = default;
  FileDescription::FileDescription(FileDescription&&) = default;
  FileDescription::FileDescription(const FileDescription&) = default;
  FileDescription& FileDescription::operator=(FileDescription&&) = default;
  FileDescription& FileDescription::operator=(const FileDescription&) = default;
  FileDescription::~FileDescription() = default;

}  // end of namespace mfront
