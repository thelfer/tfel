/*!
 * \file   graphics.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   04 jan 2008
 */

#ifndef _LIB_TFEL_GRAPHICS_HXX_
#define _LIB_TFEL_GRAPHICS_HXX_ 

#include<vector>
#include<string>
#include<gtkmm/drawingarea.h>

#include"Utilities/SmartPtr.hxx"
#include"Graphics/ArrowStyle.hxx"
#include"Graphics/Point.hxx"
#include"Graphics/Curve.hxx"
#include"Graphics/GraphTheme.hxx"
#include"Graphics/GraphCoordinates.hxx"

namespace tfel
{

  namespace graphics
  {

    struct Graph
      : public Gtk::DrawingArea
    {
      typedef Cairo::RefPtr<Cairo::Context> CairoContext;
      static const unsigned short xaxis  = 1u;
      static const unsigned short x2axis = 2u;
      static const unsigned short yaxis  = 4u;
      static const unsigned short y2axis = 8u;
      struct Label;
      struct Arrow;
      enum GraphAxis
      {
	  XY   = 5,
	  X2Y  = 6,
	  XY2  = 9,
	  X2Y2 = 10
      }; // end of enum GraphAxis
      struct Axis{
	enum AxisScale{
	  LINEARSCALE,
	  LOGSCALE
	}; // end of enum AxisScale
	explicit
	Axis(const unsigned short);
	void
	reset(void);
	const unsigned short id;
	std::map<double,std::string> tics;
	std::string label;
	double min;
	double max;
	bool userDefinedRange;
	bool userDefinedTics;
	AxisScale scale;
      }; // end of struct Axis
      Graph();
      void reset(void);
      void plot(CairoContext&,const unsigned short,const unsigned short) const;
      ~Graph();
      void showGrid(const bool = true);
      void setWidth(const unsigned short);
      void setHeight(const unsigned short);
      void unsetWidth(void);
      void unsetHeight(void);
      void setGrid(const unsigned short);
      void setXTics(const std::map<double,std::string>&);
      void setX2Tics(const std::map<double,std::string>&);
      void setYTics(const std::map<double,std::string>&);
      void setY2Tics(const std::map<double,std::string>&);
      void unsetXTics(void);
      void unsetX2Tics(void);
      void unsetYTics(void);
      void unsetY2Tics(void);
      unsigned short
      getGrid(void) const;
      bool
      hasGrid(void) const;
      void setTheme(const tfel::utilities::SmartPtr<GraphTheme>&);
      tfel::utilities::SmartPtr<GraphTheme>
      getTheme(void);
      tfel::utilities::SmartPtr<GraphTheme>
      getTheme(void) const;
      void exportToPDF(const std::string&) const;
      void exportToSVG(const std::string&) const;
      void exportToPostScript(const std::string&) const;
      void exportToPNG(const std::string&) const;
      void addLabel(const std::string&,
		    const std::string&,
		    const GraphCoordinates,
		    const GraphCoordinates);
      void addLabel(const std::string&,
		    const GraphCoordinates,
		    const GraphCoordinates);
      void clearLabel(const std::string&);
      void clearLabels(void);
      void addArrow(const GraphCoordinates,
		    const GraphCoordinates,
		    const GraphCoordinates,
		    const GraphCoordinates,
		    const ArrowStyle = ArrowStyle());
      void addArrow(const std::string&,
		    const GraphCoordinates,
		    const GraphCoordinates,
		    const GraphCoordinates,
		    const GraphCoordinates,
		    const ArrowStyle = ArrowStyle());
      void clearArrow(const std::string&);
      void clearArrows(void);
      void addCurve(tfel::utilities::SmartPtr<Curve>,const GraphAxis = XY);
      void clearCurves();
      void setXLabel(const std::string&);
      void setYLabel(const std::string&);
      void setX2Label(const std::string&);
      void setY2Label(const std::string&);
      void setUpperTitle(const std::string&);
      void setDownTitle(const std::string&);
      void setRightTitle(const std::string&);
      void setLeftTitle(const std::string&);
      void setFontFamily(const std::string&);
      void setDebugMode(const bool = true);
      void setXRange(const double,const double);
      void setYRange(const double,const double);
      void setX2Range(const double,const double);
      void setY2Range(const double,const double);
      void setXAxisLogScale(void);
      void unsetXAxisLogScale(void);
      void setX2AxisLogScale(void);
      void unsetX2AxisLogScale(void);
      void setYAxisLogScale(void);
      void unsetYAxisLogScale(void);
      void setY2AxisLogScale(void);
      void unsetY2AxisLogScale(void);
      void unsetXRange();
      void unsetYRange();
      void unsetX2Range();
      void unsetY2Range();
      double getXMin(void) const;
      double getXMax(void) const;
      double getYMin(void) const;
      double getYMax(void) const;
      double getX2Min(void) const;
      double getX2Max(void) const;
      double getY2Min(void) const;
      double getY2Max(void) const;
    protected:
      struct CurveHandler
      {
	tfel::utilities::SmartPtr<Curve> curve;
	std::vector<Point> points;
	GraphAxis axis;
      };
      //Override default signal handler:
      virtual bool on_expose_event(GdkEventExpose*);
      virtual bool on_button_press_event(GdkEventButton*);
      virtual bool on_button_release_event(GdkEventButton*);
      virtual bool on_motion_notify_event (GdkEventMotion*);
      static const double defaultLinearScaleMinValue;
      static const double defaultLinearScaleMaxValue;
      static const double defaultLogScaleMinValue;
      static const double defaultLogScaleMaxValue;
      static const unsigned short defaultWidth;
      static const unsigned short defaultHeight;
      static double log10(const double);
      std::vector<Graph::Label>::iterator
      findLabel(const std::string&);
      std::vector<Graph::Arrow>::iterator
      findArrow(const std::string&);
      void setTics(Graph::Axis&,
		   const std::map<double,std::string>&);
      void unsetTics(Graph::Axis&);
      void addCurve(CurveHandler&);
      void addCurve(CurveHandler&,
		    Graph::Axis&);
      void
      setLogScale(Graph::Axis&);
      void
      unsetLogScale(Graph::Axis&);
      std::string
      convertToString(const double);
      std::string
      convertTicsToString(const double,
			  const Graph::Axis::AxisScale);
      bool
      computeMinMax(double&,
		    double&,
		    Graph::Axis&);
      bool
      computeMinMax(Graph::Axis&);
      double
      convertHorizontalGraphCoordinatesToAbsolutePosition(const GraphLayout&,
							  const GraphCoordinates) const;
      double
      convertVerticalGraphCoordinatesToAbsolutePosition(const GraphLayout&,
							const GraphCoordinates) const;
      void
      setRange(double&, double&,
	       const double,const double,
	       const Graph::Axis::AxisScale);
      void
      setRange(Graph::Axis&,
	       const double,
	       const double,
	       const bool = true);
      void updateBordersSize(void);
      void updateCurves(const Graph::Axis&);
      void updateCurve(CurveHandler&,
		       const Graph::Axis&);
      void updateRange(Graph::Axis&);
      void updateRange(Graph::Axis&,
		       const double,
		       const double);
      void
      computePoints(CurveHandler&,
		    const double,
		    const double);
      void
      computeRange(double&,double&,const unsigned short);
      void
      computeRange2(double&,double&,const unsigned short);
      double
      computeTicsIncrements(const double,
			    const double) const;
      void
      insertIntoTics(std::map<double,std::string>&,
		     const double,
		     const double);
      void
      computeLinearScaleTics(Graph::Axis&);
      void
      computeLogScaleTics(std::map<double,std::string>&,
			  double&,
			  double&);
      void
      computeTics(Graph::Axis&);
      void
      computeGraphLayout(Graph::CairoContext&,
			 const unsigned short,
			 const unsigned short,
			 GraphLayout&) const;
      std::vector<CurveHandler> curves;
      tfel::utilities::SmartPtr<GraphTheme> theme;
      std::vector<std::string> legends;
      std::vector<Label> labels;
      std::vector<Arrow> arrows;
      Graph::Axis xAxis;
      Graph::Axis yAxis;
      Graph::Axis x2Axis;
      Graph::Axis y2Axis;
      std::string lTitle;
      std::string rTitle;
      std::string uTitle;
      std::string dTitle;
      std::string font;
      gdouble zw;
      gdouble zh;
      gdouble zx1;
      gdouble zx2;
      gdouble zy1;
      gdouble zy2;
      unsigned short width;
      unsigned short height;
      unsigned short samples; // default number of sample for curve
      Cairo::FontWeight weight;
      Cairo::FontSlant  slant;
      unsigned short grid;
      bool userDefinedFont;
      bool showGraphGrid;
      bool showXAxis;
      bool showYAxis;
      bool showBorder;
      bool hasXAxisCurve;
      bool hasX2AxisCurve;
      bool hasYAxisCurve;
      bool hasY2AxisCurve;
      bool drawZoomRectangle;
      bool debugMode;
    }; // end of struct graph

  } // end of namespace graphics

} // end of namespace tfel
  
#endif /* _LIB_TFEL_GRAPHICS_HXX */
