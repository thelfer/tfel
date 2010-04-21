/*!
 * \file   Analyser.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   08 mar 2008
 */

#include<iostream>
#include<cstdlib>
#include<cstdio>
#include<stdexcept>
#include<algorithm>
#include<iterator>
#include<sstream>
#include<cerrno>
#include<csignal>
#include<algorithm>
#include<cctype>
#include<dlfcn.h>

#include<gtkmm.h>

#if defined(HAVE_READLINE_READLINE_H)
#include <readline/readline.h>
#elif defined(HAVE_READLINE_H)
#include <readline.h>
#endif /* !defined(HAVE_READLINE_H) */

#ifdef HAVE_READLINE_HISTORY
#if defined(HAVE_READLINE_HISTORY_H)
#include <readline/history.h>
#elif defined(HAVE_HISTORY_H)
#include <history.h>
#endif /* defined(HAVE_READLINE_HISTORY_H) */
#endif /* HAVE_READLINE_HISTORY */

#include"TFEL/System/System.hxx"
#include"TFEL/System/SignalManager.hxx"
#include"TFEL/System/ProcessManager.hxx"
#include"TFEL/System/ExternalLibraryManager.hxx"

#include"TFEL/Math/Parser/ExternalFunctionManager.hxx"
#include"TFEL/Math/Parser/ExternalCFunction.hxx"
#include"TFEL/Math/Parser/ExternalCastemFunction.hxx"

#ifdef HAVE_OCTAVE
#include"TFEL/Math/Parser/ExternalOctaveFunction.hxx"
#endif /* HAVE_OCTAVE */

#include"TFEL/Graphics/Curve.hxx"
#include"TFEL/Graphics/EvaluatedCurve.hxx"
#include"TFEL/Graphics/DataCurve.hxx"
#include"TFEL/Graphics/ThemeManager.hxx"
#include"TFEL/Graphics/Grid.hxx"

#include"TFEL/Graphics/Analyser.hxx"

#ifdef HAVE_GSL
#include"TFEL/Math/Parser/EvaluatorGSLWrapper.hxx"
#endif

namespace tfel
{  

  namespace graphics
  {

    struct Analyser::ImportOptions
    {
      enum ImportType{
	Function,
	Data,
	Default
      };
      enum FunctionType{
	C,
	Castem,
#ifdef HAVE_OCTAVE 
	Octave,
#endif /* HAVE_OCTAVE */
	FDefault
      };
      ImportOptions()
	: type(Default),
	  functionType(FDefault),
	  numberOfVariables(-1)
      {}
      ImportType type;
      FunctionType functionType;
      int numberOfVariables;
    }; // end of Analyser::ImportOptions

    struct Analyser::CurveOptions
    {
      CurveOptions()
	: style(Curve::LINE),
	  axis(Graph::XY),
	  hasSpecifiedColor(false),
	  hasSpecifiedWidth(false)
      {} // end of CurveOptions()
      void setColor(const Curve::Color c_)
      {
	this->color.r = c_.r;
	this->color.g = c_.g;
	this->color.b = c_.b;
      }
      std::string title;
      std::string using_decl_x;
      std::string using_decl_y;
      Curve::Style style;
      Graph::GraphAxis axis;
      Curve::Color color;
      unsigned short width;
      bool hasSpecifiedColor;
      bool hasSpecifiedWidth;
    }; // end of struct Analyser::CurveOptions

    Analyser::Analyser(int argc,
		       char **argv)
      : tfel::utilities::ArgumentParserBase<Analyser>(argc,argv),
	PlotWindow(),
	terminal("x11"),
	dummyVariable("x")
    {
      using namespace std;
      using namespace tfel::math;
      using namespace tfel::utilities;
      using namespace tfel::math::parser;
      typedef ExternalFunctionManager::value_type MVType;
      this->reset();
      this->registerCallBacks();
      this->declareDefaultColors();
      this->parseArguments();    
      this->treatCharAsString(true);
    } // end of Analyser::Analyser()

    void
    Analyser::declareDefaultColors(void)
    {
      this->declareColor("red",1,0,0);
      this->declareColor("green",0,1,0);
      this->declareColor("blue",0,0,1);
      this->declareColor("orange",1,.5,0);
      this->declareColor("violet",.5,0,.5);
      this->declareColor("purple",.75,0,.25);
      this->declareColor("brown",.75,.5,.25);
      this->declareColor("pink",1,.75,.75);
      this->declareColor("cyan",0,1,1);
      this->declareColor("magenta",1,0,1);
      this->declareColor("yellow",1,1,0);
      this->declareColor("olive",.5,.5,0);
      this->declareColor("black",0,0,0);
      this->declareColor("darkgray",.25,.25,.25);
      this->declareColor("gray",.5,.5,.5);
      this->declareColor("lightgray",.75,.75,.75);
      this->declareColor("white",1,1,1);
    } // end of Analyser::declareDefaultColors

    void
    Analyser::declareColor(const std::string& n,
			   const double r,
			   const double g,
			   const double b)
    {
      using namespace std;
      typedef std::map<string,Curve::Color>::value_type MVType;
      Curve::Color c;
      if((r<0)||(r>1)){
	string msg("Analyser::declareColor : invalid red value");
	throw(runtime_error(msg));
      }
      if((g<0)||(g>1)){
	string msg("Analyser::declareColor : invalid green value");
	throw(runtime_error(msg));
      }
      if((b<0)||(b>1)){
	string msg("Analyser::declareColor : invalid blue value");
	throw(runtime_error(msg));
      }
      c.r = r;
      c.g = g;
      c.b = b;
      if(!this->colors.insert(MVType(n,c)).second){
	string msg("Analyser::declareColor : ");
	msg += "color '"+n+"' already declared";
	throw(runtime_error(msg));
      }
    } // end of Analyser::declareColor

    void
    Analyser::reset(void)
    {
      this->hasGrid = false;
      this->hasXMinValue = false;
      this->hasYMinValue = false;
      this->hasXMaxValue = false;
      this->hasYMaxValue = false;
      this->hasX2MinValue = false;
      this->hasY2MinValue = false;
      this->hasX2MaxValue = false;
      this->hasY2MaxValue = false;
    } // end of Analyser::reset

    std::string 
    Analyser::getVersionDescription(void) const
    {
      using namespace std;
      return "1.0";
    } // end of Analyser::getVersionDescription(void) const

    std::string 
    Analyser::getUsageDescription(void) const
    {
      using namespace std;
      string usage("Usage : ");
      usage += this->programName;
      usage += " [options] [files]";
      return usage;
    } // end of Analyser::getUsageDescription(void) const

    void
    Analyser::treatLine()
    {
      using namespace std;
      using namespace tfel::utilities;
      using namespace tfel::system;
      if(this->currentLine[0]=='!'){
	ProcessManager m;
	string::iterator p = this->currentLine.begin();
	++p;
	string cmd(p,this->currentLine.end());
	m.execute(cmd);
      } else {
	if(!this->currentLine.empty()){
	  this->clear();
	  this->splitLine(this->currentLine,this->currentLineNbr);
	  if(this->cStyleCommentOpened){
	    string msg("Analyser<Child>::treatLine : ");
	    msg += "unfinished c-style comment";
	    throw(runtime_error(msg));
	  }
	  this->splitTokens();
	  CxxTokenizer::stripComments();
	  this->analyse(this->begin(),
			this->end());
	}
      }
      this->currentLine.clear();
    } // end of treatLine

    void
    Analyser::treatUnknownArgument(void)
    {
      using namespace std;
      using namespace tfel::utilities;
      typedef std::map<string,shared_ptr<TextData> >::value_type MVType;
      if((*(this->currentArgument))[0]=='-'){
	string msg("Analyser::treatUnknownArgument : ");
	msg += "unknown argument " + *(this->currentArgument);
	throw(runtime_error(msg));
      }
      ++(this->currentArgument);
    } // end of PlotWindow::treatUnknownArgument(void)

    void
    Analyser::treatDummy(TokensContainer::const_iterator& p, 
			 const TokensContainer::const_iterator pe)
    {
      using namespace std;
      this->checkNotEndOfLine("Analyser::treatDummy",
			      "expected dummy variable name",p,pe);
      if(!this->isValidIdentifier(p->value)){
	string msg("Analyser::treatDummy : '");
	msg += p->value+"' is not a valid identifer.";
	throw(runtime_error(msg));
      }
      this->dummyVariable = p->value;
      ++p;
    } // end of Analyser::treatDummy

    void
    Analyser::registerCallBack(std::map<std::string,Analyser::MemFuncPtr>& m,
			       const std::string& name,
			       const Analyser::MemFuncPtr f)
    {
      using namespace std;
      typedef std::map<string,MemFuncPtr>::value_type MVType;
      if(!m.insert(MVType(name,f)).second){
	string msg("Analyser::registerCallBack : ");
	msg += "key '"+name+"' already declared";
	throw(runtime_error(msg));
      }      
    } // end of Analyser::registerCallBack

    void
    Analyser::registerCallBack2(std::map<std::string,Analyser::MemFuncPtr2>& m,
				const std::string& name,
				const Analyser::MemFuncPtr2 f)
    {
      using namespace std;
      typedef std::map<string,MemFuncPtr2>::value_type MVType;
      if(!m.insert(MVType(name,f)).second){
	string msg("Analyser::registerCallBack : ");
	msg += "key '"+name+"' already declared";
	throw(runtime_error(msg));
      }      
    } // end of Analyser::registerCallBack

    void
    Analyser::registerCallBacks()
    {
      this->registerCallBack(this->callBacks,"reset",&Analyser::treatReset);
      this->registerCallBack(this->callBacks,"plot",&Analyser::treatPlot);
      this->registerCallBack(this->callBacks,"replot",&Analyser::treatRePlot);
      this->registerCallBack(this->setCallBacks,"width",&Analyser::treatSetWidth);
      this->registerCallBack(this->setCallBacks,"height",&Analyser::treatSetHeight);
      this->registerCallBack(this->setCallBacks,"logscale",&Analyser::treatSetLogScale);
      this->registerCallBack(this->setCallBacks,"arrow",&Analyser::treatSetArrow);
      this->registerCallBack(this->setCallBacks,"g",&Analyser::treatSetGrid);
      this->registerCallBack(this->setCallBacks,"grid",&Analyser::treatSetGrid);
      this->registerCallBack(this->setCallBacks,"t",&Analyser::treatSetTerminal);
      this->registerCallBack(this->setCallBacks,"term",&Analyser::treatSetTerminal);
      this->registerCallBack(this->setCallBacks,"terminal",&Analyser::treatSetTerminal);
      this->registerCallBack(this->setCallBacks,"o",&Analyser::treatSetOutput);
      this->registerCallBack(this->setCallBacks,"out",&Analyser::treatSetOutput);
      this->registerCallBack(this->setCallBacks,"output",&Analyser::treatSetOutput);
      this->registerCallBack(this->setCallBacks,"label",&Analyser::treatSetLabel);
      this->registerCallBack(this->setCallBacks,"xrange",&Analyser::treatXRange);
      this->registerCallBack(this->setCallBacks,"xr",&Analyser::treatXRange);
      this->registerCallBack(this->setCallBacks,"x2range",&Analyser::treatX2Range);
      this->registerCallBack(this->setCallBacks,"x2r",&Analyser::treatX2Range);
      this->registerCallBack(this->setCallBacks,"yrange",&Analyser::treatYRange);
      this->registerCallBack(this->setCallBacks,"yr",&Analyser::treatYRange);
      this->registerCallBack(this->setCallBacks,"y2range",&Analyser::treatY2Range);
      this->registerCallBack(this->setCallBacks,"y2r",&Analyser::treatY2Range);
      this->registerCallBack(this->setCallBacks,"xlabel",&Analyser::treatXLabel);
      this->registerCallBack(this->setCallBacks,"xl",&Analyser::treatXLabel);
      this->registerCallBack(this->setCallBacks,"x2label",&Analyser::treatX2Label);
      this->registerCallBack(this->setCallBacks,"x2l",&Analyser::treatX2Label);
      this->registerCallBack(this->setCallBacks,"ylabel",&Analyser::treatYLabel);
      this->registerCallBack(this->setCallBacks,"yl",&Analyser::treatYLabel);
      this->registerCallBack(this->setCallBacks,"y2label",&Analyser::treatY2Label);
      this->registerCallBack(this->setCallBacks,"y2l",&Analyser::treatY2Label);
      this->registerCallBack(this->setCallBacks,"xtics",&Analyser::treatSetXTics);
      this->registerCallBack(this->setCallBacks,"x2tics",&Analyser::treatSetX2Tics);
      this->registerCallBack(this->setCallBacks,"ytics",&Analyser::treatSetYTics);
      this->registerCallBack(this->setCallBacks,"y2tics",&Analyser::treatSetY2Tics);
      this->registerCallBack(this->setCallBacks,"theme",&Analyser::treatTheme);
      this->registerCallBack(this->setCallBacks,"dummy",&Analyser::treatDummy);
      this->registerCallBack(this->setCallBacks,"title",&Analyser::treatSetTitle);
      this->registerCallBack(this->unsetCallBacks,"width",&Analyser::treatUnsetWidth);
      this->registerCallBack(this->unsetCallBacks,"height",&Analyser::treatUnsetHeight);
      this->registerCallBack(this->unsetCallBacks,"logscale",&Analyser::treatUnsetLogScale);
      this->registerCallBack(this->unsetCallBacks,"label",&Analyser::treatUnsetLabel);
      this->registerCallBack(this->unsetCallBacks,"xrange",&Analyser::treatUnsetXRange);
      this->registerCallBack(this->unsetCallBacks,"x2range",&Analyser::treatUnsetX2Range);
      this->registerCallBack(this->unsetCallBacks,"yrange",&Analyser::treatUnsetYRange);
      this->registerCallBack(this->unsetCallBacks,"y2range",&Analyser::treatUnsetY2Range);
      this->registerCallBack(this->unsetCallBacks,"xtics",&Analyser::treatUnsetXTics);
      this->registerCallBack(this->unsetCallBacks,"x2tics",&Analyser::treatUnsetX2Tics);
      this->registerCallBack(this->unsetCallBacks,"ytics",&Analyser::treatUnsetYTics);
      this->registerCallBack(this->unsetCallBacks,"y2tics",&Analyser::treatUnsetY2Tics);
      this->registerCallBack(this->unsetCallBacks,"xlabel",&Analyser::treatUnsetXLabel);
      this->registerCallBack(this->unsetCallBacks,"x2label",&Analyser::treatUnsetX2Label);
      this->registerCallBack(this->unsetCallBacks,"ylabel",&Analyser::treatUnsetYLabel);
      this->registerCallBack(this->unsetCallBacks,"y2label",&Analyser::treatUnsetY2Label);
      this->registerCallBack(this->unsetCallBacks,"grid",&Analyser::treatUnsetGrid);
      this->registerCallBack2(this->plotCallBacks,"t",&Analyser::treatPlotTitle);
      this->registerCallBack2(this->plotCallBacks,"title",&Analyser::treatPlotTitle);
      this->registerCallBack2(this->plotCallBacks,"w",&Analyser::treatPlotWith);
      this->registerCallBack2(this->plotCallBacks,"with",&Analyser::treatPlotWith);
      this->registerCallBack2(this->plotCallBacks,"using",&Analyser::treatPlotUsing);
      this->registerCallBack2(this->plotCallBacks,"u",&Analyser::treatPlotUsing);
      this->registerCallBack2(this->plotCallBacks,"axes",&Analyser::treatPlotAxes);
      this->registerCallBack2(this->plotCallBacks,"axis",&Analyser::treatPlotAxes);
      this->registerCallBack2(this->plotCallBacks,"lt",&Analyser::treatPlotLineType);
      this->registerCallBack2(this->plotCallBacks,"linetype",&Analyser::treatPlotLineType);
      this->registerCallBack2(this->plotCallBacks,"lw",&Analyser::treatPlotLineWidth);
      this->registerCallBack2(this->plotCallBacks,"linewidth",&Analyser::treatPlotLineWidth);
    } // end of Analyser::registerCallBacks()

#ifdef HAVE_GSL
    void
    Analyser::treatSetGSL(TokensContainer::const_iterator& p, 
			  const TokensContainer::const_iterator pe)
    {
      using namespace std;
      using namespace tfel::math::parser;
      this->readSpecifiedToken("Analyser::treatSetGSL","mode",p,pe);
      this->checkNotEndOfLine("Analyser::treatSetGSL","",p,pe);
      if(p->value=="GSL_PREC_DOUBLE"){
	getGSLMode()=GSL_PREC_DOUBLE;
      } else if (p->value=="GSL_PREC_SINGLE"){
	getGSLMode()=GSL_PREC_SINGLE;
      } else if (p->value=="GSL_PREC_APPROX"){
	getGSLMode()=GSL_PREC_APPROX;
      } else {
	string msg("Analyser::treatSetGSL : ");
	msg += "unknown gsl mode '"+p->value+"'";
	throw(runtime_error(msg));
      }
      ++p;
    } // end of Analyser::treatSetGSL
#endif
    
    void
    Analyser::treatSetWidth(TokensContainer::const_iterator& p, 
			    const TokensContainer::const_iterator pe)
    {
      this->checkNotEndOfLine("Analyser::treatSetWidth","",p,pe);
      unsigned short w = this->convertToUnsignedShort(p->value);
      ++p;
      this->area.setWidth(w);
    } // end of Analyser::treatSetWidth

    void
    Analyser::treatSetHeight(TokensContainer::const_iterator& p, 
			     const TokensContainer::const_iterator pe)
    {
      this->checkNotEndOfLine("Analyser::treatSetHeight","",p,pe);
      unsigned short h = this->convertToUnsignedShort(p->value);
      ++p;
      this->area.setHeight(h);
    } // end of Analyser::treatSetHeight

    void
    Analyser::treatUnsetLabel(TokensContainer::const_iterator& p, 
			      const TokensContainer::const_iterator pe)
    {
      using namespace std;
      if(p==pe){
	this->area.clearLabels();
      } else {
	if(!this->isValidIdentifier(p->value)){
	  string msg("Analyser::treatUnsetLabel : '");
	  msg += p->value+"' is not a valid identifer.";
	  throw(runtime_error(msg));
	}
	this->area.clearLabel(p->value);
	++p;
      }
    } // end of Analyser::treatUnsetLabel

    void
    Analyser::treatUnsetArrow(TokensContainer::const_iterator& p, 
			      const TokensContainer::const_iterator pe)
    {
      using namespace std;
      if(p==pe){
	this->area.clearArrows();
      } else {
	if(!this->isValidIdentifier(p->value)){
	  string msg("Analyser::treatUnsetArrow : '");
	  msg += p->value+"' is not a valid identifer.";
	  throw(runtime_error(msg));
	}
	this->area.clearArrow(p->value);
	++p;
      }
    } // end of Analyser::treatUnsetArrow

    void
    Analyser::treatUnsetWidth(TokensContainer::const_iterator&, 
			      const TokensContainer::const_iterator)
    {
      this->area.unsetWidth();
    } // end of Analyser::treatUnsetWidth

    void
    Analyser::treatUnsetHeight(TokensContainer::const_iterator&, 
			       const TokensContainer::const_iterator)
    {
      this->area.unsetHeight();
    } // end of Analyser::treatUnsetHeight

    void
    Analyser::treatSetLogScale(TokensContainer::const_iterator& p, 
			       const TokensContainer::const_iterator pe)
    {
      using namespace std;
      this->checkNotEndOfLine("Analyser::treatSetLogScale","",p,pe);
      if(p->value=="x"){
	this->area.setXAxisLogScale();
      } else if(p->value=="y"){
	this->area.setYAxisLogScale();
      } else if(p->value=="x2"){
	this->area.setX2AxisLogScale();
      } else if(p->value=="y2"){
	this->area.setY2AxisLogScale();
      } else {
	string msg("Analyser::treatSetLogScale : ");
	msg += "unknown axis '"+p->value+"'";
	throw(runtime_error(msg));
      }
      ++p;
    } // end of Analyser::treatSetLogScale

    void
    Analyser::treatUnsetLogScale(TokensContainer::const_iterator& p, 
				 const TokensContainer::const_iterator pe)
    {
      using namespace std;
      this->checkNotEndOfLine("Analyser::treatUnsetLogScale","",p,pe);
      if(p->value=="x"){
	this->area.unsetXAxisLogScale();
      } else if(p->value=="y"){
	this->area.unsetYAxisLogScale();
      } else if(p->value=="x2"){
	this->area.unsetX2AxisLogScale();
      } else if(p->value=="y2"){
	this->area.unsetY2AxisLogScale();
      } else {
	string msg("Analyser::treatUnsetLogScale : ");
	msg += "unknown axis '"+p->value+"'";
	throw(runtime_error(msg));
      }
      ++p;
    } // end of Analyser::treatUnsetLogScale

    std::map<double,std::string>
    Analyser::treatSetTics(TokensContainer::const_iterator& p, 
			   const TokensContainer::const_iterator pe)
    {
      using namespace std;
      using namespace tfel::utilities;
      using namespace tfel::math;
      typedef std::map<double,string>::value_type MVType;
      std::map<double,string> res;
      this->readSpecifiedToken("Analyser::treatSetTics","(",p,pe);
      this->checkNotEndOfLine("Analyser::treatSetTics","",p,pe);
      if(p->value==")"){
	string msg("Analyser::treatSetTics : ");
	msg += "empty list";
	throw(runtime_error(msg));
      }
      while(p->value!=")"){
	string sev;
	string tic;
	if(p->flag==Token::String){
	  tic = this->readString(p,pe);
	  if(tic.empty()){
	    string msg("Analyser::treatSetTics : ");
	    msg += "empty tic specified";
	    throw(runtime_error(msg));
	  }
	}
	this->checkNotEndOfLine("Analyser::treatSetTics","",p,pe);
	while((p->value!=",")&&(p->value!=")")){
	  sev += p->value;
	  ++p;
	  this->checkNotEndOfLine("Analyser::treatSetTics","",p,pe);
	}
	Evaluator ev(vector<string>(),sev,this->functions);
	const vector<string>& vars = ev.getVariablesNames();
	if(vars.size()!=0){
	  ostringstream msg;
	  if(vars.size()!=1u){
	    msg << "Analyser::treatSetTics : invalid print declaration (unknown variables ";
	    copy(vars.begin(),vars.end(),ostream_iterator<string>(msg," "));
	    msg << ")";
	  } else {
	    msg << "Analyser::treatSetTics : invalid print declaration (unknown variable "
		<< vars[0] << ")";
	  }
	  throw(runtime_error(msg.str()));
	}
	const double value = ev.getValue();
	if(tic.empty()){
	  ostringstream converter;
	  converter << value;
	  res.insert(MVType(value,converter.str()));
	} else {
	  res.insert(MVType(value,tic));
	}
	this->checkNotEndOfLine("Analyser::treatSetTics","",p,pe);
	if(p->value!=")"){
	  this->readSpecifiedToken("Analyser::treatSetTics",",",p,pe);
	  this->checkNotEndOfLine("Analyser::treatSetTics","",p,pe);
	}
      }
      ++p;
      return res;
    } // end of Analyser::treatSetTics

    void
    Analyser::treatUnsetXTics(TokensContainer::const_iterator&, 
			      const TokensContainer::const_iterator)
    {
      this->area.unsetXTics();
    } // end of Analyser::treatUnsetXTics

    void
    Analyser::treatUnsetX2Tics(TokensContainer::const_iterator&, 
			       const TokensContainer::const_iterator)
    {
      this->area.unsetX2Tics();
    } // end of Analyser::treatUnsetXTics

    void
    Analyser::treatUnsetYTics(TokensContainer::const_iterator&, 
			      const TokensContainer::const_iterator)
    {
      this->area.unsetYTics();
    } // end of Analyser::treatUnsetXTics

    void
    Analyser::treatUnsetY2Tics(TokensContainer::const_iterator&, 
			       const TokensContainer::const_iterator)
    {
      this->area.unsetY2Tics();
    } // end of Analyser::treatUnsetXTics

    void
    Analyser::treatUnsetXLabel(TokensContainer::const_iterator&, 
			       const TokensContainer::const_iterator)
    {
      this->area.setXLabel("");
    } // end of Analyser::treatUnsetXLabel

    void
    Analyser::treatUnsetX2Label(TokensContainer::const_iterator&, 
				const TokensContainer::const_iterator)
    {
      this->area.setX2Label("");
    } // end of Analyser::treatUnsetX2Label

    void
    Analyser::treatUnsetYLabel(TokensContainer::const_iterator&, 
			       const TokensContainer::const_iterator)
    {
      this->area.setYLabel("");
    } // end of Analyser::treatUnsetYLabel

    void
    Analyser::treatUnsetY2Label(TokensContainer::const_iterator&, 
				const TokensContainer::const_iterator)
    {
      this->area.setY2Label("");
    } // end of Analyser::treatUnsetY2Label

    void
    Analyser::treatUnsetGrid(TokensContainer::const_iterator&, 
			     const TokensContainer::const_iterator)
    {
      this->area.showGrid(false);
    } // end of Analyser::treatUnsetY2Label

    void
    Analyser::treatSetXTics(TokensContainer::const_iterator& p, 
			    const TokensContainer::const_iterator pe)
    {
      this->checkNotEndOfLine("Analyser::treatSetXTics","",p,pe);
      if(p->value=="auto"){
	this->area.unsetXTics();
      } else {
	this->area.setXTics(this->treatSetTics(p,pe));
      }
    } // end of Analyser::treatSetXTics

    void
    Analyser::treatSetX2Tics(TokensContainer::const_iterator& p, 
			     const TokensContainer::const_iterator pe)
    {
      this->checkNotEndOfLine("Analyser::treatSetX2Tics","",p,pe);
      if(p->value=="auto"){
	this->area.unsetX2Tics();
      } else {
	this->area.setX2Tics(this->treatSetTics(p,pe));
      }
    } // end of Analyser::treatSetX2Tics

    void
    Analyser::treatSetYTics(TokensContainer::const_iterator& p, 
			    const TokensContainer::const_iterator pe)
    {
      this->checkNotEndOfLine("Analyser::treatSetYTics","",p,pe);
      if(p->value=="auto"){
	this->area.unsetYTics();
      } else {
	this->area.setYTics(this->treatSetTics(p,pe));
      }
    } // end of Analyser::treatSetYTics

    void
    Analyser::treatSetY2Tics(TokensContainer::const_iterator& p, 
			     const TokensContainer::const_iterator pe)
    {
      this->checkNotEndOfLine("Analyser::treatSetY2Tics","",p,pe);
      if(p->value=="auto"){
	this->area.unsetY2Tics();
      } else {
	this->area.setY2Tics(this->treatSetTics(p,pe));
      }
    } // end of Analyser::treatSetY2Tics

    void
    Analyser::treatSetArrow(TokensContainer::const_iterator& p, 
			    const TokensContainer::const_iterator pe)
    {
      using namespace std;
      string name;
      ArrowStyle style;
      this->checkNotEndOfLine("Analyser::treatSetArrow","",p,pe);
      if(p->value!="from"){
	name = p->value;
	if(!this->isValidIdentifier(p->value)){
	  string msg("Analyser::treatSetArrow : '");
	  msg += p->value+"' is not a valid identifer.";
	  throw(runtime_error(msg));
	}
	++p;
	this->readSpecifiedToken("Analyser::treatSetArrow","from",p,pe);
      } else {
	++p;
      }
      const GraphCoordinates x0 = this->readCoordinates(p,pe);
      this->readSpecifiedToken("Analyser::treatSetArrow",",",p,pe);
      const GraphCoordinates y0 = this->readCoordinates(p,pe);
      this->readSpecifiedToken("Analyser::treatSetArrow ","to",p,pe);
      const GraphCoordinates x1 = this->readCoordinates(p,pe);
      this->readSpecifiedToken("Analyser::treatSetArrow",",",p,pe);
      const GraphCoordinates y1 = this->readCoordinates(p,pe);
      while(p!=pe){
	if(p->value == "head"){
	  style.style = ArrowStyle::HEAD;
	} else if(p->value == "heads"){
	  style.style = ArrowStyle::HEADS;
	} else if(p->value == "nohead"){
	  style.style = ArrowStyle::NOHEAD;
	} else if(p->value == "backhead"){
	  style.style = ArrowStyle::BACKHEAD;
	} else {
	  string msg("Analyser::treatSetArrow : ");
	  msg += "unknown arrow style '"+p->value+"'";
	  throw(runtime_error(msg));
	}
	++p;
      }
      if(name.empty()){
	this->area.addArrow(x0,y0,x1,y1,style);
      } else {
	this->area.addArrow(name,x0,y0,x1,y1,style);
      }
    } // end of Analyser::treatSetArrow

    void
    Analyser::treatSetGrid(TokensContainer::const_iterator& p, 
			   const TokensContainer::const_iterator pe)
    {
      using namespace std;
      unsigned short grid = this->area.getGrid();
      while(p!=pe){
	if(p->value=="xtics"){
	  grid = grid|Grid::X;
	} else if(p->value=="noxtics"){
	  if(grid&Grid::X){
	    grid = grid^Grid::X;
	  }
	} else if(p->value=="ytics"){
	  grid = grid|Grid::Y;
	} else if(p->value=="noytics"){
	  if(grid&Grid::Y){
	    grid = grid^Grid::Y;
	  }
	} else if(p->value=="x2tics"){
	  grid = grid|Grid::X2;
	} else if(p->value=="nox2tics"){
	  if(grid&Grid::X2){
	    grid = grid^Grid::X2;
	  }
	} else if(p->value=="y2tics"){
	  grid = grid|Grid::Y2;
	} else if(p->value=="noy2tics"){
	  if(grid&Grid::Y2){
	    grid = grid^Grid::Y2;
	  }
	} else {
	  string msg("Analyser::treatSetGrid : ");
	  msg += "unknown grid type '"+p->value+"'";
	  throw(runtime_error(msg));
	}
	++p;
      }
      this->area.setGrid(grid);
      this->area.showGrid();
    } //end of Analyser::treatSetGrid

    void
    Analyser::treatSetTitle(TokensContainer::const_iterator& p, 
			    const TokensContainer::const_iterator pe)
    {
      this->area.setUpperTitle(this->readString(p,pe));
    } //end of Analyser::treatSetTitle

    void
    Analyser::treatSetOutput(TokensContainer::const_iterator& p, 
			     const TokensContainer::const_iterator pe)
    {
      this->output = this->readString(p,pe);
    } // end of Analyser::treatSetOutput

    void
    Analyser::treatSetTerminal(TokensContainer::const_iterator& p, 
			       const TokensContainer::const_iterator pe)
    {
      using namespace std;
      this->checkNotEndOfLine("Analyser::treatSetTerminal",
			      "expected terminal name",p,pe);
      if((p->value=="eps")||
	 (p->value=="pdf")||
	 (p->value=="svg")||
	 (p->value=="png")||
	 (p->value=="x11")){
	this->terminal=p->value;
      } else {
	string msg("Analyser::treatSetTerminal : ");
	msg += "unsupported terminal '"+p->value+"'";
	throw(runtime_error(msg));
      }
      ++p;
    } // end of Analyser::treatSetOutput

    GraphCoordinates
    Analyser::readCoordinates(TokensContainer::const_iterator& p, 
			      const TokensContainer::const_iterator pe)
    {
      this->checkNotEndOfLine("Analyser::readCoordinates","",p,pe);
      if(p->value=="first"){
	return GraphCoordinates(GraphCoordinates::FIRST,this->readFunction(p,pe));
      } else if(p->value=="second"){
	return GraphCoordinates(GraphCoordinates::SECOND,this->readFunction(p,pe));
      } else if(p->value=="graph"){
	return GraphCoordinates(GraphCoordinates::GRAPH,this->readFunction(p,pe));
      } else if(p->value=="screen"){
	return GraphCoordinates(GraphCoordinates::SCREEN,this->readFunction(p,pe));
      } else if(p->value=="character"){
	return GraphCoordinates(GraphCoordinates::CHARACTER,this->readFunction(p,pe));
      }
      return GraphCoordinates(GraphCoordinates::FIRST,this->readFunction(p,pe));
    } // end of Analyser::readCoordinates

    void
    Analyser::treatSetLabel(TokensContainer::const_iterator& p, 
			    const TokensContainer::const_iterator pe)
    {
      using namespace std;
      using namespace tfel::utilities;
      string name;
      this->checkNotEndOfLine("Analyser::treatSetLabel","",p,pe);
      if(p->flag!=Token::String){
	name = p->value;
	if(!this->isValidIdentifier(p->value)){
	  string msg("Analyser::treatSetLabel : '");
	  msg += p->value+"' is not a valid identifer.";
	  throw(runtime_error(msg));
	}
	++p;
      } 
      const std::string& label = this->readString(p,pe);
      this->readSpecifiedToken("Analyser::treatSetLabel","at",p,pe);
      const GraphCoordinates x = this->readCoordinates(p,pe);
      this->readSpecifiedToken("Analyser::treatSetLabel",",",p,pe);
      const GraphCoordinates y = this->readCoordinates(p,pe);
      if(name.empty()){
	this->area.addLabel(label,x,y);
      } else {
	this->area.addLabel(name,label,x,y);
      }
    } //end of Analyser::treatSetTitle

    void
    Analyser::setRanges(void)
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
    } // end of Analyser::setRanges

    void
    Analyser::readRange(bool& hasRMinValue,
			bool& hasRMaxValue,
			double& rmin,
			double& rmax,
			TokensContainer::const_iterator& p, 
			const TokensContainer::const_iterator pe)
    {
      using namespace std;
      using namespace tfel::math;
      this->readSpecifiedToken("Analyser::readRange","[",p,pe);
      this->checkNotEndOfLine("Analyser::readRange","expected ':' or a value",p,pe);
      if(p->value!=":"){
	vector<string> vars;
	Evaluator ev(vars,this->readUntil(p,pe,":"),
		     this->functions);
	ev.removeDependencies();
	if(ev.getNumberOfVariables()!=0u){
	  const vector<string>& ev_vars = ev.getVariablesNames();
	  ostringstream msg;
	  if(ev_vars.size()!=1u){
	    msg << "Analyser::readRange : invalid range declaration (unknown variables ";
	    copy(ev_vars.begin(),ev_vars.end(),ostream_iterator<string>(msg," "));
	    msg << ")";
	  } else {
	    msg << "Analyser::readRange : invalid range declaration (unknown variable "
		<< ev_vars[0] << ")";
	  }
	  throw(runtime_error(msg.str()));
	}
	rmin = ev.getValue();
	hasRMinValue = true;
      } else {
	hasRMinValue = false;
	++p;
      }
      this->checkNotEndOfLine("Analyser::readRange","expected ']' or a value",p,pe);
      if(p->value!="]"){
	vector<string> vars;
	Evaluator ev(vars,this->readUntil(p,pe,"]"),
		     this->functions);
	ev.removeDependencies();
	if(ev.getNumberOfVariables()!=0u){
	  const vector<string>& ev_vars = ev.getVariablesNames();
	  ostringstream msg;
	  if(ev_vars.size()!=1u){
	    msg << "Analyser::readRange : invalid range declaration (unknown variables ";
	    copy(ev_vars.begin(),ev_vars.end(),ostream_iterator<string>(msg," "));
	    msg << ")";
	  } else {
	    msg << "Analyser::readRange : invalid range declaration (unknown variable "
		<< ev_vars[0] << ")";
	  }
	  throw(runtime_error(msg.str()));
	}
	rmax = ev.getValue();
	hasRMaxValue = true;
      } else {
	hasRMaxValue = false;
	++p;
      }
    } // ned of Analyser::readRange

    static double
    convertFromHexadecimal(const char c1,
			   const char c2)
    {
      using namespace std;
      static const char hexc [] = {'0','1','2','3','4','5','6','7','8','9',
				 'a','b','c','d','e','f'};
      unsigned short i1 = static_cast<unsigned short>(find(hexc,hexc+16,tolower(c1)) - hexc);
      unsigned short i2 = static_cast<unsigned short>(find(hexc,hexc+16,tolower(c2)) - hexc);
      if((i1>15)||(i2>15)){
	string msg("convertFromHexadecimal : invalid input");
	throw(runtime_error(msg));
      }
      return static_cast<double>((i1<<4)+i2)/255.;
    } // end of convertFromHexadecimal

    void
    Analyser::treatPlotLineType(CurveOptions& options,
				TokensContainer::const_iterator& p, 
				const TokensContainer::const_iterator pe)
    {
      using namespace std;
      std::map<string,Curve::Color>::const_iterator pc;
      this->checkNotEndOfLine("Analyser::treatPlotType",
			      "expected line type",p,pe);
      if(options.hasSpecifiedColor){
	string msg("Analyser::treatPlotLineType : ");
	msg += "line type already specified";
	throw(runtime_error(msg));
      }
      if(p->value=="rgb"){
	++p;
	this->checkNotEndOfLine("Analyser::treatPlotType",
				"expected line type",p,pe);
	string type = this->readString(p,pe);
	if(type.empty()){
	  string msg("Analyser::treatPlotLineType : ");
	  msg += "empty line type specified";
	  throw(runtime_error(msg));
	}
	if(type[0]=='#'){
	  // rgb
	  if(type.size()!=7){
	    string msg("Analyser::treatPlotLineType : ");
	    msg += "invalid hexadecimal number";
	    throw(runtime_error(msg));
	  }
	  options.color.r=convertFromHexadecimal(type[1],type[2]);
	  options.color.g=convertFromHexadecimal(type[3],type[4]);
	  options.color.b=convertFromHexadecimal(type[5],type[6]);
	} else {
	  pc=this->colors.find(type);
	  if(pc==this->colors.end()){
	    string msg("Analyser::treatPlotLineType : ");
	    msg += "no color '"+type+"' defined";
	    throw(runtime_error(msg));
	  }
	  options.color.r = pc->second.r;
	  options.color.g = pc->second.g;
	  options.color.b = pc->second.b;
	}
      } else {
	if(!Analyser::isUnsignedInteger(p->value)){
	  string msg("Analyser::treatPlotLineType : ");
	  msg += "unexpected token 'rgb' or a number";
	  throw(runtime_error(msg));
	}
	this->area.getTheme()->getDefaultColor(options.color,this->convertToUnsignedShort(p->value));
	++p;
      }
	options.hasSpecifiedColor=true;
    } // end of Analyser::treatPlotLineType

    void
    Analyser::treatPlotLineWidth(CurveOptions& options,
				TokensContainer::const_iterator& p, 
				const TokensContainer::const_iterator pe)
    {
      using namespace std;
      std::map<string,Curve::Color>::const_iterator pc;
      this->checkNotEndOfLine("Analyser::treatPlotWidth",
			      "expected line type",p,pe);
      if(options.hasSpecifiedWidth){
	string msg("Analyser::treatPlotLineWidth : ");
	msg += "line width already specified";
	throw(runtime_error(msg));
      }
      if(!Analyser::isUnsignedInteger(p->value)){
	string msg("Analyser::treatPlotWidth : ");
	msg += "unexpected a number";
	throw(runtime_error(msg));
      }
      options.width = this->convertToUnsignedShort(p->value);
      options.hasSpecifiedWidth=true;
      ++p;
    } // end of Analyser::treatPlotLineWidth

    void
    Analyser::treatPlotAxes(CurveOptions& options,
			    TokensContainer::const_iterator& p, 
			    const TokensContainer::const_iterator pe)
    {
      using namespace std;
      this->checkNotEndOfLine("Analyser::treatPlotAxes",
			      "expected axis name",p,pe);
      if(options.axis!=Graph::XY){
	string msg("Analyser::treatPlotAxes : ");
	msg += "axes already specified";
	throw(runtime_error(msg));
      }
      if(p->value=="x1y1"){
	options.axis = Graph::XY;
      } else if(p->value=="x1y2"){
	options.axis = Graph::XY2;
      } else if(p->value=="x2y1"){
	options.axis = Graph::X2Y;
      } else if(p->value=="x2y2"){
	options.axis = Graph::X2Y2;
      } else {
	string msg("Analyser::treatPlotAxes : ");
	msg += "unknown axis '"+p->value+"'";
	throw(runtime_error(msg));
      }
      ++p;
    } // end of Analyser::treatPlotAxes

    void
    Analyser::treatPlotUsing(CurveOptions& options,
			     TokensContainer::const_iterator& p, 
			     const TokensContainer::const_iterator pe)
    {
      using namespace std;
      if((!options.using_decl_x.empty())||
	 (!options.using_decl_y.empty())){
	string msg("Analyser::treatPlotUsing : ");
	msg += "using option already specified.";
	throw(runtime_error(msg));
      }
      this->checkNotEndOfLine("Analyser::treatPlotUsing",
			      "expected using declaration",p,pe);
      this->readDataFunctionInUsingDeclaration(options.using_decl_x,p,pe);
      this->readSpecifiedToken("Analyser::treatPlotUsing",":",p,pe);
      this->checkNotEndOfLine("Analyser::treatPlotUsing","",p,pe);
      this->readDataFunctionInUsingDeclaration(options.using_decl_y,p,pe);
    } // end of Analyser::treatPlotUsing

    void
    Analyser::treatPlotTitle(CurveOptions& options,
			     TokensContainer::const_iterator& p, 
			     const TokensContainer::const_iterator pe)
    {
      using namespace std;
      this->checkNotEndOfLine("Analyser::treatPlotTitle",
			      "expected curve legend",p,pe);
      if(!options.title.empty()){
	string msg("Analyser::treatPlotTitle : ");
	msg += "curve legend already specified";
	throw(runtime_error(msg));
      }
      options.title = this->readString(p,pe);
    } // end of Analyser::treatPlotTitle

    void
    Analyser::treatPlotWith(CurveOptions& options,
			    TokensContainer::const_iterator& p, 
			    const TokensContainer::const_iterator pe)
    {
      using namespace std;
      this->checkNotEndOfLine("Analyser::treatPlotTitle",
			      "expected curve legend",p,pe);
      if(options.style!=Curve::LINE){
	string msg("Analyser::treatPlotWith : ");
	msg += "curve style already specified";
	throw(runtime_error(msg));
      }
      if((p->value=="l")||(p->value=="lines")){
	options.style=Curve::LINE;
      } else if(p->value=="linespoints"){
	options.style=Curve::LINEPOINT;
      } else if((p->value=="d")||(p->value=="dots")){
	options.style=Curve::DOT;
      } else if((p->value=="p")||(p->value=="points")){
	options.style=Curve::POINT;
      } else if((p->value=="t")||(p->value=="triangles")){
	options.style=Curve::TRIANGLE;
      } else if((p->value=="c")||(p->value=="crosses")){
	options.style=Curve::CROSS;
      } else if((p->value=="d")||(p->value=="diamonds")){
	options.style=Curve::DIAMOND;
      } else if((p->value=="s")||(p->value=="square")){
	options.style=Curve::SQUARE;
      } else {
	string msg("Analyser::treatPlotWith : ");
	msg += "unsupported style '"+p->value+"'";
	throw(runtime_error(msg));
      }
      ++p;
    } // end of Analyser::treatPlotWith

    void
    Analyser::treatTheme(TokensContainer::const_iterator& p, 
			 const TokensContainer::const_iterator pe)
    {
      using namespace std;
      this->checkNotEndOfLine("Analyser::treatTheme",
			      "expected theme name",p,pe);
      const vector<string>& themes = ThemeManager::getThemeManager().getAvailableThemesNames();
      const std::string theme = this->readString(p,pe);
      if(find(themes.begin(),themes.end(),theme)==themes.end()){
	ostringstream msg;
	msg << "Analyser::treatTheme : "
	    << "unknown theme (read '" << theme << "').\n";
	copy(themes.begin(),themes.end(),ostream_iterator<string>(msg," "));
	msg << ".";
	throw(runtime_error(msg.str()));
      }
      this->area.setTheme(ThemeManager::getThemeManager().getTheme(theme));
    } // end of Analyser::treatTheme

    void
    Analyser::treatReset(TokensContainer::const_iterator&, 
			 const TokensContainer::const_iterator)
    {
      this->reset();
      this->area.reset();
    } // end of Analyser::treatQuit

    void
    Analyser::treatInclude(TokensContainer::const_iterator& p, 
			   const TokensContainer::const_iterator pe)
    {
      using namespace std;
      TokensContainer::const_iterator p2;
      TokensContainer::difference_type pos;
      this->checkNotEndOfLine("Analyser::treatInclude",
			      "expected file name",p,pe);
      if(p->flag!=tfel::utilities::Token::String){
	string msg("Analyser::include : ");
	msg += "expected to read a string (read '"+p->value+"').\n";
	throw(runtime_error(msg));
      }
      p2 = this->fileTokens.begin();
      pos = 0;
      while(p2!=p){
	++p2;
	++pos;
      }
      TokensContainer tmp(fileTokens);
      this->analyseFile(p->value.substr(1,p->value.size()-2));
      this->fileTokens = tmp;
      p=this->fileTokens.begin();
      advance(p,pos+1);
    } // end of Analyser::treatInclude

    void
    Analyser::treatFunctionPlot(TokensContainer::const_iterator& p, 
				const TokensContainer::const_iterator pe)
    {
      using namespace std;
      using namespace tfel::utilities;
      using namespace tfel::math;
      vector<string> vars(1,this->dummyVariable);
      unsigned short openedParenthesis=0;
      bool cont=true;
      TokensContainer::const_iterator prev = p;
      string f;
      while((p!=pe)&&(cont)){
	if(p->value=="("){
	  ++openedParenthesis;
	}
	if(p->value==")"){
	  if(openedParenthesis==0){
	    string msg("Analyser::treatFunctionPlot : ");
	    msg += "unbalanced parenthesis";
	    throw(runtime_error(msg));
	  }
	  --openedParenthesis;
	}
	if(p->value==","){
	  if(openedParenthesis==0){
	    cont = false;
	  } else {
	    f += p->value;
	    prev=p;
	    ++p;
	  }
	} else if(this->plotCallBacks.find(p->value)!=this->plotCallBacks.end()){
	  if(openedParenthesis==0){
	    // this might be the title
	    if((prev->value=="+")||
	       (prev->value=="-")||
	       (prev->value=="*")||
	       (prev->value=="/")||
	       (prev->value=="**")){
	      f += p->value;
	      prev=p;
	      ++p;
	    } else {
	      cont = false;
	    }
	  } else {
	    f += p->value;
	    prev=p;
	    ++p;
	  }
	} else {
	  f += p->value;
	  prev=p;
	  ++p;
	}
      }
      if(f.empty()){
	string msg("Analyser::treatFunctionPlot : ");
	msg += "no function given";
	throw(runtime_error(msg));
      }
      shared_ptr<Evaluator> ev(new Evaluator(vars,f,this->functions));
      ev->removeDependencies();
      const CurveOptions& options = this->treatPlotOptions(p,pe);
      if((!options.using_decl_x.empty())||
	 (!options.using_decl_y.empty())){
	string msg("Analyser::treatFunctionPlot : ");
	msg += "the 'using' keyword is invalid for functions";
	throw(runtime_error(msg));
      }
      shared_ptr<Curve> c(new EvaluatedCurve(ev,this->dummyVariable));
      this->applyCurveOptions(c,options);
      if(options.title.empty()){
	c->setLegend(f);
      }
      // here treating options
      this->area.addCurve(c,options.axis);
    } // end of Analyser::treatFunctionPlot
  
    Analyser::CurveOptions
    Analyser::treatPlotOptions(TokensContainer::const_iterator& p, 
			       const TokensContainer::const_iterator pe)
    {
      using namespace std;
      CurveOptions options;
      std::map<string,MemFuncPtr2>::const_iterator ps;
      while((p!=pe)&&(p->value!=",")){
	ps = this->plotCallBacks.find(p->value);
	if(ps == this->plotCallBacks.end()){
	  string msg("Analyser::treatPlotOptions : ");
	  msg += "unknown option '"+p->value+"'";
	  throw(runtime_error(msg));
	}
	++p;
	(this->*(ps->second))(options,p,pe);
      }
      return options;
    } // end of Analyser::treatPlotOptions

    void
    Analyser::applyCurveOptions(tfel::utilities::shared_ptr<Curve> c,
				const Analyser::CurveOptions& options)
    {
      if(!options.title.empty()){
	c->setLegend(options.title);
      }
      if(options.hasSpecifiedColor){
	c->setColor(options.color);
      }
      if(options.hasSpecifiedWidth){
	c->setWidth(options.width);
      }
      c->setStyle(options.style);
    } // end of Analyser::applyCurveOptions

    void
    Analyser::treatDataPlot(TokensContainer::const_iterator& p, 
			    const TokensContainer::const_iterator pe)
    {
      using namespace std;
      using namespace tfel::utilities;
      vector<double> vx;
      vector<double> vy;
      string file = this->readString(p,pe);
      string x;
      string y;
      const CurveOptions& options = this->treatPlotOptions(p,pe);
      TextData data(file);
      if(options.using_decl_x.empty()){
	x = "1";
      } else {
	x = options.using_decl_x;
      }
      if(options.using_decl_y.empty()){
	y = "2";
      } else {
	y = options.using_decl_y;
      }
      this->getData(vx,data,x);
      const std::string& ty = this->getData(vy,data,y);
      shared_ptr<Curve> c(new DataCurve(vx,vy));
      this->applyCurveOptions(c,options);
      if(options.title.empty()){
	if(ty.empty()){
	  string legend = '\"'+file+'\"';
	  legend += " using ";
	  if(Analyser::isUnsignedInteger(x)){
	    legend += x;
	  } else {
	    legend += "("+x+")";
	  }
	  legend += ":";
	  if(Analyser::isUnsignedInteger(y)){
	    legend += y;
	  } else {
	    legend += "("+y+")";
	  }
	  c->setLegend(legend);
	} else {
	  c->setLegend(ty);
	}
      } 
      // here treating options
      this->area.addCurve(c,options.axis);
    } // end of Analyser::treatDataPlot

    void
    Analyser::treatRePlot(TokensContainer::const_iterator&, 
			  const TokensContainer::const_iterator)
    {
      using namespace std;
      if(this->previousPlot.empty()){
	string msg("Analyser::treatRePlot : ");
	msg += "no previous plot";
	throw(runtime_error(msg));
      }
      this->currentLine.clear();
      this->analyseLine(this->previousPlot,
			this->currentLineNbr);
    } // end of Analyser::treatRePlot

    void
    Analyser::treatPlot(TokensContainer::const_iterator& p, 
			const TokensContainer::const_iterator pe)
    {
      using namespace std;
      using namespace tfel::utilities;
      using namespace tfel::math;
      this->checkNotEndOfLine("Analyser::treatPlot","",p,pe);
      if(p->value=="["){
	this->readRange(this->hasXMinValue,
			this->hasXMaxValue,
			this->xmin,this->xmax,p,pe);
      }
      this->checkNotEndOfLine("Analyser::treatPlot","",p,pe);
      if(p->value=="["){
	this->readRange(this->hasYMinValue,
			this->hasYMaxValue,
			this->ymin,this->ymax,p,pe);
      }
      this->checkNotEndOfLine("Analyser::treatPlot","",p,pe);
      this->area.clearCurves();
      this->setRanges();
      while(p!=pe){
	if(p->flag==Token::String){
	  this->treatDataPlot(p,pe);
	} else {
	  this->treatFunctionPlot(p,pe);
	}
	if(p!=pe){
	  if(p->value!=","){
	    string msg("Analyser::treatPlot : ");
	    msg += "unexpected token '"+p->value+"'";
	    throw(runtime_error(msg));
	  }
	  ++p;
	  this->checkNotEndOfLine("Analyser::treatPlot","",p,pe);
	}
      }
      this->area.queue_draw();
      if(this->terminal=="x11"){
	this->show();
      } else {
	if(this->output.empty()){
	  string msg("Analyser::treatPlot : ");
	  msg += "no output file specified";
	  throw(runtime_error(msg));
	}
	if(this->terminal=="eps"){
	  this->area.exportToPostScript(this->output);
	} else if(this->terminal=="pdf"){
	  this->area.exportToPDF(this->output);
	} else if(this->terminal=="svg"){
	  this->area.exportToSVG(this->output);
	} else if(this->terminal=="png"){
	  this->area.exportToPNG(this->output);
	} else {
	  string msg("Analyser::treatPlot : ");
	  msg += "internal error, unsupported terminal '"+this->terminal+"'";
	  throw(runtime_error(msg));
	}
      }
      this->previousPlot = this->currentLine;
    } // end of Analyser::treatPlot
      
    void
    Analyser::treatSet(TokensContainer::const_iterator& p,
		       const TokensContainer::const_iterator pe)
    {
      using namespace tfel::math::interpreter;
      InterpreterBase<Analyser>::treatSet(p,pe);
      this->area.queue_draw();
    } // end of Analyser::treatSet

    void
    Analyser::treatUnset(TokensContainer::const_iterator& p,
			 const TokensContainer::const_iterator pe)
    {
      using namespace tfel::math::interpreter;
      InterpreterBase<Analyser>::treatUnset(p,pe);
      this->area.queue_draw();
    } // end of Analyser::treatUnset

    void
    Analyser::treatUnsetXRange(TokensContainer::const_iterator&,
			       const TokensContainer::const_iterator)
    {
      this->area.unsetXRange();
    } // end of Analyser::treatUnsetXRange

    void
    Analyser::treatUnsetX2Range(TokensContainer::const_iterator&,
				const TokensContainer::const_iterator)
    {
      this->area.unsetX2Range();
    } // end of Analyser::treatUnsetXRange

    void
    Analyser::treatUnsetYRange(TokensContainer::const_iterator&,
			       const TokensContainer::const_iterator)
    {
      this->area.unsetYRange();
    } // end of Analyser::treatUnsetYRange

    void
    Analyser::treatUnsetY2Range(TokensContainer::const_iterator&,
				const TokensContainer::const_iterator)
    {
      this->area.unsetY2Range();
    } // end of Analyser::treatUnsetYRange

    void
    Analyser::treatXRange(TokensContainer::const_iterator& p,
			  const TokensContainer::const_iterator pe)
    {
      this->checkNotEndOfLine("Analyser::treatXRange","",p,pe);
      if(p->value=="auto"){
	this->area.unsetXRange();
	++p;
      } else {
	this->readRange(this->hasXMinValue,this->hasXMaxValue,
			this->xmin,this->xmax,p,pe);
      }
    } // end of Analyser::treatXRange

    void
    Analyser::treatX2Range(TokensContainer::const_iterator& p,
			   const TokensContainer::const_iterator pe)
    {
      if(p->value=="auto"){
	this->area.unsetX2Range();
	++p;
      } else {
	this->readRange(this->hasX2MinValue,this->hasX2MaxValue,
			this->x2min,this->x2max,p,pe);
      }
    } // end of Analyser::treatX2Range

    void
    Analyser::treatYRange(TokensContainer::const_iterator& p,
			  const TokensContainer::const_iterator pe)
    {
      if(p->value=="auto"){
	this->area.unsetYRange();
	++p;
      } else {
	this->readRange(this->hasYMinValue,this->hasYMaxValue,
			this->ymin,this->ymax,p,pe);
      }
    } // end of Analyser::treatYRange

    void
    Analyser::treatY2Range(TokensContainer::const_iterator& p,
			   const TokensContainer::const_iterator pe)
    {
      if(p->value=="auto"){
	this->area.unsetY2Range();
	++p;
      } else {
	this->readRange(this->hasY2MinValue,this->hasY2MaxValue,
			this->y2min,this->y2max,p,pe);
      }
    } // end of Analyser::treatY2Range

    void
    Analyser::treatXLabel(TokensContainer::const_iterator& p,
			  const TokensContainer::const_iterator pe)
    {
      using namespace std;
      this->checkNotEndOfLine("Analyser::treatXLabel",
			      "expected label name",p,pe);
      this->area.setXLabel(this->readString(p,pe));
    } // end of Analyser::treatXLabel

    void
    Analyser::treatX2Label(TokensContainer::const_iterator& p,
			   const TokensContainer::const_iterator pe)
    {
      using namespace std;
      this->checkNotEndOfLine("Analyser::treatX2label",
			      "expected label name",p,pe);
      this->area.setX2Label(this->readString(p,pe));
    } // end of Analyser::treatX2Label

    void
    Analyser::treatYLabel(TokensContainer::const_iterator& p,
			  const TokensContainer::const_iterator pe)
    {
      using namespace std;
      this->checkNotEndOfLine("Analyser::treatYlabel",
			      "expected label name",p,pe);
      this->area.setYLabel(this->readString(p,pe));
    } // end of Analyser::treatYLabel

    void
    Analyser::treatY2Label(TokensContainer::const_iterator& p,
			   const TokensContainer::const_iterator pe)
    {
      using namespace std;
      this->checkNotEndOfLine("Analyser::treatY2label",
			      "expected label name",p,pe);
      this->area.setY2Label(this->readString(p,pe));
    } // end of Analyser::treatY2Label

    void
    Analyser::treatPrint(TokensContainer::const_iterator& p,
			 const TokensContainer::const_iterator pe)
    {
      using namespace std;
      using namespace tfel::utilities;
      using namespace tfel::math;
      std::vector<std::string> vars;
      bool cont = true;
      ostringstream res;
      if(p==pe){
	string msg("Analyser::treatPrint : ");
	msg += "unexpected end of line";
	throw(runtime_error(msg));
      }
      while((p!=pe)&&(cont)){
	if(p->flag==Token::String){
	  res << p->value.substr(1,p->value.size()-2);
	  ++p;
	  if(p!=pe){
	    this->readSpecifiedToken("Analyser::treatPrint : ",
				     ",",p,pe);
	    this->checkNotEndOfLine("Analyser::treatPrint : ","",p,pe);
	  }
	} else {
	  string group = this->readNextGroup(p,pe);
	  if(group.empty()){
	    string msg("Analyser::treatPrint : ");
	    msg += "invalid expression";
	    throw(runtime_error(msg));
	  }
	  shared_ptr<Evaluator> ev(new Evaluator(vars,group,functions));
	  ev->removeDependencies();
	  if(ev->getNumberOfVariables()!=0u){
	    const vector<string>& ev_vars = ev->getVariablesNames();
	    ostringstream msg;
	    if(ev_vars.size()!=1u){
	      msg << "Analyser::treatPrint : invalid print declaration (unknown variables ";
	      copy(ev_vars.begin(),ev_vars.end(),ostream_iterator<string>(msg," "));
	      msg << ")";
	    } else {
	      msg << "Analyser::treatPrint : invalid print declaration (unknown variable "
		  << ev_vars[0] << ")";
	    }
	    throw(runtime_error(msg.str()));
	  }
	  res << ev->getValue();
	  if(p!=pe){
	    this->readSpecifiedToken("Analyser::treatPrint : ",
				     ",",p,pe);
	    this->checkNotEndOfLine("Analyser::treatPrint : ","",p,pe);
	  }
	}
      }
      cout << res.str() << endl;
    } // end of Analyser::treatPrint

    Analyser::~Analyser()
    {} // end of Analyser::~Analyser()

  } // end of  namespace graphics

} // end of namespace tfel

static int in[2];  //< pipe to the child standard output
static int cfd[2]; //< pipe to the child
static int ffd[2]; //< pipe to the father
static int gargc;
static char **gargv;
static pid_t pid;

bool
tgplot_io_handler(Glib::IOCondition)
{
  using namespace std;
  using namespace Gtk;
  using namespace tfel::graphics;
  static Analyser a(gargc,gargv);
  string line;
  cout.precision(15);
  getline(cin,line);
  try{
    a.analyseLine(line,0);
  }
  catch(runtime_error& e){
    cout << "Error while parsing line '" << a.getCurrentLine() << "'" << endl;
    cout << e.what() << endl;
    a.clearCurrentLine();
  }
  write(ffd[1],"OK",2u);    
  return true;
} // end of tgplot_io_handler

static void
tgplot_child(int argc,
	     char **argv)
{
  using namespace std;
  using namespace tfel::graphics;
  using namespace Glib;
  using namespace Gtk;
  Main m(argc,argv);
  signal_io().connect(sigc::ptr_fun(&tgplot_io_handler),STDIN_FILENO,IO_IN);
  m.run();
  ::close(ffd[1]);
} // end of tgplot_child

static void
tgplot_father_addString(std::vector<std::string>& res,
			tfel::utilities::CxxTokenizer::TokensContainer::const_iterator ps,
			tfel::utilities::CxxTokenizer::TokensContainer::const_iterator pe)
{
  using namespace std;
  if(ps!=pe){
    res.push_back(string());
    while(ps!=pe){
      res.back() += ps->value;
      if(++ps!=pe){
	res.back() += ' ';
      }
    }
  }
} // end of tgplot_father_addString

static std::vector<std::string>
tgplot_father_tokenize(const std::string& s,
		       const unsigned short n)
{
  using namespace std;
  using namespace tfel::utilities;
  struct TGTokenizer
    : public CxxTokenizer
  {
    TGTokenizer(const std::string& line,
		const unsigned short nb)
    {
      using namespace std;
      this->treatCharAsString(true);
      this->splitLine(line,nb);
      if(this->cStyleCommentOpened){
	string msg("tgplot_father_tokenize : ");
	msg += "unfinished c-style comment";
	throw(runtime_error(msg));
      }
      this->splitTokens();
      CxxTokenizer::stripComments();
    }
  };
  TGTokenizer tokenizer(s,n);
  CxxTokenizer::TokensContainer::const_iterator p  = tokenizer.begin();
  CxxTokenizer::TokensContainer::const_iterator ps = tokenizer.begin();
  const CxxTokenizer::TokensContainer::const_iterator pe = tokenizer.end();
  vector<string> res;
  while(p!=pe){
    if(p->value=="#"){
      tgplot_father_addString(res,ps,p);
      return res;
    } else if(p->value==";"){
      tgplot_father_addString(res,ps,p);
      ps=p;
      ++ps;
    }
    ++p;
  }
  tgplot_father_addString(res,ps,pe);
  return res;
} // end of tgplot_father_tokenize

static void
tgplot_father_send(const std::string& line,
		   const unsigned short n)
{
  using namespace std;
  vector<string>::const_iterator p;
  ssize_t readChar;
  char buf[3];
  buf[2]='\0';
  vector<string> res;
  try{
    res = tgplot_father_tokenize(line,n);
  } catch(runtime_error& e){
    cerr << "tgplot_father_send : error while parsing line '" 
	 << line << "'\n Error at line " << n << endl;
  }
#ifdef HAVE_READLINE_HISTORY
  ::add_history(line.c_str());
#endif /* HAVE_READLINE_HISTORY */
  for(p=res.begin();p!=res.end();++p){
    if(*p=="pause"){
      getchar();
    } else {
      write(in[1],p->c_str(),strlen(p->c_str()));
      write(in[1],"\n",1u);
      errno=0;
      while((readChar=read(ffd[0],buf,2u))==-1){
      	if(errno!=EINTR)
      	  break;
      }
      if(readChar!=2){
      	exit(EXIT_FAILURE);
      }
    }
  }
} // end of tgplot_father_send

static void
tgplot_father(void)
{
  using namespace std;
  using namespace tfel::system;
  vector<string> inputFiles;
  const char* const * p;
  vector<string>::const_iterator p2;
  unsigned short n;
  // looking for input files
  for(p=gargv+1;p!=gargv+gargc;++p){
    if((*p)[0]!='-'){
      inputFiles.push_back(*p);
    }
  }
  // activating the analyser
  tgplot_father_send("",0);
  n=1;
  // treating input Files
  for(p2=inputFiles.begin();p2!=inputFiles.end();++p2){
    ifstream file(p2->c_str());
    while(!file.eof()){
      string fline;
      getline(file,fline);
      if(!fline.empty()){
	tgplot_father_send(fline.c_str(),n);
      }
      ++n;
    }
  }
  // user inputs
#ifdef HAVE_READLINE
  char *line;
  line = ::readline("tgplot> ");
  while(line){
    ++n;
    if(*line){
      if(strlen(line)!=0){
	tgplot_father_send(line,n);
      }
    }
    ::free(line);
    line = ::readline("tgplot> ");
  }
#else
  string line;
  cout << "tgplot> ";
  if(!cin.eof()){
    getline(cin,line);
  }
  while(!cin.eof()){
    ++n;
    tgplot_father_send(line,n);
    line.clear();
    cout << "tgplot> ";
    getline(cin,line);
  }
#endif /* HAVE_READLINE */
  ::kill(pid,SIGKILL);
  ::close(in[1]);
  ::close(ffd[0]);
} // end of tgplot_father

int
main(int argc,
     char **argv)
{
  using namespace std;
  using namespace tfel::system;

  ssize_t readChar;
  char buf[3];
  buf[2]='\0';
  gargc=argc;
  gargv=argv;
  // creating pipes
  if(pipe(in)==-1){
    string msg("tgplot : pipe creation failed");
    systemCall::throwSystemError(msg,errno);
  }
  if(pipe(ffd)==-1){
    string msg("tgplot : pipe creation failed");
    systemCall::throwSystemError(msg,errno);
  }
  if(pipe(cfd)==-1){
    string msg("tgplot : pipe creation failed");
    systemCall::throwSystemError(msg,errno);
  }
  // forking
  pid=::fork();
  if(pid==-1){
    // fork failed
    // closing the pipes
    ::close(in[0]);
    ::close(in[1]);
    ::close(cfd[0]);
    ::close(cfd[1]);
    ::close(ffd[0]);
    ::close(ffd[1]);
    string msg("tgplot : fork failed");
    systemCall::throwSystemError(msg,errno);
  }
  if(pid==0){
    // the child 
    // we are in the child
    ::close(in[1]);
    ::close(cfd[1]);
    ::close(ffd[0]);
    ::dup2(in[0],STDIN_FILENO);
    // wait that the father has made its administrative job
    while((readChar=read(cfd[0],buf,2u))==-1){
      if(errno!=EINTR)
	break;
    }
    ::close(cfd[0]);
    assert(readChar==2);
    tgplot_child(argc,argv);
  }
  // here we are in the father
  ::close(in[0]);
  ::close(ffd[1]);
  ::close(cfd[0]);
  // registering is made, tell the child
  ::write(cfd[1],"OK",2u);
  ::close(cfd[1]);
  tgplot_father();
  return EXIT_SUCCESS;
} // end of main

