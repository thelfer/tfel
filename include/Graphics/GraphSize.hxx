/*!
 * \file   GraphSize.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   13 jan 2008
 */

#ifndef _LIB_TFEL_GRAPHSIZE_HXX_
#define _LIB_TFEL_GRAPHSIZE_HXX_ 

namespace tfel{

  namespace graphics
  {

    struct GraphSize
    {
      unsigned short width;  // graph width
      unsigned short height; // graph width
      unsigned short xh0;    // left  graph abscissa
      unsigned short xh1;    // right graph abscissa  
      unsigned short yh0;    // lower graph abscissa
      unsigned short yh1;    // upper graph abscissa
    }; // end of struct GraphSize

  } // end of namespace graphics

} // end of namespace tfel

#endif /* _LIB_TFEL_GRAPHSIZE_HXX */

