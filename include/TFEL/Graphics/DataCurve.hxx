/*!
 * \file   DataCurve.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   19 jan 2008
 */

#ifndef _LIB_TFEL_DATACURVE_HXX_
#define _LIB_TFEL_DATACURVE_HXX_ 

#include<vector>

#include"TFEL/Graphics/Curve.hxx"

namespace tfel{

  namespace graphics{

    struct DataCurve
      : public Curve
    {
      DataCurve();
      DataCurve(const std::vector<double>&,
		const std::vector<double>&);
      bool hasRange(void) const;
      double minRange(void) const;
      double maxRange(void) const;
      void
      getValues(std::vector<Point>&,const double,
		const double,const unsigned short);
      bool hasSpecifiedColor(void) const;
      bool hasSpecifiedNumberOfSamples(void) const;
      std::string getLegend(void) const;
      Curve::Color getColor(void) const;
      Curve::Style getStyle(void) const;
      unsigned short getNumberOfSamples(void) const;
      std::vector<double>&
      getAbscissa(void);
      const std::vector<double>&
      getAbscissa(void) const;
      std::vector<double>&
      getValues(void);
      const std::vector<double>&
      getValues(void) const;
      void setLegend(const std::string&);
      void setColor(const Color&);
      void setStyle(const Style&);
      void setNumberOfSamples(const unsigned short);
      ~DataCurve();
    public:
      std::vector<double> xvalues;
      std::vector<double> yvalues;
      std::string  legend;
      Curve::Color color;
      Curve::Style style;
      bool hasColor;
    }; // end of DataCurve
    
  } // end of namespace graphics

} // end of namespace tfel

#endif /* _LIB_TFEL_DATACURVE_HXX */

