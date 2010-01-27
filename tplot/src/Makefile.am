## Makefile.am -- Process this file with automake to produce a Makefile.in file.

INCLUDES = -I$(top_srcdir)/include       \
	   -I$(top_srcdir)/tplot/include \
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

libTFELPlot_la_LIBADD = -L@top_builddir@/src/Exception/ \
			-L@top_builddir@/src/Math/      \
			-L@top_builddir@/src/Graphics   \
			-lTFELGraphics                  \
			-lTFELMathParser                \
			-lTFELMath -lTFELException      \
			$(GTKMM_LIBS)                   \
		 	@LTLIBINTL@

if HAVE_GSL
libTFELPlot_la_LIBADD += @GSLLIBS@
endif

if HAVE_OCTAVE
libTFELPlot_la_LIBADD += @OCTAVELIBS@
endif

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
		-lTFELMathParser	        \
		-lTFELMath		        \
		-lTFELUtilities                 \
		-lTFELException                 \
		$(GTKMM_LIBS)                   \
		@LTLIBINTL@                     \
		-ldl
tplot_DEPENDENCIES = libTFELPlot.la

if HAVE_READLINE
tplot_LDADD  += @READLINE_LIBS@
endif

if HAVE_OCTAVE
tplot_LDADD  += @OCTAVELIBS@
endif

bin_PROGRAMS  += tgplot
tgplot_SOURCES = Analyser.cxx
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
		 @LTLIBINTL@
tgplot_DEPENDENCIES = libTFELPlot.la

if HAVE_READLINE
tgplot_LDADD  += @READLINE_LIBS@
endif

if HAVE_OCTAVE
tgplot_LDADD  += @OCTAVELIBS@
endif

tgplot_LDADD  += -ldl


