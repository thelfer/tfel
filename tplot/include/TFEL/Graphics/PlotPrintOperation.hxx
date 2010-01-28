/*!
 * \file   PlotPrintOperation.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   16 fév 2008
 */

#ifndef _LIB_TPLOT_PLOTPRINTOPERATION_HXX_
#define _LIB_TPLOT_PLOTPRINTOPERATION_HXX_ 

#include <gtkmm.h>
#include <vector>

#include"TFEL/Graphics/Graph.hxx"

namespace tfel
{

  namespace graphics
  {

    struct PlotPrintOperation
      : public Gtk::PrintOperation
    {
      
      static Glib::RefPtr<PlotPrintOperation> create();
      virtual ~PlotPrintOperation();
      
    protected:

      PlotPrintOperation();
      
      virtual void on_begin_print(const Glib::RefPtr<Gtk::PrintContext>&);
      
      virtual void on_draw_page(const Glib::RefPtr<Gtk::PrintContext>&,int);

    }; // end of PlotPrintOperation

  } // end of namespace graphics

} // end of namespace tfel

#endif /* _LIB_TPLOT_PLOTPRINTOPERATION_HXX */

