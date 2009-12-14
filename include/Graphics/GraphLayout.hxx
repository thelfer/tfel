/*!
 * \file   GraphLayout.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   13 jan 2008
 */

#ifndef _LIB_TFEL_GRAPHLAYOUT_HXX_
#define _LIB_TFEL_GRAPHLAYOUT_HXX_ 

namespace tfel{

  namespace graphics
  {

    struct GraphLayout
    {
      double ax;
      double bx;
      double ay;
      double by;
      double ax2;
      double bx2;
      double ay2;
      double by2;
      unsigned short ml;  // left margin size
      unsigned short mr;  // right margin size
      unsigned short mu;  // up margin size
      unsigned short md;  // down margin size
      unsigned short ttl; // left title size
      unsigned short ttr; // right title size
      unsigned short ttu; // up title size
      unsigned short ttd; // down title size
      unsigned short ll;  // left label size
      unsigned short lr;  // right label size
      unsigned short lu;  // up label size
      unsigned short ld;  // down label size
      unsigned short tl;  // left tics size
      unsigned short tr;  // right tics size
      unsigned short tu;  // up tics size
      unsigned short td;  // down tics size
      unsigned short gl;  // left  graph margin size
      unsigned short gr;  // right graph margin size
      unsigned short gu;  // upper graph margin size
      unsigned short gd;  // down  graph margin size
      unsigned short bl;  // left border size
      unsigned short br;  // right border size
      unsigned short bu;  // upper border size
      unsigned short bd;  // down border size
    }; // end of struct GraphLayout

  } // end of namespace graphics

} // end of namespace tfel

#endif /* _LIB_TFEL_GRAPHLAYOUT_HXX */

