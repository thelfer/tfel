/*!
 * \file   EvaluatedCurve.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   14 fév 2008
 */

#include<iostream>
#include"Graphics/EvaluatedCurve.hxx"

namespace tfel{

  namespace graphics{

    EvaluatedCurve::EvaluatedCurve(tfel::utilities::SmartPtr<tfel::math::Evaluator>& f,
				   const std::string& var)
      : ev(f),
	style(Curve::LINE),
	hasSamples(false),
	hasColor(false),
	isConstant(false)
    {
      using namespace std;
      const vector<string>& names = this->ev->getVariablesNames();
      if((names.size()!=0u)&&
	 (names.size()!=1u)){
	vector<string>::const_iterator p;
	string msg("EvaluatedCurve::EvaluatedCurve : ");
	msg += "function declares more than one variable ";
	msg += "(read : ";
	p = names.begin();
	while(p != names.end()){
	  msg += *p;
	  ++p;
	  if(p!=names.end()){
	    msg += ").";
	  } else {
	    msg += " ";
	  }
	}
      }
      if(names.size()==1u){
	if(names[0]!=var){
	  string msg("EvaluatedCurve::EvaluatedCurve : ");
	  msg += "function declares another variable than '"+var+"' ";
	  msg += "(read : " +names[0]+")";
	}
      } else {
	this->isConstant = true;
      }
    } // end of EvaluatedCurve::EvaluatedCurve

    bool
    EvaluatedCurve::hasSpecifiedColor(void) const
    {
      return this->hasColor;
    } // end of EvaluatedCurve::hasSpecifiedColor

    bool
    EvaluatedCurve::hasRange(void) const
    {
      return false;
    } // end of EvaluatedCurve::hasRange

    double
    EvaluatedCurve::minRange(void) const
    {
      using namespace std;
      return -numeric_limits<double>::max();
    } // end of EvaluatedCurve::

    double
    EvaluatedCurve::maxRange(void) const
    {
      using namespace std;
      return numeric_limits<double>::max();
    } // end of EvaluatedCurve::maxRange

    EvaluatedCurve::~EvaluatedCurve()
    {} // end of EvaluatedCurve::~EvaluatedCurve

    std::string 
    EvaluatedCurve::getLegend(void) const
    {
      return this->legend;
    } // end of EvaluatedCurve::getLegend(void)

    void
    EvaluatedCurve::getValues(std::vector<Point>& points,const double xmin,
				const double xmax,const unsigned short s)
    {
      using namespace std;
      const double dx = (xmax-xmin)/s;
      points.clear();
      if(this->isConstant){
	for(double x = xmin;x<xmax+0.5*dx;x+=dx){
	  Point p;
	  p.x = x;
	  p.y = this->ev->getValue();
	  points.push_back(p);
	}
      } else {
	for(double x = xmin;x<xmax+0.5*dx;x+=dx){
	  Point p;
	  this->ev->setVariableValue(0,x);
	  p.x = x;
	  try {
	    p.y = this->ev->getValue();
	  } catch(runtime_error& e){
	    cerr << e.what() << endl;
	    p.y = nan(e.what());
	  }
	  points.push_back(p);
	}
      }
    } // end of EvaluatedCurve::getValues

    void
    EvaluatedCurve::setLegend(const std::string& l)
    {
      this->legend = l;
    } // end of EvaluatedCurve::setLegend

    void
    EvaluatedCurve::setColor(const Curve::Color& c)
    {
      this->hasColor = true;
      this->color = c;
    } // end of EvaluatedCurve::setColor

    void
    EvaluatedCurve::setStyle(const Style& s)
    {
      this->style = s;
    } // end of EvaluatedCurve::getStyle

    Curve::Color
    EvaluatedCurve::getColor(void) const
    {
      using namespace std;
      if(!this->hasColor){
	string msg("EvaluatedCurve::getColor : ");
	msg += "no color has been specified.";
	throw(runtime_error(msg));
      }
      return this->color;
    } // end of EvaluatedCurve::getColor

    Curve::Style
    EvaluatedCurve::getStyle(void) const
    {
      return this->style;
    } // end of EvaluatedCurve::getStyle

    bool
    EvaluatedCurve::hasSpecifiedNumberOfSamples(void) const
    {
      return this->hasSamples;
    } // end of EvaluatedCurve::hasSpecifiedNumberOfSamples

    void
    EvaluatedCurve::setNumberOfSamples(const unsigned short f)
    {
      this->hasSamples = true;
      this->samples = f;
    } // end of EvaluatedCurve::setNumberOfSamples

    unsigned short 
    EvaluatedCurve::getNumberOfSamples(void) const
    {
      return this->samples;
    } // end of EvaluatedCurve::getNumberOfSamples

  } // end of namespace graphics

} // end of namespace tfel
