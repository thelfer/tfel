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
#include<dlfcn.h>

#include<gtkmm.h>

#if defined(HAVE_READLINE_READLINE_H)
#  include <readline/readline.h>
#elif defined(HAVE_READLINE_H)
#  include <readline.h>
#else /* !defined(HAVE_READLINE_H) */
#  error "readline is not available"
#endif /* !defined(HAVE_READLINE_H) */

#ifdef HAVE_READLINE_HISTORY
#  if defined(HAVE_READLINE_HISTORY_H)
#    include <readline/history.h>
#  elif defined(HAVE_HISTORY_H)
#    include <history.h>
#  endif /* defined(HAVE_READLINE_HISTORY_H) */
#endif /* HAVE_READLINE_HISTORY */

#include"System/System.hxx"
#include"System/SignalManager.hxx"
#include"System/ProcessManager.hxx"
#include"Math/Parser/ExternalFunctionManager.hxx"
#include"Math/Parser/ExternalCFunction.hxx"
#include"Math/Parser/ExternalCastemFunction.hxx"
#ifdef HAVE_OCTAVE
#include"Math/Parser/ExternalOctaveFunction.hxx"
#endif /* HAVE_OCTAVE */
#include"Graphics/GetFunction.h"
#include"Graphics/Curve.hxx"
#include"Graphics/EvaluatedCurve.hxx"
#include"Graphics/DataCurve.hxx"
#include"Graphics/ThemeManager.hxx"
#include"Graphics/Analyser.hxx"
#include"Graphics/Grid.hxx"

#ifdef HAVE_GSL
#include"Math/Parser/EvaluatorGSLWrapper.hxx"
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
	  axis(Graph::XY)
      {} // end of CurveOptions()
      std::string title;
      std::string using_decl_x;
      std::string using_decl_y;
      Curve::Style style;
      Graph::GraphAxis axis;
    }; // end of struct Analyser::CurveOptions

    Analyser::Analyser(int argc,
		       char **argv)
      : tfel::utilities::ArgumentParserBase<Analyser>(argc,argv),
	PlotWindow(),
	functions(new tfel::math::parser::ExternalFunctionManager()),
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
      this->parseArguments();    
      this->treatCharAsString(true);
      this->functions->insert(MVType("pi",
				     SmartPtr<ExternalFunction>(new Evaluator("3.14159265358979323846"))));
      this->locks.insert("pi");
    } // end of Analyser::Analyser()

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
    Analyser::treatUnknownArgument(void)
    {
      using namespace std;
      using namespace tfel::utilities;
      typedef std::map<string,SmartPtr<TextData> >::value_type MVType;
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
      m.insert(MVType(name,f));      
    } // end of Analyser::registerCallBack

    void
    Analyser::registerCallBack2(std::map<std::string,Analyser::MemFuncPtr2>& m,
				const std::string& name,
				const Analyser::MemFuncPtr2 f)
    {
      using namespace std;
      typedef std::map<string,MemFuncPtr2>::value_type MVType;
      m.insert(MVType(name,f));      
    } // end of Analyser::registerCallBack

    void
    Analyser::registerCallBacks()
    {
      this->registerCallBack(this->callBacks,"q",&Analyser::treatQuit);
      this->registerCallBack(this->callBacks,"quit",&Analyser::treatQuit);
      this->registerCallBack(this->callBacks,"reset",&Analyser::treatReset);
      this->registerCallBack(this->callBacks,"se",&Analyser::treatSet);
      this->registerCallBack(this->callBacks,"set",&Analyser::treatSet);
      this->registerCallBack(this->callBacks,"unset",&Analyser::treatUnset);
      this->registerCallBack(this->callBacks,"import",&Analyser::treatImport);
      this->registerCallBack(this->callBacks,"plot",&Analyser::treatPlot);
      this->registerCallBack(this->callBacks,"print",&Analyser::treatPrint);
      this->registerCallBack(this->callBacks,"const",&Analyser::treatConst);
      this->registerCallBack(this->callBacks,"lock",&Analyser::treatLock);
      this->registerCallBack(this->callBacks,"nodeps",&Analyser::treatNoDeps);
      this->registerCallBack(this->callBacks,"include",&Analyser::treatInclude);
      this->registerCallBack(this->callBacks,"replot",&Analyser::treatRePlot);
#ifdef HAVE_GSL
      this->registerCallBack(this->setCallBacks,"gsl",&Analyser::treatSetGSL);
#endif
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
      this->registerCallBack(this->unsetCallBacks,"xtics",&Analyser::treatUnsetXTics);
      this->registerCallBack(this->unsetCallBacks,"x2tics",&Analyser::treatUnsetX2Tics);
      this->registerCallBack(this->unsetCallBacks,"ytics",&Analyser::treatUnsetYTics);
      this->registerCallBack(this->unsetCallBacks,"y2tics",&Analyser::treatUnsetY2Tics);
      this->registerCallBack(this->unsetCallBacks,"grid",&Analyser::treatUnsetGrid);
      this->registerCallBack2(this->plotCallBacks,"t",&Analyser::treatPlotTitle);
      this->registerCallBack2(this->plotCallBacks,"title",&Analyser::treatPlotTitle);
      this->registerCallBack2(this->plotCallBacks,"w",&Analyser::treatPlotWith);
      this->registerCallBack2(this->plotCallBacks,"with",&Analyser::treatPlotWith);
      this->registerCallBack2(this->plotCallBacks,"using",&Analyser::treatPlotUsing);
      this->registerCallBack2(this->plotCallBacks,"u",&Analyser::treatPlotUsing);
      this->registerCallBack2(this->plotCallBacks,"axes",&Analyser::treatPlotAxes);
      this->registerCallBack2(this->plotCallBacks,"axis",&Analyser::treatPlotAxes);
    } // end of Analyser::registerCallBacks()
    
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
    
    tfel::utilities::SmartPtr<tfel::math::Evaluator>
    Analyser::readFunction(TokensContainer::const_iterator& p, 
			   const TokensContainer::const_iterator pe)
    {
      using namespace std;
      using namespace tfel::math;
      using namespace tfel::utilities;
      vector<string> vars;
      string f;
      unsigned short openedParenthesis = 0;
      bool cont=true;
      TokensContainer::const_iterator prev = p;
      TokensContainer::const_iterator pb   = p;
      while((p!=pe)&&(cont)){
	if((p->value==",")&&(openedParenthesis==0)){
	  cont = false;
	} else if(p->value=="("){
	  ++openedParenthesis;
	  f += p->value;
	  prev=p;
	  ++p;
	} else if(p->value==")"){
	  if(openedParenthesis==0){
	    string msg("Analyser::treatFunctionPlot : ");
	    msg += "unbalanced parenthesis";
	    throw(runtime_error(msg));
	  }
	  --openedParenthesis;
	  f += p->value;
	  prev=p;
	  ++p;
	} else {
	  if((openedParenthesis==0)&&(prev!=pb)){
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
	}
      }
      if(openedParenthesis!=0){
	string msg("Analyser::readFunction : ");
	msg += "unmatched parenthesis";
	throw(runtime_error(msg));
      }
      return SmartPtr<Evaluator>(new Evaluator(vars,f,this->functions));
    } // end of Analyser::readFunction

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
      unsigned short openedParenthesis;
      if((!options.using_decl_x.empty())||
	 (!options.using_decl_y.empty())){
	string msg("Analyser::treatPlotUsing : ");
	msg += "using option already specified.";
	throw(runtime_error(msg));
      }
      this->checkNotEndOfLine("Analyser::treatPlotUsing",
			      "expected using declaration",p,pe);
      if(p->value=="("){
	++p;
	this->checkNotEndOfLine("Analyser::treatPlotUsing","",p,pe);
	openedParenthesis = 0;
	while(!((p->value==")")&&(openedParenthesis==0))){
	  if(p->value=="("){
	    ++openedParenthesis;
	  }
	  if(p->value==")"){
	    if(openedParenthesis==0){
	      string msg("Analyser::treatPlotUsing : ");
	      msg += "unbalanced parenthesis";
	      throw(runtime_error(msg));
	    }
	    --openedParenthesis;
	  }
	  options.using_decl_x += p->value;
	  ++p;
	  this->checkNotEndOfLine("Analyser::treatPlotUsing","",p,pe);
	}
	++p;
      } else {
	// this shall be a column number
	if(!Analyser::isUnsignedInteger(p->value)){
	  string msg("Analyser::treatPlotUsing : ");
	  msg += "unexpected token '"+p->value+"', expected column number";
	  throw(runtime_error(msg));
	}
	options.using_decl_x = p->value;
	++p;
      }
      this->readSpecifiedToken("Analyser::treatPlotUsing",":",p,pe);
      this->checkNotEndOfLine("Analyser::treatPlotUsing","",p,pe);
      if(p->value=="("){
	++p;
	this->checkNotEndOfLine("Analyser::treatPlotUsing","",p,pe);
	openedParenthesis = 0;
	while(!((p->value==")")&&(openedParenthesis==0))){
	  if(p->value=="("){
	    ++openedParenthesis;
	  }
	  if(p->value==")"){
	    if(openedParenthesis==0){
	      string msg("Analyser::treatPlotUsing : ");
	      msg += "unbalanced parenthesis";
	      throw(runtime_error(msg));
	    }
	    --openedParenthesis;
	  }
	  options.using_decl_y += p->value;
	  ++p;
	  this->checkNotEndOfLine("Analyser::treatPlotUsing","",p,pe);
	}
	++p;
      } else {
	// this shall be a column number
	if(!Analyser::isUnsignedInteger(p->value)){
	  string msg("Analyser::treatPlotUsing : ");
	  msg += "unexpected token '"+p->value+"', expected column number";
	  throw(runtime_error(msg));
	}
	options.using_decl_y = p->value;
	++p;
      }
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
    Analyser::treatQuit(TokensContainer::const_iterator&, 
			const TokensContainer::const_iterator)
    {
      ::exit(EXIT_SUCCESS);
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
      SmartPtr<Evaluator> ev(new Evaluator(vars,f,this->functions));
      ev->removeDependencies();
      const CurveOptions& options = this->treatPlotOptions(p,pe);
      if((!options.using_decl_x.empty())||
	 (!options.using_decl_y.empty())){
	string msg("Analyser::treatFunctionPlot : ");
	msg += "the 'using' keyword is invalid for functions";
	throw(runtime_error(msg));
      }
      SmartPtr<Curve> c(new EvaluatedCurve(ev,this->dummyVariable));
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
    Analyser::applyCurveOptions(tfel::utilities::SmartPtr<Curve> c,
				const Analyser::CurveOptions& options)
    {
      if(!options.title.empty()){
	c->setLegend(options.title);
      }
      c->setStyle(options.style);
    } // end of Analyser::applyCurveOptions

    bool
    Analyser::isUnsignedInteger(const std::string& s)
    {
      using namespace std;
      string::const_iterator p;
      for(p=s.begin();p!=s.end();++p){
	if(!isdigit(*p)){
	  return false;
	}
      }
      return true;
    } // end of Analyser::isUnsignedInteger

    double
    Analyser::readDouble(const std::string& s,
			 const unsigned short l)
    {
      using namespace std;
      double res;
      istringstream is(s);
      is >> res;
      if(!is&&(!is.eof())){
	ostringstream msg;
	msg << "Analyser::readDouble : ";
	msg << "could not read value from token '"+s+"'.\n";
	msg << "Error at line : " << l;
	throw(runtime_error(msg.str()));
      }
      return res;
    } // end of Analyser::readDouble

    std::string
    Analyser::getData(std::vector<double>& v,
		      const tfel::utilities::TextData& fdata,
		      const std::string& s)
    {
      using namespace std;
      using namespace tfel::math;
      using namespace tfel::utilities;
      if(Analyser::isUnsignedInteger(s)){
	unsigned short c = this->convertToUnsignedShort(s);
	fdata.getColumn(c,v);
	return fdata.getLegend(c);
      } else {
	// assuming a function
	vector<pair<string,unsigned short> > vars;
	Evaluator e(s,this->functions);
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
	  if(((*p)[0]!='$')){
	    parser::ExternalFunctionManager::const_iterator p4;
	    p4 = this->functions->find(*p);
	    if(p4==this->functions->end()){
	      string msg("Analyser::getData : ");
	      msg += "invalid variable '"+*p+"'";
	      throw(runtime_error(msg));
	    }
	    if(p4->second->getNumberOfVariables()!=0){
	      string msg("Analyser::getData : ");
	      msg += "invalid variable '"+*p+"'";
	      throw(runtime_error(msg));
	    }
	    e.setVariableValue(*p,p4->second->getValue());
	  } else {
	    if(!Analyser::isUnsignedInteger(p->substr(1))){
	      string msg("Analyser::getData : ");
	      msg += "invalid variable name '"+*p;
	      msg += "' in function '"+s+"'";
	      throw(runtime_error(msg));
	    }
	    const unsigned short vc = this->convertToUnsignedShort(p->substr(1));
	    if(vc==0){
	      string msg("Analyser::getData : ");
	      msg += "invalid variable name "+*p;
	      msg += " in function '"+s+"'.";
	      throw(runtime_error(msg));
	    }
	    vars.push_back(make_pair(*p,vc));
	  }
	}
	for(p2=fdata.begin();p2!=fdata.end();++p2){
	  for(p3=vars.begin();p3!=vars.end();++p3){
	    if(p2->tokens.size()<p3->second){
	      ostringstream msg;
	      msg << "TextData::getColumn : line '" 
		  << p2->nbr << "' "
		  << "does not have '" << p3->second << "' columns.";
	      throw(runtime_error(msg.str()));
	    }
	    e.setVariableValue(p3->first,
			       Analyser::readDouble(p2->tokens[p3->second-1],
						    p2->nbr));
	  }
	  v.push_back(e.getValue());
	}
      }
      return "";
    } // end of Analyser::getData

    void
    Analyser::treatDataPlot(TokensContainer::const_iterator& p, 
			    const TokensContainer::const_iterator pe)
    {
      using namespace std;
      using namespace tfel::utilities;
      typedef std::map<string,SmartPtr<TextData> >::value_type MVType;
      std::map<string,SmartPtr<TextData> >::const_iterator p2;
      vector<double> vx;
      vector<double> vy;
      string file = this->readString(p,pe);
      string x;
      string y;
      const CurveOptions& options = this->treatPlotOptions(p,pe);
      if(this->dataSources.find(file)==this->dataSources.end()){
	this->dataSources.insert(MVType(file,
					SmartPtr<TextData>(new TextData(file))));
      }
      p2 = this->dataSources.find(file);
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
      this->getData(vx,*(p2->second),x);
      const std::string& ty = this->getData(vy,*(p2->second),y);
      SmartPtr<Curve> c(new DataCurve(vx,vy));
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
      using namespace std;
      this->checkNotEndOfLine("Analyser::treatSet","",p,pe);
      std::map<string,MemFuncPtr>::const_iterator ps;
      ps = this->setCallBacks.find(p->value);
      if(ps == this->setCallBacks.end()){
	string msg("Analyser::treatSet : ");
	msg += "unknown option '"+p->value+"'";
	throw(runtime_error(msg));
      }
      ++p;
      (this->*(ps->second))(p,pe);
      this->area.queue_draw();
    } // end of Analyser::treatSet

    void
    Analyser::treatUnset(TokensContainer::const_iterator& p,
			 const TokensContainer::const_iterator pe)
    {
      using namespace std;
      this->checkNotEndOfLine("Analyser::treatUnset","",p,pe);
      std::map<string,MemFuncPtr>::const_iterator ps;
      ps = this->unsetCallBacks.find(p->value);
      if(ps == this->unsetCallBacks.end()){
	string msg("Analyser::treatUnset : ");
	msg += "unknown option '"+p->value+"'";
	throw(runtime_error(msg));
      }
      ++p;
      (this->*(ps->second))(p,pe);
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

    std::vector<std::string>
    Analyser::readVariableList(TokensContainer::const_iterator& p,
			       const TokensContainer::const_iterator pe)
    {
      using namespace std;
      vector<string> vars;
      if(p==pe){
	string msg("Analyser::readVariableList : ");
	msg += "unexpected end of line (expected ')').";
	throw(runtime_error(msg));
      }
      if(p->value!="("){
	string msg("Analyser::readVariableList : ");
	msg += "unexpected token (read '"+p->value+", 'expected ')').";
	throw(runtime_error(msg));
      }
      ++p;
      if(p==pe){
	string msg("Analyser::readVariableList : ");
	msg += "unexpected end of line";
	throw(runtime_error(msg));
      }
      if(!this->isValidIdentifier(p->value)){
	string msg("Analyser::readVariableList : ");
	msg += p->value+" is not a valid identifer.";
	throw(runtime_error(msg));
      }
      vars.push_back(p->value);
      ++p;
      if(p==pe){
	string msg("Analyser::readVariableList : ");
	msg += "unexpected end of line";
	throw(runtime_error(msg));
      }
      while(p->value==","){
	++p;
	if(p==pe){
	  string msg("Analyser::readVariableList : ");
	  msg += "unexpected end of line";
	  throw(runtime_error(msg));
	}
	if(!this->isValidIdentifier(p->value)){
	  string msg("Analyser::readVariableList : ");
	  msg += p->value+" is not a valid variable name.";
	  throw(runtime_error(msg));
	}
	vars.push_back(p->value);
	++p;
	if(p==pe){
	  string msg("Analyser::readVariableList : ");
	  msg += "unexpected end of line";
	  throw(runtime_error(msg));
	}
      }
      if(p->value!=")"){
	string msg("Analyser::readVariableList : ");
	msg += "unexpected token '"+p->value+"' (expected ')')";
	throw(runtime_error(msg));
      }
      ++p;
      return vars;
    } // end of Analyser::readVariableList

    void
    Analyser::treatImport(TokensContainer::const_iterator& p,
			  const TokensContainer::const_iterator pe,
			  const bool b)
    {
      using namespace std;
      ImportOptions options;
      if(p==pe){
	string msg("Analyser::treatImport : ");
	msg += "unexpected end of line";
	throw(runtime_error(msg));
      }
      if(p->value=="<"){
	++p;
	if(p==pe){
	  string msg("Analyser::treatImport : ");
	  msg += "unexpected end of line";
	  throw(runtime_error(msg));
	}
	// import options
	while(p->value!=">"){
	  if(p->value=="function"){
	    if(options.type!=ImportOptions::Default){
	      string msg("Analyser::treatImport : ");
	      msg += "import type has already been specified";
	      throw(runtime_error(msg));
	    }
	    options.type=ImportOptions::Function;
	  } else if (p->value=="c"){
	    if(!((options.type==ImportOptions::Default)||
		 (options.type==ImportOptions::Function))){
	      string msg("Analyser::treatImport : ");
	      msg += "import type is neither 'Default' nor 'Function', ";
	      msg += "which is required for the 'c' option";
	      throw(runtime_error(msg));
	    }
	    if(options.functionType!=ImportOptions::FDefault){
	      string msg("Analyser::treatImport : ");
	      msg += "a function type has already been specified.";
	      throw(runtime_error(msg));
	    }
	    options.type=ImportOptions::Function;
	    options.functionType=ImportOptions::C;
	  } else if (p->value=="castem"){
	    if(!((options.type==ImportOptions::Default)||
		 (options.type==ImportOptions::Function))){
	      string msg("Analyser::treatImport : ");
	      msg += "import type is neither 'Default' nor 'Function', ";
	      msg += "which is required for the 'castem' option";
	      throw(runtime_error(msg));
	    }
	    if(options.functionType!=ImportOptions::FDefault){
	      string msg("Analyser::treatImport : ");
	      msg += "a function type has already been specified.";
	      throw(runtime_error(msg));
	    }
	    options.type=ImportOptions::Function;
	    options.functionType=ImportOptions::Castem;
#ifdef HAVE_OCTAVE 
	  } else if (p->value=="octave"){
	    if(!((options.type==ImportOptions::Default)||
		 (options.type==ImportOptions::Function))){
	      string msg("Analyser::treatImport : ");
	      msg += "import type is neither 'Default' nor 'Function', ";
	      msg += "which is required for the 'octave' option";
	      throw(runtime_error(msg));
	    }
	    if(options.functionType!=ImportOptions::FDefault){
	      string msg("Analyser::treatImport : ");
	      msg += "a function type has already been specified.";
	      throw(runtime_error(msg));
	    }
	    options.type=ImportOptions::Function;
	    options.functionType=ImportOptions::Octave;
#endif /* HAVE_OCTAVE */
	  } else if(p->value=="data"){
	    if(options.functionType!=ImportOptions::FDefault){
	      string msg("Analyser::treatImport : ");
	      msg += "a function type has been specified, which is inconsistent with 'data' options";
	      throw(runtime_error(msg));
	    }
	    if(options.type!=ImportOptions::Default){
	      string msg("Analyser::treatImport : ");
	      msg += "import type has already been specified";
	      throw(runtime_error(msg));
	    }
	    options.type=ImportOptions::Data;
	  } else {
	    // must be the number of variables
	    unsigned short nbr;
	    try{
	      nbr = this->convertToUnsignedShort(p->value);
	    } catch(runtime_error& e){
	      string msg("Analyser::treatImport : ");
	      msg += "unknown option '"+p->value+"'";
	      throw(runtime_error(msg));
	    }
	    if(options.numberOfVariables!=-1){
	      string msg("Analyser::treatImport : ");
	      msg += "the number of variables has alread been specified";
	      throw(runtime_error(msg));
	    }
	    options.numberOfVariables = static_cast<int>(nbr);
	  }
	  ++p;
	  if(p==pe){
	    string msg("Analyser::treatImport : ");
	    msg += "unexpected end of line";
	    throw(runtime_error(msg));
	  }
	  if((p->value!=">")&&
	     (p->value!=",")){
	    string msg("Analyser::treatImport : ");
	    msg += "unexpected token '"+p->value+"', expected ',' or '>'";
	    throw(runtime_error(msg));
	  }
	  if(p->value==","){
	    ++p;
	  }
	}
	++p;	
      }
      if((options.type==ImportOptions::Default)||
	 (options.type==ImportOptions::Function)){
	this->importFunction(p,pe,options,b);
      } else {
	string msg("Analyser::treatImport : ");
	msg += "only function import is supported";
	throw(runtime_error(msg));
      }
    } // end of Analyser::importFunction

#ifdef HAVE_OCTAVE
    void
    Analyser::importOctaveFunction(TokensContainer::const_iterator&, 
				   const TokensContainer::const_iterator,
				   const std::string& f,
				   const unsigned short nbr,
				   const bool b)
    {
      using namespace std;
      using namespace tfel::utilities;
      using namespace tfel::math::parser;
      typedef SmartPtr<ExternalFunction> EFunctionPtr;
      this->addFunction(f,EFunctionPtr(new ExternalOctaveFunction(f,nbr)),b,false);
    } // end of Graph::importOctaveFunction
#endif /* HAVE_OCTAVE*/
    
    void
    Analyser::treatImport(TokensContainer::const_iterator& p,
			  const TokensContainer::const_iterator pe)
    {
      this->treatImport(p,pe,false);
    } // end of Analyser::treatImport

    unsigned short
    Analyser::convertToUnsignedShort(const std::string& value)
    {
      using namespace std;
      string::const_iterator p;
      istringstream converter(value);
      for(p=value.begin();p!=value.end();++p){
	if(!isdigit(*p)){
	  throw(runtime_error("Analyser::convertToUnsignedShort : invalid entry"));
	}
      }
      unsigned short u;
      converter >> u;
      if(!converter&&(!converter.eof())){
	string msg("Analyser::convertToUnsignedShort : ");
	msg += "not read value from token '"+value+"'.\n";
	throw(runtime_error(msg));
      }
      return u;
    } // end of Analyser::convertToUnsignedShort

    void
    Analyser::importCastemFunction(const std::string& function,
				   const std::string& library,
				   const unsigned short varNumber,
				   void * const lib,
				   const bool b)
    {
      using namespace std;
      using namespace tfel::utilities;
      using namespace tfel::math::parser;
      typedef SmartPtr<ExternalFunction> EFunctionPtr;
      double (*func)(const double* const);
      int status = ::checkCastemFunction(lib,function.c_str(),
					 varNumber);
      if(status==-2){
	string msg("Analyser::importCastemFunction : ");
	msg += "call to checkCastemFunction failed.";
	throw(runtime_error(msg));
      }
      if(status==-1){
	string msg("Analyser::importCastemFunction : ");
	msg += "the symbol '"+function+"_nargs' is not available in library "+library;
	throw(runtime_error(msg));
      }
      if(status!=0){
	ostringstream msg;
	msg << "Analyser::importCastemFunction : "
	    << "the function '" << function << "' declares "
	    << status << " variables, not " << varNumber;
	throw(runtime_error(msg.str()));
      }
      func = ::getCastemFunction(lib,function.c_str());
      if(func==0){
	string msg("Analyser::importCastemFunction : ");
	msg += "unable to load function '"+function+"' from library '"+library+"'";
	throw(runtime_error(msg));
      }
      this->addFunction(function,EFunctionPtr(new ExternalCastemFunction(func,varNumber)),b,false);
    } // end of Analyser::importCastemFunction

    void
    Analyser::importCFunction(const std::string& function,
			      const std::string& library,
			      const unsigned short varNumber,
			      void * const lib,
			      const bool b)
    {
      using namespace std;
      using namespace tfel::utilities;
      using namespace tfel::math::parser;
      typedef SmartPtr<ExternalFunction> EFunctionPtr;
      switch (varNumber){
	double (*func0)(void);
	double (*func1)(double);
	double (*func2)(double,double);
	double (*func3)(double,double,double);
	double (*func4)(double,double,double,double);
	double (*func5)(double,double,double,double,double);
	double (*func6)(double,double,double,
			double,double,double);
	double (*func7)(double,double,double,
			double,double,double,
			double);
	double (*func8)(double,double,double,
			double,double,double,
			double,double);
	double (*func9)(double,double,double,
			double,double,double,
			double,double,double);
	double (*func10)(double,double,double,
			 double,double,double,
			 double,double,double,
			 double);
	double (*func11)(double,double,double,
			 double,double,double,
			 double,double,double,
			 double,double);
	double (*func12)(double,double,double,
			 double,double,double,
			 double,double,double,
			 double,double,double);
	double (*func13)(double,double,double,
			 double,double,double,
			 double,double,double,
			 double,double,double,
			 double);
	double (*func14)(double,double,double,
			 double,double,double,
			 double,double,double,
			 double,double,double,
			 double,double);
	double (*func15)(double,double,double,
			 double,double,double,
			 double,double,double,
			 double,double,double,
			 double,double,double);
      case 0:
	func0= ::getCFunction0(lib,function.c_str());
	if(func0==0){
	  string msg("Analyser::importCFunction : ");
	  msg += "unable to load function '"+function+"' from library '"+library+"'";
	  msg += " (";
	  msg += ::dlerror();
	  msg += ")";
	  throw(runtime_error(msg));
	}
	this->addFunction(function,EFunctionPtr(new ExternalCFunction<0>(func0)),b,false);
	break;
      case 1:
	func1 = ::getCFunction1(lib,function.c_str());
	if(func1==0){
	  string msg("Analyser::importCFunction : ");
	  msg += "unable to load function '"+function+"' from library '"+library+"'";
	  msg += " (";
	  msg += ::dlerror();
	  msg += ")";
	  throw(runtime_error(msg));
	}
	this->addFunction(function,EFunctionPtr(new ExternalCFunction<1>(func1)),b,false);
	break;
      case 2:
	func2 = ::getCFunction2(lib,function.c_str());
	if(func2==0){
	  string msg("Analyser::importCFunction : ");
	  msg += "unable to load function '"+function+"' from library '"+library+"'";
	  msg += " (";
	  msg += ::dlerror();
	  msg += ")";
	  throw(runtime_error(msg));
	}
	this->addFunction(function,EFunctionPtr(new ExternalCFunction<2>(func2)),b,false);
	break;
      case 3:
	func3 = ::getCFunction3(lib,function.c_str());
	if(func3==0){
	  string msg("Analyser::importCFunction : ");
	  msg += "unable to load function '"+function+"' from library '"+library+"'";
	  msg += " (";
	  msg += ::dlerror();
	  msg += ")";
	  throw(runtime_error(msg));
	}
	this->addFunction(function,EFunctionPtr(new ExternalCFunction<3>(func3)),b,false);
	break;
      case 4:
	func4 = ::getCFunction4(lib,function.c_str());
	if(func4==0){
	  string msg("Analyser::importCFunction : ");
	  msg += "unable to load function '"+function+"' from library '"+library+"'";
	  msg += " (";
	  msg += ::dlerror();
	  msg += ")";
	  throw(runtime_error(msg));
	}
	this->addFunction(function,EFunctionPtr(new ExternalCFunction<4>(func4)),b,false);
	break;
      case 5:
	func5 = ::getCFunction5(lib,function.c_str());
	if(func5==0){
	  string msg("Analyser::importCFunction : ");
	  msg += "unable to load function '"+function+"' from library '"+library+"'";
	  msg += " (";
	  msg += ::dlerror();
	  msg += ")";
	  throw(runtime_error(msg));
	}
	this->addFunction(function,EFunctionPtr(new ExternalCFunction<5>(func5)),b,false);
	break;
      case 6:
	func6 = ::getCFunction6(lib,function.c_str());
	if(func6==0){
	  string msg("Analyser::importCFunction : ");
	  msg += "unable to load function '"+function+"' from library '"+library+"'";
	  msg += " (";
	  msg += ::dlerror();
	  msg += ")";
	  throw(runtime_error(msg));
	}
	this->addFunction(function,EFunctionPtr(new ExternalCFunction<6>(func6)),b,false);
	break;
      case 7:
	func7 = ::getCFunction7(lib,function.c_str());
	if(func7==0){
	  string msg("Analyser::importCFunction : ");
	  msg += "unable to load function '"+function+"' from library '"+library+"'";
	  msg += " (";
	  msg += ::dlerror();
	  msg += ")";
	  throw(runtime_error(msg));
	}
	this->addFunction(function,EFunctionPtr(new ExternalCFunction<7>(func7)),b,false);
	break;
      case 8:
	func8 = ::getCFunction8(lib,function.c_str());
	if(func8==0){
	  string msg("Analyser::importCFunction : ");
	  msg += "unable to load function '"+function+"' from library '"+library+"'";
	  msg += " (";
	  msg += ::dlerror();
	  msg += ")";
	  throw(runtime_error(msg));
	}
	this->addFunction(function,EFunctionPtr(new ExternalCFunction<8>(func8)),b,false);
	break;
      case 9:
	func9 = ::getCFunction9(lib,function.c_str());
	if(func9==0){
	  string msg("Analyser::importCFunction : ");
	  msg += "unable to load function '"+function+"' from library '"+library+"'";
	  msg += " (";
	  msg += ::dlerror();
	  msg += ")";
	  throw(runtime_error(msg));
	}
	this->addFunction(function,EFunctionPtr(new ExternalCFunction<9>(func9)),b,false);
	break;
      case 10:
	func10 = ::getCFunction10(lib,function.c_str());
	if(func10==0){
	  string msg("Analyser::importCFunction : ");
	  msg += "unable to load function '"+function+"' from library '"+library+"'";
	  msg += " (";
	  msg += ::dlerror();
	  msg += ")";
	  throw(runtime_error(msg));
	}
	this->addFunction(function,EFunctionPtr(new ExternalCFunction<10>(func10)),b,false);
	break;
      case 11:
	func11 = ::getCFunction11(lib,function.c_str());
	if(func11==0){
	  string msg("Analyser::importCFunction : ");
	  msg += "unable to load function '"+function+"' from library '"+library+"'";
	  msg += " (";
	  msg += ::dlerror();
	  msg += ")";
	  throw(runtime_error(msg));
	}
	this->addFunction(function,EFunctionPtr(new ExternalCFunction<11>(func11)),b,false);
	break;
      case 12:
	func12 = ::getCFunction12(lib,function.c_str());
	if(func12==0){
	  string msg("Analyser::importCFunction : ");
	  msg += "unable to load function '"+function+"' from library '"+library+"'";
	  msg += " (";
	  msg += ::dlerror();
	  msg += ")";
	  throw(runtime_error(msg));
	}
	this->addFunction(function,EFunctionPtr(new ExternalCFunction<12>(func12)),b,false);
	break;
      case 13:
	func13 = ::getCFunction13(lib,function.c_str());
	if(func13==0){
	  string msg("Analyser::importCFunction : ");
	  msg += "unable to load function '"+function+"' from library '"+library+"'";
	  msg += " (";
	  msg += ::dlerror();
	  msg += ")";
	  throw(runtime_error(msg));
	}
	this->addFunction(function,EFunctionPtr(new ExternalCFunction<13>(func13)),b,false);
	break;
      case 14:
	func14 = ::getCFunction14(lib,function.c_str());
	if(func14==0){
	  string msg("Analyser::importCFunction : ");
	  msg += "unable to load function '"+function+"' from library '"+library+"'";
	  msg += " (";
	  msg += ::dlerror();
	  msg += ")";
	  throw(runtime_error(msg));
	}
	this->addFunction(function,EFunctionPtr(new ExternalCFunction<14>(func14)),b,false);
	break;
      case 15:
	func15 = ::getCFunction15(lib,function.c_str());
	if(func15==0){
	  string msg("Analyser::importCFunction : ");
	  msg += "unable to load function '"+function+"' from library '"+library+"'";
	  msg += " (";
	  msg += ::dlerror();
	  msg += ")";
	  throw(runtime_error(msg));
	}
	this->addFunction(function,EFunctionPtr(new ExternalCFunction<15>(func15)),b,false);
	break;
      default:
	string msg("Analyser::importCFunction : function with more than 15 variables are not allowed.");
	throw(runtime_error(msg));
      }
    } // end of Analyser::importCFunction

    void
    Analyser::importFunction(TokensContainer::const_iterator& p,
			     const TokensContainer::const_iterator pe,
			     const Analyser::ImportOptions options,
			     const bool b)
    {
      using namespace std;
      this->checkNotEndOfLine("Analyser::importFunction : ",
			      "expected function name",p,pe);
      string function = p->value;
      ++p;
      unsigned short varNumber = 0;
      if(options.numberOfVariables==-1){
	this->checkNotEndOfLine("Analyser::importFunction : ",
				"expected variable list",p,pe);
	if(p->value=="("){
	  const std::vector<std::string>& vars = this->readVariableList(p,pe);
	  varNumber = static_cast<unsigned short>(vars.size());
	} else {
	  varNumber = 0u;
	}
      } else {
	if(p!=pe){
	  if(p->value=="("){
	    const std::vector<std::string>& vars = this->readVariableList(p,pe);
	    if(vars.size()!=static_cast<unsigned short>(options.numberOfVariables)){
	      string msg("Analyser::importFunction : ");
	      msg += "the number variables of function '"+function+"'";
	      msg += "is not the same as that specified in the options";
	      throw(runtime_error(msg));
	    }
	  }
	}
	varNumber = static_cast<unsigned short>(options.numberOfVariables);
      }
#ifdef HAVE_OCTAVE 
      if(options.functionType==ImportOptions::Octave){
	this->importOctaveFunction(p,pe,function,varNumber,b);
      } else {
#endif /* HAVE_OCTAVE */
	this->checkNotEndOfLine("Analyser::importFunction : ",
				"expected library name",p,pe);
	string library = this->readString(p,pe);
	// adding the new function
	void * lib = this->openLibrary(library);
	if((options.functionType==ImportOptions::FDefault)||
	   (options.functionType==ImportOptions::C)){
	  this->importCFunction(function,library,varNumber,lib,b);
	} else if (options.functionType==ImportOptions::Castem){
	  this->importCastemFunction(function,library,varNumber,lib,b);
	} else {
	  string msg("Analyser::importFunction : ");
	  msg += "unknown function type";
	  throw(runtime_error(msg));
	}
#ifdef HAVE_OCTAVE 
      }
#endif /* HAVE_OCTAVE */
    } // end of Analyser::importFunction()
      
    void*
    Analyser::openLibrary(const std::string& library)
    {
      using namespace std;
      if(this->libraries.find(library)==this->libraries.end()){
	void *lib = dlopen(library.c_str(),RTLD_NOW);
	if(lib==0){
	  string msg("Analyser::openLibrary : library ");
	  msg += library;
	  msg += " could not be loaded, (";
	  msg += dlerror();
	  msg += ")";
	  throw(runtime_error(msg));
	}
	this->libraries[library] = lib;
      }
      return this->libraries[library];
    } // end of Analyser::openLibrary

    std::string
    Analyser::readUntil(TokensContainer::const_iterator& p,
			const TokensContainer::const_iterator pe,
			const std::string& delim)
    {
      using namespace std;
      string res;
      unsigned short openedParenthesis=0;
      this->checkNotEndOfLine("Analyser::readUntil","",p,pe);
      while((p!=pe)&&(p->value!=delim)){
	if(p->value=="("){
	  ++openedParenthesis;
	}
	if(p->value==")"){
	  if(openedParenthesis==0){
	    string msg("Analyser::readUntil : ");
	    msg += "unbalanced parenthesis";
	    throw(runtime_error(msg));
	  }
	  --openedParenthesis;
	}
	res += p->value;
	++p;
      }
      if(p->value!=delim){
	string msg("Analyser::readUntil : ");
	msg += "did not find token '"+delim+"'";
	throw(runtime_error(msg));
      }
      if(openedParenthesis!=0){
	string msg("Analyser::readUntil : ");
	msg += "unclosed parenthesis";
	throw(runtime_error(msg));
      }
      ++p;
      return res;
    } // end of Analyser::readUntil

    std::string
    Analyser::readNextGroup(TokensContainer::const_iterator& p,
			    const TokensContainer::const_iterator pe)
    {
      using namespace std;
      string res;
      unsigned short openedParenthesis=0;
      this->checkNotEndOfLine("Analyser::readNextGroup","",p,pe);
      while((p!=pe)&&(!((p->value==",")&&(openedParenthesis==0)))){
	if(p->value=="("){
	  ++openedParenthesis;
	}
	if(p->value==")"){
	  if(openedParenthesis==0){
	    string msg("Analyser::readNextGroup : ");
	    msg += "unbalanced parenthesis";
	    throw(runtime_error(msg));
	  }
	  --openedParenthesis;
	}
	res += p->value;
	++p;
      }
      if(openedParenthesis!=0){
	string msg("Analyser::readNextGroup : ");
	msg += "unclosed parenthesis";
	throw(runtime_error(msg));
      }
      return res;
    } // end of Analyser::readNextGroup

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
	  SmartPtr<Evaluator> ev(new Evaluator(vars,group,functions));
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

    const std::string
    Analyser::gatherTokenGroup(TokensContainer::const_iterator& p,
			       const TokensContainer::const_iterator pe)
    {
      using namespace std;
      string g;
      while(p!=pe){
	g += p->value;
	++p;
      }
      return g;
    } // end of Analyser::gatherTokenGroup

    bool
    Analyser::isValidIdentifier(const std::string& name)
    {
      using namespace tfel::math;
      if(!Evaluator::isValidIdentifier(name)){
	return false;
      }
      return CxxTokenizer::isValidIdentifier(name);
    } // end of Analyser::isValidIdentifier

    void
    Analyser::addFunction(const std::string& name,
			  tfel::utilities::SmartPtr<tfel::math::parser::ExternalFunction> pev,
			  const bool b1,
			  const bool b2)
    {
      using namespace std;
      if(!this->isValidIdentifier(name)){
	string msg("Analyser::addFunction : ");
	msg += "name '"+name+"' is not valid.";
	throw(runtime_error(msg));
      }
      if(this->locks.find(name)!=this->locks.end()){
	if((*(this->functions)).find(name)==(*(this->functions)).end()){
	  string msg("Analyser::addFunction : ");
	  msg += "internal error, a lock has been posed for a variable called '";
	  msg += name;
	  msg += "' that apparently don't exist";
	  throw(runtime_error(msg));
	}
	string msg("Analyser::addFunction : ");
	msg += "a variable '"+name+"' has already been declared, with const attribute";
	throw(runtime_error(msg));
      }
      if(b1){
	this->locks.insert(name);
      }
      if(b2){
	(*(this->functions))[name]=pev->resolveDependencies();
      } else {
	(*(this->functions))[name]=pev;
      }
    } // end of Analyser::addFunction

    void
    Analyser::analyseFunctionDefinition(TokensContainer::const_iterator& p,
					const TokensContainer::const_iterator pe,
					const bool b1,
					const bool b2)
    {
      using namespace std;
      using namespace tfel::utilities;
      using namespace tfel::math;
      this->checkNotEndOfLine("Analyser::analyseFunctionDefinition","",p,pe);
      string var = p->value;
      // variable or function definition
      if(!this->isValidIdentifier(var)){
	string msg("Analyser::analyseFunctionDefinition : ");
	msg += p->value+" is not a valid identifer.";
	throw(runtime_error(msg));
      }
      ++p;
      if(p==pe){
	string msg("Analyser::analyseFunctionDefinition : ");
	msg += "unexpected end of line";
	throw(runtime_error(msg));
      }
      if(p->value=="="){
	vector<string> vars;
	// adding a variable
	++p;
	if(p==pe){
	  string msg("Analyser::analyseFunctionDefinition : ");
	  msg += "unexpected end of line";
	  throw(runtime_error(msg));
	}
	string group = this->gatherTokenGroup(p,pe);
	if(group.empty()){
	  string msg("Analyser::analyseFunctionDefinition : ");
	  msg += "invalid declaraction of variable "+var;
	  throw(runtime_error(msg));
	}
	SmartPtr<tfel::math::parser::ExternalFunction> pev(new Evaluator(vars,group,functions));
	Evaluator* ev = static_cast<Evaluator *>(pev.getPtr());
	if(ev->getNumberOfVariables()!=0u){
	  string msg("Analyser::analyseFunctionDefinition : ");
	  msg += "error while declaring variable "+var;
	  if(ev->getNumberOfVariables()==1u){
	    msg += ", unknown variable "+ev->getVariablesNames()[0];
	  } else {
	    const std::vector<string>& evars = ev->getVariablesNames();		
	    std::vector<string>::const_iterator pv;
	    msg += ", unknown variables ";
	    for(pv=evars.begin();pv!=evars.end();){
	      msg+=*pv;
	      ++pv;
	      if(pv!=evars.end()){
		msg+=",";
	      }
	    }
	  }
	  throw(runtime_error(msg));
	}
	this->addFunction(var,pev,b1,b2);
      } else if (p->value=="("){
	SmartPtr<tfel::math::parser::ExternalFunction> ev;
	vector<string>::const_iterator p2;
	// adding a new function
	vector<string> vars = this->readVariableList(p,pe);
	if(vars.empty()){
	  string msg("Analyser::analyseFunctionDefinition : ");
	  msg += "no variable defined";
	  throw(runtime_error(msg));
	}
	if(p==pe){
	  string msg("Analyser::analyseFunctionDefinition : ");
	  msg += "unexpected end of line";
	  throw(runtime_error(msg));
	}
	if(p->value!="="){
	  string msg("Analyser::analyseFunctionDefinition : ");
	  msg += "unexpected token '"+p->value+"' (expected '=')";
	  throw(runtime_error(msg));
	}
	++p;
	if(p==pe){
	  string msg("Analyser::analyseFunctionDefinition : ");
	  msg += "unexpected end of line";
	  throw(runtime_error(msg));
	}
	string group = this->gatherTokenGroup(p,pe);
	if(group.empty()){
	  string msg("Analyser::analyseFunctionDefinition : ");
	  msg += "invalid declaraction of function "+var;
	  throw(runtime_error(msg));
	}
	ev = SmartPtr<tfel::math::parser::ExternalFunction> (new Evaluator(vars,group,functions));
	this->addFunction(var,ev,b1,b2);
      } else {
	string msg("Analyser::analyseFunctionDefinition : ");
	msg += "unexpected token ('"+p->value+"')";
	throw(runtime_error(msg));
      }
    } // end of Analyser::analyseFunctionDefinition

    void
    Analyser::treatConst(TokensContainer::const_iterator& p,
			 const TokensContainer::const_iterator pe)
    {
      this->analyseFunctionDefinition(p,pe,true,true);
    } // end of Analyser::treatConst

    void
    Analyser::treatLock(TokensContainer::const_iterator& p,
			const TokensContainer::const_iterator pe)
    {
      this->analyseFunctionDefinition(p,pe,true,false);
    } // end of Analyser::treatLock

    void
    Analyser::treatNoDeps(TokensContainer::const_iterator& p,
			  const TokensContainer::const_iterator pe)
    {
      this->analyseFunctionDefinition(p,pe,false,true);
    } // end of Analyser::treatNoDeps

    void
    Analyser::analyse(TokensContainer::const_iterator p,
		      const TokensContainer::const_iterator pe)
    {
      using namespace std;
      using namespace tfel::utilities;
      using namespace tfel::math;
      std::map<string,MemFuncPtr>::const_iterator pf;
      while(p!=pe){
	pf = this->callBacks.find(p->value);
	if(pf==this->callBacks.end()){
	  this->analyseFunctionDefinition(p,pe,false,false);
	} else {
	  ++p;
	  (this->*(pf->second))(p,pe);
	}
	if(p!=pe){
	  string msg("Analyser::analyse : ");
	  msg += "unexpected token '"+p->value+"'";
	  throw(runtime_error(msg));
	}
      }
    } // end of Analyse::analyse()

    void
    Analyser::analyseFile(const std::string& name)
    {
      using namespace std;
      ifstream file(name.c_str());
      if(!file){
	string msg("Analyser::analyseFile : ");
	msg += "can't open file '"+name+"'";
	throw(runtime_error(msg));
      }
      unsigned short nbr=1;
      while(!file.eof()){
	string line;
	getline(file,line);
	try{
	  this->analyseLine(line,nbr);
	}
	catch(runtime_error &e){
	  ostringstream msg;
	  msg << "Analyser::analyseFile : error at line "
	      << nbr << " of file " << name  << endl
	      << e.what();
	  throw(runtime_error(msg.str()));
	}
	++nbr;
      }
      if(!this->currentLine.empty()){
	string msg("Analyser::analyseFile : ");
	msg += "unfinished line at end of file";
	throw(runtime_error(msg));
      }
    } // end of Analyser::analyseFile
    
    std::vector<std::string>
    Analyser::tokenize(const std::string& s,
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
    } // end of Analyser::tokenize

    std::string
    Analyser::stripComments(const std::string& line)
    {
      using namespace std;
      const vector<string>& res = this->tokenize(line,'#');
      return res[0];
    } // end of Analyser::stripComments

    void
    Analyser::analyseLine(const std::string& line,
			  const unsigned short nbr)
    {
      using namespace std;
      this->currentLineNbr = nbr;
      string nline = this->stripComments(line);
      bool treatLine = false;
      if(nline.empty()){
	treatLine = true;
      } else {
	string::size_type pos = nline.find_last_not_of(' ');
	if(pos!=string::npos){
	  // line is not empty
	  if(nline[pos]=='\\'){
	    nline[pos] = ' ';
	    this->currentLine += nline;
	  } else {
	    this->currentLine += nline;
	    treatLine = true;
	  }
	}
      }
      if(treatLine){
	if(!this->currentLine.empty()){
	  if(this->currentLine[0]=='!'){
	    using namespace tfel::system;
	    ProcessManager m;
	    string cmd(++(this->currentLine.begin()),this->currentLine.end());
	    m.execute(cmd);
	    this->currentLine.clear();
	  } else {
	    this->clear();
	    this->splitLine(this->currentLine,this->currentLineNbr);
	    if(this->cStyleCommentOpened){
	      string msg("Analyser::analyseLine : ");
	      msg += "unfinished c-style comment";
	      throw(runtime_error(msg));
	    }
	    this->treatPreprocessorDirectives();
	    this->splitTokens();
	    CxxTokenizer::stripComments();
	    this->analyse(this->begin(),
			  this->end());
	    this->currentLine.clear();
	  }
	}
      }
    } // end of Analyser::analyseFile

    const std::string&
    Analyser::getCurrentLine(void) const
    {
      return this->currentLine;
    } // end of Analyser::getCurrentLine(void) const
      
    void
    Analyser::clearCurrentLine(void)
    {
      this->currentLine.clear();
    } // end of Analyser::clearCurrentLine(void)

    Analyser::~Analyser()
    {
      using namespace std;
      std::map<string,void*>::iterator p;
      for(p=this->libraries.begin();p!=this->libraries.end();++p){
	::dlclose(p->second);
      }
    } // end of Analyser::~Analyser()

  } // end of  namespace graphics

} // end of namespace tfel

int in[2];  //< pipe to the child standard output
int cfd[2]; //< pipe to the child
int ffd[2]; //< pipe to the father
int gargc;
char **gargv;
pid_t pid;

bool
tgplot_io_handler(Glib::IOCondition)
{
  using namespace std;
  using namespace Gtk;
  using namespace tfel::graphics;
  static Analyser a(gargc,gargv);
  //  unsigned short n;
  string line;
  cout.precision(15);
  //  cin.read,n);
  getline(cin,line);
  try{
    //    a.analyseLine(line,n);
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
    string t;
    while(ps!=pe){
      t += ps->value;
      if(++ps!=pe){
	t += ' ';
      }
    }
    res.push_back(t);
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
      this->splitLine(line,nb);
      if(this->cStyleCommentOpened){
	string msg("Analyser::analyseLine : ");
	msg += "unfinished c-style comment";
	throw(runtime_error(msg));
      }
      this->treatPreprocessorDirectives();
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
} // end of Analyser::tokenize

static void
tgplot_father_send(const char* const line,
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
  ::add_history(line);
#endif /* HAVE_READLINE_HISTORY */
  for(p=res.begin();p!=res.end();++p){
    if(*p=="pause"){
      getchar();
    } else {
      //  write(in[1],&n,sizeof(unsigned short));
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
  for(p2=inputFiles.begin();p2!=inputFiles.end();++p2,++n){
    ifstream file(p2->c_str());
    while(!file.eof()){
      string fline;
      getline(file,fline);
      if(!fline.empty()){
	tgplot_father_send(fline.c_str(),n);
      }
    }
  }
  // user inputs
  char *line;
  n=1;
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

