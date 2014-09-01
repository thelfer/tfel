/*!
 * \file   include/TFEL/Utilities/TerminalColors.hxx
 * \brief  This file implements the TerminalColors class. 
 * \author Helfer Thomas
 * \date   26 Jul 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_TERMINAL_COLORS_H_
#define _LIB_TFEL_TERMINAL_COLORS_H_ 

#include"TFEL/Config/TFELConfig.hxx"

namespace tfel{

  namespace utilities{

    /*!
     * \class  TerminalColors
     * \brief  This class contains char sequence corresponding to colors. 
     * This enables to write ouput messages in color in the terminal.
     * \author Helfer Thomas
     * \date   26 Jul 2006
     */
    struct TFELUTILITIES_VISIBILITY_EXPORT TerminalColors
    {
      /*!
       * \brief char sequence correponding to black.
       * \code
       * cout.write(TerminalColors::Black,sizeof(TerminalColors::Black));
       * \endcode
       */
      static const char Black[5];

      /*!
       * \brief char sequence correponding to red.
       * \code
       * cout.write(TerminalColors::Red,sizeof(TerminalColors::Red));
       * \endcode
       */
      static const char Red[5];

      /*!
       * \brief char sequence correponding to green.
       * \code
       * cout.write(TerminalColors::Green,sizeof(TerminalColors::Greeb));
       * \endcode
       */
      static const char Green[5];

      /*!
       * \brief char sequence correponding to yellow.
       * \code
       * cout.write(TerminalColors::Yellow,sizeof(TerminalColors::Yellow));
       * \endcode
       */
      static const char Yellow[5];

      /*!
       * \brief char sequence correponding to blue.
       * \code
       * cout.write(TerminalColors::Blue,sizeof(TerminalColors::Blue));
       * \endcode
       */
      static const char Blue[5];

      /*!
       * \brief char sequence correponding to purple.
       * \code
       * cout.write(TerminalColors::Purple,sizeof(TerminalColors::Purple));
       * \endcode
       */
      static const char Purple[5];

      /*!
       * \brief char sequence correponding to light blue.
       * \code
       * cout.write(TerminalColors::LightBlue,sizeof(TerminalColors::LightBlue));
       * \endcode
       */
      static const char LightBlue[5];

      /*!
       * \brief char sequence correponding to white.
       * \code
       * cout.write(TerminalColors::White,sizeof(TerminalColors::White));
       * \endcode
       */
      static const char White[5];

      /*!
       * \brief char sequence correponding to the reset command.
       * This causes the terminal to go back to its initial state.
       * \code
       * cout.write(TerminalColors::Reset,sizeof(TerminalColors::Reset));
       * \endcode
       */
      static const char Reset[4];

    };

  } // end of namespace utilities

} // end of namespace tfel

#endif /* _LIB_TFEL_TERMINAL_COLORS_H */

