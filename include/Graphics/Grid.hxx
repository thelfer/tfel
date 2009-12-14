/*!
 * \file   GraphTheme.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   03 jan 2008
 */

#ifndef _LIB_TFEL_GRAPHICS_GRID_HXX_
#define _LIB_TFEL_GRAPHICS_GRID_HXX_ 

namespace tfel{

  namespace graphics
  {

    struct Grid
    {
      static const unsigned short None = 0;
      static const unsigned short X    = 1 << 1;
      static const unsigned short X2   = 1 << 2;
      static const unsigned short Y    = 1 << 3;
      static const unsigned short Y2   = 1 << 4;
    }; // end of struct Grid

  } // end of namespace graphics

} // end of namespace tfel

#endif _LIB_TFEL_GRAPHICS_GRID_HXX_ 
