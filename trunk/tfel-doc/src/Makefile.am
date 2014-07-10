INCLUDES=-I@top_srcdir@/tfel-doc/include \
	 -I@top_srcdir@/mfront/include   \
	 -I@top_srcdir@/include

bin_PROGRAMS =  tfel-doc

tfel_doc_SOURCES = tfel-doc.cxx          \
		   TestDocMain.cxx       \
		   Global.cxx            \
		   ParserBase.cxx        \
		   ConfigParser.cxx      \
		   TestDocumentation.cxx \
		   TestDocParser.cxx     \
		   MTestDocParser.cxx    \
		   LaTeXConvertion.cxx

tfel_doc_LDADD   = -L@top_builddir@/mfront/mtest/ \
		   -L@top_builddir@/src/System    \
		   -L@top_builddir@/src/Utilities \
		   -L@top_builddir@/src/Exception \
	           -L$(top_builddir)/src/Tests    \
		   -lTFELMTest                    \
		   -lTFELSystem                   \
		   -lTFELUtilities                \
		   -lTFELException                \
		   -lTFELTests

EXTRA_DIST = CMakeLists.txt