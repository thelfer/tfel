/*!
 * \file   ElegantGraphTheme2.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   13 jan 2008
 */

#ifndef _LIB_TPLOT_ELEGANTGRAPHTHEME2_HXX_
#define _LIB_TPLOT_ELEGANTGRAPHTHEME2_HXX_ 

#include <cairomm/context.h>

#include"Graphics/ElegantGraphTheme.hxx"

namespace tfel{

  namespace graphics
  {

    struct ElegantGraphTheme2
      : public ElegantGraphTheme
    {
      virtual ~ElegantGraphTheme2();
    protected:
      virtual void
      drawHorizontalBox(Cairo::RefPtr<Cairo::Context>&,
			const unsigned short,
			const unsigned short,
			const unsigned short,
			const unsigned short) const;
      virtual void
      drawVerticalBox(Cairo::RefPtr<Cairo::Context>&,
		      const unsigned short,
		      const unsigned short,
		      const unsigned short,
		      const unsigned short) const;
    }; // end of struct ElegantGraphTheme2

  } // end of namespace graphics

} // end of namespace tfel

#endif /* _LIB_TPLOT_ELEGANTGRAPHTHEME2_HXX */

