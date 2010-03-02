/*!
 * \file   Graph.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   04 jan 2008
 */

#include<iostream>
#include<algorithm>
#include<iterator>
#include<sstream>
#include<fstream>
#include<limits>
#include<cmath>

#include"TFEL/Graphics/Grid.hxx"
#include"TFEL/Graphics/Graph.hxx"
#include"TFEL/Graphics/GraphSize.hxx"
#include"TFEL/Graphics/ThemeManager.hxx"

namespace tfel
{

  namespace graphics
  {

    const double Graph::defaultLinearScaleMinValue = -1.;
    const double Graph::defaultLinearScaleMaxValue = 1.;
    const double Graph::defaultLogScaleMinValue = 1.;
    const double Graph::defaultLogScaleMaxValue = 2.;

    const double Graph::defaultFontSize = 24;

    const unsigned short Graph::defaultWidth  = 1024;
    const unsigned short Graph::defaultHeight = 768;

    double
    Graph::log10(const double d){
      using namespace std;
      if(d<numeric_limits<double>::min()){
	ostringstream msg;
	msg << "Graph::log10 : "
	    << "negative or zero value detected (" << d<< ")";
	throw(runtime_error(msg.str()));
      }
      return std::log10(d);
    } // end of Graph::log10

    Graph::Axis::Axis(const unsigned short id_)
      : id(id_)
    {
      this->reset();
    } // end of Graph::Axis::Axis()
    
    void
    Graph::Axis::reset(void){
      this->min    = Graph::defaultLinearScaleMinValue;
      this->max    = Graph::defaultLinearScaleMaxValue;
      this->label.clear();
      this->scale = Graph::Axis::LINEARSCALE;
      this->userDefinedRange = false;
      this->userDefinedTics  = false;
    } // end of Graph::Axis::reset

    struct Graph::Label
    {
      Label(const std::string& l,
	    const GraphCoordinates x_,
	    const GraphCoordinates y_)
	: label(l),
	  x(x_),
	  y(y_)
      {} // end of Label()
      Label(const std::string& n,
	    const std::string& l,
	    const GraphCoordinates x_,
	    const GraphCoordinates y_)
	: name(n),
	  label(l),
	  x(x_),
	  y(y_)
      {} // end of Label()
      std::string name;
      std::string label;
      GraphCoordinates x;
      GraphCoordinates y;
    }; // end of struct Graph::Label

    struct Graph::Arrow
    {
      Arrow(const GraphCoordinates x0_,
	    const GraphCoordinates y0_,
	    const GraphCoordinates x1_,
	    const GraphCoordinates y1_,
	    const ArrowStyle s)
	: x0(x0_),
	  y0(y0_),
	  x1(x1_),
	  y1(y1_),
	  style(s)  
      {} // end of Arrow()
      Arrow(const std::string& n,
	    const GraphCoordinates x0_,
	    const GraphCoordinates y0_,
	    const GraphCoordinates x1_,
	    const GraphCoordinates y1_,
	    const ArrowStyle s)
	: name(n),
	  x0(x0_),
	  y0(y0_),
	  x1(x1_),
	  y1(y1_),
	  style(s)
      {} // end of Arrow()
      std::string name;
      GraphCoordinates x0;
      GraphCoordinates y0;
      GraphCoordinates x1;
      GraphCoordinates y1;
      ArrowStyle style;
    }; // end of struct Graph::Arrow

    Graph::Graph()
      : xAxis(Graph::xaxis),
	yAxis(Graph::yaxis),
	x2Axis(Graph::x2axis),
	y2Axis(Graph::y2axis),
	fontSize(Graph::defaultFontSize),
	drawZoomRectangle(false),
	debugMode(false)
    {
      using namespace tfel::utilities;
      this->reset();
      this->setTheme(ThemeManager::getThemeManager().getTheme("StandardGraphTheme"));
      this->add_events(Gdk::BUTTON_PRESS_MASK);
      this->add_events(Gdk::BUTTON_RELEASE_MASK);
      this->add_events(Gdk::POINTER_MOTION_MASK);
    } // end of Graph::Graph()

    void
    Graph::reset(void){
      this->width  = Graph::defaultWidth;
      this->height = Graph::defaultHeight;
      this->samples = 100;
      this->weight   = Cairo::FONT_WEIGHT_NORMAL;
      this->slant    = Cairo::FONT_SLANT_NORMAL;
      this->fontSize = Graph::defaultFontSize;
      this->grid     = Grid::X|Grid::Y;
      this->userDefinedFont = false;
      this->showGraphGrid = false;
      this->showXAxis = false;
      this->showYAxis = false;
      this->showBorder = true;
      this->hasXAxisCurve  = false;
      this->hasX2AxisCurve = false;
      this->hasYAxisCurve  = false;
      this->hasY2AxisCurve = false;
      this->curves.clear();
      this->legends.clear();
      this->labels.clear();
      this->arrows.clear();
      this->xAxis.reset();
      this->yAxis.reset();
      this->x2Axis.reset();
      this->y2Axis.reset();
      this->computeTics(this->xAxis);
      this->computeTics(this->yAxis);
      this->computeTics(this->x2Axis);
      this->computeTics(this->y2Axis);
    } // end of Graph::reset

    void Graph::setWidth(const unsigned short w)
    {
      using namespace std;
      if(w==0){
	string msg("Graph::setWidth : ");
	msg += "null width specified";
	throw(runtime_error(msg));
      }
      this->width = w;
    } // end of Graph::setWidth

    void Graph::setHeight(const unsigned short h)
    {
      using namespace std;
      if(h==0){
	string msg("Graph::setWidth : ");
	msg += "null width specified";
	throw(runtime_error(msg));
      }
      this->height = h;
    } // end of Graph::setHeight

    void Graph::unsetWidth(void)
    {
      this->width = Graph::defaultWidth;
    } // end of Graph::unsetWidth

    void Graph::unsetHeight(void)
    {
      this->height = Graph::defaultHeight;
    } // end of Graph::unsetHeight

    void
    Graph::setLogScale(Graph::Axis& axis)
    {
      using namespace std;
      vector<CurveHandler>::iterator p;
      bool abscissa = (axis.id&Graph::xaxis)||(axis.id&Graph::x2axis);
      bool found = false;
      if(axis.scale==Graph::Axis::LOGSCALE){
	return;
      }
      for(p=this->curves.begin();p!=this->curves.end();++p){
	if(p->axis&axis.id){
	  vector<Point>::iterator p2;
	  found = true;
	  for(p2=p->points.begin();p2!=p->points.end();++p2){
	    double value;
	    if(abscissa){
	      value = p2->x;
	    } else {
	      value = p2->y;
	    }
	    if(value<numeric_limits<double>::min()){
	      const string& legend = p->curve->getLegend();
	      ostringstream msg;
	      msg << "Graph::setLogScale : "
		  << "negative value (" << value << ") detected";
	      if(!legend.empty()){
		msg << " for curve '"+legend+"'";
	      }
	      throw(runtime_error(msg.str()));
	    }
	  }
	}
      }
      for(p=this->curves.begin();p!=this->curves.end();++p){
	if(p->axis&axis.id){
	  vector<Point>::iterator p2;
	  for(p2=p->points.begin();p2!=p->points.end();++p2){
	    if(abscissa){
	      p2->x = Graph::log10(p2->x);
	    } else {
	      p2->y = Graph::log10(p2->y);
	    }
	  }
	}
      }
      if(!axis.userDefinedRange){
	if(found){
	  this->computeMinMax(axis);
	} else {
	  axis.min=Graph::defaultLogScaleMinValue;
	  axis.max=Graph::defaultLogScaleMaxValue;;
	}
      } else {
	axis.min=Graph::log10(axis.min);
	axis.max=Graph::log10(axis.max);
      }
      axis.scale=Graph::Axis::LOGSCALE;
      this->setRange(axis,axis.min,axis.max,
		     axis.userDefinedRange);
    } // end of Graph::setLogScale

    void
    Graph::unsetLogScale(Graph::Axis& axis)
    {
      using namespace std;
      vector<CurveHandler>::iterator p;
      bool abscissa = (axis.id&Graph::xaxis)||(axis.id&Graph::x2axis);
      bool found = false;
      if(axis.scale==Graph::Axis::LINEARSCALE){
	return;
      }
      for(p=this->curves.begin();p!=this->curves.end();++p){
	if(p->axis&axis.id){
	  vector<Point>::iterator p2;
	  found = true;
	  for(p2=p->points.begin();p2!=p->points.end();++p2){
	    if(abscissa){
	      p2->x = pow(10.,p2->x);
	    } else {
	      p2->y = pow(10.,p2->y);
	    }
	  }
	}
      }
      if(!axis.userDefinedRange){
	if(found){
	  this->computeMinMax(axis);
	} else {
	  axis.min=Graph::defaultLinearScaleMinValue;
	  axis.max=Graph::defaultLinearScaleMaxValue;
	}
      } else {
	axis.min=pow(10.,axis.min);
	axis.max=pow(10.,axis.max);
      }
      axis.scale=Graph::Axis::LINEARSCALE;
      this->setRange(axis,axis.min,axis.max,
		     axis.userDefinedRange);
    } // end of Graph::unsetLogScale

    void
    Graph::setXAxisLogScale(void)
    {
      this->setLogScale(this->xAxis);
    } // end of Graph::setXAxisLogScale

    void
    Graph::unsetXAxisLogScale(void)
    {
      this->unsetLogScale(this->xAxis);
    } // end of Graph::unsetXAxisLogScale

    void
    Graph::setX2AxisLogScale(void)
    {
      this->setLogScale(this->x2Axis);
    } // end of Graph::setX2AxisLogScale

    void
    Graph::unsetX2AxisLogScale(void)
    {
      this->unsetLogScale(this->x2Axis);
    } // end of Graph::unsetX2AxisLogScale

    void
    Graph::setYAxisLogScale(void)
    {
      this->setLogScale(this->yAxis);
    } // end of Graph::setYAxisLogScale

    void
    Graph::unsetYAxisLogScale(void)
    {
      this->unsetLogScale(this->yAxis);
    } // end of Graph::unsetYAxisLogScale

    void
    Graph::setY2AxisLogScale(void)
    {
      this->setLogScale(this->y2Axis);
    } // end of Graph::setY2AxisLogScale

    void
    Graph::unsetY2AxisLogScale(void)
    {
      this->unsetLogScale(this->y2Axis);
    } // end of Graph::unsetY2AxisLogScale

    double
    Graph::convertHorizontalGraphCoordinatesToAbsolutePosition(const GraphLayout& l,
							       const GraphCoordinates c) const
    {
      using namespace std;
      double cx = c.c->getValue();
      double x  = 0.;
      switch (c.coordinateType){
      case GraphCoordinates::FIRST: 
	if(this->xAxis.scale==Graph::Axis::LOGSCALE){
	  cx = Graph::log10(cx);
	}
	x = (cx-l.bx)/l.ax;
	break;
      default:
	string msg("Graph::convertHorizontalGraphCoordinatesToAbsolutePosition : ");
	msg += "unsupported coordinate type";
	throw(runtime_error(msg));
	break;
      }
      return x;
    } // end of Graph::convertHorizontalGraphCoordinatesToAbsolutePosition

    double
    Graph::convertVerticalGraphCoordinatesToAbsolutePosition(const GraphLayout& l,
							     const GraphCoordinates c) const
    {
      using namespace std;
      double cy = c.c->getValue();
      double y = 0.;
      switch (c.coordinateType){
      case GraphCoordinates::FIRST: 
	if(this->yAxis.scale==Graph::Axis::LOGSCALE){
	  cy = Graph::log10(cy);
	}
	y = l.ay*cy+l.by;
	break;
      default:
	string msg("Graph::convertVerticalGraphCoordinatesToAbsolutePosition : ");
	msg += "unsupported coordinate type";
	throw(runtime_error(msg));
	break;
      }
      return y;
    } // end of Graph::convertVerticalGraphCoordinatesToAbsolutePosition

    void
    Graph::setTheme(const tfel::utilities::shared_ptr<GraphTheme>& t)
    {
      this->theme = t;
    } // end of Graph::setTheme

    Graph::~Graph()
    {
      this->clearCurves();
    } // end of Graph::~Graph()

    bool
    Graph::on_expose_event(GdkEventExpose *const e)
    {
      using namespace std;
      using namespace Glib;
      using namespace Cairo;
      using namespace Gdk;
      using namespace Gtk;
      double vFactor;
      double hFactor;
      // This is where we draw on the window
      Glib::RefPtr<Gdk::Window> window = this->get_window();
      if(window){ 
	Allocation allocation = get_allocation();
	const unsigned int awidth  = static_cast<unsigned int>(allocation.get_width());
	const unsigned int aheight = static_cast<unsigned int>(allocation.get_height());
	vFactor = static_cast<double>(awidth)/static_cast<double>(this->width);
	hFactor = static_cast<double>(aheight)/static_cast<double>(this->height);
	Cairo::RefPtr<Context> cr = window->create_cairo_context();
	// clip to the area indicated by the expose event so that we only redraw
	// the portion of the window that needs to be redrawn
	cr->rectangle(e->area.x, e->area.y,
		      e->area.width, e->area.height);
	cr->clip();
	cr->save();
	cr->scale(vFactor,hFactor);
	this->plot(cr,this->width,this->height);
	cr->restore();
      }
      return true;
    } // end of Graph::on_expose_event

    bool
    Graph::on_button_press_event(GdkEventButton* e)
    {
      using namespace std;
      if(e->button==1){
	this->drawZoomRectangle=true;
	this->zx1 = e->x;
	this->zy1 = e->y;
	this->zx2 = e->x;
	this->zy2 = e->y;
	this->zw  = 0;
	this->zh  = 0;
      }
      return true;
    } // end of Graph::on_button_press_event

    bool
    Graph::on_button_release_event(GdkEventButton* e)
    {
      using namespace std;
      using namespace Glib;
      using namespace Cairo;
      using namespace Gdk;
      using namespace Gtk;
      if(e->button==1){
	this->drawZoomRectangle=false;
	if((this->zw>4)&&(this->zh>4)){
	  Glib::RefPtr<Gdk::Window> window = this->get_window();
	  Allocation allocation = get_allocation();
	  const unsigned short awidth  = static_cast<unsigned short>(allocation.get_width());
	  const unsigned short aheight = static_cast<unsigned short>(allocation.get_height());
	  Cairo::RefPtr<Context> cr = window->create_cairo_context();
	  GraphLayout l;
	  this->computeGraphLayout(cr,awidth,aheight,l);
	  if(this->zx1<l.bl){
	    this->zx1=l.bl;
	  }
	  if(this->zx1>awidth-l.br){
	    this->zx1=awidth-l.br;
	  }
	  if(this->zx2<l.bl){
	    this->zx2=l.bl;
	  }
	  if(this->zx2>awidth-l.br){
	    this->zx2=awidth-l.br;
	  }
	  if(this->zy1<l.bu){
	    this->zy1=l.bu;
	  }
	  if(this->zy1>aheight-l.bd){
	    this->zy1=aheight-l.bd;
	  }
	  if(this->zy2<l.bu){
	    this->zy2=l.bu;
	  }
	  if(this->zy2>aheight-l.bd){
	    this->zy2=aheight-l.bd;
	  }
	  gdouble xu;
	  gdouble yu;
	  gdouble w = abs(this->zx2-this->zx1);
	  gdouble h = abs(this->zy2-this->zy1);
	  if((w>4)&&(h>4)){
	    if(this->zx1<this->zx2){
	      xu = this->zx1;
	    } else {
	      xu = this->zx2;
	    }
	    if(this->zy1<this->zy2){
	      yu = this->zy1;
	    } else {
	      yu = this->zy2;
	    }
	    if(this->hasXAxisCurve){
	      this->xAxis.userDefinedRange = true;
	    }
	    if(this->hasX2AxisCurve){
	      this->x2Axis.userDefinedRange = true;
	    }
	    if(this->hasYAxisCurve){
	      this->yAxis.userDefinedRange = true;
	    }
	    if(this->hasY2AxisCurve){
	      this->y2Axis.userDefinedRange = true;
	    }
	    if(this->hasXAxisCurve){
	      gdouble nx0 = l.ax*xu+l.bx;    ;
	      gdouble nx1 = l.ax*(xu+w)+l.bx;;
	      this->xAxis.userDefinedRange = true;
	      this->updateRange(this->xAxis,nx0,nx1);
	    }
	    if(this->hasX2AxisCurve){
	      gdouble nx0 = l.ax2*xu+l.bx2;
	      gdouble nx1 = l.ax2*(xu+w)+l.bx2;
	      this->x2Axis.userDefinedRange = true;
	      this->updateRange(this->x2Axis,nx0,nx1);
	    }
	    if(this->hasYAxisCurve){
	      gdouble ny0 = (yu-l.by)/l.ay;
	      gdouble ny1 = (yu+h-l.by)/l.ay;
	      this->yAxis.userDefinedRange = true;
	      this->setRange(this->yAxis.min,
			     this->yAxis.max,
			     ny1,ny0,
			     this->yAxis.scale);
	      if(!this->yAxis.userDefinedTics){
		this->computeMinMax(this->yAxis);
		this->computeTics(this->yAxis);
	      }
	    }
	    if(this->hasY2AxisCurve){
	      gdouble ny0 = (yu-l.by2)/(l.ay2);
	      gdouble ny1 = (yu+h-l.by2)/(l.ay2);
	      this->y2Axis.userDefinedRange = true;
	      this->setRange(this->y2Axis.min,
			     this->y2Axis.max,
			     ny0,ny1,
			     this->y2Axis.scale);
	      if(!this->y2Axis.userDefinedTics){
		this->computeMinMax(this->y2Axis);
		this->computeTics(this->y2Axis);
	      }
	    }
	  }
	}
	this->queue_draw();
      }
      return true;
    } // end of Graph::on_button_release_event

    bool
    Graph::on_motion_notify_event(GdkEventMotion* e)
    {
      using namespace std;
      if(this->drawZoomRectangle){
	gdouble xu;
	gdouble yu;
	this->zx2 = e->x;
	this->zy2 = e->y;
	gdouble dwidth  = abs(this->zx2-this->zx1);
	gdouble dheight = abs(this->zy2-this->zy1);
	if(this->zw<dwidth){
	  this->zw = dwidth;
	}
	if(this->zh<dheight){
	  this->zh = dheight;
	}
	if(this->zx1<this->zx2){
	  xu = this->zx1;
	} else {
	  xu = this->zx2;
	}
	if(this->zy1<this->zy2){
	  yu = this->zy1;
	} else {
	  yu = this->zy2;
	}
	this->queue_draw_area(static_cast<int>(xu-this->zw)-4,
			      static_cast<int>(yu-this->zh)-4,
			      2*static_cast<int>(this->zw)+8,
			      2*static_cast<int>(this->zh)+8);
      }
      return true;
    } // end of Graph::on_motion_notify_event

    double
    Graph::computeTicsIncrements(const double min,
				 const double max) const
    {
      using namespace std;
      const double guide = 20.;
      const double e = max-min;
      // magnitude
      const double n = pow(10.,floor(Graph::log10(e)));
      // normalised range
      const double xnorm = e/n;
      // dnorm shall be between 0 and 10
      double posns = guide / xnorm; /* approx number of tic posns per
				       decade */
      /* with guide=20, we expect 2 <= posns <= 20 */
      double d;
      if (posns > 40){
	d = 0.05;        /* eg 0, .05, .10, ... */
      } else if (posns > 20){
	d = 0.1;        /* eg 0, .1, .2, ... */
      } else if (posns > 10){
	d = 0.2;        /* eg 0,0.2,0.4,... */
      } else if (posns > 4){
	d = 0.5;        /* 0,0.5,1, */
      } else if (posns > 2){
	d = 1;        /* 0,1,2,.... */
      } else if (posns > 0.5){
	d = 2;        /* 0, 2, 4, 6 */
      } else {
	d = ceil(xnorm);
      }
      return (d*n);
    } // end of Graph::computeTicsIncrements
    
    std::string
    Graph::convertToString(const double d)
    {
      using namespace std;
      ostringstream res;
      res << d;
      return res.str();
    } // end of Graph::convertToString

    std::string
    Graph::convertTicsToString(const double d,
			       const Graph::Axis::AxisScale scale)
    {
      if(scale==Graph::Axis::LOGSCALE){
	return Graph::convertToString(pow(10.,d));
      } 
      return Graph::convertToString(d);
    }

    void
    Graph::computeLinearScaleTics(Graph::Axis& axis)
    {
      using namespace std;
      typedef std::map<double,string>::value_type MVType;
      const double d     = this->computeTicsIncrements(axis.min,axis.max);
      const double start = d*floor(axis.min/d);
      const double end   = d*ceil(axis.max/d);
      const double prec  = 100*abs(d)*numeric_limits<double>::epsilon();
      double t     = start;
      axis.tics.clear();
      while(t<end+0.5*d){
	if(!((t<axis.min)||(t>axis.max))){
	  if(abs(ceil(t)-t)<prec){
	    if(abs(t)<prec){
	      axis.tics.insert(MVType(0.,convertToString(0.)));
	    } else {
	      axis.tics.insert(MVType(ceil(t),convertToString(ceil(t))));
	    }
	  } else if(abs(floor(t)-t)<prec){
	    if(abs(t)<prec){
	      axis.tics.insert(MVType(0.,convertToString(0.)));
	    } else {
	      axis.tics.insert(MVType(floor(t),convertToString(floor(t))));
	    }
	  } else {
	    axis.tics.insert(MVType(t,convertToString(t)));
	  }
	}
	t+=d;
      }
    } // end of Graph::computeLinearScaleTics

    void
    Graph::insertIntoTics(std::map<double,std::string>& tics,
			  const double d,
			  const double d2)
    {
      using namespace std;
      typedef std::map<double,string>::value_type MVType;
      tics.insert(MVType(d,Graph::convertToString(d2)));
    } // end of Graph::insertIntoTics

    void
    Graph::computeLogScaleTics(std::map<double,std::string>& tics,
			       double& logmin,
			       double& logmax)
    {
      using namespace std;
      typedef std::map<double,string>::value_type MVType;
      static const double log10_2 = log10(2);
      static const double log10_3 = log10(3);
      static const double log10_4 = log10(4);
      static const double log10_5 = log10(5);
      static const double log10_6 = log10(6);
      static const double log10_7 = log10(7);
      static const double log10_8 = log10(8);
      static const double log10_9 = log10(9);
      logmin = floor(logmin);
      logmax = ceil(logmax);
      double min  = pow(10.,logmin);
      double deca = ceil(logmax-logmin);
      double nb   = deca/4;
      unsigned short i;
      tics.clear();
      if(nb>1){
	for(i=0;i!=5;++i){
	  this->insertIntoTics(tics,logmin+ceil(nb)*i,pow(10,logmin+ceil(nb)*i));
	  tics.insert(MVType(log10_2+logmin+i,""));
	  tics.insert(MVType(log10_3+logmin+i,""));
	  tics.insert(MVType(log10_4+logmin+i,""));
	  tics.insert(MVType(log10_5+logmin+i,""));
	  tics.insert(MVType(log10_6+logmin+i,""));
	  tics.insert(MVType(log10_7+logmin+i,""));
	  tics.insert(MVType(log10_8+logmin+i,""));
	  tics.insert(MVType(log10_9+logmin+i,""));
	}
	this->insertIntoTics(tics,logmin+ceil(nb)*5,pow(10,logmin+ceil(nb)*5));
      } else if(nb>=0.75){
	for(i=0;i!=4;++i){
	  this->insertIntoTics(tics,logmin+ceil(nb)*i,pow(10,logmin+ceil(nb)*i));
	  tics.insert(MVType(log10_2+logmin+i,""));
	  tics.insert(MVType(log10_3+logmin+i,""));
	  tics.insert(MVType(log10_4+logmin+i,""));
	  tics.insert(MVType(log10_5+logmin+i,""));
	  tics.insert(MVType(log10_6+logmin+i,""));
	  tics.insert(MVType(log10_7+logmin+i,""));
	  tics.insert(MVType(log10_8+logmin+i,""));
	  tics.insert(MVType(log10_9+logmin+i,""));
	}
	this->insertIntoTics(tics,logmin+ceil(nb)*4,pow(10,logmin+ceil(nb)*4));
      } else if(nb>=0.5){
	this->insertIntoTics(tics,logmin,min);
	tics.insert(MVType(log10_2+logmin,""));
	tics.insert(MVType(log10_3+logmin,""));
	tics.insert(MVType(log10_4+logmin,""));
	tics.insert(MVType(log10_5+logmin,""));
	tics.insert(MVType(log10_6+logmin,""));
	tics.insert(MVType(log10_7+logmin,""));
	tics.insert(MVType(log10_8+logmin,""));
	tics.insert(MVType(log10_9+logmin,""));
	this->insertIntoTics(tics,logmin+1,min*10);
	tics.insert(MVType(log10_2+logmin+1.,""));
	tics.insert(MVType(log10_3+logmin+1.,""));
	tics.insert(MVType(log10_4+logmin+1.,""));
	tics.insert(MVType(log10_5+logmin+1.,""));
	tics.insert(MVType(log10_6+logmin+1.,""));
	tics.insert(MVType(log10_7+logmin+1.,""));
	tics.insert(MVType(log10_8+logmin+1.,""));
	tics.insert(MVType(log10_9+logmin+1.,""));
	this->insertIntoTics(tics,logmin+2,min*100);
      } else {
	this->insertIntoTics(tics,logmin,min);
	tics.insert(MVType(log10_2+logmin,""));
	tics.insert(MVType(log10_3+logmin,""));
	tics.insert(MVType(log10_4+logmin,""));
	tics.insert(MVType(log10_5+logmin,""));
	tics.insert(MVType(log10_6+logmin,""));
	tics.insert(MVType(log10_7+logmin,""));
	tics.insert(MVType(log10_8+logmin,""));
	tics.insert(MVType(log10_9+logmin,""));
	this->insertIntoTics(tics,logmin+1,min*10);
      }
    } // end of Graph::computeLogScaleTics

    void
    Graph::computeTics(Graph::Axis& axis)
    {
      using namespace std;
      typedef std::map<double,string>::value_type MVType;
      if(axis.scale==Graph::Axis::LOGSCALE){
	this->computeLogScaleTics(axis.tics,
				  axis.min,
				  axis.max);
      } else {
	this->computeLinearScaleTics(axis);
      }
    } // end of Graph::computeTics

    void
    Graph::computeGraphLayout(Graph::CairoContext& cr,
			      const unsigned short gwidth,
			      const unsigned short gheight,
			      GraphLayout& l) const
    {
      cr->save();
      if(userDefinedFont){
	cr->select_font_face(font,this->slant,this->weight);
      }
      cr->set_font_size(this->fontSize);
      // margins
      l.ml = this->theme->getLeftMargin();
      l.mr = this->theme->getRightMargin();
      l.mu = this->theme->getUpperMargin();
      l.md = this->theme->getDownMargin();
      // titles
      l.ttl = this->theme->getLeftTitleWidth(cr,this->lTitle);
      l.ttr = this->theme->getRightTitleWidth(cr,this->rTitle);
      l.ttu = this->theme->getUpperTitleHeight(cr,this->uTitle);
      l.ttd = this->theme->getDownTitleHeight(cr,this->dTitle);
      // labels
      l.ll = this->theme->getYLabelWidth(cr,this->yAxis.label);
      l.lr = this->theme->getY2LabelWidth(cr,this->y2Axis.label);
      l.ld = this->theme->getXLabelHeight(cr,this->xAxis.label);
      l.lu = this->theme->getX2LabelHeight(cr,this->x2Axis.label);
      // tics
      if(this->hasYAxisCurve){
	l.tl = this->theme->getYTicsWidth(cr,this->yAxis.tics);
      } else {
	l.tl = 0u;
      }
      if(this->hasY2AxisCurve){
	l.tr = this->theme->getY2TicsWidth(cr,this->y2Axis.tics);
      } else {
	l.tr = 0u;
      }
      if(this->hasXAxisCurve){
	l.td = this->theme->getXTicsHeight(cr,this->xAxis.tics);
      } else {
	l.td = 0u;
      }
      if(this->hasX2AxisCurve){
	l.tu = this->theme->getX2TicsHeight(cr,this->x2Axis.tics);
      }  else {
	l.tu = 0u;
      }
      // graph borders
      this->theme->getGraphBorders(l.gl,l.gr,
				   l.gu,l.gd);
      // summing-up
      l.bl = static_cast<unsigned short>(l.ml+l.ttl+l.ll+l.tl+l.gl);
      l.br = static_cast<unsigned short>(l.mr+l.ttr+l.lr+l.tr+l.gr);
      l.bu = static_cast<unsigned short>(l.mu+l.ttu+l.lu+l.tu+l.gu);
      l.bd = static_cast<unsigned short>(l.md+l.ttd+l.ld+l.td+l.gd);
      // transformations
      const double xh0 = l.bl; // left border
      const double xh1 = gwidth-l.br; // right border
      const double yh0 = l.bu; // up border
      const double yh1 = gheight-l.bd; // down border
      l.ax = (this->xAxis.max-this->xAxis.min)/(xh1-xh0);
      l.bx = 0.5*(this->xAxis.max+this->xAxis.min-l.ax*(xh1+xh0));
      l.ay = (yh0-yh1)/(this->yAxis.max-this->yAxis.min);
      l.by = 0.5*(yh0+yh1-l.ay*(this->yAxis.max+this->yAxis.min));
      l.ax2 = (this->x2Axis.max-this->x2Axis.min)/(xh1-xh0);
      l.bx2 = 0.5*(this->x2Axis.max+this->x2Axis.min-l.ax2*(xh1+xh0));
      l.ay2 = (yh0-yh1)/(this->y2Axis.max-this->y2Axis.min);
      l.by2 = 0.5*(yh0+yh1-l.ay2*(this->y2Axis.max+this->y2Axis.min));
      cr->restore();
    } // end of Graph::computeGraphLayout

    void
    Graph::setGrid(const unsigned short g)
    {
      this->grid = g;
    } // end of Graph::setGrid

    unsigned short 
    Graph::getGrid(void) const
    {
      return this->grid;
    } // end of Graph::getGrid

    void
    Graph::plot(Graph::CairoContext& cr,
		const unsigned short pwidth,
		const unsigned short pheight) const
    {
      using namespace std;
      using namespace Glib;
      using namespace Cairo;
      using namespace Gdk;
      using namespace Gtk;

      GraphSize s;
      GraphLayout l;
      vector<CurveHandler>::const_iterator p;
      vector<string>::const_iterator p2;
      vector<Label>::const_iterator p3;
      vector<Arrow>::const_iterator p4;
      unsigned short nbr;

      cr->save();

      if(userDefinedFont){
	cr->select_font_face(this->font,this->slant,this->weight);
      }
      cr->set_font_size(this->fontSize);

      this->computeGraphLayout(cr,pwidth,pheight,l);

      s.width  = pwidth;
      s.height = pheight;
      s.xh0 = l.bl; // left border
      s.xh1 = static_cast<unsigned short>(s.width-l.br); // right border
      s.yh0 = l.bu; // up border
      s.yh1 = static_cast<unsigned short>(s.height-l.bd); // down border

      // draw the graph back ground
      this->theme->drawBackGround(cr,l,s);

      if(debugMode){
	// margins
	cr->save();
	cr->set_source_rgb(1.,0.,0.);
	cr->move_to(l.ml,l.mu);
	cr->line_to(l.ml,s.height-l.md);
	cr->line_to(s.width-l.mr,s.height-l.md);
	cr->line_to(s.width-l.mr,l.mu);
	cr->line_to(l.ml,l.mu);
	cr->stroke();
	cr->restore();
	// titles
	cr->save();
	cr->set_source_rgb(0.,1.,0.);
	cr->move_to(l.ml+l.ttl,l.mu+l.ttu);
	cr->line_to(l.ml+l.ttl,s.height-l.md-l.ttd);
	cr->line_to(s.width-l.mr-l.ttr,s.height-l.md-l.ttd);
	cr->line_to(s.width-l.mr-l.ttr,l.mu+l.ttu);
	cr->line_to(l.ml+l.ttl,l.mu+l.ttu);
	cr->stroke();
	cr->restore();
	// labels
	cr->save();
	cr->set_source_rgb(0.,0.,1.);
	cr->move_to(l.ml+l.ttl+l.ll,l.mu+l.ttu+l.lu);
	cr->line_to(l.ml+l.ttl+l.ll,s.height-l.md-l.ttd-l.ld);
	cr->line_to(s.width-l.mr-l.ttr-l.lr,s.height-l.md-l.ttd-l.ld);
	cr->line_to(s.width-l.mr-l.ttr-l.lr,l.mu+l.ttu+l.lu);
	cr->line_to(l.ml+l.ttl+l.ll,l.mu+l.ttu+l.lu);
	cr->stroke();
	cr->restore();
	// graph borders
	cr->save();
	cr->set_source_rgb(0.,0.,0.);
	cr->move_to(l.ml+l.ttl+l.ll+l.tl,l.mu+l.ttu+l.lu+l.tu);
	cr->line_to(l.ml+l.ttl+l.ll+l.tl,s.height-l.md-l.ttd-l.ld-l.td);
	cr->line_to(s.width-l.mr-l.ttr-l.lr-l.tr,s.height-l.md-l.ttd-l.ld-l.td);
	cr->line_to(s.width-l.mr-l.ttr-l.lr-l.tr,l.mu+l.ttu+l.lu+l.tu);
	cr->line_to(l.ml+l.ttl+l.ll+l.tl,l.mu+l.ttu+l.lu+l.tu);
	cr->stroke();
	cr->restore();
      }

      // Titles
      if(!this->uTitle.empty()){
	this->theme->printUpperTitle(cr,l,s,this->uTitle);
      }
      if(!this->dTitle.empty()){
	this->theme->printDownTitle(cr,l,s,this->dTitle);
      }
      if(!this->lTitle.empty()){
	this->theme->printLeftTitle(cr,l,s,this->lTitle);
      }
      if(!this->rTitle.empty()){
	this->theme->printRightTitle(cr,l,s,this->rTitle);
      }

      // Labels
      if(!this->xAxis.label.empty()){
	this->theme->printXLabel(cr,l,s,this->xAxis.label);
      }
      if(!this->yAxis.label.empty()){
	this->theme->printYLabel(cr,l,s,this->yAxis.label);
      }
      if(!this->x2Axis.label.empty()){
	this->theme->printX2Label(cr,l,s,this->x2Axis.label);
      }
      if(!this->y2Axis.label.empty()){
	this->theme->printY2Label(cr,l,s,this->y2Axis.label);
      }

      // draw tics
      if(this->hasXAxisCurve){
	this->theme->printXTics(cr,l,s,this->xAxis.tics);
      }
      if(this->hasYAxisCurve){
	this->theme->printYTics(cr,l,s,this->yAxis.tics);
      }
      if(this->hasX2AxisCurve){
	this->theme->printX2Tics(cr,l,s,this->x2Axis.tics);
      }
      if(this->hasY2AxisCurve){
	this->theme->printY2Tics(cr,l,s,this->y2Axis.tics);
      }

      // draw the graph back ground
      this->theme->drawGraphBackGround(cr,l,s);

      // draw curves
      nbr = 1u;
      for(p=this->curves.begin();p!=this->curves.end();++p,++nbr){
	if(p->curve->hasSpecifiedColor()){
	  if(p->axis==Graph::XY){
	    this->theme->drawCurve(cr,l.ax,l.bx,l.ay,l.by,
				   s,p->points,
				   p->curve->getStyle(),
				   p->curve->getColor(),
				   p->curve->getWidth(),nbr);
	  } else if(p->axis==Graph::X2Y){
	    this->theme->drawCurve(cr,l.ax2,l.bx2,l.ay,l.by,
				   s,p->points,
				   p->curve->getStyle(),
				   p->curve->getColor(),
				   p->curve->getWidth(),nbr);
	  } else if(p->axis==Graph::XY2){
	    this->theme->drawCurve(cr,l.ax,l.bx,l.ay2,l.by2,
				   s,p->points,
				   p->curve->getStyle(),
				   p->curve->getColor(),
				   p->curve->getWidth(),nbr);
	  } else {
	    this->theme->drawCurve(cr,l.ax2,l.bx2,l.ay2,l.by2,
				   s,p->points,
				   p->curve->getStyle(),
				   p->curve->getColor(),
				   p->curve->getWidth(),nbr);
	  }
	} else {
	  if(p->axis==Graph::XY){
	    this->theme->drawCurve(cr,l.ax,l.bx,l.ay,l.by,
				   s,p->points,
				   p->curve->getStyle(),
				   p->curve->getWidth(),nbr);
	  } else if(p->axis==Graph::X2Y){
	    this->theme->drawCurve(cr,l.ax2,l.bx2,l.ay,l.by,
				   s,p->points,
				   p->curve->getStyle(),
				   p->curve->getWidth(),nbr);
	  } else if(p->axis==Graph::XY2){
	    this->theme->drawCurve(cr,l.ax,l.bx,l.ay2,l.by2,
				   s,p->points,
				   p->curve->getStyle(),
				   p->curve->getWidth(),nbr);
	  } else {
	    this->theme->drawCurve(cr,l.ax2,l.bx2,l.ay2,l.by2,
				   s,p->points,
				   p->curve->getStyle(),
				   p->curve->getWidth(),nbr);
	  }
	}
      }

      this->theme->drawGrid(cr,l,s,this->showGraphGrid,this->grid,
			    this->xAxis.tics,this->yAxis.tics,
			    this->x2Axis.tics,this->y2Axis.tics,
			    this->xAxis.min,this->xAxis.max,
			    this->yAxis.min,this->yAxis.max,
			    this->x2Axis.min,this->x2Axis.max,
			    this->y2Axis.min,this->y2Axis.max,
			    this->hasXAxisCurve,
			    this->hasYAxisCurve,
			    this->hasX2AxisCurve,
			    this->hasY2AxisCurve);

      // x-axis
      if(this->showXAxis){
	this->theme->drawXAxis(cr,l,s);
      }

      // y-axis
      if(this->showYAxis){
	this->theme->drawYAxis(cr,l,s);
      }

      // legends
      nbr = 1u;
      for(p=this->curves.begin(),p2=this->legends.begin();
	  p!=this->curves.end();++p,++p2,++nbr){
	if(p->curve->hasSpecifiedColor()){
	  this->theme->printLegend(cr,l,s,*p2,
				   p->curve->getStyle(),
				   p->curve->getColor(),
				   nbr);
	} else {
	  this->theme->printLegend(cr,l,s,*p2,
				   p->curve->getStyle(),
				   nbr);
	}
      }

      // border
      if(this->showBorder){
	this->theme->drawGraphBorder(cr,l,s);
      }

      // draw the graph fore ground
      this->theme->drawGraphForeGround(cr,l,s);

      // draw the graph fore ground
      this->theme->drawForeGround(cr,l,s);

      // print labels
      for(p3=this->labels.begin();p3!=this->labels.end();++p3){
	this->theme->printLabel(cr,this->convertHorizontalGraphCoordinatesToAbsolutePosition(l,p3->x),
				this->convertVerticalGraphCoordinatesToAbsolutePosition(l,p3->y),
				p3->label);
      }

      // draw arrows
      for(p4=this->arrows.begin();p4!=this->arrows.end();++p4){
	this->theme->drawArrow(cr,this->convertHorizontalGraphCoordinatesToAbsolutePosition(l,p4->x0),
			       this->convertVerticalGraphCoordinatesToAbsolutePosition(l,p4->y0),
			       this->convertHorizontalGraphCoordinatesToAbsolutePosition(l,p4->x1),
			       this->convertVerticalGraphCoordinatesToAbsolutePosition(l,p4->y1),
			       p4->style);
      }

      if(this->drawZoomRectangle){
	cr->save();
	valarray<double> dashes(2);
	dashes[0]=4.;
	dashes[1]=8.;
	cr->set_dash(dashes,0.);
	cr->rectangle(this->zx1,this->zy1,
		      this->zx2-this->zx1,
		      this->zy2-this->zy1);
	cr->stroke();
	cr->restore();
      }

      cr->restore();

    } // end of Graph::plot

    void
    Graph::exportToTable(const std::string& file) const
    {
      using namespace std;
      vector<CurveHandler>::const_iterator p;
      vector<string>::const_iterator p2;
      vector<Point>::const_iterator pt;
      fstream f(file.c_str());
      for(p=this->curves.begin(),p2=this->legends.begin();
	  p!=this->curves.end();++p,++p2){
	f << "#" << *p2 << endl;
	for(pt=p->points.begin();pt!=p->points.end();++pt){
	  f << pt->x << " " << pt->y << endl;
	}
	f << endl;
      }
    } // end of Graph::exportToTable

    void
    Graph::exportToSVG(const std::string& file) const
    {
      using namespace std;
      using namespace Cairo;
      const unsigned short iwidth  = this->width;
      const unsigned short iheight = this->height;
      RefPtr<SvgSurface> surface = SvgSurface::create(file, iwidth, iheight);
      RefPtr<Context> cr = Context::create(surface);
      this->plot(cr,iwidth,iheight);
    } // end of Graph::exportToSVG

    void
    Graph::exportToPNG(const std::string& file) const
    {
      using namespace std;
      using namespace Cairo;
      const unsigned short iwidth  = this->width;
      const unsigned short iheight = this->height;
      RefPtr<ImageSurface> surface = ImageSurface::create(FORMAT_ARGB32, iwidth, iheight);
      RefPtr<Context> cr = Context::create(surface);
      this->plot(cr,iwidth,iheight);
      surface->write_to_png(file);
    } // end of Graph::exportToPNG

    void
    Graph::exportToPostScript(const std::string& file) const
    {
      using namespace std;
      using namespace Cairo;
      const unsigned short iwidth  = this->width;
      const unsigned short iheight = this->height;
      RefPtr<PsSurface> surface = PsSurface::create(file, iwidth, iheight);
      RefPtr<Context> cr = Context::create(surface);
      this->plot(cr,iwidth,iheight);
    } // end of Graph::exportToPostScript

    void
    Graph::exportToPDF(const std::string& file) const
    {
      using namespace std;
      using namespace Cairo;
      const unsigned short iwidth  = this->width;
      const unsigned short iheight = this->height;
      RefPtr<PdfSurface> surface = PdfSurface::create(file, iwidth, iheight);
      RefPtr<Context> cr = Context::create(surface);
      this->plot(cr,iwidth,iheight);
    } // end of Graph::print

    void
    Graph::clearCurves(void)
    {
      using namespace std;
      vector<CurveHandler>::const_iterator p;
      this->curves.clear();
      this->legends.clear();
      this->hasXAxisCurve  = false;
      this->hasX2AxisCurve = false;
      this->hasYAxisCurve  = false;
      this->hasY2AxisCurve = false;
    } // end of Graph::clearCurves

    void
    Graph::computeRange(double& x0,
			double& x1,
			const unsigned short axis){
      using namespace std;
      vector<CurveHandler>::const_iterator p;
      bool found = false;
      if(!this->curves.empty()){
	for(p=this->curves.begin();p!=this->curves.end();++p){
	  if((p->curve->hasRange())&&
	     (p->axis&axis)){
	    const double pmin = p->curve->minRange();
	    const double pmax = p->curve->maxRange();
	    if(!found){
	      x0 = pmin;
	      x1 = pmax;
	    } else {
	      if(x0<pmin){
		x1 = pmin;
	      }
	      if(x1>pmax){
		x1 = pmax;
	      }
	    }
	    found = true;
	  }
	} // end of for p
      }
      if(!found){
	x0 = Graph::defaultLinearScaleMinValue;
	x1 = Graph::defaultLinearScaleMaxValue;
      }
    } // end of Graph::computeRange()

    void
    Graph::computeRange2(double& x0,
			 double& x1,
			 const unsigned short axis){
      using namespace std;
      vector<CurveHandler>::const_iterator p;
      bool logx = false;
      if(this->xAxis.id==axis){
	logx = this->xAxis.scale == Graph::Axis::LOGSCALE;
      } else {
	logx = this->x2Axis.scale == Graph::Axis::LOGSCALE;
      }
      if(!this->curves.empty()){
	for(p=this->curves.begin();p!=this->curves.end();++p){
	  if((p->curve->hasRange())&&
	     (p->axis&axis)){
	    double pmin = p->curve->minRange();
	    double pmax = p->curve->maxRange();
	    if(logx){
	      pmin = Graph::log10(pmin);
	      pmax = Graph::log10(pmax);
	    }
	    if(x0<pmin){
	      x0 = pmin;
	    }
	    if(x1>pmax){
	      x1 = pmax;
	    }
	  }
	} // end of for p
      }
    } // end of Graph::computeRange2

    void Graph::computePoints(CurveHandler& h,
			      const double amin,
			      const double amax)
    {
      using namespace std;
      vector<Point>::iterator p;
      bool logx = false;
      bool logy = false;
      if(h.axis&Graph::xaxis){
	logx = this->xAxis.scale==Graph::Axis::LOGSCALE;
      } else {
	logx = this->x2Axis.scale==Graph::Axis::LOGSCALE;
      }
      if(h.axis&Graph::yaxis){
	logy = this->yAxis.scale==Graph::Axis::LOGSCALE;
      } else {
	logy = this->y2Axis.scale==Graph::Axis::LOGSCALE;
      }
      if(h.curve->hasSpecifiedNumberOfSamples()){
	h.curve->getValues(h.points,amin,amax,h.curve->getNumberOfSamples());
      } else {
	h.curve->getValues(h.points,amin,amax,this->samples);
      }
      if(logx){
	for(p=h.points.begin();p!=h.points.end();++p){
	  p->x = Graph::log10(p->x);
	}
      }
      if(logy){
	for(p=h.points.begin();p!=h.points.end();++p){
	  p->y = Graph::log10(p->y);
	}
      }
    } // end of Graph::computePoints

    void
    Graph::addCurve(CurveHandler& h)
    {
      using namespace std;
      this->curves.push_back(h);
      Graph::Axis& a1 = h.axis&Graph::xaxis ? this->xAxis : this->x2Axis;
      Graph::Axis& a2 = h.axis&Graph::yaxis ? this->yAxis : this->y2Axis;
      if(a2.userDefinedRange){
	this->updateCurve(*(--(this->curves.end())),a1);
      } else {
	this->updateCurve(*(--(this->curves.end())),a1);
	this->updateRange(a2);
      }
    } // end of Graph::addCurve2

    void
    Graph::addCurve(CurveHandler& h,
		    Graph::Axis& axis)
    {
      using namespace std;
      if(!axis.userDefinedRange){
	if(h.curve->hasRange()){
	  double x0 = h.curve->minRange();
	  double x1 = h.curve->maxRange();
	  if(axis.scale==Graph::Axis::LOGSCALE){
	    x0 = Graph::log10(x0);
	    x1 = Graph::log10(x1);
	  }
	  this->computeRange2(x0,x1,axis.id);
	  this->curves.push_back(h);
	  this->updateRange(axis,x0,x1);
	} else {
	  this->addCurve(h);
	}
      } else {
	this->addCurve(h);
      }
    } // end of Graph::addCurve
    
    void
    Graph::addCurve(tfel::utilities::shared_ptr<Curve> c,
		    const GraphAxis axis)
    {
      using namespace std;
      vector<CurveHandler>::const_iterator p;
      CurveHandler h;
      if(c.getPtr()==0){
	return;
      }
      h.curve = c;
      h.axis  = axis;
      if(axis&Graph::xaxis){
	this->hasXAxisCurve = true;
      } else {
	this->hasX2AxisCurve = true;
      }
      if(axis&Graph::yaxis){
	this->hasYAxisCurve = true;
      } else {
	this->hasY2AxisCurve = true;
      }
      if(axis&Graph::xaxis){
	this->addCurve(h,this->xAxis);
      } else {
	this->addCurve(h,this->x2Axis);
      }
      string name  = c->getLegend();
      if(name.empty()){
	ostringstream nbr;
	nbr << "Curve " << this->curves.size();
	name = nbr.str();
      }
      this->legends.push_back(name);
    } // end of Graph::addCurve

    void
    Graph::setUpperTitle(const std::string& l)
    {
      this->uTitle = l;
    } // end of Graph::setUpperTitle

    void
    Graph::setDownTitle(const std::string& l)
    {
      this->dTitle = l;
    } // end of Graph::setDownTitle

    void
    Graph::setLeftTitle(const std::string& l)
    {
      this->lTitle = l;
    } // end of Graph::setLeftTitle

    void
    Graph::setRightTitle(const std::string& l)
    {
      this->rTitle = l;
    } // end of Graph::setRightTitle

    void
    Graph::setXLabel(const std::string& l)
    {
      this->xAxis.label = l;
    } // end of Graph::setXLabel

    void
    Graph::setYLabel(const std::string& l)
    {
      this->yAxis.label = l;
    } // end of Graph::setYLabel

    void
    Graph::setX2Label(const std::string& l)
    {
      this->x2Axis.label = l;
    } // end of Graph::setX2Label

    void
    Graph::setY2Label(const std::string& l)
    {
      this->y2Axis.label = l;
    } // end of Graph::setY2Label

    void
    Graph::setFontFamily(const std::string& family)
    {
      this->userDefinedFont = true;
      this->font = family;
    } // end of Graph::setFontFamily

    void
    Graph::setDebugMode(const bool b)
    {
      this->debugMode = b;
    } // end of Graph::setFontFamily

    void
    Graph::unsetXRange(void)
    {
      this->xAxis.userDefinedRange = false;
      this->computeRange(this->xAxis.min,this->xAxis.max,Graph::xaxis);
      this->updateRange(this->xAxis,this->xAxis.min,this->xAxis.max);
    } // end of Graph::unsetXRange(void)

    void
    Graph::unsetX2Range(void)
    {
      this->x2Axis.userDefinedRange = false;
      this->computeRange(this->x2Axis.min,this->x2Axis.max,Graph::x2axis);
      this->updateRange(this->x2Axis,this->x2Axis.min,this->x2Axis.max);
    } // end of Graph::unsetX2Range(void)

    void
    Graph::unsetYRange(void)
    {
      this->yAxis.userDefinedRange = false;
      this->updateRange(this->yAxis);
    } // end of Graph::unsetYRange(void)

    void
    Graph::unsetY2Range(void)
    {
      this->y2Axis.userDefinedRange = false;
      this->updateRange(this->y2Axis);
    } // end of Graph::unsetY2Range(void)

    void
    Graph::setXRange(const double x0,const double x1)
    {
      this->xAxis.userDefinedRange = true;
      if(this->xAxis.scale==Graph::Axis::LOGSCALE){
	this->updateRange(this->xAxis,Graph::log10(x0),Graph::log10(x1));
      } else {
	this->updateRange(this->xAxis,x0,x1);
      }
    } // end of Graph::setXRange

    void
    Graph::updateCurve(CurveHandler& c,
		       const Graph::Axis& axis)
    {
      if(axis.scale==Graph::Axis::LOGSCALE){
	this->computePoints(c,pow(10.,axis.min),pow(10.,axis.max));
      } else {
	this->computePoints(c,axis.min,axis.max);
      }
    } // end of Graph::updateCurve

    void
    Graph::updateCurves(const Graph::Axis& axis)
    {
      using namespace std;
      vector<CurveHandler>::iterator p;
      for(p=this->curves.begin();p!=this->curves.end();++p){
	if(p->axis&axis.id){
	  this->updateCurve(*p,axis);
	}
      }
    } // end of Graph::updateCurves

    bool
    Graph::computeMinMax(Graph::Axis& axis)
    {
      return this->computeMinMax(axis.min,axis.max,axis);
    } // end of Graph::computeMinMax

    bool
    Graph::computeMinMax(double& fv0,
			 double& fv1,
			 Graph::Axis& axis)
    {
      using namespace std;
      vector<CurveHandler>::iterator p;
      vector<Point>::iterator p2;
      double v0 =  numeric_limits<double>::max();
      double v1 = -numeric_limits<double>::max();
      bool abscissa = (axis.id&Graph::xaxis)||(axis.id&Graph::x2axis);
      bool found = false;
      for(p=this->curves.begin();p!=this->curves.end();++p){
	if(p->axis&axis.id){
	  found = true;
	  for(p2=p->points.begin();p2!=p->points.end();++p2){
	    if(abscissa){
	      if(p2->x<v0){
		v0 = p2->x;
	      }
	      if(p2->x>v1){
		v1 = p2->x;
	      }
	    } else {
	      if(p2->y<v0){
		v0 = p2->y;
	      }
	      if(p2->y>v1){
		v1 = p2->y;
	      }
	    }
	  }
	}
      }
      if(found){
	fv0 = v0;
	fv1 = v1;
      }
      return found;
    } // end of Graph::computeMinMax

    void
    Graph::updateRange(Graph::Axis& axis)
    {
      using namespace std;
      bool found;
      if(!axis.userDefinedRange){
	double y0;
	double y1;
	found = this->computeMinMax(y0,y1,axis);
	if(found){
	  this->setRange(axis.min,axis.max,y0,y1,axis.scale);
	  if(!axis.userDefinedTics){
	    this->computeTics(axis);
	  }
	} else {
	  if(axis.scale==Graph::Axis::LOGSCALE){
	    this->setRange(axis.min,axis.max,
			   Graph::defaultLogScaleMinValue,
			   Graph::defaultLogScaleMaxValue,
			   axis.scale);
	  } else {
	    this->setRange(axis.min,axis.max,
			   Graph::defaultLinearScaleMinValue,
			   Graph::defaultLinearScaleMaxValue,
			   axis.scale);
	  }
	  if(!axis.userDefinedTics){
	    this->computeTics(axis);
	  }
	}
      }
    } // end of Graph::updateRange
    
    void
    Graph::updateRange(Graph::Axis& axis,
		       const double x0,const double x1)
    {
      using namespace std;
      this->setRange(axis.min,axis.max,x0,x1,axis.scale);
      if(!axis.userDefinedTics){
	this->computeTics(axis);      
      }
      this->updateCurves(axis);
      this->updateRange(this->yAxis);
      this->updateRange(this->y2Axis);
    } // end of Graph::updateXRange

    void
    Graph::setX2Range(const double x0,const double x1)
    {
      this->x2Axis.userDefinedRange = true;
      if(this->x2Axis.scale==Graph::Axis::LOGSCALE){
	this->updateRange(this->x2Axis,Graph::log10(x0),Graph::log10(x1));
      } else {
	this->updateRange(this->x2Axis,x0,x1);
      }
    } // end of Graph::setX2Range
    
    void
    Graph::setTics(Graph::Axis& axis,
		   const std::map<double,std::string>& tics)
    {
      axis.userDefinedTics = true;
      axis.tics.clear();
      axis.tics = tics;
    } // end of Graph::setXTics

    void
    Graph::setXTics(const std::map<double,std::string>& tics)
    {
      this->setTics(this->xAxis,tics);
    } // end of Graph::setXTics

    void
    Graph::setX2Tics(const std::map<double,std::string>& tics)
    {
      this->setTics(this->x2Axis,tics);
    } // end of Graph::setX2Tics

    void
    Graph::setYTics(const std::map<double,std::string>& tics)
    {
      this->setTics(this->yAxis,tics);
    } // end of Graph::setYTics

    void
    Graph::setY2Tics(const std::map<double,std::string>& tics)
    {
      this->setTics(this->y2Axis,tics);
    } // end of Graph::setY2Tics

    void
    Graph::unsetTics(Graph::Axis& axis)
    {
      axis.userDefinedTics = false;
      this->computeTics(axis);
    } // end of Graph::unsetXTics

    void
    Graph::unsetXTics(void)
    {
      this->unsetTics(this->xAxis);
    } // end of Graph::unsetXTics

    void
    Graph::unsetYTics(void)
    {
      this->unsetTics(this->yAxis);
    } // end of Graph::unsetXTics
    
    void
    Graph::unsetX2Tics(void)
    {
      this->unsetTics(this->x2Axis);
    } // end of Graph::unsetXTics

    void
    Graph::unsetY2Tics(void)
    {
      this->unsetTics(this->y2Axis);
    } // end of Graph::unsetXTics
    
    void
    Graph::setRange(double& rmin, double& rmax,
		    const double r0,const double r1,
		    const Graph::Axis::AxisScale scale)
    {
      using namespace std;
      if(scale==Graph::Axis::LOGSCALE){
	if((abs(r1-r0)<100*abs(r1)*numeric_limits<double>::min())||
	   (abs(r1-r0)<100*numeric_limits<double>::min())){
	  rmin = Graph::defaultLogScaleMinValue;
	  rmax = Graph::defaultLogScaleMaxValue;
	} else {
	  rmin = r0;
	  rmax = r1;
	}
      } else {
	if((abs(r1-r0)<100*abs(r1)*numeric_limits<double>::min())||
	   (abs(r1-r0)<100*numeric_limits<double>::min())){
	  if(abs(r1)<100*numeric_limits<double>::min()){
	  // ok drawing (almost) zero
	    rmin = Graph::defaultLinearScaleMinValue;
	    rmax = Graph::defaultLinearScaleMaxValue;
	  } else {
	    rmin = r1-abs(r1)*0.1;
	    rmax = r1+abs(r1)*0.1;
	  }
	} else {
	  rmin = r0;
	  rmax = r1;
	}
      }
    } // end of Graph::setRange

    void
    Graph::setRange(Graph::Axis& axis,
		    const double y0,const double y1,
		    const bool b)
    {
      using namespace std;
      axis.userDefinedRange = b;
      this->setRange(axis.min,axis.max,y0,y1,axis.scale);
      if(!axis.userDefinedTics){
	this->computeTics(axis);
      }
    } // end of Graph::setRange

    void
    Graph::setYRange(const double y0,const double y1)
    {
      using namespace std;
      if(this->yAxis.scale==Graph::Axis::LOGSCALE){
	this->setRange(this->yAxis,Graph::log10(y0),Graph::log10(y1));
      } else {
	this->setRange(this->yAxis,y0,y1);
      }
    } // end of Graph::setYRange

    void
    Graph::setY2Range(const double y0,const double y1)
    {
      if(this->y2Axis.scale==Graph::Axis::LOGSCALE){
	this->setRange(this->y2Axis,Graph::log10(y0),Graph::log10(y1));
      } else {
	this->setRange(this->y2Axis,y0,y1);
      }
    } // end of Graph::setY2Range

    void
    Graph::addLabel(const std::string& label,
		    const GraphCoordinates x,
		    const GraphCoordinates y)
    {
      this->labels.push_back(Label(label,x,y));
    } // end of Graph::addLabel

    void
    Graph::addLabel(const std::string& name,
		    const std::string& label,
		    const GraphCoordinates x,
		    const GraphCoordinates y)
    {
      using namespace std;
      vector<Graph::Label>::iterator p = this->findLabel(name);
      if(p==this->labels.end()){
	this->labels.push_back(Label(name,label,x,y));
      } else {
	*p = Label(name,label,x,y);
      }
    } // end of Graph::addLabel

    void
    Graph::addArrow(const GraphCoordinates x0,
		    const GraphCoordinates y0,
		    const GraphCoordinates x1,
		    const GraphCoordinates y1,
		    const ArrowStyle s)
    {
      this->arrows.push_back(Arrow(x0,y0,x1,y1,s));
    } // end of Graph::addArrow

    void
    Graph::addArrow(const std::string& name,
		    const GraphCoordinates x0,
		    const GraphCoordinates y0,
		    const GraphCoordinates x1,
		    const GraphCoordinates y1,
		    const ArrowStyle s)
    {
      using namespace std;
      vector<Graph::Arrow>::iterator p = this->findArrow(name);
      if(p==this->arrows.end()){
	this->arrows.push_back(Arrow(name,x0,y0,x1,y1,s));
      } else {
	*p = Arrow(name,x0,y0,x1,y1,s);
      }
    } // end of Graph::addArrow

    std::vector<Graph::Label>::iterator
    Graph::findLabel(const std::string& n)
    {
      using namespace std;
      vector<Graph::Label>::iterator p;
      for(p=this->labels.begin();
	  p!=this->labels.end();++p){
	if(p->name==n){
	  return p;
	}
      }
      return this->labels.end();
    } // end of Graph::findLabel

    std::vector<Graph::Arrow>::iterator
    Graph::findArrow(const std::string& n)
    {
      using namespace std;
      vector<Graph::Arrow>::iterator p;
      for(p=this->arrows.begin();
	  p!=this->arrows.end();++p){
	if(p->name==n){
	  return p;
	}
      }
      return this->arrows.end();
    } // end of Graph::findArrow

    void
    Graph::clearLabel(const std::string& n)
    {
      using namespace std;
      vector<Graph::Label>::iterator p = this->findLabel(n);
      if(p==this->labels.end()){
	string msg("Graph::clearLabel : ");
	msg += "unknown label '"+n+"'";
	throw(runtime_error(msg));
      }
      this->labels.erase(p);
    } // end of Graph::clearLabels(void)

    void
    Graph::clearLabels(void)
    {
      this->labels.clear();
    } // end of Graph::clearLabels(void)

    void
    Graph::clearArrow(const std::string& n)
    {
      using namespace std;
      vector<Graph::Arrow>::iterator p = this->findArrow(n);
      if(p==this->arrows.end()){
	string msg("Graph::clearArrow : ");
	msg += "unknown arrow '"+n+"'";
	throw(runtime_error(msg));
      }
      this->arrows.erase(p);
    } // end of Graph::clearArrows(void)

    void
    Graph::clearArrows(void)
    {
      this->arrows.clear();
    } // end of Graph::clearArrows(void)

    void
    Graph::showGrid(const bool b)
    {
      this->showGraphGrid = b;
    } // end of Graph::showGrid

    bool
    Graph::hasGrid(void) const
    {
      return this->showGraphGrid;
    } // end of Graph::showGrid

    double
    Graph::getXMin(void) const
    {
      return this->xAxis.min;
    } // end of Graph::getXMin

    double
    Graph::getXMax(void) const
    {
      return this->xAxis.max;
    } // end of Graph::getXMax

    double
    Graph::getYMin(void) const
    {
      return this->yAxis.min;
    } // end of Graph::getYMin

    double
    Graph::getYMax(void) const
    {
      return this->yAxis.max;
    } // end of Graph::getYMax

    double
    Graph::getX2Min(void) const
    {
      return this->x2Axis.min;
    } // end of Graph::getX2Min

    double
    Graph::getX2Max(void) const
    {
      return this->x2Axis.max;
    } // end of Graph::getX2Max

    double
    Graph::getY2Min(void) const
    {
      return this->y2Axis.min;
    } // end of Graph::getY2Min

    double
    Graph::getY2Max(void) const
    {
      return this->y2Axis.max;
    } // end of Graph::getY2Max

    tfel::utilities::shared_ptr<GraphTheme>
    Graph::getTheme(void)
    {
      return this->theme;
    } // end of Graph::getTheme

    tfel::utilities::shared_ptr<GraphTheme>
    Graph::getTheme(void) const
    {
      return this->theme;
    } // end of Graph::getTheme

  } // end of namespace graphics

} // end of namespace tfel
