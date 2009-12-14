## Makefile.am -- Process this file with automake to produce a Makefile.in file.

INCLUDES = -I$(top_srcdir)/include       \
	   -I$(top_srcdir)/TPlot/include \
	    $(GTKMM_CFLAGS)

localedir = $(datadir)/locale
AM_CPPFLAGS = -DLOCALEDIR=\"$(localedir)\"

lib_LTLIBRARIES = libTFELPlot.la 
libTFELPlot_la_SOURCES = AddCurveDialog.cxx       \
			 CEAGraphTheme.cxx	 \
			 GNUGraphTheme.cxx	 \
			 ElegantGraphTheme.cxx	 \
			 ElegantGraphTheme2.cxx	 \
			 PlotPrintOperation.cxx	 \
			 PlotWindow.cxx		 \
			 ThemeSelectionDialog.cxx

bin_PROGRAMS  = tplot
tplot_SOURCES = plot.cxx 
tplot_LDFLAGS = -export-dynamic
tplot_LDADD   = -L.                             \
		-L$(top_builddir)/src/Graphics  \
		-L$(top_builddir)/src/System    \
		-L$(top_builddir)/src/Math      \
		-L$(top_builddir)/src/Utilities \
		-L$(top_builddir)/src/Exception \
		-lTFELPlot                      \
		-lTFELGraphics   	        \
		-lTFELSystem    	        \
		-lTFELMath		        \
		-lTFELUtilities                 \
		-lTFELException                 \
		$(GTKMM_LIBS)                   \
		@LTLIBINTL@                     \
		-ldl

if HAVE_READLINE
bin_PROGRAMS  += tgplot
tgplot_SOURCES = Analyser.cxx GetFunction.c
tgplot_LDFLAGS = -export-dynamic
tgplot_LDADD   = -L.                             \
		 -L$(top_builddir)/src/Graphics  \
		 -L$(top_builddir)/src/System    \
		 -L$(top_builddir)/src/Math      \
		 -L$(top_builddir)/src/Utilities \
		 -L$(top_builddir)/src/Exception \
		 -lTFELPlot                      \
		 -lTFELGraphics   	         \
		 -lTFELSystem    	         \
		 -lTFELMath		         \
		 -lTFELUtilities                 \
		 -lTFELException                 \
		 $(GTKMM_LIBS)                   \
		 @LTLIBINTL@                     \
		 @READLINE_LIBS@ -ldl
if HAVE_OCTAVE
tgplot_LDADD  += @OCTAVERLIBS@
endif

endif

