/*!
 * \file   StandardGraphTheme.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   13 jan 2008
 */

#include<iostream>
#include<sstream>
#include<cmath>
#include<limits>

#include"Graphics/ThemeManager.hxx"
#include"Graphics/StandardGraphTheme.hxx"

namespace tfel
{

  namespace graphics
  {

    const double StandardGraphTheme::defaultRedColors[3]   = {1.0000,0.4392,1};
    const double StandardGraphTheme::defaultGreenColors[3] = {0.7019,0.7372,0};
    const double StandardGraphTheme::defaultBlueColors[3]  = {0.0627,0.1215,0};
    
    const unsigned short
    StandardGraphTheme::defaultColorsNumber = 3u;
    
    unsigned short
    StandardGraphTheme::getLeftMargin(void) const
    {
      return 15;
    } // end of StandardGraphTheme::getLeftMargin(void)

    unsigned short
    StandardGraphTheme::getRightMargin(void) const
    {
      return 15;
    } // end of StandardGraphTheme::getRightMargin

    unsigned short
    StandardGraphTheme::getUpperMargin(void) const
    {
      return 15;
    } // end of StandardGraphTheme::getUpperMargin

    unsigned short
    StandardGraphTheme::getDownMargin(void) const
    {
      return 15;
    } // end of StandardGraphTheme::getDownMargin
    
    unsigned short
    StandardGraphTheme::getYTicsWidth(Cairo::RefPtr<Cairo::Context>& cr,
				      const std::map<double,std::string>& tics) const
    {
      using namespace std;
      using namespace Cairo;
      TextExtents extents;
      std::map<double,string>::const_iterator p;
      unsigned short res;
      if(tics.empty()){
	return 0;
      }
      res = 0;
      for(p=tics.begin();p!=tics.end();++p){
	cr->get_text_extents(p->second,extents);
	if(res<extents.width){
	  res=static_cast<unsigned short>(ceil(extents.width));
	}
      }
      return static_cast<unsigned short>(res+8u);
    } // end of StandardGraphTheme::getYTicsWidth

    unsigned short
    StandardGraphTheme::getY2TicsWidth(Cairo::RefPtr<Cairo::Context>& cr,
				      const std::map<double,std::string>& tics) const
    {
      using namespace std;
      using namespace Cairo;
      TextExtents extents;
      std::map<double,string>::const_iterator p;
      unsigned short res;
      if(tics.empty()){
	return 0;
      }
      res = 0;
      for(p=tics.begin();p!=tics.end();++p){
	cr->get_text_extents(p->second,extents);
	if(res<extents.width){
	  res=static_cast<unsigned short>(ceil(extents.width));
	}
      }
      return static_cast<unsigned short>(res+8u);
    } // end of StandardGraphTheme::getY2TicsWidth
    
    unsigned short
    StandardGraphTheme::getXTicsHeight(Cairo::RefPtr<Cairo::Context>& cr,
				       const std::map<double,std::string>& tics) const
    {
      using namespace std;
      using namespace Cairo;
      TextExtents extents;
      map<double,std::string>::const_iterator p;
      unsigned short res;
      if(tics.empty()){
	return 0;
      }
      res = 0;
      for(p=tics.begin();p!=tics.end();++p){
	cr->get_text_extents(p->second,extents);
	if(res<extents.height){
	  res=static_cast<unsigned short>(ceil(extents.height));
	}
      }
      return static_cast<unsigned short>(res+8u);
    } // end of StandardGraphTheme::getXTicsHeight

    unsigned short
    StandardGraphTheme::getX2TicsHeight(Cairo::RefPtr<Cairo::Context>& cr,
					const std::map<double,std::string>& tics) const
    {
      using namespace std;
      using namespace Cairo;
      TextExtents extents;
      map<double,std::string>::const_iterator p;
      unsigned short res;
      if(tics.empty()){
	return 0;
      }
      res = 0;
      for(p=tics.begin();p!=tics.end();++p){
	cr->get_text_extents(p->second,extents);
	if(res<extents.height){
	  res=static_cast<unsigned short>(ceil(extents.height));
	}
      }
      return static_cast<unsigned short>(res+8u);
    } // end of StandardGraphTheme::getX2TicsHeight

    void
    StandardGraphTheme::getGraphBorders(unsigned short& gl,unsigned short& gr,
					unsigned short& gu,unsigned short& gd) const
    {
      gl = 6u;
      gr = 6u;
      gu = 6u;
      gd = 6u;
    } // end of StandardGraphTheme::getGraphBorders

    void
    StandardGraphTheme::printXLabel(Cairo::RefPtr<Cairo::Context>& cr,
				    const GraphLayout& l,
				    const GraphSize& s,
				    const std::string& xlabel) const
    {
      using namespace Cairo;
      TextExtents extents;
      cr->save();
      cr->get_text_extents(xlabel,extents);
      cr->move_to(static_cast<unsigned short>(s.xh0+0.5*(s.xh1-s.xh0)-0.5*extents.width),
		  static_cast<unsigned short>(s.height-l.md-l.ttd-0.5*(l.ld)+0.5*extents.height));
      cr->show_text(xlabel);
      cr->restore();
    } // end of StandardGraphTheme::printXLabel

    void
    StandardGraphTheme::printYLabel(Cairo::RefPtr<Cairo::Context>& cr,
				    const GraphLayout& l,
				    const GraphSize& s,
				    const std::string& ylabel) const
    {
      using namespace Cairo;
      TextExtents extents;
      cr->save();
      cr->get_text_extents(ylabel,extents);
      cr->move_to(static_cast<unsigned short>(l.ml+l.ttl+0.5*(l.ll+extents.height)),
		  static_cast<unsigned short>(s.yh0+0.5*(s.yh1-s.yh0)+0.5*extents.width));
      cr->rotate(-0.5*M_PI);
      cr->show_text(ylabel);
      cr->restore();
    } // end of StandardGraphTheme::printYLabel

    void
    StandardGraphTheme::printX2Label(Cairo::RefPtr<Cairo::Context>& cr,
				     const GraphLayout& l,
				     const GraphSize& s,
				     const std::string& x2label) const
    {
      using namespace Cairo;
      TextExtents extents;
      cr->save();
      cr->get_text_extents(x2label,extents);
      cr->move_to(static_cast<unsigned short>(s.xh0+0.5*(s.xh1-s.xh0)-0.5*extents.width),
		  static_cast<unsigned short>(l.mu+l.ttu+0.5*l.lu+0.5*extents.height));
      cr->show_text(x2label);
      cr->restore();
    } // end of StandardGraphTheme::printX2Label

    void
    StandardGraphTheme::printY2Label(Cairo::RefPtr<Cairo::Context>& cr,
				     const GraphLayout& l,
				     const GraphSize& s,
				     const std::string& y2label) const
    {
      using namespace Cairo;
      TextExtents extents;
      cr->save();
      cr->get_text_extents(y2label,extents);
      cr->move_to(static_cast<unsigned short>(s.width-l.mr-l.ttr-0.5*(l.lr-extents.height)),
		  static_cast<unsigned short>(s.yh0+0.5*(s.yh1-s.yh0)+0.5*extents.width));
      cr->rotate(-0.5*M_PI);
      cr->show_text(y2label);
      cr->restore();
    } // end of StandardGraphTheme::printY2Label

    void
    StandardGraphTheme::printXTics(Cairo::RefPtr<Cairo::Context>& cr,
				    const GraphLayout& l,
				    const GraphSize& s,
				    const std::map<double,std::string>& xtics) const
    {
      using namespace std;
      using namespace Cairo;
      TextExtents extents;
      std::map<double,string>::const_iterator p;
      cr->save();
      for(p=xtics.begin();p!=xtics.end();++p){
	cr->get_text_extents(p->second,extents);
	cr->move_to(static_cast<unsigned short>((p->first-l.bx)/l.ax)-0.5*extents.width,
		    static_cast<unsigned short>(s.height-l.md-l.ttd-l.ld-0.5*(l.td)+0.5*extents.height));
	cr->show_text(p->second);
      }
      cr->restore();
    } // end of StandardGraphTheme::printXTics

    void
    StandardGraphTheme::printYTics(Cairo::RefPtr<Cairo::Context>& cr,
				    const GraphLayout& l,
				    const GraphSize&,
				    const std::map<double,std::string>& ytics) const
    {
      using namespace std;
      using namespace Cairo;
      TextExtents extents;
      std::map<double,string>::const_iterator p;
      cr->save();
      for(p=ytics.begin();p!=ytics.end();++p){
	cr->get_text_extents(p->second,extents);
	cr->move_to(static_cast<unsigned short>(l.ml+l.ttl+l.ll+0.5*(l.tl)-0.5*extents.width),
		    static_cast<unsigned short>(l.ay*(p->first)+l.by+0.5*extents.height));
	cr->show_text(p->second);
      }
      cr->restore();
    } // end of StandardGraphTheme::printYTics

    void
    StandardGraphTheme::printX2Tics(Cairo::RefPtr<Cairo::Context>& cr,
				     const GraphLayout& l,
				     const GraphSize&,
				     const std::map<double,std::string>& x2tics) const
    {
      using namespace std;
      using namespace Cairo;
      TextExtents extents;
      std::map<double,string>::const_iterator p;
      cr->save();
      for(p=x2tics.begin();p!=x2tics.end();++p){
	cr->get_text_extents(p->second,extents);
	cr->move_to(static_cast<unsigned short>((p->first-l.bx2)/l.ax2)-0.5*extents.width,
		    static_cast<unsigned short>(l.mu+l.ttu+l.lu+0.5*(l.tu)+0.5*extents.height));
	cr->show_text(p->second);
      }
      cr->restore();
    } // end of StandardGraphTheme::printX2Tics

    void
    StandardGraphTheme::printY2Tics(Cairo::RefPtr<Cairo::Context>& cr,
				     const GraphLayout& l,
				     const GraphSize& s,
				     const std::map<double,std::string>& y2tics) const
    {
      using namespace std;
      using namespace Cairo;
      TextExtents extents;
      std::map<double,string>::const_iterator p;
      cr->save();
      for(p=y2tics.begin();p!=y2tics.end();++p){
	cr->get_text_extents(p->second,extents);
	cr->move_to(static_cast<unsigned short>(s.width-l.mr-l.ttr-l.lr-0.5*(l.tr+extents.width)),
		    static_cast<unsigned short>(l.ay2*(p->first)+l.by2+0.5*extents.height));
	cr->show_text(p->second);
      }
      cr->restore();
    } // end of StandardGraphTheme::printY2Tics

    void
    StandardGraphTheme::printUpperTitle(Cairo::RefPtr<Cairo::Context>& cr,
					const GraphLayout& l,
					const GraphSize& s,
					const std::string& t) const
    {
      using namespace std;
      using namespace Cairo;
      TextExtents extents;
      cr->save();
      cr->get_text_extents(t,extents);
      cr->move_to(static_cast<unsigned short>(s.xh0+0.5*(s.xh1-s.xh0-extents.width)),
		  static_cast<unsigned short>(l.mu+0.5*(l.ttu+extents.height)));
      cr->show_text(t);
      cr->restore();
    } // end of StandardGraphTheme::printUpperTitle

    void
    StandardGraphTheme::printDownTitle(Cairo::RefPtr<Cairo::Context>& cr,
				       const GraphLayout& l,
				       const GraphSize& s,
				       const std::string& t) const
    {
      using namespace Cairo;
      TextExtents extents;
      cr->save();
      cr->get_text_extents(t,extents);
      cr->move_to(static_cast<unsigned short>(s.xh0+0.5*(s.xh1-s.xh0-extents.width)),
		  static_cast<unsigned short>(s.height-l.md-0.5*(l.ttl-extents.height)));
      cr->show_text(t);
      cr->restore();
    } // end of StandardGraphTheme::printDownTitle

    void
    StandardGraphTheme::printLeftTitle(Cairo::RefPtr<Cairo::Context>& cr,
				       const GraphLayout& l,
				       const GraphSize& s,
				       const std::string& t) const
    {
      using namespace std;
      using namespace Cairo;
      TextExtents extents;
      cr->save();
      cr->get_text_extents(t,extents);
      cr->move_to(static_cast<unsigned short>(l.ml+l.ttl-2),
		  static_cast<unsigned short>(s.yh0+0.5*(s.yh1-s.yh0)+0.5*extents.width));
      cr->rotate(-0.5*M_PI);
      cr->show_text(t);
      cr->restore();
    } // end of StandardGraphTheme::printLeftTitle

    void
    StandardGraphTheme::printRightTitle(Cairo::RefPtr<Cairo::Context>& cr,
					const GraphLayout& l,
					const GraphSize& s,
					const std::string& t) const
    {
      using namespace Cairo;
      TextExtents extents;
      cr->save();
      cr->get_text_extents(t,extents);
      cr->move_to(static_cast<unsigned short>(s.width-l.mr-0.5*(l.ttr)+0.5*extents.height),
		  static_cast<unsigned short>(s.yh0+0.5*(s.yh1-s.yh0)+0.5*extents.width));
      cr->rotate(-0.5*M_PI);
      cr->show_text(t);
      cr->restore();
    } // end of StandardGraphTheme::printRightTitle

    void
    StandardGraphTheme::drawForeGround(Cairo::RefPtr<Cairo::Context>&,
				       const GraphLayout&,
				       const GraphSize&) const
    {} // end of StandardGraphTheme::drawForeGround

    void
    StandardGraphTheme::drawBackGround(Cairo::RefPtr<Cairo::Context>& cr,
				       const GraphLayout&,
				       const GraphSize&) const
    {
      cr->save();
      cr->set_source_rgb(1.,1.,1.);
      cr->paint();
      cr->restore();
    } // end of StandardGraphTheme::drawBackGround

    void
    StandardGraphTheme::drawGraphForeGround(Cairo::RefPtr<Cairo::Context>&,
					    const GraphLayout&,
					    const GraphSize&) const
    {} // end of StandardGraphTheme::drawGraphForeGround

    void
    StandardGraphTheme::drawGraphBackGround(Cairo::RefPtr<Cairo::Context>&,
					    const GraphLayout&,
					    const GraphSize&) const
    {} // end of StandardGraphTheme::drawGraphBackGround

    void
    StandardGraphTheme::drawXAxis(Cairo::RefPtr<Cairo::Context>& cr,
				  const GraphLayout& l,
				  const GraphSize& s) const
    {
      cr->save();
      cr->set_line_width(2.0);
      cr->move_to(s.xh0,l.by);
      cr->line_to(s.xh1,l.by);
      cr->stroke();
      cr->restore();
    } // end of StandardGraphTheme::drawXAxis

    void
    StandardGraphTheme::drawYAxis(Cairo::RefPtr<Cairo::Context>& cr,
				  const GraphLayout& l,
				  const GraphSize& s) const
    {
	cr->save();
	cr->set_line_width(2.0);
	cr->move_to(-l.bx/l.ax,s.yh0);
	cr->line_to(-l.bx/l.ax,s.yh1);
	cr->stroke();
	cr->restore();
    } // end of StandardGraphTheme::drawYAxis

    void
    StandardGraphTheme::drawGraphBorder(Cairo::RefPtr<Cairo::Context>& cr,
					const GraphLayout&,
					const GraphSize& s) const
    {
      cr->save();
      cr->set_line_width(2.0);
      cr->move_to(s.xh0,s.yh0);
      cr->line_to(s.xh0,s.yh1);
      cr->line_to(s.xh1,s.yh1);
      cr->line_to(s.xh1,s.yh0);
      cr->line_to(s.xh0,s.yh0);
      cr->close_path();   // line back to start point
      cr->stroke();
      cr->restore();
    } // end of StandardGraphTheme::drawGraphBorder

    void
    StandardGraphTheme::drawHorizontalGrid(Cairo::RefPtr<Cairo::Context>& cr,
					   const GraphLayout& l,
					   const GraphSize& s,
					   const std::map<double,std::string>& tics,
					   const double tmin,const double tmax) const
    {
      using namespace std;
      std::map<double,string>::const_iterator p;
      for(p=tics.begin();p!=tics.end();++p){
	if((p->first>=tmin)&&(p->first<=tmax)){
	  cr->move_to((p->first-l.bx)/l.ax,s.yh0);
	  cr->line_to((p->first-l.bx)/l.ax,s.yh1);
	}
	cr->stroke();
      }
    } // end of StandardGraphTheme::drawHorizontalGrid

    void
    StandardGraphTheme::drawVerticalGrid(Cairo::RefPtr<Cairo::Context>& cr,
					 const GraphLayout& l,
					 const GraphSize& s,
					 const std::map<double,std::string>& tics,
					 const double tmin,const double tmax) const
    {
      using namespace std;
      std::map<double,string>::const_iterator p;
      for(p=tics.begin();p!=tics.end();++p){
	if((p->first>=tmin)&&(p->first<=tmax)){
	  cr->move_to(s.xh0,l.ay*(p->first)+l.by);
	  cr->line_to(s.xh1,l.ay*(p->first)+l.by);
	}
	cr->stroke();
      }
    } // end of StandardGraphTheme::drawVerticalGrid

    void
    StandardGraphTheme::drawHorizontalGridMark(Cairo::RefPtr<Cairo::Context>& cr,
					       const GraphLayout& l,
					       const GraphSize& s,
					       const std::map<double,std::string>& tics,
					       const double tmin,const double tmax) const
    {
      using namespace std;
      std::map<double,string>::const_iterator p;
      for(p=tics.begin();p!=tics.end();++p){
	if((p->first>=tmin)&&(p->first<=tmax)){
	  cr->move_to((p->first-l.bx)/l.ax,s.yh0);
	  cr->line_to((p->first-l.bx)/l.ax,s.yh0+10.);
	  cr->stroke();
	  cr->move_to((p->first-l.bx)/l.ax,s.yh1);
	  cr->line_to((p->first-l.bx)/l.ax,s.yh1-10.);
	  cr->stroke();
	}
      }
    } // end of StandardGraphTheme::drawHorizontalGridMark

    void
    StandardGraphTheme::drawVerticalGridMark(Cairo::RefPtr<Cairo::Context>& cr,
					     const GraphLayout& l,
					     const GraphSize& s,
					     const std::map<double,std::string>& tics,
					     const double tmin,const double tmax) const
    {
      using namespace std;
      std::map<double,string>::const_iterator p;
      for(p=tics.begin();p!=tics.end();++p){
	if((p->first>=tmin)&&(p->first<=tmax)){
	  cr->move_to(s.xh0,l.ay*(p->first)+l.by);
	  cr->line_to(s.xh0+10,l.ay*(p->first)+l.by);
	  cr->stroke();
	  cr->move_to(s.xh1,l.ay*(p->first)+l.by);
	  cr->line_to(s.xh1-10,l.ay*(p->first)+l.by);
	  cr->stroke();
	}
      }
    } // end of StandardGraphTheme::drawVerticalGridMark

    void
    StandardGraphTheme::drawGrid(Cairo::RefPtr<Cairo::Context>& cr,
				 const GraphLayout& l,
				 const GraphSize& s,
				 const bool showGrid,
				 const unsigned short grid,
				 const std::map<double,std::string>& xtics,
				 const std::map<double,std::string>& ytics,
				 const std::map<double,std::string>& x2tics,
				 const std::map<double,std::string>& y2tics,
				 const double xmin,  const double xmax,
				 const double ymin,  const double ymax,
				 const double x2min, const double x2max,
				 const double y2min, const double y2max,
				 const bool x , const bool y,
				 const bool x2, const bool y2) const
    {
      using namespace std;
      if(showGrid){
	cr->save();
	valarray<double> dashes(2);
	dashes[0]=4.;
	dashes[1]=8.;
	cr->set_dash(dashes,0.);
	cr->set_line_width(1.0);
	cr->set_source_rgb(0.2941,0.333,0.3411);
	// cr->set_source_rgb(0.60,0.60,0.60);
	if((grid&Grid::X)&&(x)){
	  this->drawHorizontalGrid(cr,l,s,xtics,xmin,xmax);
	}
	if((grid&Grid::Y)&&(y)){
	  this->drawVerticalGrid(cr,l,s,ytics,ymin,ymax);
	}
	if((grid&Grid::X2)&&(x2)){
	  this->drawHorizontalGrid(cr,l,s,x2tics,x2min,x2max);
	}
	if((grid&Grid::Y2)&&(y2)){
	  this->drawVerticalGrid(cr,l,s,y2tics,y2min,y2max);
	}
	cr->restore();
      }
      cr->save();
      cr->set_line_width(1.0);
      cr->set_source_rgb(0.0,0.0,0.0);
      if(x){
	this->drawHorizontalGridMark(cr,l,s,xtics,xmin,xmax);
      }
      if(y){
	this->drawVerticalGridMark(cr,l,s,ytics,ymin,ymax);
      }
      if(x2){
	this->drawHorizontalGridMark(cr,l,s,x2tics,x2min,x2max);
      }
      if(y2){
	this->drawVerticalGridMark(cr,l,s,y2tics,y2min,y2max);
      }
      cr->restore();
    } // end of StandardGraphTheme::drawGrid

    void
    StandardGraphTheme::drawCurve(Cairo::RefPtr<Cairo::Context>& cr,
				  const double ax,
				  const double bx,
				  const double ay,
				  const double by,
				  const GraphSize& s,
				  const std::vector<Point>& points,
				  const Curve::Style style,
				  const unsigned short nbr) const
    {
      Curve::Color cdefault;
      this->getDefaultColor(cdefault,nbr);
      this->drawCurve(cr,ax,bx,ay,by,s,points,style,cdefault,nbr);
    }

    void
    StandardGraphTheme::drawCurveWithLines(Cairo::RefPtr<Cairo::Context>& cr,
					   const double ax,
					   const double bx,
					   const double ay,
					   const double by,
					   const std::vector<Point>& points) const
    {
      using namespace std;
      vector<Point>::const_iterator p;
      vector<Point>::const_iterator pe;
      bool found;
      bool previous;
      const double radius = 2u;
      cr->set_line_width(2);
      if(points.empty()){
	return;
      }
      p  = points.begin();
      pe = points.end();
      found=false;
      previous=false;
      while((p!=pe)&&(!found)){
	if(!(isnan(p->x)||isnan(p->y))){
	  cr->move_to((p->x-bx)/ax,
		      ay*(p->y)+by);
	  found=true;
	  previous=true;
	}
	++p;
      }
      while(p!=pe){
	if(!(isnan(p->x)||isnan(p->y))){
	  if(previous){
	    cr->line_to((p->x-bx)/ax,
			ay*(p->y)+by);
	    cr->stroke();
	    cr->move_to((p->x-bx)/ax,
			ay*(p->y)+by);
	  } else {
	    cr->move_to((p->x-bx)/ax,
			ay*(p->y)+by);
	  }
	  previous=true;
	} else {
	  if(previous){
	    cr->arc((p->x-bx)/ax,ay*(p->y)+by,radius,0,2*M_PI);
	    cr->fill_preserve();
	    cr->stroke();
	  }
	  previous=false;
	}
	++p;
      }
    } // end of StandardGraphTheme::drawCurveWithLines

    void
    StandardGraphTheme::drawCurveWithDots(Cairo::RefPtr<Cairo::Context>& cr,
					     const double ax,
					     const double bx,
					     const double ay,
					     const double by,
					     const std::vector<Point>& points) const
    {
      using namespace std;
      vector<Point>::const_iterator p;
      const double radius = 2u;
      if(points.empty()){
	return;
      }
      p=points.begin();
      while(p!=points.end()){
	cr->arc((p->x-bx)/ax,ay*(p->y)+by,radius,0,2*M_PI);
	cr->fill_preserve();
	cr->stroke();
	++p;
      }
    } // end of StandardGraphTheme::drawCurveWithDots

    void
    StandardGraphTheme::drawCurveWithSquares(Cairo::RefPtr<Cairo::Context>& cr,
					     const double ax,
					     const double bx,
					     const double ay,
					     const double by,
					     const std::vector<Point>& points) const
    {
      using namespace std;
      vector<Point>::const_iterator p;
      const double extent = 3u;
      if(points.empty()){
	return;
      }
      p=points.begin();
      while(p!=points.end()){
	const double x0 = (p->x-bx)/ax;
	const double x1 = ay*(p->y)+by;
	cr->rectangle(x0-extent,x1-extent,2*extent,2*extent);
	cr->fill_preserve();
	cr->stroke();
	++p;
      }
    } // end of StandardGraphTheme::drawCurveWithSquares

    void
    StandardGraphTheme::drawCurveWithTriangles(Cairo::RefPtr<Cairo::Context>& cr,
					     const double ax,
					     const double bx,
					     const double ay,
					     const double by,
					     const std::vector<Point>& points) const
    {
      using namespace std;
      vector<Point>::const_iterator p;
      const double extent = 3u;
      if(points.empty()){
	return;
      }
      p=points.begin();
      while(p!=points.end()){
	const double x0 = (p->x-bx)/ax;
	const double x1 = ay*(p->y)+by;
	cr->move_to(x0,x1-extent);
	cr->line_to(x0-extent,x1+extent);
	cr->line_to(x0+extent,x1+extent);
	cr->close_path();
	cr->fill_preserve();
	cr->stroke();
	++p;
      }
    } // end of StandardGraphTheme::drawCurveWithTriangles

    void
    StandardGraphTheme::drawCurveWithDiamonds(Cairo::RefPtr<Cairo::Context>& cr,
					     const double ax,
					     const double bx,
					     const double ay,
					     const double by,
					     const std::vector<Point>& points) const
    {
      using namespace std;
      vector<Point>::const_iterator p;
      const double extent = 3;
      if(points.empty()){
	return;
      }
      p=points.begin();
      while(p!=points.end()){
	const double x0 = (p->x-bx)/ax;
	const double x1 = ay*(p->y)+by;
	cr->move_to(x0,x1-extent);
	cr->line_to(x0-extent,x1);
	cr->line_to(x0,x1+extent);
	cr->line_to(x0+extent,x1);
	cr->close_path();
	cr->fill_preserve();
	cr->stroke();
	++p;
      }
    } // end of StandardGraphTheme::drawCurveWithDiamonds

    void
    StandardGraphTheme::drawCurveWithCrosses(Cairo::RefPtr<Cairo::Context>& cr,
					     const double ax,
					     const double bx,
					     const double ay,
					     const double by,
					     const std::vector<Point>& points) const
    {
      using namespace std;
      vector<Point>::const_iterator p;
      const double extent = 3;
      if(points.empty()){
	return;
      }
      p=points.begin();
      while(p!=points.end()){
	const double x0 = (p->x-bx)/ax;
	const double x1 = ay*(p->y)+by;
	cr->move_to(x0-extent,x1-extent);
	cr->line_to(x0+extent,x1+extent);
	cr->stroke();
	cr->move_to(x0-extent,x1+extent);
	cr->line_to(x0+extent,x1-extent);
	cr->stroke();
	++p;
      }
    } // end of StandardGraphTheme::drawCurveWithCrosses

    void
    StandardGraphTheme::drawCurveWithPoints(Cairo::RefPtr<Cairo::Context>& cr,
					     const double ax,
					     const double bx,
					     const double ay,
					     const double by,
					     const std::vector<Point>& points) const
    {
      using namespace std;
      vector<Point>::const_iterator p;
      const double extent = 3;
      if(points.empty()){
	return;
      }
      p=points.begin();
      while(p!=points.end()){
	const double x0 = (p->x-bx)/ax;
	const double x1 = ay*(p->y)+by;
	cr->move_to(x0-extent,x1);
	cr->line_to(x0+extent,x1);
	cr->stroke();
	cr->move_to(x0,x1+extent);
	cr->line_to(x0,x1-extent);
	cr->stroke();
	++p;
      }
    } // end of StandardGraphTheme::drawCurveWithPoints

    void
    StandardGraphTheme::drawCurve(Cairo::RefPtr<Cairo::Context>& cr,
				  const double ax,
				  const double bx,
				  const double ay,
				  const double by,
				  const GraphSize& s,
				  const std::vector<Point>& points,
				  const Curve::Style style,
				  const Curve::Color color,
				  const unsigned short) const
    {
      using namespace std;
      cr->save();
      cr->rectangle(s.xh0,s.yh0,
		    s.xh1-s.xh0,s.yh1-s.yh0);
      cr->clip();
      cr->set_source_rgb(color.r,color.g,color.b);
      if(style==Curve::LINE){
	this->drawCurveWithLines(cr,ax,bx,ay,by,points);
      } else if(style==Curve::LINEPOINT){
	this->drawCurveWithLines(cr,ax,bx,ay,by,points);
	this->drawCurveWithPoints(cr,ax,bx,ay,by,points);
      } else if(style==Curve::DOT){
	this->drawCurveWithDots(cr,ax,bx,ay,by,points);
      } else if(style==Curve::POINT){
	this->drawCurveWithPoints(cr,ax,bx,ay,by,points);
      } else if(style==Curve::DIAMOND){
	this->drawCurveWithDiamonds(cr,ax,bx,ay,by,points);
      } else if(style==Curve::SQUARE){
	this->drawCurveWithSquares(cr,ax,bx,ay,by,points);
      } else if(style==Curve::TRIANGLE){
	this->drawCurveWithTriangles(cr,ax,bx,ay,by,points);
      } else {
	this->drawCurveWithCrosses(cr,ax,bx,ay,by,points);
      }
      cr->restore();
    } // end of StandardGraphTheme::drawCurves

    void
    StandardGraphTheme::getDefaultColor(Curve::Color & color,
					const unsigned short nbr) const
    {
      const unsigned short index = static_cast<unsigned short>((nbr == 0) ? 0 :
							       (nbr-1)%(StandardGraphTheme::defaultColorsNumber));
      color.r = StandardGraphTheme::defaultRedColors[index];
      color.g = StandardGraphTheme::defaultGreenColors[index];
      color.b = StandardGraphTheme::defaultBlueColors[index];
    } // end of StandardGraphTheme::getDefaultColor

    void
    StandardGraphTheme::printLegend(Cairo::RefPtr<Cairo::Context>& cr,
				    const GraphLayout& l,
				    const GraphSize& s,
				    const std::string& legend,
				    const Curve::Style style,
				    const unsigned short nbr) const
    {
      Curve::Color cdefault;
      this->getDefaultColor(cdefault,nbr);
      this->printLegend(cr,l,s,legend,style,cdefault,nbr);
    } // end of StandardGraphTheme::printLegend

    void
    StandardGraphTheme::printLegend(Cairo::RefPtr<Cairo::Context>& cr,
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
      cr->save();
      cr->set_line_width(2);
      const unsigned short xh = static_cast<unsigned short>(s.xh1-15u);
      unsigned short yh  = static_cast<unsigned short>(s.yh0+20*nbr);
      cr->get_text_extents(l,extents);
      cr->move_to(xh-30-extents.width,static_cast<unsigned short>(yh+floor(extents.height)));
      cr->show_text(l);
      this->drawCurveLegend(cr,color,style,xh,static_cast<unsigned short>(yh+0.5*extents.height));
      yh = static_cast<unsigned short>(yh+floor(extents.height));
      cr->restore();      
    } // end of StandardGraphTheme::printLegends

    unsigned short
    StandardGraphTheme::getXLabelHeight(Cairo::RefPtr<Cairo::Context>& cr,
					const std::string& l) const
    {
      using namespace Cairo;
      TextExtents extents;
      if(l.size()==0){
	return 0;
      }
      cr->get_text_extents(l,extents);
      return static_cast<unsigned short>(ceil(extents.height+4));
    } // end of StandardGraphTheme::getXLabelHeight;

    unsigned short
    StandardGraphTheme::getX2LabelHeight(Cairo::RefPtr<Cairo::Context>& cr,
					 const std::string& l) const
    {
      using namespace Cairo;
      TextExtents extents;
      if(l.size()==0){
	return 0;
      }
      cr->get_text_extents(l,extents);
      return static_cast<unsigned short>(ceil(extents.height+4));
    } // end of StandardGraphTheme::getX2LabelHeight;

    unsigned short
    StandardGraphTheme::getYLabelWidth(Cairo::RefPtr<Cairo::Context>& cr,
				       const std::string& l) const
    {
      using namespace Cairo;
      TextExtents extents;
      if(l.size()==0){
	return 0;
      }
      cr->get_text_extents(l,extents);
      return static_cast<unsigned short>(ceil(extents.height+4));
    } // end of StandardGraphTheme::getYLabelWidth;

    unsigned short
    StandardGraphTheme::getY2LabelWidth(Cairo::RefPtr<Cairo::Context>& cr,
					const std::string& l) const
    {
      using namespace Cairo;
      TextExtents extents;
      if(l.size()==0){
	return 0;
      }
      cr->get_text_extents(l,extents);
      return static_cast<unsigned short>(ceil(extents.height+4));
    } // end of StandardGraphTheme::getY2LabelWidth;

    StandardGraphTheme::~StandardGraphTheme()
    {} // end of StandardGraphTheme::~StandardGraphTheme

    unsigned short
    StandardGraphTheme::getDownTitleHeight(Cairo::RefPtr<Cairo::Context>& cr,
					   const std::string& l) const
    {
      using namespace Cairo;
      TextExtents extents;
      if(l.size()==0){
	return 0;
      }
      cr->get_text_extents(l,extents);
      return static_cast<unsigned short>(ceil(extents.height+4));
    } // end of StandardGraphTheme::getDownTitleHeight;

    unsigned short
    StandardGraphTheme::getUpperTitleHeight(Cairo::RefPtr<Cairo::Context>& cr,
					    const std::string& l) const
    {
      using namespace Cairo;
      TextExtents extents;
      if(l.size()==0){
	return 0;
      }
      cr->get_text_extents(l,extents);
      return static_cast<unsigned short>(ceil(extents.height)+4);
    } // end of StandardGraphTheme::getUpperTitleHeight;

    unsigned short
    StandardGraphTheme::getLeftTitleWidth(Cairo::RefPtr<Cairo::Context>& cr,
					  const std::string& l) const
    {
      using namespace Cairo;
      TextExtents extents;
      if(l.size()==0){
	return 0;
      }
      cr->get_text_extents(l,extents);
      return static_cast<unsigned short>(ceil(extents.height)+4);
    } // end of StandardGraphTheme::getLeftTitleWidth;

    unsigned short
    StandardGraphTheme::getRightTitleWidth(Cairo::RefPtr<Cairo::Context>& cr,
					   const std::string& l) const
    {
      using namespace Cairo;
      TextExtents extents;
      if(l.size()==0){
	return 0;
      }
      cr->get_text_extents(l,extents);
      return static_cast<unsigned short>(ceil(extents.height+4));
    } // end of StandardGraphTheme::getRightTitleWidth;

    void
    StandardGraphTheme::drawCurveLegend(Cairo::RefPtr<Cairo::Context>& cr,
				  const Curve::Color color,
				  const Curve::Style style,
				  const unsigned short xh,
				  const unsigned short yh) const
    {
      const double extent = 3;
      const double radius = 2u;
      cr->save();
      cr->set_source_rgb(color.r,color.g,color.b);
      if((style==Curve::LINE)||(style==Curve::LINEPOINT)){
	cr->move_to(xh,yh);
	cr->line_to(xh-20,yh);
	cr->close_path();
	cr->stroke();
      } else if(style==Curve::DOT){
	cr->arc(xh-10,yh,radius,0,2*M_PI);
	cr->fill();
      } else if((style==Curve::POINT)||(style==Curve::LINEPOINT)){
	const double x0 = xh-10;
	const double x1 = yh;
	cr->move_to(x0-extent,x1);
	cr->line_to(x0+extent,x1);
	cr->stroke();
	cr->move_to(x0,x1+extent);
	cr->line_to(x0,x1-extent);
	cr->close_path();
	cr->stroke();
      } else if(style==Curve::DIAMOND){
	const double x0 = xh-10;
	const double x1 = yh;
	cr->move_to(x0,x1-extent);
	cr->line_to(x0-extent,x1);
	cr->line_to(x0,x1+extent);
	cr->line_to(x0+extent,x1);
	cr->close_path();
	cr->fill_preserve();
	cr->stroke();
      } else if(style==Curve::SQUARE){
	const double x0 = xh-10;
	const double x1 = yh;
	cr->rectangle(x0-extent,x1-extent,2*extent,2*extent);
	cr->close_path();
	cr->fill_preserve();
	cr->stroke();
      } else if(style==Curve::TRIANGLE){
	const double x0 = xh-10;
	const double x1 = yh;
	cr->move_to(x0,x1-extent);
	cr->line_to(x0-extent,x1+extent);
	cr->line_to(x0+extent,x1+extent);
	cr->close_path();
	cr->fill_preserve();
	cr->stroke();
      } else {
	const double x0 = xh-10;
	const double x1 = yh;
	cr->move_to(x0-extent,x1-extent);
	cr->line_to(x0+extent,x1+extent);
	cr->stroke();
	cr->move_to(x0-extent,x1+extent);
	cr->line_to(x0+extent,x1-extent);
	cr->close_path();
	cr->stroke();
      }
      cr->restore();
    } // end of StandardGraphTheme::drawCurveLegend

    void
    StandardGraphTheme::printLabel(Cairo::RefPtr<Cairo::Context>& cr,
				   const double x0,
				   const double y0,
				   const std::string& label) const
    {
      cr->save();
      cr->move_to(x0,y0);
      cr->show_text(label);
      cr->restore();      
    } // end of StandardGraphTheme::printLabel

    void
    StandardGraphTheme::drawArrow(Cairo::RefPtr<Cairo::Context>& cr,
				  const double x0,
				  const double y0,
				  const double x1,
				  const double y1,
				  const ArrowStyle s) const
    {
      using namespace std;
      cr->save();
      cr->set_source_rgb(1.,0.,0.);
      cr->move_to(x0,y0);
      cr->line_to(x1,y1);
      cr->stroke();
      if(s.style!=ArrowStyle::NOHEAD){
	// the head
	const double r = sqrt((x1-x0)*(x1-x0)+(y1-y0)*(y1-y0));
	if(r>10*numeric_limits<double>::min()){
	  const double nx = (x1-x0)/r;
	  const double ny = (y1-y0)/r;
	  const double vx = -ny;
	  const double vy = nx;
	  if((s.style==ArrowStyle::HEAD)||
	     (s.style==ArrowStyle::HEADS)){
	    const double x2 = x1-6*(2*nx+vx);
	    const double y2 = y1-6*(2*ny+vy);
	    const double x3 = x1-6*(2*nx-vx);
	    const double y3 = y1-6*(2*ny-vy);
	    cr->move_to(x1,y1);
	    cr->line_to(x2,y2);
	    cr->stroke();
	    cr->move_to(x1,y1);
	    cr->line_to(x3,y3);
	    cr->stroke();
	  }
	  if((s.style==ArrowStyle::BACKHEAD)||
	     (s.style==ArrowStyle::HEADS)){
	    const double x2 = x1+6*(2*nx+vx);
	    const double y2 = y1+6*(2*ny+vy);
	    const double x3 = x1+6*(2*nx-vx);
	    const double y3 = y1+6*(2*ny-vy);
	    cr->move_to(x1,y1);
	    cr->line_to(x2,y2);
	    cr->stroke();
	    cr->move_to(x1,y1);
	    cr->line_to(x3,y3);
	    cr->stroke();
	  }
	}
      }
      cr->restore();      
    } // end of StandardGraphTheme::drawArrow

  } // end of namespace graphics

} // end of namespace tfel


