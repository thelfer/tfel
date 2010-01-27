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

#include"Utilities/SmartPtr.hxx"
#include"Utilities/CxxTokenizer.hxx"
#include"Utilities/TextData.hxx"
#include"Utilities/ArgumentParserBase.hxx"
#include"Math/Evaluator.hxx"

#include"Graphics/PlotWindow.hxx"

namespace tfel
{  

  namespace graphics
  {

    struct Analyser
      : public tfel::utilities::ArgumentParserBase<Analyser>,
	protected tfel::utilities::CxxTokenizer,
	protected PlotWindow
    {
      Analyser(int argc,
	       char **argv);
      const std::string&
      getCurrentLine(void) const;
      void
      clearCurrentLine(void);
      void
      analyseFile(const std::string&);
      void
      analyseLine(const std::string&,
		  const unsigned short);
      ~Analyser();
    protected:

      struct ImportOptions;
      struct CurveOptions;

      typedef void (Analyser::* MemFuncPtr)(TokensContainer::const_iterator&,
					    const TokensContainer::const_iterator);
      typedef void (Analyser::* MemFuncPtr2)(CurveOptions&,
					     TokensContainer::const_iterator&,
					     const TokensContainer::const_iterator);

      std::map<std::string,MemFuncPtr>  callBacks;
      std::map<std::string,MemFuncPtr>  setCallBacks;
      std::map<std::string,MemFuncPtr>  unsetCallBacks;
      std::map<std::string,MemFuncPtr>  showCallBacks;
      std::map<std::string,MemFuncPtr2> plotCallBacks;
      tfel::utilities::SmartPtr<tfel::math::parser::ExternalFunctionManager> functions;
      std::map<std::string,tfel::utilities::SmartPtr<tfel::utilities::TextData> > dataSources;
      std::set<std::string> locks;
      std::string terminal;
      std::string output;
      std::string dummyVariable;
      std::string currentLine;
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
      
      static bool
      isUnsignedInteger(const std::string&);

      static double
      readDouble(const std::string&,
		 const unsigned short);
      std::string
      getVersionDescription(void) const;
  
      std::string
      getUsageDescription(void) const;
      void
      reset(void);
      bool
      isValidIdentifier(const std::string&);
      std::string
      stripComments(const std::string&);
      std::vector<std::string>
      tokenize(const std::string&,
	       const char);
      std::string
      getData(std::vector<double>&,
	      const tfel::utilities::TextData&,
	      const std::string&);
      void
      analyse(TokensContainer::const_iterator,
	      const TokensContainer::const_iterator);
      void
      treatUnknownArgument(void);
      void setRanges(void);
      tfel::utilities::SmartPtr<tfel::math::Evaluator>
      readFunction(TokensContainer::const_iterator&, 
		   const TokensContainer::const_iterator);
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
      analyseFunctionDefinition(TokensContainer::const_iterator&,
				const TokensContainer::const_iterator,
				const bool,
				const bool);
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
      unsigned short
      convertToUnsignedShort(const std::string&);
      void
      addFunction(const std::string&,
		  tfel::utilities::SmartPtr<tfel::math::parser::ExternalFunction>,
		  const bool,
		  const bool);
      void
      importCFunction(const std::string&,
		      const std::string&,
		      const unsigned short,
		      const bool);
      void
      importCastemFunction(const std::string&,
			   const std::string&,
			   const unsigned short,
			   const bool);
#ifdef HAVE_OCTAVE
      void
      importOctaveFunction(TokensContainer::const_iterator&, 
			   const TokensContainer::const_iterator,
			   const std::string&,
			   const unsigned short nbr,
			   const bool);
#endif /* HAVE_OCTAVE*/
      void
      importFunction(TokensContainer::const_iterator&, 
		     const TokensContainer::const_iterator,
		     const ImportOptions,
		     const bool);
      std::string
      readNextGroup(TokensContainer::const_iterator&, 
		    const TokensContainer::const_iterator);
      std::string
      readUntil(TokensContainer::const_iterator&,
		const TokensContainer::const_iterator,
		const std::string&);
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
      treatQuit(TokensContainer::const_iterator&, 
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
      treatConst(TokensContainer::const_iterator&, 
		 const TokensContainer::const_iterator);
      void
      treatLock(TokensContainer::const_iterator&, 
		const TokensContainer::const_iterator);
      void
      treatNoDeps(TokensContainer::const_iterator&, 
		  const TokensContainer::const_iterator);
      void
      treatRePlot(TokensContainer::const_iterator&, 
		   const TokensContainer::const_iterator);
      void
      treatInclude(TokensContainer::const_iterator&, 
		   const TokensContainer::const_iterator);
      void
      treatImport(TokensContainer::const_iterator&, 
		  const TokensContainer::const_iterator);
      void
      treatImport(TokensContainer::const_iterator&, 
		  const TokensContainer::const_iterator,
		  const bool);
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
      treatSet(TokensContainer::const_iterator&, 
	       const TokensContainer::const_iterator);
      void
      treatUnset(TokensContainer::const_iterator&, 
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
      const std::string
      gatherTokenGroup(TokensContainer::const_iterator&, 
		       const TokensContainer::const_iterator);

      friend class
      tfel::utilities::ArgumentParserBase<Analyser>;

    }; // end of struct Analyser

  } // end of  namespace graphics

} // end of namespace tfel

#endif /* _LIB_TPLOT_ANALYSER_HXX */

