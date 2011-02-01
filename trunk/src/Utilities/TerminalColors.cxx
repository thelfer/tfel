/*!
 * \file   TerminalColors.cxx
 * \brief  
 * \author Helfer Thomas
 * \date   04 sep 2006
 */

#include"TFEL/Utilities/TerminalColors.hxx"

namespace tfel{

  namespace utilities{

    const char TerminalColors::Black[]     = {033, '[', '3', '0', 'm'};
    const char TerminalColors::Red[]       = {033, '[', '3', '1', 'm'};      
    const char TerminalColors::Green[]     = {033, '[', '3', '2', 'm'};      
    const char TerminalColors::Yellow[]    = {033, '[', '3', '3', 'm'};      
    const char TerminalColors::Blue[]      = {033, '[', '3', '4', 'm'};      
    const char TerminalColors::Purple[]    = {033, '[', '3', '5', 'm'};      
    const char TerminalColors::LightBlue[] = {033, '[', '3', '6', 'm'};      
    const char TerminalColors::White[]     = {033, '[', '3', '7', 'm'};      
    const char TerminalColors::Reset[]     = {033, '[', 'm', 017};      

  } // end of namespace utilities

} // end of namespace tfel
