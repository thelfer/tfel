/*!
 * \file   GNUGraphTheme.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   13 jan 2008
 */

#ifndef _LIB_TPLOT_GNUGRAPHTHEME_HXX_
#define _LIB_TPLOT_GNUGRAPHTHEME_HXX_ 

#include <cairomm/context.h>

#include"TFEL/Graphics/StandardGraphTheme.hxx"

namespace tfel{

  namespace graphics
  {

    struct GNUGraphTheme
      : public StandardGraphTheme
    {
      virtual void drawGraphBackGround(Cairo::RefPtr<Cairo::Context>&,
				       const GraphLayout&,
				       const GraphSize&) const;
      virtual ~GNUGraphTheme();
    }; // end of struct GNUGraphTheme

  } // end of namespace graphics

} // end of namespace tfel

#endif /* _LIB_TPLOT_GNUGRAPHTHEME_HXX */

