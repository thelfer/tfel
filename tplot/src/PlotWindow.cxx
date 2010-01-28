/*!
 * \file   PlotWindow.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   17 fév 2008
 */

#include<iostream>
#include<vector>
#include<map>
#include<cassert>
#include<sstream>
#include<cctype>

#include<glibmm/i18n.h>

#include"TFEL/Graphics/Graph.hxx"
#include"TFEL/Graphics/AddCurveDialog.hxx"
#include"TFEL/Graphics/ThemeSelectionDialog.hxx"
#include"TFEL/Graphics/EvaluatedCurve.hxx"
#include"TFEL/Graphics/DataCurve.hxx"
#include"TFEL/Graphics/ThemeManager.hxx"
#include"TFEL/Graphics/PlotWindow.hxx"

namespace tfel
{

  namespace graphics
  {

    PlotWindow::PlotWindow()
      : pageSetup(Gtk::PageSetup::create()),
	settings(Gtk::PrintSettings::create()),
	fullScreenMode(false)
    {
      using namespace std;
      using namespace Gtk;
      using namespace Glib;
      using namespace tfel::utilities;
      this->set_default_size(640,400);
      this->add(this->vbox);
      this->actionGroup = ActionGroup::create();
      this->uiManager   = UIManager::create();
      this->actionGroup->add(Gtk::Action::create("FileMenu",_("_File")));
      this->actionGroup->add(Gtk::Action::create("EditMenu",_("_Edit")));
      this->actionGroup->add(Gtk::Action::create("ViewMenu",_("_View")));
      this->actionGroup->add(Gtk::Action::create("ThemeMenu",_("_Theme")));
      this->actionGroup->add(Gtk::Action::create("HelpMenu",Gtk::Stock::HELP));
      this->actionGroup->add(Gtk::Action::create("Export",_("_Export")),
			     sigc::mem_fun(*this, &PlotWindow::exportGraph));
      this->actionGroup->add(Gtk::Action::create("Print",Gtk::Stock::PRINT),
			     sigc::mem_fun(*this, &PlotWindow::print));
      this->actionGroup->add(Gtk::Action::create("PrintPreview",Gtk::Stock::PRINT_PREVIEW),
			     sigc::mem_fun(*this, &PlotWindow::printPreview));
      this->actionGroup->add(Gtk::Action::create("Quit", Gtk::Stock::QUIT),
			     sigc::mem_fun(*this, &PlotWindow::quit));
      this->actionGroup->add(Gtk::Action::create("AddCurve",_("Add Curve")),
			     sigc::mem_fun(*this, &PlotWindow::addCurve));
      this->actionGroup->add(Gtk::Action::create("FullScreen",_("FullScreen")),
			     sigc::mem_fun(*this, &PlotWindow::toggleFullScreen));
      this->actionGroup->add(Gtk::Action::create("ViewGrid",_("View grid")),
			     sigc::mem_fun(*this, &PlotWindow::toggleGrid));
      this->actionGroup->add(Gtk::Action::create("ChooseTheme",_("Choose theme")),
			     sigc::mem_fun(*this, &PlotWindow::chooseTheme));
      this->actionGroup->add(Gtk::Action::create("About",Gtk::Stock::ABOUT),
			     sigc::mem_fun(*this, &PlotWindow::about));
      this->uiManager->insert_action_group(actionGroup);
      this->add_accel_group(this->uiManager->get_accel_group());
      ustring ui_info = "<ui>"
	"  <menubar name='MenuBar'>"
	"    <menu action='FileMenu'>"
	"      <menuitem action='Export'/>"
	"      <menuitem action='Print'/>"
	"      <menuitem action='PrintPreview'/>"
	"      <menuitem action='Quit'/>"
	"    </menu>"
	"    <menu action='EditMenu'>"
	"      <menuitem action='AddCurve'/>"
	"    </menu>"
	"    <menu action='ViewMenu'>"
	"      <menuitem action='FullScreen'/>"
	"      <menuitem action='ViewGrid'/>"
	"      <menu action='ThemeMenu'>"
	"        <menuitem action='ChooseTheme'/>"
	"      </menu>"
	"    </menu>"
	"    <menu action='HelpMenu'>"
	"      <menuitem action='About'/>"
	"    </menu>"
	"  </menubar>"
	"</ui>";
      // 	ustring ui_info = "<ui>"
      // 	  "  <menubar name='MenuBar'>"
      // 	  "    <menu action='FileMenu'>"
      // 	  "      <menuitem action='Export'/>"
      // 	  "      <menuitem action='Quit'/>"
      // 	  "    </menu>"
      // 	  "    <menu action='EditMenu'>"
      // 	  "      <menuitem action='AddCurve'/>"
      // 	  "    </menu>"
      // 	  "    <menu action='ViewMenu'>"
      // 	  "      <menuitem action='FullScreen'/>"
      // 	  "      <menuitem action='ViewGrid'/>"
      // 	  "      <menu action='ThemeMenu'>"
      // 	  "        <menuitem action='ChooseTheme'/>"
      // 	  "        <menuitem action='LoadLibrary'/>"
      // 	  "      </menu>"
      // 	  "    </menu>"
      // 	  "    <menu action='HelpMenu'>"
      // 	  "      <menuitem action='About'/>"
      // 	  "    </menu>"
      // 	  "  </menubar>"
      // 	  "</ui>";
      try{
	this->uiManager->add_ui_from_string(ui_info);
      }
      catch(const Glib::Error& ex){
	cerr << "building menus failed: " <<  ex.what() << endl;
      }
      this->menu = this->uiManager->get_widget("/MenuBar");
      if(menu){
	this->vbox.pack_start(*(this->menu), Gtk::PACK_SHRINK);
      }
      this->vbox.pack_end(this->area);
      this->vbox.show();
      this->area.show();
      this->menu->hide();
    } // end of PlotWindow::PlotWindow
    
    void
    PlotWindow::chooseTheme(void)
    {
      using namespace std;
      using namespace Gtk;
      using namespace tfel::graphics;
      ThemeSelectionDialog dialog;
      string theme;
      int res;
      bool run = true;
      res = dialog.run();
      //Handle the response:
      while(run){
	int result = dialog.run();
	//Handle the response:
	if(result==RESPONSE_OK){
	  theme = dialog.getSelected();
	  if(!theme.empty()){
	    this->area.setTheme(ThemeManager::getThemeManager().getTheme(theme));
	  }
	  this->queue_draw();
	  run = false;
	} else if (result==RESPONSE_APPLY){
	  theme = dialog.getSelected();
	  if(!theme.empty()){
	    this->area.setTheme(ThemeManager::getThemeManager().getTheme(theme));
	  }
	  this->queue_draw();
	} else {
	  run = false;
	}
      }
    } // end of void PlotWindow::chooseTheme(void)

    bool
    PlotWindow::on_key_press_event(GdkEventKey* e)
    {
      using namespace std;
      if(e->keyval=='m'){
	if(this->menu->is_visible()){
	  this->menu->hide();
	} else {
	  this->menu->show();
	}
      } else if(e->keyval=='a'){
	this->addCurve();
      } else if(e->keyval=='f'){
	this->toggleFullScreen();
      } else if(e->keyval=='g'){
	this->toggleGrid();
      } else if(e->keyval=='e'){
	this->exportGraph();
      } else if(e->keyval=='q'){
	this->quit();
      } else if(e->keyval=='t'){
	this->chooseTheme();
      } else {
	Gtk::Window::on_key_press_event(e);
      }
      return true;
    } // end of PlotWindow::on_key_press_event

    void
    PlotWindow::print(void)
    {
      this->printOperation(Gtk::PRINT_OPERATION_ACTION_PRINT_DIALOG);
    } // end of PlotWindow::print()

    void
    PlotWindow::printPreview(void)
    {
      this->printOperation(Gtk::PRINT_OPERATION_ACTION_PREVIEW);
    } // end of PlotWindow::print()

    void
    PlotWindow::exportGraph(void)
    {
      using namespace std;
      using namespace Gtk;
      string filename;
      FileChooserDialog dialog("Please choose a file",
			       FILE_CHOOSER_ACTION_SAVE);
      dialog.set_transient_for(*this);
      
      //Add response buttons the the dialog:
      dialog.add_button(Stock::CANCEL, RESPONSE_CANCEL);
      dialog.add_button(Stock::OPEN, RESPONSE_OK);
      
      //Add filters, so that only certain file types can be selected:

      FileFilter filter_eps;
      filter_eps.set_name(_("Encapsulated PostScript"));
      filter_eps.add_mime_type("application/postscript");
      dialog.add_filter(filter_eps);

      FileFilter filter_svg;
      filter_svg.set_name("SVG files");
      filter_svg.add_mime_type("image/svg+xml");
      dialog.add_filter(filter_svg);

      FileFilter filter_pdf;
      filter_pdf.set_name("Portable Document Format");
      filter_pdf.add_pattern("application/pdf");
      dialog.add_filter(filter_pdf);
      
      FileFilter filter_png;
      filter_png.set_name("Portable Network Graphics");
      filter_png.add_pattern("image/png");
      dialog.add_filter(filter_png);

      //Show the dialog and wait for a user response:
      int result = dialog.run();

      //Handle the response:
      if(result==RESPONSE_CANCEL){
	return;
      }
      filename = dialog.get_filename();
      string extension;
      string::size_type pos = filename.find_last_of('.');
      if(pos==string::npos){
	string msg("PlotWindow::PlotWindow : ");
	msg += "invalid filename name " + filename;
	throw(runtime_error(msg));
      }
      extension = filename.substr(pos+1);
      if(extension=="eps"){
	this->area.exportToPostScript(filename);
      } else if(extension=="pdf"){
	this->area.exportToPDF(filename);
      } else if(extension=="svg"){
	this->area.exportToSVG(filename);
      } else if(extension=="png"){
	this->area.exportToPNG(filename);
      } else {
	string msg("PlotWindow::PlotWindow : ");
	msg += "invalid filename name " + filename;
	msg += " (unsupported extension "+extension+")";
	throw(runtime_error(msg));
      }
    } // end of PlotWindow::exportGraph()

    void
    PlotWindow::quit(void)
    {
      this->hide();
    } // end of PlotWindow::quit()

    void
    PlotWindow::toggleFullScreen(void)
    {
      if(!this->fullScreenMode){
	this->fullScreenMode = true;
	this->fullscreen();
      } else {
	this->fullScreenMode = false;
	this->unfullscreen();
      }
    } // end of PlotWindow::toggleFullScreen()

    void
    PlotWindow::toggleGrid(void)
    {
      if(this->area.hasGrid()){
	this->area.showGrid(false);
      } else {
	this->area.showGrid();
      }
      this->queue_draw();
    } // end of PlotWindow::toggleGrid()

    void
    PlotWindow::addCurve(void)
    {
      using namespace std;
      using namespace Gtk;
      AddCurveDialog dialog;
      Curve::Color color;
      this->area.getTheme()->getDefaultColor(color,1);
      dialog.setDefaultColor(color);
      bool run = true;
      dialog.set_transient_for(*this);
      //Show the dialog and wait for a user response:
      while(run){
	int result = dialog.run();
	//Handle the response:
	if(result==RESPONSE_OK){
	  const vector<tfel::utilities::SmartPtr<Curve> >& curves = dialog.getCurve();
	  const Graph::GraphAxis axis = dialog.getAxis(); 
	  vector<tfel::utilities::SmartPtr<Curve> >::const_iterator p;
	  for(p=curves.begin();p!=curves.end();++p){
	    this->area.addCurve(*p,axis);	    
	  }
	  this->queue_draw();
	  run = false;
	} else if (result==RESPONSE_APPLY){
	  const vector<tfel::utilities::SmartPtr<Curve> >& curves = dialog.getCurve();
	  const Graph::GraphAxis axis = dialog.getAxis(); 
	  vector<tfel::utilities::SmartPtr<Curve> >::const_iterator p;
	  for(p=curves.begin();p!=curves.end();++p){
	    this->area.addCurve(*p,axis);	    
	  }
	  this->queue_draw();
	} else {
	  run = false;
	}
      }
    } // end of PlotWindow::addCurve

    void
    PlotWindow::about(void)
    {
      Gtk::AboutDialog aDialog;
      aDialog.set_transient_for(*this);
      aDialog.set_name(_("about plot"));
      //       aDialog.set_website("http://gtkmm.org/";);
      //       aDialog.set_website_label("gtkmm Website");
      aDialog.set_version(VERSION);
      aDialog.set_copyright("\xC2\xA9 2008 Helfer Thomas");
      aDialog.set_comments("Comments about the application");
      aDialog.set_license("GPL");
      
      std::vector<Glib::ustring> authors;
      authors.push_back("Helfer Thomas");
      aDialog.set_authors(authors);
      
      std::vector<Glib::ustring> documenters;
      documenters.push_back("Helfer Thomas");
      aDialog.set_documenters(documenters);

      aDialog.set_translator_credits("Helfer Thomas");
      aDialog.run();
    } // end of PlotWindow::about()

    PlotWindow::~PlotWindow()
    {} // end of PlotWindow::PlotWindow

    void
    PlotWindow::on_printoperation_status_changed(const Glib::RefPtr<PlotPrintOperation>& op)
    {
      Glib::ustring status;
      if(op->is_finished()){
	status = "Print job completed.";
      } else {
	//You could also use get_status().
	status = op->get_status_string();
      }
      // m_Statusbar.push(status, m_ContextId);
    } // end of PlotWindow::on_printoperation_status_changed
  
    void
    PlotWindow::on_printoperation_done(Gtk::PrintOperationResult res,
				       const Glib::RefPtr<PlotPrintOperation>& op)
    {
      //Printing is "done" when the print data is spooled.
      if (res == Gtk::PRINT_OPERATION_RESULT_ERROR){
	Gtk::MessageDialog err_dialog(*this, "Error printing form", false,
				      Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK, true);
	err_dialog.run();
      } else if (res == Gtk::PRINT_OPERATION_RESULT_APPLY){
	//Update PrintSettings with the ones used in this PrintOperation:
	this->settings = op->get_print_settings();
      }

      if (!op->is_finished()){
	//We will connect to the status-changed signal to track status
	//and update a status bar. In addition, you can, for example,
	//keep a list of active print ops, or provide a progress dialog.
	op->signal_status_changed().connect(sigc::bind(sigc::mem_fun(*this,
								     &PlotWindow::on_printoperation_status_changed),op));
      }
    } // end of PlotWindow::on_printoperation_done

    void
    PlotWindow::printOperation(Gtk::PrintOperationAction action)
    {
      //Create a new PrintOperation with our PageSetup and PrintSettings:
      //(We use our derived PrintOperation class)
      Glib::RefPtr<PlotPrintOperation> op = PlotPrintOperation::create();
      op->set_track_print_status();
      op->set_default_page_setup(pageSetup);
      op->set_print_settings(settings);

      op->signal_done().connect(sigc::bind(sigc::mem_fun(*this,
							 &PlotWindow::on_printoperation_done),op));

      op->signal_draw_page().connect(sigc::mem_fun(*this,
						   &PlotWindow::draw));

      try{
	op->run(action,*this);
      }
      catch(const Gtk::PrintError& ex){
	//See documentation for exact Gtk::PrintError error codes.
	std::cerr << "An error occurred while trying to run a print operation:"
		  << ex.what() << std::endl;
      }
    } // end of PlotWindow::printOperation

    void
    PlotWindow::draw(const Glib::RefPtr<Gtk::PrintContext>& context,int)
    {
      const double width  = context->get_width();
      const double height = context->get_height();
      Cairo::RefPtr<Cairo::Context> ctx = context->get_cairo_context();
      this->area.plot(ctx,static_cast<unsigned short>(width),static_cast<unsigned short>(height));
    } // end of PlotWindow::draw

  } // end of namespace graphics

} // end of namespace tfel

