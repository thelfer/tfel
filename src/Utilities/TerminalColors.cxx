/*!
 * \file   src/Utilities/TerminalColors.cxx
 * \brief
 * \author Thomas Helfer
 * \date   04 sep 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "TFEL/Utilities/TerminalColors.hxx"

namespace tfel {

  namespace utilities {

#if !(defined _WIN32 || defined _WIN64 || defined __CYGWIN__)
    constexpr char TerminalColors::Black[];
    constexpr char TerminalColors::Red[];
    constexpr char TerminalColors::Green[];
    constexpr char TerminalColors::Yellow[];
    constexpr char TerminalColors::Blue[];
    constexpr char TerminalColors::Purple[];
    constexpr char TerminalColors::LightBlue[];
    constexpr char TerminalColors::White[];
    constexpr char TerminalColors::Reset[];
#else
    const char TerminalColors::Black[5] = {033, '[', '3', '0', 'm'};
    const char TerminalColors::Red[5] = {033, '[', '3', '1', 'm'};
    const char TerminalColors::Green[5] = {033, '[', '3', '2', 'm'};
    const char TerminalColors::Yellow[5] = {033, '[', '3', '3', 'm'};
    const char TerminalColors::Blue[5] = {033, '[', '3', '4', 'm'};
    const char TerminalColors::Purple[5] = {033, '[', '3', '5', 'm'};
    const char TerminalColors::LightBlue[5] = {033, '[', '3', '6', 'm'};
    const char TerminalColors::White[5] = {033, '[', '3', '7', 'm'};
    const char TerminalColors::Reset[4] = {033, '[', 'm', 017};
#endif

  }  // end of namespace utilities

}  // end of namespace tfel
