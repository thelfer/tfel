/*!
 * \file   include/TFEL/Utilities/TerminalColors.hxx
 * \brief  This file implements the TerminalColors class.
 * \author Thomas Helfer
 * \date   26 Jul 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_TERMINAL_COLORS_HXX
#define LIB_TFEL_TERMINAL_COLORS_HXX

#include "TFEL/Config/TFELConfig.hxx"

namespace tfel {

  namespace utilities {

    /*!
     * \class  TerminalColors
     * \brief  This class contains char sequence corresponding to colors.
     * This enables to write ouput messages in color in the terminal.
     * \author Thomas Helfer
     * \date   26 Jul 2006
     */
    struct TFELUTILITIES_VISIBILITY_EXPORT TerminalColors {
      /*!
       * \brief char sequence correponding to black.
       * \code
       * cout.write(TerminalColors::Black,sizeof(TerminalColors::Black));
       * \endcode
       */
#if !(defined _WIN32 || defined _WIN64 || defined __CYGWIN__)
      static constexpr char Black[5] = {033, '[', '3', '0', 'm'};
#else
      static const char Black[5];
#endif
      /*!
       * \brief char sequence correponding to red.
       * \code
       * cout.write(TerminalColors::Red,sizeof(TerminalColors::Red));
       * \endcode
       */
#if !(defined _WIN32 || defined _WIN64 || defined __CYGWIN__)
      static constexpr char Red[5] = {033, '[', '3', '1', 'm'};
#else
      static const char Red[5];
#endif
      /*!
       * \brief char sequence correponding to green.
       * \code
       * cout.write(TerminalColors::Green,sizeof(TerminalColors::Greeb));
       * \endcode
       */
#if !(defined _WIN32 || defined _WIN64 || defined __CYGWIN__)
      static constexpr char Green[5] = {033, '[', '3', '2', 'm'};
#else
      static const char Green[5];
#endif
      /*!
       * \brief char sequence correponding to yellow.
       * \code
       * cout.write(TerminalColors::Yellow,sizeof(TerminalColors::Yellow));
       * \endcode
       */
#if !(defined _WIN32 || defined _WIN64 || defined __CYGWIN__)
      static constexpr char Yellow[5] = {033, '[', '3', '3', 'm'};
#else
      static const char Yellow[5];
#endif
      /*!
       * \brief char sequence correponding to blue.
       * \code
       * cout.write(TerminalColors::Blue,sizeof(TerminalColors::Blue));
       * \endcode
       */
#if !(defined _WIN32 || defined _WIN64 || defined __CYGWIN__)
      static constexpr char Blue[5] = {033, '[', '3', '4', 'm'};
#else
      static const char Blue[5];
#endif
      /*!
       * \brief char sequence correponding to purple.
       * \code
       * cout.write(TerminalColors::Purple,sizeof(TerminalColors::Purple));
       * \endcode
       */
#if !(defined _WIN32 || defined _WIN64 || defined __CYGWIN__)
      static constexpr char Purple[5] = {033, '[', '3', '5', 'm'};
#else
      static const char Purple[5];
#endif
      /*!
       * \brief char sequence correponding to light blue.
       * \code
       * cout.write(TerminalColors::LightBlue,sizeof(TerminalColors::LightBlue));
       * \endcode
       */
#if !(defined _WIN32 || defined _WIN64 || defined __CYGWIN__)
      static constexpr char LightBlue[5] = {033, '[', '3', '6', 'm'};
#else
      static const char LightBlue[5];
#endif
      /*!
       * \brief char sequence correponding to white.
       * \code
       * cout.write(TerminalColors::White,sizeof(TerminalColors::White));
       * \endcode
       */
#if !(defined _WIN32 || defined _WIN64 || defined __CYGWIN__)
      static constexpr char White[5] = {033, '[', '3', '7', 'm'};
#else
      static const char White[5];
#endif
      /*!
       * \brief char sequence correponding to the reset command.
       * This causes the terminal to go back to its initial state.
       * \code
       * cout.write(TerminalColors::Reset,sizeof(TerminalColors::Reset));
       * \endcode
       */
#if !(defined _WIN32 || defined _WIN64 || defined __CYGWIN__)
      static constexpr char Reset[4] = {033, '[', 'm', 017};
#else
      static const char Reset[4];
#endif
    };

  }  // end of namespace utilities

}  // end of namespace tfel

#endif /* LIB_TFEL_TERMINAL_COLORS_HXX */
