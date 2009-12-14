/*!
 * \file   AddFunction.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   22 fév 2008
 */

#include<iostream>
#include<limits>
#include<sstream>
#include<stdexcept>
#include<glibmm/i18n.h>

#include"Utilities/SmartPtr.hxx"
#include"Utilities/TextData.hxx"
#include"Graphics/EvaluatedCurve.hxx"
#include"Graphics/DataCurve.hxx"
#include"Graphics/AddCurveDialog.hxx"

namespace tfel
{

  namespace graphics
  {

    double
    AddCurveDialog::readDouble(const std::string& s,
			       const unsigned short l)
    {
      using namespace std;
      double res;
      istringstream is(s);
      is >> res;
      if(!is&&(!is.eof())){
	ostringstream msg;
	msg << "AddCurveDialog::readDouble : ";
	msg << "could not read value from token '"+s+"'.\n";
	msg << "Error at line : " << l;
	throw(runtime_error(msg.str()));
      }
      return res;
    } // end of AddCurveDialog::readDouble

    std::vector<std::string>
    AddCurveDialog::tokenize(const std::string& s)
    {
      using namespace std;
      vector<string> res;
      string::size_type b = 0u;
      string::size_type e = s.find_first_of(':', b);
      while (string::npos != e || string::npos != b){
	// Found a token, add it to the vector.
	res.push_back(s.substr(b, e - b));
	b = s.find_first_not_of(':', e);
	e = s.find_first_of(':', b);
      }
      return res;
    } // end of AddCurveDialog::tokenize

    bool
    AddCurveDialog::isInteger(const std::string& s)
    {
      using namespace std;
      string::const_iterator p;
      for(p=s.begin();p!=s.end();++p){
	if(!isdigit(*p)){
	  return false;
	}
      }
      return true;
    } // end of AddCurveDialog::isInteger

    unsigned short
    AddCurveDialog::convertToUnsignedShort(const std::string& s)
    {
      using namespace std;
      istringstream converter(s);
      unsigned short i;
      converter >> i;
      if(!converter&&(!converter.eof())){
	string msg("AddCurveDialog::convertToUnsignedShort : ");
	msg += "can't convert string '"+s+"' in unsigned short";
	throw(runtime_error(msg));
      }
      return i;
    } // end of AddCurveDialog::convertToUnsignedShort

    AddCurveDialog::AddCurveDialog()
      : table(5,2),
	etable(2,2),
	dtable(3,2),
	slAlign(0,0.5,0.,0.),
	samplesAlign(0.5,0.5,1.,0.),
	abscissaAlign(0.5,0.5,1.,0.),
	columnAlign(0.5,0.5,1.,0.),
	alAlign(0,0.5,0.,0.),
	clAlign(0,0.5,0.,0.),
	elAlign(0,0.5,0.,0.),
	dlAlign(0,0.5,0.,0.),
	nlAlign(0,0.5,0.,0.),
	sAlign(0.5,0.5,1.,0.),
	samplesLAlign(0,0.5,0.,0.),
	abscissaLAlign(0,0.5,0.,0.),
	columnLAlign(0,0.5,0.,0.),
	aAlign(0.5,0.5,1.,0.),
	cAlign(0.5,0.5,1.,0.),
	eAlign(0.5,0.5,1.,0.),
	nAlign(0.5,0.5,1.,0.),
	fButton(_("Choose a data file")),
	lEntry(_("Function definition : ")),
	lSamples(_("Number of samples : ")),
	lAbscissa(_("Abscissa column number :")),
	lColumn(_("Data column number :")),
	ldEntry(_("Choose a data file : ")),
	lName(_("Curve name : ")),
	colorButton(Gdk::Color("red")),
	lColor(_("Choose curve color : ")),
	lStyle(_("Choose curve style : ")),
	lAxis(_("Choose curve axis : ")),
	color(Gdk::Color("red"))
    {
      using namespace Gtk;
      this->set_title(_("Add a new curve"));
      this->colorButton.set_title(_("Choose curve color"));
      this->lEntry.set_alignment(Gtk::ALIGN_LEFT,Gtk::ALIGN_TOP);
      this->lSamples.set_alignment(Gtk::ALIGN_LEFT,Gtk::ALIGN_TOP);
      this->lAbscissa.set_alignment(Gtk::ALIGN_LEFT,Gtk::ALIGN_TOP);
      this->lColumn.set_alignment(Gtk::ALIGN_LEFT,Gtk::ALIGN_TOP);
      this->lName.set_alignment(Gtk::ALIGN_LEFT,Gtk::ALIGN_TOP);
      this->lColor.set_alignment(Gtk::ALIGN_LEFT,Gtk::ALIGN_TOP);
      this->lStyle.set_alignment(Gtk::ALIGN_LEFT,Gtk::ALIGN_TOP);
      this->lAxis.set_alignment(Gtk::ALIGN_LEFT,Gtk::ALIGN_TOP);
      this->cStyle.append_text(_("lines"));
      this->cStyle.append_text(_("dots"));
      this->cStyle.append_text(_("squares"));
      this->cStyle.append_text(_("triangles"));
      this->cStyle.append_text(_("diamonds"));
      this->cStyle.append_text(_("crosses"));
      this->cStyle.set_active_text(_("lines"));
      this->cAxis.append_text(_("XY"));
      this->cAxis.append_text(_("XY2"));
      this->cAxis.append_text(_("X2Y"));
      this->cAxis.append_text(_("X2Y2"));
      this->cAxis.set_active_text(_("XY"));
      this->abscissa.set_text("1");
      this->column.set_text("2");
      this->slAlign.add(this->lStyle);
      this->samplesLAlign.add(this->lSamples);
      this->abscissaLAlign.add(this->lAbscissa);
      this->columnLAlign.add(this->lColumn);
      this->alAlign.add(this->lAxis);
      this->clAlign.add(this->lColor);
      this->elAlign.add(this->lEntry);
      this->dlAlign.add(this->ldEntry);
      this->nlAlign.add(this->lName);
      this->sAlign.add(this->cStyle);
      this->samplesAlign.add(this->samples);
      this->abscissaAlign.add(this->abscissa);
      this->columnAlign.add(this->column);
      this->aAlign.add(this->cAxis);
      this->cAlign.add(this->colorButton);
      this->eAlign.add(this->entry);
      this->nAlign.add(this->cName);
      this->etable.attach(this->elAlign,0,1,0,1);
      this->etable.attach(this->eAlign,1,2,0,1);
      this->etable.attach(this->samplesLAlign,0,1,1,2);
      this->etable.attach(this->samplesAlign,1,2,1,2);
      this->dtable.attach(this->dlAlign,0,1,0,1);
      this->dtable.attach(this->fButton,1,2,0,1);
      this->dtable.attach(this->abscissaLAlign,0,1,1,2);
      this->dtable.attach(this->abscissaAlign,1,2,1,2);
      this->dtable.attach(this->columnLAlign,0,1,2,3);
      this->dtable.attach(this->columnAlign,1,2,2,3);
      this->pages.append_page(this->dtable,_("Data Curve"));
      this->pages.append_page(this->etable,_("Evaluated Curve"));
      this->table.attach(this->pages,0,2,0,1);
      this->table.attach(this->nlAlign,0,1,1,2);
      this->table.attach(this->nAlign,1,2,1,2);
      this->table.attach(this->clAlign,0,1,2,3);
      this->table.attach(this->cAlign,1,2,2,3);
      this->table.attach(this->slAlign,0,1,3,4);
      this->table.attach(this->sAlign,1,2,3,4);
      this->table.attach(this->alAlign,0,1,4,5);
      this->table.attach(this->aAlign,1,2,4,5);
      this->get_vbox()->pack_start(this->table,Gtk::PACK_SHRINK);
      this->add_button(Stock::CANCEL, RESPONSE_CANCEL);
      this->add_button(Stock::ADD,RESPONSE_APPLY);
      this->add_button(Stock::OK,RESPONSE_OK);
      this->set_default_response(RESPONSE_OK);
      this->show_all();
    } // end of AddCurveDialog::AddCurveDialog

    void
    AddCurveDialog::applyCurveSettings(Curve *const pc)
    {
      using namespace std;
      Curve::Color c;
      Glib::ustring style = this->cStyle.get_active_text();
      c.r = this->colorButton.get_color().get_red()/static_cast<double>(numeric_limits<gushort>::max());
      c.g = this->colorButton.get_color().get_green()/static_cast<double>(numeric_limits<gushort>::max());
      c.b = this->colorButton.get_color().get_blue()/static_cast<double>(numeric_limits<gushort>::max());
      pc->setColor(c);
      if(style==_("dots")){
	pc->setStyle(Curve::DOT);
      } else if(style==_("squares")){
	pc->setStyle(Curve::SQUARE);
      } else if(style==_("triangles")){
	pc->setStyle(Curve::TRIANGLE);
      } else if(style==_("diamonds")){
	pc->setStyle(Curve::DIAMOND);
      } else if(style==_("crosses")){
	pc->setStyle(Curve::CROSS);
      } else {
	pc->setStyle(Curve::LINE);
      }
    } // end of AddCurveDialog::applyCurveSettings

    std::vector<tfel::utilities::SmartPtr<Curve> >
    AddCurveDialog::getEvaluatedCurve(void)
    {
      using namespace std;
      using namespace Glib;
      using namespace Gtk;
      using namespace tfel::utilities;
      using namespace tfel::math;
      using namespace Gtk;
      vector<tfel::utilities::SmartPtr<Curve> > res;
      if(this->entry.get_text().empty()){
	res.push_back(SmartPtr<Curve>(0));
	return res;
      }
      EvaluatedCurve *pc;
      try{
	SmartPtr<Evaluator> ev(new Evaluator(this->entry.get_text()));
	pc = new EvaluatedCurve(ev,"x");
      }
      catch(runtime_error& e){
	ustring msg("AddCurveDialog::getEvaluatedCurve : ");
	msg += "error while evaluating "+this->entry.get_text();
	MessageDialog dialog(*this,msg,false,MESSAGE_WARNING);
	msg += " (";
	msg += e.what();
	msg += ")";
	dialog.set_secondary_text(e.what());
	clog << msg << endl;
	dialog.run();
	res.push_back(SmartPtr<Curve>(0));
	return res;
      }
      this->applyCurveSettings(pc);
      if(!this->samples.get_text().empty()){
	try{
	  unsigned short i = AddCurveDialog::convertToUnsignedShort(this->samples.get_text());
	  if((i!=0)&&(i!=1)){
	    pc->setNumberOfSamples(i);
	  } else {
	    clog << "AddCurveDialog::getEvaluatedCurve : "
	      "number of samples must be greater than 2, using default\n";
	  }
	} 
	catch(runtime_error& e) {
	  clog << "AddCurveDialog::getEvaluatedCurve : "
	          "invalid number of samples ("
	       << e.what() << "), using default\n";
	}
      }
      if(this->cName.get_text().empty()){
	pc->setLegend(this->entry.get_text());
      } else {
	pc->setLegend(this->cName.get_text());
      }
      res.push_back(SmartPtr<Curve>(pc));
      return res;
    } // end of AddCurveDialog::getEvaluatedCurve


    std::string
    AddCurveDialog::getData(std::vector<double>& v,
			    const tfel::utilities::TextData& data,
			    const std::string& s)
    {
      using namespace std;
      using namespace tfel::math;
      using namespace tfel::utilities;
      if(AddCurveDialog::isInteger(s)){
	unsigned short c = AddCurveDialog::convertToUnsignedShort(s);
	data.getColumn(c,v);
	return data.getLegend(c);
      } else {
	// assuming a function
	vector<pair<string,unsigned short> > vars;
	Evaluator e(s);
	const vector<string>& vnames = e.getVariablesNames();
	if(vnames.empty()){
	  string msg("Analyser::getData : ");
	  msg += "function '"+s+"' does not declare any variable";
	  throw(runtime_error(msg));
	}
	vector<string>::const_iterator p;
	vector<TextData::Line>::const_iterator p2;
	vector<pair<string,unsigned short> >::const_iterator p3;
	for(p=vnames.begin();p!=vnames.end();++p){
	  if(((*p)[0]!='$')||
	     !(AddCurveDialog::isInteger(p->substr(1)))){
	    string msg("AddCurveDialog::getData : ");
	    msg += "invalid variable name '"+*p;
	    msg += "' in function '"+s+"'";
	    throw(runtime_error(msg));
	  }
	  const unsigned short vc = AddCurveDialog::convertToUnsignedShort(p->substr(1));
	  if(vc==0){
	    string msg("AddCurveDialog::getData : ");
	    msg += "invalid variable name "+*p;
	    msg += " in function '"+s+"'.";
	    throw(runtime_error(msg));
	  }
	  vars.push_back(make_pair(*p,vc));
	}
	for(p2=data.begin();p2!=data.end();++p2){
	  for(p3=vars.begin();p3!=vars.end();++p3){
	    if(p2->tokens.size()<p3->second){
	      ostringstream msg;
	      msg << "TextData::getColumn : line '" 
		  << p2->nbr << "' "
		  << "does not have '" << p3->second << "' columns.";
	      throw(runtime_error(msg.str()));
	    }
	    e.setVariableValue(p3->first,
			       AddCurveDialog::readDouble(p2->tokens[p3->second-1],
							  p2->nbr));
	  }
	  v.push_back(e.getValue());
	}
      }
      return "";
    } // end of AddCurveDialog::getData

    tfel::utilities::SmartPtr<Curve>
    AddCurveDialog::getDataCurve(const tfel::utilities::TextData& data,
				 const std::string& sa,
				 const std::string& sc)
    {
      using namespace std;
      using namespace Glib;
      using namespace Gtk;
      using namespace tfel::utilities;
      std::string legend;
      DataCurve * pc = 0;
      try{
	pc = new DataCurve;
	vector<double> vc;
	this->getData(pc->getAbscissa(),
		      data,sa);
	legend = this->getData(pc->getValues(),data,sc);
      }
      catch(runtime_error& e){
	ustring msg("AddCurveDialog::getDataCurve : ");
	msg += "error while reading file "+this->fButton.get_filename();
	MessageDialog dialog(*this,msg,false,MESSAGE_WARNING);
	msg += " (";
	msg += e.what();
	msg += ")";
	dialog.set_secondary_text(e.what());
	clog << msg << endl;
	dialog.run();
	delete pc;
	return SmartPtr<Curve>(0);
      }
      if(this->cName.get_text().empty()){
	if(legend.empty()){
	  ostringstream name;
	  name << fButton.get_filename() << " using " << sa << ":"  << sc;
	  pc->setLegend(name.str());
	} else {
	  pc->setLegend(legend);
	}
      } else {
	pc->setLegend(this->cName.get_text());
      }
      this->applyCurveSettings(pc);
      return SmartPtr<Curve>(pc);
    } // end of AddCurveDialog::getSimpleDataCurve(void)

    std::vector<tfel::utilities::SmartPtr<Curve> >
    AddCurveDialog::getDataCurve(void)
    {
      using namespace std;
      using namespace Glib;
      using namespace Gtk;
      using namespace tfel::utilities;
      vector<tfel::utilities::SmartPtr<Curve> > res;
      vector<tfel::utilities::SmartPtr<Curve> >::const_iterator p2;
      ustring a = this->abscissa.get_text();
      ustring c = this->column.get_text();
      ustring f = this->fButton.get_filename();
      vector<string>::const_iterator p;
      if(f.empty()){
	res.push_back(SmartPtr<Curve>(0));
	return res;
      }
      if(a.empty()){
	ustring msg("AddCurveDialog::getDataCurve : ");
	msg += "no abscissa value given.";
	MessageDialog dialog(*this,msg,false,MESSAGE_WARNING);
	clog << msg << endl;
	dialog.run();
	res.push_back(SmartPtr<Curve>(0));
	return res;
      }
      if(c.empty()){
	ustring msg("AddCurveDialog::getDataCurve : ");
	msg += "no column value given.";
	MessageDialog dialog(*this,msg,false,MESSAGE_WARNING);
	clog << msg << endl;
	dialog.run();
	res.push_back(SmartPtr<Curve>(0));
	return res;
      }
      const vector<string>& tc = AddCurveDialog::tokenize(c);
      try{
	TextData data(this->fButton.get_filename());
	for(p=tc.begin();p!=tc.end();++p){
	  res.push_back(this->getDataCurve(data,a,*p));
	}
      }
      catch(runtime_error& e){
	res.clear();
	ustring msg("AddCurveDialog::getDataCurve : ");
	msg += "can't open file "+this->fButton.get_filename();
	MessageDialog dialog(*this,msg,false,MESSAGE_WARNING);
	msg += " (";
	msg += e.what();
	msg += ")";
	dialog.set_secondary_text(e.what());
	clog << msg << endl;
	dialog.run();
	res.push_back(SmartPtr<Curve>(0));
	return res;
      }
      return res;
    } // end of AddCurveDialog::getDataCurve

    void
    AddCurveDialog::setDefaultColor(const Curve::Color& c)
    {
      using namespace std;
      this->color.set_red(static_cast<gushort>(c.r*numeric_limits<gushort>::max()));
      this->color.set_green(static_cast<gushort>(c.g*numeric_limits<gushort>::max()));
      this->color.set_blue(static_cast<gushort>(c.b*numeric_limits<gushort>::max()));
      this->colorButton.set_color(this->color);
    } // end of AddCurveDialog::setDefaultColor

    std::vector<tfel::utilities::SmartPtr<Curve> >
    AddCurveDialog::getCurve(void)
    {
      if(this->pages.get_current_page()==1){
	return this->getEvaluatedCurve();
      }
      return this->getDataCurve();
    } // end of AddCurveDialog::getCurve

    Graph::GraphAxis
    AddCurveDialog::getAxis(void)
    {
      Glib::ustring axis  = this->cAxis.get_active_text();
      if(axis==_("XY")){
	return Graph::XY;
      } else if(axis==_("XY2")){
	return Graph::XY2;
      } else if(axis==_("X2Y")){
	return Graph::X2Y;
      }
      return Graph::X2Y2;
    } // end of AddCurveDialog::getAxis

  } // end of namespace graphics

} // end of namespace tfel

