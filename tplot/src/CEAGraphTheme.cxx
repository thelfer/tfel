/*!
 * \file   CEAGraphTheme.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   13 jan 2008
 */

#include"TFEL/Graphics/ThemeManager.hxx"
#include"TFEL/Graphics/CEAGraphTheme.hxx"

namespace tfel
{

  namespace graphics
  {

    const Curve::Color
    CEAGraphTheme::getFirstBoxColor(void) const
    {
      Curve::Color c;
      c.r = 0.43921;
      c.g = 0.7372;
      c.b = 0.1215;
      return c;
    } // end of CEAGraphTheme::getFirstBoxColor

    const Curve::Color
    CEAGraphTheme::getSecondBoxColor(void) const
    {
      Curve::Color c;
      c.r = 0.;
      c.g = 1.;
      c.b = 0.17;
      return c;
    } // end of CEAGraphTheme::getSecondBoxColor

    const Curve::Color
    CEAGraphTheme::getTextColor(void) const
    {
      Curve::Color c;
      c.r = 1.;
      c.g = 1.;
      c.b = 1.;
      return c;
    } // end of CEAGraphTheme::getSecondBoxColor

    void
    CEAGraphTheme::drawBackGround(Cairo::RefPtr<Cairo::Context>& cr,
				  const GraphLayout&,
				  const GraphSize& s) const
    {
      cr->save();
      cr->set_source_rgb(1.,1.,1.);
      cr->paint();
      cr->set_source_rgb(0.6,0.6,0.6);
      cr->rectangle(s.xh0+10,s.yh0+10,s.xh1-s.xh0,s.yh1-s.yh0);
      cr->fill();
      cr->restore();
    } // end of CEAGraphTheme::drawBackGround

    void
    CEAGraphTheme::getGraphBorders(unsigned short& gl,unsigned short& gr,
				   unsigned short& gu,unsigned short& gd) const
    {
      gl = 6;
      gr = 16;
      gu = 6;
      gd = 16;
    } // end of CEAGraphTheme::getGraphBorders

    CEAGraphTheme::~CEAGraphTheme()
    {} // end of CEAGraphTheme::~CEAGraphTheme

    StandardThemeProxy<CEAGraphTheme> ceaGraphThemeProxy("CEAGraphTheme");

  } // end of namespace graphics

} // end of namespace tfel


