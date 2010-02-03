/*!
 * \file   Analyser.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   08 mar 2008
 */

#ifndef _LIB_TPLOT_ANALYSER_HXX_
#define _LIB_TPLOT_ANALYSER_HXX_ 

#include<set>
#include<map>
#include<string>

#include"TFEL/Utilities/SmartPtr.hxx"
#include"TFEL/Utilities/TextData.hxx"
#include"TFEL/Utilities/ArgumentParserBase.hxx"
#include"TFEL/Math/Interpreter/InterpreterBase.hxx"

#include"TFEL/Graphics/PlotWindow.hxx"

namespace tfel
{  

  namespace graphics
  {

    struct Analyser
      : public tfel::utilities::ArgumentParserBase<Analyser>,
	public tfel::math::interpreter::InterpreterBase<Analyser>,
	protected PlotWindow
    {
      Analyser(int argc,
	       char **argv);
      ~Analyser();
    protected:

      struct ImportOptions;
      struct CurveOptions;

      typedef void (Analyser::* MemFuncPtr2)(CurveOptions&,
					     TokensContainer::const_iterator&,
					     const TokensContainer::const_iterator);
      
      std::string
      getVersionDescription(void) const;
  
      std::string
      getUsageDescription(void) const;

      void
      reset(void);

      void
      treatLine();

      void
      treatSet(TokensContainer::const_iterator&,
	       const TokensContainer::const_iterator);
      
      void
      treatUnset(TokensContainer::const_iterator&,
		 const TokensContainer::const_iterator);

      void
      treatUnknownArgument(void);

      void setRanges(void);

      GraphCoordinates
      readCoordinates(TokensContainer::const_iterator&, 
		      const TokensContainer::const_iterator);

      void
      treatSetWidth(TokensContainer::const_iterator&, 
		    const TokensContainer::const_iterator);
      void
      treatSetHeight(TokensContainer::const_iterator&, 
		     const TokensContainer::const_iterator);
      void
      treatSetLogScale(TokensContainer::const_iterator&, 
		       const TokensContainer::const_iterator);
      void
      treatSetLabel(TokensContainer::const_iterator&, 
		    const TokensContainer::const_iterator);
      void
      treatSetTerminal(TokensContainer::const_iterator&, 
		       const TokensContainer::const_iterator);
      void
      treatSetOutput(TokensContainer::const_iterator&, 
		     const TokensContainer::const_iterator);
      void
      treatSetArrow(TokensContainer::const_iterator&, 
		    const TokensContainer::const_iterator);
      void
      treatSetGrid(TokensContainer::const_iterator&, 
		   const TokensContainer::const_iterator);
      CurveOptions
      treatPlotOptions(TokensContainer::const_iterator&, 
		       const TokensContainer::const_iterator);
      void
      applyCurveOptions(tfel::utilities::SmartPtr<Curve>,
			const CurveOptions&);

#ifdef HAVE_GSL
      void
      treatSetGSL(TokensContainer::const_iterator&, 
		  const TokensContainer::const_iterator);
#endif

      void
      registerCallBack(std::map<std::string,MemFuncPtr>&,
		       const std::string&,
		       const Analyser::MemFuncPtr);

      void
      registerCallBack2(std::map<std::string,MemFuncPtr2>&,
			const std::string&,
			const Analyser::MemFuncPtr2);

      void
      registerCallBacks(void);

      void
      readRange(bool&,bool&,
		double&,double&,
		TokensContainer::const_iterator&, 
		const TokensContainer::const_iterator);

      std::vector<std::string>
      readVariableList(TokensContainer::const_iterator&, 
		       const TokensContainer::const_iterator);
      void
      treatReset(TokensContainer::const_iterator&, 
		 const TokensContainer::const_iterator);

      void
      treatFunctionPlot(TokensContainer::const_iterator&, 
			const TokensContainer::const_iterator);

      void
      treatDataPlot(TokensContainer::const_iterator&, 
		    const TokensContainer::const_iterator);

      void
      treatPlot(TokensContainer::const_iterator&, 
		const TokensContainer::const_iterator);

      void
      treatRePlot(TokensContainer::const_iterator&, 
		   const TokensContainer::const_iterator);

      void
      treatInclude(TokensContainer::const_iterator&, 
		   const TokensContainer::const_iterator);

      void
      treatPlotTitle(CurveOptions&,
		     TokensContainer::const_iterator&, 
		     const TokensContainer::const_iterator);

      void
      treatPlotWith(CurveOptions&,
		    TokensContainer::const_iterator&, 
		    const TokensContainer::const_iterator);

      void
      treatPlotUsing(CurveOptions&,
		     TokensContainer::const_iterator&, 
		     const TokensContainer::const_iterator);

      void
      treatPlotAxes(CurveOptions&,
		    TokensContainer::const_iterator&, 
		    const TokensContainer::const_iterator);
    
      void
      treatPlotLineType(CurveOptions&n,
			TokensContainer::const_iterator&, 
			const TokensContainer::const_iterator);
      void
      treatPlotLineWidth(CurveOptions&n,
			 TokensContainer::const_iterator&, 
			 const TokensContainer::const_iterator);

      void
      treatUnsetLabel(TokensContainer::const_iterator&, 
		      const TokensContainer::const_iterator);

      void
      treatUnsetArrow(TokensContainer::const_iterator&, 
		      const TokensContainer::const_iterator);

      void
      treatUnsetWidth(TokensContainer::const_iterator&, 
		      const TokensContainer::const_iterator);

      void
      treatUnsetHeight(TokensContainer::const_iterator&, 
		       const TokensContainer::const_iterator);

      void
      treatSetTitle(TokensContainer::const_iterator&, 
		    const TokensContainer::const_iterator);

      void
      treatPrint(TokensContainer::const_iterator&, 
		 const TokensContainer::const_iterator);

      void
      treatTheme(TokensContainer::const_iterator&, 
		 const TokensContainer::const_iterator);

      void
      treatDummy(TokensContainer::const_iterator&, 
		 const TokensContainer::const_iterator);

      void
      treatUnsetLogScale(TokensContainer::const_iterator&, 
			 const TokensContainer::const_iterator);

      void
      treatUnsetXRange(TokensContainer::const_iterator&, 
		       const TokensContainer::const_iterator);

      void
      treatUnsetX2Range(TokensContainer::const_iterator&, 
			const TokensContainer::const_iterator);

      void
      treatUnsetYRange(TokensContainer::const_iterator&, 
		       const TokensContainer::const_iterator);

      void
      treatUnsetY2Range(TokensContainer::const_iterator&, 
			const TokensContainer::const_iterator);

      std::map<double,std::string>
      treatSetTics(TokensContainer::const_iterator&, 
		   const TokensContainer::const_iterator);

      void
      treatSetXTics(TokensContainer::const_iterator&, 
		    const TokensContainer::const_iterator);

      void
      treatSetX2Tics(TokensContainer::const_iterator&, 
		     const TokensContainer::const_iterator);

      void
      treatSetYTics(TokensContainer::const_iterator&, 
		    const TokensContainer::const_iterator);

      void
      treatSetY2Tics(TokensContainer::const_iterator&, 
		     const TokensContainer::const_iterator);

      void
      treatUnsetGrid(TokensContainer::const_iterator&, 
		     const TokensContainer::const_iterator);

      void
      treatUnsetXTics(TokensContainer::const_iterator&, 
		      const TokensContainer::const_iterator);

      void
      treatUnsetX2Tics(TokensContainer::const_iterator&, 
		       const TokensContainer::const_iterator);

      void
      treatUnsetYTics(TokensContainer::const_iterator&, 
		      const TokensContainer::const_iterator);

      void
      treatUnsetY2Tics(TokensContainer::const_iterator&, 
		       const TokensContainer::const_iterator);

      void
      treatUnsetXLabel(TokensContainer::const_iterator&, 
		       const TokensContainer::const_iterator);

      void
      treatUnsetX2Label(TokensContainer::const_iterator&, 
			const TokensContainer::const_iterator);

      void
      treatUnsetYLabel(TokensContainer::const_iterator&, 
		       const TokensContainer::const_iterator);

      void
      treatUnsetY2Label(TokensContainer::const_iterator&, 
			const TokensContainer::const_iterator);

      void
      treatXRange(TokensContainer::const_iterator&, 
		  const TokensContainer::const_iterator);

      void
      treatX2Range(TokensContainer::const_iterator&, 
		   const TokensContainer::const_iterator);

      void
      treatYRange(TokensContainer::const_iterator&, 
		  const TokensContainer::const_iterator);

      void
      treatY2Range(TokensContainer::const_iterator&, 
		   const TokensContainer::const_iterator);

      void
      treatXLabel(TokensContainer::const_iterator&, 
		  const TokensContainer::const_iterator);

      void
      treatX2Label(TokensContainer::const_iterator&, 
		   const TokensContainer::const_iterator);

      void
      treatYLabel(TokensContainer::const_iterator&, 
		  const TokensContainer::const_iterator);

      void
      treatY2Label(TokensContainer::const_iterator&, 
		   const TokensContainer::const_iterator);

      void
      declareDefaultColors();

      void
      declareColor(const std::string&,
		   const double,
		   const double,
		   const double);

      friend class
      tfel::utilities::ArgumentParserBase<Analyser>;

      friend class
      tfel::math::interpreter::InterpreterBase<Analyser>;


      std::map<std::string,MemFuncPtr>  showCallBacks;
      std::map<std::string,MemFuncPtr2> plotCallBacks;

      std::map<std::string,tfel::utilities::SmartPtr<tfel::utilities::TextData> > dataSources;

      std::map<std::string,Curve::Color> colors;

      std::string terminal;
      std::string output;
      std::string dummyVariable;
      std::string previousPlot;

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
      unsigned short currentLineNbr;

    }; // end of struct Analyser

  } // end of  namespace graphics

} // end of namespace tfel

#endif /* _LIB_TPLOT_ANALYSER_HXX */

