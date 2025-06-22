/*!
 * \file   include/TFEL/System/wstreamView.hxx
 * \brief
 *
 * \author Helfer Thomas
 * \date   18 nov 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_WSTREAMVIEW_HXX_
#define LIB_TFEL_WSTREAMVIEW_HXX_

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/System/stream_traits.hxx"
#include "TFEL/System/basic_wstream.hxx"

namespace tfel {

  namespace system {

    template <bool isBlocking>
    struct wstreamView;

    template <bool is>
    struct stream_traits<wstreamView<is>> {
      static constexpr bool isBlocking = is;
    };  // end of struct stream_traits<wstreamView<isBlocking> >

    template <bool isBlocking>
    struct wstreamView
        : public basic_wstream<wstreamView<isBlocking>,
                               stream_traits<wstreamView<isBlocking>>> {
      wstreamView(const int);

      int getFileDescriptor(void) const;

     private:
      const int fd;

    };  // end of struct wstreamView

  }  // end of namespace system

}  // end of namespace tfel

#include "TFEL/System/wstreamView.ixx"

#endif /* LIB_TFEL_WSTREAMVIEW_HXX_ */
