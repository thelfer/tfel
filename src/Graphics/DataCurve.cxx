/*!
 * \file   DataCurve.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   20 jan 2008
 */

#include<iostream>
#include<iterator>
#include<algorithm>
#include<set>
#include<cassert>

#include"Graphics/DataCurve.hxx"

namespace tfel{

  namespace graphics{

    DataCurve::DataCurve()
      : style(Curve::LINE),
	hasColor(false)
    {} // end of DataCurve::DataCurve

    DataCurve::DataCurve(const std::vector<double>& x,
			 const std::vector<double>& y)
      : xvalues(x),
	yvalues(y),
	style(Curve::LINE),
	hasColor(false)
    {
      assert(xvalues.size()==yvalues.size());
    } // end of DataCurve::DataCurve

    bool
    DataCurve::hasSpecifiedColor(void) const
    {
      return this->hasColor;
    } // end of DataCurve::hasSpecifiedColor

    bool
    DataCurve::hasRange(void) const
    {
      return true;
    } // end of DataCurve::hasRange

    double
    DataCurve::minRange(void) const
    {
      using namespace std;
      return *(min_element(xvalues.begin(),xvalues.end()));
    } // end of DataCurve::minRange

    double
    DataCurve::maxRange(void) const
    {
      using namespace std;
      return *(max_element(xvalues.begin(),xvalues.end()));
    } // end of DataCurve::maxRange

    void
    DataCurve::getValues(std::vector<Point>& points,
			 const double x0,
			 const double x1,
			 const unsigned short)
    {
      using namespace std;
      set<vector<double>::size_type> index;
      vector<double>::size_type nbr;
      set<vector<double>::size_type>::const_iterator p;
      points.clear();
      if(this->xvalues.empty()){
	return;
      } else if(this->xvalues.size()==1u){
	const double x = this->xvalues.front();
	if((x>=x0)&&(x<=x1)){
	  Point point;
	  point.x = x;
	  point.y = this->yvalues.front();
	  points.push_back(point);
	}
      } else {
	for(nbr=0u;nbr!=this->xvalues.size()-1;++nbr){
	  const double xl = this->xvalues[nbr];
	  const double xr = this->xvalues[nbr+1];
	  if(((xl>=x0)&&(xl<=x1))||
	     ((xr>=x0)&&(xr<=x1))){
	    index.insert(nbr);
	    index.insert(nbr+1);
	  }
	}
	for(p=index.begin();p!=index.end();++p){
	  Point point;
	  point.x = this->xvalues[*p];
	  point.y = this->yvalues[*p];
	  points.push_back(point);
	}
      }
    } // end of DataCurve::getValues

    void
    DataCurve::setLegend(const std::string& l)
    {
      this->legend = l;
    } // end of DataCurve::setLegend

    void
    DataCurve::setColor(const Curve::Color& c)
    {
      this->hasColor = true;
      this->color = c;
    } // end of DataCurve::setColor

    void
    DataCurve::setStyle(const Style& s)
    {
      this->style = s;
    } // end of DataCurve::setStyle

    Curve::Color
    DataCurve::getColor(void) const
    {
      using namespace std;
      if(!this->hasColor){
	string msg("DataCurve::getColor : ");
	msg += "no color has been specified.";
	throw(runtime_error(msg));
      }
      return this->color;
    } // end of DataCurve::getColor

    Curve::Style
    DataCurve::getStyle(void) const
    {
      return this->style;
    } // end of DataCurve::setStyle

    std::string 
    DataCurve::getLegend(void) const
    {
      return this->legend;
    } // end of DataCurve::getLegend(void)

    bool
    DataCurve::hasSpecifiedNumberOfSamples(void) const
    {
      return false;
    } // end of DataCurve::hasSpecifiedNumberOfSamples

    void
    DataCurve::setNumberOfSamples(const unsigned short)
    {} // end of DataCurve::setNumberOfSamples

    unsigned short 
    DataCurve::getNumberOfSamples(void) const
    {
      return 0u;
    } // end of DataCurve::getNumberOfSamples

    DataCurve::~DataCurve()
    {} // end of DataCurve::~DataCurve

      std::vector<double> xvalues;
      std::vector<double> yvalues;

    std::vector<double>&
    DataCurve::getAbscissa(void)
    {
      return this->xvalues;
    } // end of DataCurve::getAbscissa

    const std::vector<double>&
    DataCurve::getAbscissa(void) const
    {
      return this->xvalues;
    } // end of DataCurve::getAbscissa

    std::vector<double>&
    DataCurve::getValues(void)
    {
      return this->yvalues;
    } // end of DataCurve::getValues

    const std::vector<double>&
    DataCurve::getValues(void) const
    {
      return this->yvalues;
    } // end of DataCurve::getValues

  } // end of namespace graphics

} // end of namespace tfel

