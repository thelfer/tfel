/*!
 * \file   EvaluatedCurve.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   14 fév 2008
 */

#ifndef _LIB_TFEL_EVALUATEDCURVE_HXX_
#define _LIB_TFEL_EVALUATEDCURVE_HXX_ 

#include"TFEL/Utilities/SmartPtr.hxx"
#include"TFEL/Math/Evaluator.hxx"
#include"TFEL/Graphics/Curve.hxx"

namespace tfel
{

  namespace graphics
  {
    
    struct EvaluatedCurve
      : public Curve
    {
      EvaluatedCurve(tfel::utilities::SmartPtr<tfel::math::Evaluator>&,
		     const std::string& = "x");
      bool hasRange(void) const;
      double minRange(void) const;
      double maxRange(void) const;
      void
      getValues(std::vector<Point>&,const double,
		const double,const unsigned short);
      bool hasSpecifiedColor(void) const;
      bool hasSpecifiedNumberOfSamples(void) const;
      std::string getLegend(void)  const;
      Curve::Color getColor(void)  const;
      Curve::Style getStyle(void)  const;
      unsigned short getNumberOfSamples(void) const;
      void setLegend(const std::string&);
      void setColor(const Color&);
      void setStyle(const Style&);
      unsigned short getWidth() const;
      void setWidth(const unsigned short);
      void setNumberOfSamples(const unsigned short);
      ~EvaluatedCurve();
    private:
      tfel::utilities::SmartPtr<tfel::math::Evaluator> ev;
      std::string  legend;
      Curve::Color color;
      Curve::Style style;
      unsigned short samples;
      unsigned short width;
      bool hasSamples;
      bool hasColor;
      bool isConstant;
    }; // end of struct EvaluatedCurve

  } // end of namespace graphics

} // end of namespace tfel

#endif /* _LIB_TFEL_EVALUATEDCURVE_HXX */

