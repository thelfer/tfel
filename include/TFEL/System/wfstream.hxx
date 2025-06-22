/*!
 * \file   include/TFEL/System/wfstream.hxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   14 nov 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_WFSTREAM_HXX
#define LIB_TFEL_WFSTREAM_HXX

#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "TFEL/Config/TFELConfig.hxx"
#include <memory>
#include "TFEL/System/stream_traits.hxx"
#include "TFEL/System/basic_wstream.hxx"

namespace tfel::system {

  struct wfstream;

  template <>
  struct stream_traits<wfstream> {
    static constexpr bool isBlocking = false;
  };  // end of stream_traits<wfstream>

  struct TFELSYSTEM_VISIBILITY_EXPORT wfstream
      : public basic_wstream<wfstream, stream_traits<wfstream>>,
        protected std::shared_ptr<int> {
    // default constructor
    wfstream();

    // copy constructor
    wfstream(const wfstream&);

    // assignement operator
    wfstream& operator=(const wfstream&);

    wfstream(const std::string&,
             const int = O_CREAT | O_TRUNC | O_WRONLY,
             const mode_t = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

    void open(const std::string&,
              const int = O_CREAT | O_TRUNC | O_WRONLY,
              const mode_t = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

    void close();

    int getFileDescriptor() const;

    // destructor
    ~wfstream();

  };  // end of struct basic_wstream

}  // end of namespace tfel::system

#endif /* LIB_TFEL_WFSTREAM_HXX */
