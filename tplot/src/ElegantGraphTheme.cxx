/*!
 * \file   ElegantGraphTheme.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   13 jan 2008
 */

#include<iostream>
#include<cstdlib>

#include"Graphics/ThemeManager.hxx"
#include"Graphics/ElegantGraphTheme.hxx"

namespace tfel
{

  namespace graphics
  {
    
    void
    ElegantGraphTheme::getGraphBorders(unsigned short& gl,unsigned short& gr,
				  unsigned short& gu,unsigned short& gd) const
    {
      gl = 6;
      gr = 16;
      gu = 6;
      gd = 16;
    } // end of ElegantGraphTheme::getGraphBorders

    void
    ElegantGraphTheme::drawBackGround(Cairo::RefPtr<Cairo::Context>& cr,
			      const GraphLayout&,
			      const GraphSize& s) const
    {
      using namespace std;
      cr->save();
      cr->set_source_rgb(0.45,0.45,0.45);
      cr->rectangle(s.xh0+10,s.yh0+10,s.xh1-s.xh0,s.yh1-s.yh0);
      cr->fill();
      cr->restore();
    } // end of ElegantGraphTheme::drawBackGround

    void
    ElegantGraphTheme::drawGraphBackGround(Cairo::RefPtr<Cairo::Context>& cr,
				   const GraphLayout&,
				   const GraphSize& s) const
    {
      using namespace Cairo;
      cr->save();
      cr->set_source_rgb(1.,1.,1.);
      cr->rectangle(s.xh0,s.yh0,s.xh1-s.xh0,s.yh1-s.yh0);
      cr->fill();
      cr->restore();
    } // end of ElegantGraphTheme::drawBackGround

    const Curve::Color
    ElegantGraphTheme::getFirstBoxColor(void) const
    {
      Curve::Color c;
      c.r = 0.45;
      c.g = 0.45;
      c.b = 0.45;
      return c;
    } // end of ElegantGraphTheme::getFirstBoxColor

    const Curve::Color
    ElegantGraphTheme::getSecondBoxColor(void) const
    {
      Curve::Color c;
      c.r = 0.;
      c.g = 0.;
      c.b = 0.;
      return c;
    } // end of ElegantGraphTheme::getSecondBoxColor

    const Curve::Color
    ElegantGraphTheme::getTextColor(void) const
    {
      Curve::Color c;
      c.r = 1.;
      c.g = 1.;
      c.b = 1.;
      return c;
    } // end of ElegantGraphTheme::getSecondBoxColor

    void
    ElegantGraphTheme::drawHorizontalBox(Cairo::RefPtr<Cairo::Context>& cr,
				 const unsigned short x,
				 const unsigned short y,
				 const unsigned short w,
				 const unsigned short h) const
    {
      using namespace Cairo;
      RefPtr<LinearGradient> grad;
      Curve::Color c1 = this->getFirstBoxColor();
      Curve::Color c2 = this->getSecondBoxColor();
      cr->save();
      grad = LinearGradient::create(0.,y,0.,y+0.75*h);
      grad->add_color_stop_rgb(0,c1.r,c1.g,c1.b);
      grad->add_color_stop_rgb(1,c2.r,c2.g,c2.b);
      cr->set_source_rgb(c2.r,c2.g,c2.b);
      cr->move_to(x,y);
      cr->line_to(x+w,y);
      cr->stroke();
      cr->arc(x+w,y+0.5*h,0.5*h,1.5*M_PI,0.5*M_PI);
      cr->stroke();
      cr->move_to(x+w,y+h);
      cr->line_to(x,y+h);
      cr->stroke();
      cr->arc(x,y+0.5*h,0.5*h,0.5*M_PI,1.5*M_PI);
      cr->stroke();
      cr->set_source(grad);
      cr->rectangle(x,y,w,h);
      cr->fill();
      cr->arc(x,y+0.5*h,0.5*h,0.5*M_PI,1.5*M_PI);
      cr->fill();
      cr->arc(x+w,y+0.5*h,0.5*h,1.5*M_PI,0.5*M_PI);
      cr->fill();
      cr->restore();
    } // end of ElegantGraphTheme::drawHorizontalBox

    void
    ElegantGraphTheme::drawVerticalBox(Cairo::RefPtr<Cairo::Context>& cr,
				 const unsigned short x,
				 const unsigned short y,
				 const unsigned short w,
				 const unsigned short h) const
    {
      using namespace Cairo;
      RefPtr<LinearGradient> grad;
      Curve::Color c1 = this->getFirstBoxColor();
      Curve::Color c2 = this->getSecondBoxColor();
      cr->save();
      grad =  LinearGradient::create(x,0.,x+0.75*w,0.);
      grad->add_color_stop_rgb(0,c1.r,c1.g,c1.b);
      grad->add_color_stop_rgb(1,c2.r,c2.g,c2.b);
      cr->set_source_rgb(c2.r,c2.g,c2.b);
      cr->move_to(x,y);
      cr->line_to(x,y+h);
      cr->stroke();
      cr->arc(x+0.5*w,y+h,0.5*w,0,M_PI);
      cr->stroke();
      cr->move_to(x+w,y+h);
      cr->line_to(x+w,y);
      cr->stroke();
      cr->arc(x+0.5*w,y,0.5*w,M_PI,0.);
      cr->stroke();
      cr->set_source(grad);
      cr->rectangle(x,y,w,h);
      cr->fill();
      cr->arc(x+0.5*w,y,0.5*w,M_PI,0.);
      cr->fill();
      cr->arc(x+0.5*w,y+h,0.5*w,0,M_PI);
      cr->fill();
      cr->restore();
    } // end of ElegantGraphTheme::drawVerticalBox

    void
    ElegantGraphTheme::printXLabel(Cairo::RefPtr<Cairo::Context>& cr,
			   const GraphLayout& l,
			   const GraphSize& s,
			   const std::string& xlabel) const
    {
      using namespace Cairo;
      TextExtents extents;
      Curve::Color c = this->getTextColor();
      cr->save();
      cr->get_text_extents(xlabel,extents);
      this->drawHorizontalBox(cr,
			      static_cast<unsigned short>(s.xh0+0.5*(s.xh1-s.xh0)-0.5*extents.width),
			      static_cast<unsigned short>(s.height-l.md-l.ttd-l.ld),
			      static_cast<unsigned short>(extents.width),
			      static_cast<unsigned short>(l.ld));
      cr->set_source_rgb(c.r,c.g,c.b);
      cr->move_to(static_cast<unsigned short>(s.xh0+0.5*(s.xh1-s.xh0)-0.5*extents.width),
		  static_cast<unsigned short>(s.height-l.md-l.ttd-0.5*(l.ld)+0.5*extents.height));
      cr->show_text(xlabel);
      cr->restore();
    } // end of ElegantGraphGraphTheme::printXLabel

    void
    ElegantGraphTheme::printYLabel(Cairo::RefPtr<Cairo::Context>& cr,
				    const GraphLayout& l,
				    const GraphSize& s,
				    const std::string& ylabel) const
    {
      using namespace Cairo;
      TextExtents extents;
      Curve::Color c = this->getTextColor();
      cr->save();
      cr->get_text_extents(ylabel,extents);
      this->drawVerticalBox(cr,
			    static_cast<unsigned short>(l.ml+l.ttl),
			    static_cast<unsigned short>(s.yh0+0.5*(s.yh1-s.yh0)-0.5*extents.width),
			    static_cast<unsigned short>(l.ll),
			    static_cast<unsigned short>(extents.width));
      cr->move_to(static_cast<unsigned short>(l.ml+l.ttl+0.5*(l.ll+extents.height)),
		  static_cast<unsigned short>(s.yh0+0.5*(s.yh1-s.yh0)+0.5*extents.width));
      cr->rotate(-0.5*M_PI);
      cr->set_source_rgb(c.r,c.g,c.b);
      cr->show_text(ylabel);
      cr->restore();
    } // end of ElegantGraphTheme::printYLabel

    void
    ElegantGraphTheme::printX2Label(Cairo::RefPtr<Cairo::Context>& cr,
				     const GraphLayout& l,
				     const GraphSize& s,
				     const std::string& x2label) const
    {
      using namespace Cairo;
      TextExtents extents;
      Curve::Color c = this->getTextColor();
      cr->save();
      cr->get_text_extents(x2label,extents);
      this->drawHorizontalBox(cr,
			      static_cast<unsigned short>(s.xh0+0.5*(s.xh1-s.xh0)-0.5*extents.width),
			      static_cast<unsigned short>(l.mu+l.ttu),
			      static_cast<unsigned short>(extents.width),
			      static_cast<unsigned short>(l.lu));
      cr->set_source_rgb(c.r,c.g,c.b);
      cr->move_to(static_cast<unsigned short>(s.xh0+0.5*(s.xh1-s.xh0)-0.5*extents.width),
		  static_cast<unsigned short>(l.mu+l.ttu+0.5*l.lu+0.5*extents.height));
      cr->show_text(x2label);
      cr->restore();
    } // end of ElegantGraphTheme::printX2Label

    void
    ElegantGraphTheme::printY2Label(Cairo::RefPtr<Cairo::Context>& cr,
				     const GraphLayout& l,
				     const GraphSize& s,
				     const std::string& y2label) const
    {
      using namespace Cairo;
      TextExtents extents;
      Curve::Color c = this->getTextColor();
      cr->save();
      cr->get_text_extents(y2label,extents);
      this->drawVerticalBox(cr,
			    static_cast<unsigned short>(s.width-l.mr-l.ttr-l.lr),
			    static_cast<unsigned short>(s.yh0+0.5*(s.yh1-s.yh0)-0.5*extents.width),
			    static_cast<unsigned short>(l.lr),
			    static_cast<unsigned short>(extents.width));
      cr->move_to(static_cast<unsigned short>(s.width-l.mr-l.ttr-0.5*(l.lr-extents.height)),
		  static_cast<unsigned short>(s.yh0+0.5*(s.yh1-s.yh0)+0.5*extents.width));
      cr->set_source_rgb(c.r,c.g,c.b);
      cr->rotate(-0.5*M_PI);
      cr->show_text(y2label);
      cr->restore();
    } // end of ElegantGraphTheme::printY2Label

    void
    ElegantGraphTheme::printLegend(Cairo::RefPtr<Cairo::Context>& cr,
			      const GraphLayout& l,
			      const GraphSize& s,
			      const std::string& n,
			      const Curve::Style st,
			      const unsigned short nb) const
    {
      StandardGraphTheme::printLegend(cr,l,s,n,st,nb);
    } // end of ElegantGraphTheme::printLegend

    void
    ElegantGraphTheme::printLegend(Cairo::RefPtr<Cairo::Context>& cr,
				    const GraphLayout&,
				    const GraphSize& s,
				    const std::string& l,
				    const Curve::Style style,
				    const Curve::Color color,
				    const unsigned short nbr) const
    {
      using namespace std;
      using namespace Cairo;
      TextExtents extents;
      Curve::Color c = this->getTextColor();
      cr->save();
      cr->set_line_width(2);
      const unsigned short xh = static_cast<unsigned short>(s.xh1-15);
      unsigned short yh  = static_cast<unsigned short>(s.yh0+20*nbr);
      cr->get_text_extents(l,extents);
      this->drawHorizontalBox(cr,
			      static_cast<unsigned short>(xh-32-extents.width),
			      static_cast<unsigned short>(yh-2),
			      static_cast<unsigned short>(ceil(extents.width)+36),
			      static_cast<unsigned short>(ceil(extents.height)+4));
      cr->set_source_rgb(c.r,c.g,c.b);
      cr->move_to(xh-30-extents.width,static_cast<unsigned short>(yh+floor(extents.height)));
      cr->show_text(l);
      this->drawCurveLegend(cr,color,style,xh,static_cast<unsigned short>(yh+0.5*extents.height));
      yh = static_cast<unsigned short>(yh+floor(extents.height));
      cr->restore();      
    } // end of ElegantGraphTheme::printLegends

    ElegantGraphTheme::~ElegantGraphTheme()
    {} // end of ElegantGraphTheme::~ElegantGraphTheme

    StandardThemeProxy<ElegantGraphTheme> elegantGraphThemeProxy("ElegantGraphTheme");

  } // end of namespace graphics

} // end of namespace tfel


