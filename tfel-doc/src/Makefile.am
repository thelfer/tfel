INCLUDES=-I@top_srcdir@/tfel-doc/include \
	 -I@top_srcdir@/include

bin_PROGRAMS =  tfel-doc

tfel_doc_SOURCES = tfel-doc.cxx          \
		   TestDocMain.cxx       \
		   Global.cxx            \
		   ParserBase.cxx        \
		   ConfigParser.cxx      \
		   TestDocumentation.cxx \
		   TestDocParser.cxx     \
		   LaTeXConvertion.cxx

tfel_doc_LDADD   = -L@top_builddir@/src/System    \
		   -L@top_builddir@/src/Utilities \
		   -L@top_builddir@/src/Exception \
		   -lTFELSystem                   \
		   -lTFELUtilities                \
		   -lTFELException
