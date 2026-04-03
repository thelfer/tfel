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

namespace tfel::utilities {

  /*!
   * \class  TerminalColors
   * \brief  This class contains char sequence corresponding to colors.
   * This enables to write ouput messages in color in the terminal.
   * \author Thomas Helfer
   * \date   26 Jul 2006
   */
  struct TerminalColors {
    /*!
     * \brief char sequence correponding to black.
     * \code
     * cout.write(TerminalColors::Black,sizeof(TerminalColors::Black));
     * \endcode
     */
    static constexpr char Black[5] = {033, '[', '3', '0', 'm'};
    /*!
     * \brief char sequence correponding to red.
     * \code
     * cout.write(TerminalColors::Red,sizeof(TerminalColors::Red));
     * \endcode
     */
    static constexpr char Red[5] = {033, '[', '3', '1', 'm'};
    /*!
     * \brief char sequence correponding to green.
     * \code
     * cout.write(TerminalColors::Green,sizeof(TerminalColors::Greeb));
     * \endcode
     */
    static constexpr char Green[5] = {033, '[', '3', '2', 'm'};
    /*!
     * \brief char sequence correponding to yellow.
     * \code
     * cout.write(TerminalColors::Yellow,sizeof(TerminalColors::Yellow));
     * \endcode
     */
    static constexpr char Yellow[5] = {033, '[', '3', '3', 'm'};
    /*!
     * \brief char sequence correponding to blue.
     * \code
     * cout.write(TerminalColors::Blue,sizeof(TerminalColors::Blue));
     * \endcode
     */
    static constexpr char Blue[5] = {033, '[', '3', '4', 'm'};
    /*!
     * \brief char sequence correponding to purple.
     * \code
     * cout.write(TerminalColors::Purple,sizeof(TerminalColors::Purple));
     * \endcode
     */
    static constexpr char Purple[5] = {033, '[', '3', '5', 'm'};
    /*!
     * \brief char sequence correponding to light blue.
     * \code
     * cout.write(TerminalColors::LightBlue,sizeof(TerminalColors::LightBlue));
     * \endcode
     */
    static constexpr char LightBlue[5] = {033, '[', '3', '6', 'm'};
    /*!
     * \brief char sequence correponding to white.
     * \code
     * cout.write(TerminalColors::White,sizeof(TerminalColors::White));
     * \endcode
     */
    static constexpr char White[5] = {033, '[', '3', '7', 'm'};
    /*!
     * \brief char sequence correponding to the reset command.
     * This causes the terminal to go back to its initial state.
     * \code
     * cout.write(TerminalColors::Reset,sizeof(TerminalColors::Reset));
     * \endcode
     */
    static constexpr char Reset[4] = {033, '[', 'm', 017};
  };

}  // end of namespace tfel::utilities

#endif /* LIB_TFEL_TERMINAL_COLORS_HXX */
