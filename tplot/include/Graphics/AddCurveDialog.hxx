/*!
 * \file   AddFunctionDialog.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   22 fév 2008
 */

#ifndef _LIB_TPLOT_ADDFUNCTIONDIALOG_HXX_
#define _LIB_TPLOT_ADDFUNCTIONDIALOG_HXX_ 

#include<string>
#include<vector>
#include<gtkmm.h>

#include"Utilities/TextData.hxx"

#include"Graphics/Curve.hxx"
#include"Graphics/Graph.hxx"

namespace tfel
{

  namespace graphics
  {

    struct AddCurveDialog
      : public Gtk::Dialog
    {
      AddCurveDialog();
      void setDefaultColor(const Curve::Color&);
      std::vector<tfel::utilities::SmartPtr<Curve> >
      getCurve(void);
      Graph::GraphAxis
      getAxis(void);
    private:
      static double
      readDouble(const std::string&,
		 const unsigned short);
      static bool
      isInteger(const std::string&);
      static std::vector<std::string>
      tokenize(const std::string&);
      AddCurveDialog(const AddCurveDialog&);
      AddCurveDialog&
      operator=(const AddCurveDialog);
      static unsigned short
      convertToUnsignedShort(const std::string&);
      void
      response(int);
      void
      applyCurveSettings(Curve *const);
      std::vector<tfel::utilities::SmartPtr<Curve> >
      getEvaluatedCurve(void);
      std::string
      getData(std::vector<double>&,
	      const tfel::utilities::TextData&,
	      const std::string&);
      std::vector<tfel::utilities::SmartPtr<Curve> >
      getDataCurve(void);
      tfel::utilities::SmartPtr<Curve>
      getDataCurve(const tfel::utilities::TextData&,
		   const std::string&,
		   const std::string&);
      Gtk::Notebook  pages;
      Gtk::Table table;
      Gtk::Table etable;
      Gtk::Table dtable;
      Gtk::Alignment slAlign;
      Gtk::Alignment samplesAlign;
      Gtk::Alignment abscissaAlign;
      Gtk::Alignment columnAlign;
      Gtk::Alignment alAlign;
      Gtk::Alignment clAlign;
      Gtk::Alignment elAlign;
      Gtk::Alignment dlAlign;
      Gtk::Alignment nlAlign;
      Gtk::Alignment sAlign;
      Gtk::Alignment samplesLAlign;
      Gtk::Alignment abscissaLAlign;
      Gtk::Alignment columnLAlign;
      Gtk::Alignment aAlign;
      Gtk::Alignment cAlign;
      Gtk::Alignment eAlign;
      Gtk::Alignment nAlign;
      Gtk::FileChooserButton fButton;
      Gtk::Entry entry;
      Gtk::Entry samples;
      Gtk::Entry abscissa;
      Gtk::Entry column;
      Gtk::Label lEntry;
      Gtk::Label lSamples;
      Gtk::Label lAbscissa;
      Gtk::Label lColumn;
      Gtk::Label ldEntry;
      Gtk::Entry cName;
      Gtk::Label lName;
      Gtk::ColorButton colorButton;
      Gtk::Label lColor;
      Gtk::ComboBoxText cStyle;
      Gtk::Label lStyle;
      Gtk::ComboBoxText cAxis;
      Gtk::Label lAxis;
      Gdk::Color color;
    }; // end of struct AddCurveDialog

  } // end of namespace graphics

} // end of namespace tfel

#endif /* _LIB_TPLOT_ADDFUNCTIONDIALOG_HXX */

