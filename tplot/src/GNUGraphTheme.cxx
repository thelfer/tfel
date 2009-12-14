/*!
 * \file   SimpleGraphTheme.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   13 jan 2008
 */

#include<iostream>
#include<cstdlib>

#include <librsvg/rsvg.h>
#include <librsvg/rsvg-cairo.h>

#include"Graphics/ThemeManager.hxx"
#include"Graphics/GNUGraphTheme.hxx"

namespace tfel
{

  namespace graphics
  {
    
    void
    GNUGraphTheme::drawGraphBackGround(Cairo::RefPtr<Cairo::Context>& cr,
					  const GraphLayout&,
					  const GraphSize& s) const
    {
      using namespace std;
      RsvgHandle* handle;
      GError *error;
      RsvgDimensionData dimension;
      double scaleFactor;
      handle =  rsvg_handle_new_from_file("gnu.svg", &error);
      if(handle==0){
	cerr << "load failed" << endl;
	exit(-1);
      }
      cr->save();
      rsvg_handle_get_dimensions(handle,&dimension);
      scaleFactor=static_cast<double>(s.yh1-s.yh0)/static_cast<double>(dimension.height);
      cr->translate(static_cast<unsigned short>(s.xh0+0.5*(s.xh1-s.xh0)-0.5*scaleFactor*dimension.width),
		    static_cast<unsigned short>(s.yh0));
      cr->scale(scaleFactor,scaleFactor);
      rsvg_handle_render_cairo(handle,cr->cobj());
      cr->restore();
      rsvg_handle_free(handle);
    } // end of GNUGraphTheme::drawGraphBackGround

    GNUGraphTheme::~GNUGraphTheme()
    {} // end of GNUGraphTheme::~GNUGraphTheme

    StandardThemeProxy<GNUGraphTheme> gnuGraphThemeProxy("GNUGraphTheme");

  } // end of namespace graphics

} // end of namespace tfel


