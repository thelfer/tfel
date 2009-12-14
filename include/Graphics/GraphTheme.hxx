/*!
 * \file   GraphTheme.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   03 jan 2008
 */

#ifndef _LIB_TFEL_GRAPHTHEME_HXX_
#define _LIB_TFEL_GRAPHTHEME_HXX_ 

#include<map>
#include<vector>
#include<string>

#include<cairomm/context.h>

#include"Graphics/Grid.hxx"
#include"Graphics/Point.hxx"
#include"Graphics/ArrowStyle.hxx"
#include"Graphics/Curve.hxx"
#include"Graphics/GraphSize.hxx"
#include"Graphics/GraphLayout.hxx"

namespace tfel{

  namespace graphics
  {

    struct GraphTheme
    {
      virtual void getDefaultColor(Curve::Color&,const unsigned short) const = 0;
      virtual unsigned short getYTicsWidth(Cairo::RefPtr<Cairo::Context>&,
					   const std::map<double,std::string>&) const = 0;
      virtual unsigned short getY2TicsWidth(Cairo::RefPtr<Cairo::Context>&,
					    const std::map<double,std::string>&) const = 0;
      virtual unsigned short getXTicsHeight(Cairo::RefPtr<Cairo::Context>&,
					    const std::map<double,std::string>&) const = 0;
      virtual unsigned short getX2TicsHeight(Cairo::RefPtr<Cairo::Context>&,
					     const std::map<double,std::string>&) const = 0;
      virtual unsigned short getLeftMargin(void) const = 0;
      virtual unsigned short getRightMargin(void) const = 0;
      virtual unsigned short getUpperMargin(void) const = 0;
      virtual unsigned short getDownMargin(void) const = 0;
      virtual void printUpperTitle(Cairo::RefPtr<Cairo::Context>&,
				   const GraphLayout&,
				   const GraphSize&,
				   const std::string&) const = 0;
      virtual void printDownTitle(Cairo::RefPtr<Cairo::Context>&,
				  const GraphLayout&,
				  const GraphSize&,
				  const std::string&) const = 0;
      virtual void printRightTitle(Cairo::RefPtr<Cairo::Context>&,
				   const GraphLayout&,
				   const GraphSize&,
				   const std::string&) const = 0;
      virtual void printLeftTitle(Cairo::RefPtr<Cairo::Context>&,
				  const GraphLayout&,
				  const GraphSize&,
				  const std::string&) const = 0;
      virtual void printXLabel(Cairo::RefPtr<Cairo::Context>&,
			       const GraphLayout&,
			       const GraphSize&,
			       const std::string&) const = 0;
      virtual void printYLabel(Cairo::RefPtr<Cairo::Context>&,
			       const GraphLayout&,
			       const GraphSize&,
			       const std::string&) const = 0;
      virtual void printX2Label(Cairo::RefPtr<Cairo::Context>&,
			 	const GraphLayout&,
			 	const GraphSize&,
			 	const std::string&) const = 0;
      virtual void printY2Label(Cairo::RefPtr<Cairo::Context>&,
				const GraphLayout&,
				const GraphSize&,
				const std::string&) const = 0;
      virtual void printXTics(Cairo::RefPtr<Cairo::Context>&,
			      const GraphLayout&,
			      const GraphSize&,
			      const std::map<double,std::string>&) const = 0;
      virtual void printYTics(Cairo::RefPtr<Cairo::Context>&,
			      const GraphLayout&,
			      const GraphSize&,
			      const std::map<double,std::string>&) const = 0;
      virtual void printX2Tics(Cairo::RefPtr<Cairo::Context>&,
			       const GraphLayout&,
			       const GraphSize&,
			       const std::map<double,std::string>&) const = 0;
      virtual void printY2Tics(Cairo::RefPtr<Cairo::Context>&,
			       const GraphLayout&,
			       const GraphSize&,
			       const std::map<double,std::string>&) const = 0;
      virtual void getGraphBorders(unsigned short&,unsigned short&,
				   unsigned short&,unsigned short&) const = 0;
      virtual void drawGrid(Cairo::RefPtr<Cairo::Context>&,
			    const GraphLayout&,
			    const GraphSize&,
			    const bool,
			    const unsigned short,
			    const std::map<double,std::string>&,
			    const std::map<double,std::string>&,
			    const std::map<double,std::string>&,
			    const std::map<double,std::string>&,
			    const double,const double,
			    const double,const double,
			    const double,const double,
			    const double,const double,
			    const bool, const bool,
			    const bool,const bool) const = 0;
      virtual void drawXAxis(Cairo::RefPtr<Cairo::Context>&,
			     const GraphLayout&,
			     const GraphSize&) const = 0;
      virtual void drawYAxis(Cairo::RefPtr<Cairo::Context>&,
			     const GraphLayout&,
			     const GraphSize&) const = 0;
      virtual void drawGraphBorder(Cairo::RefPtr<Cairo::Context>&,
				   const GraphLayout&,
				   const GraphSize&) const = 0;
      virtual void drawForeGround(Cairo::RefPtr<Cairo::Context>&,
				  const GraphLayout&,
				  const GraphSize&) const = 0;
      virtual void drawBackGround(Cairo::RefPtr<Cairo::Context>&,
				  const GraphLayout&,
				  const GraphSize&) const = 0;
      virtual void drawGraphForeGround(Cairo::RefPtr<Cairo::Context>&,
				       const GraphLayout&,
				       const GraphSize&) const =  0;
      virtual void drawGraphBackGround(Cairo::RefPtr<Cairo::Context>&,
				       const GraphLayout&,
				       const GraphSize&) const =  0;
      virtual void printLabel(Cairo::RefPtr<Cairo::Context>&,
			      const double,
			      const double,
			      const std::string&) const = 0;
      virtual void drawArrow(Cairo::RefPtr<Cairo::Context>&,
			     const double,
			     const double,
			     const double,
			     const double,
			     const ArrowStyle) const = 0;
      virtual void drawCurve(Cairo::RefPtr<Cairo::Context>&,
			     const double,
			     const double,
			     const double,
			     const double,
			     const GraphSize&,
			     const std::vector<Point>&,
			     const Curve::Style,
			     const unsigned short) const = 0;
      virtual void drawCurve(Cairo::RefPtr<Cairo::Context>&,
			     const double,
			     const double,
			     const double,
			     const double,
			     const GraphSize&,
			     const std::vector<Point>&,
			     const Curve::Style,
			     const Curve::Color,
			     const unsigned short) const = 0;
      virtual void printLegend(Cairo::RefPtr<Cairo::Context>&,
			       const GraphLayout&,
			       const GraphSize&,
			       const std::string&,
			       const Curve::Style,
			       const unsigned short) const = 0;
      virtual void printLegend(Cairo::RefPtr<Cairo::Context>&,
			       const GraphLayout&,
			       const GraphSize&,
			       const std::string&,
			       const Curve::Style,
			       const Curve::Color,
			       const unsigned short) const = 0;
      virtual unsigned short getXLabelHeight(Cairo::RefPtr<Cairo::Context>&,
					     const std::string&) const = 0;
      virtual unsigned short getX2LabelHeight(Cairo::RefPtr<Cairo::Context>&,
					      const std::string&) const = 0;
      virtual unsigned short getYLabelWidth(Cairo::RefPtr<Cairo::Context>&,
					    const std::string&) const = 0;
      virtual unsigned short getY2LabelWidth(Cairo::RefPtr<Cairo::Context>&,
					     const std::string&) const = 0;

      virtual unsigned short getDownTitleHeight(Cairo::RefPtr<Cairo::Context>&,
						const std::string&) const = 0;
      virtual unsigned short getUpperTitleHeight(Cairo::RefPtr<Cairo::Context>&,
						 const std::string&) const = 0;
      virtual unsigned short getLeftTitleWidth(Cairo::RefPtr<Cairo::Context>&,
					       const std::string&) const = 0;
      virtual unsigned short getRightTitleWidth(Cairo::RefPtr<Cairo::Context>&,
						const std::string&) const = 0;

      virtual ~GraphTheme();
    }; // end of struct GraphTheme

  } // end of namespace graphics

} // end of namespace tfel

#endif /* _LIB_TFEL_GRAPHTHEME_HXX */

