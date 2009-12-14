/*!
 * \file   ElegantGraphTheme2.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   13 jan 2008
 */

#include<iostream>
#include<cstdlib>

#include"Graphics/ThemeManager.hxx"
#include"Graphics/ElegantGraphTheme2.hxx"

namespace tfel
{

  namespace graphics
  {
    
    ElegantGraphTheme2::~ElegantGraphTheme2()
    {} // end of ElegantGraphTheme2::~ElegantGraphTheme2

    void
    ElegantGraphTheme2::drawHorizontalBox(Cairo::RefPtr<Cairo::Context>& cr,
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
    } // end of ElegantGraphTheme2::drawHorizontalBox

    void
    ElegantGraphTheme2::drawVerticalBox(Cairo::RefPtr<Cairo::Context>& cr,
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
    } // end of ElegantGraphTheme2::drawVerticalBox

    StandardThemeProxy<ElegantGraphTheme2> elegantGraphTheme2Proxy("ElegantGraphTheme2");

  } // end of namespace graphics

} // end of namespace tfel


