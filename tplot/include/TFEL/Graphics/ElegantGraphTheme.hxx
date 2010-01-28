/*!
 * \file   ElegantGraphTheme.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   13 jan 2008
 */

#ifndef _LIB_TPLOT_ELEGANGRAPHTTHEME_HXX_
#define _LIB_TPLOT_ELEGANGRAPHTTHEME_HXX_ 

#include <cairomm/context.h>

#include"TFEL/Graphics/StandardGraphTheme.hxx"

namespace tfel{

  namespace graphics
  {

    struct ElegantGraphTheme
      : public StandardGraphTheme
    {
      virtual void getGraphBorders(unsigned short&,unsigned short&,
				   unsigned short&,unsigned short&) const;
      virtual void drawGraphBackGround(Cairo::RefPtr<Cairo::Context>&,
				       const GraphLayout&,
				       const GraphSize&) const;
      virtual void drawBackGround(Cairo::RefPtr<Cairo::Context>&,
				  const GraphLayout&,
				  const GraphSize&) const;
      virtual void
      printXLabel(Cairo::RefPtr<Cairo::Context>&,
		  const GraphLayout&,
		  const GraphSize&,
		  const std::string&) const;
      virtual void
      printX2Label(Cairo::RefPtr<Cairo::Context>&,
		   const GraphLayout&,
		   const GraphSize&,
		   const std::string&) const;
      virtual void
      printYLabel(Cairo::RefPtr<Cairo::Context>&,
		  const GraphLayout&,
		  const GraphSize&,
		  const std::string&) const;
      virtual void
      printY2Label(Cairo::RefPtr<Cairo::Context>&,
		   const GraphLayout&,
		   const GraphSize&,
		   const std::string&) const;
      virtual void printLegend(Cairo::RefPtr<Cairo::Context>&,
			       const GraphLayout&,
			       const GraphSize&,
			       const std::string&,
			       const Curve::Style,
			       const unsigned short) const;
      virtual void printLegend(Cairo::RefPtr<Cairo::Context>&,
			       const GraphLayout&,
			       const GraphSize&,
			       const std::string&,
			       const Curve::Style,
			       const Curve::Color,
			       const unsigned short) const;
      virtual const Curve::Color
      getFirstBoxColor() const;
      virtual const Curve::Color
      getSecondBoxColor() const;
      virtual const Curve::Color
      getTextColor() const;
      virtual ~ElegantGraphTheme();
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
    }; // end of struct ElegantGraphTheme

  } // end of namespace graphics

} // end of namespace tfel

#endif /* _LIB_TPLOT_ELEGANTGRAPHTHEME_HXX */

