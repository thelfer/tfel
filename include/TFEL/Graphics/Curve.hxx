/*!
 * \file   Curves.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   04 jan 2008
 */

#ifndef _LIB_TFEL_CURVES_HXX_
#define _LIB_TFEL_CURVES_HXX_ 

#include<vector>
#include<cmath>
#include<string>
#include<stdexcept>
#include<limits>

#include"TFEL/Graphics/Point.hxx"

namespace tfel{

  namespace graphics{

    struct Curve
    {
      enum Style{
	LINE,
	LINEPOINT,
	DOT,
	SQUARE,
	TRIANGLE,
	CROSS,
	DIAMOND,
	POINT
      };
      struct Color
      {
	double r;
	double g;
	double b;
      }; // end of struct Color
      virtual bool hasRange(void) const = 0;
      virtual double minRange(void) const = 0;
      virtual double maxRange(void) const = 0;
      virtual void
      getValues(std::vector<Point>&,const double,
		const double,const unsigned short) = 0;
      virtual bool hasSpecifiedColor(void)   const = 0;
      virtual bool hasSpecifiedNumberOfSamples(void)   const = 0;
      virtual std::string    getLegend(void) const = 0;
      virtual Curve::Color   getColor(void)  const = 0;
      virtual Curve::Style   getStyle(void)  const = 0;
      virtual unsigned short getNumberOfSamples(void) const = 0;
      virtual void setLegend(const std::string&) = 0;
      virtual void setColor(const Color&) = 0;
      virtual void setStyle(const Style&) = 0;
      virtual void setNumberOfSamples(const unsigned short) = 0;
      virtual unsigned short getWidth() const = 0;
      virtual void setWidth(const unsigned short) = 0;
      virtual ~Curve();
    }; // end of struct Curve

  } // end of namespace graphics

} // end of namespace tfel

#endif /* _LIB_TFEL_CURVES_HXX */

