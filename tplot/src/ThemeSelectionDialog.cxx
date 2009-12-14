/*!
 * \file   ThemeSelectionDialog.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   25 fév 2008
 */

#include<sstream>
#include"Graphics/ThemeManager.hxx"
#include"Graphics/ThemeSelectionDialog.hxx"

namespace tfel
{

  namespace graphics
  {

    ThemesList::ModelColumns::ModelColumns()
    {
      this->add(column);
    } // end of ThemesList::ModelColumns::ModelColumns
    
    ThemesList::ThemesList()
    {
      using namespace std;
      const vector<string>& themes = ThemeManager::getThemeManager().getAvailableThemesNames();
      vector<string>::const_iterator p;
      /* Create a new scrolled window, with scrollbars only if needed */
      this->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
      
      this->add(this->tree);
      
      /* create list store */
      this->list = Gtk::ListStore::create(this->m_Columns);
      
      tree.set_model(this->list);
      
      /* Add themes to the window */
      for(p=themes.begin();p!=themes.end();++p){
	Gtk::TreeModel::Row row = *(this->list->append());
	row[m_Columns.column] = *p;
      }
      
      //Add the Model's column to the View's columns:
      this->tree.append_column("Themes",this->m_Columns.column);
      this->show_all_children();

    } // end of ThemesList::ThemesList()

    const Glib::ustring
    ThemesList::getSelected(void)
    {
      using namespace std;
      using namespace Glib;
      using namespace Gtk;
      Glib::RefPtr<Gtk::TreeView::Selection> selection = this->tree.get_selection();
      if(selection){
	TreeModel::iterator iter = selection->get_selected();
	if(iter){
	  ustring theme = (*iter)[m_Columns.column];
	  return theme;
	}
      }
      return "";
    } // end of ThemesList::getSelected(void) const

    
    ThemesList::~ThemesList()
    {} // end of namespace ThemesList::~ThemesList()
     
    ThemeSelectionDialog::ThemeSelectionDialog()
    {
      using namespace Gtk;
      this->get_vbox()->pack_start(this->list);
      this->add_button(Stock::CANCEL,RESPONSE_CANCEL);
      this->add_button(Stock::APPLY,RESPONSE_APPLY);
      this->add_button(Stock::OK,RESPONSE_OK);
      this->set_default_response(RESPONSE_OK);
      this->show_all();
    } // end of ThemeSelectionDialog::ThemeSelectionDialog()

    const Glib::ustring
    ThemeSelectionDialog::getSelected(void)
    {
      return this->list.getSelected();
    } // end of ThemeSelectionDialog::getSelected

  } // end of namespace graphics

} // end of namespace tfel
