/*!
 * \file   plot.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   21 jan 2008
 */

#include<glibmm/i18n.h>
#include<gtkmm.h>

#include"TFEL/Utilities/TextData.hxx"
#include"TFEL/Utilities/ArgumentParserBase.hxx"
#include"TFEL/Graphics/DataCurve.hxx"
#include"TFEL/Graphics/ThemeManager.hxx"
#include"TFEL/Graphics/PlotWindow.hxx"

namespace tfel
{

  namespace graphics
  {

    struct TPlot
      : public PlotWindow,
	public tfel::utilities::ArgumentParserBase<TPlot>
    {
      /*!
       * \param const bool, stand alone mode
       */
      TPlot(const int,
	    const char * const * const);
      void
      initialize(void);
      ~TPlot();
    protected:

      struct Data
      {
	std::vector<unsigned short> yvalues;
	std::string fileName;
	std::string title;
	Curve::Style style;
	Curve::Color color;
	unsigned short xvalues;
	Graph::GraphAxis axis;
	bool hasColor;
      }; // end of struct Data

      static const std::vector<std::string>
      tokenize(const std::string&,const char);
	
      void registerArgumentCallBacks(void);

      void
      treatUnknownArgument(void);

      void treatTheme(void);

      void treatXY(void);

      void treatX2Y(void);

      void treatXY2(void);

      void treatX2Y2(void);

      void treatOutput(void);

      void treatColor(void);

      void treatUsing(void);

      void treatWithGrid(void);

      void treatXMin(void);

      void treatXMax(void);

      void treatYMin(void);

      void treatYMax(void);

      void treatTitle(void);

      void treatUpperTitle(void);

      void treatDownTitle(void);

      void treatLeftTitle(void);

      void treatRightTitle(void);

      void treatUpperLabel(void);

      void treatDownLabel(void);

      void treatLeftLabel(void);

      void treatRightLabel(void);

      void treatWithLines(void);

      void treatWithDiamonds(void);

      void treatWithCrosses(void);

      void treatWithSquares(void);

      void treatWithTriangles(void);

      void treatWithDots(void);

      void setRanges();

      void getStringFromArgs(std::string&,
			     const std::string&,
			     const std::string&);

      std::string
      getVersionDescription(void) const;
  
      std::string
      getUsageDescription(void) const;

      friend class
      tfel::utilities::ArgumentParserBase<TPlot>;

      std::map<std::string,tfel::utilities::SmartPtr<tfel::utilities::TextData> > dataSources;
      std::vector<Data> data;
      std::string output;
      std::string upperTitle;
      std::string downTitle;
      std::string rightTitle;
      std::string leftTitle;
      std::string upperLabel;
      std::string downLabel;
      std::string rightLabel;
      std::string leftLabel;
      double xmin;
      double xmax;
      double ymin;
      double ymax;
      double x2min;
      double x2max;
      double y2min;
      double y2max;
      bool hasGrid;
      bool hasXMinValue;
      bool hasYMinValue;
      bool hasXMaxValue;
      bool hasYMaxValue;
      bool hasX2MinValue;
      bool hasY2MinValue;
      bool hasX2MaxValue;
      bool hasY2MaxValue;
      bool fullScreenMode;
    }; // end of struct TPlots

    TPlot::TPlot(const int argc,
		 const char * const * const argv)
      : tfel::utilities::ArgumentParserBase<TPlot>(argc,argv),
	hasGrid(false),
	hasXMinValue(false),
	hasYMinValue(false),
	hasXMaxValue(false),
	hasYMaxValue(false),
	hasX2MinValue(false),
	hasY2MinValue(false),
	hasX2MaxValue(false),
	hasY2MaxValue(false),
	fullScreenMode(false)
    {
      using namespace std;
      using namespace Gtk;
      using namespace Glib;
      using namespace tfel::utilities;
      this->registerArgumentCallBacks();
      this->parseArguments();    
      this->initialize();
      if(!this->output.empty()){
	string extension;
	string::size_type pos = this->output.find_last_of('.');
	if(pos==string::npos){
	  string msg("TPlot::PlotWindow : ");
	  msg += "invalid output name " + this->output;
	  throw(runtime_error(msg));
	}
	extension = this->output.substr(pos+1);
	if(extension=="eps"){
	  this->area.exportToPostScript(this->output);
	} else if(extension=="pdf"){
	  this->area.exportToPDF(this->output);
	  } else if(extension=="svg"){
	  this->area.exportToSVG(this->output);
	  } else if(extension=="png"){
	  this->area.exportToPNG(this->output);
	} else {
	  string msg("TPlot::PlotWindow : ");
	  msg += "invalid output name " + this->output;
	  msg += " (unsupported extension "+extension+")";
	    throw(runtime_error(msg));
	}
	exit(EXIT_SUCCESS);
      }
    }

    const std::vector<std::string>
    TPlot::tokenize(const std::string& s,
			 const char c)
    {
      using namespace std;
      vector<string> res;
      string::size_type b = 0u;
      string::size_type e = s.find_first_of(c, b);
      while (string::npos != e || string::npos != b){
	// Found a token, add it to the vector.
	res.push_back(s.substr(b, e - b));
	b = s.find_first_not_of(c, e);
	e = s.find_first_of(c, b);
      }
      return res;
    } // end of TPlot::tokenize

    TPlot::~TPlot()
    {} // end of TPlot::PlotWindow

    void
    TPlot::registerArgumentCallBacks(void)
    {
      this->registerNewCallBack("--xy","-xy",&TPlot::treatXY,
				"specify axis used for drawing");
      this->registerNewCallBack("--x2y","-x2y",&TPlot::treatX2Y,
				"specify axis used for drawing");
      this->registerNewCallBack("--xy2","-xy2",&TPlot::treatXY2,
				"specify axis used for drawing");
      this->registerNewCallBack("--x2y2","-x2y2",&TPlot::treatX2Y2,
				"specify axis used for drawing");
      this->registerNewCallBack("--using","-u",&TPlot::treatUsing,
				"specify columns used for drawing");
      this->registerNewCallBack("--output","-o",&TPlot::treatOutput,
				"specify an output file");
      this->registerNewCallBack("--color","-c",&TPlot::treatColor,
				"specify color used for drawing the previous curve");
      this->registerNewCallBack("--with-grid",&TPlot::treatWithGrid,
				"add grid");
      this->registerNewCallBack("--xmin",&TPlot::treatXMin,
				"set minimal x value");
      this->registerNewCallBack("--xmax",&TPlot::treatXMax,
				"set maximal x value");
      this->registerNewCallBack("--ymin",&TPlot::treatYMin,
				"set minimal y value");
      this->registerNewCallBack("--ymax",&TPlot::treatYMax,
				"set maximal y value");
      this->registerNewCallBack("--theme",&TPlot::treatTheme,
				"set the graph theme");
      this->registerNewCallBack("--title","-t",&TPlot::treatTitle,
				"set title of the previous curve");
      this->registerNewCallBack("--upperTitle","-ut",&TPlot::treatUpperTitle,
				"set graph upper title");
      this->registerNewCallBack("--downTitle","-dt",&TPlot::treatDownTitle,
				"set graph down title");
      this->registerNewCallBack("--leftTitle","-lt",&TPlot::treatLeftTitle,
				"set graph left title");
      this->registerNewCallBack("--rightTitle","-rt",&TPlot::treatRightTitle,
				"set graph right title");
      this->registerNewCallBack("--x2title",&TPlot::treatUpperTitle,
				"set graph upper title");
      this->registerNewCallBack("--xtitle",&TPlot::treatDownTitle,
				"set graph down title");
      this->registerNewCallBack("--ytitle",&TPlot::treatLeftTitle,
				"set graph left title");
      this->registerNewCallBack("--y2title",&TPlot::treatRightTitle,
				"set graph right title");
      this->registerNewCallBack("--upperLabel","-ul",&TPlot::treatUpperLabel,
				"set graph upper label");
      this->registerNewCallBack("--downLabel","-dl",&TPlot::treatDownLabel,
				"set graph down label");
      this->registerNewCallBack("--leftLabel","-ll",&TPlot::treatLeftLabel,
				"set graph left label");
      this->registerNewCallBack("--rightLabel","-rl",&TPlot::treatRightLabel,
				"set graph right label");
      this->registerNewCallBack("--x2label",&TPlot::treatUpperLabel,
				"set graph upper label");
      this->registerNewCallBack("--xlabel",&TPlot::treatDownLabel,
				"set graph down label");
      this->registerNewCallBack("--ylabel",&TPlot::treatLeftLabel,
				"set graph left label");
      this->registerNewCallBack("--y2label",&TPlot::treatRightLabel,
				"set graph right label");
      this->registerNewCallBack("--with-lines",&TPlot::treatWithLines,
				"draw the preceeding curve with lines");
      this->registerNewCallBack("--with-dots",&TPlot::treatWithDots,
				"draw the preceeding curve with dots");
      this->registerNewCallBack("--with-squares",&TPlot::treatWithSquares,
				"draw the preceeding curve with squares");
      this->registerNewCallBack("--with-triangles",&TPlot::treatWithTriangles,
				"draw the preceeding curve with triangles");
      this->registerNewCallBack("--with-diamonds",&TPlot::treatWithDiamonds,
				"draw the preceeding curve with diamonds");
      this->registerNewCallBack("--with-crosses",&TPlot::treatWithCrosses,
				"draw the preceeding curve with crosses");
    } // end of TPlot::registerArgumentCallBacks

    void
    TPlot::getStringFromArgs(std::string& w,
				  const std::string& method,
				  const std::string& key)
    {
      using namespace std;
      if(!w.empty()){
	string msg("TPlot::"+method+" : ");
	msg += "key "+key+" has already been declared";
	throw(runtime_error(msg));
      }
      if(this->currentArgument==this->args.end()){
	string msg("TPlot::"+method+" : ");
	msg += "no argument given to the "+key+"  declaration";
	throw(runtime_error(msg));
      }
      w = *(this->currentArgument);
      ++(this->currentArgument);
    } // end of TPlot::getStringFromArgs

    void
    TPlot::treatOutput(void)
    {
      this->getStringFromArgs(this->output,"treatOutput","using");
    } // end of TPlot::treatOutput

    void
    TPlot::treatTheme(void)
    {
      using namespace std;
      if(this->currentArgument==this->args.end()){
	string msg("TPlot::treatTheme : ");
	msg += "no argument given to the theme declaration";
	throw(runtime_error(msg));
      }
      this->area.setTheme(ThemeManager::getThemeManager().getTheme(*(this->currentArgument)));
      ++(this->currentArgument);
    } // end of TPlot::treatTheme

    void
    TPlot::treatTitle(void)
    {
      using namespace std;
      if(this->currentArgument==this->args.end()){
	string msg("TPlot::treatTitle : ");
	msg += "no argument given to the title declaration";
	throw(runtime_error(msg));
      }
      this->data.back().title = *(this->currentArgument);
      ++(this->currentArgument);
    } // end of TPlot::treatTitle

    void
    TPlot::treatWithLines(void)
    {
      using namespace std;
      if(this->data.empty()){
	string msg("TPlot::treatWithLines : ");
	msg += "no curve declared";
	throw(runtime_error(msg));
      }
      this->data.back().style = Curve::LINE;
    } // end of TPlot::treatWithLines

    void
    TPlot::treatWithCrosses(void)
    {
      using namespace std;
      if(this->data.empty()){
	string msg("TPlot::treatWithCrosses : ");
	msg += "no curve declared";
	throw(runtime_error(msg));
      }
      this->data.back().style = Curve::CROSS;
    } // end of TPlot::treatWithCrosses

    void
    TPlot::treatWithDiamonds(void)
    {
      using namespace std;
      if(this->data.empty()){
	string msg("TPlot::treatWithDiamonds : ");
	msg += "no curve declared";
	throw(runtime_error(msg));
      }
      this->data.back().style = Curve::DIAMOND;
    } // end of TPlot::treatWithDiamonds

    void
    TPlot::treatWithSquares(void)
    {
      using namespace std;
      if(this->data.empty()){
	string msg("TPlot::treatWithSquares : ");
	msg += "no curve declared";
	throw(runtime_error(msg));
      }
      this->data.back().style = Curve::SQUARE;
    } // end of TPlot::treatWithSquares

    void
    TPlot::treatWithTriangles(void)
    {
      using namespace std;
      if(this->data.empty()){
	string msg("TPlot::treatWithTriangles : ");
	msg += "no curve declared";
	throw(runtime_error(msg));
      }
      this->data.back().style = Curve::TRIANGLE;
    } // end of TPlot::treatWithTriangles

    void
    TPlot::treatWithDots(void)
    {
      using namespace std;
      if(this->data.empty()){
	string msg("TPlot::treatWithDots : ");
	msg += "no curve declared";
	throw(runtime_error(msg));
      }
      this->data.back().style = Curve::DOT;
    } // end of TPlot::treatWithDots

    void
    TPlot::treatXY(void)
    {
      using namespace std;
      if(this->data.empty()){
	string msg("TPlot::treatXY : ");
	msg += "no curve specified";
	throw(runtime_error(msg));
      }
      this->data.back().axis = Graph::XY;
    } // end of TPlot::treatXY
    
    void
    TPlot::treatX2Y(void)
    {
      using namespace std;
      if(this->data.empty()){
	string msg("TPlot::treatX2Y : ");
	msg += "no curve specified";
	throw(runtime_error(msg));
      }
      this->data.back().axis = Graph::X2Y;
    } // end of TPlot::treatX2Y
    
    void
    TPlot::treatXY2(void)
    {
      using namespace std;
      if(this->data.empty()){
	string msg("TPlot::treatXY2 : ");
	msg += "no curve specified";
	throw(runtime_error(msg));
      }
      this->data.back().axis = Graph::XY2;
    } // end of TPlot::treatX2Y
    
    void
    TPlot::treatX2Y2(void)
    {
      using namespace std;
      if(this->data.empty()){
	string msg("TPlot::treatX2Y2 : ");
	msg += "no curve specified";
	throw(runtime_error(msg));
      }
      this->data.back().axis = Graph::X2Y2;
    } // end of TPlot::treatX2Y
    
    void
    TPlot::treatUpperTitle(void)
    {
      this->getStringFromArgs(this->upperTitle,"treatUpperTitle","upperTitle");
    } // end of TPlot::treatUpperTitle

    void
    TPlot::treatDownTitle(void)
    {
      this->getStringFromArgs(this->downTitle,"treatDownTitle","downTitle");
    } // end of TPlot::treatDownTitle

    void
    TPlot::treatLeftTitle(void)
    {
      this->getStringFromArgs(this->leftTitle,"treatLeftTitle","leftTitle");
    } // end of TPlot::treatLeftTitle

    void
    TPlot::treatRightTitle(void)
    {
      this->getStringFromArgs(this->rightTitle,"treatRightTitle","rightTitle");
    } // end of TPlot::treatRightTitle

    void
    TPlot::treatUpperLabel(void)
    {
      this->getStringFromArgs(this->upperLabel,"treatUpperLabel","upperLabel");
    } // end of TPlot::treatUpperLabel

    void
    TPlot::treatDownLabel(void)
    {
      this->getStringFromArgs(this->downLabel,"treatDownLabel","downLabel");
    } // end of TPlot::treatDownLabel

    void
    TPlot::treatLeftLabel(void)
    {
      this->getStringFromArgs(this->leftLabel,"treatLeftLabel","leftLabel");
    } // end of TPlot::treatLeftLabel

    void
    TPlot::treatRightLabel(void)
    {
      this->getStringFromArgs(this->rightLabel,"treatRightLabel","rightLabel");
    } // end of TPlot::treatRightLabel

    void
    TPlot::treatWithGrid(void)
    {
      this->hasGrid = true;
    } // end of TPlot::treatWithGrid

    void
    TPlot::treatColor(void)
    {
      using namespace std;
      if(this->data.empty()){
	string msg("TPlot::treatColor : ");
	msg += "no curve specified";
	throw(runtime_error(msg));
      }
      if(this->currentArgument==this->args.end()){
	string msg("TPlot::treatColor : ");
	msg += "no argument given to the color declaration";
	throw(runtime_error(msg));
      }
      if(*(this->currentArgument)=="red"){
	this->data.back().hasColor = true;
	this->data.back().color.r = 1.;
	this->data.back().color.g = 0.;
	this->data.back().color.b = 0.;
      } else if(*(this->currentArgument)=="green"){
	this->data.back().hasColor = true;
	this->data.back().color.r = 0.;
	this->data.back().color.g = 1.;
	this->data.back().color.b = 0.;
      } else if(*(this->currentArgument)=="blue"){
	this->data.back().hasColor = true;
	this->data.back().color.r = 0.;
	this->data.back().color.g = 0.;
	this->data.back().color.b = 1.;
      } else if(*(this->currentArgument)=="white"){
	this->data.back().hasColor = true;
	this->data.back().color.r = 1.;
	this->data.back().color.g = 1.;
	this->data.back().color.b = 1.;
      } else if(*(this->currentArgument)=="black"){
	this->data.back().hasColor = true;
	this->data.back().color.r = 0.;
	this->data.back().color.g = 0.;
	this->data.back().color.b = 0.;
      } else if(*(this->currentArgument)=="pink"){
	this->data.back().hasColor = true;
	this->data.back().color.r = 1.;
	this->data.back().color.g = 0.75;
	this->data.back().color.b = 0.8;
      } else if(*(this->currentArgument)=="gold"){
	this->data.back().hasColor = true;
	this->data.back().color.r = 1.;
	this->data.back().color.g = 0.84;
	this->data.back().color.b = 0.;
      } else if(*(this->currentArgument)=="yellow"){
	this->data.back().hasColor = true;
	this->data.back().color.r = 1.;
	this->data.back().color.g = 1.;
	this->data.back().color.b = 0.;
      } else {
	string msg("TPlot::treatColor : ");
	msg += "unknown color "+*(this->currentArgument);
	throw(runtime_error(msg));
      }
      ++(this->currentArgument);
    } // end of TPlot::treatColor

    void
    TPlot::treatXMin(void)
    {
      using namespace std;
      istringstream converter;
      if(this->currentArgument==this->args.end()){
	string msg("TPlot::treatUsing : ");
	msg += "no argument given to the xmin declaration";
	throw(runtime_error(msg));
      }
      converter.str(*(this->currentArgument));
      converter >> this->xmin;
      this->hasXMinValue = true;
      ++(this->currentArgument);
    } // end of TPlot::treatXMin

    void
    TPlot::treatYMin(void)
    {
      using namespace std;
      istringstream converter;
      if(this->currentArgument==this->args.end()){
	string msg("TPlot::treatUsing : ");
	msg += "no argument given to the ymin declaration";
	throw(runtime_error(msg));
      }
      converter.str(*(this->currentArgument));
      converter >> this->ymin;
      this->hasYMinValue = true;
      ++(this->currentArgument);
    } // end of TPlot::treatYMin

    void
    TPlot::treatXMax(void)
    {
      using namespace std;
      istringstream converter;
      if(this->currentArgument==this->args.end()){
	string msg("TPlot::treatUsing : ");
	msg += "no argument given to the xmax declaration";
	throw(runtime_error(msg));
      }
      converter.str(*(this->currentArgument));
      converter >> this->xmax;
      this->hasXMaxValue = true;
      ++(this->currentArgument);
    } // end of TPlot::treatXMax

    void
    TPlot::treatYMax(void)
    {
      using namespace std;
      istringstream converter;
      if(this->currentArgument==this->args.end()){
	string msg("TPlot::treatUsing : ");
	msg += "no argument given to the ymax declaration";
	throw(runtime_error(msg));
      }
      converter.str(*(this->currentArgument));
      converter >> this->ymax;
      this->hasYMaxValue = true;
      ++(this->currentArgument);
    } // end of TPlot::treatYMax

    void
    TPlot::treatUsing(void)
    {
      using namespace std;
      vector<string>::const_iterator p;
      string::const_iterator p2;
      istringstream converter;
      if(this->data.empty()){
	string msg("TPlot::treatUsing : ");
	msg += "no curve specified";
	throw(runtime_error(msg));
      }
      if(this->currentArgument==this->args.end()){
	string msg("TPlot::treatUsing : ");
	msg += "no argument given to the using declaration";
	throw(runtime_error(msg));
      }
      const vector<string>& c = TPlot::tokenize(*(this->currentArgument),':');
      if(c.size()==1){
	string msg("TPlot::treatUsing : ");
	msg += "only one column specified";
	throw(runtime_error(msg));
      }
      p=c.begin();
      for(p2=p->begin();p2!=p->end();++p2){
	if(!isdigit(*p2)){
	  string msg("TPlot::treatUsing : ");
	  msg += "invalid column specification '"+*p+"'";
	  throw(runtime_error(msg));
	}
      }
      converter.str(*p);
      converter >> this->data.back().xvalues;
      this->data.back().yvalues.clear();
      ++p;
      for(;p!=c.end();++p){
	istringstream converter2;
	unsigned short column;
	for(p2=p->begin();p2!=p->end();++p2){
	  if(!isdigit(*p2)){
	    string msg("TPlot::treatUsing : ");
	    msg += "invalid column specification '"+*p+"'";
	    throw(runtime_error(msg));
	  }
	}
	converter2.str(*p);
	converter2 >> column;
	this->data.back().yvalues.push_back(column);
      }
      ++(this->currentArgument);
    } // end of TPlot::treatUsing

    void
    TPlot::treatUnknownArgument(void)
    {
      using namespace std;
      using namespace tfel::utilities;
      typedef std::map<string,SmartPtr<TextData> >::value_type MVType;
      if((*(this->currentArgument))[0]=='-'){
	string msg("TPlot::treatUnknownArgument : ");
	msg += "unknown argument " + *(this->currentArgument);
	throw(runtime_error(msg));
      }
      Data d;
      d.axis = Graph::XY;
      d.hasColor = false;
      d.fileName = *(this->currentArgument);
      d.style = Curve::LINE;
      d.xvalues = 1;
      d.yvalues.push_back(2);
      if(this->dataSources.find(d.fileName)==this->dataSources.end()){
	this->dataSources.insert(MVType(d.fileName,
					SmartPtr<TextData>(new TextData(d.fileName))));
      }
      this->data.push_back(d);
      ++(this->currentArgument);
    } // end of TPlot::treatUnknownArgument(void)

    std::string 
    TPlot::getVersionDescription(void) const
    {
      using namespace std;
      return "1.0";
    }

    std::string 
    TPlot::getUsageDescription(void) const
    {
      using namespace std;
      string usage("Usage : ");
      usage += this->programName;
      usage += " [options] [files]";
      return usage;
    }

    void
    TPlot::initialize(void)
    {
      using namespace std;
      using namespace Gtk;
      using namespace tfel::utilities;
      using namespace tfel::graphics;
  
      using std::map;
      
      vector<Data>::const_iterator p;
      map<string,SmartPtr<TextData> >::const_iterator p2;
      vector<unsigned short>::const_iterator p3;
      string extension;

      if(this->data.empty()){
	return;
      }

      if(!this->upperTitle.empty()){
	this->area.setUpperTitle(this->upperTitle);
      }

      if(!this->downTitle.empty()){
	this->area.setDownTitle(this->downTitle);
      }

      if(!this->leftTitle.empty()){
	this->area.setLeftTitle(this->leftTitle);
      }

      if(!this->rightTitle.empty()){
	this->area.setRightTitle(this->rightTitle);
      }

      if(!this->upperLabel.empty()){
	this->area.setX2Label(this->upperLabel);
      } else {
	if(this->data.size()==1u){
	  if(this->data.front().axis&Graph::x2axis){
	    p2 = this->dataSources.find(this->data.front().fileName);
	    assert(p2!=this->dataSources.end());
	    this->area.setX2Label(p2->second->getLegend(this->data.front().xvalues));
	  }
	}
      }

      if(!this->downLabel.empty()){
	this->area.setXLabel(this->downLabel);
      } else {
	if(this->data.size()==1u){
	  if(this->data.front().axis&Graph::xaxis){
	    p2 = this->dataSources.find(this->data.front().fileName);
	    assert(p2!=this->dataSources.end());
	    this->area.setXLabel(p2->second->getLegend(this->data.front().xvalues));
	  }
	}
      }

      if(!this->leftLabel.empty()){
	this->area.setYLabel(this->leftLabel);
      } else {
	if(this->data.size()==1u){
	  if(this->data.front().yvalues.size()==1u){
	    if(this->data.front().axis&Graph::yaxis){
	      p2 = this->dataSources.find(this->data.front().fileName);
	      this->area.setYLabel(p2->second->getLegend(this->data.front().yvalues.front()));
	    }
	  }
	}
      }

      if(!this->rightLabel.empty()){
	this->area.setY2Label(this->rightLabel);
      } else {
	if(this->data.size()==1u){
	  if(this->data.front().yvalues.size()==1u){
	    if(this->data.front().axis&Graph::y2axis){
	      p2 = this->dataSources.find(this->data.front().fileName);
	      this->area.setY2Label(p2->second->getLegend(this->data.front().yvalues.front()));
	    }
	  }
	}
      }

      if(this->hasGrid){
	this->area.showGrid(true);
      } else {
	this->area.showGrid(false);
      }

      for(p=this->data.begin();p!=this->data.end();++p){
	p2 = this->dataSources.find(p->fileName);
	assert(p2!=this->dataSources.end());
	for(p3=p->yvalues.begin();p3!=p->yvalues.end();++p3){
	  DataCurve * curve = new DataCurve(p2->second->getColumn(p->xvalues),
					    p2->second->getColumn(*p3));
	  curve->setStyle(p->style);
	  if(p->hasColor){
	    curve->setColor(p->color);
	  }
	  if(!p->title.empty()){
	    curve->setLegend(p->title);
	  } else {
	    if(!p2->second->getLegend(*p3).empty()){
	      curve->setLegend(p2->second->getLegend(*p3));
	    } else {
	      ostringstream legend;
	      legend << "\"" << p->fileName << "\" using " << p->xvalues << ":" << *p3;
	      curve->setLegend(legend.str());
	    }
	  }
	  this->area.addCurve(SmartPtr<Curve>(curve),p->axis);
	}
      }
      this->setRanges();
    } // end of TPlot::initialize

    void
    TPlot::setRanges()
    {
      using namespace std;
      if(this->hasXMinValue||this->hasXMaxValue){
	double amin = this->area.getXMin();
	double amax = this->area.getXMax();
	if(this->hasXMinValue){
	  amin = this->xmin;
	}
	if(this->hasXMaxValue){
	  amax = this->xmax;
	}
	this->area.setXRange(amin,amax);
      }
      if(this->hasYMinValue||this->hasYMaxValue){
	double amin = this->area.getYMin();
	double amax = this->area.getYMax();
	if(this->hasYMinValue){
	  amin = this->ymin;
	}
	if(this->hasYMaxValue){
	  amax = this->ymax;
	}
	this->area.setYRange(amin,amax);
      }
      if(this->hasX2MinValue||this->hasX2MaxValue){
	double amin = this->area.getX2Min();
	double amax = this->area.getX2Max();
	if(this->hasX2MinValue){
	  amin = this->x2min;
	}
	if(this->hasX2MaxValue){
	  amax = this->x2max;
	}
	this->area.setX2Range(amin,amax);
      }
      if(this->hasY2MinValue||this->hasY2MaxValue){
	double amin = this->area.getY2Min();
	double amax = this->area.getY2Max();
	if(this->hasY2MinValue){
	  amin = this->y2min;
	}
	if(this->hasY2MaxValue){
	  amax = this->y2max;
	}
	this->area.setY2Range(amin,amax);
      }
    } // end of TPlot::setRanges

  } // end of namespace graphics

} // end of namespace tfel


int
main(int argc,
     char** argv)
{
  using namespace tfel::graphics;
  setlocale(LC_ALL, "");
  bindtextdomain(PACKAGE, LOCALEDIR);
  textdomain(PACKAGE);
  Gtk::Main kit(argc, argv);
  TPlot tplot(argc,argv);
  //Shows the window and returns when it is closed.
  Gtk::Main::run(tplot);
  return EXIT_SUCCESS;
}
