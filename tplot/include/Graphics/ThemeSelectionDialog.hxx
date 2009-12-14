/*!
 * \file   ThemeSelectionDialog.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   25 fév 2008
 */

#ifndef _LIB_TPLOT_THEMESELECTIONDIALOG_HXX_
#define _LIB_TPLOT_THEMESELECTIONDIALOG_HXX_ 

#include<string>
#include<gtkmm.h>

namespace tfel
{

  namespace graphics
  {

    struct ThemesList
      : public Gtk::ScrolledWindow
    {
      ThemesList();
      
      const Glib::ustring
      getSelected(void);
      
      virtual ~ThemesList();
      
    protected:

      struct ModelColumns
	: public Gtk::TreeModel::ColumnRecord
      {
      public:
	
	ModelColumns();
	
	Gtk::TreeModelColumn<Glib::ustring> column;
      }; // end of struct ModelColumns

      ModelColumns m_Columns;
      
      Glib::RefPtr<Gtk::ListStore> list; //The Tree Model.
      Gtk::TreeView tree; //The Tree View.
      
    }; // end of struct ThemesList

    struct ThemeSelectionDialog
      : public Gtk::Dialog
    {
      ThemeSelectionDialog();

      const Glib::ustring
      getSelected(void);

    private:
      ThemesList list;
    }; // end of struct ThemeSelectionDialog

  } // end of namespace graphics

} // end of namespace tfel

#endif /* _LIB_TPLOT_THEMESELECTIONDIALOG_HXX */

