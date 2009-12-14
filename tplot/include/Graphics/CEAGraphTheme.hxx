/*!
 * \file   CEAGraphTheme.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   13 jan 2008
 */

#ifndef _LIB_TPLOT_CEAGRAPHTHEME_HXX_
#define _LIB_TPLOT_CEAGRAPHTHEME_HXX_ 

#include <cairomm/context.h>

#include"Graphics/ElegantGraphTheme.hxx"

namespace tfel{

  namespace graphics
  {

    struct CEAGraphTheme
      : public ElegantGraphTheme
    {
      virtual void getGraphBorders(unsigned short&,unsigned short&,
				   unsigned short&,unsigned short&) const;
      virtual void drawBackGround(Cairo::RefPtr<Cairo::Context>&,
				  const GraphLayout&,
				  const GraphSize&) const;
      virtual const Curve::Color
      getFirstBoxColor() const;
      virtual const Curve::Color
      getSecondBoxColor() const;
      virtual const Curve::Color
      getTextColor() const;
      virtual ~CEAGraphTheme();
    }; // end of struct CEAGraphTheme

  } // end of namespace graphics

} // end of namespace tfel

#endif /* _LIB_TPLOT_CEAGRAPHTHEME_HXX */

