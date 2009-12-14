/*!
 * \file   PlotWindow.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   17 fév 2008
 */

#ifndef _LIB_TPLOT_PLOTWINDOW_HXX_
#define _LIB_TPLOT_PLOTWINDOW_HXX_ 

#include<gtkmm.h>
#include"Graphics/Graph.hxx"
#include"Graphics/PlotPrintOperation.hxx"

namespace tfel
{

  namespace graphics
  {

    struct PlotWindow
      : public Gtk::Window
    {
      PlotWindow();
      ~PlotWindow();
    protected:
      virtual bool on_key_press_event(GdkEventKey*);
      virtual void on_printoperation_status_changed(const Glib::RefPtr<PlotPrintOperation>&);
      virtual void on_printoperation_done(Gtk::PrintOperationResult,
					  const Glib::RefPtr<PlotPrintOperation>&);
      void chooseTheme(void);
      void exportGraph(void);
      void printOperation(Gtk::PrintOperationAction);
      void printPreview(void);
      void print(void);
      void draw(const Glib::RefPtr<Gtk::PrintContext>&,int);
      void toggleGrid(void);
      void toggleFullScreen(void);
      void addCurve(void);
      void quit(void);
      void about(void);
      Gtk::VBox vbox;
      Glib::RefPtr<Gtk::UIManager>   uiManager;
      Glib::RefPtr<Gtk::ActionGroup> actionGroup;
      //Printing-related objects:
      Glib::RefPtr<Gtk::PageSetup>     pageSetup;
      Glib::RefPtr<Gtk::PrintSettings> settings;
      Gtk::Widget* menu;
      Graph area;
      bool fullScreenMode;
    }; // end of struct PlotWindows

  } // end of namespace graphics

} // end of namespace tfel
  

#endif /* _LIB_TPLOT_PLOTWINDOW_HXX */

