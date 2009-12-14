/*!
 * \file   PrintPlotOperation.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   16 fév 2008
 */

#include"Graphics/PlotPrintOperation.hxx"

namespace tfel
{

  namespace graphics
  {

    PlotPrintOperation::PlotPrintOperation()
    {} // end of PlotPrintOperation::PlotPrintOperation

    PlotPrintOperation::~PlotPrintOperation()
    {} // end of PlotPrintOperation::~PlotPrintOperation()

    Glib::RefPtr<PlotPrintOperation>
    PlotPrintOperation::create()
    {
      return Glib::RefPtr<PlotPrintOperation>(new PlotPrintOperation());
    } // end of PlotPrintOperation::create

    void
    PlotPrintOperation::on_begin_print(const Glib::RefPtr<Gtk::PrintContext>&)
    {
      this->set_n_pages(1);
    } // end of PlotPrintOperation::on_begin_print
    
    void
    PlotPrintOperation::on_draw_page(const Glib::RefPtr<Gtk::PrintContext>&,int)
    {} // end of PlotPrintOperation::on_draw_page

  } // end of namespace graphics

} // end of namespace tfel

