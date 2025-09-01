/*!
 * \file   include/TFEL/System/rfstream.hxx
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

#ifndef LIB_TFEL_SYSTEM_RFSTREAM_HXX
#define LIB_TFEL_SYSTEM_RFSTREAM_HXX

#include <string>
#include <memory>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/System/stream_traits.hxx"
#include "TFEL/System/basic_rstream.hxx"

namespace tfel::system {

  // forward declaration
  struct rfstream;

  /*!
   * \brief partial specialisation of the `stream_traits` for the
   * `rfstream` class.
   */
  template <>
  struct stream_traits<rfstream> {
    static constexpr bool isBlocking = false;
  };  // end of stream_traits<rfstream>

  //! \brief a simple wrapper around stream
  struct TFELSYSTEM_VISIBILITY_EXPORT rfstream
      : public basic_rstream<rfstream, stream_traits<rfstream>>,
        protected std::shared_ptr<int> {
    //! default constructor
    rfstream();

    //! copy constructor
    rfstream(const rfstream&);

    // assignement operator
    rfstream& operator=(const rfstream&);

    rfstream(const std::string&, const int = O_RDONLY);

    void open(const std::string&, const int = O_RDONLY);

    void close();

    int getFileDescriptor() const;

    //! \brief destructor
    ~rfstream();

  };  // end of struct basic_wstream

}  // end of namespace tfel::system

#endif /* LIB_TFEL_SYSTEM_RFSTREAM_HXX */
